/**************************************************************************************************
* @file FxModelConfigReader.h
* @note 模型列表
* @author w00085
* @data 2021-5-13
**************************************************************************************************/
#ifndef FX_MODEL_CONFIG_SET_MODEL_LIST_H
#define FX_MODEL_CONFIG_SET_MODEL_LIST_H

#include <vector>
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <public/common/UnitCommon.h>
#include <bdpDB/GxTinyXml.h>

namespace FxModelSet
{
	/**
	  * @class FxModelConfigReader
	  * @brief 模型数据集配置读写器
	  * @author w00085
	*/
	class CFxModelConfigReader : public HGxDB::CGxTiXmlVisitor
	{
	public:
		CFxModelConfigReader();
		/**
		*@note: 开始解析
		*/
		bool loadFromFile(const std::string& strCfgFilePath);
		/**
		*@note: 获取解析数据
		*/
		MapEquipInfoData GetLoadData();

		bool VisitEnter( const HGxDB::CGxTiXmlElement& ele , const HGxDB::CGxTiXmlAttribute* attr );

	private:
		MapEquipInfoData   m_mapEquipInfoData;
	};
}


#endif // FX_MODEL_CONFIG_SET_MODEL_LIST_H

