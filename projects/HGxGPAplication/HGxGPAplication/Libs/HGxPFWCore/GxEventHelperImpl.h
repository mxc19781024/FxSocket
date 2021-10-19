#ifndef GXEVENTHELPERIMPL_H
#define GXEVENTHELPERIMPL_H

#include "GxPFWCoreExport.h"

#include "GxEventHelperInterface.h"
/**
* @class	: CGxEventHelperImpl
* @brief	: 事件辅助实现类
* @note
* @author	: w00025
*/

namespace HGxPFWCore
{
    class CGxEventHelperImpl : public QObject, public CGxEventHelperInterface
    {
        Q_OBJECT
    public:
        CGxEventHelperImpl();
        ~CGxEventHelperImpl();

        /// 单例
        static CGxEventHelperImpl *instance();

        void postEvent(const QString strEventTopic, const ctkDictionary &properties = ctkDictionary());

        void sendEvent(const QString strEventTopic, const ctkDictionary &properties = ctkDictionary());

        QList<ctkServiceRegistration> subscribeTopics(QObject* service, const QStringList &strTopics, ctkPluginContext *pContext);

        ctkServiceRegistration subscribeTopic(QObject* service, const QString &strTopic, ctkPluginContext *pContext);

    private:

        //QScopedPointer<CGxEventHelperImplPrivate> d;
        static CGxEventHelperImpl *s_pInstance;
    };
}

#endif // !GXEVENTHELPERIMPL_H
