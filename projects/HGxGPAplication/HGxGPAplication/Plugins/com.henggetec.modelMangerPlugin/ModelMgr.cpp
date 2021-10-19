#include <ModelMgr.h>
#include <bdpDB/GxLogFunc.h>
#include <QVariant>
#include "FxModelConfigReader.h"
#include <bdpDB\GxRegister.h>


#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

namespace FxDrive
{
	CModelMgr::CModelMgr()
	{
		FxModelSet::CFxModelConfigReader wr;
		if (!wr.loadFromFile(HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("/UserConfig/modelconfig.xml")))
		{
			LOG(E_LOG_NOTICE) << "load model list file falied ";
		}
		m_modelAttributeObjectsIndexMap = wr.GetModelAttributeData();
	}

	CModelMgr::~CModelMgr()
	{
		
	}


	void CModelMgr::getModelAttribute(std::string strName, SModelAttribute& modelAttribute)
	{
		if (m_modelAttributeObjectsIndexMap.contains(strName))
		{
			modelAttribute = m_modelAttributeObjectsIndexMap[strName];
		}
	}

}
