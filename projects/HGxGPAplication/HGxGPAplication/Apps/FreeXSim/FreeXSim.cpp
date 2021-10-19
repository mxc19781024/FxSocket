#include "FreeXSim.h"
#include "SimTimer.h"

FreeXSim::FreeXSim(QWidget *parent)
	: QWidget(parent)
	,m_dSimTimeLen(0)
	,m_pRadarSim(NULL)
	,m_pSimData(NULL)
{
	ui.setupUi(this);

	this->setWindowTitle(QString::fromLocal8Bit("仿真端"));

	//生成数据
	connect(ui.genrateBtn,SIGNAL(clicked()),this,SLOT(generDataSlot()));
	
	//开始仿真
	connect(ui.startSimBtn,SIGNAL(clicked()),this,SLOT(simDataSlot()));

	//减速
	connect(ui.descSpeedBtn,SIGNAL(clicked()),this,SLOT(desSpeedSlot()));

	//加速
	connect(ui.increSpeedBtn,SIGNAL(clicked()),this,SLOT(increSpeedSlot()));

	//停止
	connect(ui.stopBtn,SIGNAL(clicked()),this,SLOT(stopSimSlot()));

	//暂停
	connect(ui.pauseBtn,SIGNAL(clicked()),this,SLOT(pasueSlot()));

#ifndef _DEBUG
	ui.genrateBtn->hide();
#endif

}

FreeXSim::~FreeXSim()
{
	if (m_pRadarSim != NULL)
	{
		delete m_pRadarSim;
		m_pRadarSim = NULL;
	}
}

void FreeXSim::stopSimSlot()
{
	if (m_pRadarSim)
	{
		m_pRadarSim->Stop();
	}

	if (m_pSimData)
	{
		m_pSimData->Stop();
	}
}

void FreeXSim::pasueSlot()
{
	if (m_pSimData)
	{
		m_pSimData->SetPause(!m_pSimData->GetPause());
		QString str = m_pSimData->GetPause() ? QString::fromLocal8Bit("继续") : QString::fromLocal8Bit("暂停");
		ui.pauseBtn->setText(str);
	}
}

void FreeXSim::setCurSpeedLbl()
{
	QString strText;
	if (SimTimer::getInstance()->getTimeFactor() > 1)
	{
		//
		int nSpeed = SimTimer::getInstance()->getTimeFactor();
		strText = QString::fromLocal8Bit("当前倍速：1/%1").arg(nSpeed);
	}
	else
	{
		int nSpeed = 1 / SimTimer::getInstance()->getTimeFactor();
		strText = QString::fromLocal8Bit("当前倍速：%1").arg(nSpeed);
	}

	ui.curBsLbl->setText(strText);

}

void FreeXSim::setSceneNameSlot(QString strText)
{
	// 
	ui.sceneLbl->setText(strText);
}

void FreeXSim::generDataSlot()
{
	if (m_pRadarSim == NULL)
	{
		m_pRadarSim = new RadarGenSim;

		connect(m_pRadarSim,SIGNAL(SimTimeLenSignal(double)),this,SLOT(timeLenSlot(double )),Qt::QueuedConnection);
		connect(m_pRadarSim,SIGNAL(CurTimeSignal(double)),this,SLOT(curTimeSlot(double )),Qt::QueuedConnection);
	}
	m_pRadarSim->Start();
}

void FreeXSim::simDataSlot()
{

	if (m_pSimData == NULL)
	{
		m_pSimData = new SimData;

		connect(m_pSimData,SIGNAL(SimTimeLenSignal(double)),this,SLOT(timeLenSlot(double )),Qt::QueuedConnection);
		connect(m_pSimData,SIGNAL(CurTimeSignal(double)),this,SLOT(curTimeSlot(double )),Qt::QueuedConnection);
		connect(m_pSimData,SIGNAL(CurSceneNameSignal(QString)),this,SLOT(setSceneNameSlot(QString )),Qt::QueuedConnection);
	}

	if (m_pSimData->isRunning())
	{
		return;
	}

	m_pSimData->Start();

	// 设置开始继续状态
	QString str = m_pSimData->GetPause() ? QString::fromLocal8Bit("继续") : QString::fromLocal8Bit("暂停");
	ui.pauseBtn->setText(str);
	
	SimTimer::getInstance()->setTimeFactor(1);
	setCurSpeedLbl();
	ui.curSimPrbar->setValue(0);
}

void FreeXSim::timeLenSlot(double dTimeLen)
{
	m_dSimTimeLen = dTimeLen;
}

void FreeXSim::curTimeSlot(double dCurTime)
{
	if (m_dSimTimeLen < 0.00000001)
	{
		return;
	}

	//进度
	double dProgress = dCurTime / m_dSimTimeLen;
	// 持续时间

	// 剩余时间
	double dSyTime = m_dSimTimeLen - dCurTime;

	if (dSyTime < 0)
	{
		dSyTime = 0;
	}
	ui.curSimPrbar->setValue(dProgress * 100);
	ui.elapseTimeLbl->setText(SimTimer::secondToStrHMS(dCurTime));
	ui.residTimeLbl->setText(SimTimer::secondToStrHMS(dSyTime));
}
//sleep（X） X * 0.5   响应加快  
void FreeXSim::increSpeedSlot()
{
	if (m_pSimData)
	{
		double dCurFactor = SimTimer::getInstance()->getTimeFactor();
		if (dCurFactor > 1/64.0)
		{
			m_pSimData->SetTimeFactor(dCurFactor * 0.5);
		}
		setCurSpeedLbl();
	}
}

//sleep（X） X * 2   响应变慢 
void FreeXSim::desSpeedSlot()
{
	if (m_pSimData)
	{
		double dCurFactor = SimTimer::getInstance()->getTimeFactor();
		if (dCurFactor < 64)
		{
			m_pSimData->SetTimeFactor(dCurFactor * 2);
		}
		setCurSpeedLbl();
	}

}