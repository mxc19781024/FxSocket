#include "GxMainWindow.h"

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxServiceQuerier.h>
#include <GxEventHelperInterface.h>

#include <service/event/ctkEventConstants.h>

#include "GxUiFrameworkServiceInterface.h"

#include "GxLogService.h"




#include <QDebug>

using namespace HGxPFWCore;

CGxMainWindow::CGxMainWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    initFramework();
}

void CGxMainWindow::handleEvent(const ctkEvent &event)
{
    QString strTopics = event.getTopic();

    int a = event.getProperty("property1").toInt();
    int b = event.getProperty("property2").toInt();
    int c = event.getProperty("property3").toInt();

}

void CGxMainWindow::initFramework()
{
    initTestEvent();

    CGxPluginFrameworkCore::fwLauncher()->startAll();


    CGxServiceQuerier<CGxUiFrameworkServiceInterface> uiFrameworkService(HGxFWLauncherInstance.getPluginContext());
    if (uiFrameworkService)
    {
        uiFrameworkService->setCustomStyle(CGxGuiStyle::Dark);
    }


    CGxServiceQuerier<CGxLogService> logService(HGxFWLauncherInstance.getPluginContext());
    if (logService)
    {
        logService->debug("this is a debug message", true);
    }


}

void CGxMainWindow::initTestEvent()
{
    HGxEventHelperInstance.subscribeTopic(this, "com/henggetec/test", HGxFWLauncherInstance.getPluginContext());

    // 	ctkDictionary dic;
    // 	dic.insert(ctkEventConstants::EVENT_TOPIC, "com/henggetec/test");
    // 	HGxFWLauncherInstance.getPluginContext()->registerService<ctkEventHandler>(this, dic);
}
