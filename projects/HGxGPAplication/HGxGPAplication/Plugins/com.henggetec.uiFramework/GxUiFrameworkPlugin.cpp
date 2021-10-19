#include "GxUiFrameworkPlugin_p.h"
#include <QtPlugin>

#include "GxPluginFrameworkCore.h"

CGxUiFrameworkPlugin* CGxUiFrameworkPlugin::instance = 0;

CGxUiFrameworkPlugin::CGxUiFrameworkPlugin()
    : context(0)
    , m_pServiceImp(nullptr)
{
}

CGxUiFrameworkPlugin::~CGxUiFrameworkPlugin()
{

}

void CGxUiFrameworkPlugin::start(ctkPluginContext* context)
{
    instance = this;
    this->context = context;

    if (!m_pServiceImp)
    {
        m_pServiceImp = new HGxGui::CGxGuiMainWindow("业务演示平台", "1");

        ctkDictionary props;
        props.insert(ctkPluginConstants::SERVICE_RANKING, 0);
        context->registerService<CGxUiFrameworkServiceInterface>(m_pServiceImp, props);
    }
}

void CGxUiFrameworkPlugin::stop(ctkPluginContext* context)
{
    Q_UNUSED(context)

        //m_pServiceImp->deleteLater();
        delete m_pServiceImp;
    m_pServiceImp = nullptr;

    HGXPFCoreInstance.ungetService<CGxUiFrameworkServiceInterface>(context);

}

CGxUiFrameworkPlugin* CGxUiFrameworkPlugin::getInstance()
{
    return instance;
}

ctkPluginContext* CGxUiFrameworkPlugin::getPluginContext() const
{
    return context;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(com_henggetec_uiFramework, CGxUiFrameworkPlugin)
#endif


