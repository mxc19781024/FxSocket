#ifndef MAP_LAYER_TREEWIDGET_H
#define MAP_LAYER_TREEWIDGET_H

#include <iostream>
#include <QMetaType>
#include <QTreeWidget>

#include <ctkPluginContext.h>
#include <service/event/ctkEventHandler.h>

#include <layer_ui/CLayerManager.h>
#include <map_layer_ui/IMapLayerUiService.h>
//#include <map_layer/IFxMapLayer.h>
//#include <FreeXCore/IFxDocument.h>
//#include <FreeXServiceUtilUi/Common.h>

#include "LayerProperty/CMapLayerPropertyMgr.h"

//#include "map_layer/IFxSeaMapLayer.h"

#include "Common.h"

#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerLive.h>
#include <gdpScene/layer/GxLayerManager.h>

class QMenu;
class CLinkFileWindow;
class CPyramidProcessListener;
/** 
* @class CMapLayerTreeWidget
* @brief 二维图层树
* @note 二维图层树
* @author k00026
*/
class CSeaMapLayerRootItem;


class CMapLayerTreeWidget : public QTreeWidget, public IMapLayerUiService, /*public FxCore::IFxDocument, */public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(IMapLayerUiService)
	//Q_INTERFACES(FxCore::IFxDocument)
	Q_INTERFACES(ctkEventHandler)
public:
	// 右键菜单项
	enum MapLayerMenuAction
	{
		ADD_DATA,		// 添加数据
		ZOOM_TO_LAYER,	// 缩放至图层范围
		MOVE_UP,		// 上移
		MOVE_DOWN,		// 下移
		MOVE_TO_TOP,	// 置顶
		MOVE_TO_BOTTOM,	// 置底
		DELETE_LAYER,	// 删除
		RENAME,			// 重命名
		PROPERTIES		// 属性
	};
	Q_ENUMS(MapLayerMenuAction)

	CMapLayerTreeWidget(ctkPluginContext *pContext, QWidget *parent = nullptr);
	~CMapLayerTreeWidget();

	void CMapLayerTreeWidget::clearMapChildLayers(QTreeWidgetItem* pRootItem);

	HGxScene::CGxLayerManager* getLayerManager(const std::string& strSceneID);

	/** 
	* @brief 通过本地路径添加图层
	* @note 路径可能是文件，也可能是文件夹；如果是文件夹，则递归添加里面所有文件
	*/
	void addMapLayerFromLocalFile(const QString &filePath) override;

	/** 
	* @brief 通过服务添加图层
	* @note 
	*/
	void addMapLayerFromService(const QString &url, const QString &layerName, const QString &driverType, CLayerConfig::LayerType layerType, const QString &layerFormat = "") override;
	
	/** 
	* @brief 通过地图图层添加数据图层
	* @note add by w00025
	*/
	void addMapLayerFromService(const QString &strName, const SLayerPropertyStruct& stLayerStruct, CLayerConfig::LayerType layerType);


	// 添加矢量图层
	void addVectorLayer(const QString &filePath, const QString &layerName);

	// 添加影像图层
	void addImageLayer(const QString &layerPath, const QString &layerName);

	void addLocalFileLayer(const QString &name, const QString &path);

/**************此处需要FxDocument*************/

// 	bool load(const FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess) override;
// 
// 	bool save(FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess) override;

	void moveLayerToTop();
	void moveLayerToBottom();
	void moveLayerUp();
	void moveLayerDown();
	void deleteLayer();

	// 插件启动时初始化
	void initWhenPluginStart();

	// 获取图层管理面板
	QWidget* getMapLayerPropertyMgrWgt();

protected:
	// 弹出右键菜单事件
	void contextMenuEvent(QContextMenuEvent *event) override;

	// 拖动drag事件
	void dragEnterEvent(QDragEnterEvent *event) override;

	// 拖动drop事件
	void dropEvent(QDropEvent *event) override;

	// 事件处理
	void handleEvent(const ctkEvent &event) override;

	/************暂时不提供海图*****************/

	//void CreatSeaMapRootItem();

	protected slots:

		void pluginEvent( const ctkPluginEvent &event );
private:
	// 初始化图层类型
	void initLayerTypes();
	// 遍历文件夹添加图层（不递归里面的文件夹）
	void scanDirAddLayers(const QString &filePath);

	void init();

	/**********************此处需要获取底层二维图层服务*********************/

// 	// 底层二维图层服务
// 	FxMap::IFxMapLayerService *mapLayerService(int sceneId);

	/************暂时不提供海图*****************/
//	FxMap::IFxSeaMpLayerService *seaMapLayerService(int iSceneId);


private:

	// 生成金字塔缓存
	bool generatePyramid(const QString &name, const QString &path, const QString &driverType, CLayerConfig::LayerType layerType);

	// 发送通知软件变化的事件
	void sendModifiedEvent(bool bModified);

	/************此处需要TinyXml、workspace等*****************/

// 	// 加载图层线程函数
// 	FxMap::IFxMapLayerService *loadLayerInThread(int, const FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess);
private slots:
	// 图层名称改变槽函数
	void layerNameChangedSlot(const QString &);
	// 右键菜单点击槽函数
	void contextMenuActionSlot(QAction *action);
	// 树节点重命名结束槽函数
	void editingFinishedSlot(const QModelIndex &);

	/** 
	* @brief 通过鼠标右键打开窗口添加数据槽函数
	* @note 添加本地图层到地图图层
	* @param name 图层名称
	* @param path 数据路径
	* @param driver 驱动类型
	* @param layerType 图层类型
	*/
	void addLayerToSceneSlot(const QString &strName, const QString &path, const QString &driver, CLayerConfig::LayerType layerType);

	/** 
	* @brief 通过鼠标右键打开窗口添加数据槽函数
	* @note 添加服务图层到地图图层add by w00025
	* @param name 图层名称
	* @param path 图层信息结构体
	*/
	void addServerLayerToSceneSlot(const QString &strName, const SLayerPropertyStruct& stLayerStruct, CLayerConfig::LayerType layerType);

	// 重命名快捷键F2
	void renameShortCutSlot();

	// 删除快捷键Del
	void deleteShortCutSlot();

	// 属性快捷键R
	void propertyShortCutSlot();

	// 树选中节点改变槽函数
	void itemSelectionChangedSlot();

	// 取消金字塔缓存处理槽函数
	void cancelProcessSlot();

	// 金字塔缓存进行槽函数
	void pyramidProcessSlot(double);

	// 金字塔缓存结束槽函数
	void onPyramidProcessFinishedSlot(const QString &name, const QString &path, const QString &driverType, int layerType);

	// 图层load线程处理结束
	void loadLayerThreadFinishedSlot();

signals:
	/**  
	  * @brief 场景创建信号
	  * @param 0 [in] 场景ID
	  * @param 1 [in] 标识是否为从文件打开,区别新创建的空白场景
	*/
	void sceneCreatedSignal(std::string strSceneID, bool bCreateFromFile); 

	/**
	* @brief 场景切换信号
	* @param 0 [in] 场景ID
	*/
	void sceneSwitchSignal(std::string strSceneID); 

	/**
	* @brief 清除选中信号
	*/
	void clearSelectSignal();

	/**
	* @brief 初始化图层树信号
	*/
	void initTreeWidgetSignal();

	/// 海图图层快捷键 begin
	void shorcutRenameItemSignal();
	
	void shortcutDeleteItemSignal();

	void shortcutPropertyItemSignal();
	/// 海图图层快捷键 end

private slots:
	/**  
	  * @brief 场景创建槽函数
	  * @param 0 [in] 场景ID
	  * @param 1 [in] 标识是否为从文件打开,区别新创建的空白场景
	*/
	void sceneCreatedSlot(std::string strSceneId, bool bCreateFromFile);

	/**
	* @brief 初始化图层树槽函数
	*/
	void initTreeSlot();

private:
	std::string m_strSceneID;				// 当前场景id
	QMenu *m_pContextMenu;				// 右键菜单
	QTreeWidgetItem *m_pRootItem;		// 根节点

	QTreeWidgetItem *m_pCurDragItem;	// 当前拖动的item;

	CPyramidProcessListener *m_pPyramidProcess;

	CMapLayerPropertyMgr *m_pPropertyMgr;

	std::string							m_strCachePath;

	QWidget*						m_pMainWindow;

	/************暂时不提供海图*****************/
// 	/// 二维海图节点 
// 	CSeaMapLayerRootItem *m_pSeaMapRootItem;
};

#endif // MAP_LAYER_TREEWIDGET_H
