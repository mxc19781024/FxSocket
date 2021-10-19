#ifndef CSERVICE_OBJECT_H
#define CSERVICE_OBJECT_H



#include <ctkPluginContext.h>
#include <service_ui/IServiceUiInterface.h>
//#include "FreeXServiceUtilUi/Common.h"
#include "public/Common.h"

class CServerDataTree;

namespace HGxDB
{
	class CGxTiXmlElement;
}

class CServiceObject : public QObject, public IServiceUiInterface
{
	Q_OBJECT
	Q_INTERFACES(IServiceUiInterface)
public:
	CServiceObject();
	~CServiceObject(void);

	/**  
		* @brief 加载文档
		* @param pEle [in] xml节点，包含需要解析的内容
		* @param pProcess [in] 用于监测处理进度
		* @return 是否加载成功
	*/
	bool load( const std::string &strFilePath = ""/*, FxCore::IFxWorkspaceProcess* pProcess=NULL*/) override;
	
	//加载恒道云服务
	bool loadHGServer(const std::string &strFilePath = ""/*, FxCore::IFxWorkspaceProcess* pProcess = NULL*/);

	/**  
		* @brief 保存文档
		* @param pEle [in] xml节点，包含需要解析的内容
		* @param pProcess [in] 用于监测处理进度
		* @return 是否保存成功
	*/
	bool save( const std::string &strFilePath = ""/*, FxCore::IFxWorkspaceProcess* pProcess=NULL*/) override;


	/**  
		* @brief 获取服务界面指针
		* @return 服务界面指针
	*/
	QWidget *getWgt() const override;


private:
	/**  
		* @brief 保存OGC服务信息
		* @note add by w00025
		* @param type [in] OGC服务类型
		* @return 服务界面指针
	*/
	void saveOGCServiceInfo(ENetServiceType type, HGxDB::CGxTiXmlElement *pEle);

	/**  
		* @brief 保存Tile服务信息
		* @note add by w00025
		* @param type [in] OGC服务类型
		* @return 服务界面指针
	*/
	void saveTileServiceInfo(ENetServiceType type, HGxDB::CGxTiXmlElement *pEle);

	/**  
		* @brief 保存数据集信息
		* @note add by w00025
		* @return 服务界面指针
	*/
	void saveDatesetInfo(HGxDB::CGxTiXmlElement *pEle);

	/** 
	* @brief 反序列化获取本地服务信息
	* @note add by w00025
	* @param pEle[in] xml节点
	* @param vecServiceInfo[out] 服务数据
	*/
	void getLocalService( const HGxDB::CGxTiXmlElement *pEle , QVector<SLinkInfo>& vecServiceInfo );

	/** 
	* @brief 反序列化获取OGC服务信息
	* @note add by w00025
	* @param type[in] 服务类型
	* @param pEle[in] xml节点
	* @param vecServiceInfo[out] 服务数据
	*/
	void getOGCService( ENetServiceType type, const HGxDB::CGxTiXmlElement *pEle , QVector<SNetService>& vecServiceInfo);

	/** 
	* @brief 反序列化获取Tile服务信息
	* @note add by w00025
	* @param type[in] 服务类型
	* @param pEle[in] xml节点
	* @param vecServiceInfo[out] 服务数据
	*/
	void getTileService( ENetServiceType type, const HGxDB::CGxTiXmlElement *pEle , QVector<SNetService>& vecServiceInfo);

	/** 
	* @brief 反序列化获取数据集信息
	* @note add by w00025
	* @param pEle[in] xml节点
	* @param vecServiceInfo[out] 服务数据
	*/
	void getDataset( const HGxDB::CGxTiXmlElement *pEle , QVector<SNetService>& vecServiceInfo);

	/** 
	* @brief 加载本地服务
	* @note add by w00025
	* @param vecLinkInfo[in] 本地数据
	*/
	void loadLocalService(const QVector<SLinkInfo>& vecLinkInfo);

	/** 
	* @brief 加载网络服务
	* @note add by w00025
	* @param type[in] 服务类型
	* @param vecServiceInfo[in] 服务数据
	*/
	void loadNetService(ENetServiceType type, const QVector<SNetService>& vecServiceInfo);

	//加载恒道网络服务
	void loadHGNetService(ENetServiceType type, const QVector<SNetService>& vecServiceInfo);

	bool saveWhenMainWndClose();

private:
	// 服务插件维护的xml文件路径
	std::string			m_strFilePath;

	//恒道云服务配置文件路径
	std::string	 m_strHGServerFilePath;

	// 服务树指针
	CServerDataTree		*m_pServerDataTree;
};

#endif