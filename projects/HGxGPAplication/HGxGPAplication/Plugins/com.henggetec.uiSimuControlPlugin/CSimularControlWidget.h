/**************************************************************************************************
* @file CSimuCtrlWidget.h
* @note 简要介绍
* @author g00068
* @date 2019-3-14
**************************************************************************************************/
#ifndef CSIMU_CTRL_WIDGET_H
#define CSIMU_CTRL_WIDGET_H

#include <ctkPluginContext.h>
#include <FxBaseWindow/CFxBaseWindow.h>
#include <QTimer>
#include "service/event/ctkEventHandler.h"
#include "ui_CSimularControlWidget.h"

class CBizSimuCtrlWidget : public QWidget, public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
	CBizSimuCtrlWidget(ctkPluginContext *context, QWidget *parent = nullptr);
	~CBizSimuCtrlWidget();
signals:
	void simuControlShowSignal();
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
		void CloseBtnClickedSlot();
		void simuControlShowSlot();
protected:
	/**
	* @brief	事件处理
	* @param	event [in] 事件
	*/
	virtual void handleEvent(const ctkEvent& event);
private:
	Ui::SimuControlWidget* ui;
	QTimer m_timerSimuTime;
	QTimer m_timerUTC;
	QTimer m_timerRatio;
	
	double m_dTimeSimuTime;
	double m_dTimeUTC;
	double m_dTimeRatio;
	bool   m_bSimuState;
};

#endif //CSIMU_CTRL_WIDGET_H
