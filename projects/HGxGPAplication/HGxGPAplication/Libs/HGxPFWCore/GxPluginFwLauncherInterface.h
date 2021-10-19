#ifndef GXPLUGINFWLAUNCHERINTERFACE_H
#define GXPLUGINFWLAUNCHERINTERFACE_H

#include "GxPFWCoreExport.h"


#include <ctkPluginContext.h>
#include <ctkServiceReference.h>

#include <QSharedPointer>
#include <QString>

/**
* @class	: CGxPluginFwLauncherInterface
* @brief	: 框架启动接口类
* @note
* @author	: w00025
*/

namespace HGxPFWCore
{
    class CGxPluginFwLauncherInterface
    {

    public:
        /**
        * @brief startAll
        * @details 启动应用程序插件目录下的所有插件
        **/
        virtual void startAll() = 0;

        /**
        * @brief start
        * @details 启动插件
        * @param symbolicName [in] 插件的唯一标识
        * @param options [in] 插件激活策略，默认是ctkPlugin::START_ACTIVATION_POLICY
        *  按照manifest_headers.cmake中声明的激活策略去激活插件
        * @param context [in] 框架上下文，如果传入值为空，则由框架内部获取到框架上下文完成插件的启动
        * @return 启动成功返回true，启动失败返回false
        * @note 在manifest_headers.cmake中声明的激活策略只能显示声明为eager加载，如果想延迟加载，则无需在 \n
        * manifest_headers.cmake文件进行声明。\n
        * 如果传入的插件激活策略为ctkPlugin::START_TRASIENT，则即使没有在manifest_headers.cmake文件中声明激活策略 \n
        * 插件也会按照eager加载即程序一启动就加载
        */
        virtual bool start(const QString& symbolicName = QString(),
            ctkPlugin::StartOptions options = ctkPlugin::START_ACTIVATION_POLICY,
            ctkPluginContext* context = 0) = 0;

        /**
        * @brief stop
        * @details 停止插件
        * @param symbolicName [in] 插件的唯一标识
        * @param options [in] 插件的停止策略。默认是STOP_TRANSIENT
        * @param context [in] 框架上下文，如果传入值为空，则由框架内部获取到框架上下文完成插件的停止
        * @return 停止成功返回true，停止失败返回false
        */
        virtual bool stop(const QString& symbolicName = QString(),
            ctkPlugin::StopOptions options = 0, ctkPluginContext* context = 0) = 0;

        /**
        * @brief uninstall
        * @details 卸载插件，将插件在内存中占用的资源也一并释放掉
        * @param plugin [in] 插件实例化对象
        * @return 卸载成功返回true，卸载失败返回false
        * @note 此函数不太稳定，请谨慎使用
        */
        virtual bool uninstall(const QSharedPointer<ctkPlugin>& plugin) = 0;

        /**
        * @brief getPluginContext
        * @details 获取框架上下文
        * @return 框架上下文
        * @note 框架如果没有启动，则获取到的框架上下文为空
        */
        virtual ctkPluginContext* getPluginContext() const = 0;

        /**
        * @brief addSearchPath
        * @details 添加插件所在的查找路径
        * @param searchPath [in] 插件所在的目录
        * @return 无
        * @note 应用启动时，会从默认的插件目录中加载插件，当用户定义的目录中也存在插件且需要加载时，可以调用此函数进行路径的添加
        */
        virtual void addSearchPath(const QString& searchPath) = 0;

        /**
        * @brief getPluginSymbolicNames
        * @details 根据插件目录的路径来获取所有插件的唯一标识
        * @param searchPath [in] 插件所在的目录
        * @return 返回所有插件的唯一标识
        * @note
        */
        virtual QStringList getPluginSymbolicNames(const QString& searchPath) = 0;

        /**
        * @brief getPlugins
        * @details 获取应用中所有已安装的插件对象
        * @return 插件对象列表
        * @note [备注/注意事项]
        */
        virtual QList<QSharedPointer<ctkPlugin> > getPlugins() const = 0;

        /**
        * @brief getPlugin
        * @details 根据插件的唯一标识获取插件对象
        * @param symbolicName1 [in] 插件的唯一标识
        * @return 返回对应的插件对象
        * @note [备注/注意事项]
        */
        virtual QSharedPointer<ctkPlugin> getPlugin(const QString& symbolicName) const = 0;

        //virtual long install(const QString& symbolicName, ctkPluginContext* context = 0) = 0;

        //virtual QString getPluginPath(const QString& symbolicName) = 0;

        //virtual QSharedPointer<ctkPluginFramework> getPluginFramework() = 0;
    };
}


#endif // !GXPLUGINFWLAUNCHERINTERFACE_H
