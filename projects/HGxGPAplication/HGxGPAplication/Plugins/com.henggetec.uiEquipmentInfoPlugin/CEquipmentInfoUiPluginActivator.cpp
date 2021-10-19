#include <FreeXFramework/FreeXLog.h>
#include <QtPlugin>
#include <QPushButton>
#include "CEquipmentInfoUiPluginActivator.h"
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

CEquipmentInfoUiPluginActivator::CEquipmentInfoUiPluginActivator()
	: m_pEquipmentInfoWgt(NULL)
{

}

CEquipmentInfoUiPluginActivator::~CEquipmentInfoUiPluginActivator()
{

}

void CEquipmentInfoUiPluginActivator::start(ctkPluginContext *context)
{
	FREEX_LOG_DEBUG << "[UI]装备信息界面插件启动";

	// 在主程序创建菜单
	if (m_pEquipmentInfoWgt == nullptr)
	{
		m_pEquipmentInfoWgt = new CEquipmentInfoWidget(context,ICore::mainWindowManager()->mainWindow());
		m_pEquipmentInfoWgt->show();

		XGisApp::g_sizeDefaultScreen.width();
		XGisApp::g_sizeDefaultScreen.height();
		m_pEquipmentInfoWgt->move(20,150);

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

void CEquipmentInfoUiPluginActivator::onAccusationLinkBtnClickedSlot(bool bShow)
{
	if (m_pEquipmentInfoWgt)
	{
		if (m_pEquipmentInfoWgt->isHidden())
		{
			m_pEquipmentInfoWgt->show();
		}
		else
		{
			m_pEquipmentInfoWgt->hide();
		}
		//m_pAlarmInfoWgt->setFixedSize(800, 400);	
	}
}

void CEquipmentInfoUiPluginActivator::stop(ctkPluginContext *)
{
	if (m_pEquipmentInfoWgt != m_pEquipmentInfoWgt)
	{
		m_pEquipmentInfoWgt->deleteLater();
		m_pEquipmentInfoWgt = nullptr;
	}
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
Q_EXPORT_PLUGIN2(Equipment_Info_ui, CEquipmentInfoUiPluginActivator)
#endif
