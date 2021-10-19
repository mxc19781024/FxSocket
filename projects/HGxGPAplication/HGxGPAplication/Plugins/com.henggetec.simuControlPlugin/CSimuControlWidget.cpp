#include <CSimuControlWidget.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <service/event/ctkEventConstants.h>
#include <simu_control_ui/ISimuControlUiService.h>

#include "bdpUtil/GxTimeManager.h"

CSimuControlWidget::CSimuControlWidget(QWidget *parent)
	: QWidget(parent)
	,m_dTimeSimuTime(100.0)
	,m_dTimeUTC(1000.0)
	,m_dTimeRatio(100.0)
{
	ui.setupUi(this);

	connect(ui.StartPushBtn, SIGNAL(clicked()), this, SLOT(startPushBtnClickedSlot()));
	connect(ui.StopPushBtn, SIGNAL(clicked()), this, SLOT(stopPushBtnClickedSlot()));
	connect(ui.ReversePushBtn, SIGNAL(clicked()), this, SLOT(reversePushBtnClickedSlot()));
	connect(ui.AccPushBtn, SIGNAL(clicked()), this, SLOT(accPushBtnClickedSlot()));
	connect(ui.DecPushBtn, SIGNAL(clicked()), this, SLOT(decPushBtnClickedSlot()));
	ui.StartPushBtn->setToolTip(tr("start/pause"));
	ui.StopPushBtn->setToolTip(tr("stop"));
	ui.ReversePushBtn->setToolTip(tr("forward direction/reverse direction"));
	ui.AccPushBtn->setToolTip(tr("speed up"));
	ui.DecPushBtn->setToolTip(tr("speed down"));
	//添加倍率单位 l180
	ui.RatioSpx->setPrefix("x");
	connect(ui.RatioSpx, SIGNAL(valueChanged(double)), this, SLOT(ratioValueChangedSlot(double)));

	connect(ui.SimuDtEdt, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(simuValueChangedSlot(const QDateTime &)));

	connect(ui.TimeSlider, SIGNAL(sliderPressed()), this, SLOT(sliderPressedSlot()));
	connect(ui.TimeSlider, SIGNAL(sliderReleased()), this, SLOT(sliderReleasedSlot()));
	connect(ui.TimeSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChangedSlot(int)));

 	connect(&m_timerSimuTime, SIGNAL(timeout()), this, SLOT(updateSimuTimeSlot()));
 	connect(&m_timerUTC, SIGNAL(timeout()), this, SLOT(updateUTCTimeSlot()));
 	
 	connect(&m_timerRatio, SIGNAL(timeout()), this, SLOT(updateRatioSlot()));
	
 	m_timerSimuTime.start(m_dTimeSimuTime);
	m_timerUTC.start(m_dTimeUTC);
 	m_timerRatio.start(m_dTimeRatio);

	//初始状态设置为开启 l180
	ui.StartPushBtn->setChecked(true);

 	//加载仿真时间是恢复仿真时间默认值 l180
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->stop();

	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->start();

	//读样式表文件
	QFile file(":/qss/Resources/qss/freex_simu_control_ui.css");
	if (file.open(QIODevice::ReadOnly))
	{
		QString str = file.readAll();
		setStyleSheet(str);
		file.close();
	}
}

CSimuControlWidget::~CSimuControlWidget()
{
 	//该插件卸载的时候会停止计时
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->stop();
}

void CSimuControlWidget::startPushBtnClickedSlot()
{
 	if (ui.StartPushBtn->isChecked())
 	{
		HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->start();
 	}
 	else
 	{
		HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->pause();
 	}
}

void CSimuControlWidget::stopPushBtnClickedSlot()
{
	if (ui.StartPushBtn->isChecked())
	{
		ui.StartPushBtn->click();
	}
	if (ui.ReversePushBtn->isChecked())
	{
		ui.ReversePushBtn->click();
	}

 	updateSimuTimeSlot();
 	updateRatioSlot();

	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->stop();
}

void CSimuControlWidget::reversePushBtnClickedSlot()
{
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->reverse();
}

void CSimuControlWidget::accPushBtnClickedSlot()
{
 	double dCurSimuTimeRatio = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeRatio();
 	dCurSimuTimeRatio *= 2.0;
 	if (dCurSimuTimeRatio>ui.RatioSpx->maximum())
 	{		
		dCurSimuTimeRatio = ui.RatioSpx->maximum();
 	}
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeRatio(dCurSimuTimeRatio);
}

void CSimuControlWidget::decPushBtnClickedSlot()
{
 	double dCurSimuTimeRatio = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeRatio();
 	dCurSimuTimeRatio *= 0.5;
 	if (dCurSimuTimeRatio<ui.RatioSpx->minimum())
 	{
 		dCurSimuTimeRatio = ui.RatioSpx->minimum();
 	}
 	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeRatio(dCurSimuTimeRatio);
}

void CSimuControlWidget::updateSimuTimeSlot()
{
 	double dJD = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent();
 	double dSec;
 	int nYear, nMonth, nDay, nHour, nMin;
 	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->jdToCal(dJD, nYear, nMonth, nDay, nHour, nMin, dSec);
	QDateTime dateTime;
	dateTime.setDate(QDate(nYear,nMonth,nDay));
 	dateTime.setTime(QTime(nHour,nMin,dSec));
 	if (!ui.SimuDtEdt->hasFocus())
 	{
 		ui.SimuDtEdt->blockSignals(true);
 		ui.SimuDtEdt->setDateTime(dateTime);
 		ui.SimuDtEdt->blockSignals(false);
	}
 	ui.TimeSlider->blockSignals(true);
 	ui.TimeSlider->setValue(dateTime.time().hour()*3600.0 + dateTime.time().minute()*60.0 + dateTime.time().second());
 	ui.TimeSlider->blockSignals(false);
}

void CSimuControlWidget::updateUTCTimeSlot()
{
	//qDebug() << QDateTime::currentDateTimeUtc().toString(Qt::SystemLocaleDate);
	ui.UTCTimeLabel->setText(QDateTime::currentDateTimeUtc().toString("yyyy/M/d H:mm:ss"));
}

void CSimuControlWidget::sliderPressedSlot()
{
 	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->enableClockTimer(false);
 	m_timerSimuTime.stop();
 	m_timerRatio.stop();
}

void CSimuControlWidget::sliderReleasedSlot()
{
 	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->enableClockTimer(true);
 	m_timerSimuTime.start(m_dTimeSimuTime);
 	m_timerRatio.start(m_dTimeRatio);
}

void CSimuControlWidget::sliderValueChangedSlot(int nValue)
{
 	double dJD = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent();
 	double dSec;
 	int nYear, nMonth, nDay, nHour, nMin;
 	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->jdToCal(dJD, nYear, nMonth, nDay, nHour, nMin, dSec);
 	nHour = nValue/3600.0;
 	nMin = (nValue - nHour*3600)/60.0;
	dSec = nValue - nHour*3600 - nMin*60.0;
 	QDateTime dateTime;
 	dateTime.setDate(QDate(nYear,nMonth,nDay));
 	dateTime.setTime(QTime(nHour,nMin,dSec));
 	ui.SimuDtEdt->blockSignals(true);
 	ui.SimuDtEdt->setDateTime(dateTime);
 	ui.SimuDtEdt->blockSignals(false);
	simuValueChangedSlot(dateTime);
}

void CSimuControlWidget::ratioValueChangedSlot( double dValue )
{
	if (dValue<ui.RatioSpx->minimum())
		dValue = ui.RatioSpx->minimum();
	else if (dValue>ui.RatioSpx->maximum())
		dValue = ui.RatioSpx->maximum();
	
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeRatio(dValue);
}

void CSimuControlWidget::updateRatioSlot()
{
	if (!ui.RatioSpx->hasFocus())
	{
		ui.RatioSpx->blockSignals(true);
		ui.RatioSpx->setValue(HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeRatio());
		ui.RatioSpx->blockSignals(false);
	}
}

void CSimuControlWidget::simuValueChangedSlot( const QDateTime & dateTime )
{
	double dJD = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->calToJD(dateTime.date().year(), dateTime.date().month(), dateTime.date().day()
		, dateTime.time().hour(), dateTime.time().minute(), dateTime.time().second());
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeCurrent(dJD);
}
