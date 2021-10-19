#include "EntityObjSim.h"
#include "MathCalc.h"
#include "SimTimer.h"
#include "ConfigMgr.h"
#include <QDebug>

template<class T >
void getComMsgEn(double dLogicTime, double dAbsTime, std::vector<T> msgSt, MsgType msgType, std::vector<ComMsg> &vecMsg)
{
	for (int nIndex = 0; nIndex < msgSt.size(); ++nIndex)
	{
		//结构体
		/************************************************************************/
			/* // 消息头
			struct MsgHead
			{
				// 消息类型
				MsgType type;
				// 数据区长度
				unsigned dataLen;
				//
				double dTime;
			};

			struct ComMsg
			{
				MsgHead msgHead;
				char data[MAX_MSG_LEN];
			};                                                                     */
		/************************************************************************/
		ComMsg msg;
		//消息头
		msg.msgHead.type = msgType;
		msg.msgHead.dataLen = sizeof(T);
		msg.msgHead.dTime = msgSt[nIndex].dTime;

		msgSt[nIndex].dTime += dAbsTime;

		//数据部分 
		memcpy(msg.data,&msgSt[nIndex],msg.msgHead.dataLen);
		vecMsg.push_back(msg);
	}
}

template<class T >
void getComMsgEn(double dLogicTime, double dAbsTime, std::vector<T> msgSt, MsgType msgType, std::map< double,std::vector<ComMsg> > &mapVecMsg)
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

// 等号运算符
bool operator == ( SimDetect a, SimDetect b)
{
	return a.nEntityId == b.nEntityId && a.nTargetId == b.nTargetId;
}

// 小于运算符
bool operator < ( SimDetect a, SimDetect b)
{
	return a.nEntityId < b.nEntityId && a.nTargetId < b.nTargetId;
}

#ifndef SPLIT_CHAR
#define SPLIT_CHAR " "
#endif

double EntityObjSim::s_curLogicTime = 0;
double EntityObjSim::s_dRelPeriod = 1;
double EntityObjSim::s_dReferenTime = 0;

EntityObjSim::EntityObjSim()
	:m_unCurTrackIndex(0)
{
	// 当前实体航迹下标
	m_unCurTrackIndex = 1;

	// 当前雷达传感器实体下标
	m_unSenorIndex = 0;

	// 当前探测下标
	m_unDetectIndex = 0;
}

EntityObjSim::~EntityObjSim()
{
	// 关闭文件
	m_fileSenor.close();
	m_fileDetect.close();

	// 当前实体航迹下标
	m_unCurTrackIndex = 1;

	// 当前雷达传感器实体下标
	m_unSenorIndex = 0;

	// 当前探测下标
	m_unDetectIndex = 0;

	m_mapDetectState.clear();
}


SimEntityObj & EntityObjSim::getSimEntity()
{
	return m_simObj;
}
//								   【in】  仿真实体对象
void EntityObjSim::GenerDetectData( SimEntityObj & simObj)
{
	if (!IsExsit(simObj))
	{
		// 实体不存在
		m_fileDetect.close();
		return;
	}

	// 根据航迹判断当前是否被检测到
	// 计算当前实体 位置姿态数据
	std::vector<EntityPtPs> curPtps;
	bool bRet = CaclEntityPtPs(simObj,curPtps,true);

	if (!bRet )
	{
		return;
	}

	if (simObj.entiyInfo.nSenorNum > 0)
	{
		if (!m_fileDetect.isOpen())
		{
			m_fileDetect.setFileName(FilePathMgr::getDetectDataPath(simObj.entiyInfo.nEntityID));
			if (!m_fileDetect.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
				return;
		}
	}

	// 如果传感器开则判断是否在其范围内
	// 如果是红方实体则探测蓝方实体状况
	std::map<int, std::vector<EntityObjSim*> > &m_mapCampEntityObj = EnityObjSimMrg::instance()->getObjSims();
	std::vector<EntityObjSim*>  vecSimObj;
	if (simObj.entiyInfo.enCamp == 1)
	{
		// 通过路径来判断
		if (m_mapCampEntityObj.count(2))
		{
			vecSimObj = m_mapCampEntityObj[2];
		}
	}
	else if (simObj.entiyInfo.enCamp == 2)
	{
		// 如果是蓝方实体则探测红方实体
		if (m_mapCampEntityObj.count(1))
		{
			vecSimObj = m_mapCampEntityObj[1];
		}
	}

	SimDetect detect;
	bool bUse = false;
	QString strId;

	for (int i = 0; i < vecSimObj.size(); ++i)
	{
		for (int ptIndex = 0; ptIndex < curPtps.size(); ++ptIndex )
		{
			bUse = false;
			//探测目标
			detect.enTrackState = DetecTrarget(vecSimObj[i]->getSimEntity(),curPtps[ptIndex]);
			detect.nEntityId = m_simObj.entiyInfo.nEntityID;
			if (vecSimObj[i] == NULL)
			{
				continue;
			}
			detect.nTargetId = vecSimObj[i]->getSimEntity().entiyInfo.nEntityID;
			// 

			strId = getDetectStrId(detect);
			if (m_mapDetectState.count(strId))
			{
				// 
				if (m_mapDetectState[strId].enTrackState != detect.enTrackState)
				{
					bUse = true;
					m_mapDetectState[strId].enTrackState = detect.enTrackState;
				}
			}
			else
			{
				if (detect.enTrackState == TRACK_STATE)
				{
					m_mapDetectState[strId] = detect;
					bUse = true;
				}
			}

			if (bUse)
			{
				QString strLine;
				strLine += QString::number(s_curLogicTime,'f',10) + SPLIT_CHAR;
				strLine += QString::number(detect.nEntityId) + SPLIT_CHAR;
				strLine += QString::number(detect.nTargetId) + SPLIT_CHAR;
				strLine += QString::number(detect.enTrackState) + "\n";
				///qDebug() << strLine;
				m_fileDetect.write(strLine.toLocal8Bit());
			}
		}
	}
}

EnTrackState EntityObjSim::DetecTrarget( SimEntityObj &simTargetObj,EntityPtPs entiyPtps )
{
	EnTrackState tracState = UNTRACK_STATE;
	for (int nSenorIndex = 0; nSenorIndex < m_simObj.entiyInfo.nSenorNum; ++nSenorIndex)
	{
		// 遍历每个传感器 在此处制作简单的计算
		// 
		Senor senorData = m_simObj.entiyInfo.senor[nSenorIndex];

		if (CaclSenorState(m_simObj,senorData) != EnMachState::OPEN_STATE)
		{
			continue;
		}

		std::vector< EntityPtPs > ptPs;

		bool bRet = CaclEntityPtPs(simTargetObj,ptPs,true);

		if (bRet)
		{
			int nIndex = ptPs.size() - 1;
			EllipsoidModel ell;
			// 计算与实体之间的距离
			double dDis = ell.distance(entiyPtps.pt.dX,entiyPtps.pt.dY,entiyPtps.pt.dZ,ptPs[nIndex].pt.dX,ptPs[nIndex].pt.dY,ptPs[nIndex].pt.dZ);

			/// 小于雷达威力范围则探测到装备写入文件
			if (dDis < senorData.dRaduis)
			{
				// 只要被一个雷达检测到说明该实体被雷达检测到，全部没检测到说明没有探测到该目标
				tracState = TRACK_STATE;

				break;
			}
		}
	}

	return tracState;
}

void EntityObjSim::GenreRadarBeamData( SimEntityObj & simObj)
{
	if (!IsExsit(simObj))
	{
		m_fileSenor.close();
		return;
	}

	if (simObj.entiyInfo.nSenorNum > 0)
	{
		if (!m_fileSenor.isOpen())
		{
			m_fileSenor.setFileName(FilePathMgr::getSenorDataPath(simObj.entiyInfo.nEntityID));
			if (!m_fileSenor.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
				return;
		}
	}

	// 遍历每个传感器
	for (int nSenorIndex = 0; nSenorIndex < simObj.entiyInfo.nSenorNum; ++nSenorIndex)
	{
		// 遍历每个传感器 在此处制作简单的计算
		// 
		Senor &senor = simObj.entiyInfo.senor[nSenorIndex];
		senor.enState = (EnMachState) CaclSenorState(simObj,simObj.entiyInfo.senor[nSenorIndex]);

		QString strLine;

		strLine += QString::number(s_curLogicTime,'f',6) + SPLIT_CHAR;

		strLine += QString::number(senor.nEntityID) + SPLIT_CHAR;
		strLine += QString::number(senor.nSeorId) + SPLIT_CHAR;

		strLine += QString::number(senor.dAzStartAngle,'f',6) + SPLIT_CHAR;
		strLine += QString::number(senor.dAzEndAngle,'f',6) + SPLIT_CHAR;

		strLine += QString::number(senor.dPicthStartAngle,'f',6) + SPLIT_CHAR;
		strLine += QString::number(senor.dPicthEndAngle,'f',6) + SPLIT_CHAR;
		strLine += QString::number(senor.dRaduis,'f',6) + SPLIT_CHAR;
		strLine += QString::number(senor.enState) + SPLIT_CHAR;
		strLine += QString::number(senor.dScanPeriod) + SPLIT_CHAR;
		strLine += QString::number(senor.nPrefer) + SPLIT_CHAR;

		// 如果传感器关闭 
		if (senor.enState != 1)
		{
			// 只写入雷达自身数据
		}
		else
		{
			//写入雷达自身数据以及雷达波数据
			// 雷达波束
			// 
			double dScanPeriod = senor.dScanPeriod;

			double dPerDeg = (senor.dAzEndAngle - senor.dAzStartAngle) / (dScanPeriod / SimTimer::getInstance()->getPeriod());

			// 写雷达波束数据
			for (int nBeamIndex = 0; nBeamIndex < senor.nBeamNum; ++nBeamIndex)
			{
				// 
				RadarBeamInfo &beamData = senor.beam[nBeamIndex];
				double dBeamAz = beamData.dAzAngle;
				double dHorWidth = beamData.dHorWidth;

				if (beamData.snTurn == 1 && dBeamAz + dPerDeg + (dHorWidth / 2.0) > senor.dAzEndAngle )
				{
					beamData.snTurn = 2;
				}

				if (beamData.snTurn == 2 && dBeamAz - dPerDeg - (dHorWidth / 2.0) < senor.dAzStartAngle)
				{
					beamData.snTurn = 1;
				}

				int nTurn = beamData.snTurn == 1 ? 1 : -1;

				beamData.dAzAngle = dBeamAz + dPerDeg * nTurn;

				strLine += QString::number(beamData.nBeamId) + SPLIT_CHAR;
				strLine += QString::number(beamData.dHorWidth,'f',6) + SPLIT_CHAR;
				strLine += QString::number(beamData.dVerWidth,'f',6) + SPLIT_CHAR;
				strLine += QString::number(beamData.dAzAngle,'f',6) + SPLIT_CHAR;
				strLine += QString::number(beamData.dPitchAngle,'f',6) + SPLIT_CHAR;
				strLine += QString::number(beamData.snTurn,'f',6) + SPLIT_CHAR;
			}
		}
		strLine += "\n";
		m_fileSenor.write(strLine.toLocal8Bit());
	}
}


void EntityObjSim::StopGenrateData()
{
	if (m_fileSenor.isOpen())
	{
		m_fileSenor.close();
	}

	if (m_fileDetect.isOpen())
	{
		m_fileDetect.close();
	}

	// 当前实体航迹下标
	m_unCurTrackIndex = 1;

	// 当前雷达传感器实体下标
	m_unSenorIndex = 0;

	// 当前探测下标
	m_unDetectIndex = 0;

	//
	m_unSepIndex = 0;

	m_mapDetectState.clear();
}

bool EntityObjSim::getObjComMsg( std::vector<ComMsg> &vecComsg )
{
	// 该实体在该时刻到下周期内所有数据取出来准备回放
	{
		NewEntity newEntity;
		if (getNewEntity(newEntity))
		{
			// 
			newEntity.dTime = m_simObj.dExsitStartTime;
			std::vector<NewEntity> vecNewEntity;
			vecNewEntity.push_back(newEntity);
			getComMsgEn<NewEntity>(s_curLogicTime,s_dReferenTime,vecNewEntity,MsgType::NewEntityMsg,vecComsg);

			//qDebug() << "New Entity LogicTime==" << s_curLogicTime << "RelaTime--" << vecComsg[0].msgHead.dTime << "nEntityId" << newEntity.nEntityID;
		}
	}

	{
		std::vector< EntityStu> vecEntityStu;
		if (getEntityStu(vecEntityStu))
		{
			//	qDebug() << "StuState LogicTime==" << s_curLogicTime << "RelaTime--" << vecEntityStu[0].dTime << "nEntityId" << m_simObj.entiyInfo.nEntityID 
			//		<< "Lon="<< vecEntityStu[0].dLon << "Lat=" << vecEntityStu[0].dLat << "Alt=" << vecEntityStu[0].dAlt ;
			getComMsgEn<EntityStu>(s_curLogicTime,s_dReferenTime,vecEntityStu,MsgType::EntityStuMsg,vecComsg);
		}
	}

	{
		std::vector<Senor> vecSenor;
		if (getCurSenor(m_simObj, vecSenor))
		{
			// 
			//if (m_simObj.entiyInfo.nEntityID == 1)
			{
				//qDebug() << "LogicTime==" << s_curLogicTime << "Time" << vecSenor[0].dTime;
				getComMsgEn<Senor>(s_curLogicTime,s_dReferenTime,vecSenor,MsgType::SenorMsg,vecComsg);
			}
		}
	}

	{
		std::vector<DetectCommand> vecComand;
		if (getCurDetect(m_simObj, vecComand))
		{
			//qDebug() << "StuState LogicTime==" << s_curLogicTime << "RelaTime--" << vecComand[0].dTime << "nEntityId" << m_simObj.entiyInfo.nEntityID 
			//	<< "TargetId="<< vecComand[0].nTargetId << "State=" << vecComand[0].enTrackState;
			getComMsgEn<DetectCommand>(s_curLogicTime,s_dReferenTime,vecComand,MsgType::DetecTargetMsg,vecComsg);

		}
	}

	{
		// 分离事件
		std::vector<SpeareSubMsg> vecSpeMsg;
		if (getCurSubEvent(m_simObj,vecSpeMsg))
		{
			// 分离事件
			getComMsgEn<SpeareSubMsg>(s_curLogicTime,s_dReferenTime,vecSpeMsg,MsgType::SpeSubMsg,vecComsg);
		}
	}
	return true;
}

bool EntityObjSim::getObjComMsg( std::map<double ,std::vector<ComMsg> > &mapVecComsg )
{

	{
		NewEntity newEntity;
		if (getNewEntity(newEntity))
		{
			// 
			newEntity.dTime = s_curLogicTime;
			std::vector<NewEntity> vecNewEntity;
			vecNewEntity.push_back(newEntity);
			getComMsgEn<NewEntity>(s_curLogicTime,s_dReferenTime,vecNewEntity,MsgType::NewEntityMsg,mapVecComsg);
		}
	}

	{
		std::vector< EntityStu > entityStu;
		if (getEntityStu(entityStu))
		{
			getComMsgEn<EntityStu>(s_curLogicTime,s_dReferenTime,entityStu,MsgType::EntityStuMsg,mapVecComsg);
		}
	}

	{
		std::vector<Senor> vecSenor;
		if (getCurSenor(m_simObj, vecSenor))
		{
			// 
			getComMsgEn<Senor>(s_curLogicTime,s_dReferenTime,vecSenor,MsgType::SenorMsg,mapVecComsg);
		}

	}

	{
		std::vector<DetectCommand> vecComand;

		if (getCurDetect(m_simObj, vecComand))
		{
			getComMsgEn<DetectCommand>(s_curLogicTime,s_dReferenTime,vecComand,MsgType::DetecTargetMsg,mapVecComsg);
		}
	}

	return true;
}

void EntityObjSim::startSim()
{
	if (m_fileSenor.isOpen())
	{
		m_fileSenor.close();
	}

	if (m_fileDetect.isOpen())
	{
		m_fileDetect.close();
	}

	// 当前实体航迹下标
	m_unCurTrackIndex = 1;

	// 当前雷达传感器实体下标
	m_unSenorIndex = 0;

	// 当前探测下标
	m_unDetectIndex = 0;

	// 
	m_unSepIndex = 0;

	m_mapDetectState.clear();
}

bool EntityObjSim::IsExsit( const SimEntityObj & simObj )
{
	// 判断该实体是否已经死亡
	if ( !(s_curLogicTime >= simObj.dExsitStartTime && s_curLogicTime <= simObj.dExsitEndTime) )
	{
		return false;
	}

	return true;
}


bool EntityObjSim::getNewEntity(NewEntity &newEntity)
{
	// 
	if (DataTimeInPeriod(SimTimer::getInstance()->getPeriod(), m_simObj.dExsitStartTime))
	{
		newEntity = m_simObj.entiyInfo;
		return true;
	}

	return false;
}

bool EntityObjSim::getEntityStu(std::vector< EntityStu> &vecEntityStu)
{
	if (m_simObj.entiyInfo.nEntityID == 4)
	{
		int jj = 0;
	}
	std::vector< EntityPtPs> pts;
	// 
	EntityStu entityStu;

	bool bRet = CaclEntityPtPs(m_simObj,pts,true);
	if (bRet)
	{
		for (int nIndex = 0; nIndex < pts.size(); ++nIndex)
		{
			
			entityStu.dTime = pts[nIndex].dTime;
			entityStu.nEntityID = m_simObj.entiyInfo.nEntityID;
			entityStu.dLon = pts[nIndex].pt.dX;
			entityStu.dLat = pts[nIndex].pt.dY;
			entityStu.dAlt = pts[nIndex].pt.dZ;
			entityStu.dPicthAngle = pts[nIndex].posture.dX;
			entityStu.dRolAngle = pts[nIndex].posture.dY;
			entityStu.dAzAngle = pts[nIndex].posture.dZ;

			// 计算通信状态状态
			getCurCommonState(m_simObj,entityStu.enCommuState,s_curLogicTime);

			vecEntityStu.push_back(entityStu);
		}

	}
	return bRet;
}


int EntityObjSim::CaclSenorState( SimEntityObj &simObj, Senor & senorData )
{
	// 
	int nSenorId = senorData.nSeorId;
	// 判断该传感器状态  传感器状态 1-开机 2-关机 3-故障 
	int nSenorState = 1;
	if (simObj.mapSenorsState.count(nSenorId))
	{
		const std::vector<TimeSegState> &vecTimeSeg = simObj.mapSenorsState[1];
		for (int i = 0; i < vecTimeSeg.size(); ++i)
		{
			// 
			if (vecTimeSeg[i].dStartTime <= s_curLogicTime && s_curLogicTime < vecTimeSeg[i].dEndTime)
			{
				nSenorState = vecTimeSeg[i].nState;
			}
		}
	}
	else
	{
		nSenorState = senorData.enState;
	}

	return nSenorState;
}

bool EntityObjSim::CaclEntityPtPs(SimEntityObj &simObj,std::vector<EntityPtPs> &outPtPs,bool bNeed )
{
	//
	if (simObj.vecTrackData.size() == 0)
	{
		return false;
	}

	// 判断该实体是否已经死亡
	if ( !IsExsit(simObj) )
	{
		return false;
	}

	// 如果有航迹则选取航迹数据中的点
	int nTrackPtSize = simObj.vecTrackData.size();

	if (nTrackPtSize > 0)
	{
		// 如果第一个点的时间已经超过当前时间说明该
		if (simObj.vecTrackData[0].dTime > s_curLogicTime)
		{
			return false;
		}
	}

	// 找第一个大于该时间点的点
	for (;m_unCurTrackIndex < nTrackPtSize; ++m_unCurTrackIndex)
	{
		if (simObj.vecTrackData[m_unCurTrackIndex].dTime > s_curLogicTime )
		{
			EntityPtPs pts;
			EntityPtPs pts1 = simObj.vecTrackData[m_unCurTrackIndex-1];
			EntityPtPs pts2 = simObj.vecTrackData[m_unCurTrackIndex];

			if (pts2.dTime == pts1.dTime)
			{
				pts = pts1;
			}
			else
			{
				pts.dTime = s_curLogicTime;
				double dFactor = (s_curLogicTime - pts1.dTime) / (pts2.dTime - pts1.dTime);
				pts.pt.dX = pts1.pt.dX + (pts2.pt.dX - pts1.pt.dX) * dFactor;
				pts.pt.dY = pts1.pt.dY + (pts2.pt.dY - pts1.pt.dY) * dFactor;
				pts.pt.dZ = pts1.pt.dZ + (pts2.pt.dZ - pts1.pt.dZ) * dFactor;

				// 姿态考虑 俯仰[-180-180] 横滚[-180-180] 方位[0-360]
				// 考虑方位从 跨0 或者 360时的情况
				if (abs(pts2.posture.dZ - pts1.posture.dZ) > 180)
				{
					if (pts2.posture.dZ - pts1.posture.dZ < 0)
					{
						// 说明递加从跨越360
						pts2.posture.dZ += 360;
					}
					else
					{
						// 递减跨越0
						pts1.posture.dZ += 360; 
					}
				}
				pts.posture.dX = pts1.posture.dX + (pts2.posture.dX - pts1.posture.dX) * dFactor;
				pts.posture.dY = pts1.posture.dY + (pts2.posture.dY - pts1.posture.dY) * dFactor;
				pts.posture.dZ = pts1.posture.dZ + (pts2.posture.dZ - pts1.posture.dZ) * dFactor;
			}

			outPtPs.push_back(pts);
			break;
		}
		//if (DataTimeInPeriod(SimTimer::getInstance()->getPeriod(),simObj.vecTrackData[m_unCurTrackIndex].dTime) )
		//{
		//	outPtPs.push_back(simObj.vecTrackData[m_unCurTrackIndex]);
		//}
		//else if (simObj.vecTrackData[m_unCurTrackIndex].dTime >= s_curLogicTime + SimTimer::getInstance()->getPeriod())
		//{
		//	break;
		//}
	}

	if (bNeed)
	{
		if (outPtPs.size() == 0)
		{
			if (m_unCurTrackIndex > 0 && m_unCurTrackIndex < nTrackPtSize )
			{
				outPtPs.push_back(simObj.vecTrackData[m_unCurTrackIndex - 1]);
			}
			else
			{
				outPtPs.push_back(simObj.vecTrackData[0]);
			}
		}
	}

	return outPtPs.size() > 0;
}


bool EntityObjSim::getCurSenor( SimEntityObj &simObj, std::vector<Senor> &vecSenor )
{
	// 判断该实体是否已经死亡
	if ( !IsExsit(simObj) )
	{
		return false;
	}

	// 如果有航迹则选取航迹数据中的点
	int nTrackPtSize = simObj.vecSenorData.size();

	if (nTrackPtSize == 0)
	{
		return false;
	}

	if (nTrackPtSize > 0)
	{
		// 计算的值在这个时刻到下一个周期时刻内 所以要向前延伸一个周期
		if (simObj.vecSenorData[0].dTime - SimTimer::getInstance()->getPeriod()/SimTimer::getInstance()->getTimeFactor() > s_curLogicTime || simObj.vecSenorData[nTrackPtSize - 1].dTime < s_curLogicTime )
		{
			return false;
		}
	}

	// 找第一个大于该时间点的点
	for (;m_unSenorIndex < nTrackPtSize; ++m_unSenorIndex)
	{
		if (DataTimeInPeriod(SimTimer::getInstance()->getPeriod(),simObj.vecSenorData[m_unSenorIndex].dTime))
		{
			//
			vecSenor.push_back(simObj.vecSenorData[m_unSenorIndex]);
		}
		else if (simObj.vecSenorData[m_unSenorIndex].dTime >= s_curLogicTime + SimTimer::getInstance()->getPeriod()/SimTimer::getInstance()->getTimeFactor())
		{
			break;
		}
	}

	if (m_unSenorIndex == nTrackPtSize)
	{
		m_unSenorIndex = 0;
	}

	return vecSenor.size() > 0;
}

bool EntityObjSim::getCurDetect( SimEntityObj &simObj, std::vector<DetectCommand> &vecComand )
{
	// 判断该实体是否已经死亡
	if ( !IsExsit(simObj) )
	{
		return false;
	}

	// 如果有航迹则选取航迹数据中的点
	int nTrackPtSize = simObj.vecDetectCommand.size();

	if (nTrackPtSize == 0)
	{
		return false;
	}

	if (nTrackPtSize > 0)
	{
		if (simObj.vecDetectCommand[0].dTime - SimTimer::getInstance()->getPeriod() / SimTimer::getInstance()->getPeriod()/SimTimer::getInstance()->getTimeFactor()> s_curLogicTime  || simObj.vecDetectCommand[nTrackPtSize - 1].dTime < s_curLogicTime )
		{
			return false;
		}
	}

	// 找第一个大于该时间点的点
	for (;m_unDetectIndex < nTrackPtSize; ++m_unDetectIndex)
	{
		if (DataTimeInPeriod(SimTimer::getInstance()->getPeriod(),simObj.vecDetectCommand[m_unDetectIndex].dTime))
		{
			//
			//qDebug() << "StuState LogicTime==" << s_curLogicTime << "RelaTime--" << simObj.vecDetectCommand[m_unDetectIndex].dTime << "nEntityId" << simObj.vecDetectCommand[m_unDetectIndex].nEntityID 
			//	<< "TargetId="<< simObj.vecDetectCommand[m_unDetectIndex].nTargetId << "State=" << simObj.vecDetectCommand[m_unDetectIndex].enTrackState;
			vecComand.push_back(simObj.vecDetectCommand[m_unDetectIndex]);
		}
		else if (simObj.vecDetectCommand[m_unDetectIndex].dTime >= s_curLogicTime + SimTimer::getInstance()->getPeriod()/SimTimer::getInstance()->getTimeFactor())
		{
			break;
		}
	}

	if (m_unDetectIndex == nTrackPtSize)
	{
		m_unDetectIndex = 0;
	}

	return vecComand.size() > 0;
}

bool EntityObjSim::getCurCommonState( SimEntityObj &simObj,EnConeState &nCommState, double dTime )
{
	nCommState = simObj.entiyInfo.enCommuState;
	for (int nIndex = 0; nIndex < simObj.vecCommusState.size(); ++nIndex)
	{
		if (simObj.vecCommusState[nIndex].dStartTime <= dTime && simObj.vecCommusState[nIndex].dEndTime > dTime)
		{
			nCommState =(EnConeState) simObj.vecCommusState[nIndex].nState;
			break;
		}
	}

	return true;
}

bool EntityObjSim::getCurSubEvent( SimEntityObj &simObj,std::vector<SpeareSubMsg> &vecSepMsg )
{
	if (simObj.entiyInfo.sepNum < 1)
	{
		return false;
	}
	// 判断该实体是否已经死亡
	if ( !IsExsit(simObj) )
	{
		return false;
	}
	// 如果有航迹则选取航迹数据中的点
	int nTrackPtSize = simObj.vecSepSubInfo.size();

	if (nTrackPtSize == 0)
	{
		return false;
	}

	if (nTrackPtSize > 0)
	{
		if (simObj.vecSepSubInfo[0].dTime - SimTimer::getInstance()->getPeriod() / SimTimer::getInstance()->getPeriod()/SimTimer::getInstance()->getTimeFactor()> s_curLogicTime 
			|| simObj.vecSepSubInfo[nTrackPtSize - 1].dTime < s_curLogicTime )
		{
			return false;
		}
	}

	// 找第一个大于该时间点的点
	for (;m_unSepIndex < nTrackPtSize; ++m_unSepIndex)
	{
		if (DataTimeInPeriod(SimTimer::getInstance()->getPeriod(),simObj.vecSepSubInfo[m_unSepIndex].dTime))
		{
			//
			//qDebug() << "StuState LogicTime==" << s_curLogicTime << "RelaTime--" << simObj.vecDetectCommand[m_unDetectIndex].dTime << "nEntityId" << simObj.vecDetectCommand[m_unDetectIndex].nEntityID 
			//	<< "TargetId="<< simObj.vecDetectCommand[m_unDetectIndex].nTargetId << "State=" << simObj.vecDetectCommand[m_unDetectIndex].enTrackState;
			
			SpeareSubMsg spear;
			spear.chLevel = simObj.vecSepSubInfo[m_unSepIndex].chLevel;
			spear.dTime = simObj.vecSepSubInfo[m_unSepIndex].dTime;
			spear.nPrimId = simObj.entiyInfo.nEntityID;
			spear.nSubId = simObj.vecSepSubInfo[m_unSepIndex].nSubId;
			vecSepMsg.push_back(spear);
		}
		else if (simObj.vecSepSubInfo[m_unSepIndex].dTime >= s_curLogicTime + SimTimer::getInstance()->getPeriod()/SimTimer::getInstance()->getTimeFactor())
		{
			break;
		}
	}

	if (m_unSepIndex == nTrackPtSize)
	{
		m_unSepIndex = 0;
	}

	return vecSepMsg.size() > 0;
}

bool EntityObjSim::DataTimeInPeriod( double dPeriod,double dDataTime )
{
	if (dDataTime >= s_curLogicTime && dDataTime < s_curLogicTime + dPeriod / SimTimer::getInstance()->getTimeFactor() )
	{
		return true;
	}

	return false;
}

QString EntityObjSim::getDetectStrId( SimDetect &simDetect )
{
	return QString("%1_%2").arg(simDetect.nEntityId).arg(simDetect.nTargetId);
}

EnityObjSimMrg * EnityObjSimMrg::s_pEnityObjSimMrg = NULL;

EnityObjSimMrg * EnityObjSimMrg::instance()
{
	if (s_pEnityObjSimMrg == NULL)
	{
		s_pEnityObjSimMrg = new EnityObjSimMrg;
	}

	return s_pEnityObjSimMrg;
}

void EnityObjSimMrg::destroyInstance()
{
	if (s_pEnityObjSimMrg != NULL)
	{
		delete s_pEnityObjSimMrg;
		s_pEnityObjSimMrg = NULL;
	}
}

void EnityObjSimMrg::addObjSim( EntityObjSim *pObjSim )
{
	if (pObjSim == NULL)
	{
		return;
	}

	// 添加所有实体
	m_mapObjSim[pObjSim->getSimEntity().entiyInfo.enCamp].push_back(pObjSim);
}

void EnityObjSimMrg::destroyObjSim()
{
	// 删除所有实体
	for (std::map<int, std::vector<EntityObjSim*> >::iterator itr = m_mapObjSim.begin();
		itr != m_mapObjSim.end();++itr)
	{
		//
		for (int nIndex = 0; nIndex < itr->second.size(); ++nIndex)
		{
			// 
			if (itr->second[nIndex] != NULL)
			{
				delete itr->second[nIndex];
				itr->second[nIndex] = NULL;
			}
		}
	}

	m_mapObjSim.clear();
}

std::map<int, std::vector<EntityObjSim*> > & EnityObjSimMrg::getObjSims()
{
	// 
	return m_mapObjSim;
}

void EnityObjSimMrg::geneSimData()
{
	preSimObjLife();
	// 
	for (std::map<int, std::vector<EntityObjSim*> >::iterator itr = m_mapObjSim.begin();
		itr != m_mapObjSim.end();++itr)
	{
		//
		for (std::vector<EntityObjSim*>::iterator simItr = itr->second.begin();
			simItr != itr->second.end();simItr++)
		{
			// 生成探测数据
			(*simItr)->GenerDetectData((*simItr)->getSimEntity());

			// 生成雷达探测数据
			(*simItr)->GenreRadarBeamData((*simItr)->getSimEntity());
		}
	}
}

void EnityObjSimMrg::preSimObjLife()
{
	// 删除所有实体
	double dLogicTime = SimTimer::getInstance()->getCurLogicTime();

	for (std::map<int, std::vector<EntityObjSim*> >::iterator itr = m_mapObjSim.begin();
		itr != m_mapObjSim.end();++itr)
	{
		//
		for (std::vector<EntityObjSim*>::iterator simItr = itr->second.begin();
			simItr != itr->second.end();)
		{
			if ( dLogicTime > (*simItr)->getSimEntity().dExsitEndTime )
			{
				delete (*simItr);
				*simItr = NULL;
				simItr = itr->second.erase(simItr);
			}
			else
			{
				simItr++;
			}
		}
	}
}


void EnityObjSimMrg::getObjComMsg( std::vector<ComMsg> &vecMsg )
{
	//
	preSimObjLife();

	for (std::map<int, std::vector<EntityObjSim*> >::iterator itr = m_mapObjSim.begin();
		itr != m_mapObjSim.end();++itr)
	{
		//
		for (std::vector<EntityObjSim*>::iterator simItr = itr->second.begin();
			simItr != itr->second.end();simItr++)
		{
			(*simItr)->getObjComMsg(vecMsg);
		}

	}
}

void EnityObjSimMrg::clearAll()
{
	for (std::map<int, std::vector<EntityObjSim*> >::iterator itr = m_mapObjSim.begin();
		itr != m_mapObjSim.end();++itr)
	{
		//
		for (std::vector<EntityObjSim*>::iterator simItr = itr->second.begin();
			simItr != itr->second.end();simItr++)
		{
			delete (*simItr);
			*simItr = NULL;
		}
	}

	m_mapObjSim.clear();
}
