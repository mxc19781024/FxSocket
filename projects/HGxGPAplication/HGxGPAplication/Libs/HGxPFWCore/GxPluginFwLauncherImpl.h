#ifndef GXPLUGINFWLAUNCHERIMPL_H
#define GXPLUGINFWLAUNCHERIMPL_H

#include <QObject>
#include <QScopedPointer>

#include "GxPluginFwLauncherInterface.h"

/**
* @class	: CGxPluginFwLauncherImpl
* @brief	: 框架启动实现类
* @note
* @author	: w00025
*/

namespace HGxPFWCore
{
    class CGxPluginFwLauncherImplPrivate;
    class CGxPluginFwLauncherImpl : public QObject, public CGxPluginFwLauncherInterface
    {
        Q_OBJECT
    public:
        CGxPluginFwLauncherImpl();
        ~CGxPluginFwLauncherImpl();

        /// 单例
        static CGxPluginFwLauncherImpl *instance();

        void startAll();

        bool start(const QString& symbolicName = QString(),
            ctkPlugin::StartOptions options = ctkPlugin::START_ACTIVATION_POLICY,
            ctkPluginContext* context = 0);

        bool stop(const QString& symbolicName = QString(),
            ctkPlugin::StopOptions options = 0, ctkPluginContext* context = 0);

        bool uninstall(const QSharedPointer<ctkPlugin>& plugin);

        ctkPluginContext* getPluginContext() const;

        void addSearchPath(const QString& searchPath);

        QStringList getPluginSymbolicNames(const QString& searchPath);

        QList<QSharedPointer<ctkPlugin> > getPlugins() const;

        QSharedPointer<ctkPlugin> getPlugin(const QString& symbolicName) const;

        //QSharedPointer<ctkPluginFramework> getPluginFramework();

        //long install(const QString& symbolicName, ctkPluginContext* context = 0);

        //QString getPluginPath(const QString& symbolicName);

    private:
        static CGxPluginFwLauncherImpl *s_pInstance;

        QScopedPointer<CGxPluginFwLauncherImplPrivate> d;

    };
}


#endif // !GXPLUGINFWLAUNCHERIMPL_H
