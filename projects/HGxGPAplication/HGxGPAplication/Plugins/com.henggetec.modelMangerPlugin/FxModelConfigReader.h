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
#include <public/ModelStructDef.h>
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
		MapModelAttributeData GetModelAttributeData();

		bool VisitEnter( const HGxDB::CGxTiXmlElement& ele , const HGxDB::CGxTiXmlAttribute* attr );

	private:
		MapModelAttributeData   m_mapModelAttributeData;
	};
}


#endif // FX_MODEL_CONFIG_SET_MODEL_LIST_H

