#include "GxPluginFrameworkCore.h"
#include "GxPluginFwLauncherImpl.h"
#include "GxEventHelperImpl.h"

namespace HGxPFWCore
{
    static CGxPluginFrameworkCore *m_pInstance = nullptr;

    CGxPluginFrameworkCore::CGxPluginFrameworkCore()
    {
        m_pInstance = this;
    }

    CGxPluginFrameworkCore::~CGxPluginFrameworkCore()
    {

    }

    CGxPluginFrameworkCore * CGxPluginFrameworkCore::instance()
    {
        return m_pInstance;
    }

    CGxPluginFwLauncherInterface * CGxPluginFrameworkCore::fwLauncher()
    {
        return CGxPluginFwLauncherImpl::instance();
    }

    CGxEventHelperInterface * CGxPluginFrameworkCore::eventHelper()
    {
        return CGxEventHelperImpl::instance();
    }

}