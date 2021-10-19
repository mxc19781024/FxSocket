#include <CSimularControlUiPlugin.h>
#include <CSimularControlWidget.h>
#include "GxUiFrameworkServiceInterface.h"
#include "GxPluginFrameworkCore.h"
#include "GxPluginFwLauncherInterface.h"

CBizSimuCtrlUiPlugin::CBizSimuCtrlUiPlugin(ctkPluginContext *context)
	:m_pContext(context)
	,m_pSimuControlWgt(nullptr)
{
	if (!m_pSimuControlWgt)
	{
		CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

		if (uiFrameworkService)
		{
			m_pSimuControlWgt = new CBizSimuCtrlWidget(context,uiFrameworkService->mainWindow());
			m_pSimuControlWgt->hide();
			m_pSimuControlWgt->move((1920 - m_pSimuControlWgt->width()) - 20, (1080 - m_pSimuControlWgt->height()) - 170);
		}
		//m_pSimuControlWgt->move(XGisApp::g_sizeDefaultScreen.width()-m_pSimuControlWgt->width() - 20 ,20 );
	}	
}

CBizSimuCtrlUiPlugin::~CBizSimuCtrlUiPlugin()
{

}

QWidget* CBizSimuCtrlUiPlugin::getWgt()
{
	return m_pSimuControlWgt;
}
