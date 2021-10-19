#include "SimData.h"

#include "SimTimer.h"

#include "EntityObjSim.h"
#include "FreeXSimSceneParse.h"
#include "ConfigMgr.h"

#include <algorithm>

#include <QSettings>
#include <QDateTime>

#include <QDebug>
#include <QMutexLocker>

#include"MathCalc.h"
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif
// 
// 检测数据是否在该时刻到下一时刻内
bool DataTimeInPeriod( double dLogicTime,double dPeriod,double dDataTime)
{
	if (dDataTime >= dLogicTime && dDataTime < dLogicTime + dPeriod / SimTimer::getInstance()->getTimeFactor())
	{
		return true;
	}

	return false;
}

// 按照时间排序
bool compare(ComMsg a,ComMsg b)
{
	return a.msgHead.dTime < b.msgHead.dTime;
}

template<class T >
void getComMsg(double dLogicTime, double dAbsTime, std::vector<T> msgSt, MsgType msgType, std::vector<ComMsg> &vecMsg)
{
	for (int nIndex = 0; nIndex < msgSt.size(); ++nIndex)
	{
		ComMsg msg;
		msg.msgHead.type = msgType;
		msg.msgHead.dataLen = sizeof(T);
		msg.msgHead.dTime = msgSt[nIndex].dTime;
		msgSt[nIndex].dTime += dAbsTime;
		memcpy(msg.data,&msgSt[nIndex],msg.msgHead.dataLen);
		vecMsg.push_back(msg);
	}
}

template<class T >
void getComMsgMap(double dLogicTime, double dAbsTime, std::vector<T> msgSt, MsgType msgType, std::map< double,std::vector<ComMsg> > &mapVecMsg)
{
	for (int nIndex = 0; nIndex < msgSt.size(); ++nIndex)
	{
		ComMsg msg;
		msg.msgHead.type = msgType;
		msg.msgHead.dataLen = sizeof(T);
		msg.msgHead.dTime = dLogicTime;
		msgSt[nIndex].dTime += dAbsTime;
		memcpy(msg.data,&msgSt[nIndex],msg.msgHead.dataLen);
		mapVecMsg[msg.msgHead.dTime].push_back(msg);
	}
}

SimData::SimData(void)
	:m_bPause(false)
	,m_bStop(false)
	,m_dCurEplaseTime(0)
	,m_dStartSysTime(0)
	,m_dStopTime(0)
{
	ParseNetInfo();

	m_pSendUdp = new CMultiCast;
	m_pSendUdp->InitSocket();
	m_pSendUdp->JoinGroup(m_sendNetInfo.strIp.toLocal8Bit().data(),m_sendNetInfo.nPort,1);
}


SimData::~SimData(void)
{
	if (m_pSendUdp != NULL)
	{
		delete m_pSendUdp;
		m_pSendUdp = NULL;
	}
}

void SimData::GenerSimObj()
{
	FreeXSimSceneData freexData;
	FreeXSimSceneParse parse(freexData);
	parse.ParseScneSim(FilePathMgr::getDataFileDir() + "SceneConfig.xml");

	emit SimTimeLenSignal(freexData.getSimInfo().dTimeLen);
	emit CurSceneNameSignal(freexData.getSimInfo().chBattleName);
	/// 
	std::map< int,std::vector<SimEntityObj> > & mapSimObj = freexData.getEntityObjs();
	EnityObjSimMrg::instance()->clearAll();
	for (std::map< int,std::vector<SimEntityObj> >::iterator itr = mapSimObj.begin();
		itr != mapSimObj.end(); ++itr)
	{
		for (int nIndex = 0; nIndex < itr->second.size(); ++nIndex)
		{
			EntityObjSim *pObjSim = new EntityObjSim;
			pObjSim->startSim();
			pObjSim->getSimEntity() = itr->second[nIndex];
			EnityObjSimMrg::instance()->addObjSim(pObjSim);
		}
	}
	
	QSet<int> vecIdInt;
	for (int i = 0; i < 9; ++i)
	{
		vecIdInt.insert(10000+i);
	}
	
	//for (std::map< int,std::vector<SimEntityObj> >::iterator itr = mapSimObj.begin();
	//	itr != mapSimObj.end(); ++itr)
	//{
	//	for (int nIndex = 0; nIndex < itr->second.size(); ++nIndex)
	//	{
	//		/*	EntityObjSim *pObjSim = new EntityObjSim;
	//		pObjSim->startSim();
	//		pObjSim->getSimEntity() = itr->second[nIndex];
	//		EnityObjSimMrg::instance()->addObjSim(pObjSim);*/

	//		if (vecIdInt.contains(itr->second[nIndex].entiyInfo.nEntityID))
	//		{
	//			if (itr->second[nIndex].vecTrackData.size() > 1)
	//			{
	//				EllipsoidModel dd;

	//				EntityPtPs pt1 = itr->second[nIndex].vecTrackData[0];
	//				EntityPtPs pt2 = itr->second[nIndex].vecTrackData[1];
	//				double dDis = dd.distance(pt1.pt.dX,pt1.pt.dY,pt1.pt.dZ,pt2.pt.dX,pt2.pt.dY,pt2.pt.dZ);

	//				double dH = dDis / 1000.0 /57;

	//				qDebug() << "id===" << itr->second[nIndex].entiyInfo.nEntityID << "SECODE==	" << dH * 3600;	
	//			}
	//		}
	//	}
	//}
	// 事件信息数组
	m_vecEnventInfo.vecT.clear();
	m_vecEnventInfo.resetIndex(0);
	m_strEvenFilePath = freexData.getEvenFilePath();
	m_vecEnventInfo.vecT = freexData.getEnventInfo(); 

	// 击中事件
	m_vecHitCommand.vecT.clear();
	m_vecHitCommand.resetIndex(0);
	m_strHitFilePath = freexData.getHitFilePath();
	m_vecHitCommand.vecT = freexData.getHitCommand();

	// 销毁命令
	m_vecDestoryCommand.vecT.clear();
	m_vecDestoryCommand.resetIndex(0);
	m_strDesFilePath = freexData.getDesFilePath();
	m_vecDestoryCommand.vecT = freexData.getDestoryCommand();

	// 视点信息
	m_vecViewCommandInfo.vecT.clear();
	m_vecViewCommandInfo.resetIndex(0);
	m_strViewFilePath = freexData.getViewFilePath();
	m_vecViewCommandInfo.vecT = freexData.getViewCommandInfo();

	// 字幕消息
	m_vecScreenWordInfo.vecT.clear();
	m_vecScreenWordInfo.resetIndex(0);
	m_strScrInfoFilePath = freexData.getScrInfoFilePath();
	m_vecScreenWordInfo.vecT = freexData.getScreenWordInfo();

	// 锁定目标信息
	m_vecAimInfo.vecT.clear();
	m_vecAimInfo.resetIndex(0);
	m_strAimDataFilePath = freexData.getAimFilePath();
	m_vecAimInfo.vecT = freexData.getAimInfoMsg();

	// 空域消息
	m_vecAirSpaceInfo.vecT.clear();
	m_vecAirSpaceInfo.resetIndex(0);
	m_strAirSpaceFilePath = freexData.getAirSpaceFilePath();
	m_vecAirSpaceInfo.vecT = freexData.getAirSpaceInfo();

	m_simInfo = freexData.getSimInfo();
}

void SimData::Start()
{
	SimTimer::setDelayStop(false);
	m_bStop = false;
	m_bPause = false;
	start();
}

void SimData::Stop()
{
	SimTimer::setDelayStop(true);
	m_bStop = true;

	ComMsg msg;
	msg.msgHead.type = ClearMsg;
	msg.msgHead.dataLen = 2;
	// 发送
	if (m_pSendUdp != NULL)
	{
		m_pSendUdp->SendData((char *)(&msg), sizeof(msg));
	}
}

void SimData::DataSim()
{
	EntityObjSim::s_curLogicTime = SimTimer::getInstance()->getCurLogicTime();
	EntityObjSim::s_dReferenTime = SimTimer::getInstance()->getStartAbsTime();
	EntityObjSim::s_dRelPeriod = SimTimer::getInstance()->getPeriod();

	m_vecComMsg.clear();

	// 处理收集所有消息
	HandlCommInfo(m_vecComMsg);
	EnityObjSimMrg::instance()->getObjComMsg(m_vecComMsg);
	// 按照时间排序回放
	std::sort(m_vecComMsg.begin(),m_vecComMsg.end(),compare);
	// 
	double dElaspTime = EntityObjSim::s_curLogicTime;
	double dDelt = 0;

	for (int nIndex = 0; nIndex < m_vecComMsg.size(); ++nIndex)
	{
		// 当前时间已经小于等于逝去逻辑时间
		if (m_vecComMsg[nIndex].msgHead.dTime <= dElaspTime)
		{
			// 已经延后需要直接发送
		}
		else
		{
			//校准系统时间 
			double dCurSysTime = SimTimer::getCurSysTime();
			// 当前系统时间落后逻辑时间的秒数 如果 dSysLogicDelt 大于零 说明 当前系统时间快于逻辑时间 消息直接发送 
			// 与仿真速度关联
			double dSysLogicDelt = ( (dCurSysTime - m_dStartSysTime) - m_dCurEplaseTime );
			// 下一时刻需要延时的时间间隔 = 下一个时间戳 - 系统逻辑时间落后时差
			// 与仿真速度关联
			dDelt = (m_vecComMsg[nIndex].msgHead.dTime - dElaspTime) /** SimTimer::getInstance()->getTimeFactor()*/  - dSysLogicDelt;
			if (dDelt > 0)
			{
				SimTimer::delay( dDelt * SimTimer::getInstance()->getTimeFactor());
				dElaspTime += m_vecComMsg[nIndex].msgHead.dTime - dElaspTime ;
			}
			else
			{
				// 已经滞后直接发送
			}
		}

		// 发送
		if (m_pSendUdp != NULL)
		{
			m_pSendUdp->SendData((char *)(&m_vecComMsg[nIndex]),sizeof(m_vecComMsg[nIndex]));
		}
	}
	
	// 绝对时间
	//校准系统时间 
	double dCurSysTime = SimTimer::getCurSysTime();
	// 当前系统时间落后逻辑时间的秒数 如果 dSysLogicDelt 大于零 说明 当前系统时间快于逻辑时间 消息直接发送 
	double dSysLogicDelt = ( (dCurSysTime - m_dStartSysTime) - m_dCurEplaseTime );

	if(dSysLogicDelt < 0)
	{
		SimTimer::delay(-dSysLogicDelt);
	}
}

void SimData::SetPause( bool bPause )
{
	QMutexLocker loker(&m_mutex);
	if (m_bStop == false )
	{
		if (bPause == true && m_bPause == false)
		{
			// 记录暂停时刻时间
			m_dStopTime = SimTimer::getCurSysTime();
		}
		else if (bPause == false && m_bPause == true)
		{
			m_dStartSysTime += SimTimer::getCurSysTime() - m_dStopTime;
			m_dStopTime = 0;
		}
	}
	m_bPause = bPause;
	
}

bool SimData::GetPause()
{
	return m_bPause;
}

void SimData::SetTimeFactor( double dFactor )
{
	QMutexLocker loker(&m_mutex);
	SimTimer::getInstance()->setTimeFactor(dFactor);
}

void SimData::run()
{
	// 读取想定文件生成对象
	SimTimer::getInstance()->setCurLogicTime(0);
	SimTimer::getInstance()->setDeriod(0.03);
	// 
	GenerSimObj();

	double dLogicTime = SimTimer::getInstance()->getCurLogicTime();

	// 当前系统时间
	m_dStartSysTime = SimTimer::getCurSysTime();

	m_dCurEplaseTime = 0;

	//场景初始化消息 
	SendSceneInitMsg();

	// 先发送场景信息
	SendSceneInfoMsg();

	while ( (dLogicTime >=0 && dLogicTime <= m_simInfo.dTimeLen) && (!m_bStop))
	{
		QMutexLocker loker(&m_mutex);

		if (m_bPause)
		{
			continue;
		}

		// 模拟数据
		DataSim();

		m_dCurEplaseTime += SimTimer::getInstance()->getPeriod() /** SimTimer::getInstance()->getTimeFactor()*/;
		// 时钟加加
		SimTimer::getInstance()->setCurLogicTime(dLogicTime + SimTimer::getInstance()->getPeriod() / SimTimer::getInstance()->getTimeFactor());
		dLogicTime = SimTimer::getInstance()->getCurLogicTime();

		int nBer = dLogicTime / SimTimer::getInstance()->getPeriod();

		if (nBer % 5 == 1)
		{
			emit CurTimeSignal(dLogicTime);
		}
	}

	emit CurTimeSignal(dLogicTime);
}

void SimData::ParseNetInfo()
{
	QString DataFileDir = FilePathMgr::getDataFileDir();
	QSettings setings(DataFileDir  + "NetCfg.ini",QSettings::Format::IniFormat);

	setings.beginGroup("sendNetInfo");

	m_sendNetInfo.strIp = setings.value("IP").toString();
	m_sendNetInfo.nPort = setings.value("Port").toInt();

	setings.endGroup();

	setings.beginGroup("recvNetInfo");

	m_recvNetInfo.strIp = setings.value("IP").toString();
	m_recvNetInfo.nPort = setings.value("Port").toInt();

	setings.endGroup();
}

void SimData::HandlCommInfo( std::vector<ComMsg> &vecMsg )
{
	double dCurLogicTime = SimTimer::getInstance()->getCurLogicTime();
	double dAbsCurTime = m_simInfo.dPreferTime /*+ dCurLogicTime*/;
	double dRelPeriod = SimTimer::getInstance()->getPeriod(); 

	// 组好包直接发送
	// 心跳包
	TimeMsg timMs;
	timMs.dTime = dCurLogicTime +dAbsCurTime;
	ComMsg heatMsg;
	heatMsg.msgHead.type = MsgType::TimeCommandMsg;
	heatMsg.msgHead.dataLen = sizeof(TimeMsg);
	memcpy(heatMsg.data,&timMs,heatMsg.msgHead.dataLen);
	vecMsg.push_back(heatMsg);

	// 视点信息包
	std::vector<ViewInfoMsg> viwcmd;
	if (m_vecViewCommandInfo.getCurT(dCurLogicTime,dRelPeriod,viwcmd))
	{
	//	for (int i = 0; i < viwcmd.size(); ++i)
		//{
			//qDebug() << "--CurTime--:" << dCurLogicTime << "ViewInfoMsg-Time" << viwcmd[i].dTime << "View_Type===" << viwcmd[i].enViewType;
//
			//if (viwcmd[i].enViewType == EN_TRAVETARGET_VIEW)
			//{
			//	int jj = 0;
			//}
		//}
		getComMsg<ViewInfoMsg>(dCurLogicTime,dAbsCurTime,viwcmd,MsgType::ViewMsg,vecMsg);
	}

	//// 字幕包
	std::vector<ScreenWordInfo> srcInfo;
	if (m_vecScreenWordInfo.getCurT(dCurLogicTime,dRelPeriod,srcInfo))
	{
		//for (int i = 0; i < srcInfo.size(); ++i)
		//{
		//	qDebug() << "--CurTime--:" << dCurLogicTime << "ScreenWordInfo-Time" << srcInfo[i].dTime << "--content" << QString(srcInfo[i].chMsg);

		//}
		getComMsg<ScreenWordInfo>(dCurLogicTime,dAbsCurTime,srcInfo,MsgType::ScreenWordMsg,vecMsg);
	}

	std::vector<HitCommand> htCmd;
	//// 击中包
	if (m_vecHitCommand.getCurT(dCurLogicTime,dRelPeriod,htCmd))
	{
		getComMsg<HitCommand>(dCurLogicTime,dAbsCurTime,htCmd,MsgType::HitMsg,vecMsg);

		//for (int i = 0; i < htCmd.size(); ++i)
		//{
		//	qDebug() << "--CurTime--:" << dCurLogicTime << "ScreenWordInfo-Time" << htCmd[i].dTime ;
		//}

		//qDebug() << "--CurTime--:" << dCurLogicTime << "EnventInfo-Time" << htCmd[0].dTime << "--content" << htCmd[0].nEntityID << "sss" << htCmd[0].nTargetId;
	}
	
	//// 销毁包
	std::vector<DestoryCommand> dsCmd;
	if(m_vecDestoryCommand.getCurT(dCurLogicTime,dRelPeriod,dsCmd))
	{
		getComMsg(dCurLogicTime,dAbsCurTime,dsCmd,MsgType::DestoryMsg,vecMsg);

		//qDebug() << "--CurTime--:" << dCurLogicTime << "EnventInfo-Time" << dsCmd[0].dTime << "--content" << QString::number(dsCmd[0].nEntityID);
	}

	//// 事件包
	std::vector<EventCommand> eveCmd;
	if (m_vecEnventInfo.getCurT(dCurLogicTime,dRelPeriod,eveCmd))
	{
		//qDebug() << "--CurTime--:" << dCurLogicTime << "EnventInfo-Time" << eveCmd[0].dTime << "--content" << QString(eveCmd[0].chDescript);
		getComMsg(dCurLogicTime,dAbsCurTime,eveCmd,MsgType::EnventMsg,vecMsg);
	}

	// 瞄准消息
	std::vector<AimInfoMsg> vecAim;
	if (m_vecAimInfo.getCurT(dCurLogicTime,dRelPeriod,vecAim))
	{
		for (int i = 0; i < vecAim.size(); ++i)
		{
			// 瞄准调整时间
			vecAim[i].dContinueTime = vecAim[i].dContinueTime * SimTimer::getInstance()->getTimeFactor();
		}

		getComMsg(dCurLogicTime,dAbsCurTime,vecAim,MsgType::AimMsg,vecMsg);
	}

	// 空域
	std::vector<AirSpeceSectorInfo> vecAir;
	if (m_vecAirSpaceInfo.getCurT(dCurLogicTime,dRelPeriod,vecAir))
	{
		getComMsg(dCurLogicTime,dAbsCurTime,vecAir,MsgType::AirSpaceMsg,vecMsg);
		qDebug() << "AirSpeceSectorInfo ===" << vecAir.size();
	}
}

void SimData::HandlCommInfo( std::map<double, std::vector<ComMsg> > &mapVecComsg )
{
	double dCurLogicTime = SimTimer::getInstance()->getCurLogicTime();
	double dAbsCurTime = m_simInfo.dPreferTime /*+ dCurLogicTime*/;
	double dRelPeriod = SimTimer::getInstance()->getPeriod()/* * SimTimer::getInstance()->getTimeFactor()*/; 

	// 组好包直接发送
	// 心跳包
	TimeMsg timMs;
	timMs.dTime = dCurLogicTime +dAbsCurTime;
	ComMsg heatMsg;
	heatMsg.msgHead.type = MsgType::TimeCommandMsg;
	heatMsg.msgHead.dataLen = sizeof(TimeMsg);
	memcpy(heatMsg.data,&timMs,heatMsg.msgHead.dataLen);

	// 视点信息包
	std::vector<ViewInfoMsg> viwcmd;
	if (m_vecViewCommandInfo.getCurT(dCurLogicTime,dRelPeriod,viwcmd))
	{
		getComMsgMap<ViewInfoMsg>(dCurLogicTime,dAbsCurTime,viwcmd,MsgType::ViewMsg,mapVecComsg);
	}

	// 字幕包
	std::vector<ScreenWordInfo> srcInfo;
	if (m_vecScreenWordInfo.getCurT(dCurLogicTime,dRelPeriod,srcInfo))
	{
		getComMsgMap<ScreenWordInfo>(dCurLogicTime,dAbsCurTime,srcInfo,MsgType::ScreenWordMsg,mapVecComsg);
	}

	// 击中包
	std::vector<HitCommand> htCmd;
	if (m_vecHitCommand.getCurT(dCurLogicTime,dRelPeriod,htCmd))
	{
		getComMsgMap<HitCommand>(dCurLogicTime,dAbsCurTime,htCmd,MsgType::HitMsg,mapVecComsg);
	}

	// 销毁包
	std::vector<DestoryCommand> dsCmd;

	if(m_vecDestoryCommand.getCurT(dCurLogicTime,dRelPeriod,dsCmd))
	{
		getComMsgMap(dCurLogicTime,dAbsCurTime,dsCmd,MsgType::DestoryMsg,mapVecComsg);
	}

	// 事件包
	std::vector<EventCommand> eveCmd;
	if (m_vecEnventInfo.getCurT(dCurLogicTime,dRelPeriod,eveCmd))
	{
		getComMsgMap(dCurLogicTime,dAbsCurTime,eveCmd,MsgType::EnventMsg,mapVecComsg);
	}

}

void SimData::SendSceneInfoMsg()
{
	if (m_pSendUdp == NULL)
	{
		return;
	}

	ComMsg heatMsg;
	heatMsg.msgHead.dTime = SimTimer::getInstance()->getCurLogicTime() + m_simInfo.dPreferTime;
	heatMsg.msgHead.type = MsgType::SimInfoMsg;
	heatMsg.msgHead.dataLen = sizeof(SimInfo);
	memcpy(heatMsg.data,&m_simInfo,sizeof(SimInfo));

	m_pSendUdp->SendData((char*)(&heatMsg),sizeof(ComMsg));
}

void SimData::SendSceneInitMsg()
{
	if (m_pSendUdp == NULL)
	{
		return;
	}

	ComMsg heatMsg;
	heatMsg.msgHead.dTime = SimTimer::getInstance()->getCurLogicTime() + m_simInfo.dPreferTime;
	heatMsg.msgHead.type = MsgType::InitSimMsg;
	heatMsg.msgHead.dataLen = 0;

	m_pSendUdp->SendData((char*)(&heatMsg),sizeof(ComMsg));
}
