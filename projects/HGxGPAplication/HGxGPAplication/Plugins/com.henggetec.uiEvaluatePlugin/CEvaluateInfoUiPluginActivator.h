/**************************************************************************************************
* @file		: CEvaluateInfoUiPluginActivator.h
* @note		: 装备评估界面插件启动器
* @author	: w00085
* @date		: 2020-02-15
**************************************************************************************************/
#ifndef CEQUIPMENTUIPLUGINACTIVATOR_H
#define CEQUIPMENTUIPLUGINACTIVATOR_H

#include <QObject>
#include "ctkPluginActivator.h"
#include "CEvaluateInfoWidget.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

/**
* @class CEvaluateInfoUiPluginActivator
* @brief 装备评估界面插件启动器
* @note 
* @author w00085
*/
class CEvaluateInfoUiPluginActivator : public QObject, public ctkPluginActivator
{
	Q_OBJECT
		Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
		Q_PLUGIN_METADATA(IID "Evaluate_Info_ui")
#endif

public:
	CEvaluateInfoUiPluginActivator();
	~CEvaluateInfoUiPluginActivator();

	void start(ctkPluginContext *);
	void stop(ctkPluginContext *);

	private slots:
		void onAccusationLinkBtnClickedSlot(bool);
private:	
	CEvaluateInfoWidget*	m_pEvaluateInfoWgt;

};

#endif // CALARMINFOUIPLUGINACTIVATOR_H
