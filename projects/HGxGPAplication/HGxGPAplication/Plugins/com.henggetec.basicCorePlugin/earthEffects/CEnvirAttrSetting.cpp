//#include <osgEarth/DateTime>
//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXFramework/IFxSimulationTimer.h>
//#include <scene/IFxSceneService.h>
//#include <earth_environment/IFxEnvironmentManager.h>
#include <math.h>
#include "CSpecialEffectsControl.h"
#include "CEnvirAttrSetting.h"

#include <gdpTool/environment/GxEnvirementCom.h>
#include <gdpTool/environment/GxWeatherOption.h>
#include <gdpTool/environment/GxOceanOption.h>
#include <gdpTool/environment/GxSkyOption.h>

CEnvirAttrSetting::CEnvirAttrSetting(QWidget *parent)
	: CFxBaseWindow(parent)
	, m_pSimuTimer(NULL)
	, m_pSysTimer(NULL)
	, m_iListIndex(0)
{
	ui.setupUi(m_pCentralWgt);

	m_pEffectControl = qobject_cast<CSpecialEffectsControl*>(parent);

	setWindowTitle(tr("environment setting"));
	setWindowModality(Qt::NonModal);
	initWgt();
	initConnection();

	resize(300, 850);
}

CEnvirAttrSetting::~CEnvirAttrSetting()
{

}

void CEnvirAttrSetting::initWgt()
{
	//l180 添加单位
	ui.YunTuSpinBox->setSuffix("km");
	ui.RainHeightSpinBox->setSuffix("m");
	ui.SnowHeightSpinBox->setSuffix("m");

	//l180 添加单位
	ui.OceanHeightSpinBox->setSuffix("m");
	ui.WindVecSpinBox->setSuffix("m/s");
	ui.WindDirSpinBox->setSuffix(QChar(0x00b0)); // °
	m_lstSimuVel.clear();

	//添加可用的仿真速率
	m_lstSimuVel << DEFAULT_SIMUSPEED << 0.1 << 0.5 << 3.0 << 5.0 << 10.0 << 30.0 << 60.0
		<< 180.0 << 300.0 << 600.0 << 1800.0 << 3600.0 << 7200.0;
	sortList();

	//初始化仿真及系统计时器
	resetSlot();
	m_pSysTimer = new QTimer(this);
	m_pSysTimer->setInterval(DEFAULT_INTERVAL);
	connect(m_pSysTimer, SIGNAL(timeout()), this, SLOT(sysTimerTimeOutSlot()));
}

void CEnvirAttrSetting::initConnection()
{
	//connect(ui.SlowDownPushBtn, SIGNAL(clicked()), this, SLOT(slowDownSlot()));
	//connect(ui.SpeedUpPushBtn, SIGNAL(clicked()), this, SLOT(speedUpSlot()));
	//connect(ui.ResetPushBtn, SIGNAL(clicked()), this, SLOT(resetSlot()));
	connect(ui.SunLightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxChangedSlot(int)));
	connect(ui.SunLightSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChangedSlot(int)));

	connect(ui.OceanHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxOceanHeightSlot(int)));
	connect(ui.OceanHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(spinBoxOceanHeightSlot(int)));
	connect(ui.WindVecSpinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxWindVecSlot(int)));
	connect(ui.WindVecSlider, SIGNAL(valueChanged(int)), this, SLOT(spinBoxWindVecSlot(int)));
	connect(ui.WindDirSpinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxWindDirSlot(int)));
	connect(ui.WindDirSlider, SIGNAL(valueChanged(int)), this, SLOT(spinBoxWindDirSlot(int)));

	connect(ui.RainConcentrationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxRainConcentrationSlot(double)));
	connect(ui.RainConcentrationSlider, SIGNAL(valueChanged(int)), this, SLOT(spinBoxRainConcentrationSlot(int)));

	connect(ui.RainHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxRainHeightSlot(int)));
	connect(ui.RainHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(spinBoxRainHeightSlot(int)));

	connect(ui.SnowConcentrationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxSnowConcentrationSlot(double)));
	connect(ui.SnowConcentrationSlider, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSnowConcentrationSlot(int)));

	connect(ui.SnowHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSnowHeightSlot(int)));
	connect(ui.SnowHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSnowHeightSlot(int)));

	connect(ui.WuHuaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxWuHuaSlot(double)));
	connect(ui.WuHuaSlider, SIGNAL(valueChanged(int)), this, SLOT(spinBoxWuHuaSlot(int)));

	connect(ui.YunTuSpinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxYunTuSlot(int)));
	connect(ui.YunTuSldier, SIGNAL(valueChanged(int)), this, SLOT(sliderYunTuSlot(int)));

}

void CEnvirAttrSetting::sortList()
{
	//列表重排
	for(int i = 0; i < m_lstSimuVel.count(); i++)
	{
		for(int j = i+1; j < m_lstSimuVel.count(); j++)
		{
			//去重
			if(m_lstSimuVel.at(j) == m_lstSimuVel.at(i))
			{
				m_lstSimuVel.takeAt(j);
				i--;
				j--;
			}
			//排序
			else if(m_lstSimuVel.at(j) < m_lstSimuVel.at(i))
				m_lstSimuVel.swap(i,j);
		}
	}
}

bool CEnvirAttrSetting::resetCurrentIndex()
{
	/*ui.SlowDownPushBtn->setEnabled(false);
	ui.SpeedUpPushBtn->setEnabled(false);
	for(int i = 0; i < m_lstSimuVel.count(); i++)
	{
	if(m_lstSimuVel.at(i) == DEFAULT_SIMUSPEED)
	{
	m_iListIndex = i;

	if(m_iListIndex > 0)
	ui.SlowDownPushBtn->setEnabled(true);
	if(m_iListIndex < m_lstSimuVel.count() - 1)
	ui.SpeedUpPushBtn->setEnabled(true);

	return true;
	}
	}*/
	return false;
}

void CEnvirAttrSetting::simuTimerTimeOutSlot()
{
	// 	//计算当前仿真时间
	//  	qreal rInter = m_pSimuTimer->interval();
	//  	qreal rThreoInter = DEFAULT_INTERVAL / m_lstSimuVel.at(m_iListIndex);
	// 
	//  	if(DEFAULT_INTERVAL / m_lstSimuVel.at(m_iListIndex) <= MINI_INTERVAL)
	//  		m_timeSimuTime = m_timeSimuTime.addSecs(rInter / rThreoInter);
	//  	else
	//  		m_timeSimuTime = m_timeSimuTime.addSecs(1);
	// 	
	// 	int iYear = m_timeSimuTime.date().year();
	// 	int iMonth = m_timeSimuTime.date().month();
	// 	int iDay = m_timeSimuTime.date().day();
	// 	int iHour = m_timeSimuTime.time().hour();
	// 	int iMinu = m_timeSimuTime.time().minute();
	// 	int iSec = m_timeSimuTime.time().second();
	// 
	// 	//设置Earth时间
	// 	FxEarthEnv::IFxSkyEnvironment *pSkyNode = getSkyNode();
	// 	if (pSkyNode)
	// 	{
	// 		double dHours = (iHour * 3600 + iMinu * 60 + iSec) / 3600.0;
	// 		osgEarth::DateTime dateTime(iYear, iMonth, iDay, dHours);
	// 		pSkyNode->SetDateTime(dateTime);
	// 		//设置时间显示信息
	// 		ui.yearLineEdt->setText(QString::number(m_timeSimuTime.date().year()));
	// 		ui.monthLineEdt->setText(QString::number(m_timeSimuTime.date().month()));
	// 		ui.dayLineEdt->setText(QString::number(m_timeSimuTime.date().day()));
	// 		ui.hourLineEdt->setText(QString::number(m_timeSimuTime.time().hour()));
	// 		ui.minuLineEdt->setText(QString::number(m_timeSimuTime.time().minute()));
	// 		ui.secLineEdt->setText(QString::number(m_timeSimuTime.time().second()));
	// 	}
}

void CEnvirAttrSetting::sysTimerTimeOutSlot()
{
	//ui.SysTimeLineEdt->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm:ss"));
}

void CEnvirAttrSetting::resetSlot()
{
	//	//置为系统时间并恢复默认速率
	//	m_timeSimuTime = QDateTime::currentDateTime();
	////	if(m_pSimuTimer == NULL)
	////	{
	////		m_pSimuTimer = new QTimer(this);
	////		connect(m_pSimuTimer, SIGNAL(timeout()), this, SLOT(simuTimerTimeOutSlot()));
	////	}
	//
	//	/*设置当前列表索引为DEFAULT_SIMUSPEED的索引号,
	//	  失败则添加DEFAULT_SIMUSPEED并排序*/
	//	if(!resetCurrentIndex())
	//	{
	//		m_lstSimuVel << DEFAULT_SIMUSPEED;
	//		sortList();
	//		resetCurrentIndex();
	//	}
	////	m_pSimuTimer->setInterval(DEFAULT_INTERVAL / DEFAULT_SIMUSPEED);
	//	ui.vel_aLbl->setText(QString("x%1").arg(DEFAULT_SIMUSPEED));
	//	ui.vel_bLbl->setText(QString("x%1").arg(DEFAULT_SIMUSPEED));
	//
	//	// 调用底层接口将速率恢复为1.0
	//	FxSimulationTimerInstance()->setSimulateTimeRatio(1.0);
}

void CEnvirAttrSetting::speedUpSlot()
{
	//	if(m_iListIndex < m_lstSimuVel.count() - 1)
	//		m_iListIndex++;
	//	if(m_iListIndex == m_lstSimuVel.count() - 1)
	//		ui.SpeedUpPushBtn->setEnabled(false);
	//	if(m_iListIndex > 0)
	//		ui.SlowDownPushBtn->setEnabled(true);
	//
	//	//当前时间间隔大于最小时间间隔则设置
	//// 	if(DEFAULT_INTERVAL / m_lstSimuVel.at(m_iListIndex) > MINI_INTERVAL)
	//// 	{
	//// 		m_pSimuTimer->setInterval(DEFAULT_INTERVAL / m_lstSimuVel.at(m_iListIndex));
	//// 	}
	//
	//	// 调用底层接口设置仿真速率
	//	FxSimulationTimerInstance()->setSimulateTimeRatio(m_lstSimuVel.at(m_iListIndex));
	//
	//	ui.vel_aLbl->setText(QString("x%1").arg(m_lstSimuVel.at(m_iListIndex)));
	//	ui.vel_bLbl->setText(QString("x%1").arg(m_lstSimuVel.at(m_iListIndex)));
}

void CEnvirAttrSetting::slowDownSlot()
{
	//	if(m_iListIndex > 0)
	//		m_iListIndex--;
	//	if(m_iListIndex == 0)
	//		ui.SlowDownPushBtn->setEnabled(false);
	//	if(m_iListIndex < m_lstSimuVel.count() - 1)
	//		ui.SpeedUpPushBtn->setEnabled(true);
	//
	//	//当前时间间隔大于最小时间间隔则设置
	//// 	if(DEFAULT_INTERVAL / m_lstSimuVel.at(m_iListIndex) > MINI_INTERVAL)
	//// 	{
	//// 		m_pSimuTimer->setInterval(DEFAULT_INTERVAL / m_lstSimuVel.at(m_iListIndex));
	//// 	}
	//	// 调用底层接口设置仿真速率
	//	FxSimulationTimerInstance()->setSimulateTimeRatio(m_lstSimuVel.at(m_iListIndex));
	//
	//	ui.vel_aLbl->setText(QString("x%1").arg(m_lstSimuVel.at(m_iListIndex)));
	//	ui.vel_bLbl->setText(QString("x%1").arg(m_lstSimuVel.at(m_iListIndex)));
}

void CEnvirAttrSetting::sliderChangedSlot(int iValue)
{
	HGxTool::CGxSkyOption* pSkyOption = m_pEffectControl->getSkyOption();
	if (pSkyOption)
	{
		float fValue = iValue / 255.0;
		pSkyOption->dDensity = fValue;
		pSkyOption->updateEnvirementNode();
		emit envirAttrChangedSignal();
		ui.SunLightSpinBox->blockSignals(true);
		ui.SunLightSpinBox->setValue(iValue);
		ui.SunLightSpinBox->blockSignals(false);
	}
}

void CEnvirAttrSetting::spinBoxChangedSlot(int iValue)
{
	HGxTool::CGxSkyOption* pSkyOption = m_pEffectControl->getSkyOption();
	if (pSkyOption)
	{
		float fValue = iValue / 255.0;
		pSkyOption->dDensity = fValue;
		pSkyOption->updateEnvirementNode();
		emit envirAttrChangedSignal();
		ui.SunLightSlider->blockSignals(true);
		ui.SunLightSlider->setValue(iValue);
		ui.SunLightSlider->blockSignals(false);
	}
}
void CEnvirAttrSetting::spinBoxRainConcentrationSlot(int val)
{
	HGxTool::CGxWeatherOption* pWeatherOption = m_pEffectControl->getWeatherOption();
 	if (pWeatherOption != nullptr)
 	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (val / 100.0f != pWeatherOption->fRainIntensity)
		{
			pWeatherOption->fRainIntensity = val / 100.0f;
			pWeatherOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.RainConcentrationSpinBox->blockSignals(true);
	ui.RainConcentrationSlider->blockSignals(true);
	ui.RainConcentrationSpinBox->setValue(val/100.0f);
	ui.RainConcentrationSlider->setValue(val);
	ui.RainConcentrationSpinBox->blockSignals(false);
	ui.RainConcentrationSlider->blockSignals(false);

}
void CEnvirAttrSetting::spinBoxRainConcentrationSlot(double val)
{
	HGxTool::CGxWeatherOption* pWeatherOption = m_pEffectControl->getWeatherOption();
	if (pWeatherOption != nullptr)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (val != pWeatherOption->fRainIntensity)
		{
			pWeatherOption->fRainIntensity = val;
			pWeatherOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.RainConcentrationSpinBox->blockSignals(true);
	ui.RainConcentrationSlider->blockSignals(true);
	ui.RainConcentrationSpinBox->setValue(val);
	ui.RainConcentrationSlider->setValue(val*100);
	ui.RainConcentrationSpinBox->blockSignals(false);
	ui.RainConcentrationSlider->blockSignals(false);
}

void CEnvirAttrSetting::spinBoxRainHeightSlot(int val)
{
	HGxTool::CGxWeatherOption* pWeatherOption = m_pEffectControl->getWeatherOption();
	if (pWeatherOption != nullptr)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180

		if (val != pWeatherOption->fRainHeight)
		{
			pWeatherOption->fRainHeight = val;
			pWeatherOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.RainConcentrationSpinBox->blockSignals(true);
	ui.RainConcentrationSlider->blockSignals(true);
	ui.RainHeightSpinBox->setValue(val);
	ui.RainHeightSlider->setValue(val);
	ui.RainConcentrationSpinBox->blockSignals(false);
	ui.RainConcentrationSlider->blockSignals(false);
}

void CEnvirAttrSetting::spinBoxSnowConcentrationSlot(int val)
{
	HGxTool::CGxWeatherOption* pWeatherOption = m_pEffectControl->getWeatherOption();
	if (pWeatherOption != nullptr)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (val / 100.0 != pWeatherOption->fSnowIntensity)
		{
			pWeatherOption->fSnowIntensity = val / 100.0f;
			pWeatherOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.SnowConcentrationSpinBox->blockSignals(true);
	ui.SnowConcentrationSlider->blockSignals(true);
	ui.SnowConcentrationSpinBox->setValue(val/100.0);
	ui.SnowConcentrationSlider->setValue(val);
	ui.SnowConcentrationSpinBox->blockSignals(false);
	ui.SnowConcentrationSlider->blockSignals(false);
}
void CEnvirAttrSetting::spinBoxSnowConcentrationSlot(double val)
{
	HGxTool::CGxWeatherOption* pWeatherOption = m_pEffectControl->getWeatherOption();
	if (pWeatherOption != nullptr)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (val != pWeatherOption->fSnowIntensity)
		{
			pWeatherOption->fSnowIntensity = val;
			pWeatherOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.SnowConcentrationSpinBox->blockSignals(true);
	ui.SnowConcentrationSlider->blockSignals(true);
	ui.SnowConcentrationSpinBox->setValue(val);
	ui.SnowConcentrationSlider->setValue(val*100);
	ui.SnowConcentrationSpinBox->blockSignals(false);
	ui.SnowConcentrationSlider->blockSignals(false);

}

void CEnvirAttrSetting::spinBoxSnowHeightSlot(int val)
{
	HGxTool::CGxWeatherOption* pWeatherOption = m_pEffectControl->getWeatherOption();
	if (pWeatherOption != nullptr)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (val != pWeatherOption->fSnowHeight)
		{
			pWeatherOption->fSnowHeight = val;
			pWeatherOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.SnowHeightSpinBox->blockSignals(true);
	ui.SnowHeightSlider->blockSignals(true);
	ui.SnowHeightSpinBox->setValue(val);
	ui.SnowHeightSlider->setValue(val);
	ui.SnowHeightSpinBox->blockSignals(false);
	ui.SnowHeightSlider->blockSignals(false);
}

void CEnvirAttrSetting::spinBoxOceanHeightSlot(int height)
{
	HGxTool::CGxOceanOption* pOceanOption = m_pEffectControl->getOceanOption();
 
 	if (pOceanOption != nullptr)
 	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (height != pOceanOption->dSeaLevel)
		{
			pOceanOption->dSeaLevel = height;
			pOceanOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.OceanHeightSpinBox->blockSignals(true);
	ui.OceanHeightSlider->blockSignals(true);
	ui.OceanHeightSpinBox->setValue(height);
	ui.OceanHeightSlider->setValue(height);
	ui.OceanHeightSpinBox->blockSignals(false);
	ui.OceanHeightSlider->blockSignals(false);
}

void CEnvirAttrSetting::spinBoxWindVecSlot(int vec)
{
	HGxTool::CGxOceanOption* pOceanOption = m_pEffectControl->getOceanOption();

	if (pOceanOption != nullptr)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (vec != pOceanOption->dWindSpeed)
		{
			pOceanOption->dWindSpeed = vec;
			pOceanOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.WindVecSpinBox->blockSignals(true);
	ui.WindVecSlider->blockSignals(true);
	ui.WindVecSpinBox->setValue(vec);
	ui.WindVecSlider->setValue(vec);
	ui.WindVecSpinBox->blockSignals(false);
	ui.WindVecSlider->blockSignals(false);
}

void CEnvirAttrSetting::spinBoxWindDirSlot(int dir)
{
	HGxTool::CGxOceanOption* pOceanOption = m_pEffectControl->getOceanOption();

	if (pOceanOption != nullptr)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (dir != pOceanOption->dWindDirection)
		{
			pOceanOption->dWindDirection = dir;
			pOceanOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.WindDirSpinBox->blockSignals(true);
	ui.WindDirSlider->blockSignals(true);
	ui.WindDirSpinBox->setValue(dir);
	ui.WindDirSlider->setValue(dir);
	ui.WindDirSpinBox->blockSignals(false);
	ui.WindDirSlider->blockSignals(false);
}

void CEnvirAttrSetting::spinBoxWuHuaSlot(int val)
{
	HGxTool::CGxWeatherOption* pWeatherOption = m_pEffectControl->getWeatherOption();
	if (pWeatherOption != nullptr)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
  		if(val/100.0 != pWeatherOption->fFogIntensity)
  		{
			pWeatherOption->fFogIntensity = val / 100.0;
			pWeatherOption->updateEnvirementNode();
  			emit envirAttrChangedSignal();
  		}
 	}

	ui.WuHuaSlider->blockSignals(true);
	ui.WuHuaSpinBox->blockSignals(true);
	ui.WuHuaSlider->setValue(val);
	ui.WuHuaSpinBox->setValue(val/100.0);
	ui.WuHuaSlider->blockSignals(false);
	ui.WuHuaSpinBox->blockSignals(false);
}
void CEnvirAttrSetting::spinBoxWuHuaSlot(double val)
{
	HGxTool::CGxWeatherOption* pWeatherOption = m_pEffectControl->getWeatherOption();
	if (pWeatherOption != nullptr)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (val != pWeatherOption->fFogIntensity)
		{
			pWeatherOption->fFogIntensity = val;
			pWeatherOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.WuHuaSlider->blockSignals(true);
	ui.WuHuaSpinBox->blockSignals(true);
	ui.WuHuaSlider->setValue(val*100);
	ui.WuHuaSpinBox->setValue(val);
	ui.WuHuaSlider->blockSignals(false);
	ui.WuHuaSpinBox->blockSignals(false);
}
void CEnvirAttrSetting::spinBoxYunTuSlot(int val)
{

	HGxTool::CGxSkyOption* pSkyOption = m_pEffectControl->getSkyOption();
	if (pSkyOption)
	{
		//值没有真正的改变则不行新设置，不发送改变的信号 l180
		if (val * 1000 != pSkyOption->dCloudMinDis)
		{
			pSkyOption->dCloudMinDis = val * 1000;
			pSkyOption->updateEnvirementNode();
			emit envirAttrChangedSignal();
		}
 	}

	ui.YunTuSldier->blockSignals(true);
	ui.YunTuSpinBox->blockSignals(true);
	ui.YunTuSldier->setValue(val);
	ui.YunTuSpinBox->setValue(val);
	ui.YunTuSldier->blockSignals(false);
	ui.YunTuSpinBox->blockSignals(false);
}

void CEnvirAttrSetting::sliderYunTuSlot(int val)
{
	HGxTool::CGxSkyOption* pSkyOption = m_pEffectControl->getSkyOption();
	if (pSkyOption)
	{
 		//值没有真正的改变则不行新设置，不发送改变的信号 l180
  		if(val*1000 != pSkyOption->dCloudMinDis)
  		{
 			pSkyOption->dCloudMinDis = val * 1000;
			pSkyOption->updateEnvirementNode();
  			emit envirAttrChangedSignal();
  		}	
 	}

	ui.YunTuSldier->blockSignals(true);
	ui.YunTuSpinBox->blockSignals(true);
	ui.YunTuSldier->setValue(val);
	ui.YunTuSpinBox->setValue(val);
	ui.YunTuSldier->blockSignals(false);
	ui.YunTuSpinBox->blockSignals(false);
}

void CEnvirAttrSetting::startTimer( bool bStart )
{
	if(bStart)
	{
		resetSlot();
		m_pSysTimer->start();
		//m_pSimuTimer->start();
	}
	else
	{
		//m_pSimuTimer->stop();
		m_pSysTimer->stop();
	}
}

void CEnvirAttrSetting::setSunLightIntensity( const float fIntensity )
{
	int nValue = fIntensity * 255.0 + 0.5;

	HGxTool::CGxSkyOption* pSkyOption = m_pEffectControl->getSkyOption();
	if (pSkyOption)
	{
		float fValue = nValue / 255.0;
		pSkyOption->dDensity = fValue;
		pSkyOption->updateEnvirementNode();
	}

	ui.SunLightSpinBox->blockSignals(true);
	ui.SunLightSlider->blockSignals(true);
	ui.SunLightSpinBox->setValue(nValue);
	ui.SunLightSlider->setValue(nValue);
	ui.SunLightSpinBox->blockSignals(false);
	ui.SunLightSlider->blockSignals(false);
}
