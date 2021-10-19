#include <FreeXFramework/FreeXLog.h>
#include <QtPlugin>
#include "CGanateChartUiPluginActivator.h"
#include "CMainWindowManager.h"
#include "ICore.h"
#include "CToolManager.h"
#include "CMenuContainer.h"
#include "CMenuManager.h"
#include "CFxAppGlobal.h"
#include <FreeXFramework/IFxSimulationTimer.h>


#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

CGanateChartUiPluginActivator::CGanateChartUiPluginActivator()
	: m_pGanateChartWgt(NULL)
{

}

CGanateChartUiPluginActivator::~CGanateChartUiPluginActivator()
{

}

void CGanateChartUiPluginActivator::start(ctkPluginContext *context)
{
	FREEX_LOG_DEBUG << "[UI]甘特图信息界面插件启动";

	// 在主程序创建菜单
	if (m_pGanateChartWgt == nullptr)
	{
		m_pGanateChartWgt = new CFxGanttChart(context,ICore::mainWindowManager()->mainWindow());
		m_pGanateChartWgt->setFixedSize(750,150);

		//m_pGanateChartWgt->setGraduationParameter(30,10,150,15,130,40,QColor(105, 106, 105));
		//double dCurTime = FxSimulationTimerInstance()->getSimulateTimeCurrent();
		//int nCurTime= dCurTime;
		//double dCurrentTime;
		//if (dCurTime - nCurTime >= 0.5)
		//{
		//	dCurrentTime = int(dCurTime) + 0.5;
		//}
		//else
		//{
		//	dCurrentTime = int(dCurTime) - 0.5;
		//}
		//m_pGanateChartWgt->setBeginTime(dCurrentTime);
		//
		//m_pGanateChartWgt->setTotalTime(1,0,0,0);
		//int i = 0;
		//while (i < 8)
		//{
		//	SObjectInfo stObjectInfo;
		//	stObjectInfo.m_strID = QString::number(i);
		//	stObjectInfo.m_strName = QString::number(i) + "区行动";
		//	//stObjectInfo.m_strID = QString::number(i).rightJustified(4, QLatin1Char('0'), true);
		//	//stObjectInfo.m_strName = QString("Object").append(stObjectInfo.m_strID);
		//	double dTime = 0;
		//	int j = 0;
		//	while (dTime < 1.0)
		//	{
		//		dTime += (rand() % 7201 + 600) / 86400.0;
		//		double dTimeAdd = (rand() % 7201 + 600) / 86400.0;
		//		SEventInfo stEventInfo(QString::number(j), QString::number(rand() % 7), dCurrentTime + dTime, dCurrentTime + dTime + dTimeAdd, QColor(rand() % 256, rand() % 256, rand() % 256));
		//		stEventInfo.m_colorEvent = QColor(255, 128, 0);
		//		stObjectInfo.m_mapIDToEventInfo.insert(stEventInfo.m_strID, stEventInfo);
		//		dTime += dTimeAdd;
		//		dTime += (rand() % 7201 + 600) / 86400.0;
		//		++j;
		//	}
		//	m_pGanateChartWgt->addObjectInfo(stObjectInfo);
		//	m_pGanateChartWgt->setGanttHeight(4);
		//	m_pGanateChartWgt->setGanttInterval(20);
		//	m_pGanateChartWgt->setPointerColor(QColor(255, 0, 0));
		//	m_pGanateChartWgt->setGrid(false);
		//	++i;
		//}
		
		m_pGanateChartWgt->show();

		XGisApp::g_sizeDefaultScreen.width();
		XGisApp::g_sizeDefaultScreen.height();
		m_pGanateChartWgt->move((XGisApp::g_sizeDefaultScreen.width()-m_pGanateChartWgt->width())-35,911);

		// 在主程序创建菜单
		//调整日志工具位置，在主窗口显示（2019-9-21 w00224）
		CMenuManager *pMenuMgr = ICore::menuManager();
		if (pMenuMgr == nullptr)
		{
			FREEX_LOG_DEBUG << "[UI]装备信息界面插件加载失败";
			return;
		}
	}
}

void CGanateChartUiPluginActivator::stop(ctkPluginContext *)
{
	if (m_pGanateChartWgt != m_pGanateChartWgt)
	{
		m_pGanateChartWgt->deleteLater();
		m_pGanateChartWgt = nullptr;
	}
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
Q_EXPORT_PLUGIN2(CGanate_Chart_ui, CGanateChartUiPluginActivator)
#endif
