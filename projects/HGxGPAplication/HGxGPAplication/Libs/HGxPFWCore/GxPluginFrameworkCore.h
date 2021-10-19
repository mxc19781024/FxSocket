#ifndef GXPLUGINFRRAMEWORKCORE_H
#define GXPLUGINFRRAMEWORKCORE_H

#include <GxPFWCoreExport.h>

#include <ctkPluginContext.h>
#include <ctkServiceReference.h>

/**
* @class	: CGxPluginFrameworkCore
* @brief	: 框架核心接口类
* @note
* @author	: w00025
*/

namespace HGxPFWCore
{
    class CGxPluginFwLauncherInterface;
    class CGxEventHelperInterface;

    class HGX_PFCORE_EXPORT CGxPluginFrameworkCore
    {

    public:
        /**
        * @brief instance
        * @details 插件框架核心入口
        * @return 返回插件框架核心单例对象
        */
        static CGxPluginFrameworkCore *instance();

        /**
        * @brief fwLauncher
        * @details 框架辅助类，用来进行框架的初始化以及插件的管理
        * @return 返回框架辅助类的单例对象
        */
        static CGxPluginFwLauncherInterface *fwLauncher();

        /**
        * @brief eventHelper
        * @details 事件辅助类,便于二次开发人员通过事件机制进行插件间通信、插件应用间通信
        * @return 返回事件辅助类的单例对象
        */
        static CGxEventHelperInterface *eventHelper();

        /**
        * @brief getService
        * @details 获取服务的模板函数
        * @param pContext [in] 插件上下文或者框架上下文
        * @return 服务接口类指针
        * @note 建议在应用程序内部获取插件服务时，传入框架上下文；在插件内部获取其他插件服务时，传入插件上下文
        */
        template<typename T> T *getService(ctkPluginContext *pContext)
        {
            ctkServiceReference serviceRef = pContext->getServiceReference<T>();

            if (!serviceRef)
            {
                return nullptr;
            }

            return pContext->getService<T>(serviceRef);
        }

        /**
        * @brief ungetService
        * @details 释放服务的模板函数
        * @param pContext [in] 插件上下文或者框架上下文
        * @return 释放成功返回true，释放失败返回false
        * @note 建议在应用程序内部获取插件服务时，传入框架上下文；在插件内部获取其他插件服务时，传入插件上下文
        */
        template<typename T>
        bool ungetService(ctkPluginContext *pContext)
        {
            ctkServiceReference serviceRef = pContext->getServiceReference<T>();

            if (!serviceRef)
            {
                return false;
            }

            return pContext->ungetService(serviceRef);
        }


    private:
        CGxPluginFrameworkCore();
        ~CGxPluginFrameworkCore();
    };
}
// 框架核心实例
#define HGXPFCoreInstance (*HGxPFWCore::CGxPluginFrameworkCore::instance())

// 插件管理实例
#define HGxFWLauncherInstance (*HGxPFWCore::CGxPluginFrameworkCore::fwLauncher())

// 事件辅助类实例
#define HGxEventHelperInstance (*HGxPFWCore::CGxPluginFrameworkCore::eventHelper())

#endif // !GXPLUGINFRRAMEWORKCORE_H
