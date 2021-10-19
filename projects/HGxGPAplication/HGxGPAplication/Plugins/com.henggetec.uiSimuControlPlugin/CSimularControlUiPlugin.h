#ifndef CSIMU_CTRL_UI_PLUGIN_H
#define CSIMU_CTRL_UI_PLUGIN_H

#include <QObject>
#include <ctkPluginContext.h>
#include <simu_control_ui/ISimuControlUiService.h>

class CBizSimuCtrlWidget;
class CBizSimuCtrlUiPlugin : public QObject, public ISimuControlUiService
{
	Q_OBJECT
	Q_INTERFACES(ISimuControlUiService)

public:
	CBizSimuCtrlUiPlugin(ctkPluginContext *context);
	~CBizSimuCtrlUiPlugin();

	QWidget* getWgt();

private:
	ctkPluginContext* m_pContext;
	CBizSimuCtrlWidget* m_pSimuControlWgt;
};

#endif // CSIMU_CTRL_UI_PLUGIN_H
