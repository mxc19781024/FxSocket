#include "CEarthLayerTreeWidget.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QFileInfo>
#include <QPainter>

#include "ctkPluginFrameworkFactory.h"
#include "ctkPluginFramework.h"
#include "ctkPluginException.h"
#include "ctkPluginContext.h"
#include "ctkPluginFrameworkLauncher.h"

//#include <service/IFxDataService.h>

//#include <service/IFxDataService.h>
//#include <service/layer/IServiceLayer.h>
//#include "earth_layer/IFxEarthLayer.h"
//#include "scene/IFxSceneService.h"

//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXCommonUtils/EnvironmentVariableReader.h>
//#include <FreeXCommonUtils/PathRegistry.h>
//#include <FreeXFramework/LoadInfo.h>
#include <bdpCommon/GxStrUtil.h>
//#include <FreeXGeoUtil/MapGrid.h>

#include "FxEditingDelegate/CEditingDelegate.h"
#include "CEarthLayerItem.h"
#include "CCommonItem.h"

//#include "service_ui/IServiceUiInterface.h"
//#include <systemset_ui/ISysSetUiInterface.h>
//#include "FreeXDataResUtilUi/CDataResourcesUtils.h"
//#include "FreeXDataResUtilUi/CLinkFileWindow.h"
//#include "FreeXDataResUtilUi/CServerDataWindow.h"
#include "public/GxDataResourcesUtils.h"
#include "public/GxLinkFileWindow.h"
#include "public/GxServerDataWindow.h"

#include <QShortcut>
//#include "ICore.h"
//#include "CToolManager.h"

#include <QIcon>
#include <QSettings>
#include "scene_ui/ISceneUiService.h"
#include <scene_ui/ISceneInterface.h>
#include "FxMessageBox/CMessageBox.h"

#include "CEarthPyramidCache.h"
//#include <FreeXGeoUtil/PyramidBuilder.h>
#include "CPyramidProcessListener.h"
//#include "CFxUiGlobal.h"
//#include "CMainWindowManager.h"

#include <QFutureWatcher>
#include <qtconcurrentrun.h>
#include <functional>
//#include <FxLoadIndicator/CLoadIndicator.h>
//#include "earth/IFxEarthManipulatorManager.h"
//#include <earth/IFxEarthScene.h>
//#include <FreeXGeoUtil/CoordConverter.h>
//#include <FreeXCommonUtils/MathCommonUtil.h>
//#include <FreeXFramework/FxServiceQuerier.h>
#include "FxLoadIndicator/CLoadIndicator.h"

#include <QScrollBar>
#include <FxToast/CToast.h>

#include "ctkServiceEvent.h"


/************暂时不提供海图*****************/
//#include "CSeaEarthLayerItem.h"

#include <bdpDB/GxRegister.h>

#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/scene/GxSceneBase.h>

#include <gdpScene/CommonGeo.h>
#include <gdpScene/layer/GxLayerManager.h>
#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerConfig.h>
#include <gdpScene/layer/GxLayerLive.h>
#include <gdpScene/layer/GxLayerConfigFactory.h>

#include <bdpCommon/GxMathUtil.h>
#include <gdpAlgor/GxCoordConverter.h>
#include <gdpAlgor/GxPyramidBuilder.h>
#include <gdpAlgor/GxMapGrid.h>
#include <gdpAlgor/GxGeoMathUtil.h>


#include <osgEarth/Viewpoint>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>
#include "GxUiFrameworkServiceInterface.h"


// 图层类型，仅用于图层“分类”树节点，不用于图层节点
#define LayerTypeRole (Qt::UserRole + 12)

// 当前树节点文字内容，用于重命名
#define CurrentTextRole (Qt::UserRole + 13)

#define MAX_VISIBLE_VAL FLT_MAX

Q_DECLARE_METATYPE(CLayerConfig::LayerType);
//输入的数据大于minPyramidLevel时生成金字塔,默认为5 l180
int minPyramidLevel = 5;

void CEarthLayerTreeWidget::clearEarthChildLayers(QTreeWidgetItem* pRootItem)
{
	HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

	if (pRootItem && pLayerManager)
	{
		QList<QTreeWidgetItem *> lisItems = pRootItem->takeChildren();
		for (int n = 0; n < lisItems.size(); ++n)
		{
			CCheckableTreeItem *pCurrentItem = dynamic_cast<CCheckableTreeItem *>(lisItems[n]);
			if (pCurrentItem && (pCurrentItem->type() == CLayerConfig::LayerItem))
			{
				CEarthLayerItem *pEarthLayerItem = static_cast<CEarthLayerItem*>(pCurrentItem);
				if (pEarthLayerItem)
				{
					pLayerManager->deleteLayer(pEarthLayerItem->earthLayer());
				}
			}
		}
	}
}

CEarthLayerTreeWidget::CEarthLayerTreeWidget(ctkPluginContext *pContext, QWidget *parent)
	: QTreeWidget(parent)
	, m_pContextMenu(nullptr)
	, m_strSceneID("")
	, m_pPyramidProcess(nullptr)
	, m_pAddTiltModelWgt(nullptr)
	, m_pTiltToolWgt(nullptr)
	, m_pMainWindow(NULL)
{
	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<SLayerPropertyStruct>("SLayerPropertyStruct");

	QSettings setting(QString::fromUtf8(HGX_FILE_CONFIG().getFullPath("config/minPyramidLevel.ini").c_str()), QSettings::IniFormat);
 	//读取数据多大生成金字塔 l180
 	QString  strMinPyramidLevel = setting.value("minPyramidLevel").toString();
 	if ("" != strMinPyramidLevel)
 	{
 		minPyramidLevel = strMinPyramidLevel.toInt();
 	}

	/************暂时不提供海图*****************/
	//m_pSeaLayerRoot = nullptr;

	m_strCachePath = HGX_SYS_SETTINGS().strCachePath + "/EarthLayerCache";

    setHeaderHidden(true);

	m_pPropertyMgr = new CEarthLayerPropertyMgr(this);
	connect(m_pPropertyMgr, SIGNAL(layerNameChangedSignal(const QString &)), this, SLOT(layerNameChangedSlot(const QString &)));

	// add by w00025
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	/// add by yq00012
	setEditTriggers(QAbstractItemView::NoEditTriggers);

	(const_cast<QScrollBar *>(verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);

	CEditingDelegate *pDelegate = new CEditingDelegate(this);
	connect(pDelegate, SIGNAL(editingFinishedSignal(const QModelIndex &)), this, SLOT(editingFinishedSlot(const QModelIndex &)));
	setItemDelegate(pDelegate);

	connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChangedSlot()));
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(itemSelectionChangedSlot()));
	connect(this, SIGNAL(sceneCreatedSignal(std::string, bool)), this, SLOT(sceneCreatedSlot(std::string, bool)));
	connect(this, SIGNAL(sceneSwitchSignal(std::string)), this, SLOT(sceneSwitchSlot(std::string)));
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(itemDoubleClickedSlot(QTreeWidgetItem *, int)));

	// 快捷键
	new QShortcut(QKeySequence("F2"), this, SLOT(renameShortCutSlot()), nullptr, Qt::WidgetWithChildrenShortcut);
	new QShortcut(QKeySequence("Delete"), this, SLOT(deleteShortCutSlot()), nullptr, Qt::WidgetWithChildrenShortcut);
	new QShortcut(QKeySequence("R"), this, SLOT(propertyShortCutSlot()), nullptr, Qt::WidgetWithChildrenShortcut);

/*************************此处需要获取插件管理***********************/

	//FreeXFrameworkInstance.pluginMgr().getFramePluginContext()->connectPluginListener(this,SLOT(pluginEvent(ctkPluginEvent)));

	HGxEventHelperInstance.subscribeTopic(this, "event/sceneCreated", HGxFWLauncherInstance.getPluginContext());
	HGxEventHelperInstance.subscribeTopic(this, "event/sceneSwitch", HGxFWLauncherInstance.getPluginContext());
}

CEarthLayerTreeWidget::~CEarthLayerTreeWidget()
{
	//ICore::toolManager()->tryToSetCurrentPropertyWidget(nullptr);

 	// 清空图层树上的图层
 	clearEarthChildLayers(m_pImageTypeItem);
 	clearEarthChildLayers(m_pEleTypeItem);
 	clearEarthChildLayers(m_pVectorTypeItem);
 	clearEarthChildLayers(m_pLiveTypeItem);
}

bool CEarthLayerTreeWidget::generatePyramid(const QString &strName, const QString &strPath, const QString &strDriverType, CLayerConfig::LayerType layerType)
{
	// 判断是否是本地文件
	QFileInfo fileInfo(strPath);
	if (fileInfo.exists())
	{
		if (fileInfo.suffix() != "tif" && fileInfo.suffix() != "img")
		{
			return false;
		}

 		//判断金字塔是否存在，如果存在就不再重新生成
		if (HGxAlgor::CFxPyramidBuilder::hasPyramidData(strPath.toUtf8().data()))
 		{
 		return false;
 		} 
 	    //获取文件最大可以生成多少级金字塔 l180
 		int nLevel = 0;
 		if(CLayerConfig::ImageLayer ==  layerType)
 		{
			nLevel = HGxAlgor::getFileMaxLevel(strPath.toStdString(), 256); // 影像默认块大小为256
 		}else if(CLayerConfig::ElevationLayer == layerType)
 		{
			nLevel = HGxAlgor::getFileMaxLevel(strPath.toStdString(), 16); // 高程默认块大小为16
 		}
 		if( nLevel > minPyramidLevel)
 		{
			/*********************此处需要主窗口*********************/
 			CMessageBox::ReturnButton ret = CMessageBox::question(tr("the file size is too large,pyramid cache is recommended"), m_pMainWindow);
 			if (ret == CMessageBox::Confirm)
 			{
 				connect(CEarthPyramidCache::getInstance(this), SIGNAL(cancelCacheSignal()), this, SLOT(cancelProcessSlot()), Qt::UniqueConnection);
 				if (m_pPyramidProcess == nullptr)
 				{
 					m_pPyramidProcess = new CPyramidProcessListener(this);
 					connect(m_pPyramidProcess, SIGNAL(onPyramidProcessSignal(double)), this, SLOT(pyramidProcessSlot(double)));
 					connect(m_pPyramidProcess, SIGNAL(onPyramidProcessFinishedSignal(const QString &, const QString &, const QString &, int)), this, SLOT(onPyramidProcessFinishedSlot(const QString &, const QString &, const QString &, int)));
 				}
 				m_pPyramidProcess->initProcess(strName, strPath, strDriverType, layerType);
				CEarthPyramidCache::getInstance(this)->showCacheWgt();
				HGxAlgor::CFxPyramidBuilder::build(strPath.toUtf8().data(), m_pPyramidProcess, true);
 				return true;
 			}
 		}
 	}
	return false;
}

void CEarthLayerTreeWidget::sendModifiedEvent(bool bModified)
{
	ctkDictionary message;
	message.insert("changeStatus", bModified);

	HGxEventHelperInstance.postEvent("event/pluginStatus", message);
}

// FxEarth::IFxEarthLayerService * CEarthLayerTreeWidget::getLayerServiceRefThread(int iSceneId)
// {
// 	ICore::mainWindowManager()->mainWindow()->setUpdatesEnabled(false);
// 	ICore::mainWindowManager()->globalMutex()->lock();
// 	FxEarth::IFxEarthLayerService *pEarthLayerService = earthLayerService(iSceneId);
// 	ICore::mainWindowManager()->globalMutex()->unlock();
// 	ICore::mainWindowManager()->mainWindow()->setUpdatesEnabled(true);
// 	return pEarthLayerService;
// }

// FxEarth::IFxEarthLayerService *CEarthLayerTreeWidget::getEarthLayerServiceThread(int iSceneId, const FxCommonUtils::TiXmlElement *pSceneEle,const FxCommonUtils::TiXmlElement *pRootEle, FxCore::IFxWorkspaceProcess *pProcess)
// {
// 	ICore::mainWindowManager()->globalMutex()->lock();
// 	ISceneUiService *pSceneService = getService<ISceneUiService>();
// 	if (pSceneService)
// 	{
// 		pSceneService->pauseRender(true);
// 	}
// 	FxEarth::IFxEarthLayerService *pLayerService = earthLayerService(iSceneId);
// 	pLayerService->load(pSceneEle, pProcess);
// 

	/************暂时不提供海图*****************/

// 	/// 加载海图
// 	FxScene::IFxSceneService * pSceneSvice = getService<FxScene::IFxSceneService>();
// 	if (pSceneSvice != nullptr)
// 	{
// 		// 获取海图服务
// 		//ctkServiceReference serviceRef = pSceneSvice->getServiceReference<FxEarth::IFxSeaEarthLayerService>(m_iSceneId);
//  		FxEarth::IFxSeaEarthLayerService *pEarthLayerService = pSceneSvice->getService<FxEarth::IFxSeaEarthLayerService>(m_iSceneId).get();
// // 		if (serviceRef)
// // 		{
// // 			pEarthLayerService = FreeXFrameworkInstance.getService<FxEarth::IFxSeaEarthLayerService>(serviceRef);
// // 		}
// 		if (pEarthLayerService != nullptr)
// 		{
// 			pEarthLayerService->clearLayers();
// 			pEarthLayerService->load(pSceneEle,pProcess);
// 		}
// 	}
// 
// 
// 	ICore::mainWindowManager()->globalMutex()->unlock();
// 
// 	return pLayerService;
// }

void CEarthLayerTreeWidget::reInitLayerTree()
{
    // 清空所有节点
    /// clear();
    /// 判断是否加入到树中

	/************暂时不提供海图*****************/
//     if (m_pSeaLayerRoot != NULL)
//     {
//         ///
//         QTreeWidgetItem *pItem = dynamic_cast<QTreeWidgetItem *>(m_pSeaLayerRoot);
//         int nIndex = indexOfTopLevelItem(pItem);
//         if (nIndex == -1 )
//         {
//             addTopLevelItem(m_pSeaLayerRoot);
//         }
//     }


    int iTopLeveCount = topLevelItemCount();
    for (int nTopLeveIndex = iTopLeveCount - 1; nTopLeveIndex >=0; --nTopLeveIndex)
    {
        QTreeWidgetItem *pItem = takeTopLevelItem(nTopLeveIndex);
        delete pItem;
        pItem = NULL;
    }

	/************暂时不提供海图*****************/
    //m_pSeaLayerRoot = NULL;

    // 初始化根节点
    QTreeWidgetItem *pTopCategoryItem1 = new CCommonItem(CLayerConfig::TopCategory);
    pTopCategoryItem1->setText(0, tr("map layer"));
    addTopLevelItem(pTopCategoryItem1);
    pTopCategoryItem1->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/root.png"));

	/************暂时不提供海图*****************/
//     /// 添加海图节点
//     CreatSeaLayerRootItem();
//     if (m_pSeaLayerRoot != NULL)
//     {
//         addTopLevelItem(m_pSeaLayerRoot);
//     }


	// 初始化图层分类
	m_pImageTypeItem = new CCommonItem(CLayerConfig::SecondCategory);
	m_pImageTypeItem->setText(0, (tr("image layer")));
	m_pImageTypeItem->setData(0, LayerTypeRole, CLayerConfig::ImageLayer);
	pTopCategoryItem1->addChild(m_pImageTypeItem);
	m_pImageTypeItem->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/layer.png"));

	m_pEleTypeItem = new CCommonItem(CLayerConfig::SecondCategory);
	m_pEleTypeItem->setText(0, tr("elevation layer"));
	m_pEleTypeItem->setData(0, LayerTypeRole, CLayerConfig::ElevationLayer);
	pTopCategoryItem1->addChild(m_pEleTypeItem);
	m_pEleTypeItem->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/layer.png"));

	m_pVectorTypeItem = new CCommonItem(CLayerConfig::SecondCategory);
	m_pVectorTypeItem->setText(0, tr("vector layer"));
	m_pVectorTypeItem->setData(0, LayerTypeRole, CLayerConfig::VectorLayer);
	pTopCategoryItem1->addChild(m_pVectorTypeItem);
	m_pVectorTypeItem->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/layer.png"));

	m_pLiveTypeItem = new CCommonItem(CLayerConfig::SecondCategory);
	m_pLiveTypeItem->setText(0, tr("live layer"));
	m_pLiveTypeItem->setData(0, LayerTypeRole, CLayerConfig::LiveLayer);
	pTopCategoryItem1->addChild(m_pLiveTypeItem);
	m_pLiveTypeItem->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/layer.png"));

	HGxScene::CGxLayersSet vecImgLayer;
	HGxScene::CGxLayersSet vecModelLayer;
	HGxScene::CGxLayersSet vecElevationLayer;
	HGxScene::CGxLayersSet vecLiveLayer;

	// 从底层获取所有图层
	getLayerManager(m_strSceneID)->getImageLayers(vecImgLayer);
	getLayerManager(m_strSceneID)->getModelLayers(vecModelLayer);
	getLayerManager(m_strSceneID)->getElevationLayers(vecElevationLayer);
	getLayerManager(m_strSceneID)->getLiveLayers(vecLiveLayer);
 
 	// 初始化所有图层
 	for (int i = 0; i < vecImgLayer.size(); ++i)
 	{
		HGxScene::CGxGlobalLayer* pEarthLayer = vecImgLayer[i];
 		CEarthLayerItem *pItem = new CEarthLayerItem(CLayerConfig::LayerItem, pEarthLayer);
 		pItem->setText(0, QString::fromUtf8(pEarthLayer->getName().c_str()));
		pItem->setData(0, Qt::UserRole, CLayerConfig::ImageLayer);
 		m_pImageTypeItem->insertChild(0, pItem);
 		if (!pEarthLayer->getVisible())
 		{
 			pItem->setCheckState(0, Qt::Unchecked);
 		}
 		if (i == vecImgLayer.size() - 1)
 		{
 			pItem->updateParent(pItem, true);
 		}
 	}

	for (int i = 0; i < vecModelLayer.size(); ++i)
	{
		HGxScene::CGxGlobalLayer* pEarthLayer = vecModelLayer[i];
		CEarthLayerItem *pItem = new CEarthLayerItem(CLayerConfig::LayerItem, pEarthLayer);
		pItem->setText(0, QString::fromUtf8(pEarthLayer->getName().c_str()));
		pItem->setData(0, Qt::UserRole, CLayerConfig::VectorLayer);
		m_pVectorTypeItem->insertChild(0, pItem);
		if (!pEarthLayer->getVisible())
		{
			pItem->setCheckState(0, Qt::Unchecked);
		}
		if (i == vecModelLayer.size() - 1)
		{
			pItem->updateParent(pItem, true);
		}
	}

	for (int i = 0; i < vecElevationLayer.size(); ++i)
	{
		HGxScene::CGxGlobalLayer* pEarthLayer = vecElevationLayer[i];
		CEarthLayerItem *pItem = new CEarthLayerItem(CLayerConfig::LayerItem, pEarthLayer);
		pItem->setText(0, QString::fromUtf8(pEarthLayer->getName().c_str()));
		pItem->setData(0, Qt::UserRole, CLayerConfig::ElevationLayer);
		m_pEleTypeItem->insertChild(0, pItem);
		if (!pEarthLayer->getVisible())
		{
			pItem->setCheckState(0, Qt::Unchecked);
		}
		if (i == vecElevationLayer.size() - 1)
		{
			pItem->updateParent(pItem, true);
		}
	}

 	for (int i = 0; i < vecLiveLayer.size(); ++i)
 	{
		HGxScene::CGxGlobalLayer* pEarthLayer = vecLiveLayer[i];
 		CEarthLayerItem *pItem = new CEarthLayerItem(CLayerConfig::LayerItem, pEarthLayer);
 		pItem->setText(0, QString::fromUtf8(pEarthLayer->getName().c_str()));
		pItem->setData(0, Qt::UserRole, CLayerConfig::LiveLayer);
 		m_pLiveTypeItem->insertChild(0, pItem);
 		if (!pEarthLayer->getVisible())
 		{
 			pItem->setCheckState(0, Qt::Unchecked);
 		}
 		if (i == vecLiveLayer.size() - 1)
 		{
 			pItem->updateParent(pItem, true);
 		}
 	}

	expandAll();
}

void CEarthLayerTreeWidget::itemSelectionChangedSlot()
{
// 	////add by yq00012
// 	QTreeWidgetItem *pItem = currentItem();
// 	if (pItem != NULL)
// 	{
// 		/// 如果是海图节点不做处理
// 		if (pItem->type() == CLayerConfig::SeaLayerCategory)
// 		{
// 			return;
// 		}
// 	}
// 
// 	CEarthLayerItem *pCurrentItem = dynamic_cast<CEarthLayerItem *>(currentItem());
// 	if (pCurrentItem == nullptr)
// 	{
// 
// 		m_pPropertyMgr->changeCurrentLayer(nullptr);
// 	}
// 	else
// 	{
// 		m_pPropertyMgr->changeCurrentLayer(pCurrentItem->earthLayer());
// 	}
}

void CEarthLayerTreeWidget::cancelProcessSlot()
{
	CMessageBox::ReturnButton ret = CMessageBox::question(tr("whether cancel and clear the generated pyramid cache?"), m_pMainWindow);
	if (ret == CMessageBox::Confirm)
	{
		if (m_pPyramidProcess != nullptr)
		{
			m_pPyramidProcess->stopProcessing();
		}
		//关闭金字塔缓存窗口
		CEarthPyramidCache::getInstance(this)->closeCacheWgt();

		if (m_pPyramidProcess != nullptr)
		{
			m_pPyramidProcess->stopProcessing();
		}
	}
}

void CEarthLayerTreeWidget::pyramidProcessSlot(double dfPercent)
{
	CEarthPyramidCache::getInstance()->setCacheProcess(dfPercent);
}

void CEarthLayerTreeWidget::onPyramidProcessFinishedSlot(const QString &strName, const QString &strPath, const QString &strDriverType, int iLayerType)
{
	CEarthPyramidCache::getInstance()->closeCacheWgt();
	addLayer(strName, strPath, strDriverType, CLayerConfig::LayerType(iLayerType));
}

void CEarthLayerTreeWidget::futureFinishedSlot()
{
	/**************此处需要获取地图图层服务*********************/

// 	QFutureWatcher<FxEarth::IFxEarthLayerService *> *pWatcher = static_cast<QFutureWatcher<FxEarth::IFxEarthLayerService *> *>(sender());
// 	FxEarth::IFxEarthLayerService *pEarthLayerService = pWatcher->result();
// 	pWatcher->deleteLater();

	reInitLayerTree();

// 	//z00011 增加任务数量
// 	FxCore::CLoadInfo::Instance()->removeCount();
// 	//end
	//CLoadIndicator::removeUseCount();

}

void CEarthLayerTreeWidget::getEarthLayerServiceFinishedSlot()
{
	/**************此处需要获取主窗口*********************/	

	//ICore::mainWindowManager()->mainWindow()->setUpdatesEnabled(true);
	
	/**************此处需要重写*********************/

//  	QFutureWatcher<FxEarth::IFxEarthLayerService *> *pWatcher = static_cast<QFutureWatcher<FxEarth::IFxEarthLayerService *> *>(sender());
//  	FxEarth::IFxEarthLayerService *pLayerService = pWatcher->result();
//  	int iLoadSceneId = pWatcher->property("sceneId").toInt();
//  	pWatcher->deleteLater();
//  	if (pLayerService == nullptr)
//  	{
//   		//z00011 增加任务数量
//  		FxCore::CLoadInfo::Instance()->removeCount();
//   		//end 
//  		return;
//  	}
// 
//  	// 如果是当前场景，则初始化界面
//  	if (iLoadSceneId == m_iSceneId)
//  	{
 
 		int iTopLeveCount = topLevelItemCount();
 		for (int nTopLeveIndex = iTopLeveCount - 1; nTopLeveIndex >=0; --nTopLeveIndex)
 		{
 			QTreeWidgetItem *pItem = takeTopLevelItem(nTopLeveIndex);
 			delete pItem;
 			pItem = NULL;
 		}
 
		/************暂时不提供海图*****************/
        //m_pSeaLayerRoot = NULL;
 
 
 		QTreeWidgetItem *pTopCategoryItem1 = new CCommonItem(CLayerConfig::TopCategory);
 		pTopCategoryItem1->setText(0, tr("map layer"));
 		addTopLevelItem(pTopCategoryItem1);
 		pTopCategoryItem1->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/root.png"));
 
 		m_pImageTypeItem = new CCommonItem(CLayerConfig::SecondCategory);
 		m_pImageTypeItem->setText(0, tr("image layer"));
 		m_pImageTypeItem->setData(0, LayerTypeRole, CLayerConfig::ImageLayer);
 		pTopCategoryItem1->addChild(m_pImageTypeItem);
 		m_pImageTypeItem->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/layer.png"));
 
 		m_pEleTypeItem = new CCommonItem(CLayerConfig::SecondCategory);
 		m_pEleTypeItem->setText(0, tr("elevation layer"));
 		m_pEleTypeItem->setData(0, LayerTypeRole, CLayerConfig::ElevationLayer);
 		pTopCategoryItem1->addChild(m_pEleTypeItem);
 		m_pEleTypeItem->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/layer.png"));
 
 		m_pVectorTypeItem = new CCommonItem(CLayerConfig::SecondCategory);
 		m_pVectorTypeItem->setText(0, tr("vector layer"));
 		m_pVectorTypeItem->setData(0, LayerTypeRole, CLayerConfig::VectorLayer);
 		pTopCategoryItem1->addChild(m_pVectorTypeItem);
 		m_pVectorTypeItem->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/layer.png"));
 
 		m_pLiveTypeItem = new CCommonItem(CLayerConfig::SecondCategory);
 		m_pLiveTypeItem->setText(0, tr("live layer"));
 		m_pLiveTypeItem->setData(0, LayerTypeRole, CLayerConfig::LiveLayer);
 		pTopCategoryItem1->addChild(m_pLiveTypeItem);
 		m_pLiveTypeItem->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/layer.png"));

		HGxScene::CGxLayersSet vecImgLayer;
		HGxScene::CGxLayersSet vecModelLayer;
		HGxScene::CGxLayersSet vecElevationLayer;
		HGxScene::CGxLayersSet vecLiveLayer;
 
		// 从底层获取所有图层
		getLayerManager(m_strSceneID)->getImageLayers(vecImgLayer);
		getLayerManager(m_strSceneID)->getModelLayers(vecModelLayer);
		getLayerManager(m_strSceneID)->getElevationLayers(vecElevationLayer);
		getLayerManager(m_strSceneID)->getLiveLayers(vecLiveLayer);
 
 		for (int i = 0; i < vecImgLayer.size(); ++i)
 		{
 			HGxScene::CGxGlobalLayer *pEarthLayer = vecImgLayer[i];
 			CEarthLayerItem *pItem = new CEarthLayerItem(CLayerConfig::LayerItem, pEarthLayer);
 			pItem->setText(0, QString::fromUtf8(pEarthLayer->getName().c_str()));
			pItem->setData(0, Qt::UserRole, CLayerConfig::ImageLayer);
 			m_pImageTypeItem->insertChild(0, pItem);
 			if (!pEarthLayer->getVisible())
 			{
 				pItem->setCheckState(0, Qt::Unchecked);
 			}
 			if (i == vecImgLayer.size() - 1)
 			{
 				pItem->updateParent(pItem, true);
 			}
 		}
 
 		for (int i = 0; i < vecModelLayer.size(); ++i)
 		{
			HGxScene::CGxGlobalLayer *pEarthLayer = vecModelLayer[i];
 			CEarthLayerItem *pItem = new CEarthLayerItem(CLayerConfig::LayerItem, pEarthLayer);
 			pItem->setText(0, QString::fromUtf8(pEarthLayer->getName().c_str()));
			pItem->setData(0, Qt::UserRole, CLayerConfig::VectorLayer);
 			m_pVectorTypeItem->insertChild(0, pItem);
 			if (!pEarthLayer->getVisible())
 			{
 				pItem->setCheckState(0, Qt::Unchecked);
 			}
 			if (i == vecModelLayer.size() - 1)
 			{
 				pItem->updateParent(pItem, true);
 			}
 		}
 
 		for (int i = 0; i < vecElevationLayer.size(); ++i)
 		{
			HGxScene::CGxGlobalLayer *pEarthLayer = vecElevationLayer[i];
 			CEarthLayerItem *pItem = new CEarthLayerItem(CLayerConfig::LayerItem, pEarthLayer);
 			pItem->setText(0, QString::fromUtf8(pEarthLayer->getName().c_str()));
			pItem->setData(0, Qt::UserRole, CLayerConfig::ElevationLayer);
 			m_pEleTypeItem->insertChild(0, pItem);
 			if (!pEarthLayer->getVisible())
 			{
 				pItem->setCheckState(0, Qt::Unchecked);
 			}
 			if (i == vecElevationLayer.size() - 1)
 			{
 				pItem->updateParent(pItem, true);
 			}
 		}
 
 		for (int i = 0; i < vecLiveLayer.size(); ++i)
 		{
			HGxScene::CGxGlobalLayer *pEarthLayer = vecLiveLayer[i];
 			CEarthLayerItem *pItem = new CEarthLayerItem(CLayerConfig::LayerItem, pEarthLayer);
 			pItem->setText(0,QString::fromUtf8(pEarthLayer->getName().c_str()));
			pItem->setData(0, Qt::UserRole, CLayerConfig::LiveLayer);
 			m_pLiveTypeItem->insertChild(0, pItem);
 			if (!pEarthLayer->getVisible())
 			{
 				pItem->setCheckState(0, Qt::Unchecked);
 			}
 			if (i == vecLiveLayer.size() - 1)
 			{
 				pItem->updateParent(pItem, true);
 			}
 		}
 
		/************暂时不提供海图*****************/
//          if (m_pSeaLayerRoot == NULL)
//          {
//              ////
//              CreatSeaLayerRootItem();
//          }
//          if (m_pSeaLayerRoot != NULL)
//          {
//              addTopLevelItem(m_pSeaLayerRoot);
//          }
 
         expandAll();
    // }

// 	CLoadIndicator::removeUseCount();
// 
//  //z00011 增加任务数量
//  FxCore::CLoadInfo::Instance()->removeCount();
//  //end 

	if (HGxScene::CGxControllerMgr::getInstance())
	{
		HGxScene::CGxControllerMgr::getInstance()->pauseSimulation(m_strSceneID, false);
	}
}

void CEarthLayerTreeWidget::addImageLayer(const QString &strName, const QString &strPath, const QString &strDriverType, bool bFromLocal, const QString &layerFormat )
{
// 	if (generatePyramid(strName, strPath, strDriverType, CLayerConfig::ImageLayer))
// 	{
// 		return;
// 	}
	addLayer(strName, strPath, strDriverType, CLayerConfig::ImageLayer, layerFormat);
}

void CEarthLayerTreeWidget::addEleLayer(const QString &strName, const QString &strPath, const QString &strDriverType, bool bFromLocal)
{
// 	if (generatePyramid(strName, strPath, strDriverType, CLayerConfig::ElevationLayer))
// 	{
// 		return;
// 	}
	addLayer(strName, strPath, strDriverType, CLayerConfig::ElevationLayer);
}

void CEarthLayerTreeWidget::addVectorLayer(const QString &strName, const QString &strPath, const QString &strDriverType, bool bFromLocal)
{
// 	if (generatePyramid(strName, strPath, strDriverType, CLayerConfig::VectorLayer))
// 	{
// 		return;
// 	}
	addLayer(strName, strPath, strDriverType, CLayerConfig::VectorLayer);
}

void CEarthLayerTreeWidget::addLiveLayer(HGxScene::CGxGlobalLayer* pLiveLayer)
{
	// 创建图层，并添加到场景中
	CEarthLayerItem *pEarthLayerItem = new CEarthLayerItem(CLayerConfig::LayerItem, pLiveLayer);

	QString strName = pLiveLayer->getName().c_str();
	if (strName.isEmpty())
	{
		pEarthLayerItem->setText(0, tr("tilt photography model"));
	}
	else
	{
		pEarthLayerItem->setText(0, QString::fromUtf8(pLiveLayer->getName().c_str()));
	}

	pEarthLayerItem->setData(0, Qt::UserRole, CLayerConfig::LiveLayer);
	m_pLiveTypeItem->insertChild(0, pEarthLayerItem);
	m_pLiveTypeItem->setExpanded(true);

	pEarthLayerItem->updateParent(pEarthLayerItem, true);
	topLevelItem(0)->setExpanded(true);
	setCurrentItem(pEarthLayerItem);

 	// 添加图层后自动定位 l180
 	{
		HGxScene::CGxSceneBase* pEarthScene = HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID);

  		if(pEarthScene)
  		{
 			HGxScene::CGxLayerLive* pLayer =  static_cast<HGxScene::CGxLayerLive*>(pLiveLayer);
 
 			if (NULL == pLayer)
 			{
 				return;
 			}

			HGxScene::CGxLayerManager::getOrCreateLayerManager(m_strSceneID)->addLayer(pLayer);

			HGxScene::CGxGeoContext* pGeoContext = static_cast<HGxScene::CGxGeoContext*>(pEarthScene->getSceneContext());

			if (NULL == pGeoContext)
			{
				return;
			}
 
 			osg::BoundingSphere bound = pLayer->getGraphNode()->getBound();
 
  			if(0.0 > bound.radius())//如果包围球计算不正确，用高程定位
  			{
  				osg::Vec3d llh = pLayer->getPosition();
 				HGxAlgor::degreeLL2LLH(pGeoContext->getMapNode(), llh);

				HGxCommon::SGxViewPoint stViewPoint;
				stViewPoint.stPoint = HGxCommon::CGxPoint(llh);
				stViewPoint.vecAttitude = osg::Vec3d(-90, 0, 0);
				stViewPoint.dDistance = 2000;

  				pEarthScene->locate(stViewPoint, 2); 
  			}
  			else //包围球有效，用包围球心计算真实海拔高度
  			{
  				osg::Vec3d llh;
				HGxAlgor::xyz2DegreeLLH(pGeoContext->getMapNode(), pLayer->getGraphNode()->getBound().center(), llh);

				HGxCommon::SGxViewPoint stViewPoint;
				stViewPoint.stPoint = HGxCommon::CGxPoint(llh);
				stViewPoint.vecAttitude = osg::Vec3d(-90, 0, 0);
				stViewPoint.dDistance = 2000;

  				pEarthScene->locate(stViewPoint, 2); 
  			}
  		}
 	}
}

void CEarthLayerTreeWidget::addLayer(const QString &strName, const QString &strPath, const QString &strDriverType, CLayerConfig::LayerType layerType, const QString &layerFormat )
{
	sendModifiedEvent(true);
	qDebug() << "addLayer:::::" << "to handled topic ";
	HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);
	if (NULL == pLayerManager)
	{
		return;
	}

  	// 创建图层，并添加到场景中
 	osg::ref_ptr<HGxScene::CGxLayerConfig> pLayerConfig = NULL;
  
  	switch (layerType)
  	{
  		// 影像
  	case CLayerConfig::ImageLayer:
  		{
			HGxScene::CGxLayerImageConfig* pLayerCfg = getLayerManager(m_strSceneID)->layerConfigFactory()->createImageLayerConfig(HGxScene::SLayerOption(strName.toUtf8().data(), strDriverType.toUtf8().data(), strPath.toUtf8().data()));
  			
			pLayerCfg->sourceConfig().layerFormat() = layerFormat.toUtf8().data();
				
		    pLayerCfg->cacheConfig().cachePath() = m_strCachePath;

			pLayerConfig = pLayerCfg;
  		}
  		break;
  		// 高程
  	case CLayerConfig::ElevationLayer:
  		{
			HGxScene::CGxLayerElevationConfig* pLayerCfg = getLayerManager(m_strSceneID)->layerConfigFactory()->createElevationLayerConfig(HGxScene::SLayerOption(strName.toUtf8().data(), strDriverType.toUtf8().data(), strPath.toUtf8().data()));
			
			pLayerCfg->cacheConfig().cachePath() = m_strCachePath;

			pLayerConfig = pLayerCfg;
  		}
  		break;
  		// 矢量
  	case CLayerConfig::VectorLayer:
  		{
			HGxScene::CGxLayerModelConfig* pLayerCfg = getLayerManager(m_strSceneID)->layerConfigFactory()->createModelLayerConfig(HGxScene::SLayerOption(strName.toUtf8().data(), strDriverType.toUtf8().data(), strPath.toUtf8().data()));
  			pLayerConfig = pLayerCfg;
  		}
  		break;
  	}
	qDebug() << "addLayer:::end::" << "to handled topic ";
 	if(pLayerConfig.valid())
 	{
 		CLoadIndicator::addUseCount(m_pMainWindow);
 
		QFutureWatcher<HGxScene::CGxGlobalLayer*>* pWatcher = new QFutureWatcher<HGxScene::CGxGlobalLayer*>();
   		connect(pWatcher, SIGNAL(finished()), this, SLOT(addLayerFinishSlot()));
   		pWatcher->setFuture(QtConcurrent::run( std::bind(&CEarthLayerTreeWidget::addLayerInThread, this, pLayerConfig) ));
 	}
}

void CEarthLayerTreeWidget::addLayer( const QString &strName, const SLayerPropertyStruct& stLayerStruct, CLayerConfig::LayerType layerType )
{
	if(strName != stLayerStruct.strLayerName)
	{
		//FREEX_LOG_ERROR << "addLayer: strName != stLayerStruct.strLayerName";
	}
	return addLayer(strName, stLayerStruct.strServerWebsite, stLayerStruct.strDriveName, layerType);
}

HGxScene::CGxGlobalLayer* CEarthLayerTreeWidget::addLayerInThread(HGxScene::CGxLayerConfig* pLayerConfig)
{
	if (HGxScene::CGxControllerMgr::getInstance())
	{
		HGxScene::CGxControllerMgr::getInstance()->pauseSimulation(m_strSceneID, true);
	}
	
	if (getLayerManager(m_strSceneID))
	{
		HGxScene::CGxGlobalLayer* pLayer = getLayerManager(m_strSceneID)->createLayer(*pLayerConfig);

		if (pLayerConfig)
		{
			getLayerManager(m_strSceneID)->addLayer(pLayer);

			return pLayer;
		}
	}

	return NULL;
}

// bool CEarthLayerTreeWidget::load(const FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess)
// {
// 	qDebug() << "loading earth layer";
// 	// 读取三维图层节点
// 	const FxCommonUtils::TiXmlElement *pRoot = pEle->FirstChildElement("earthlayer_ui");
// 	if (pRoot == nullptr)
// 	{
// 		return false;
// 	}
// 
// 	ISceneUiService *pSceneService = getService<ISceneUiService>();
// 	if (!pSceneService)
// 	{
// 		return false;
// 	}
// 
// 	for (const FxCommonUtils::TiXmlElement *pSceneEle = pRoot->FirstChildElement(); pSceneEle != nullptr; pSceneEle = pSceneEle->NextSiblingElement())
// 	{
// 		QString strSceneName = QString::fromUtf8(pSceneEle->Attribute("SceneName"));
// 
// 		// 根据场景名称获取场景对象
// 		ISceneInterface *pInterface = pSceneService->scene(strSceneName);
// 		if (pInterface == nullptr)
// 		{
// 			continue;
// 		}
// 		// 获取场景id
// 		int iSceneId = pSceneService->scene(strSceneName)->sceneId();
// 
// 		//z00011 增加任务数量
// 		FxCore::CLoadInfo::Instance()->addCount();
// 		//end 
// 
// 		// 根据场景id获取三维图层服务
// 		CLoadIndicator::addUseCount(ICore::mainWindowManager()->mainWindow());
// 		ICore::mainWindowManager()->mainWindow()->setUpdatesEnabled(false);
// 		QFutureWatcher<FxEarth::IFxEarthLayerService *> *pWatcher = new QFutureWatcher<FxEarth::IFxEarthLayerService *>();
// 		pWatcher->setProperty("sceneId", iSceneId);// 将场景id存入属性中
// 		connect(pWatcher, SIGNAL(finished()), this, SLOT(getEarthLayerServiceFinishedSlot()));
// 		pWatcher->setFuture(QtConcurrent::run(std::bind(&CEarthLayerTreeWidget::getEarthLayerServiceThread, this, iSceneId, pSceneEle, pEle, pProcess)));
// 	}
// 
// 	return true;
// }
// 
// bool CEarthLayerTreeWidget::save(FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess)
// {
// 	FxCommonUtils::TiXmlElement *pRoot = new FxCommonUtils::TiXmlElement("earthlayer_ui");
// 	pEle->LinkEndChild(pRoot);
// 
// 	// 获取所有三维场景
// 	ISceneUiService *pSceneService = getService<ISceneUiService>();
// 	if (pSceneService == nullptr)
// 	{
// 		return false;
// 	}
// 
// 	QList<ISceneInterface *> listMapScenes = pSceneService->earthScenes();
// 	// 保存每个三维场景中的图层
// 	foreach(auto *pScene, listMapScenes)
// 	{
// 		FxCommonUtils::TiXmlElement *pSceneEle = new FxCommonUtils::TiXmlElement("scene");
// 		// modified by w00025 工作空间是utf-8编码
// 		pSceneEle->SetAttribute("SceneName", pScene->sceneName().toUtf8().data());
// 		pRoot->LinkEndChild(pSceneEle);
// 
// 		FxEarth::IFxEarthLayerService *pLayerService = earthLayerService(pScene->sceneId());
// 		if (pLayerService != nullptr)
// 		{
// 			pLayerService->save(pSceneEle, pProcess);
// 		}
// 
// 
// 		/// 加载海图
// 		FxScene::IFxSceneService * pSceneSvice = getService<FxScene::IFxSceneService>();
// 		if (pSceneSvice != nullptr)
// 		{
// 			// 获取海图服务
// 			//ctkServiceReference serviceRef = pSceneSvice->getServiceReference<FxEarth::IFxSeaEarthLayerService>(m_iSceneId);
// 			FxEarth::IFxSeaEarthLayerService *pEarthLayerService = pSceneSvice->getService<FxEarth::IFxSeaEarthLayerService>(m_iSceneId).get();
// 			// 			if (serviceRef)
// 			// 			{
// 			// 				pEarthLayerService = FreeXFrameworkInstance.getService<FxEarth::IFxSeaEarthLayerService>(serviceRef);
// 			// 			}
// 			if (pEarthLayerService != nullptr)
// 			{
// 				pEarthLayerService->save(pSceneEle, pProcess);
// 			}
// 		}
// 
// 	}
// 
// 	return true;
// }

void CEarthLayerTreeWidget::addLayerFinishSlot()
{
	CLoadIndicator::removeUseCount();

	if (HGxScene::CGxControllerMgr::getInstance())
	{
		HGxScene::CGxControllerMgr::getInstance()->pauseSimulation(m_strSceneID, false);
	}

	QFutureWatcher<HGxScene::CGxGlobalLayer* >* pWatcher = static_cast<QFutureWatcher<HGxScene::CGxGlobalLayer*> *>(sender());
	if (!pWatcher)
	{
		return;
	}
	pWatcher->deleteLater();

	HGxScene::CGxGlobalLayer* pEarthLayer = pWatcher->result();

	if (pEarthLayer)
	{
		CEarthLayerItem* pEarthLayerItem = new CEarthLayerItem(CLayerConfig::LayerItem, pEarthLayer);
		pEarthLayerItem->setText(0, QString::fromUtf8(pEarthLayer->getName().c_str()));

		if (pEarthLayer->getLayerType() == HGxScene::E_IMAGE_LAYER)
		{
			pEarthLayerItem->setData(0, Qt::UserRole, CLayerConfig::ImageLayer);
			m_pImageTypeItem->insertChild(0, pEarthLayerItem);
			m_pImageTypeItem->setExpanded(true);
		}
		else if (pEarthLayer->getLayerType() == HGxScene::E_ELEVA_LAYER)
		{
			pEarthLayerItem->setData(0, Qt::UserRole, CLayerConfig::ElevationLayer);
			m_pEleTypeItem->insertChild(0, pEarthLayerItem);
			m_pEleTypeItem->setExpanded(true);
		}
		else if (pEarthLayer->getLayerType() == HGxScene::E_VECTOR_LAYER)
		{
			pEarthLayerItem->setData(0, Qt::UserRole, CLayerConfig::VectorLayer);
			m_pVectorTypeItem->insertChild(0, pEarthLayerItem);
			m_pVectorTypeItem->setExpanded(true);
		}

		pEarthLayerItem->updateParent(pEarthLayerItem, true);
		topLevelItem(0)->setExpanded(true);
		setCurrentItem(pEarthLayerItem);
	}
}

void CEarthLayerTreeWidget::beginAddImgLayer()
{
	setCurrentItem(m_pImageTypeItem);

	QAction act(this);
	act.setData(CEarthLayerTreeWidget::ADD_DATA);
	contextMenuActionSlot(&act);
}

void CEarthLayerTreeWidget::beginAddEleLayer()
{
	setCurrentItem(m_pEleTypeItem);

	QAction act(this);
	act.setData(CEarthLayerTreeWidget::ADD_DATA);
	contextMenuActionSlot(&act);
}

void CEarthLayerTreeWidget::beginAddVecLayer()
{
	setCurrentItem(m_pVectorTypeItem);

	QAction act(this);
	act.setData(CEarthLayerTreeWidget::ADD_DATA);
	contextMenuActionSlot(&act);
}

void CEarthLayerTreeWidget::beginAddSeaLayer()
{
/************暂时不提供海图*****************/

//  if (m_pSeaLayerRoot != NULL)
//  {
//      m_pSeaLayerRoot->beginAddSeaLayer();
//  }
// 	else
// 	{
// 		CMessageBox::information(tr("earth seamap plugin not load,please check"),  ICore::mainWindowManager()->sceneWindow());
// 	}

}
void CEarthLayerTreeWidget::delLayer()
{
	deleteShortCutSlot();
}

void CEarthLayerTreeWidget::initWhenPluginStart()
{
	/***********************此处需要获取当前场景*********************/

// 	// 获取当前场景id
// 	FxCore::CFxServicePtr<ISceneUiService> pSceneService = FreeXFrameworkInstance.getService<ISceneUiService>();
// 	if(!pSceneService)
// 	{
// 		return;
// 	}
// 
// 	QString strCurSceneType = pSceneService->activeSceneType();
// 	if (strCurSceneType == "scene_map")
// 	{
// 		return;
// 	}
// 	int iCurSceneId = pSceneService->activeScene();
// 	if (iCurSceneId < 0 || iCurSceneId == m_iSceneId)
// 	{
// 		return;
// 	}

// 	//z00011 增加任务数量
// 	FxCore::CLoadInfo::Instance()->addCount();
// 	//end 

//	// 初始化图层树
//	m_iSceneId = iCurSceneId;

	/***********************此处需要获取主窗口，暂时用NULL*********************/

	//CLoadIndicator::addUseCount(/*ICore::mainWindowManager()->mainWindow()*/NULL);

	/***********************此处需要获取三维图层服务*********************/

// 	QFutureWatcher<FxEarth::IFxEarthLayerService *> *pWatcher = new QFutureWatcher<FxEarth::IFxEarthLayerService *>();
// 	connect(pWatcher, SIGNAL(finished()), this, SLOT(futureFinishedSlot()));
// 	pWatcher->setFuture(QtConcurrent::run(std::bind(&CEarthLayerTreeWidget::getLayerServiceRefThread, this, m_iSceneId)));
}

QWidget* CEarthLayerTreeWidget::getEarthLayerPropertyMgrWgt()
{
	return m_pPropertyMgr;
}

void CEarthLayerTreeWidget::addLiveLayerSlot()
{
	if (NULL == m_pAddTiltModelWgt)
	{
		CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());
		if (NULL == uiFrameworkService)
		{
			return;
		}

		osg::ref_ptr<HGxScene::CGxLayerLiveConfig> pLayerConfig = new HGxScene::CGxLayerLiveConfig;
		HGxScene::CGxLayerLive* pLayer = static_cast<HGxScene::CGxLayerLive*>(HGxScene::CGxLayerManager::getOrCreateLayerManager(m_strSceneID)->createLayer(*pLayerConfig.get()));
		m_pAddTiltModelWgt = new CTiltAddModelWgt(true, pLayer, parentWidget() ? parentWidget() : uiFrameworkService->mainWindow());
		connect(m_pAddTiltModelWgt, SIGNAL(linkTiltToolSignal()), this, SLOT(slotTiltTool()));
		m_pAddTiltModelWgt->setTileModelShow(true);
		m_pAddTiltModelWgt->show();
	}
	else
	{
		bool bStatus = m_pAddTiltModelWgt->getTileModelShow();
		if (!bStatus)
		{
			if (m_pAddTiltModelWgt)
			{
				disconnect(m_pAddTiltModelWgt, SIGNAL(linkTiltToolSignal()), this, SLOT(slotTiltTool()));
				delete m_pAddTiltModelWgt;
				m_pAddTiltModelWgt = NULL;
			}

			CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());
			if (NULL == uiFrameworkService)
			{
				return;
			}

			osg::ref_ptr<HGxScene::CGxLayerLiveConfig> pLayerConfig = new HGxScene::CGxLayerLiveConfig;
			HGxScene::CGxLayerLive* pLayer = static_cast<HGxScene::CGxLayerLive*>(HGxScene::CGxLayerManager::getOrCreateLayerManager(m_strSceneID)->createLayer(*pLayerConfig.get()));
			m_pAddTiltModelWgt = new CTiltAddModelWgt(true,pLayer, parentWidget() ? parentWidget() : uiFrameworkService->mainWindow());
			connect(m_pAddTiltModelWgt, SIGNAL(linkTiltToolSignal()), this, SLOT(slotTiltTool()));
			m_pAddTiltModelWgt->setTileModelShow(true);
			m_pAddTiltModelWgt->show();
		}
	}
}
void CEarthLayerTreeWidget::slotTiltTool()
{
	if (NULL == m_pTiltToolWgt)
	{
		/***********************此处需要获取主窗口，暂时用NULL*********************/

		m_pTiltToolWgt = new CTiltToolWgt(/*parentWidget() ? parentWidget() : ICore::mainWindowManager()->mainWindow()*/);
		m_pTiltToolWgt->show();
	}
	else
	{
		if (m_pTiltToolWgt)
		{
			delete m_pTiltToolWgt;
			m_pTiltToolWgt = NULL;
		}

		/***********************此处需要获取主窗口，暂时用NULL*********************/

		m_pTiltToolWgt = new CTiltToolWgt(/*parentWidget() ? parentWidget() : ICore::mainWindowManager()->mainWindow()*/);
		m_pTiltToolWgt->show();

	}
}
void CEarthLayerTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QTreeWidgetItem *pCurrentItem = itemAt(event->pos());
	if (pCurrentItem == nullptr)
	{
		return;
	}

	if (m_pContextMenu == nullptr)
	{
		m_pContextMenu = new QMenu(this);
		m_pContextMenu->setWindowFlags(m_pContextMenu->windowFlags() | Qt::FramelessWindowHint);
		m_pContextMenu->setAttribute(Qt::WA_TranslucentBackground);
		connect(m_pContextMenu, SIGNAL(triggered(QAction *)), this, SLOT(contextMenuActionSlot(QAction *)));
		m_pContextMenu->hide();
	}
	m_pContextMenu->clear();

    CLayerConfig::TreeItemType iItemType = (CLayerConfig::TreeItemType)pCurrentItem->type();
    switch (iItemType)
    {
    case CLayerConfig::TopCategory:
        break;
    case CLayerConfig::SecondCategory:
        {
            CLayerConfig::LayerType layerType = (CLayerConfig::LayerType)pCurrentItem->data(0, LayerTypeRole).toInt();
            if (CLayerConfig::LiveLayer == layerType)
            {
                QAction *pAction = new QAction(tr("add tilt photography"), m_pContextMenu);
                pAction->setIcon(QIcon(":/images_earth_layer/Resources/images_earth_layer/tianjia_normal.png"));
                pAction->setData(CEarthLayerTreeWidget::ADD_TILEMODEL);
                m_pContextMenu->addAction(pAction);
                m_pContextMenu->popup(event->globalPos());
                connect(pAction, SIGNAL(triggered()), this, SLOT(addLiveLayerSlot()));
            }
            else
            {
                QAction *pAction = new QAction(tr("add data"), m_pContextMenu);
                pAction->setIcon(QIcon(":/images_earth_layer/Resources/images_earth_layer/tianjia_normal.png"));
                pAction->setData(CEarthLayerTreeWidget::ADD_DATA);
                m_pContextMenu->addAction(pAction);
                m_pContextMenu->popup(event->globalPos());
            }
        }
        break;
    case CLayerConfig::LayerItem:
        {
            QAction *pAction1 = new QAction(tr("rename"), m_pContextMenu);
            pAction1->setData(CEarthLayerTreeWidget::RENAME);
            pAction1->setShortcut(QKeySequence("F2"));
            QAction *pAction2 = new QAction(tr("delete"), m_pContextMenu);
            pAction2->setData(CEarthLayerTreeWidget::DELETE_LAYER);
            pAction2->setShortcut(QKeySequence("Delete"));
            QAction *pAction3 = new QAction(tr("property"), m_pContextMenu);
            pAction3->setData(CEarthLayerTreeWidget::PROPERTIES);
            pAction3->setShortcut(QKeySequence("R"));
            QAction* pAction4 = new QAction(tr("zoom to layer range"), m_pContextMenu);
            pAction4->setData(CEarthLayerTreeWidget::ZOOM_TO_LAYER);
            m_pContextMenu->addAction(pAction4);
            m_pContextMenu->addAction(pAction1);
            m_pContextMenu->addAction(pAction2);
            m_pContextMenu->addAction(pAction3);
            m_pContextMenu->popup(event->globalPos());
        }
        break;
    case  CLayerConfig::SeaLayerCategory:
        {

/************暂时不提供海图*****************/

//             /// 通过Item 获取Action
//             if (m_pSeaLayerRoot != NULL)
//             {
//                 m_pContextMenu->addActions(m_pSeaLayerRoot->GetAction(pCurrentItem));
//             }
//             m_pContextMenu->exec(QCursor::pos());

        }
        break;
    default:
        break;
    }
}

void CEarthLayerTreeWidget::handleEvent(const ctkEvent &event)
{
	if (event.getTopic() == "event/sceneCreated")
	{
		std::string strSceneType = event.getProperty("SceneType").toString().toStdString();
		if (strSceneType == GX_EARTH_SCENE_TYPE)
		{
			// 记录当前场景id
			m_strSceneID = event.getProperty("SceneID").toString().toStdString();

			bool bCreateFromFile = false;/*(event.getProperty("Condition").toString() != "CreatedFromUser");*/

// 			//z00011 增加任务数量
// 			FxCore::CLoadInfo::Instance()->addCount();
// 			//end 

			// begin g00061 2019-9-28 注释，具体实现代码应放到sceneCreatedSignal槽函数中实现，避免处理超时
// 			if (NULL == m_pSeaLayerRoot)
// 			{
// 				CreatSeaLayerRootItem();
// 			}
// 			else
// 			{
// 				m_pSeaLayerRoot->SetSceneId(m_iSceneId);
// 			}
			//end

			emit sceneCreatedSignal(m_strSceneID, bCreateFromFile);
		}
	}
	else if (event.getTopic() == "event/sceneSwitch")
	{
		QString strSceneType = event.getProperty("SceneType").toString();
		if (strSceneType == GX_EARTH_SCENE_TYPE)
		{
			// 记录当前场景id
			m_strSceneID = event.getProperty("SceneID").toString().toStdString();

			// 如果是打开工作空间文件时发出的，则返回
			if (event.getProperty("Condition").toString() == "CreatedFromFile")
			{
				return;
			}
			sceneSwitchSlot(m_strSceneID);

			reInitLayerTree();
		}
		else
		{
			clearSelection();

			/***********************此处需要获取工具管理*********************/
			//ICore::toolManager()->tryToSetCurrentPropertyWidget(nullptr);
		}
	}
}

void CEarthLayerTreeWidget::layerNameChangedSlot(const QString &strLayerName)
{
	sendModifiedEvent(true);

	CEarthLayerItem *pEarthLayerItem = static_cast<CEarthLayerItem *>(currentItem());
	if (pEarthLayerItem)
	{
		pEarthLayerItem->setText(0, strLayerName);
	}
}

void CEarthLayerTreeWidget::contextMenuActionSlot(QAction *pAction)
{
	if (pAction == nullptr)
	{
		return;
	}
	QTreeWidgetItem *pCurrentItem = currentItem();
	if (pCurrentItem == nullptr)
	{
		return;
	}
	CEarthLayerTreeWidget::EarthLayerMenuAction menuAction = (CEarthLayerTreeWidget::EarthLayerMenuAction)(pAction->data().toInt());

	switch (menuAction)
	{
		// 添加数据
	case CEarthLayerTreeWidget::ADD_DATA:
		{
			// 设置文件选择类型
			QStringList filters;
			QString strLayer = pCurrentItem->text(0);
			if (strLayer == tr("image layer"))
			{
				filters <<"img" << "tif";
			}
			else if (strLayer == tr("elevation layer"))
			{
				filters << "shp" << "img" << "kml" << "tif";
			}
			else if (strLayer == tr("vector layer"))
			{
				filters << "shp" << "kml";
			}				

			// 某些数据类型可以作为多种图层类型添加（如:tif），图层类型由外部指定（解决右键无法添加高程图层问题 fixed by g00034）
			QVariant val = pCurrentItem->data(0, LayerTypeRole);
			if(val.isValid())
			{

				/***********************此处需要获取主窗口，暂时用NULL*********************/

				CGxDataResourcesUtils::getServerDataWindow3D(/*ICore::mainWindowManager()->mainWindow()*/)->setCurLayerType((CLayerConfig::LayerType)val.toInt());
			}

			/***********************此处需要获取主窗口，暂时用NULL*********************/

			CGxDataResourcesUtils::getServerDataWindow3D(/*ICore::mainWindowManager()->mainWindow()*/)->setFileFilter(filters);
			CGxDataResourcesUtils::getServerDataWindow3D(/*ICore::mainWindowManager()->mainWindow()*/)->show();

			/***********************此处需要获取主窗口，暂时用NULL*********************/

			bool b = connect( CGxDataResourcesUtils::getServerDataWindow3D(/*ICore::mainWindowManager()->mainWindow()*/), SIGNAL(addLayerToSceneSignal(const QString &, const QString &, const QString &, CLayerConfig::LayerType)), this, SLOT(addLayerToSceneSlot(const QString &, const QString &, const QString &, CLayerConfig::LayerType)), Qt::UniqueConnection);
			b = connect(CGxDataResourcesUtils::getServerDataWindow3D(/*ICore::mainWindowManager()->mainWindow()*/), SIGNAL(addServerLayerToSceneSignal(const QString &, const SLayerPropertyStruct &, CLayerConfig::LayerType)), this, SLOT(addServerLayerToSceneSlot(const QString&, const SLayerPropertyStruct &, CLayerConfig::LayerType)), Qt::UniqueConnection);				
			int a = 0;
		}
		break;
		// 重命名
	case CEarthLayerTreeWidget::RENAME:
		{
			pCurrentItem->setData(0, CurrentTextRole, pCurrentItem->text(0));
			pCurrentItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
			editItem(pCurrentItem, 0);
		}
		break;
		// 删除
	case CEarthLayerTreeWidget::DELETE_LAYER:
		{
			deleteShortCutSlot();
		}
		break;
		// 缩放至图层范围
	case CEarthLayerTreeWidget::ZOOM_TO_LAYER:
		{
			if (nullptr != pCurrentItem)
			{
				itemDoubleClickedSlot(pCurrentItem, 0);
			}
		}
		break;
		// 属性
	case CEarthLayerTreeWidget::PROPERTIES:
		{
			CEarthLayerItem *pEarthLayerItem = static_cast<CEarthLayerItem *>(pCurrentItem);
			if (pEarthLayerItem == nullptr)
			{
				return;
			}

			m_pPropertyMgr->showLayerProperty(pEarthLayerItem->earthLayer());
		}
		break;
	default:
		break;
	}
}

void CEarthLayerTreeWidget::editingFinishedSlot(const QModelIndex &index)
{
	sendModifiedEvent(true);

	QTreeWidgetItem *pEditingItem = itemFromIndex(index);
	
	///如果是海图图层返回不做处理 add by yq00012
	int nType = pEditingItem->type();
	if (nType == CLayerConfig::SeaLayerCategory)
	{
		return;
	}
	
	pEditingItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

	CEarthLayerItem *pEarthLayerItem = static_cast<CEarthLayerItem *>(pEditingItem);
	if (pEarthLayerItem == nullptr)
	{
		return;
	}

	QString strTextAfterEdit = pEarthLayerItem->text(0);
	if (strTextAfterEdit.isEmpty())
	{
		pEarthLayerItem->setText(0, pEarthLayerItem->data(0, CurrentTextRole).toString());
		return;
	}

 	// 更新树节点和底层信息
 	HGxScene::CGxGlobalLayer *pLayer = pEarthLayerItem->earthLayer();
 	if (pLayer == nullptr)
 	{
 		return;
 	}
 	pLayer->setName(strTextAfterEdit.toUtf8().data());

	m_pPropertyMgr->changeCurrentLayer(pLayer);
}

void CEarthLayerTreeWidget::addLayerToSceneSlot(const QString &strName, const QString &strPath, const QString &strDriverType, CLayerConfig::LayerType layerType)
{
	QTreeWidgetItem *pCurrentItem = currentItem();
	if (pCurrentItem == nullptr)
	{
		return;
	}
	if (pCurrentItem->type() != CLayerConfig::SecondCategory)
	{
		return;
	}
	CLayerConfig::LayerType typeLayerType = (CLayerConfig::LayerType)pCurrentItem->data(0, LayerTypeRole).toInt();
	if (typeLayerType != layerType)
	{
		return;
	}

// 	if (generatePyramid(strName, strPath, strDriverType, layerType))
// 	{
// 		return;
// 	}
	addLayer(strName, strPath, strDriverType, layerType);
}

void CEarthLayerTreeWidget::renameShortCutSlot()
{
	emit shorcutRenameItemSignal();

	sendModifiedEvent(true);

	QList<QTreeWidgetItem *> listSelcItems = selectedItems();
	if (listSelcItems.size() <= 0 || listSelcItems.size() > 1)
	{
		return;
	}
	CCheckableTreeItem *pCurrentItem = dynamic_cast<CCheckableTreeItem *>(listSelcItems[0]);
	if (pCurrentItem == nullptr)
	{
		return;
	}
	CLayerConfig::TreeItemType iItemType = (CLayerConfig::TreeItemType)pCurrentItem->type();
	if (pCurrentItem->type() == CLayerConfig::LayerItem)
	{
		QAction act(this);
		act.setData(CEarthLayerTreeWidget::RENAME);
		contextMenuActionSlot(&act);
	}
}

void CEarthLayerTreeWidget::deleteShortCutSlot()
{
	emit shortcutDeleteItemSignal();

	sendModifiedEvent(true);

	QList<QTreeWidgetItem *> listSelcItems = selectedItems();
	if (listSelcItems.size() <= 0)
	{
		return;
	}

	if (listSelcItems.size() == 1)
	{
		CCheckableTreeItem *pCurrentItem = dynamic_cast<CCheckableTreeItem *>(listSelcItems[0]);
		if (pCurrentItem == nullptr)
		{
			return;
		}
		CLayerConfig::TreeItemType iItemType = (CLayerConfig::TreeItemType)pCurrentItem->type();
		if (pCurrentItem->type() == CLayerConfig::LayerItem)
		{
			CEarthLayerItem *pEarthLayerItem = static_cast<CEarthLayerItem *>(pCurrentItem);
			if (pEarthLayerItem && getLayerManager(m_strSceneID))
			{
				/***********************此处需要获取主窗口，暂时用NULL*********************/

				CMessageBox::ReturnButton ret = CMessageBox::question(tr("are you sure to delete the selected layer?")/*,   ICore::mainWindowManager()->mainWindow()*/);
				if (ret == CMessageBox::Cancel)
				{
					return;
				}

				m_pPropertyMgr->deleteLayerProperty(pEarthLayerItem->earthLayer());

				getLayerManager(m_strSceneID)->deleteLayer(pEarthLayerItem->earthLayer());

 				// 界面中删除
 				CCheckableTreeItem *pParentItem = static_cast<CCheckableTreeItem *>(pCurrentItem->parent());
 				if (pParentItem != nullptr)
 				{
 					pParentItem->removeChild(pCurrentItem);
 					delete pCurrentItem;
 					pCurrentItem = nullptr;
 
 					pParentItem->updateSelf();
 				}
			}
		}
	}
	else
	{
		/***********************此处需要获取主窗口，暂时用NULL*********************/

		CMessageBox::ReturnButton ret = CMessageBox::question(tr("are you sure to delete the selected layer?")/*,   ICore::mainWindowManager()->mainWindow()*/);
		if (ret == CMessageBox::Cancel)
		{
			return;
		}

		// 批量删除图层 add by w00025
		for (int i = 0; i < listSelcItems.size(); ++i)
		{
			CCheckableTreeItem *pCurrentItem = dynamic_cast<CCheckableTreeItem *>(listSelcItems[i]);
			if (pCurrentItem == nullptr)
			{
				continue;
			}

			CLayerConfig::TreeItemType iItemType = (CLayerConfig::TreeItemType)pCurrentItem->type();
			if (pCurrentItem->type() == CLayerConfig::LayerItem)
			{
				CEarthLayerItem *pEarthLayerItem = static_cast<CEarthLayerItem *>(pCurrentItem);
				if (pEarthLayerItem && getLayerManager(m_strSceneID))
				{
					m_pPropertyMgr->deleteLayerProperty(pEarthLayerItem->earthLayer());

					getLayerManager(m_strSceneID)->deleteLayer(pEarthLayerItem->earthLayer());

 					// 界面中删除
 					CCheckableTreeItem *pParentItem = static_cast<CCheckableTreeItem *>(pCurrentItem->parent());
 					if (pParentItem != nullptr)
 					{
 						pParentItem->removeChild(pCurrentItem);
 						delete pCurrentItem;
 						pCurrentItem = nullptr;
 
 						pParentItem->updateSelf();
 					}
				}
			}
		}
	}
}

void CEarthLayerTreeWidget::propertyShortCutSlot()
{
	emit shortcutPropertyItemSignal();

	QList<QTreeWidgetItem *> listSelcItems = selectedItems();
	if (listSelcItems.size() <= 0 || listSelcItems.size() > 1)
	{
		return;
	}

	CCheckableTreeItem *pCurrentItem = static_cast<CCheckableTreeItem *>(listSelcItems[0]);
	if (pCurrentItem == nullptr)
	{
		return;
	}
	CLayerConfig::TreeItemType iItemType = (CLayerConfig::TreeItemType)pCurrentItem->type();
	if (pCurrentItem->type() == CLayerConfig::LayerItem)
	{
		QAction act(this);
		act.setData(CEarthLayerTreeWidget::PROPERTIES);
		contextMenuActionSlot(&act);
	}
}

void CEarthLayerTreeWidget::sceneCreatedSlot(std::string strSceneID, bool bCreateFromFile)
{
/************暂时不提供海图*****************/

     //// begin g00061 2019-9-28 具体实现代码应放到sceneCreatedSignal槽函数中实现，避免处理超时
     //if (NULL == m_pSeaLayerRoot)
     //{
     //    CreatSeaLayerRootItem();
     //}
     //else
     //{
     //    m_pSeaLayerRoot->SetSceneId(m_iSceneId);
     //}
     ////end

	if (NULL == m_pMainWindow)
	{
		CGxUiFrameworkServiceInterface* pUiFrameWork = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

		m_pMainWindow = pUiFrameWork->mainWindow();
	}

	if (getLayerManager(strSceneID))
	{
 		if(!bCreateFromFile)
 		{
 			//FREEX_LOG_INFO << "添加默认图层";	
			osg::ref_ptr<HGxScene::CGxLayerConfig> pLayerConfig = getLayerManager(strSceneID)->layerConfigFactory()->createImageLayerConfig(HGxScene::SLayerOption("default_img", "gdal", HGX_FILE_CONFIG().getFullPath("earth/world.tif")));
			
			if (pLayerConfig.valid())
			{
				HGxScene::CGxGlobalLayer* pLayer = getLayerManager(strSceneID)->createLayer(*pLayerConfig.get());
				if (pLayer)
				{
					getLayerManager(strSceneID)->addLayer(pLayer);
				}
			}
 		}

		reInitLayerTree();
	}

	CLoadIndicator::removeUseCount();
	//异步发送事件
	ctkDictionary event;
	HGxEventHelperInstance.sendEvent("event/LoginFinished", event);
//z00011 增加任务数量
//FxCore::CLoadInfo::Instance()->removeCount();
//end 
}

void CEarthLayerTreeWidget::sceneSwitchSlot(std::string iSceneId)
{
	/***********************此处需要三维图层服务*********************/

// 	QFutureWatcher<FxEarth::IFxEarthLayerService *> *pWatcher = new QFutureWatcher<FxEarth::IFxEarthLayerService *>();
// 	connect(pWatcher, SIGNAL(finished()), this, SLOT(futureFinishedSlot()));
// 	pWatcher->setFuture(QtConcurrent::run(std::bind(&CEarthLayerTreeWidget::getLayerServiceRefThread, this, iSceneId)));
}

HGxScene::CGxLayerManager* CEarthLayerTreeWidget::getLayerManager(const std::string& strSceneID)
{
	return HGxScene::CGxLayerManager::getOrCreateLayerManager(strSceneID);
}

void CEarthLayerTreeWidget::addServerLayerToSceneSlot( const QString &strName, const SLayerPropertyStruct& stLayerStruct, CLayerConfig::LayerType layerType )
{
	QTreeWidgetItem *pCurrentItem = currentItem();
	if (pCurrentItem == nullptr)
	{
		return;
	}
	if (pCurrentItem->type() != CLayerConfig::SecondCategory)
	{
		return;
	}
	CLayerConfig::LayerType typeLayerType = (CLayerConfig::LayerType)pCurrentItem->data(0, LayerTypeRole).toInt();
	if (typeLayerType != layerType)
	{
		CMessageBox::information(tr("The currently selected service type does not math"), m_pMainWindow);
		return;
	}
	addLayer(strName, stLayerStruct, layerType);
}

void CEarthLayerTreeWidget::itemDoubleClickedSlot( QTreeWidgetItem *item, int column )
{
	CEarthLayerItem *pEarthLayerItem = dynamic_cast<CEarthLayerItem *>(item);
	if (pEarthLayerItem == nullptr || pEarthLayerItem->earthLayer() == nullptr)
	{
		return;
	}

	CLayerConfig::LayerType iItemType = (CLayerConfig::LayerType)pEarthLayerItem->data(0, Qt::UserRole).toInt();

	HGxScene::CGxSceneBase* pEarthScene = HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID);
 
 	if (pEarthScene)
 	{
		HGxScene::CGxGeoContext *pGeoContext = dynamic_cast<HGxScene::CGxGeoContext*> (pEarthScene->getSceneContext());

		if (!pGeoContext)
		{
			return;
		}

 		switch (iItemType)
 		{
 		case CLayerConfig::LiveLayer:
 			{
				HGxScene::CGxLayerLive *pLiveLayer = static_cast<HGxScene::CGxLayerLive *>(pEarthLayerItem->earthLayer());

 				// 修改定位 l180
 				osg::BoundingSphere bound =  pLiveLayer->getGraphNode()->getBound();
 
 				if(0.0 > bound.radius())//如果包围球计算不正确，用高程定位
 				{
 					osg::Vec3d llh = pLiveLayer->getPosition();
					HGxAlgor::degreeLL2LLH(pGeoContext->getMapNode(), llh);

					HGxCommon::SGxViewPoint stViewPoint;
					stViewPoint.stPoint = HGxCommon::CGxPoint(llh);
					stViewPoint.vecAttitude = osg::Vec3d(-90, 0, 0);
					stViewPoint.dDistance = 2000;

 					pEarthScene->locate(stViewPoint, 2);
 				}
 				else //包围球有效，用包围球心计算真实海拔高度
 				{
 					osg::Vec3d llh;
					HGxAlgor::xyz2DegreeLLH(pGeoContext->getMapNode(), pLiveLayer->getGraphNode()->getBound().center(), llh);
 					osgEarth::Viewpoint point(pLiveLayer->getPosition().x(),pLiveLayer->getPosition().y() ,llh.z(), 0, -90, 2000);

					HGxCommon::SGxViewPoint stViewPoint;
					stViewPoint.stPoint = HGxCommon::CGxPoint(llh);
					stViewPoint.vecAttitude = osg::Vec3d(-90, 0, 0);
					stViewPoint.dDistance = 2000;

					pEarthScene->locate(stViewPoint, 2);
 				}
 
 			}
 			break;
 		case  CLayerConfig::ImageLayer:
 		case  CLayerConfig::ElevationLayer:
 		case  CLayerConfig::VectorLayer:
 			{
 				if (pEarthLayerItem->earthLayer() == nullptr)
 				{
 					return;
 				}

				osg::BoundingBoxd bbRange = pEarthLayerItem->earthLayer()->getLayerRegion();
				if (!bbRange.valid())
				{
					pEarthScene->locateHome(0);
					return;
				}

 				osg::Vec3d llh;
 				llh.x() = (bbRange._min.x() + bbRange._max.x()) / 2;
 				llh.y() = (bbRange._min.y() + bbRange._max.y()) / 2;
 				double dRange = 0;
 				if (bbRange._max.x() - bbRange._min.x() > 180 || bbRange._max.y() - bbRange._min.y() > 90)
				{
 					dRange = osg::WGS_84_RADIUS_EQUATOR * 3;
 				}
 				else
 				{
 					osg::Vec3d vecXyz1, vecXyz2;
					HGxAlgor::degreeLLH2XYZ(pGeoContext->getMapNode(), bbRange._min, vecXyz1);
					HGxAlgor::degreeLLH2XYZ(pGeoContext->getMapNode(), bbRange._max, vecXyz2);
					
					dRange = HGxAlgor::calculateDistance(vecXyz1, vecXyz2) * 1.5;
 				}
 				osgEarth::Viewpoint point(llh.x(), llh.y(), 0, 0, -90, dRange);

				HGxCommon::SGxViewPoint stViewPoint;
				stViewPoint.stPoint = HGxCommon::CGxPoint(llh);
				stViewPoint.vecAttitude = osg::Vec3d(-90, 0, 0);
				stViewPoint.dDistance = dRange;

				pEarthScene->locate(stViewPoint, 0);
 			}
 			break;
 		}
 	}
}

void CEarthLayerTreeWidget::CreatSeaLayerRootItem()
{
/************暂时不提供海图*****************/

//     if (m_pSeaLayerRoot != NULL)
//     {
//         /// 获取海图三维底层服务
//         return ;
//     }
// 
// 	if (m_iSceneId == -1)
// 	{
// 		return;
// 	}
// 
// 
// 
//  	// 获取场景服务
//  	FxCore::CFxServicePtr<FxScene::IFxSceneService> pSceneService = FreeXFrameworkInstance.getService<FxScene::IFxSceneService>();
//  	if(!pSceneService)
//  	{
//  		return;
//  	}
// 
// 
// 
//  	// 根据场景服务，获取二维/三维图层服务
//  	FxCore::CFxServicePtr<FxEarth::IFxSeaEarthLayerService> pEarthLayerService = pSceneService->getService<FxEarth::IFxSeaEarthLayerService>(m_iSceneId);
//  	if(!pEarthLayerService)
//  	{
//  		return;
//  	}
// 
//     m_pSeaLayerRoot = new CSeaEarthLayerRootItem(this);
//     m_pSeaLayerRoot->SetSceneId(m_iSceneId);
//     m_pSeaLayerRoot->InitTreeItem();

}

void CEarthLayerTreeWidget::pluginEvent( const ctkPluginEvent &event )
{
/************暂时不提供海图*****************/

//     QString strConstantname = "freex.seaearth.layer";
// 
// 	QString strName = event.getPlugin()->getSymbolicName();
// 
// 	if (ctkPluginEvent::STOPPED == event.getType())
// 	{
// 		if (m_pSeaLayerRoot == NULL)
// 		{
// 			return;
// 		}
// 
// 		//FxEarth::IFxSeaEarthLayerService *pEarthLayerService = nullptr;
// 		if(strName == strConstantname)
// 		{
// 			this->takeTopLevelItem(indexOfTopLevelItem(m_pSeaLayerRoot));
// 			delete m_pSeaLayerRoot;
// 			m_pSeaLayerRoot = NULL;
// 		}
// 
// 	}
// 	else if (ctkPluginEvent::STARTED == event.getType())
// 	{
// 
// 		if(strName == strConstantname)
// 		{
// 			if (m_pSeaLayerRoot == NULL && m_iSceneId != -1)
// 			{
// 				CreatSeaLayerRootItem();
// 
//                 if (m_pSeaLayerRoot)
//                 {
//                     addTopLevelItem(m_pSeaLayerRoot);
//                     this->expandItem(m_pSeaLayerRoot);
//                 }
//             }
//         }
//     }

}

void CEarthLayerTreeWidget::addSeaLayer( const QString &name, const QString &path,const QString &driver/*="s57"*/,bool fromlocal /*= true*/ )
{
/************暂时不提供海图*****************/

//     /// 添加海图图层
//     if (m_pSeaLayerRoot == NULL)
//     {
// 		/****************此处需要获取主窗口，先用NULL代替*******************/
// 
// 		CMessageBox::information(tr("3d chart plugin cannot add this data without loading")/*,  ICore::mainWindowManager()->sceneWindow()*/);
//         return;
//     }
// 	
//     QFileInfo fileInfo(path);
//     m_pSeaLayerRoot->addSeaLayer(fileInfo.baseName(),path,driver,fromlocal);

}

