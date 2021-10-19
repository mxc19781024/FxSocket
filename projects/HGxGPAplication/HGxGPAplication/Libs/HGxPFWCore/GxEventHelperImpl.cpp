#include <service/event/ctkEventAdmin.h>
#include <service/event/ctkEventConstants.h>
#include <service/event/ctkEventHandler.h>

#include "GxEventHelperImpl.h"
#include "GxPluginFrameworkCore.h"
#include "GxPluginFwLauncherInterface.h"
#include "GxServiceQuerier.h"

namespace HGxPFWCore
{
    /*	class CGxEventHelperImplPrivate
        {
        public:
            CGxEventHelperImplPrivate()
            {

            }
            ~CGxEventHelperImplPrivate()
            {

            }
        };*/



    CGxEventHelperImpl* CGxEventHelperImpl::s_pInstance = nullptr;

    CGxEventHelperImpl::CGxEventHelperImpl()
    {

    }

    CGxEventHelperImpl::~CGxEventHelperImpl()
    {
        if (s_pInstance)
        {
            delete s_pInstance;
            s_pInstance = nullptr;
        }
    }

    CGxEventHelperImpl* CGxEventHelperImpl::instance()
    {
        if (!s_pInstance)
        {
            s_pInstance = new CGxEventHelperImpl;

            CGxPluginFrameworkCore::fwLauncher()->start("liborg_commontk_eventadmin", ctkPlugin::START_TRANSIENT);
        }
        return s_pInstance;
    }

    void CGxEventHelperImpl::postEvent(const QString strEventTopic, const ctkDictionary &properties /*= ctkDictionary()*/)
    {
        CGxServiceQuerier<ctkEventAdmin> eventService(CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
        if (eventService)
        {
            ctkEvent event(strEventTopic, properties);
            eventService->postEvent(event);
        }
    }

    void CGxEventHelperImpl::sendEvent(const QString strEventTopic, const ctkDictionary &properties /*= ctkDictionary()*/)
    {
        CGxServiceQuerier<ctkEventAdmin> eventService(CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
        if (eventService)
        {
            ctkEvent event(strEventTopic, properties);
            eventService->sendEvent(event);
        }
    }

    QList<ctkServiceRegistration> CGxEventHelperImpl::subscribeTopics(QObject* service, const QStringList &strTopics, ctkPluginContext *pContext)
    {
        QList<ctkServiceRegistration> listServiceRegistration;

        QStringListIterator listIterator(strTopics);

        while (listIterator.hasNext())
        {
            ctkServiceRegistration registration = subscribeTopic(service, listIterator.next(), pContext);
            listServiceRegistration.push_back(registration);
        }
        return listServiceRegistration;
    }

    ctkServiceRegistration CGxEventHelperImpl::subscribeTopic(QObject* service, const QString &strTopic, ctkPluginContext *pContext)
    {
        ctkDictionary props;
        ctkServiceRegistration registration;
        props.insert(ctkEventConstants::EVENT_TOPIC, strTopic);
        if (pContext)
        {
            registration = pContext->registerService<ctkEventHandler>(service, props);
        }

        return registration;
    }

}