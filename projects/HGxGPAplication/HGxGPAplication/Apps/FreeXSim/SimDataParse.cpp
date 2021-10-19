#include "SimDataParse.h"

#define SPLIT_CHAR " "

#include<QFile>
#include <QStringList>
#include <QDebug>

SimDataParse::SimDataParse(void)
{
	
}


SimDataParse::~SimDataParse(void)
{
}

void SimDataParse::ParseSenorData( QString strFilePath, std::vector<Senor> &vecSenorData )
{
	// 
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString strLine;
	QStringList strList;
	RadarBeamInfo beamData;

	int nLineSize = 0;

	Senor senorData;

	int nIndex = 0;

	while (!file.atEnd()) 
	{
		strLine = file.readLine();

		if (strLine.isEmpty())
		{
			continue;
		}
		strList = strLine.split(SPLIT_CHAR);

		if (strList.size() < 11 )
		{
			// 
			continue;
		}

		nIndex = 0;
		nLineSize = strList.size();

		senorData.dTime = strList.at(nIndex++).toDouble();
		senorData.nEntityID = strList.at(nIndex++).toDouble();
		senorData.nSeorId = strList.at(nIndex++).toDouble();
		senorData.dAzStartAngle = strList.at(nIndex++).toDouble();
		senorData.dAzEndAngle = strList.at(nIndex++).toDouble();
		senorData.dPicthStartAngle = strList.at(nIndex++).toDouble();
		senorData.dPicthEndAngle = strList.at(nIndex++).toDouble();
		senorData.dRaduis = strList.at(nIndex++).toDouble();
		senorData.enState = (EnMachState)strList.at(nIndex++).toInt();
		senorData.dScanPeriod = strList.at(nIndex++).toDouble();
		senorData.nPrefer = strList.at(nIndex++).toDouble();
		int nBeaNum = 0;
		for (; nIndex < nLineSize;)
		{
			if (nLineSize - nIndex >= 6)
			{
				beamData.nBeamId = strList.at(nIndex++).toInt();
				beamData.dHorWidth = strList.at(nIndex++).toDouble();
				beamData.dVerWidth = strList.at(nIndex++).toDouble();
				beamData.dAzAngle = strList.at(nIndex++).toDouble();
				beamData.dPitchAngle = strList.at(nIndex++).toDouble();
				beamData.snTurn = strList.at(nIndex++).toDouble();
				beamData.nSenorId = senorData.nSeorId;
				senorData.beam[nBeaNum++]=beamData;
				
			}
			else
			{
				break;
			}
		}
		senorData.nBeamNum = nBeaNum;

		vecSenorData.push_back(senorData);
	}

}

void SimDataParse::ParseEventData( QString strFilePath,std::vector<EventCommand> &vecEvent )
{
	// 
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString strLine;
	QStringList strList;
	int nSize = 0;
	while (!file.atEnd()) 
	{
		EventCommand ecd;
		strLine = file.readLine();

		if (strLine.isEmpty())
		{
			continue;
		}

		strList = strLine.trimmed().split(SPLIT_CHAR,QString::SkipEmptyParts);

		if (strList.size() == 2)
		{
			// 
			ecd.dTime = strList.at(0).toDouble();
			std::string strMsg = strList.at(1).toStdString();

			nSize = strMsg.size() > DESCRIPT_TXT_MAX_LEN ? DESCRIPT_TXT_MAX_LEN : strMsg.size();
			memcpy(ecd.chDescript,strMsg.c_str(),nSize);
			vecEvent.push_back(ecd);
		}
	}
}

void SimDataParse::ParseViewData( QString strFilePath,std::vector<ViewInfoMsg> &vecViewCommandInfo )
{
	// 
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString strLine;
	QStringList strList;

	int nSize = 0;
	while (!file.atEnd()) 
	{
		strLine = file.readLine();

		if (strLine.isEmpty())
		{
			continue;
		}

		strList = strLine.trimmed().split(SPLIT_CHAR,QString::SkipEmptyParts);

		if (strList.size() > 2)
		{
			ViewInfoMsg viwInfoMsg;

			viwInfoMsg.dTime = strList.at(0).toDouble();

			viwInfoMsg.enViewType = (EnViewType) strList.at(1).toInt();

			switch(viwInfoMsg.enViewType)
			{
			case EN_LOCAT_VIEW:
				 {
					 if (parseViewLocal(strList,viwInfoMsg))
					 {
						 vecViewCommandInfo.push_back(viwInfoMsg);
					 }
				 }
				 break;
			case EN_PEER_VIEW:
				{
					if (parseViewPeer(strList,viwInfoMsg))
					{
						vecViewCommandInfo.push_back(viwInfoMsg);
					}
				}
				break;
			case EN_PEERE2T_VIEW:
				{
					if (parseViewE2T(strList,viwInfoMsg))
					{
						vecViewCommandInfo.push_back(viwInfoMsg);
					}
				}
				break;
			case EN_TRACK_VIEW:
				{
					if (parseViewTrack(strList,viwInfoMsg))
					{
						vecViewCommandInfo.push_back(viwInfoMsg);
					}
				}
				break;
			case EN_TRAVE_VIEW:
				{
					if (parseViewTrave(strList,viwInfoMsg))
					{
						vecViewCommandInfo.push_back(viwInfoMsg);
					}
				}
				break;
			case EN_TRAVETARGET_VIEW:
				{
					if (parseViewTraveTarget(strList,viwInfoMsg))
					{
						vecViewCommandInfo.push_back(viwInfoMsg);
					}
				}
				break;
			default:
				break;
			}
		}
	}
}


void SimDataParse::ParseScreenWord( QString strFilePath,std::vector<ScreenWordInfo> &vecScreenWordInfo )
{
	// 
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString strLine;
	QStringList strList;
	
	int nSize = 0;
	while (!file.atEnd()) 
	{
		strLine = file.readLine();

		if (strLine.isEmpty())
		{
			continue;
		}

		strList = strLine.trimmed().split(SPLIT_CHAR,QString::SkipEmptyParts);

		if (strList.size() == 2)
		{
			ScreenWordInfo scw;
			// 
			scw.dTime = strList.at(0).toDouble();
			std::string strMsg = strList.at(1).trimmed().toStdString();
		
			nSize = strMsg.size() > DESCRIPT_TXT_MAX_LEN ? DESCRIPT_TXT_MAX_LEN : strMsg.size();
			memcpy(scw.chMsg,strMsg.c_str(),nSize);
			vecScreenWordInfo.push_back(scw);
		}
	}
}

void SimDataParse::ParseDetectData( QString strFilePath,std::vector<DetectCommand> &vecTrackCommand )
{
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString strLine;
	QStringList strList;
	DetectCommand tcd;
	while (!file.atEnd()) 
	{
		strLine = file.readLine();

		if (strLine.isEmpty())
		{
			continue;
		}

		strList = strLine.trimmed().split(SPLIT_CHAR,QString::SkipEmptyParts);

		if (strList.size() == 4)
		{
			// 
			tcd.dTime = strList.at(0).toDouble();
			tcd.nEntityID = strList.at(1).toInt();
			tcd.nTargetId = strList.at(2).toInt();
			tcd.enTrackState = (EnTrackState)strList.at(3).toUInt();
			vecTrackCommand.push_back(tcd);
		}
	}
}

void SimDataParse::ParseHitData( QString strFilePath, std::vector<HitCommand> &vecHitCommand )
{
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString strLine;
	QStringList strList;
	while (!file.atEnd()) 
	{
		HitCommand hitCd;
		strLine = file.readLine();

		if (strLine.isEmpty())
		{
			continue;
		}

		strList = strLine.trimmed().split(SPLIT_CHAR,QString::SkipEmptyParts);

		if (strList.size() == 3)
		{
			// 
			hitCd.dTime = strList.at(0).toDouble();
			hitCd.nEntityID = strList.at(1).toInt();
			hitCd.nTargetId = strList.at(2).toInt();
			vecHitCommand.push_back(hitCd);
		}
	}
}

void SimDataParse::ParseDestryData( QString strFilePath,std::vector<DestoryCommand> &vecDestoryCommand )
{
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString strLine;
	QStringList strList;
	while (!file.atEnd()) 
	{
		DestoryCommand dcd;
		strLine = file.readLine();

		if (strLine.isEmpty())
		{
			continue;
		}

		strList = strLine.trimmed().split(SPLIT_CHAR,QString::SkipEmptyParts);

		if (strList.size() == 2)
		{
			// 
			dcd.dTime = strList.at(0).toDouble();
			dcd.nEntityID = strList.at(1).toInt();
			vecDestoryCommand.push_back(dcd);
		}
	}
}

void SimDataParse::ParseTracLineData( QString strFilePath, std::vector<EntityPtPs> &vecTrackPath )
{
	//
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString strLine;
	QStringList strList;
	EntityPtPs enPt;
	while (!file.atEnd()) 
	{
		strLine = file.readLine();
		strList = strLine.trimmed().split(SPLIT_CHAR,QString::SkipEmptyParts);

		if (strList.size() == 7)
		{
			// 
			enPt.dTime = strList.at(0).toDouble();
			// 经度
			enPt.pt.dX = strList.at(1).toDouble();
			// 纬度
			enPt.pt.dY = strList.at(2).toDouble();
			// 高度
 			enPt.pt.dZ = strList.at(3).toDouble();

			// 俯仰
			enPt.posture.dX = strList.at(5).toDouble();
			// 横滚
			enPt.posture.dY = strList.at(4).toDouble();
			// 偏航
			enPt.posture.dZ = strList.at(6).toDouble();
			vecTrackPath.push_back(enPt);
		}
	}

}

void SimDataParse::ParseAimMsgData( QString strFilePath, std::vector<AimInfoMsg> &vecAimInfos )
{
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;


	QString strLine;
	QStringList strList;
	while (!file.atEnd()) 
	{
		AimInfoMsg aimInfo;
		strLine = file.readLine();
		strList = strLine.trimmed().split(SPLIT_CHAR,QString::SkipEmptyParts);

		if (strList.size() == 5)
		{
			// 
			aimInfo.dTime = strList.at(0).toDouble();
			// 实体ID
			aimInfo.nEntityId = strList.at(1).toInt();
			// 目标ID
			aimInfo.nTargetId = strList.at(2).toInt();
			// 瞄准状态
			aimInfo.bAim = strList.at(3).toInt();
			// 持续时长
			aimInfo.dContinueTime = strList.at(4).toDouble();

			vecAimInfos.push_back(aimInfo);
		}

		
	}
}

void SimDataParse::ParseAirSpaceData( QString strFilePath, std::vector<AirSpeceSectorInfo> &vecAirSpaceInfo )
{
	//
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;


	QString strLine;
	QStringList strList;
	while (!file.atEnd()) 
	{
		AirSpeceSectorInfo airSpace;
		strLine = file.readLine();
		strList = strLine.trimmed().split(SPLIT_CHAR,QString::SkipEmptyParts);

		if (strList.size() == 12)
		{
			//时戳 id 名称 阵营(1-红 2-蓝) 中心经度 纬度 高度 半径 开始角度 角度范围 空域高度 状态
			// 
			airSpace.dTime = strList.at(0).toDouble();
			// 实体ID id
			airSpace.nEntityId = strList.at(1).toInt();
			// 名称
			QString strName = strList.at(2);
			memcpy(airSpace.chName,strName.toStdString().c_str(),strName.toStdString().size());
			// 阵营(1-红 2-蓝)
			airSpace.camp =(EnCamp)strList.at(3).toInt();
			// 中心经度
			airSpace.dLon = strList.at(4).toDouble();
			// 纬度
			airSpace.dLat = strList.at(5).toDouble();
			// 高度
			airSpace.dAlt = strList.at(6).toDouble();
			// 半径
			airSpace.dRadius = strList.at(7).toDouble();
			// 开始角度
			airSpace.dStartAngle = strList.at(8).toDouble();
			// 角度范围
			airSpace.dSwingAngle = strList.at(9).toDouble();
			// 空域高度
			airSpace.dH = strList.at(10).toDouble();

			airSpace.enType = (EnOperateType)strList.at(11).toInt();

			vecAirSpaceInfo.push_back(airSpace);
		}

	}
}

bool SimDataParse::parseViewLocal( const QStringList &strList, ViewInfoMsg &viewInfoMsg )
{
	bool bRet = false;
	if (strList.size() == 9)
	{
		ViewLocatCmd vcd;

		vcd.dFocalLon = strList.at(2).toDouble();

		vcd.dFocalLat = strList.at(3).toDouble();

		vcd.dFocalAlt = strList.at(4).toDouble();

		vcd.dHeading = strList.at(5).toDouble();

		vcd.dPitch = strList.at(6).toDouble();

		vcd.dRange = strList.at(7).toDouble();

		vcd.dTimeLen = strList.at(8).toDouble();

		memcpy(viewInfoMsg.chMsg,&vcd,sizeof(ViewLocatCmd));

		bRet = true;
	}
	
	return bRet;
}

bool SimDataParse::parseViewPeer( const QStringList &strList, ViewInfoMsg &viewInfoMsg )
{
	bool bRet = false;
	if (strList.size() == 6)
	{
		PeerView vcd;

		vcd.dFocalLon = strList.at(2).toDouble();

		vcd.dFocalLat = strList.at(3).toDouble();

		vcd.dFocalAlt = strList.at(4).toDouble();

		vcd.nEntityId = strList.at(5).toInt();

		memcpy(viewInfoMsg.chMsg,&vcd,sizeof(PeerView));

		bRet = true;
	}

	return bRet;
}

bool SimDataParse::parseViewTrave(const QStringList &strList, ViewInfoMsg &viewInfoMsg)
{
	bool bRet = false;
	if (strList.size() == 15)
	{
		TraveView vcd;

		int i = 2;

		vcd.dFocalLon = strList.at(i++).toDouble();

		vcd.dFocalLat = strList.at(i++).toDouble();

		vcd.dFocalAlt = strList.at(i++).toDouble();

		vcd.dHeading = strList.at(i++).toDouble();

		vcd.dPitch = strList.at(i++).toDouble();

		vcd.dRoll = strList.at(i++).toDouble();

		vcd.dFocalLon1 = strList.at(i++).toDouble();

		vcd.dFocalLat1 = strList.at(i++).toDouble();

		vcd.dFocalAlt1 = strList.at(i++).toDouble();

		vcd.dHeading1 = strList.at(i++).toDouble();

		vcd.dPitch1 = strList.at(i++).toDouble();

		vcd.dRoll1 = strList.at(i++).toDouble();

		vcd.dLenTime = strList.at(i++).toDouble();

		memcpy(viewInfoMsg.chMsg,&vcd,sizeof(TraveView));

		bRet = true;
	}

	return bRet;
}

bool SimDataParse::parseViewTraveTarget(const QStringList &strList, ViewInfoMsg &viewInfoMsg)
{
	bool bRet = false;
	if (strList.size() == 10)
	{
		TraveTargetView vcd;
		int i = 2;
		vcd.dFocalLon = strList.at(i++).toDouble();

		vcd.dFocalLat = strList.at(i++).toDouble();

		vcd.dFocalAlt = strList.at(i++).toDouble();

		vcd.dFocalLon1 = strList.at(i++).toDouble();

		vcd.dFocalLat1 = strList.at(i++).toDouble();

		vcd.dFocalAlt1 = strList.at(i++).toDouble();

		vcd.nTargetId = strList.at(i++).toInt();

		vcd.dLenTime = strList.at(i++).toDouble();

		memcpy(viewInfoMsg.chMsg,&vcd,sizeof(TraveTargetView));

		bRet = true;
	}

	return bRet;
}

bool SimDataParse::parseViewE2T( const QStringList &strList, ViewInfoMsg &viewInfoMsg )
{
	bool bRet = false;
	if (strList.size() == 4)
	{
		PeerE2TView vcd;

		vcd.nEntitId = strList.at(2).toDouble();

		vcd.nTargetId = strList.at(3).toDouble();

		memcpy(viewInfoMsg.chMsg,&vcd,sizeof(PeerE2TView));

		bRet = true;
	}

	return bRet;
}

bool SimDataParse::parseViewTrack( const QStringList &strList, ViewInfoMsg &viewInfoMsg )
{
	bool bRet = false;
	if (strList.size() == 7)
	{
		TracView vcd;
		vcd.enTrackType = (EnTrackType)strList.at(2).toInt();
		vcd.nTargetId = strList.at(3).toInt();
		vcd.dPitch = strList.at(4).toDouble();
		vcd.dAngle = strList.at(5).toDouble();
		vcd.dDis = strList.at(6).toDouble();
		memcpy(viewInfoMsg.chMsg,&vcd,sizeof(TracView));
		bRet = true;
	}

	return bRet;
}
