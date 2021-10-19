#include "RadarGenSim.h"
#include "SimTimer.h"

#include "EntityObjSim.h"
#include "FreeXSimSceneParse.h"
#include "ConfigMgr.h"

RadarGenSim::RadarGenSim(QObject *parent)
	:QThread(parent)
	,m_bStop(false)
{
	// 读取想定文件生成对象
	SimTimer::getInstance()->setCurLogicTime(0);
	SimTimer::getInstance()->setDeriod(0.5);
	GenerSimObj();
}


RadarGenSim::~RadarGenSim(void)
{
	if (!isFinished())
	{
		Stop();
		wait();
		quit();
	}
}

void RadarGenSim::SimGenerData()
{
	EnityObjSimMrg::instance()->geneSimData();
}

void RadarGenSim::GenerSimObj()
{
	FreeXSimSceneData freexData;
	FreeXSimSceneParse parse(freexData);
	parse.ParseScneSim(FilePathMgr::getDataFileDir() + "SceneConfig.xml");

	emit SimTimeLenSignal(freexData.getSimInfo().dTimeLen);
	/// 
	std::map< int,std::vector<SimEntityObj> > & mapSimObj = freexData.getEntityObjs();

	for (std::map< int,std::vector<SimEntityObj> >::iterator itr = mapSimObj.begin();
		itr != mapSimObj.end(); ++itr)
	{
		for (int nIndex = 0; nIndex < itr->second.size(); ++nIndex)
		{
			EntityObjSim *pObjSim = new EntityObjSim;
			pObjSim->getSimEntity() = itr->second[nIndex];
			EnityObjSimMrg::instance()->addObjSim(pObjSim);
		}
	}

	m_simInfo = freexData.getSimInfo();
}

void RadarGenSim::Start()
{
	m_bStop = false;
	start();
}

void RadarGenSim::Stop()
{
	m_bStop = true;
}

void RadarGenSim::run()
{
	
	double dLogicTime = SimTimer::getInstance()->getCurLogicTime();
	while ( (dLogicTime >=0 && dLogicTime <= m_simInfo.dTimeLen) && (!m_bStop))
	{
		EntityObjSim::s_curLogicTime = SimTimer::getInstance()->getCurLogicTime();
		EntityObjSim::s_dReferenTime = SimTimer::getInstance()->getStartAbsTime();
		EntityObjSim::s_dRelPeriod = SimTimer::getInstance()->getRelElapseTime();

		// 模拟数据
		SimGenerData();

		// 时钟加加
		SimTimer::getInstance()->setCurLogicTime(dLogicTime + SimTimer::getInstance()->getPeriod());
		dLogicTime = SimTimer::getInstance()->getCurLogicTime();

		int nBer = dLogicTime / SimTimer::getInstance()->getPeriod();
		if (nBer % 5 == 1)
		{
			emit CurTimeSignal(dLogicTime);
		}

	}

	emit CurTimeSignal(dLogicTime);
}

