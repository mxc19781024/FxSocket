#include "CEquipEnergyInfoWidget.h"
#include "ui_CEquipEnergyInfoWidget.h"
#include <QStandardItemModel>
#include <QPixmapCache>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QFile>
#include "QLinkedList"
#include "QPainter"
#include "qlistwidget.h"
#include "GxPluginFwLauncherInterface.h"
#include "GxPluginFrameworkCore.h"
#include "GxEventHelperInterface.h"
#include "service\event\ctkEventConstants.h"
#include "QTimer"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif
//资源路径
//const QString c_strResrcPath  = ":/resrc";


//调整节点的定时器触发间隔

CEquipEnergyInfoWidget::CEquipEnergyInfoWidget(ctkPluginContext *context, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CEquipEnergyInfoWidget)
	, m_nMaxItemCount(2)
{
    ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);
	this->setFixedSize(753,153);
	ctkDictionary dic;
	dic.insert(ctkEventConstants::EVENT_TOPIC, SFxEquipInfoEventConstants::Topic().c_str());
	context->registerService<ctkEventHandler>(this, dic);
	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/LoginFinished", HGxFWLauncherInstance.getPluginContext());
	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/clearScene", HGxFWLauncherInstance.getPluginContext());
	ui->background_frame->setStyleSheet("QFrame#background_frame{border-image: url(:/img/resrc/images/beijing.png);}");
//      //样式表，需要注意路径
// 	QFile mainwndStyleFile(":/qss/resrc/qss/equip_energy_info.css");
// 	if (mainwndStyleFile.open(QIODevice::ReadOnly | QIODevice::Text))
// 	{
// 		QString str = styleSheet() + mainwndStyleFile.readAll();
// 		setStyleSheet(str);
//  	}

	ui->scrollArea->setStyleSheet("background-color: transparent;");
	ui->scrollArea->viewport()->setStyleSheet("background: transparent;");
	ui->scrollAreaWidgetContents->setStyleSheet("background: transparent;");
	ui->scrollArea->setFrameShape(QFrame::NoFrame);
	test();

	connect(this, SIGNAL(EquipEnergyInfoShowSignal()), this, SLOT(EquipEnergyInfoShowSlot()));
	connect(this, SIGNAL(EquipEnergyInfoClearSignal()), this, SLOT(EquipEnergyInfoClearSlot()));
	connect(this, SIGNAL(addSEquipInfoSignal(SEquipInfo)), this, SLOT(addSEquipInfoSlot(SEquipInfo)));

	QTimer* pTimer = new QTimer();
	connect(pTimer, SIGNAL(timeout()), this, SLOT(TimerUpdateSlot()));
	pTimer->start(1000);
}

CEquipEnergyInfoWidget::~CEquipEnergyInfoWidget()
{
    delete ui;
}

void CEquipEnergyInfoWidget::handleEvent(const ctkEvent& event)
{
	if (event.getTopic() == "event/LoginFinished")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit EquipEnergyInfoShowSignal();
	}
	else if (event.getTopic() == "event/EquipInfo/published")
	{
		SEquipInfo sInfo = event.getProperty(SFxEquipInfoEventConstants::EquipInfo().c_str()).value<SEquipInfo>();
		//储存log
		//m_mapSEquipInfo[sInfo.name] = sInfo;
		//界面必须在主线程中处理
		emit addSEquipInfoSignal(sInfo);
	}
	if (event.getTopic() == "event/clearScene")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit EquipEnergyInfoClearSignal();
	}

}

void CEquipEnergyInfoWidget::paintEvent( QPaintEvent* p )
{
	QPainter painter(this);
	QStyleOption opt;
	opt.init(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	QWidget::paintEvent(p);
}

void CEquipEnergyInfoWidget::test()
{
	for (int i = 0; i < m_nMaxItemCount ; ++i)
	{
		CEquipEnergyItemWidget* pItem = new CEquipEnergyItemWidget(this);
		pItem->hide();
		ui->verticalLayout->insertWidget(0, pItem);
		m_itemList.push_back(pItem);
// 		SEquipInfo sEquipInfo;
// 		if (0 == i)
// 		{
// 			sEquipInfo.strIndex = "02";
// 			sEquipInfo.strType = "航天器";
// 			sEquipInfo.strID = "发现号 OV-103";
// 			sEquipInfo.strIconPath = "texture/OV-103.png";
// 			sEquipInfo.dEnergyNum = 0.5;
// 			sEquipInfo.dLoadNum = 0.33;
// 			sEquipInfo.dParaA = 268;
// 			sEquipInfo.dParaB = 97;
// 			sEquipInfo.dParaC = 255;
// 			
// 		}
// 		else if ( 1 == i)
// 		{
// 			sEquipInfo.strIndex = "01";
// 			sEquipInfo.strType = "航空器";
// 			sEquipInfo.strID = "LRS-B B21";
// 			sEquipInfo.strIconPath = "texture/B-2.png";
// 			sEquipInfo.dEnergyNum = 0.85;
// 			sEquipInfo.dLoadNum = 0.15;
// 			sEquipInfo.dParaA = 56;
// 			sEquipInfo.dParaB = 69;
// 			sEquipInfo.dParaC = 12;
// 			
// 		}
// 		pItem->setEquipEnergyInfo(sEquipInfo);
// 		ui->verticalLayout->insertWidget(0,pItem);
// 		m_itemList.push_back(pItem);
	}
}

void CEquipEnergyInfoWidget::EquipEnergyInfoShowSlot()
{
	this->show();
}

void CEquipEnergyInfoWidget::TimerUpdateSlot()
{
	if (m_equipInfoList.size() == 0)
	{
		return;
	}
	for (int i = 0; i < m_itemList.size(); ++i)
	{
		m_itemList.at(i)->show();
		SEquipInfo sEquipInfo = m_equipInfoList.at(rand() % m_equipInfoList.size());
		if (sEquipInfo.strID == "0")
		{
			sEquipInfo = m_equipInfoList.at(rand() % m_equipInfoList.size());
		}
		sEquipInfo.strIndex = "0" + QString::number(i + 1).toStdString();
		m_itemList.at(i)->setEquipEnergyInfo(sEquipInfo);
	}
}

void CEquipEnergyInfoWidget::addSEquipInfoSlot(SEquipInfo sEquipInfo)
{
	if (sEquipInfo.strID != "0")
	{
		m_equipInfoList.push_back(sEquipInfo);
	}
}

void CEquipEnergyInfoWidget::EquipEnergyInfoClearSlot()
{
	m_equipInfoList.clear();
}

void CEquipEnergyInfoWidget::removeExpiredItems()
{
	while (m_itemList.size() > m_nMaxItemCount)
	{
		auto item = m_itemList.first();
		m_itemList.removeFirst();
		delete item;
	}
}

void CEquipEnergyInfoWidget::setMaxItemCount( int nCount )
{
	m_nMaxItemCount = nCount;
	removeExpiredItems();
}

//void CEquipEnergyInfoWidget::InitFrameChart1()
//{
//	QListWidgetItem* pitem = new QListWidgetItem();
//	CEquipEnergyAttibutes* pAttibutes = new CEquipEnergyAttibutes();
//	QSize size = pitem->sizeHint();
//
//	pitem->setSizeHint(QSize(size.width(),100));
//	//pitem->setText(QString::fromStdString(iter->strName));
//
//	//ui->listWidget->addItem(pAttibutes);
//
//	m_customBar=new ProgressBar(20);
//	QHBoxLayout* layout=new QHBoxLayout(ui->widget);
//	layout->setContentsMargins(0,4,0,0);
//	m_customBar->setFixedWidth(170);
//	m_customBar->setFixedHeight(22);
//	m_customBar->setProgressValue(0.5);
//	layout->addWidget(m_customBar);
//
//	m_customBar2=new ProgressBar(20);
//	QHBoxLayout* layout_2=new QHBoxLayout(ui->widget_2);
//	layout_2->setContentsMargins(0,4,0,0);
//	m_customBar2->setFixedWidth(170);
//	m_customBar2->setFixedHeight(22);
//	m_customBar2->setProgressValue(0.33);
//	layout_2->addWidget(m_customBar2);
//
//	m_customBar3=new ProgressBar(20);
//	QHBoxLayout* layout_3=new QHBoxLayout(ui->widget_3);
//	layout_3->setContentsMargins(0,4,0,0);
//	m_customBar3->setFixedWidth(170);
//	m_customBar3->setFixedHeight(22);
//	m_customBar3->setProgressValue(0.911);
//	layout_3->addWidget(m_customBar3);
//
//	m_customBar4=new ProgressBar(20);
//	QHBoxLayout* layout_4=new QHBoxLayout(ui->widget_4);
//	layout_4->setContentsMargins(0,4,0,0);
//	m_customBar4->setFixedWidth(170);
//	m_customBar4->setFixedHeight(22);
//	m_customBar4->setProgressValue(0.568);
//	layout_4->addWidget(m_customBar4);
//}

//ProgressBar::ProgressBar( int count, QWidget* parent)
//	: QWidget(parent),
//	  m_allCount(count)
//{
//	init();
//	m_progressValue->setStyleSheet("color:rgb(255,122,50) ");
//}
//void ProgressBar:: setProgressValue(double value)
//{
//	
//	for ( int  i = 0 ; i < m_allCount ; i++)
//	{
//		if( i  < value*m_allCount )
//		{
//			m_listProgress[i]->setStyleSheet("background:rgb(255,122,50) ; max-height:2px; max-width:5px");
//		}
//		else
//		{
//			m_listProgress[i]->setStyleSheet("background:rgb(110,110,110) ; max-height:2px; max-width:5px");
//		}
//	}
//
//	m_progressValue->setText(QString::number(value*100,'f',0)+"%");
//}
//
//void ProgressBar::init()
//{
//	QWidget* wgt=new QWidget(this);
//	QHBoxLayout* layout=new QHBoxLayout(wgt);
//	QHBoxLayout* layout_2=new QHBoxLayout(wgt);
//	layout->setContentsMargins(0,0,0,0);
//	layout_2->setSpacing(2);
//	layout_2->setContentsMargins(0,0,0,0);
//
//	for(int i=0 ; i < m_allCount ; i++)
//	{
//		QLabel*  lab = new QLabel;
//		lab->setStyleSheet("background:rgb(110,110,110) ; max-height:2px; max-width:5px");
//		layout_2->addWidget(lab);
//		m_listProgress.push_back(lab);
//	}
//	
//	m_progressValue=new QLabel;
//
//	layout->addLayout(layout_2);
//	layout->addWidget(m_progressValue);
//}
