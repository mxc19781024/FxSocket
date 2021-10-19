#include "CGxServicePlugin_p.h"
#include <QtPlugin>

#include "CServiceObject.h"

CGxServicePlugin* CGxServicePlugin::instance = 0;

CGxServicePlugin::CGxServicePlugin()
  : context(0)
{
}

CGxServicePlugin::~CGxServicePlugin()
{
  
}

void CGxServicePlugin::start(ctkPluginContext* context)
{
	instance = this;
	this->context = context;

	m_pServiceObj = new CServiceObject;
	ctkDictionary dic;
	context->registerService<IServiceUiInterface>(m_pServiceObj, dic);

}

void CGxServicePlugin::stop(ctkPluginContext* context)
{
	Q_UNUSED(context)

	if (m_pServiceObj != nullptr)
	{
		m_pServiceObj->deleteLater();
		m_pServiceObj = NULL;
	}
}

CGxServicePlugin* CGxServicePlugin::getInstance()
{
  return instance;
}

ctkPluginContext* CGxServicePlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_servicePlugin, CGxServicePlugin)
#endif

