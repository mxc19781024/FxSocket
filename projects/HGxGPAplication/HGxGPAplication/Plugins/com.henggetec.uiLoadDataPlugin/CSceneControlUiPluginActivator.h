#include "CSceneControlWidget.h"
/**************************************************************************************************
* @file		: CSceneControlUiPluginActivator.h.h
* @note		: 加载场景插件启动器
* @author	: w00085
* @date		: 2020-02-15
**************************************************************************************************/
#ifndef CSCENECONTROLUIPLUGINACTIVATOR_H
#define CSCENECONTROLUIPLUGINACTIVATOR_H

#include <QObject>
#include "ctkPluginActivator.h"


#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

/**
* @class CSceneControlUiPluginActivator
* @brief 加载场景插件启动器
* @note 
* @author w00085
*/

class CSceneControlUiPluginActivator : public QObject, public ctkPluginActivator
{
	Q_OBJECT
	Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	Q_PLUGIN_METADATA(IID "Scene_Control_ui")
#endif

public:
	CSceneControlUiPluginActivator();
	~CSceneControlUiPluginActivator();

	void start(ctkPluginContext *);
	void stop(ctkPluginContext *);

private slots:
	void onAccusationLinkBtnClickedSlot(bool);
private:	
	CSceneControlWidget*	m_pSceneControlWgt;

};

#endif // CLOADSCENEUIPLUGINACTIVATOR_H
