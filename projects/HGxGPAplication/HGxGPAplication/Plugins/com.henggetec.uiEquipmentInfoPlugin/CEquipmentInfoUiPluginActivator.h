/**************************************************************************************************
* @file		: CEquipmentInfoUiPluginActivator.h
* @note		: 装备信息界面插件启动器
* @author	: w00085
* @date		: 2020-02-15
**************************************************************************************************/
#ifndef CEQUIPMENTUIPLUGINACTIVATOR_H
#define CEQUIPMENTUIPLUGINACTIVATOR_H

#include <QObject>
#include "ctkPluginActivator.h"
#include "CEquipmentInfoWidget.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

/**
* @class CEquipmentInfoUiPluginActivator
* @brief 装备信息界面插件启动器
* @note 
* @author w00085
*/
class CEquipmentInfoUiPluginActivator : public QObject, public ctkPluginActivator
{
	Q_OBJECT
	Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	Q_PLUGIN_METADATA(IID "Equipment_Info_ui")
#endif

public:
	CEquipmentInfoUiPluginActivator();
	~CEquipmentInfoUiPluginActivator();

	void start(ctkPluginContext *);
	void stop(ctkPluginContext *);

private slots:
	void onAccusationLinkBtnClickedSlot(bool);
private:	
	CEquipmentInfoWidget*	m_pEquipmentInfoWgt;

};

#endif // CALARMINFOUIPLUGINACTIVATOR_H
