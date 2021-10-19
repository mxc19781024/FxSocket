#ifndef GXEVENTHELPERINTERFACE_H
#define GXEVENTHELPERINTERFACE_H

#include <QString>

#include <ctkDictionary.h>
#include <ctkServiceRegistration.h>

/**
* @class	: CGxEventHelperInterface
* @brief	: 事件辅助接口类
* @details  : 方便二次开发人员使用ctk中的事件机制来完成插件与插件间的通信，插件与应用程序之间的通信
* @note
* @author	: w00025
*/

namespace HGxPFWCore
{
    class CGxEventHelperInterface
    {
    public:
        /**
        * @brief postEvent
        * @details 异步发送事件，具有异步性、有序性的特点；此方法在事件传递完成之前返回给调用方
        * @param strEventTopic [in] 发送事件的主题内容
        * @param properties [in] 主题的相关属性
        * @return 无
        * @note 调用postEvent后，所有订阅该主题的事件监听者都会收到此事件；
        *       properties的数据结构本质上是一个Hash表，可以输入多个键值对，键值是一一对应的
        */
        virtual void postEvent(const QString strEventTopic, const ctkDictionary &properties = ctkDictionary()) = 0;

        /**
        * @brief sendEvent
        * @details 同步发送事件；此方法在事件传递完成之后返回给调用方
        * @param strEventTopic [in] 发送事件的主题内容
        * @param properties [in] 主题的相关属性
        * @return 无
        * @note 调用sendEvent后，所有订阅该主题的事件监听者都会收到此事件；
        *       properties的数据结构本质上是一个Hash表，可以输入多个键值对，键值是一一对应的
        */
        virtual void sendEvent(const QString strEventTopic, const ctkDictionary &properties = ctkDictionary()) = 0;

        /**
        * @brief subscribeTopics
        * @details 一个事件处理对象订阅多个不同主题的事件
        * @param service [in] 事件处理对象
        * @param strTopics [in] 需要处理的事件主题的集合
        * @param pContext [in] 插件或者框架上下文
        * @return 返回服务注册对象列表，服务注册对象可以用来更新服务属性(setProperties) 以及注销之前注册的服务
        * @note 建议在应用程序内部订阅事件时，传入框架上下文；在插件内部订阅事件时，传入插件上下文；
        * @		订阅多个不同主题的事件，前提是事件处理插件正常被安装启动
        */

        virtual QList<ctkServiceRegistration> subscribeTopics(QObject* service, const QStringList &strTopics, ctkPluginContext *pContext) = 0;

        /**
        * @brief subscribeTopic
        * @details 一个事件处理对象订阅一个主题事件
        * @param service [in] 事件处理对象
        * @param strTopics [in] 需要处理的事件主题
        * @return 返回服务注册对象，服务注册对象可以用来更新服务属性(setProperties) 以及注销之前注册的服务
        * @note 订阅主题事件，前提是事件处理插件正常被安装启动
        */

        virtual ctkServiceRegistration subscribeTopic(QObject* service, const QString &strTopic, ctkPluginContext *pContext) = 0;

    };
}


#endif // !GXPLUGINFRRAMEWORKCOREINTERFACE_H
