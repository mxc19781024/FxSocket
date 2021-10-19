#include "henggetecUiLoginPlugin_p.h"
#include <QtPlugin>
#include "LoginUI.h"

henggetecUiLoginPlugin* henggetecUiLoginPlugin::instance = 0;

henggetecUiLoginPlugin::henggetecUiLoginPlugin()
  : context(0)
{
}

henggetecUiLoginPlugin::~henggetecUiLoginPlugin()
{
  
}

void henggetecUiLoginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;

  CLogin* pCLogin = new CLogin(context);
  pCLogin->hide();
}

void henggetecUiLoginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

henggetecUiLoginPlugin* henggetecUiLoginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* henggetecUiLoginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_uiLogin, henggetecUiLoginPlugin)
#endif

