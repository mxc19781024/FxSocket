#include <FreeXFramework/FreeXLog.h>
#include <QtPlugin>
#include <QPushButton>
#include "CEvaluateInfoUiPluginActivator.h"
#include "CMainWindowManager.h"
#include "ICore.h"
#include "CToolManager.h"
#include "CMenuContainer.h"
#include "CMenuManager.h"
#include "CFxAppGlobal.h"


#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

CEvaluateInfoUiPluginActivator::CEvaluateInfoUiPluginActivator()
	: m_pEvaluateInfoWgt(NULL)
{

}

CEvaluateInfoUiPluginActivator::~CEvaluateInfoUiPluginActivator()
{

}

void CEvaluateInfoUiPluginActivator::start(ctkPluginContext *context)
{
	FREEX_LOG_DEBUG << "[UI]装备信息界面插件启动";

	// 在主程序创建菜单
	if (m_pEvaluateInfoWgt == nullptr)
	{
		m_pEvaluateInfoWgt = new CEvaluateInfoWidget(context,ICore::mainWindowManager()->mainWindow());
		m_pEvaluateInfoWgt->show();

		XGisApp::g_sizeDefaultScreen.width();
		XGisApp::g_sizeDefaultScreen.height();
		m_pEvaluateInfoWgt->move((XGisApp::g_sizeDefaultScreen.width()-m_pEvaluateInfoWgt->width())-20,150);

		// 在主程序创建菜单
		//调整日志工具位置，在主窗口显示（2019-9-21 w00224）
		CMenuManager *pMenuMgr = ICore::menuManager();
		if (pMenuMgr == nullptr)
		{
			FREEX_LOG_DEBUG << "[UI]装备信息界面插件加载失败";
			return;
		}

		// 		QString strId = "ChartsVisualization";
		// 		CMenuContainer *pMenuContainer = ICore::menuManager()->widget(strId);
		// 		if (pMenuContainer == NULL)
		// 		{
		// 			CChartsVisualizationMenuAction *pChartsVisualizationAction = new CChartsVisualizationMenuAction();
		// 			pChartsVisualizationAction->SetActionFlag("图表示例");
		// 			pChartsVisualizationAction->showMaximized();
		// 			QImage *pAlarmInfoImage = new QImage(":/img/resrc/images/Themetic.png");
		// 			pChartsVisualizationAction->SetActionUnitImage(pAlarmInfoImage);
		// 			connect(pChartsVisualizationAction, SIGNAL(MenuActionActiveClickedSignal(bool)), this, SLOT(onAccusationLinkBtnClickedSlot(bool)));
		// 			pMenuContainer = ICore::menuManager()->addMenuWidget(strId, pChartsVisualizationAction, 0);
		// 		}
		// 
		// 		if (pMenuContainer == NULL)
		// 		{
		// 			return;
		// 		}
	}

}

void CEvaluateInfoUiPluginActivator::onAccusationLinkBtnClickedSlot(bool bShow)
{
	if (m_pEvaluateInfoWgt)
	{
		if (m_pEvaluateInfoWgt->isHidden())
		{
			m_pEvaluateInfoWgt->show();
		}
		else
		{
			m_pEvaluateInfoWgt->hide();
		}
		//m_pAlarmInfoWgt->setFixedSize(800, 400);	
	}
}

void CEvaluateInfoUiPluginActivator::stop(ctkPluginContext *)
{
	if (m_pEvaluateInfoWgt != m_pEvaluateInfoWgt)
	{
		m_pEvaluateInfoWgt->deleteLater();
		m_pEvaluateInfoWgt = nullptr;
	}
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
Q_EXPORT_PLUGIN2(CEvaluate_Info_ui, CEvaluateInfoUiPluginActivator)
#endif
