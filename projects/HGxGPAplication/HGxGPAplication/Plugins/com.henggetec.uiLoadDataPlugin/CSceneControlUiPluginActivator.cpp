#include <FreeXFramework/FreeXLog.h>
#include <QtPlugin>
#include <QPushButton>
#include "CSceneControlUiPluginActivator.h"
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

CSceneControlUiPluginActivator::CSceneControlUiPluginActivator()
	: m_pSceneControlWgt(NULL)
{

}

CSceneControlUiPluginActivator::~CSceneControlUiPluginActivator()
{

}

void CSceneControlUiPluginActivator::start(ctkPluginContext *context)
{
	FREEX_LOG_DEBUG << "[UI]场景加载界面插件启动";

	// 在主程序创建菜单
	if (m_pSceneControlWgt == nullptr)
	{
		m_pSceneControlWgt = new CSceneControlWidget(context,ICore::mainWindowManager()->mainWindow());
		m_pSceneControlWgt->show();

		XGisApp::g_sizeDefaultScreen.width();
		XGisApp::g_sizeDefaultScreen.height();
		m_pSceneControlWgt->move(750,925);

		// 在主程序创建菜单
		//调整日志工具位置，在主窗口显示（2019-9-21 w00224）
		CMenuManager *pMenuMgr = ICore::menuManager();
		if (pMenuMgr == nullptr)
		{
			FREEX_LOG_DEBUG << "[UI]场景加载界面插件加载失败";
			return;
		}
	}
}

void CSceneControlUiPluginActivator::onAccusationLinkBtnClickedSlot(bool bShow)
{
	if (m_pSceneControlWgt)
	{
		if (m_pSceneControlWgt->isHidden())
		{
			m_pSceneControlWgt->show();
		}
		else
		{
			m_pSceneControlWgt->hide();
		}
	}
}

void CSceneControlUiPluginActivator::stop(ctkPluginContext *)
{
	if (m_pSceneControlWgt != m_pSceneControlWgt)
	{
		m_pSceneControlWgt->deleteLater();
		m_pSceneControlWgt = nullptr;
	}
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
Q_EXPORT_PLUGIN2(Scene_Control_ui, CSceneControlUiPluginActivator)
#endif
