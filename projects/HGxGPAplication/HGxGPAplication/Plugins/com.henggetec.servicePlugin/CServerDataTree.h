#ifndef CSERVERDATATREE_H
#define CSERVERDATATREE_H

#include <QWidget>
#include <QStandardItemModel>

#include <service/event/ctkEventHandler.h>
//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXCommonUtils/EnvironmentVariableReader.h>
//#include <FreeXCommonUtils/PathRegistry.h>

//#include <scene/IFxSceneService.h>
//#include <earth_layer/IFxEarthLayer.h>
//#include <map_layer/IFxMapLayer.h>
//#include <service/IFxDataService.h>

//#include "FreeXServiceUtilUi/Common.h"
#include "ui_CServerDataTree.h"

#include "layer_ui/CLayerManager.h"

#include <gdpUtil/service/GxDataService.h>
#include <gdpUtil/service/GxServiceLayer.h>

/**
	* @class CServerDataTree
	* @brief 二三维场景服务
	* @author 王焕腾
*/

class CServerDataTree : public QWidget
{
	Q_OBJECT
public:
	CServerDataTree(QWidget *parent = 0);
	~CServerDataTree();

	/**  
	  * @brief 获取服务树model
	  * @return 服务树model
	*/
	QStandardItemModel *getItemModel() const {return m_pItemModel;} 

	/** 
 	* @brief 获取场景ID
 	* @note add by w00025
 	* @return 场景ID
 	*/
 	std::string getSceneID();

	/** 
 	* @brief 获取本地服务数据信息集合
 	* @note add by w00025
 	* @param vecLinkInfo[out] 本地服务数据信息集合
 	* @return 场景名称
 	*/
	void getLocalserviceList(QVector<SLinkInfo> &vecLinkInfo);

	/** 
 	* @brief 根据网络服务枚举类型获取网络服务数据信息集合
 	* @note add by w00025
 	* @param type[in] 网络服务类型
	* @param vecNetService[out] 网络服务数据信息集合 
 	* @return 场景名称
 	*/ 
	void getNetServiceList(ENetServiceType type, QVector<SNetService> &vecNetService);

	/** 
 	* @brief 还原本地服务
 	* @note add by w00025
	* @param vecLinkInfo[in] 本地服务数据信息集合 
 	*/ 
	void restoreLocalServer(const QVector<SLinkInfo>& vecLinkInfo);

	/** 
 	* @brief 还原网络服务
 	* @note add by w00025
 	* @param type[in] 网络服务类型
	* @param vecNetService[in] 网络服务数据信息集合 
 	*/ 
	void restoreNetServer(ENetServiceType type, const QVector<SNetService>& vecServiceInfo);

	/**
	* @brief 还原恒道网络服务
	* @note add by w00025
	* @param type[in] 网络服务类型
	* @param vecNetService[in] 网络服务数据信息集合
	*/
	void restoreHGNetServer(ENetServiceType type, const QVector<SNetService>& vecServiceInfo);

	/** 
	* @brief 获取OGC服务信息
	* @note add by w00025
	* @param stNetService[in] 序列化后的网络服务信息
	* @param stServerInfo[out] OGC服务信息
	*/
	void getOGCServerInfo(const SNetService &stNetService, SServerPropertyStruct &stServerInfo);

	/** 
	* @brief 获取Tile服务信息
	* @note add by w00025
	* @param stNetService[in] 序列化后的网络服务信息
	* @param stServerInfo[out] OGC服务信息
	*/
	void getTileServerInfo(const SNetService &stNetService, SServerPropertyStruct &stServerInfo);

	/** 
	* @brief 获取数据集信息
	* @note add by w00025
	* @param stNetService[in] 序列化后的网络服务信息
	* @param stServerInfo[out] OGC服务信息
	*/
	void getDatasetInfo(const SNetService &stNetService, SServerPropertyStruct &stServerInfo);

private:
	/** 
	* @brief 初始化
	*/
	void init();

	/** 
	* @brief 初始化信号和槽
	*/
	void initSignalsAndSlots();

	/** 
	* @brief 存储节点信息
	*/
	void storageData(ETreeItemType nItemType, EServerType nServerType, QStandardItem *pItem);

	/** 
	* @brief 存储图层集属性信息
	*/
	void storageServerProData(const SServerPropertyStruct & stServerPropertyStruct, QStandardItem *pItem);

	/** 
	* @brief 存储图层属性信息
	*/
	void storageLayerProData(const SLayerPropertyStruct & stLayerPropertyStruct, QStandardItem *pItem);

	/** 
	* @brief 刷新本地服务
	*/
	void reflashLocalServer(const QString strPath, QStandardItem* pParentItem);

	/** 
	* @brief 刷新OGC服务图层集
	*/
	void refreshOGCLayerset(QStandardItem* pItem);

	/** 
	* @brief 刷新TILE服务图层
	*/
	void refreshTileLayer(QStandardItem* pItem);

	/** 
	* @brief 刷新数据集图层
	*/
	void refreshDatasetLayer(QStandardItem* pItem);

	/** 
	* @brief 检验系统中文件夹下是添加还是删除还是不变
	*/
	void checkDirState(const QString strPath, QStandardItem* pParentItem);

	/** 
	* @brief 获取当前的Item
	*/
	QStandardItem* getCurrentItem();

	/**  
	  * @brief 获取OGC网络服务数据集合
	  * @note  add by w00025
	  * @param pItem [in] 对应OGC网络类型的树节点
	  * @param vecNetService [out] 网络服务数据集合
	*/
	void getOGCServiceInfo(QStandardItem *pItem, QVector<SNetService> &vecNetService);

	/**  
	  * @brief 获取Tile或者Dataset服务数据集合
	  * @note  add by w00025
	  * @param pItem [in] 对应类型的树节点
	  * @param vecNetService [out] 服务数据集合
	*/
	void getTileOrDatasetInfo(QStandardItem *pItem, QVector<SNetService> &vecNetService);

	/**  
	  * @brief 设置节点断开状态
	  * @note  add by w00025
	  * @param pItem [in] 节点对应的服务类型
	  * @param vecNetService [out] 服务数据集合
	*/
	void setNodeDisConStatus(ENetServiceType type, const SServerPropertyStruct &stPropertyStruct);

	/**  
	  * @brief 获取当前激活场景
	  * @note  add by w00025
	  * @return 返回激活场景的字符串
	*/
	QString getActiveScene();

signals:

protected:
	void paintEvent(QPaintEvent *event);
	/**  
	  * @brief 恒歌云服务底层刷新处理
	*/
	void HGLayeritemRefresh(QStandardItem *pCurrentItem);

private slots:

	/**  
	  * @brief 点击树节点时响应的槽函数
	  * @note add by w00025
	*/
	void itemClickedSlot(const QModelIndex &index);

	/**  
	  * @brief 根据服务图层名称判断树节点下是否存在该图层
	  * @note add by w00025
	  * @param strLayerName [in] 图层名称
	  * @param pItem [in] 树节点
	  * @return 存在返回true，不存在返回false
	*/
	bool bFindLayerInServiceList(const QString &strLayerName, QStandardItem *pItem);

	/**  
	  * @brief 刷新OGC服务节点
	  * @note add by w00025
	  * @param stServerInfo [in] 节点所包含的数据信息
	  * @param pItem [in] 树节点
	*/
	void loadOGCLayerNode(QStandardItem *pItem, const SServerPropertyStruct& stServerInfo);

	/**  
	  * @brief 右键菜单响应的槽函数
	*/
	void contextMenuSlot(const QPoint &pos);

	/**  
	  * @brief 添加服务信号槽
	*/
	void addServerSlot();

	/**  
	  * @brief 重命名信号槽
	*/
	void reNameSlot();

	/**  
	  * @brief 更新服务信号槽
	*/
	void reFlashServerSlot();

	/**  
	  * @brief 删除节点信号槽
	*/
	void itemDeleteSLot();

	/**  
	  * @brief 查看图层属性
	*/
	void showLayerPropertySlot();

	/**  
	  * @brief 添加到场景信号槽
	*/
	void addToSceneSlot();

	/**  
	  * @brief 添加高程到场景信号槽
	*/
	void addAltitudeToSceneSlot();

	/**  
	  * @brief 添加影像到场景信号槽
	*/
	void addImageToSceneSlot();

	/**  
	  * @brief 添加海图文件到场景信号槽
	*/
	void addToSeaSceneSlot();
	/**  
	  * @brief 搜索框
	*/
	void searchPlotTreeSlot(const QString &keyword);

	/**  
	  * @brief 链接到本地文件或者本地文件夹
	*/
	void linkToSlot();

	/**  
	  * @brief 取消链接
	*/
	void disConnectSlot();

   /**
     * @brief 打开文件位置
   */
	void openFolderSlot();

   /**
     * @brief 文件刷新
   */
	void refreshFolderSlot();

	/**
     * @brief 编辑结束
   */
	void editItemFinshedSlot(QStandardItem *pItem);
	
	/**
     * @brief 图层集属性
   */
	void reSetByProEditSlot(const  SServerPropertyStruct & stServerPropertyStrut);
	
	/**
     * @brief 文件刷新
   */
	void reLayerSetByProEditSlot(const  SLayerPropertyStruct & stLayerPropertyStrut);

	/**
     * @brief 底部刷新按钮
   */
	void on_pFlashTBtn_clicked();
	
	/**
     * @brief 底部展开按钮
   */
	void on_pExpandTBtn_clicked();
	
	/**
     * @brief 底部收缩按钮
   */
	void on_pCollapseTBtn_clicked();
	
	/**
     * @brief 底部删除按钮
   */
	void on_pDeleteTBtn_clicked();

	/**
     * @brief 展开树节点
   */
	void expandItemsSlot( const QModelIndex & index);

private:
	// 树节点是否处于编辑状态
	bool												m_bItemEdit;     

	// 存储图层信息
	std::vector<HGxUtil::CGxServiceLayer*>	m_vecLayers;

	//存储支持数据格式
	std::vector<QString>							m_serviceFormats;

	QStandardItemModel*									m_pItemModel;

	//  指定类型的服务指针
	HGxUtil::CGxDataService*							m_pService;

	// 链接文件支持的后缀名集合
	//QSet<QString>										m_setSuffix;
	
	Ui::CServerDataTree									ui;

	bool m_bEmbed;
	//当前项的名字（为了使输入名字为空时恢复用) l180
	QString                                             m_strItemName;
	                    
};

#endif // CSERVERDATATREE_H
