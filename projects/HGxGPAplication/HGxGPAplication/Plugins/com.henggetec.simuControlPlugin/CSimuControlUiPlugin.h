#ifndef CSIMU_CONTROL_UI_PLUGIN_H
#define CSIMU_CONTROL_UI_PLUGIN_H

#include <QObject>

#include <ctkPluginContext.h>
#include <simu_control_ui/ISimuControlUiService.h>
class CSimuControlWidget;
class CSimuControlUiPlugin : public QObject, public ISimuControlUiService
{
	Q_OBJECT
	Q_INTERFACES(ISimuControlUiService)

public:
	CSimuControlUiPlugin();

	~CSimuControlUiPlugin();

	QWidget* getWgt();

private:

	CSimuControlWidget*  m_pSimuControlWgt;
};

#endif // CSIMU_CONTROL_UI_PLUGIN_H
