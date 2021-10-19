#include "CEquipmentInfoWidget.h"
#include "ui_CEquipmentInfoWidget.h"
#include <QStandardItemModel>
#include <QPixmapCache>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QFile>
#include <QTimer>
#include "QLinkedList"
#include "EquipmentAttibutes.h"
#include "EquipmentSetting.h"
#include <service\event\ctkEventConstants.h>
#include <QMetaType>
#include "QPainter"
#include "GxPluginFrameworkCore.h"
#include "public\IDriveMgrInterface.h"
#include "GxPluginFwLauncherInterface.h"
#include "GxServiceQuerier.h"
#include "ProgressBall.h"
#include "GxEventHelperInterface.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif
//资源路径
//const QString c_strResrcPath  = ":/resrc";

//样式表路径
#define EQUIPMENTINFO_STYLE_PATH ":/qss/resrc/qss/equipment_info"

//调整节点的定时器触发间隔

CEquipmentInfoWidget::CEquipmentInfoWidget(ctkPluginContext *context, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CEquipmentInfoWidget)
{
    ui->setupUi(this);
	//连接数据库
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);

	ctkDictionary dic;
	dic.insert(ctkEventConstants::EVENT_TOPIC, SFxEquipmentInfoEventConstants::Topic().c_str());
	context->registerService<ctkEventHandler>(this, dic);

	ctkDictionary dic2;
	dic2.insert(ctkEventConstants::EVENT_TOPIC, SFxRemoveUintEventConstants::Topic().c_str());
	context->registerService<ctkEventHandler>(this, dic2);
	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/LoginFinished", HGxFWLauncherInstance.getPluginContext());
	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/clearScene", HGxFWLauncherInstance.getPluginContext());
     //样式表，需要注意路径
// 	QFile mainwndStyleFile(EQUIPMENTINFO_STYLE_PATH);
// 	if (mainwndStyleFile.open(QIODevice::ReadOnly | QIODevice::Text))
// 	{
// 		setStyleSheet(mainwndStyleFile.readAll());
// 	}
	InitFrameChart1();
	connect(this, SIGNAL(addSEquipmentInfoSignal(SEquipmentInfo)), this, SLOT(addSEquipmentInfoSlot(SEquipmentInfo)));
	connect(this, SIGNAL(RemoveUintSignal(SRemoveUintInfo)), this, SLOT(RemoveUintSlot(SRemoveUintInfo)));
	connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(switchEquipmentSlot(int)));
	connect(this, SIGNAL(EquipmentInfoShowSignal()), this, SLOT(EquipmentInfoShowSlot()));
	connect(this, SIGNAL(EquipmentInfoClearSignal()), this, SLOT(EquipmentInfoClearSlot()));
	QTimer* pTimer = new QTimer();
	connect(pTimer, SIGNAL(timeout()), this, SLOT(TimerUpdateSlot()));
	pTimer->start(1000);
}

CEquipmentInfoWidget::~CEquipmentInfoWidget()
{
    delete ui;
}

void CEquipmentInfoWidget::handleEvent(const ctkEvent& event)
{
	if (event.getTopic() == "event/LoginFinished")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit EquipmentInfoShowSignal();
	}
	if (event.getTopic() == "event/clearScene")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit EquipmentInfoClearSignal();
	}
	else if (event.getTopic() == "freex/RemoveUint")
	{
		SRemoveUintInfo sInfo = event.getProperty(SFxRemoveUintEventConstants::RemoveUintInfo().c_str()).value<SRemoveUintInfo>();
		//界面必须在主线程中处理
		emit RemoveUintSignal(sInfo);
	}
	else
	{
		SEquipmentInfo sInfo = event.getProperty(SFxEquipmentInfoEventConstants::EquipmentInfo().c_str()).value<SEquipmentInfo>();
		//储存log
		m_mapSEquipmentInfo[sInfo.name] = sInfo;
		//界面必须在主线程中处理
		emit addSEquipmentInfoSignal(sInfo);
	}
}

void CEquipmentInfoWidget::InitFrameChart1()
{
	// 设置反锯齿
	QVector<RadarNode> vecData;
	RadarNode node;

	node.dScale = 0.91;
	node.qName = "打击";
	node.fontColor = QColor(110, 110, 110);
	vecData.push_back(node);

	node.dScale = 0.36;
	node.qName = "巡航";
	node.fontColor = QColor(110, 110, 110);
	vecData.push_back(node);

	node.dScale = 0.83;
	node.qName = "性能";
	node.fontColor = QColor(110, 110, 110);
	vecData.push_back(node);

	node.dScale = 0.51;
	node.qName = "续航";
	node.fontColor = QColor(110, 110, 110);
	vecData.push_back(node);

	node.dScale = 0.63;
	node.qName = "协调";
	node.fontColor = QColor(110, 110, 110);
	vecData.push_back(node);

	node.dScale = 0.50;
	node.qName = "探测";
	node.fontColor = QColor(110, 110, 110);
	vecData.push_back(node);

	ui->pRadarChart->SetData(vecData);
	ui->pRadarChart->SetTextBorderOpen(false);
	QColor backStartColor("#005298");
	QColor backEndColor("#00ffd8");
	backStartColor = QColor(255, 122, 50);
	backEndColor = QColor(255, 122, 50);
	backStartColor.setAlpha(0.7 * 255);
	backEndColor.setAlpha(0.7 * 255);
	ui->pRadarChart->SetBackGradient(backStartColor, backEndColor);
	ui->pRadarChart->SetLineGradient(QColor(255, 122, 50), QColor(255, 122, 50));
	ui->pRadarChart->SetRingBackColor(QColor(255, 255, 255, 0), QColor(255, 255, 255, 0), QColor(255, 255, 255, 0));
	ui->pRadarChart->setMinimumSize(QSize(202, 202));

}

void CEquipmentInfoWidget::addSEquipmentInfoSlot( SEquipmentInfo sSEquipmentInfo)
{
	  ui->comboBox->addItem((sSEquipmentInfo.name).c_str());
}

void CEquipmentInfoWidget::RemoveUintSlot(SRemoveUintInfo sInfo)
{
	int nSize = ui->comboBox->count();
	for (int n = 0;n <= nSize;++n)
	{
		QString strText = ui->comboBox->itemText(n);
		if (m_mapSEquipmentInfo.contains(strText.toStdString()))
		{
			if (m_mapSEquipmentInfo[strText.toStdString()].ID == sInfo.ID)
			{
				ui->comboBox->removeItem(n);
				m_mapSEquipmentInfo.remove(strText.toStdString());
				break;
			}

		}
	}

}

void CEquipmentInfoWidget::TimerUpdateSlot()
{

}

void CEquipmentInfoWidget::EquipmentInfoShowSlot()
{
	this->show();
}

void CEquipmentInfoWidget::EquipmentInfoClearSlot()
{
	ui->comboBox->clear();
	m_mapSEquipmentInfo.clear();
	ui->comboBox->removeItem(0);
}

void CEquipmentInfoWidget::switchEquipmentSlot(int index)
{
	//无选中状态不作处理 l180
	if(index < 0)
	{
		return;
	}
	// 获取当前选中的文本
	QString strText = ui->comboBox->itemText(index);
	std::string strID = strText.toStdString();

	if (m_mapSEquipmentInfo.contains(strID))
	{
		strID = m_mapSEquipmentInfo[strID].ID;
		SEquipmentInfo sEquipmentInfo = m_mapSEquipmentInfo[strID];
		ui->EquipmentAtttibutes->setMaxSpeed(sEquipmentInfo.maxSpeed);
		ui->EquipmentAtttibutes->setMaxOil(sEquipmentInfo.maxOil);
		ui->EquipmentAtttibutes->setAmmunitionNumber(sEquipmentInfo.ammunitionNumber);
		ui->pRadarChart->SetData(sEquipmentInfo.sSEquipCap);
// 		ui->Equiomentsetting->setEquipmentPowerData(sEquipmentInfo.);
// 		ui->Equiomentsetting->setEquipmentTemperatureData();
// 		ui->Equiomentsetting->setEquipmentPressData();
	}

	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (driveMgrInterface)
	{
		
		driveMgrInterface->thirdTrackUnit(strID);
	}

}