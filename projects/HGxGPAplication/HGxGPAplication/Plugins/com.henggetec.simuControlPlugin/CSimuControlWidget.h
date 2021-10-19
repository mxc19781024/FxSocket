/**************************************************************************************************
* @file CSimuControlWidget.h
* @note 简要介绍
* @author g00068
* @date 2019-3-14
**************************************************************************************************/
#ifndef CSIMU_CONTROL_WIDGET_H
#define CSIMU_CONTROL_WIDGET_H
#include "ui_CSimuControlWidget.h"
//#include <FreeXFramework/SimulationTimer.h>

#include "bdpUtil/GxSimulationTimer.h"

#include <QTimer>

namespace HGxUtil
{
	class CTimer;
	class CGxSimulationTimer;
	class CGxTimeManager;
}

class CSimuControlWidget : public QWidget
{
	Q_OBJECT

public:
	CSimuControlWidget(QWidget *parent = nullptr);
	~CSimuControlWidget();

	private slots:
		void startPushBtnClickedSlot();
		void stopPushBtnClickedSlot();
		void reversePushBtnClickedSlot();
		void accPushBtnClickedSlot();
		void decPushBtnClickedSlot();
		void updateSimuTimeSlot();
		void updateUTCTimeSlot();
		void updateRatioSlot();
		void sliderPressedSlot();
		void sliderReleasedSlot();
		void sliderValueChangedSlot(int nValue);
		void ratioValueChangedSlot(double dValue);
		void simuValueChangedSlot(const QDateTime & dateTime);

private:
	Ui::SimuControlWidget ui;

	QTimer m_timerSimuTime;
	QTimer m_timerUTC;
	QTimer m_timerRatio;
	
	double m_dTimeSimuTime;
	double m_dTimeUTC;
	double m_dTimeRatio;
};

#endif //CSIMU_CONTROL_WIDGET_H
