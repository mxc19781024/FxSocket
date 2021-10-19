/**************************************************************************************************
* @file		: CGanateChartUiPluginActivator.h
* @note		: ����ͼ������������
* @author	: w00068
* @date		: 2020-02-15
**************************************************************************************************/
#ifndef CGANATECHARTUIPLUGINACTIVATOR_H
#define CGANATECHARTUIPLUGINACTIVATOR_H

#include <QObject>
#include "ctkPluginActivator.h"
#include <FxGanttChartWidget.h>

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 ��������
#pragma execution_character_set("utf-8")
#endif

/**
* @class CGanateChartUiPluginActivator
* @brief װ������������������
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