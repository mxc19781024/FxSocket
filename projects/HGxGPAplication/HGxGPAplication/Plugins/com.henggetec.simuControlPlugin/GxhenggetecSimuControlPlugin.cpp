#include "GxhenggetecSimuControlPlugin_p.h"
#include <QtPlugin>
#include <QWidget>

#include <CSimuControlUiPlugin.h>

CGxhenggetecSimuControlPlugin* CGxhenggetecSimuControlPlugin::instance = 0;

CGxhenggetecSimuControlPlugin::CGxhenggetecSimuControlPlugin()
  : context(0)
  , m_pSimuControlObj(NULL)
{
}

CGxhenggetecSimuControlPlugin::~CGxhenggetecSimuControlPlugin()
{
  
}

void CGxhenggetecSimuControlPlugin::start(ctkPluginContext* context)
{
	instance = this;
	this->context = context;

	m_pSimuControlObj = new CSimuControlUiPlugin();
	ctkDictionary dic;
	context->registerService<ISimuControlUiService>(m_pSimuControlObj, dic);
}

void CGxhenggetecSimuControlPlugin::stop(ctkPluginContext* context)
{
	Q_UNUSED(context)

	delete m_pSimuControlObj;
	m_pSimuControlObj = NULL;
}

CGxhenggetecSimuControlPlugin* CGxhenggetecSimuControlPlugin::getInstance()
{
	return instance;
}

ctkPluginContext* CGxhenggetecSimuControlPlugin::getPluginContext() const
{
	return context;
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
Q_EXPORT_PLUGIN2(com_henggetec_simuControlPlugin, CGxhenggetecSimuControlPlugin)
#endif


