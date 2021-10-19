#ifndef EARTH_LAYER_TREEWIDGET_H
#define EARTH_LAYER_TREEWIDGET_H

#include <QTreeWidget>
#include <QShortcut>

//#include <earth_layer/IFxEarthLayer.h>
#include <earth_layer_ui/IEarthLayerUiService.h>
#include <layer_ui/CLayerManager.h>

#include "ctkPluginContext.h"
#include <service/event/ctkEventHandler.h>
//#include <FreeXCore/IFxDocument.h>
//#include "FreeXServiceUtilUi/Common.h"
#include "CTiltAddModelWgt.h"
#include "CTiltToolWgt.h"
#include "LayerProperty/CEarthLayerPropertyMgr.h"
#include <bdpCommon/GxStrUtil.h>

#include "Common.h"

#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerLive.h>
#include <gdpScene/layer/GxLayerManager.h>

class QMenu;
class QWidget;
class CPyramidProcessListener;
class CSeaEarthLayerRootItem;

/**
* @class	: CEarthLayerTreeWidget
* @brief	: （三维）图层树
* @note		: 三维图层树
* @author	: k00026
**/
class CEarthLayerTreeWidget : public QTreeWidget, public IEarthLayerUiService, /*public FxCore::IFxDocument, */public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(IEarthLayerUiService)
	//Q_INTERFACES(FxCore::IFxDocument)
	Q_INTERFACES(ctkEventHandler)

public:
	// 右键菜单项
	enum EarthLayerMenuAction
	{
		NO_ACTION,
		ADD_DATA,
		ADD_TILEMODEL,
		DELETE_LAYER,
		RENAME,
		PROPERTIES,
		ZOOM_TO_LAYER	// 缩放至图层范围
	};
	Q_ENUMS(EarthLayerMenuAction)

	CEarthLayerTreeWidget(ctkPluginContext *pContext, QWidget *parent = nullptr);
	~CEarthLayerTreeWidget();

	/**
	* @brief	: 添加影像图层
	* @params	: name [in] 图层名称
	* @params	: path [in] 数据路径
	* @params	: driverType [in] 驱动类型
	* @params	: fromLocal [in] 是否是添加本地文件
	*/
	void addImageLayer(const QString &name, const QString &path, const QString &driverType, bool fromLocal = true, const QString &layerFormat = "");

	/**
	* @brief	: 添加高程图层
	* @params	: name [in] 图层名称
	* @params	: path [in] 数据路径
	* @params	: driverType [in] 驱动类型
	* @params	: fromLocal [in] 是否是添加本地文件
	*/
	void addEleLayer(const QString &name, const QString &path, const QString &driverType, bool fromLocal = true);

	/**
	* @brief	: 添加矢量图层
	* @params	: name [in] 图层名称
	* @params	: path [in] 数据路径
	* @params	: driverType [in] 驱动类型
	* @params	: fromLocal [in] 是否是添加本地文件
	*/
	void addVectorLayer(const QString &name, const QString &path, const QString &driverType, bool fromLocal = true);


	/**  
	  * @brief 添加三维实景图层
	*/
	void addLiveLayer(HGxScene::CGxGlobalLayer* pLiveLayer);

	/**
	* @brief	: 从本地添加图层
	* @params	: name [in] 图层名称
	* @params	: path [in] 数据路径
	* @params	: driverType [in] 驱动类型
	* @params	: layerType [in] 图层类型
	*/
	void addLayer(const QString &name, const QString &path, const QString &driverType, CLayerConfig::LayerType layerType, const QString &layerFormat = "");

	/**
	* @brief	: 从服务添加图层
	* @params	: name [in] 图层名称
	* @params	: path [in] 数据路径
	* @params	: driverType [in] 驱动类型
	* @params	: layerType [in] 图层类型
	*/
	void addLayer(const QString &strName, const SLayerPropertyStruct& stLayerStruct, CLayerConfig::LayerType layerType);

	/// 
	void addSeaLayer(const QString &name, const QString &path,const QString &driver="s57",bool fromlocal = true);

// 	// 工作空间加载
// 	bool load(const FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess) override;
// 
// 	// 工作空间保存
// 	bool save(FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess) override;

	// 图层树下方几个按钮点击 begin
	void beginAddImgLayer();
	void beginAddEleLayer();
	void beginAddVecLayer();
	void beginAddSeaLayer();
	void delLayer();
	// 图层树下方几个按钮点击 end

	// 插件启动时初始化
	void initWhenPluginStart();

	/**
	* @brief	获取三维地球图层管理
	* @return	图层管理指针
	*/
	QWidget* getEarthLayerPropertyMgrWgt();

protected:
	// 鼠标右键事件（弹出菜单）
	void contextMenuEvent(QContextMenuEvent *) override;

	// 事件处理
	void handleEvent(const ctkEvent &event) override;

private:
	void clearEarthChildLayers(QTreeWidgetItem* pRootItem);

	HGxScene::CGxLayerManager* getLayerManager(const std::string& strSceneID);

// 	// 设置矢量点图层属性
// 	void setVecDotLayerInfo(FxEarth::IFeLayerModel *pEarthLayer, const CLayerConfig::StDotLayerInfo &vecDotLayerInfo);
// 
// 	// 设置矢量线图层属性
// 	void setVecLineLayerInfo(FxEarth::IFeLayerModel *pEarthLayer, const CLayerConfig::StLineLayerInfo &vecLineLayerInfo);
// 
// 	// 设置矢量面图层属性
// 	void setVecFacetLayerInfo(FxEarth::IFeLayerModel *pEarthLayer, const CLayerConfig::StFacetLayerInfo &vecFacetLayerInfo);

	// 生产金字塔缓存
	bool generatePyramid(const QString &name, const QString &path, const QString &driverType, CLayerConfig::LayerType layerType);

	// 发送通知软件变化的事件
	void sendModifiedEvent(bool bModified);

	/**********************此处需获取底层三维图层服务引用线程****************/

	//FxEarth::IFxEarthLayerService *getLayerServiceRefThread(int iSceneId);

	/**********************此处需获取底层三维图层服务线程****************/

	//FxEarth::IFxEarthLayerService *getEarthLayerServiceThread(int, const FxCommonUtils::TiXmlElement *pEle,const FxCommonUtils::TiXmlElement *pRootEle, FxCore::IFxWorkspaceProcess *pProcess);

	/**********************此处需获取三维图层服务****************/

	void reInitLayerTree();

 	// 添加图层线程函数
 	HGxScene::CGxGlobalLayer* addLayerInThread(HGxScene::CGxLayerConfig* pLayerConfig);

private slots:
	// 线程中添加图层完成后槽函数，用于处理界面逻辑
	void addLayerFinishSlot();

	// 图层名称改变槽函数
	void layerNameChangedSlot(const QString &);

	void futureFinishedSlot();

	// 菜单项点击槽函数
	void contextMenuActionSlot(QAction *);
	// 树节点重命名结束槽函数
	void editingFinishedSlot(const QModelIndex &);

	/** 
	* @brief 树节点鼠标双击槽函数
	* @param item [in]（双击的树节点） 
	* @param column [in]（树节点所在列） 
	*/
	void itemDoubleClickedSlot(QTreeWidgetItem *item, int column);

	/**
	* @brief	: 添加图层槽函数
	* @params	: name [in] 图层名称
	* @params	: path [in] 数据路径
	* @params	: driverType [in] 驱动类型
	* @params	: layerType [in] 图层类型
	*/
	void addLayerToSceneSlot(const QString &name, const QString &path, const QString &driver, CLayerConfig::LayerType layerType);

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


	void getEarthLayerServiceFinishedSlot();

	/**  
	  * @brief 添加三维实景槽函数
	*/
	void addLiveLayerSlot();
	/**  
	  * @brief 格式转换工具槽函数
	*/
	void slotTiltTool();

	/// 监听服务插件
	void pluginEvent(const ctkPluginEvent &event);
signals:
	/**  
	  * @brief 场景创建信号
	  * @param 0 [in] 场景ID
	  * @param 1 [in] 标识是否为从文件打开,区别新创建的空白场景
	*/
	void sceneCreatedSignal(std::string strSceneID,	bool bCreateFromFile); 
	void sceneSwitchSignal(std::string);		// 场景切换信号

	/// add by yq00012
	void shorcutRenameItemSignal();

	void shortcutDeleteItemSignal();

	void shortcutPropertyItemSignal();

private slots:
	/**  
	  * @brief 场景创建槽函数
	  * @param 0 [in] 场景ID
	  * @param 1 [in] 标识是否为从文件打开,区别新创建的空白场景
	*/
	void sceneCreatedSlot(std::string strSceneID, bool bCreateFromFile); 
	void sceneSwitchSlot(std::string strSceneID);			// 场景切换槽函数

	protected:

		/// 创建海图根节点 add00012 20190926
		void CreatSeaLayerRootItem();
private:
	std::string m_strSceneID;			// 当前场景id
	QMenu *m_pContextMenu;				// 右键菜单

	QTreeWidgetItem *m_pImageTypeItem;	// 影像图层分类树节点
	QTreeWidgetItem *m_pEleTypeItem;	// 高程图层分类树节点
	QTreeWidgetItem *m_pVectorTypeItem;	// 矢量图层分类树节点
	QTreeWidgetItem *m_pLiveTypeItem;	//实景图层分类树节点

	/************暂时不提供海图*****************/
	//CSeaEarthLayerRootItem *m_pSeaLayerRoot; /// 海图图层根节点


	CPyramidProcessListener					*m_pPyramidProcess;

	// 倾斜摄影添加窗口
	CTiltAddModelWgt*				    m_pAddTiltModelWgt;
	// 倾斜摄影添加窗口
	CTiltToolWgt*				        m_pTiltToolWgt;

	CEarthLayerPropertyMgr *m_pPropertyMgr;

	std::string							m_strCachePath;

	QWidget*						m_pMainWindow;
};

#endif // EARTH_LAYER_TREEWIDGET_H
