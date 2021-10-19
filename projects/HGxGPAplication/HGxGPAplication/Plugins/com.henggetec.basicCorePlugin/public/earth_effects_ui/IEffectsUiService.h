#ifndef IEFFECTS_UI_SERVICE_H
#define IEFFECTS_UI_SERVICE_H

#include <QWidget>
#include <QtPlugin>

class IEffectsUiService
{
public:
	/**  
		* @brief 获取环境插件的窗口指针
		* @return 环境插件的窗口指针
	*/
	virtual QWidget* getWgt() = 0;
};

Q_DECLARE_INTERFACE(IEffectsUiService, "freex_effects_ui_service")

#endif
