//#include <ICore.h>
//#include <CMainWindowManager.h>

#include "CServiceObject.h"
#include "CServerDataTree.h"
//#include <FreeXCommonUtils/StrUtil.h>
#include <bdpCommon/GxStrUtil.h>
#include <bdpDB/GxRegister.h>
#include <bdpDB/GxTinyXml.h>


CServiceObject::CServiceObject()
	: m_pServerDataTree(NULL)
{
 	if (!m_pServerDataTree)
 	{
 		m_pServerDataTree = new CServerDataTree;
 	}

	m_strFilePath = HGX_FILE_CONFIG().getPluginPath();
	//恒道云服务配置文件
	m_strHGServerFilePath = HGX_FILE_CONFIG().getFullPath("config/freex_server_ui/FreeXServer.xml");
	int a = 0;
	//ICore::mainWindowManager()->registerHandlerWhenClose(this, std::bind(&CServiceObject::saveWhenMainWndClose, this));
}


CServiceObject::~CServiceObject(void)
{

}

bool CServiceObject::load( const std::string &strFilePath/*, FxCore::IFxWorkspaceProcess* pProcess*/)
{
	// 0. 打开文件
	HGxDB::CGxTiXmlDocument doc;
	if (!doc.LoadFile((HGxCommon::utf8ToCurCode(m_strFilePath).c_str()), HGxDB::E_TIXML_ENCODING_UTF8))
	{
		return false;
	}
	const HGxDB::CGxTiXmlElement* pElePlugin = doc.FirstChildElement("Service");

	const HGxDB::CGxTiXmlElement* pElement = NULL;
	for (pElement = pElePlugin->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
	{
		std::string strElementValue = pElement->Value();
		if (strElementValue == "localServicelist")
		{
			QVector<SLinkInfo> vecLinkInfo;
			getLocalService(pElement, vecLinkInfo);
			loadLocalService(vecLinkInfo);
		}
		else if (strElementValue == "wmslist")
		{
			QVector<SNetService> vecServiceInfo;
			getOGCService(WMS, pElement, vecServiceInfo );
			loadNetService(WMS, vecServiceInfo);
		}
		else if (strElementValue == "wmtslist")
		{
			QVector<SNetService> vecServiceInfo;
			getOGCService(WMTS, pElement, vecServiceInfo );
			loadNetService(WMTS, vecServiceInfo);
		}
		else if (strElementValue == "wfslist")
		{
			QVector<SNetService> vecServiceInfo;
			getOGCService(WFS, pElement, vecServiceInfo );
			loadNetService(WFS, vecServiceInfo);
		}
		else if (strElementValue == "wcslist")
		{
			QVector<SNetService> vecServiceInfo;
			getOGCService(WCS, pElement, vecServiceInfo );
			loadNetService(WCS, vecServiceInfo);
		}
		else if (strElementValue == "tmslist")
		{
			QVector<SNetService> vecServiceInfo;
			getTileService(TMS, pElement, vecServiceInfo);
			loadNetService(TMS, vecServiceInfo);
		}
		else if (strElementValue == "xyzlist")
		{
			QVector<SNetService> vecServiceInfo;
			getTileService(XYZ, pElement, vecServiceInfo);
			loadNetService(XYZ, vecServiceInfo);
		}
		else if (strElementValue == "datasetlist")
		{
			QVector<SNetService> vecDatasetInfo;
			getDataset( pElement, vecDatasetInfo);
			loadNetService(DATASETS, vecDatasetInfo);
		}
	}

	//加载恒道云服务
	loadHGServer(m_strHGServerFilePath);

	return false;
}

bool CServiceObject::loadHGServer(const std::string &strFilePath/*, FxCore::IFxWorkspaceProcess* pProcess*/)
{
	// 0. 打开文件
	HGxDB::CGxTiXmlDocument doc;
	if (!doc.LoadFile((HGxCommon::utf8ToCurCode(strFilePath).c_str()), HGxDB::E_TIXML_ENCODING_UTF8))
	{
		return false;
	}
	const HGxDB::CGxTiXmlElement* pElePlugin = doc.FirstChildElement("Service");

	const HGxDB::CGxTiXmlElement* pElement = NULL;
	for (pElement = pElePlugin->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
	{
		std::string strElementValue = pElement->Value();
		if (strElementValue == "wmslist")
		{
			QVector<SNetService> vecServiceInfo;
			getOGCService(WMS, pElement, vecServiceInfo);
			loadHGNetService(WMS, vecServiceInfo);
		}
		else if (strElementValue == "wmtslist")
		{
			QVector<SNetService> vecServiceInfo;
			getOGCService(WMTS, pElement, vecServiceInfo);
			loadHGNetService(WMTS, vecServiceInfo);
		}
		else if (strElementValue == "wfslist")
		{
			QVector<SNetService> vecServiceInfo;
			getOGCService(WFS, pElement, vecServiceInfo);
			loadHGNetService(WFS, vecServiceInfo);
		}
		else if (strElementValue == "wcslist")
		{
			QVector<SNetService> vecServiceInfo;
			getOGCService(WCS, pElement, vecServiceInfo);
			loadHGNetService(WCS, vecServiceInfo);
		}
		else if (strElementValue == "tmslist")
		{
			QVector<SNetService> vecServiceInfo;
			getTileService(TMS, pElement, vecServiceInfo);
			loadHGNetService(TMS, vecServiceInfo);
		}
	}
	return false;
}

bool CServiceObject::save(const std::string &strFilePath/*, FxCore::IFxWorkspaceProcess* pProcess*/)
{
	// 服务插件需要独立一个文件来进行维护
	HGxDB::CGxTiXmlDocument doc(HGxCommon::utf8ToCurCode(m_strFilePath).c_str());
	HGxDB::CGxTiXmlDeclaration declar("1.0", "UTF-8", "");
	doc.InsertEndChild(declar);

	// 新建服务节点
	HGxDB::CGxTiXmlElement* pElePlugin = new HGxDB::CGxTiXmlElement("Service");
	doc.LinkEndChild(pElePlugin);

	if (!m_pServerDataTree)
	{
		return false;
	}

	HGxDB::CGxTiXmlElement *pLocalserListEle = new HGxDB::CGxTiXmlElement("localServicelist");
	
	// 保存本地服务信息
	QVector<SLinkInfo> vecLinkInfo;
	m_pServerDataTree->getLocalserviceList(vecLinkInfo);

	auto iter = vecLinkInfo.begin();
	for (; iter != vecLinkInfo.end(); ++iter)
	{
		HGxDB::CGxTiXmlElement *pLocalserverEle = new HGxDB::CGxTiXmlElement("localservice");

		HGxDB::CGxTiXmlElement *pLinkFileNameEle = new HGxDB::CGxTiXmlElement("linkfileName");
		HGxDB::CGxTiXmlText * linkFileNameText = new HGxDB::CGxTiXmlText(iter->strFileName.toUtf8().data());
		pLinkFileNameEle->LinkEndChild(linkFileNameText);

		HGxDB::CGxTiXmlElement *pLinkFilePathEle = new HGxDB::CGxTiXmlElement("linkfilePath");
		HGxDB::CGxTiXmlText * linkFilePathText = new HGxDB::CGxTiXmlText(iter->strAbsoluteFilePath.toUtf8().data());
		pLinkFilePathEle->LinkEndChild(linkFilePathText);

		if (iter->eLinkType == FOLDERLINK)
		{
			pLocalserverEle->SetAttribute("LinkType", "FolderLink");
		}
		else if (iter->eLinkType == FILELINK)
		{
			pLocalserverEle->SetAttribute("LinkType", "FileLink");
		}
		pLocalserverEle->LinkEndChild(pLinkFileNameEle);
		pLocalserverEle->LinkEndChild(pLinkFilePathEle);
		pLocalserListEle->LinkEndChild(pLocalserverEle);
	}
	
	// 1.保存OGC网络服务信息
	// 1.1 WMS
	HGxDB::CGxTiXmlElement *pWMSListEle = new HGxDB::CGxTiXmlElement("wmslist");
	saveOGCServiceInfo(WMS, pWMSListEle);
	
	// 1.2 WMS
	HGxDB::CGxTiXmlElement *pWMTSListEle = new HGxDB::CGxTiXmlElement("wmtslist");
	saveOGCServiceInfo(WMTS, pWMTSListEle);

	// 1.3 WMS
	HGxDB::CGxTiXmlElement *pWFSListEle = new HGxDB::CGxTiXmlElement("wfslist");
	saveOGCServiceInfo(WFS, pWFSListEle);
	
	// 1.4 WMS
	HGxDB::CGxTiXmlElement *pWCSListEle = new HGxDB::CGxTiXmlElement("wcslist");
	saveOGCServiceInfo(WCS, pWCSListEle);
	
	// 1.5 TMS
	HGxDB::CGxTiXmlElement *pTMSListEle = new HGxDB::CGxTiXmlElement("tmslist");
	saveTileServiceInfo(TMS, pTMSListEle);

	// 1.6 XYZ
	HGxDB::CGxTiXmlElement *pXYZListEle = new HGxDB::CGxTiXmlElement("xyzlist");
	saveTileServiceInfo(XYZ, pXYZListEle);
	
	// 1.7 DATASET
	HGxDB::CGxTiXmlElement *pDatasetListEle = new HGxDB::CGxTiXmlElement("datasetlist");
	saveDatesetInfo(pDatasetListEle);
	
	pElePlugin->LinkEndChild(pLocalserListEle);
	pElePlugin->LinkEndChild(pWMSListEle);
	pElePlugin->LinkEndChild(pWMTSListEle);
	pElePlugin->LinkEndChild(pWFSListEle);
	pElePlugin->LinkEndChild(pWCSListEle);
	pElePlugin->LinkEndChild(pTMSListEle);
	pElePlugin->LinkEndChild(pXYZListEle);
	pElePlugin->LinkEndChild(pDatasetListEle);

	return doc.SaveFile();
}

QWidget * CServiceObject::getWgt() const
{
	CServerDataTree *pServerWgt = NULL;

	if (m_pServerDataTree)
	{
		pServerWgt = m_pServerDataTree;
	}

	return pServerWgt;
}

void CServiceObject::saveOGCServiceInfo( ENetServiceType type, HGxDB::CGxTiXmlElement *pEle )
{
	QVector<SNetService> vecNetService;
	m_pServerDataTree->getNetServiceList(type, vecNetService);

	std::string strTag = "";
	switch (type)
	{
	case WMS:
		strTag = "WMS";
		break; 
	case WMTS:
		strTag = "WMTS";
		break;
	case WFS:
		strTag = "WFS";
		break;
	case WCS:
		strTag = "WCS";
		break;
	default:
		break;
	}
	auto iter = vecNetService.begin();
	for (; iter != vecNetService.end(); ++iter)
	{
		HGxDB::CGxTiXmlElement *pOGCEle = new HGxDB::CGxTiXmlElement(strTag.c_str());

		HGxDB::CGxTiXmlElement *pServerNameEle = new HGxDB::CGxTiXmlElement("servername");
		HGxDB::CGxTiXmlText * serverNameText = new HGxDB::CGxTiXmlText(iter->strServerName.toUtf8().data());
		pServerNameEle->LinkEndChild(serverNameText);

		HGxDB::CGxTiXmlElement *pServerVersionEle = new HGxDB::CGxTiXmlElement("serverversion");
		HGxDB::CGxTiXmlText * serverVersionText = new HGxDB::CGxTiXmlText(iter->strServerVersion.toUtf8().data());
		pServerVersionEle->LinkEndChild(serverVersionText);

		HGxDB::CGxTiXmlElement *pServerWebsiteEle = new HGxDB::CGxTiXmlElement("serverwebsite");
		HGxDB::CGxTiXmlText * serverWebsiteText = new HGxDB::CGxTiXmlText(iter->strServerWebsite.toUtf8().data());
		pServerWebsiteEle->LinkEndChild(serverWebsiteText);

		HGxDB::CGxTiXmlElement *pUserNameEle = new HGxDB::CGxTiXmlElement("username");
		HGxDB::CGxTiXmlText * nameText = new HGxDB::CGxTiXmlText(iter->strUserName.toUtf8().data());
		pUserNameEle->LinkEndChild(nameText);

		HGxDB::CGxTiXmlElement *pPasswordEle = new HGxDB::CGxTiXmlElement("password");
		HGxDB::CGxTiXmlText * passwordText = new HGxDB::CGxTiXmlText(iter->strPassword.toUtf8().data());
		pPasswordEle->LinkEndChild(passwordText);

		HGxDB::CGxTiXmlElement *pDefaultEle = new HGxDB::CGxTiXmlElement("defaultLayer");
		HGxDB::CGxTiXmlText * defaultText = new HGxDB::CGxTiXmlText(HGxCommon::boolToString(iter->bDefault).c_str());
		pDefaultEle->LinkEndChild(defaultText);

		pOGCEle->LinkEndChild(pServerNameEle);
		pOGCEle->LinkEndChild(pServerVersionEle);
		pOGCEle->LinkEndChild(pServerWebsiteEle);
		pOGCEle->LinkEndChild(pUserNameEle);
		pOGCEle->LinkEndChild(pPasswordEle);
		pOGCEle->LinkEndChild(pDefaultEle);
		pEle->LinkEndChild(pOGCEle);
	}
}

void CServiceObject::saveTileServiceInfo( ENetServiceType type, HGxDB::CGxTiXmlElement *pEle )
{
	QVector<SNetService> vecNetService;
	m_pServerDataTree->getNetServiceList(type, vecNetService);

	std::string strTag = "";
	switch (type)
	{
	case TMS:
		strTag = "TMS";
		break;
	case XYZ:
		strTag = "XYZ";
		break;
	default:
		break;
	}
	auto iter = vecNetService.begin();
	for (; iter != vecNetService.end(); ++iter)
	{
		HGxDB::CGxTiXmlElement *pTileEle = new HGxDB::CGxTiXmlElement(strTag.c_str());

		HGxDB::CGxTiXmlElement *pServerNameEle = new HGxDB::CGxTiXmlElement("servername");
		HGxDB::CGxTiXmlText * serverNameText = new HGxDB::CGxTiXmlText(iter->strServerName.toUtf8().data());
		pServerNameEle->LinkEndChild(serverNameText);

		HGxDB::CGxTiXmlElement *pServerWebsiteEle = new HGxDB::CGxTiXmlElement("serverwebsite");
		HGxDB::CGxTiXmlText * serverWebsiteText = new HGxDB::CGxTiXmlText(iter->strServerWebsite.toUtf8().data());
		pServerWebsiteEle->LinkEndChild(serverWebsiteText);

		HGxDB::CGxTiXmlElement *pLayerTypeEle = new HGxDB::CGxTiXmlElement("layertype");
		HGxDB::CGxTiXmlText * layertypeText = new HGxDB::CGxTiXmlText(iter->strLayerType.toUtf8().data());
		pLayerTypeEle->LinkEndChild(layertypeText);

		HGxDB::CGxTiXmlElement *pLayerNameEle = new HGxDB::CGxTiXmlElement("layername");
		HGxDB::CGxTiXmlText * layerNameText = new HGxDB::CGxTiXmlText(iter->strLayerName.toUtf8().data());
		pLayerNameEle->LinkEndChild(layerNameText);

		HGxDB::CGxTiXmlElement *pDefaultEle = new HGxDB::CGxTiXmlElement("defaultLayer");
		HGxDB::CGxTiXmlText * defaultText = new HGxDB::CGxTiXmlText(HGxCommon::boolToString(iter->bDefault).c_str());
		pDefaultEle->LinkEndChild(defaultText);

		pTileEle->LinkEndChild(pServerNameEle);
		pTileEle->LinkEndChild(pServerWebsiteEle);
		pTileEle->LinkEndChild(pLayerTypeEle);
		pTileEle->LinkEndChild(pLayerNameEle);
		pTileEle->LinkEndChild(pDefaultEle);
		pEle->LinkEndChild(pTileEle);
	}
}

void CServiceObject::saveDatesetInfo( HGxDB::CGxTiXmlElement *pEle )
{
	QVector<SNetService> vecNetService;
	m_pServerDataTree->getNetServiceList(DATASETS, vecNetService);
	auto iter = vecNetService.begin();
	for (; iter != vecNetService.end(); ++iter)
	{
		HGxDB::CGxTiXmlElement *pDatasetEle = new HGxDB::CGxTiXmlElement("dataset");

		HGxDB::CGxTiXmlElement *pDriverNameEle = new HGxDB::CGxTiXmlElement("drivename");
		HGxDB::CGxTiXmlText * driverNameText = new HGxDB::CGxTiXmlText(iter->strDriverName.toUtf8().data());
		pDriverNameEle->LinkEndChild(driverNameText);

		HGxDB::CGxTiXmlElement *pServerNameEle = new HGxDB::CGxTiXmlElement("servername");
		HGxDB::CGxTiXmlText * serverNameText = new HGxDB::CGxTiXmlText(iter->strServerName.toUtf8().data());
		pServerNameEle->LinkEndChild(serverNameText);

		HGxDB::CGxTiXmlElement *pServerWebsiteEle = new HGxDB::CGxTiXmlElement("serverwebsite");
		HGxDB::CGxTiXmlText * serverWebsiteText = new HGxDB::CGxTiXmlText(iter->strServerWebsite.toUtf8().data());
		pServerWebsiteEle->LinkEndChild(serverWebsiteText);

		HGxDB::CGxTiXmlElement *pLayerTypeEle = new HGxDB::CGxTiXmlElement("layertype");
		HGxDB::CGxTiXmlText * layertypeText = new HGxDB::CGxTiXmlText(iter->strLayerType.toUtf8().data());
		pLayerTypeEle->LinkEndChild(layertypeText);

		HGxDB::CGxTiXmlElement *pLayerNameEle = new HGxDB::CGxTiXmlElement("layername");
		HGxDB::CGxTiXmlText * layerNameText = new HGxDB::CGxTiXmlText(iter->strLayerName.toUtf8().data());
		pLayerNameEle->LinkEndChild(layerNameText);

		HGxDB::CGxTiXmlElement *pDefaultEle = new HGxDB::CGxTiXmlElement("defaultLayer");
		HGxDB::CGxTiXmlText * defaultText = new HGxDB::CGxTiXmlText(HGxCommon::boolToString(iter->bDefault).c_str());
		pDefaultEle->LinkEndChild(defaultText);

		pDatasetEle->LinkEndChild(pDriverNameEle);
		pDatasetEle->LinkEndChild(pServerNameEle);
		pDatasetEle->LinkEndChild(pServerWebsiteEle);
		pDatasetEle->LinkEndChild(pLayerTypeEle);
		pDatasetEle->LinkEndChild(pLayerNameEle);
		pDatasetEle->LinkEndChild(pDefaultEle);
		pEle->LinkEndChild(pDatasetEle);
	}
}

void CServiceObject::getLocalService( const HGxDB::CGxTiXmlElement *pEle , QVector<SLinkInfo>& vecServiceInfo )
{
	const HGxDB::CGxTiXmlElement* pElement = NULL;
	for (pElement = pEle->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
	{
		const HGxDB::CGxTiXmlElement* pLinkFileNameElement = pElement->FirstChildElement("linkfileName");
		const HGxDB::CGxTiXmlElement* pLinkFilePathElement = pLinkFileNameElement->NextSiblingElement("linkfilePath");

		SLinkInfo stLinkInfo;
		std::string strLinkType = pElement->Attribute("LinkType");
		if (strLinkType == "FileLink")
		{
			stLinkInfo.eLinkType = FILELINK;
		}
		else if (strLinkType == "FolderLink")
		{
			stLinkInfo.eLinkType = FOLDERLINK;
		}
		stLinkInfo.strFileName = QString::fromUtf8(pLinkFileNameElement->GetText());
		stLinkInfo.strAbsoluteFilePath = QString::fromUtf8(pLinkFilePathElement->GetText());

		vecServiceInfo.push_back(stLinkInfo);
	}
}

void CServiceObject::getOGCService( ENetServiceType type, const HGxDB::CGxTiXmlElement *pEle , QVector<SNetService>& vecServiceInfo )
{
	const HGxDB::CGxTiXmlElement* pElement = NULL;
	for (pElement = pEle->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
	{
		const HGxDB::CGxTiXmlElement* pServerNameElement = pElement->FirstChildElement("servername");
		const HGxDB::CGxTiXmlElement* pServerVersionElement = pServerNameElement->NextSiblingElement("serverversion");
		const HGxDB::CGxTiXmlElement* pServerWebsiteElement = pServerNameElement->NextSiblingElement("serverwebsite");
		const HGxDB::CGxTiXmlElement* pServerUsernameElement = pServerNameElement->NextSiblingElement("username");
		const HGxDB::CGxTiXmlElement* pServerPasswordElement = pServerNameElement->NextSiblingElement("password");
		const HGxDB::CGxTiXmlElement* pDefaultLayerElement = pServerNameElement->NextSiblingElement("defaultLayer");

		SNetService stOGCStruct;
		stOGCStruct.strServerType = QString::fromUtf8(pElement->Value());
		stOGCStruct.strServerName = QString::fromUtf8(pServerNameElement->GetText());
		stOGCStruct.strServerVersion = QString::fromUtf8(pServerVersionElement->GetText());
		stOGCStruct.strServerWebsite = QString::fromUtf8(pServerWebsiteElement->GetText());
		stOGCStruct.strUserName = QString::fromUtf8(pServerUsernameElement->GetText());
		stOGCStruct.strPassword = QString::fromUtf8(pServerPasswordElement->GetText());
		stOGCStruct.bDefault = HGxCommon::stringToBool(pDefaultLayerElement->GetText());

		vecServiceInfo.push_back(stOGCStruct);
	}
}

void CServiceObject::getTileService( ENetServiceType type, const HGxDB::CGxTiXmlElement *pEle , QVector<SNetService>& vecServiceInfo )
{
	const HGxDB::CGxTiXmlElement* pElement = NULL;
	for (pElement = pEle->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
	{
		const HGxDB::CGxTiXmlElement* pServerNameElement = pElement->FirstChildElement("servername");
		const HGxDB::CGxTiXmlElement* pServerWebsiteElement = pServerNameElement->NextSiblingElement("serverwebsite");
		const HGxDB::CGxTiXmlElement* pLayerTypeElement = pServerWebsiteElement->NextSiblingElement("layertype");
		const HGxDB::CGxTiXmlElement* pLayerNameElement = pLayerTypeElement->NextSiblingElement("layername");
		const HGxDB::CGxTiXmlElement* pDefaultLayerElement = pLayerTypeElement->NextSiblingElement("defaultLayer");

		SNetService stTileStruct;
		stTileStruct.strServerType = QString::fromUtf8(pElement->Value());
		stTileStruct.strServerName = QString::fromUtf8(pServerNameElement->GetText());
		stTileStruct.strServerWebsite = QString::fromUtf8(pServerWebsiteElement->GetText());
		stTileStruct.strLayerType = QString::fromUtf8(pLayerTypeElement->GetText());
		stTileStruct.strLayerName = QString::fromUtf8(pLayerNameElement->GetText());
		stTileStruct.bDefault = HGxCommon::stringToBool(pDefaultLayerElement->GetText());
		vecServiceInfo.push_back(stTileStruct);
	}
}

void CServiceObject::getDataset( const HGxDB::CGxTiXmlElement *pEle , QVector<SNetService>& vecServiceInfo )
{
	const HGxDB::CGxTiXmlElement* pElement = NULL;
	for (pElement = pEle->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
	{
		const HGxDB::CGxTiXmlElement* pDriveNameElement = pElement->FirstChildElement("drivename");
		const HGxDB::CGxTiXmlElement* pServerNameElement = pDriveNameElement->NextSiblingElement("servername");
		const HGxDB::CGxTiXmlElement* pServerWebsiteElement = pServerNameElement->NextSiblingElement("serverwebsite");
		const HGxDB::CGxTiXmlElement* pLayerTypeElement = pServerWebsiteElement->NextSiblingElement("layertype");
		const HGxDB::CGxTiXmlElement* pLayerNameElement = pLayerTypeElement->NextSiblingElement("layername");
		const HGxDB::CGxTiXmlElement* pDefaultLayerElement = pLayerTypeElement->NextSiblingElement("defaultLayer");

 		SNetService stDatasetStruct;
 		stDatasetStruct.strServerName = QString::fromUtf8(pServerNameElement->GetText());
 		stDatasetStruct.strServerWebsite = QString::fromUtf8(pServerWebsiteElement->GetText());
		stDatasetStruct.strDriverName = QString::fromUtf8(pDriveNameElement->GetText());
		stDatasetStruct.strLayerType = QString::fromUtf8(pLayerTypeElement->GetText());
		stDatasetStruct.strLayerName = QString::fromUtf8(pLayerNameElement->GetText());
		stDatasetStruct.bDefault = HGxCommon::stringToBool(pDefaultLayerElement->GetText());

 		vecServiceInfo.push_back(stDatasetStruct);
	}
}

void CServiceObject::loadLocalService( const QVector<SLinkInfo>& vecLinkInfo )
{
	if (m_pServerDataTree)
	{
		m_pServerDataTree->restoreLocalServer(vecLinkInfo);
	}
}

void CServiceObject::loadNetService( ENetServiceType type, const QVector<SNetService>& vecServiceInfo )
{
	if (m_pServerDataTree)
	{
		m_pServerDataTree->restoreNetServer(type, vecServiceInfo);
	}
}

void CServiceObject::loadHGNetService(ENetServiceType type, const QVector<SNetService>& vecServiceInfo)
{
	if (m_pServerDataTree)
	{
		m_pServerDataTree->restoreHGNetServer(type, vecServiceInfo);
	}
}

bool CServiceObject::saveWhenMainWndClose()
{
	save();
	return true;
}
