#ifndef GXUIFRAMEWORKPLUGIN_P_H
#define GXUIFRAMEWORKPLUGIN_P_H

#include <ctkPluginActivator.h>
#include <QtPlugin>
#include "GxGuiDef.h"
#include "GxGuiMainWindow.h"
HGxGUI_USE_NAMESPACE
class CGxGuiMainWindow;	//实现类接口

class CGxUiFrameworkPlugin :
    public QObject, public ctkPluginActivator
{
    Q_OBJECT
        Q_INTERFACES(ctkPluginActivator)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
        Q_PLUGIN_METADATA(IID "com_henggetec_uiFramework")
#endif

public:

    CGxUiFrameworkPlugin();
    ~CGxUiFrameworkPlugin();

    void start(ctkPluginContext* context);
    void stop(ctkPluginContext* context);

    static CGxUiFrameworkPlugin* getInstance();

    ctkPluginContext* getPluginContext() const;


private:

    static CGxUiFrameworkPlugin* instance;
    ctkPluginContext* context;

    HGxGui::CGxGuiMainWindow* m_pServiceImp;
}; // CGxUiFrameworkPlugin

#endif // GXUIFRAMEWORKPLUGIN_P_H
