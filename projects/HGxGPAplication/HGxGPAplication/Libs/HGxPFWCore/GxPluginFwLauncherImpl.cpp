#include "GxPluginFwLauncherImpl.h"
#include "GxGPAppConfig.h"
#include "GxPluginFrameworkCore.h"
#include "GxServiceQuerier.h"


#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginException.h>
#include <ctkPluginFramework.h>

#include <QDebug>
#include <QDirIterator>
#include <QListIterator>
#include <GxLogService.h>
#include "service\event\ctkEventAdmin.h"
#include <iostream>

namespace HGxPFWCore
{

    class CGxPluginFwLauncherImplPrivate
    {
    public:
        CGxPluginFwLauncherImplPrivate()
            : fwFactory(0)
        {
     #ifdef CMAKE_INTDIR
                 QString pluginPath = HGX_PLUGINS_LIBRARIES_DIR  CMAKE_INTDIR "/";
     #else
                 QString pluginPath = CTK_PLUGIN_DIR;
     #endif
     
                 pluginSearchPaths.append(pluginPath);
     
                 pluginLibFilter << "*.dll" << "*.so" << "*.dylib";
        }

        ~CGxPluginFwLauncherImplPrivate()
        {

        }

        QSharedPointer<ctkPlugin> install(const QUrl& pluginPath, ctkPluginContext* context)
        {
            try
            {
                return context->installPlugin(pluginPath);
            }
            catch (const ctkPluginException& exc)
            {
                qWarning() << "Failed to install plugin " << pluginPath << ":" << exc.printStackTrace();
                return QSharedPointer<ctkPlugin>();
            }
        }

        QSharedPointer<ctkPlugin> install(const QString& symbolicName, ctkPluginContext* context = nullptr)
        {
            QString pluginPath = getPluginPath(symbolicName);
            if (pluginPath.isEmpty()) return QSharedPointer<ctkPlugin>();

            ctkPluginContext* pc = context;

            if (pc == 0 && fwFactory == 0)
            {
                fwFactory.reset(new ctkPluginFrameworkFactory(fwProps));
                try
                {
                    fwFactory->getFramework()->init();
                    pc = fwFactory->getFramework()->getPluginContext();
                }
                catch (const ctkPluginException& exc)
                {
                    qCritical() << QObject::tr("Failed to initialize the plug-in framework:") << exc;
                    fwFactory.reset();
                    return QSharedPointer<ctkPlugin>();
                }
            }

            return install(QUrl::fromLocalFile(pluginPath), pc);
        }

        bool initFramework(ctkPlugin::StartOptions options)
        {
            if (fwFactory == 0)
            {
				fwProps.insert(ctkPluginConstants::FRAMEWORK_STORAGE_CLEAN, ctkPluginConstants::FRAMEWORK_STORAGE_CLEAN_ONFIRSTINIT);
                fwFactory.reset(new ctkPluginFrameworkFactory(fwProps));
                try
                {
					if (fwFactory->getFramework())
					{
						fwFactory->getFramework()->start();
					}
                }
                catch (const ctkPluginException& exc)
                {
                    qCritical() << QObject::tr("Failed to start the plug-in framework") << exc;
                    fwFactory.reset();
                    return false;
                }
            }
            else if (fwFactory->getFramework()->getState() != ctkPlugin::ACTIVE)
            {
                try
                {
                    fwFactory->getFramework()->start(options);
                }
                catch (const ctkPluginException& exc)
                {
                    qCritical() << QObject::tr("Failed to start the plug-in framework:") << exc;
                    fwFactory.reset();
                    return false;
                }
            }
            return true;
        }

        QString getPluginPath(const QString &symbolicName)
        {
            QString pluginFileName(symbolicName);
            pluginFileName.replace(".", "_");
            foreach(QString searchPath, pluginSearchPaths)
            {
                QDirIterator dirIter(searchPath, pluginLibFilter, QDir::Files);
                while (dirIter.hasNext())
                {
                    dirIter.next();
                    QFileInfo fileInfo = dirIter.fileInfo();
                    QString fileBaseName = fileInfo.baseName();
                    if (fileBaseName.startsWith("lib")) fileBaseName = fileBaseName.mid(3);

#ifdef QT_NO_DEBUG

#else
                    if (fileBaseName.endsWith("d") && !fileBaseName.startsWith("org_commontk"))
                    {
                        fileBaseName = fileBaseName.remove(fileBaseName.size() - 1, 1);
                    }
#endif

                    if (fileBaseName == pluginFileName)
                    {
                        return fileInfo.canonicalFilePath();
                    }
                }
            }
            return QString();
        }

    public:
        QStringList pluginSearchPaths;
        QStringList pluginLibFilter;
        ctkProperties fwProps;
        QScopedPointer<ctkPluginFrameworkFactory> fwFactory;

    };

    CGxPluginFwLauncherImpl* CGxPluginFwLauncherImpl::s_pInstance = nullptr;

    CGxPluginFwLauncherImpl::CGxPluginFwLauncherImpl() :d(new CGxPluginFwLauncherImplPrivate())
    {
        // 初始化框架
        d->initFramework(ctkPlugin::START_TRANSIENT);
    }

    CGxPluginFwLauncherImpl::~CGxPluginFwLauncherImpl()
    {
        if (s_pInstance)
        {
            delete s_pInstance;
            s_pInstance = nullptr;
        }
    }

    CGxPluginFwLauncherImpl* CGxPluginFwLauncherImpl::instance()
    {
        if (!s_pInstance)
        {
            s_pInstance = new CGxPluginFwLauncherImpl;

            // 启动ctk的事件管理插件和日志插件    名字                           方式                            插件上下文
           bool n =  s_pInstance->start("org_commontk_eventadmin", ctkPlugin::START_TRANSIENT, s_pInstance->getPluginContext());
		   if (n)
		   {
			   qDebug() << "org_commontk_eventadmin Successfully opened pluggable";
		   }
		   else
		   {
			   qDebug() << "org_commontk_eventadmin Failed to open pluggable ";
		   }

		   bool b_instance =  s_pInstance->start("com_henggetec_log", ctkPlugin::START_ACTIVATION_POLICY, s_pInstance->getPluginContext());
		   if (b_instance)
		   {
			   qDebug() << "com_henggetec_log is success open pluggable";
		   }
		   else
		   {
			   qDebug() << "com_henggetec_log is not open pluggable ";
		   }
        }
        return s_pInstance;
    }

    void CGxPluginFwLauncherImpl::startAll()
    {
//         // 初始化框架
         d->initFramework(ctkPlugin::START_TRANSIENT);
// 
         // 先启动ctk的事件管理插件和日志插件
       start("org_commontk_eventadmin", ctkPlugin::START_TRANSIENT, getPluginContext());
		 start("com_henggetec_log", ctkPlugin::START_ACTIVATION_POLICY, getPluginContext());

        // 获取日志服务
        CGxServiceQuerier<CGxLogService> logService(HGxFWLauncherInstance.getPluginContext());

        QList<QSharedPointer<ctkPlugin>> plugins;
        // 先安装插件目录下的插件
        QStringListIterator pluginSearchPathsIterator(d->pluginSearchPaths);
        while (pluginSearchPathsIterator.hasNext())
        {
            QString strPluginSearchPath = pluginSearchPathsIterator.next().toLocal8Bit().constData();

            QFileInfo fileInfo(strPluginSearchPath);
            if (!fileInfo.exists())
            {
                continue;
            }

			QDir qdir(strPluginSearchPath);
			QStringList filter;
			filter << "*.dll";
			QList<QFileInfo> *fileInfoList = new QList<QFileInfo>(qdir.entryInfoList(filter));

            QDirIterator dirIter(strPluginSearchPath, d->pluginLibFilter, QDir::Files);
			int count = fileInfoList->count();
			int nIndex = 0;
            while (dirIter.hasNext())
            {
                QString strInfoMsg;
                QString strPluginPath = dirIter.next();
                QFileInfo pluginInfo = QFileInfo(strPluginPath);
                QString pluginSymbolicName = pluginInfo.baseName();
                try
                {
                    if (pluginSymbolicName.contains("libcom_henggetec_"))
                    {
                        strInfoMsg = QString("install %1 plugin").arg(pluginSymbolicName);
                        if (logService)
                        {
                            logService->info(strInfoMsg.toStdString(), true);
                        }
                        QSharedPointer<ctkPlugin> plugin = d->install(QUrl::fromLocalFile(strPluginPath), getPluginContext());
                        plugins.push_back(plugin);
						//w00085
						HGxPFWCore::CGxServiceQuerier<ctkEventAdmin> eventService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
						if (eventService)
						{
							nIndex++;
							ctkDictionary sceneCreatedDic;
							sceneCreatedDic.insert("iValue", 80/count* nIndex);
							sceneCreatedDic.insert("strMessage", strInfoMsg.toStdString().c_str());
							ctkEvent event = ctkEvent("event/Progerss", sceneCreatedDic);
							eventService->sendEvent(event);
						}
                    }

                }
                catch (const ctkPluginException& e)
                {
                    if (logService)
                    {
                        strInfoMsg = QString("install %1 failed: %2").arg(pluginSymbolicName).arg(e.what());
                        logService->critical(strInfoMsg.toStdString(), true);
                    }
                    else
                    {
                        qCritical() << e.what();
                    }
                }
            }
        }

        // 2 启动其他插件
        QListIterator<QSharedPointer<ctkPlugin>> pluginsIterator(plugins);
        while (pluginsIterator.hasNext())
        {
            QSharedPointer<ctkPlugin> plugin = pluginsIterator.next();
            try
            {
                if (plugin)
                {
                    QString strSymbolicName = plugin->getSymbolicName();
                    QString strStartMsg = QString("start %1 plugin").arg(strSymbolicName);
                    if (logService)
                    {
                        logService->info(strStartMsg.toStdString(), true);
                    }

                    plugin->start(ctkPlugin::START_TRANSIENT);
                }
            }
            catch (const ctkPluginException &e)
            {
                QString strCriticalMsg = QString("Fail to start plugin: %1").arg(e.message());
                if (logService)
                {
                    logService->critical(strCriticalMsg.toStdString(), true);
                }
                else
                {
                    qCritical() << QObject::tr("Fail to start plugin") << e.message();
                }
            }
        }
    }

    bool CGxPluginFwLauncherImpl::start(const QString& symbolicName, ctkPlugin::StartOptions options, ctkPluginContext* context)
    {
        // 获取日志服务
        CGxServiceQuerier<CGxLogService> logService(HGxFWLauncherInstance.getPluginContext());
        // 如果没有传入插件上下文以及框架工厂没有实例化，则先实例化框架工厂，启动框架
        if (context == 0 && d->fwFactory == 0) {
            d->fwFactory.reset(new ctkPluginFrameworkFactory(d->fwProps));
            try
            {
                d->fwFactory->getFramework()->start();
            }
            catch (const ctkPluginException& exc)
            {
                if (logService)
                {
                    QString strMsg = QString("Failed to start the plug-in framework: %1").arg(exc.message());
                    logService->critical(strMsg.toStdString(), true);
                }
                else
                {
                    qCritical() << QObject::tr("Failed to start the plug-in framework") << exc;
                }
                
                d->fwFactory.reset();
                return false;
            }
        }
        else if (context == 0 && d->fwFactory->getFramework()->getState() != ctkPlugin::ACTIVE)
        {
            try
            {
                d->fwFactory->getFramework()->start(options);
            }
            catch (const ctkPluginException& exc)
            {
                if (logService)
                {
                    QString strMsg = QString("Failed to start the plug-in framework: %1").arg(exc.message());
                    logService->critical(strMsg.toStdString(), true);
                }
                else
                {
                    qCritical() << QObject::tr("Failed to start the plug-in framework") << exc;
                }
                d->fwFactory.reset();
                return false;
            }
        }

        if (!symbolicName.isEmpty())
        {
			std::cout << symbolicName.toStdString() << std::endl;
            QString pluginPath = d->getPluginPath(symbolicName);
			//std::cout << pluginPath.toStdString() << std::endl;
            if (pluginPath.isEmpty()) return false;

            ctkPluginContext* pc = context ? context : getPluginContext();
            try
            {
                if (logService)
                {
                    QString strInfoMsg = QString("install plugin: symbolicName is %1").arg(symbolicName);
                    logService->info(strInfoMsg.toStdString(), true);
                }
                pc->installPlugin(QUrl::fromLocalFile(pluginPath))->start(options);
            }
            catch (const ctkPluginException& exc)
            {
                if (logService)
                {
                    QString strWarnMsg = QString("Failed to install plugin: symbolicName is %1").arg(symbolicName);
                    logService->warn(strWarnMsg.toStdString(), true);
                }
                else
                {
                    qWarning() << QObject::tr("Failed to install plugin:") << exc;
                }
                return false;
            }
        }

        return true;
    }

    bool CGxPluginFwLauncherImpl::stop(const QString& symbolicName /*= QString()*/, ctkPlugin::StopOptions options /*= 0*/, ctkPluginContext* context /*= 0*/)
    {
        // 获取日志服务
        CGxServiceQuerier<CGxLogService> logService(HGxFWLauncherInstance.getPluginContext());

        if (d->fwFactory == 0) return true;

        ctkPluginContext* pc = context ? context : getPluginContext();
        if (pc == 0)
        {
            if (logService)
            {
                logService->warn("No valid plug-in context available", true);
            }
            else
            {
                qWarning() << QObject::tr("No valid plug-in context available");
            }
            return false;
        }

        if (!symbolicName.isEmpty())
        {
            QString pluginPath = d->getPluginPath(symbolicName);
            if (pluginPath.isEmpty()) return false;

            QString strWarnInfo;
            try
            {
                QList<QSharedPointer<ctkPlugin> > plugins = pc->getPlugins();
                foreach(QSharedPointer<ctkPlugin> plugin, plugins)
                {
                    if (plugin->getSymbolicName() == symbolicName)
                    {
                        if (logService)
                        {
                            QString strInfoMsg = QString("stop plugin, symbolic name is %1").arg(symbolicName);
                            logService->info(strInfoMsg.toStdString(), true);
                        }
                        plugin->stop(options);
                        return true;
                    }
                }
                if (logService)
                {
                    strWarnInfo = QString("Plugin %1 not found").arg(symbolicName);
                    logService->warn(strWarnInfo.toStdString(), true);
                }
                else
                {
                    qWarning() << QObject::tr("Plug-in") << symbolicName << QObject::tr("not found");
                }
                return false;
            }
            catch (const ctkPluginException& exc)
            {
                if (logService)
                {
                    strWarnInfo = QString("Failed to stop plug-in: %1,message is %2").arg(symbolicName).arg(exc.message());
                    logService->warn(strWarnInfo.toStdString(), true);
                }
                else
                {
                    qWarning() << QObject::tr("Failed to stop plug-in:") << exc;
                }
                return false;
            }
        }
        else
        {
            // stop the framework
            QSharedPointer<ctkPluginFramework> fw =
                qSharedPointerCast<ctkPluginFramework>(pc->getPlugin(0));
            try
            {
                fw->stop();
                ctkPluginFrameworkEvent fe = fw->waitForStop(5000);
                if (fe.getType() == ctkPluginFrameworkEvent::FRAMEWORK_WAIT_TIMEDOUT)
                {
                    if (logService)
                    {
                        logService->warn("Stopping the plugin framework timed out", true);
                    }
                    else
                    {
                        qWarning() << QObject::tr("Stopping the plugin framework timed out");
                    }
                    
                    return false;
                }
            }
            catch (const ctkRuntimeException& e)
            {
                if (logService)
                {
                    QString strMsg = QString("Stopping the plugin framework failed: %1").arg(e.message());
                    logService->warn(strMsg.toStdString(), true);
                }
                else
                {
                    qWarning() << QObject::tr("Stopping the plugin framework failed: ") << e;
                }
                
                return false;
            }

            return true;
        }
    }

    /*	long CGxPluginFwLauncherImpl::install(const QString& symbolicName, ctkPluginContext* context)
       {
           QSharedPointer<ctkPlugin> plugin = d->install(symbolicName, context);
           if (plugin)
           {
               return plugin->getPluginId();
           }
           return -1;
       }*/

    bool CGxPluginFwLauncherImpl::uninstall(const QSharedPointer<ctkPlugin>& plugin)
    {
        if (plugin)
        {
            plugin->uninstall();
            return true;
        }
        return false;
    }

    ctkPluginContext* CGxPluginFwLauncherImpl::getPluginContext() const
    {
		if (d->fwFactory == 0)
		{
			return 0;
		}
		auto test = d->fwFactory->getFramework();
		auto test1 = test->getPluginContext();
        return test1;
    }

    /*QSharedPointer<ctkPluginFramework> CGxPluginFwLauncherImpl::getPluginFramework()
    {
        if (d->fwFactory)
            return d->fwFactory->getFramework();
        return QSharedPointer<ctkPluginFramework>();
    }*/

    void CGxPluginFwLauncherImpl::addSearchPath(const QString& searchPath)
    {
        d->pluginSearchPaths.prepend(searchPath);
    }

    /*QString CGxPluginFwLauncherImpl::getPluginPath(const QString & symbolicName)
    {
        QString pluginFileName(symbolicName);
        pluginFileName.replace(".", "_");
        foreach(QString searchPath, d->pluginSearchPaths)
        {
            QDirIterator dirIter(searchPath, d->pluginLibFilter, QDir::Files);
            while (dirIter.hasNext())
            {
                dirIter.next();
                QFileInfo fileInfo = dirIter.fileInfo();
                QString fileBaseName = fileInfo.baseName();
                if (fileBaseName.startsWith("lib")) fileBaseName = fileBaseName.mid(3);

                if (fileBaseName == pluginFileName)
                {
                    return fileInfo.canonicalFilePath();
                }
            }
        }
        return QString();
    }*/

    QStringList CGxPluginFwLauncherImpl::getPluginSymbolicNames(const QString& searchPath)
    {
        QStringList result;
        QDirIterator dirIter(searchPath, d->pluginLibFilter, QDir::Files);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QFileInfo fileInfo = dirIter.fileInfo();
            QString fileBaseName = fileInfo.baseName();
            if (fileBaseName.startsWith("lib")) fileBaseName = fileBaseName.mid(3);
            result << fileBaseName.replace("_", ".");
        }

        return result;
    }

    QList<QSharedPointer<ctkPlugin> > CGxPluginFwLauncherImpl::getPlugins() const
    {
        QList<QSharedPointer<ctkPlugin> > listPlugins;

        ctkPluginContext *pFwContext = getPluginContext();

        if (pFwContext)
        {
            return pFwContext->getPlugins();
        }

        return listPlugins;
    }

    QSharedPointer<ctkPlugin> CGxPluginFwLauncherImpl::getPlugin(const QString& symbolicName) const
    {
        QSharedPointer<ctkPlugin> targetPlugin;

        QListIterator<QSharedPointer<ctkPlugin>> pluginsIterator(getPlugins());

        while (pluginsIterator.hasNext())
        {
            QSharedPointer<ctkPlugin> plugin = pluginsIterator.next();
            QString strSymbolicName = plugin->getSymbolicName();
            if (strSymbolicName == symbolicName)
            {
                targetPlugin = plugin;

                return targetPlugin;
            }
        }

        return targetPlugin;
    }

}