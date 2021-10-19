//
//
//

#include "AlarmInfo.h"
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QScrollBar>
#include <QTimer>
#include <QDebug>
#include <service\event\ctkEventConstants.h>
#include <QMetaType>
#include "AlarmInfoItem.h"
#include "QAbstractScrollArea"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif


CAlarmInfo::CAlarmInfo(QWidget *parent) 
	: QWidget(parent)
	, m_pparent(parent)
	, m_maxItemCount(3)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);

	QFile mainwndStyleFile(":/qss/resrc/qss/evaluate_info.css");
	if (mainwndStyleFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(mainwndStyleFile.readAll());
	}
	ui.scrollArea->setStyleSheet("background-color: transparent;");
	ui.scrollArea->viewport()->setStyleSheet("background: transparent;");
	ui.scrollAreaWidgetContents->setStyleSheet("background: transparent;");
	ui.scrollArea->setFrameShape(QFrame::NoFrame);
	ui.scrollArea->verticalScrollBar()->hide();

	//connect(this, SIGNAL(createEventSignal(SAlarmInfo)), this, SLOT(createEventSlot(SAlarmInfo)));
// 	// 测试
// 	for (int i = 0; i < m_maxItemCount; ++i)
// 	{
// 		SAlarmInfo  sAlarmInfo;
// 		sAlarmInfo.level = static_cast<ELevel>(i % 2);
// 		sAlarmInfo.time = QTime::currentTime();
// 		sAlarmInfo.data = QDate::currentDate();
// 		if (0 == i)
// 		{
// 			sAlarmInfo.info = "我方战机A摧毁敌方103号战机";
// 		}
// 		else if (1 == i)
// 		{
// 			sAlarmInfo.info = "我方战机A被摧毁";
// 		}
// 		else if (2 == i)
// 		{
// 			sAlarmInfo.info = "我方战机B被摧毁";
// 		}
// 		createItem(sAlarmInfo);
// 	}
}

CAlarmInfo::~CAlarmInfo()
{
	
}

void CAlarmInfo::setInfoData(SAlarmInfo sAlarminfo)
{
	m_AlarmInfoList.push_back(sAlarminfo);
	while (m_AlarmInfoList.size() > m_maxItemCount)
	{
		m_AlarmInfoList.removeFirst();
	}
	for (int i = 0;i < m_AlarmInfoList.size();++i)
	{
		createItem(sAlarminfo);
		m_itemList.at(i)->show();
		m_itemList.at(i)->setLevel(m_AlarmInfoList.at(i).level);
		m_itemList.at(i)->setData(m_AlarmInfoList.at(i).data);
		m_itemList.at(i)->setTime(m_AlarmInfoList.at(i).time);
		m_itemList.at(i)->setInfo(m_AlarmInfoList.at(i).info);
	}
}

CAlarmInfoItem *CAlarmInfo::createItem(SAlarmInfo sAlarminfo)
{
	if (m_itemList.size()== 3)
	{
		return NULL;
	}
	CAlarmInfoItem *item = new CAlarmInfoItem(0);
	//item->resize(190,81);
	item->setLevel(sAlarminfo.level);
	item->setData(sAlarminfo.data);
	item->setTime(sAlarminfo.time);
	item->setInfo(sAlarminfo.info);
	ui.verticalLayout->addWidget(item);
	m_itemList.push_back(item);
	//removeExpiredItems();
	return item;
}

void CAlarmInfo::setMaxItemCount(size_t maxItemCount)
{
	m_maxItemCount = maxItemCount;
	removeExpiredItems();
}

void CAlarmInfo::Hide()
{
	hide();
}

void CAlarmInfo::Show()
{
	show();
}

void CAlarmInfo::removeExpiredItems()
{
	while (m_itemList.size() > m_maxItemCount)
	{
		auto item = m_itemList.first();
		m_itemList.removeFirst();
		delete item;
	}
	ui.verticalLayout->update();
}

void CAlarmInfo::createEventSlot(SAlarmInfo sInfo)
{
	createItem(sInfo);
}

void CAlarmInfo::handleEvent(const ctkEvent &event)
{
// 	if (event.getTopic() == "event/AlarmInfo/published")
// 	{
// 		SAlarmInfo sInfo = event.getProperty(SFxAlarmInfoEventConstants::AlarmInfo().c_str()).value<SAlarmInfo>();
// 		//界面必须在主线程中处理
// 		emit createEventSignal(sInfo);
// 	}
}
