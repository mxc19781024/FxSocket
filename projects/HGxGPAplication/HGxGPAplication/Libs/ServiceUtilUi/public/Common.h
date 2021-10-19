#pragma  once
#include <QVariant>
#include <QString>
#include <QVector>

/**  
	* @enum 服务数据Role
*/
enum EServerDataRoles
{
	 ItemTypeRole		= 			Qt::UserRole + 1,
	 ItemServerTypeRole = 			Qt::UserRole + 2,
	 FilePathRole		=			Qt::UserRole + 3,
	 ServerProRole		=			Qt::UserRole + 4,
	 LayerProRole		=			Qt::UserRole + 5,
	 ConnectTypeRole	=			Qt::UserRole + 6,
	 LinkRole			=			Qt::UserRole + 7,
	 HGLayerRole = Qt::UserRole + 8,	//是否为恒道云服务图层 该图层只能刷新不能添加删除
	 HGServersRole = Qt::UserRole + 9	//恒道云服务 WMS WMTS WCS WFS 节点下保存QVector<ServerPropertyStruct> 用于刷新
};

/**  
	  * @enum 链接类型
*/
enum ELinkType
{
	// 文件夹链接
	FOLDERLINK = 1,

	// 文件链接
	FILELINK = 2
};

/**  
	* @enum 属性窗口类型
*/
enum EWindowType
{
	/// OGC类型
	OGC = 1,

	/// OGCSET类型
	OGCSET = 2,

	/// DATASET类型
	DATASET = 3,

	/// TILE类型之XYZ
	TITLE_XYZ = 4,

	/// TILE类型之TMS
	TITLE_TMS = 5,

	/// 未知类型
	UNKNOW = 6
};

/**  
	* @enum 树形节点类型
*/
enum ETreeItemType
{
	// 服务类型节点item
	SERVER_TYPE_ITEM,

	// 服务节点item
	SERVER_ITEM,

	// 图层集节点item
	LAYER_SET_ITEM,

	// 图层节点item
	LAYER_ITEM,

	NONE_ITEM
};

/**  
	* @enum 属性窗口类型
*/
enum EServerType
{
	// 本地服务
	LOCAL_SERVER,

	// OGC服务
	OGC_SERVER,

	 // Tile服务
	TILE_SERVER,

	// 数据集
	DATASET_SERVER,

	NONE_SERVER
};

/**  
	* @enum 服务类型
	* @note add by w00025
*/
enum ENetServiceType
{
	WMS = 1,
	WMTS = 2,
	WFS = 3,
	WCS = 4,
	TMS = 5,
	XYZ = 6,
	DATASETS = 7
};

/**  
	* @brief 网络服务数据结构
*/
struct SNetService
{
	SNetService()
	{
		strServerType = "";
		strServerName = "";
		strServerWebsite = "";
		strServerVersion = "";
		strUserName = "";
		strPassword = "";
		strDriverName = "";
		strLayerType = "";
		strLayerName = "";
		bDefault = false;
	}
	// 服务类型
	QString strServerType;

	// 服务名称/图层名称
	QString strServerName;

	// 服务网址
	QString strServerWebsite;

	// 服务版本
	QString strServerVersion;

	// 用户名
	QString strUserName;

	// 密码
	QString strPassword;

	// 驱动类型
	QString strDriverName;

	// 图层类型
	QString strLayerType;

	// 数据集或者TMS\XYZ的图层名称
	QString strLayerName;

	//是否为默认图层
	bool bDefault;
};

/**  
	* @brief 文件/文件夹链接信息
*/
struct SLinkInfo
{
	SLinkInfo()
	{
		eLinkType = FOLDERLINK;
		strAbsoluteFilePath = "";
		strFileName = "";
	}
	ELinkType eLinkType;
	QString strAbsoluteFilePath;
	QString strFileName;

};
Q_DECLARE_METATYPE(SLinkInfo)

/**  
	* @brief 图层属性结构
*/
struct SLayerPropertyStruct
{
	// 服务类型
	QString strServerType;

	// 服务名称
	QString strServerName;

	// 服务地址
	QString strServerWebsite;

	// 图层的名称
	QString strLayerName;

	// 图层类型
	QString strLayerType;

	// 驱动名称
	QString strDriveName;  

	// 图层信息
	QString strLayerInfo;

	//图层的瓦片格式
	QString strLayerFormat;

	//图层支持的瓦片格式
	std::vector<QString> strLayerFormats;

	//是否为默认图层
	bool bDefault;

	SLayerPropertyStruct()
	{
		strServerType = "";
		strServerName = "";
		strServerWebsite = "";
		strLayerName = "";
		strLayerType = "";
		strDriveName = "";
		strLayerInfo = "";
		strLayerFormat = "";
		strLayerFormats = std::vector<QString>();
		bDefault = false;
	}
};
Q_DECLARE_METATYPE(SLayerPropertyStruct)

/**  
	* @brief 服务属性结构
*/
struct SServerPropertyStruct
{
	SServerPropertyStruct()
	{
		bConnect = true;
		bDefault = false;
		strServerType = "";
		strServerName = "";
		strServerWebsite = "";
		strServerVersion = "";
		strUserName = "";
		strPassword = "";
		vecLayerProperty .clear();
	}

	// 服务连接状态
	bool	bConnect;

	//是否为默认图层
	bool bDefault;

	// 服务器类型
	QString strServerType;

	// 服务名称
	QString strServerName;
	
	// 服务版本
	QString strServerVersion;
	
	// 服务网址
	QString strServerWebsite;

	// 用户名
	QString strUserName;

	// 密码
	QString strPassword;

	//服务支持的数据格式
	std::vector<QString>			vecFormats;

	 // 服务下所有图层的信息
	std::vector<SLayerPropertyStruct > vecLayerProperty;        
};



Q_DECLARE_METATYPE(SServerPropertyStruct)
Q_DECLARE_METATYPE(QVector<SServerPropertyStruct>)
