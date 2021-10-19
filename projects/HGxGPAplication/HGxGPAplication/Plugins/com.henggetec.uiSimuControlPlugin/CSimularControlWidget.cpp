#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <service/event/ctkEventConstants.h>
#include <CSimularControlWidget.h>
#include <simu_control_ui/ISimuControlUiService.h>
#include "GxEventHelperInterface.h"
#include "GxPluginFwLauncherInterface.h"
#include "GxPluginFrameworkCore.h"
#include "bdpUtil/GxTimeManager.h"

CBizSimuCtrlWidget::CBizSimuCtrlWidget(ctkPluginContext *context, QWidget *parent)
	:QWidget(parent)
	,m_dTimeSimuTime(100.0)
	,m_dTimeUTC(1000.0)
	,m_dTimeRatio(100.0)
	,m_bSimuState(true)
	,ui(new Ui::SimuControlWidget)
{
	ui->setupUi(this);

	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/LoginFinished", HGxFWLauncherInstance.getPluginContext());

	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);
	//setWindowOpacity(0.1);
	//setStyleSheet("background: transparent;");
	connect(ui->CloseBtn, SIGNAL(clicked()), this, SLOT(CloseBtnClickedSlot()));
	connect(ui->StartPushBtn, SIGNAL(clicked()), this, SLOT(startPushBtnClickedSlot()));
	connect(ui->StopPushBtn, SIGNAL(clicked()), this, SLOT(stopPushBtnClickedSlot()));
	connect(ui->ReversePushBtn, SIGNAL(clicked()), this, SLOT(reversePushBtnClickedSlot()));
	connect(ui->AccPushBtn, SIGNAL(clicked()), this, SLOT(accPushBtnClickedSlot()));
	connect(ui->DecPushBtn, SIGNAL(clicked()), this, SLOT(decPushBtnClickedSlot()));
	//connect(ui->pBtnModeState)
	ui->StartPushBtn->setToolTip(tr("开始/暂停"));
	ui->StopPushBtn->setToolTip(tr("重置"));
	ui->ReversePushBtn->setToolTip(tr("正向/反向"));
	ui->AccPushBtn->setToolTip(tr("加速"));
	ui->DecPushBtn->setToolTip(tr("减速"));
	//添加倍率单位 l180
	ui->RatioSpx->setPrefix("x");
	connect(ui->RatioSpx, SIGNAL(valueChanged(double)), this, SLOT(ratioValueChangedSlot(double)));

	connect(ui->SimuDtEdt, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(simuValueChangedSlot(const QDateTime &)));

	connect(ui->TimeSlider, SIGNAL(sliderPressed()), this, SLOT(sliderPressedSlot()));
	connect(ui->TimeSlider, SIGNAL(sliderReleased()), this, SLOT(sliderReleasedSlot()));
	connect(ui->TimeSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChangedSlot(int)));
	connect(&m_timerSimuTime, SIGNAL(timeout()), this, SLOT(updateSimuTimeSlot()));
	//connect(&m_timerUTC, SIGNAL(timeoutSignal()), this, SLOT(updateUTCTimeSlot()));

	connect(&m_timerRatio, SIGNAL(timeout()), this, SLOT(updateRatioSlot()));
	connect(this, SIGNAL(simuControlShowSignal()), this, SLOT(simuControlShowSlot()));
	m_timerSimuTime.start(m_dTimeSimuTime);
	m_timerUTC.start(m_dTimeUTC);
	m_timerRatio.start(m_dTimeRatio);

	//初始状态设置为开启 l180
	ui->StartPushBtn->setChecked(true);
	//加载仿真时间是恢复仿真时间默认值 l180
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->stop();

	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->start();

	QFile file(":/Resources/qss/freex_biz_simu_control_ui.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = styleSheet() + file.readAll();
		setStyleSheet(str);
	}

	ui->scrollArea->setStyleSheet("background-color: transparent;");
	ui->scrollArea->viewport()->setStyleSheet("background: transparent;");
	ui->scrollAreaWidgetContents->setStyleSheet("background: transparent;");
	ui->scrollArea->setFrameShape(QFrame::NoFrame);
	ui->CloseBtn->hide();
}

CBizSimuCtrlWidget::~CBizSimuCtrlWidget()
{
	//该插件卸载的时候会停止计时
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->stop();
}

void CBizSimuCtrlWidget::startPushBtnClickedSlot()
{
	if (ui->StartPushBtn->isChecked())
	{
		HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->start();
	}
	else
	{
		HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->pause();
	}
}

void CBizSimuCtrlWidget::stopPushBtnClickedSlot()
{
	if (ui->StartPushBtn->isChecked())
	{
		ui->StartPushBtn->click();
	}
	if (ui->ReversePushBtn->isChecked())
	{
		ui->ReversePushBtn->click();
	}
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->stop();
	updateSimuTimeSlot();
	updateRatioSlot();
}

void CBizSimuCtrlWidget::reversePushBtnClickedSlot()
{
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->reverse();
}

void CBizSimuCtrlWidget::accPushBtnClickedSlot()
{
	double dCurSimuTimeRatio = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeRatio();
	dCurSimuTimeRatio *= 2.0;
	if (dCurSimuTimeRatio>ui->RatioSpx->maximum())
	{
		dCurSimuTimeRatio = ui->RatioSpx->maximum();
	}
	ui->RatioSpx->setValue(dCurSimuTimeRatio);
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeRatio(dCurSimuTimeRatio);
}

void CBizSimuCtrlWidget::decPushBtnClickedSlot()
{
	double dCurSimuTimeRatio = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeRatio();
	dCurSimuTimeRatio *= 0.5;
	if (dCurSimuTimeRatio<ui->RatioSpx->minimum())
	{
		dCurSimuTimeRatio = ui->RatioSpx->minimum();
	}
	ui->RatioSpx->setValue(dCurSimuTimeRatio);
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeRatio(dCurSimuTimeRatio);
}

void CBizSimuCtrlWidget::updateSimuTimeSlot()
{
	double dJD = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent();
	double dSec;
	int nYear, nMonth, nDay, nHour, nMin;
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->jdToCal(dJD, nYear, nMonth, nDay, nHour, nMin, dSec);
	QDateTime dateTime;
	dateTime.setDate(QDate(nYear,nMonth,nDay));
	dateTime.setTime(QTime(nHour,nMin,dSec));
	if (!ui->SimuDtEdt->hasFocus())
	{
		ui->SimuDtEdt->blockSignals(true);
		ui->SimuDtEdt->setDateTime(dateTime);
		ui->SimuDtEdt->blockSignals(false);
	}
	ui->TimeSlider->blockSignals(true);
	ui->TimeSlider->setValue(dateTime.time().hour()*3600.0 + dateTime.time().minute()*60.0 + dateTime.time().second());
	ui->TimeSlider->blockSignals(false);

}

void CBizSimuCtrlWidget::updateUTCTimeSlot()
{
	//ui->UTCTimeLabel->setText(QDateTime::currentDateTimeUtc().toString(Qt::SystemLocaleDate));
}


void CBizSimuCtrlWidget::sliderPressedSlot()
{
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->enableClockTimer(false);
	m_timerSimuTime.stop();
	m_timerRatio.stop();
}

void CBizSimuCtrlWidget::sliderReleasedSlot()
{
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->enableClockTimer(true);
	m_timerSimuTime.start(m_dTimeSimuTime);
	m_timerRatio.start(m_dTimeRatio);
}

void CBizSimuCtrlWidget::sliderValueChangedSlot(int nValue)
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
	ui->SimuDtEdt->blockSignals(true);
	ui->SimuDtEdt->setDateTime(dateTime);
	ui->SimuDtEdt->blockSignals(false);
	simuValueChangedSlot(dateTime);
}

void CBizSimuCtrlWidget::ratioValueChangedSlot( double dValue )
{
	if (dValue<ui->RatioSpx->minimum())
		dValue = ui->RatioSpx->minimum();
	else if (dValue>ui->RatioSpx->maximum())
		dValue = ui->RatioSpx->maximum();
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeRatio(dValue);
}

void CBizSimuCtrlWidget::updateRatioSlot()
{
	if (!ui->RatioSpx->hasFocus())
	{
		ui->RatioSpx->blockSignals(true);
		ui->RatioSpx->setValue(HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeRatio());
		ui->RatioSpx->blockSignals(false);
	}
}

void CBizSimuCtrlWidget::simuValueChangedSlot( const QDateTime & dateTime )
{
	double dJD = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->calToJD(dateTime.date().year(), dateTime.date().month(), dateTime.date().day()
		, dateTime.time().hour(), dateTime.time().minute(), dateTime.time().second());
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeCurrent(dJD);
}

void CBizSimuCtrlWidget::CloseBtnClickedSlot()
{
	close();
}
void CBizSimuCtrlWidget::simuControlShowSlot()
{
	this->show();
}

void CBizSimuCtrlWidget::handleEvent(const ctkEvent& event)
{
	if (event.getTopic() == "event/LoginFinished")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit simuControlShowSignal();
	}
}
