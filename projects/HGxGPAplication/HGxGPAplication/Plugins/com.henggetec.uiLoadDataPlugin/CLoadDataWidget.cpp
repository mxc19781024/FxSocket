#include "CLoadDataWidget.h"
#include "ui_CLoadDataWidget.h"
#include <QStandardItemModel>
#include <QPixmapCache>
#include <QDomDocument>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QFile>
#include "QLinkedList"
#include "QPainter"
#include "qlistwidget.h"
#include "qlabel.h"
#include <io.h>
#include "FxMessageBox/CMessageBox.h"
#include "bdpDB/GxRegister.h"
#include "GxPluginFwLauncherInterface.h"
#include "GxEventHelperInterface.h"
#include "GxPluginFrameworkCore.h"
#include "IFxBizDataMgrService.h"
#include "GxServiceQuerier.h"


#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

CLoadDataWidget::CLoadDataWidget(ctkPluginContext *context, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CLoadDataWidget)
	, m_pContext(context)
{
	ui->setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);
	//ui->pSceneInfoListWidget->setAttribute(Qt::WA_TranslucentBackground);
	hide();
	ReadFileInfo();
	connect(ui->LoadSceneBtn, SIGNAL(clicked()), this, SLOT(SlotLoadScene()));
	connect(ui->pSceneInfoListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(SlotLoadScene()));
	connect(ui->CloseBtn,SIGNAL(clicked()),this,SLOT(SlotCloseWidget()));

	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/LoadFileFinished", HGxFWLauncherInstance.getPluginContext());

	ui->pSceneInfoListWidget->setStyleSheet("background-color: transparent;");
	ui->pSceneInfoListWidget->viewport()->setStyleSheet("background: transparent;");
	ui->pSceneInfoListWidget->verticalScrollBar()->setMaximumWidth(10);
	//ui->pSceneInfoListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{border:0px; width:40px;}"); 
	
	//ui->scrollAreaWidgetContents->setStyleSheet("background: transparent;");
}

CLoadDataWidget::~CLoadDataWidget()
{
    delete ui;
}

void CLoadDataWidget::InitListItem()
{
	if (m_vecItemInfo.size())
	{
		for (auto iter = m_vecItemInfo.begin(); iter != m_vecItemInfo.end();iter++)
		{
			QListWidgetItem* pitem = new QListWidgetItem();
			QSize size = pitem->sizeHint();

			pitem->setSizeHint(QSize(size.width(),100));
			ui->pSceneInfoListWidget->addItem(pitem);
			CLoadDataItemWidget* pItemWidget = new CLoadDataItemWidget(this);
			pItemWidget->setItemInfo(*iter);
			m_mapPath2Item[iter->strPath] = pitem;
			m_mapPath2Type[iter->strPath] = iter->strType;
			ui->pSceneInfoListWidget->setItemWidget(pitem,pItemWidget);
		}
	}
}

void CLoadDataWidget::SlotLoadScene()
{
	QListWidgetItem * pItem = ui->pSceneInfoListWidget->currentItem();
	std::string strPath;
	for (auto iter = m_mapPath2Item.begin(); iter != m_mapPath2Item.end();iter ++)
	{
		if (iter->second == pItem)
		{
			strPath = iter->first;
		}
	}

	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IFxBizDataMgrService> dataMgrService(HGxFWLauncherInstance.getPluginContext());
	//if (CMessageBox::question(tr("确认加载？")) == CMessageBox::Confirm)
	{
		auto itr = m_mapPath2Type.find(strPath);
		if (itr == m_mapPath2Type.end())
			return;
		if (itr->second == "Debris" || itr->second == "Satellite")
		{
			SignalSwitchMP(0);
		}
		else
		{
			SignalSwitchMP(1);
		}
		dataMgrService->LoadFile(strPath);
// 		//异步发送事件
// 		ctkDictionary event;
// 		event.insert("LoadFilePath", strPath.c_str());
// 		HGxEventHelperInstance.postEvent("event/LoadFileFinished", event);
		hide();
	}
}

void CLoadDataWidget::ReadFileInfo()
{
	struct _finddata_t fileinfo;
	std::string strXml;
	std::string strPath = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("UserConfig") + "\\*.xml";

	intptr_t handle;
	if ((handle = _findfirst(strPath.c_str(),&fileinfo)) == -1L)
	{
		return ;
	}
	else 
	{
		strXml = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("UserConfig") + "\\" + fileinfo.name;
		parseChart(strXml);
		while (!(_findnext(handle,&fileinfo)))
		{
			strXml = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("UserConfig") + "\\" + fileinfo.name;
			parseChart(strXml);
		}
		_findclose(handle);
	}
	InitListItem();
}

void CLoadDataWidget::parseChart( std::string strPath )
{
	QFile file(strPath.c_str());
	if (file.open(QIODevice::ReadOnly))
	{	
		QDomDocument doc;
		if (doc.setContent(&file))
		{
			QDomElement	root = doc.documentElement();
			for (QDomElement domEle=root.firstChildElement(); !domEle.isNull(); domEle=domEle.nextSiblingElement())
			{
				if (domEle.tagName() == "ChartList")
				{
					SItemInfo itemInfo;
					for (QDomElement ChartEle=domEle.firstChildElement(); !ChartEle.isNull(); ChartEle=ChartEle.nextSiblingElement())
					{
						if (ChartEle.tagName() == "Qimage")
						{
							itemInfo.strImage = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(ChartEle.text().toStdString());
						}
						else if (ChartEle.tagName() == "Name")
						{
							itemInfo.strName = ChartEle.text().toStdString();
						}
						else if (ChartEle.tagName() == "StartTime")
						{
							itemInfo.strStartTime = ChartEle.text().toStdString();
						} 
						else if (ChartEle.tagName() == "EndTime")
						{
							itemInfo.strEndTime = ChartEle.text().toStdString();
						} 
						else if (ChartEle.tagName() == "Describe")
						{
							itemInfo.strDescribe = ChartEle.text().toStdString();
						}
						else if (ChartEle.tagName() == "Type")
						{
							itemInfo.strType = ChartEle.text().toStdString();
						}
					}
					itemInfo.strPath = strPath;
					m_vecItemInfo.push_back(itemInfo);
				}
			}
		}
		file.close();
	}		
}

void CLoadDataWidget::handleEvent(const ctkEvent& event)
{
	if (event.getTopic() == "event/LoadFileFinished")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		//emit setSyncBtnSignal();
	}
}

void CLoadDataWidget::SlotCloseWidget()
{
	close();
}

