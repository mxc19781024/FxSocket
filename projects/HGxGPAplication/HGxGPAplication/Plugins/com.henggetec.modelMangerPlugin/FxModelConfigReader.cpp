#include <osgEarth/FileUtils>
#include <QString>
#include <bdpDB/GxTinyUtil.h>
#include <FxModelConfigReader.h>
#include <osgDB/ConvertUTF>
#include <bdpCommon/GxStrUtil.h>

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
			SModelAttribute sModelAttribute;

			sModelAttribute.sSPreEquipBsaeInfo.name = HGxCommon::utf8ToCurCode(ele.Attribute("Name"));
			sModelAttribute.sSPreEquipBsaeInfo.strtype = HGxCommon::utf8ToCurCode(ele.Attribute("Type"));
			sModelAttribute.sSPreEquipBsaeInfo.description = HGxCommon::utf8ToCurCode(ele.Attribute("Description"));
			//如果是相对路径 读取当前data下的全路径
			sModelAttribute.sSPreEquipBsaeInfo.model_path = HGxCommon::utf8ToCurCode(ele.Attribute("Model_path"));
			//图片路径
			sModelAttribute.sSPreEquipBsaeInfo.image_path = HGxCommon::utf8ToCurCode(ele.Attribute("Image_path"));
			QString str =  ele.Attribute("MaxVoyage");
			sModelAttribute.sSPreEquipAttachInfo.sEquipCap.maxVoyage =  str.toDouble();
			str =  ele.Attribute("MaxSpeed");
			sModelAttribute.sSPreEquipAttachInfo.sEquipCap.maxSpeed = str.toDouble();
			str =  ele.Attribute("MaxOil");
			sModelAttribute.sSPreEquipAttachInfo.sEquipCap.maxOil = str.toDouble();
			str =  ele.Attribute("AmmunitionNumber");
			sModelAttribute.sSPreEquipAttachInfo.sEquipCap.ammunitionNumber = str.toInt();
			sModelAttribute.sSPreEquipAttachInfo.sEquipCap.combatCapability = HGxCommon::utf8ToCurCode(ele.Attribute("CombatCapability"));
			sModelAttribute.sSPreEquipAttachInfo.sEquipCap.degreeDamage = HGxCommon::utf8ToCurCode(ele.Attribute("DegreeDamage"));
			str = ele.Attribute("Hit");
			sModelAttribute.sSPreEquipAttachInfo.sCombatCap.dHit = str.toDouble();
			str = ele.Attribute("Cruise");
			sModelAttribute.sSPreEquipAttachInfo.sCombatCap.dCruise = str.toDouble();
			str = ele.Attribute("Performance");
			sModelAttribute.sSPreEquipAttachInfo.sCombatCap.dPerformance = str.toDouble();
			str = ele.Attribute("Endurance");
			sModelAttribute.sSPreEquipAttachInfo.sCombatCap.dEndurance = str.toDouble();
			str = ele.Attribute("Coordinate");
			sModelAttribute.sSPreEquipAttachInfo.sCombatCap.dCoordinate = str.toDouble();
			str = ele.Attribute("Probe");
			sModelAttribute.sSPreEquipAttachInfo.sCombatCap.dProbe = str.toDouble();

			m_mapModelAttributeData[sModelAttribute.sSPreEquipBsaeInfo.name] = sModelAttribute;
		}
		return true;
	}

	MapModelAttributeData CFxModelConfigReader::GetModelAttributeData()
	{
		return m_mapModelAttributeData;
	}

}
