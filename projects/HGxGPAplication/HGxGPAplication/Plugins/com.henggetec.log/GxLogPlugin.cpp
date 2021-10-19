#include <QtPlugin>

#include "GxLog.h"

#include "GxLogPlugin.h"

CGxLogPlugin::CGxLogPlugin()
	: m_pCGxLog(nullptr)
{
}

CGxLogPlugin::~CGxLogPlugin()
{
}

void CGxLogPlugin::start(ctkPluginContext *pContext)
{
	m_pCGxLog.reset(new CGxLog);
	pContext->registerService<CGxLogService>(m_pCGxLog.data());
}

void CGxLogPlugin::stop(ctkPluginContext *context)
{
	m_pCGxLog.reset();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(com_henggetec_log, CGxLogPlugin);
#endif