#include "DataEleConvertor.h"
#include <QMetaType>
#include <osg/Vec4>
#include "bdpDB/GxRegister.h"
#include "common/UnitCommon.h"
#include "bdpCommon/GxStrUtil.h"
#include "QSettings"
#include "osg/ref_ptr"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif




CDataEleConvertor::CDataEleConvertor()
{

}

CDataEleConvertor* CDataEleConvertor::getInstance()
{
	static osg::ref_ptr<CDataEleConvertor>  s_rpObjectManager = new CDataEleConvertor();
	return s_rpObjectManager.get();
}

std::string CDataEleConvertor::getEntityID(int id)
{
	return QString("%1").arg(id).toStdString();
}

std::string CDataEleConvertor::getSensorID(int sensorOwnerID, int sensorID)
{
	return QString("%1_%2").arg(sensorOwnerID).arg(sensorID).toStdString();
}

std::string CDataEleConvertor::getAirSpaceID(int enId)
{
	return QString("AirSpace_%1").arg(enId).toStdString();
}

std::string CDataEleConvertor::getCommAttriID(int nCommEffectOwnerEntityID)
{
	return QString("commEffect_%1").arg(nCommEffectOwnerEntityID).toStdString();
}

std::string CDataEleConvertor::getDetectAttriID(int nEntityID, int nTargetID)
{
	return QString("detect_%1_%2").arg(nEntityID).arg(nTargetID).toStdString();
}

std::string CDataEleConvertor::getHitEffectID(int nTargetId)
{
	return QString("hitEffect_%1").arg(nTargetId).toStdString();
}

std::string CDataEleConvertor::getAimEffectID( int nEntityID, int nTargetID )
{
	return QString("Aim_%1_%2").arg(nEntityID).arg(nTargetID).toStdString();
}

bool CDataEleConvertor::isCommEffectVisible(unsigned short usCommuState)
{
	return (LINK_CON == usCommuState);
}

osg::Vec4 CDataEleConvertor::getPathColor( int camp )
{
	osg::Vec4 color(1, 1, 1, 1);
	switch(camp)
	{
	case 1://红方
		color = osg::Vec4(255.0/255.0, 106.0/255.0, 106.0/255.0, 0.5);
		break;
	case 2://蓝方
		color = osg::Vec4(106.0/255.0, 165.0/255.0, 255.0/255.0, 0.5);
	default:
		break;
	}
	return color;
}

osg::Vec4 CDataEleConvertor::getModelColor( int camp )
{
	osg::Vec4 color(1, 1, 1, 1);
	switch(camp)
	{
	case 1://红方
		color = osg::Vec4(255.0/255.0, 18.0/255.0, 68.0/255.0, 1);
		break;
	case 2://蓝方
		color = osg::Vec4(57.0/255.0, 136.0/255.0, 255.0/255.0, 1);
	default:
		break;
	}
	return color;
}

SNewObjectData CDataEleConvertor::getEntityAttri(const NewEntity &newEntityInfo)
{
	SNewObjectData sNewAircraftObjectData;

	// 不同的业务实体类型显示样式等设置
	std::string modelPath = "";
	std::string imagePath = "";
	sNewAircraftObjectData.enEquType = newEntityInfo.enEquType;
	sNewAircraftObjectData.enCamp = newEntityInfo.enCamp;
	sNewAircraftObjectData.enSpaceType = newEntityInfo.enSpaceType;
	switch (newEntityInfo.enEquType)
	{
	case AIRCAFT_EUT:
	{
		modelPath = "Model/entity/su-27.ive";
		imagePath = "Model/F-18.png";
	}
	break;
	case TANK_EUT:
	{
		if (newEntityInfo.enCamp == BLUE_CAMP)
		{
			modelPath = "Model/entity/t-90_blue.ive";
		}
		else if (newEntityInfo.enCamp == RED_CAMP)
		{
			modelPath = "Model/entity/t-90_red.ive";
		}
		imagePath = "Model/Tank.png";
	}
	break;
	case MISSILE_VECHICLE_EUT:
	{
		if (newEntityInfo.enCamp == BLUE_CAMP)
		{
			modelPath = "Model/entity/DF-100_blue.ive";
		}
		else if (newEntityInfo.enCamp == RED_CAMP)
		{
			modelPath = "Model/entity/DF-100_red.ive";
		}
		imagePath = "Model/entity/DF-100.png";
	}
	break;
	case MISSILE_EUT:
	{
		modelPath = "Model/entity/daodan.ive";
		imagePath = "Model/entity/daodan.png";
	}
	break;
	case CARRIER_EUT:
	{
		modelPath = "Model/entity/hangmu.ive";
		imagePath = "Model/entity/hangmu.png";
	}
	break;
	case FRIGATE_EUT:
	{
		modelPath = "Model/054.ive";
		imagePath = "Model/054.png";
	}
	break;
	case COPTER_EUT:
	{
		modelPath = "Model/WZ_9.ive";
	}
	default:
		break;
	}

	if (newEntityInfo.sepNum == 0)
	{
		if (newEntityInfo.nComPriId > 0)
		{
			if (newEntityInfo.enEquType == MISSILE_EUT)
			{
				if (newEntityInfo.enCamp == BLUE_CAMP)
				{
					modelPath = "Model/entity/PL-15_blue.ive";
				}
				else if (newEntityInfo.enCamp == RED_CAMP)
				{
					modelPath = "Model/entity/PL-15_red.ive";
				}
			}
		}
	}

	if (newEntityInfo.sepNum == 0)
	{
		QString strName = newEntityInfo.cName;
		if (strName.contains("坦克"))
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 200, 30);
		}
		else if (strName.contains("导弹车") || strName.contains("指挥车"))
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 100, 20);
		}
		else if (strName.contains("飞机"))
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 120, 25);
		}
		else if (newEntityInfo.nComPriId > 0 && newEntityInfo.enEquType == MISSILE_EUT)
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 50, 2.0);
		}
		else
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 30, 2.5);
		}
	}
	else
	{

	}

	sNewAircraftObjectData.dTime = newEntityInfo.dTime;
	std::string strID = std::to_string(_Longlong(newEntityInfo.nEntityID));
	strcpy(sNewAircraftObjectData.strID, strID.c_str());
	strcpy(sNewAircraftObjectData.strModelFilePath, modelPath.c_str());

	strcpy(sNewAircraftObjectData.strName, newEntityInfo.cName);
	//sNewAircraftObjectData.sPreEquipInfo = m_mapEquipInfoData[strModelName];
	if (newEntityInfo.enCamp == 1)  //红方
	{
		sNewAircraftObjectData.modelColor[0] = 1.0;
		sNewAircraftObjectData.modelColor[1] = 0.0;
		sNewAircraftObjectData.modelColor[2] = 0.0;
		sNewAircraftObjectData.modelColor[3] = 1.0;
		sNewAircraftObjectData.nameColor[0] = 1.0;
		sNewAircraftObjectData.nameColor[1] = 0.0;
		sNewAircraftObjectData.nameColor[2] = 0.0;
		sNewAircraftObjectData.nameColor[3] = 1.0;
	}
	else if (newEntityInfo.enCamp == 2)//蓝方
	{
		sNewAircraftObjectData.modelColor[0] = 0.0;
		sNewAircraftObjectData.modelColor[1] = 0.0;
		sNewAircraftObjectData.modelColor[2] = 1.0;
		sNewAircraftObjectData.modelColor[3] = 1.0;
		sNewAircraftObjectData.nameColor[0] = 0.0;
		sNewAircraftObjectData.nameColor[1] = 0.0;
		sNewAircraftObjectData.nameColor[2] = 1.0;
		sNewAircraftObjectData.nameColor[3] = 1.0;
	}
	sNewAircraftObjectData.modelScale[0] = 10.0;
	sNewAircraftObjectData.modelScale[1] = 10.0;
	sNewAircraftObjectData.modelScale[2] = 10.0;
	sNewAircraftObjectData.nameFontSize = 20.0;
	sNewAircraftObjectData.imageImageSize[0] = 64.0;
	sNewAircraftObjectData.imageImageSize[1] = 64.0;

	strcpy(sNewAircraftObjectData.strImageFilePath, imagePath.c_str());
	sNewAircraftObjectData.vecPosture[0] = newEntityInfo.dPicthAngle;
	sNewAircraftObjectData.vecPosture[1] = newEntityInfo.dRolAngle; 
	sNewAircraftObjectData.vecPosture[2] = -newEntityInfo.dAzAngle;

	osg::Vec3d vecLLH = osg::Vec3d(newEntityInfo.dLon, newEntityInfo.dLat, newEntityInfo.dAlt);
// 	osg::Vec3d vecXYZ = osg::Vec3d(0, 0, 0);
// 	HGxAlgor::DegreeLLH2WGS84XYZ(vecLLH, vecXYZ);

	sNewAircraftObjectData.vecPositionLLH[0] = vecLLH.x();//lists.at(10).toDouble();
	sNewAircraftObjectData.vecPositionLLH[1] = vecLLH.y();//lists.at(11).toDouble();
	sNewAircraftObjectData.vecPositionLLH[2] = vecLLH.z();//lists.at(12).toDouble();

	sNewAircraftObjectData.nDriveStyle = 1;

	return sNewAircraftObjectData;
}



