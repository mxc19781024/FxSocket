#include <osgEarth/FileUtils>
#include <QString>
#include <bdpDB/GxTinyUtil.h>
#include <FxModelConfigReader.h>
#include <osgDB/ConvertUTF>
#include "bdpCommon/GxStrUtil.h"

namespace FxModelSet
{
	CFxModelConfigReader::CFxModelConfigReader()
		: HGxDB::CGxTiXmlVisitor()
	{

	}
	bool CFxModelConfigReader::loadFromFile(const std::string& strCfgFilePath)
	{
		if(!strCfgFilePath.empty())
		{
			HGxDB::CGxTiXmlDocument document(HGxCommon::utf8ToCurCode(strCfgFilePath).c_str());

			if(document.LoadFile(HGxDB::E_TIXML_ENCODING_UTF8))
			{
				if(document.Accept(this))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool CFxModelConfigReader::VisitEnter( const HGxDB::CGxTiXmlElement& ele , const HGxDB::CGxTiXmlAttribute* attr )
	{
		if(HGxDB::TinyUtil::AcceptKey("Unit", ele.Value()))
		{	
			SPreEquipInfo sPreEquipInfo;
		    sPreEquipInfo.name = HGxCommon::utf8ToCurCode(ele.Attribute("Name"));
			sPreEquipInfo.strtype = HGxCommon::utf8ToCurCode(ele.Attribute("Type"));
			sPreEquipInfo.description = HGxCommon::utf8ToCurCode(ele.Attribute("Description"));
			//如果是相对路径 读取当前data下的全路径
			sPreEquipInfo.model_path = HGxCommon::utf8ToCurCode(ele.Attribute("Model_path"));
			//图片路径
			sPreEquipInfo.image_path = HGxCommon::utf8ToCurCode(ele.Attribute("Image_path"));
			QString str =  ele.Attribute("MaxVoyage");
			sPreEquipInfo.maxVoyage =  str.toDouble();
			str =  ele.Attribute("MaxSpeed");
			sPreEquipInfo.maxSpeed = str.toDouble();
			str =  ele.Attribute("MaxOil");
			sPreEquipInfo.maxOil = str.toDouble();
			str =  ele.Attribute("AmmunitionNumber");
			sPreEquipInfo.ammunitionNumber = str.toInt();
			sPreEquipInfo.combatCapability = HGxCommon::utf8ToCurCode(ele.Attribute("CombatCapability"));
			sPreEquipInfo.degreeDamage = HGxCommon::utf8ToCurCode(ele.Attribute("DegreeDamage"));
			str = ele.Attribute("Hit");
			sPreEquipInfo.sPreEquipCap.dHit = str.toDouble();
			str = ele.Attribute("Cruise");
			sPreEquipInfo.sPreEquipCap.dCruise = str.toDouble();
			str = ele.Attribute("Performance");
			sPreEquipInfo.sPreEquipCap.dPerformance = str.toDouble();
			str = ele.Attribute("Endurance");
			sPreEquipInfo.sPreEquipCap.dEndurance = str.toDouble();
			str = ele.Attribute("Coordinate");
			sPreEquipInfo.sPreEquipCap.dCoordinate = str.toDouble();
			str = ele.Attribute("Probe");
			sPreEquipInfo.sPreEquipCap.dProbe = str.toDouble();

			m_mapEquipInfoData[sPreEquipInfo.name] = sPreEquipInfo;
		}
		return true;
	}

	MapEquipInfoData CFxModelConfigReader::GetLoadData()
	{
		return m_mapEquipInfoData;
	}

}
