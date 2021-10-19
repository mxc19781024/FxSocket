/**************************************************************************************************
* @file		: CGanateChartUiPluginActivator.h
* @note		: 甘特图界面插件启动器
* @author	: w00068
* @date		: 2020-02-15
**************************************************************************************************/
#ifndef CGANATECHARTUIPLUGINACTIVATOR_H
#define CGANATECHARTUIPLUGINACTIVATOR_H

#include <QObject>
#include "ctkPluginActivator.h"
#include <FxGanttChartWidget.h>

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

/**
* @class CGanateChartUiPluginActivator
* @brief 装备评估界面插件启动器
* @note 
* @author w00085
*/
class CGanateChartUiPluginActivator : public QObject, public ctkPluginActivator
{
	Q_OBJECT
		Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
		Q_PLUGIN_METADATA(IID "Ganate_Chart_ui")
#endif

public:
	CGanateChartUiPluginActivator();
	~CGanateChartUiPluginActivator();

	void start(ctkPluginContext *);
	void stop(ctkPluginContext *);
private:	
	CFxGanttChart* 	m_pGanateChartWgt;

};

#endif // CGANATEINFOUIPLUGINACTIVATOR_H