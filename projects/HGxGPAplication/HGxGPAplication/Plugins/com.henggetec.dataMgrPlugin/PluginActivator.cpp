#include "PluginActivator.h"

#include <FreeXFramework/FreeXLog.h>

#include <QtPlugin>
#include <QStringList>

#include <CBizDataInterface.h>

//////////////////////////////////////////////////////////////////////////
//
CFreeXPluginActivator::CFreeXPluginActivator()
{
	
}

void CFreeXPluginActivator::start(ctkPluginContext* context)
{
	CBizDataInterface* pDataPlugin = new CBizDataInterface(context);
	context->registerService<IFxBizDataMgrService>(pDataPlugin);

	m_pBizDataPlugin = pDataPlugin;
}

void CFreeXPluginActivator::stop(ctkPluginContext* context)
{
	Q_UNUSED(context)
	if(m_pBizDataPlugin)
	{
		delete m_pBizDataPlugin;
		m_pBizDataPlugin = NULL;
	}
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
Q_EXPORT_PLUGIN2(freex_plugin_biz_datamgr_service, CFreeXPluginActivator)
#endif
