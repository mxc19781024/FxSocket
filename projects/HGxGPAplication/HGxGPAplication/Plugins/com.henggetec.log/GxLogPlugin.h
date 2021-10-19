#ifndef CGX_LOG_PLUGIN_H
#define CGX_LOG_PLUGIN_H

#include <ctkPluginActivator.h>

class CGxLog;

class CGxLogPlugin : public QObject, public ctkPluginActivator
{
	Q_OBJECT
	Q_INTERFACES(ctkPluginActivator)

#if QT_VERSION >= 0x050000
	Q_PLUGIN_METADATA(IID "com_henggetec_log")
#endif

public:
	CGxLogPlugin();
	~CGxLogPlugin();

	void start(ctkPluginContext* context);

	void stop(ctkPluginContext* context);

private:
	QScopedPointer<CGxLog> m_pCGxLog;
};

#endif // !FX_LOG_PLUGIN_ACTIVATOR_H
