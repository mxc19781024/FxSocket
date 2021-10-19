#include <QShortcut>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileInfo>
#include <QPainter>
#include <QDropEvent>
#include <QList>

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QtConcurrent>
#endif

#include "ctkPluginFrameworkFactory.h"
#include "ctkPluginFramework.h"
#include "ctkPluginException.h"
#include "ctkPluginContext.h"
#include "ctkPluginFrameworkLauncher.h"

//#include <service/IFxDataService.h>

//#include <service/IFxDataService.h>
//#include <service/layer/IServiceLayer.h>
//#include <map_layer/IFxMapLayer.h>
//#include <scene/IFxSceneService.h>
#include <scene_ui/ISceneUiService.h>
#include <scene_ui/ISceneInterface.h>
#include <map_layer_ui/IMapLayerUiService.h>
//#include <service_ui/IServiceUiInterface.h>
//#include <FreeXDataResUtilUi/CDataResourcesUtils.h>
//#include "FreeXDataResUtilUi/CLinkFileWindow.h"
//#include "FreeXDataResUtilUi/CServerDataWindow.h"

#include "public/GxDataResourcesUtils.h"
#include "public/GxLinkFileWindow.h"
#include "public/GxServerDataWindow.h"

#include "FXEditingDelegate/CEditingDelegate.h"
#include "FxToast/CToast.h"

#include <FxMessageBox/CMessageBox.h>
//#include <map/IMapManipulatorManager.h>
//#include <map/IFxMapScene.h>
//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXCommonUtils/EnvironmentVariableReader.h>
//#include <FreeXCommonUtils/PathRegistry.h>
//#include <FreeXFramework/LoadInfo.h>
#include <bdpCommon/GxStrUtil.h>

//#include <FreeXGeoUtil/PyramidBuilder.h>
#include <FxEditingDelegate/CEditingDelegate.h>
#include <layer_ui/CLayerManager.h>
//#include <systemset_ui/ISysSetUiInterface.h>

#include "CMapLayerItem.h"
#include "CCommonItem.h"
#include "CMapLayerTreeWidget.h"

#include "CMapPyramidCache.h"
#include "CPyramidProcessListener.h"

#include <FxToast/CToast.h>
#include <QScrollBar>
#include "FxLoadIndicator/CLoadIndicator.h"

#include <bdpDB/GxRegister.h>

#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/manipulator/GxBaseManipulator.h>
#include <bdpScene/manipulator/GxManipulatorManager.h>

#include <gdpScene/CommonGeo.h>
#include <gdpScene/layer/GxLayerManager.h>
#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerConfig.h>
#include <gdpScene/layer/GxLayerLive.h>
#include <gdpScene/layer/GxLayerConfigFactory.h>

#include <gdpScene/manipulator/GxMapManipulator.h>

#include <bdpCommon/GxMathUtil.h>
#include <gdpAlgor/GxCoordConverter.h>
#include <gdpAlgor/GxPyramidBuilder.h>
#include <gdpAlgor/GxMapGrid.h>

#include <osgEarth/Viewpoint>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>
#include "GxUiFrameworkServiceInterface.h"

/**********暂时不需要海图***************/
//#include "CSeaMapLayerItem.h"

#define CurrentTextRole (Qt::UserRole + 20)

#define MAX_VISIBLE_VAL FLT_MAX

#define MinPyramidLayerSize 100 // 金字塔缓存最小图层大小(单位：MB) 

CMapLayerTreeWidget::CMapLayerTreeWidget(ctkPluginContext *pContext, QWidget *parent)
	: QTreeWidget(parent)
	, m_pContextMenu(nullptr)
	, m_pCurDragItem(nullptr)
	, m_strSceneID("")
	, m_pPyramidProcess(nullptr)
	, m_strCachePath("")
	, m_pMainWindow(NULL)

	/**********暂时不需要海图***************/

	//,m_pSeaMapRootItem(nullptr)
{
	qRegisterMetaType<std::string>("std::string");

	setHeaderHidden(true);	// 隐藏头

	m_pPropertyMgr = new CMapLayerPropertyMgr(this);
	connect(m_pPropertyMgr, SIGNAL(layerNameChangedSignal(const QString &)), this, SLOT(layerNameChangedSlot(const QString &)));

	// add by w00025
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	(const_cast<QScrollBar *>(verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	// 树节点编辑
	CEditingDelegate *pDelegate = new CEditingDelegate(this);
	connect(pDelegate, SIGNAL(editingFinishedSignal(const QModelIndex &)), this, SLOT(editingFinishedSlot(const QModelIndex &)));
	setItemDelegate(pDelegate);

	connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChangedSlot()));
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(itemSelectionChangedSlot()));
	connect(this, SIGNAL(sceneCreatedSignal(std::string, bool)), this, SLOT(sceneCreatedSlot(std::string,bool)));
	connect(this, SIGNAL(clearSelectSignal()), this, SLOT(clearSelection()));
	connect(this, SIGNAL(initTreeWidgetSignal()), this, SLOT(initTreeSlot()));

	// 快捷键
	new QShortcut(QKeySequence("F2"), this, SLOT(renameShortCutSlot()), nullptr, Qt::WidgetWithChildrenShortcut);
	new QShortcut(QKeySequence("Delete"), this, SLOT(deleteShortCutSlot()), nullptr, Qt::WidgetWithChildrenShortcut);
	new QShortcut(QKeySequence("R"), this, SLOT(propertyShortCutSlot()), nullptr, Qt::WidgetWithChildrenShortcut);

	// 支持拖动
	setDragEnabled(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setDefaultDropAction(Qt::CopyAction);

	m_strCachePath = HGX_SYS_SETTINGS().strCachePath + "/MapLayerCache";

	/**********此处通过框架连接插件监听***************/

	//FreeXFrameworkInstance.pluginMgr().getFramePluginContext()->connectPluginListener(this,SLOT(pluginEvent(ctkPluginEvent)));


	HGxEventHelperInstance.subscribeTopic(this, "event/sceneCreated", HGxFWLauncherInstance.getPluginContext());
	HGxEventHelperInstance.subscribeTopic(this, "event/sceneSwitch", HGxFWLauncherInstance.getPluginContext());
}

void CMapLayerTreeWidget::clearMapChildLayers(QTreeWidgetItem* pRootItem)
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
				CMapLayerItem *pMapLayerItem = static_cast<CMapLayerItem *>(pCurrentItem);
				if (pMapLayerItem)
				{
					pLayerManager->deleteLayer(pMapLayerItem->mapLayer());
				}
			}
		}
	}
}

CMapLayerTreeWidget::~CMapLayerTreeWidget()
{
	//ICore::toolManager()->tryToSetCurrentPropertyWidget(nullptr);

 	// 清空图层树上的图层
 	clearMapChildLayers(m_pRootItem);
}

bool CMapLayerTreeWidget::generatePyramid(const QString &strName, const QString &strPath, const QString &strDriverType, CLayerConfig::LayerType layerType)
{
	// 判断是否是本地文件
	QFileInfo fileInfo(strPath);
	if (fileInfo.exists())
	{
		if (fileInfo.suffix() != "tif" && fileInfo.suffix() != "img")
		{
			return false;
		}
		qint64 iFileSize = fileInfo.size();
		double dfFileMbSize = iFileSize / 1024.0 / 1024.0;

 		if (dfFileMbSize >= MinPyramidLayerSize)
 		{
 			if (HGxAlgor::CFxPyramidBuilder::hasPyramidData(strPath.toUtf8().data()))
 			{
 				return false;
 			}

 			CMessageBox::ReturnButton ret = CMessageBox::question(tr("the file size is too large,pyramid cache is recommended"), m_pMainWindow);
 			if (ret == CMessageBox::Confirm)
 			{
 				connect(CMapPyramidCache::getInstance(this), SIGNAL(cancelCacheSignal()), this, SLOT(cancelProcessSlot()), Qt::UniqueConnection);
 				if (m_pPyramidProcess == nullptr)
 				{
 					m_pPyramidProcess = new CPyramidProcessListener(this);
 					connect(m_pPyramidProcess, SIGNAL(onPyramidProcessSignal(double)), this, SLOT(pyramidProcessSlot(double)));
 					connect(m_pPyramidProcess, SIGNAL(onPyramidProcessFinishedSignal(const QString &, const QString &, const QString &, int)), this, SLOT(onPyramidProcessFinishedSlot(const QString &, const QString &, const QString &, int)));
 				}
 				m_pPyramidProcess->initProcess(strName, strPath, strDriverType, layerType);
 				CMapPyramidCache::getInstance(this)->showCacheWgt();
 				HGxAlgor::CFxPyramidBuilder::build(strPath.toUtf8().data(), m_pPyramidProcess, true);
 				return true;
 			}
 		}
	}
	return false;
}

void CMapLayerTreeWidget::sendModifiedEvent(bool bModified)
{
	ctkDictionary message;
	message.insert("changeStatus", bModified);

	HGxEventHelperInstance.postEvent("event/pluginStatus", message);
}

HGxScene::CGxLayerManager* CMapLayerTreeWidget::getLayerManager(const std::string& strSceneID)
{
	return HGxScene::CGxLayerManager::getOrCreateLayerManager(strSceneID);
}

void CMapLayerTreeWidget::addMapLayerFromLocalFile(const QString &strFilePath)
{
	// 判断是否是文件夹
	QFileInfo fileInfo(strFilePath);
	QString strSuffix = fileInfo.suffix();
	if (fileInfo.isDir())
	{
		// scanDirAddLayers(strFilePath);
		return;
	}
	else if (fileInfo.isFile())
	{
		QStringList filterList;
		filterList << ".tif" << ".img" << ".shp" << ".kml" << ".000";

		if (filterList.contains("." + fileInfo.suffix()))
		{
			if (strSuffix == "tif" || strSuffix == "img")
			{
				// 生成金字塔缓存
				if (generatePyramid(fileInfo.fileName(), strFilePath, "gdal", CLayerConfig::ImageLayer))
				{
					return;
				}
				addImageLayer(strFilePath, fileInfo.fileName());
			}
			else if (strSuffix == "shp" || strSuffix == "kml")
			{
				addVectorLayer(strFilePath, fileInfo.fileName());
			}
			else if (strSuffix == "000")
			{

/**********************暂时不需要海图*****************************/

// 				/// 此处添加提示
// 
// 				/// 
// 				if (m_pSeaMapRootItem != NULL)
// 				{
// 					m_pSeaMapRootItem->addSeaLayer("",strFilePath);
// 				}
// 				else
// 				{
// 					CToast::information(tr("tips"), tr("2d chart plugin not load,please check!"),  ICore::mainWindowManager()->sceneWindow());
// 				}
			}
		}
	}
}

void CMapLayerTreeWidget::addImageLayer(const QString &strLayerPath, const QString &strLayerName)
{
	HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);
	
	if (NULL == pLayerManager)
	{
		return;
	}

	osg::ref_ptr<HGxScene::CGxLayerImageConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createImageLayerConfig(HGxScene::SLayerOption(strLayerName.toUtf8().data(), HGxScene::DriverType::DRIVER_GDAL, strLayerPath.toUtf8().data()));

	if (!pLayerCfg.valid())
	{
		return;
	}

 	pLayerCfg->cacheConfig().cachePath() = 	m_strCachePath;

	HGxScene::CGxGlobalLayer* pLayer = pLayerManager->createLayer(*pLayerCfg.get());
 
 	if (pLayer == nullptr)
 	{
 		return;
 	}

	pLayerManager->addLayer(pLayer);

	// 添加界面树节点
	CMapLayerItem *pLayerItem = new CMapLayerItem(CLayerConfig::LayerItem, pLayer);
	pLayerItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled);
	pLayerItem->setText(0, strLayerName);
	// 新添加的节点在最上面
	if (m_pRootItem != nullptr)
	{
		m_pRootItem->insertChild(0, pLayerItem);
		pLayerItem->updateParent(pLayerItem, true);
		m_pRootItem->setExpanded(true);
		setCurrentItem(pLayerItem);
	}
	sendModifiedEvent(true);
}

void CMapLayerTreeWidget::addLocalFileLayer(const QString &strName, const QString &strFilePath)
{
	// 判断是否是文件夹
	QFileInfo fileInfo(strFilePath);
	QString strSuffix = fileInfo.suffix();
	if (fileInfo.isDir())
	{
		// scanDirAddLayers(strFilePath);
		return;
	}
	else if (fileInfo.isFile())
	{
		QStringList filterList;
		filterList << ".tif" << ".img" << ".shp" << ".kml" << ".000";

		if (filterList.contains("." + fileInfo.suffix()))
		{
			if (strSuffix == "tif" || strSuffix == "img")
			{
				addImageLayer(strFilePath, strName);
			}
			else if (strSuffix == "shp" || strSuffix == "kml")
			{
				addVectorLayer(strFilePath, strName);
			}
			else if (strSuffix == "000")
			{

/**********************暂时不需要海图*****************************/

// 				/// 此处添加提示
// 
// 				/// 
// 				if (m_pSeaMapRootItem != NULL)
// 				{
// 					m_pSeaMapRootItem->addSeaLayer("",strFilePath);
// 				}
// 				else
// 				{
// 					CToast::information(tr("tips"), tr("2d chart plugin not load,please check!"), ICore::mainWindowManager()->sceneWindow());
// 				}
 			}
		}
	}
}

// FxMap::IFxMapLayerService * CMapLayerTreeWidget::loadLayerInThread( int iSceneId, const FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess )
// {
// 	ICore::mainWindowManager()->globalMutex()->lock();
// 	CLoadIndicator::addUseCount(ICore::mainWindowManager()->mainWindow());
// 
// 	ISceneUiService *pSceneService = getService<ISceneUiService>();
// 	if (pSceneService)
// 	{
// 		pSceneService->pauseRender(true);
// 	}
// 
// 	FxMap::IFxMapLayerService *pLayerService = mapLayerService(iSceneId);
// 	if(pLayerService)
// 	{
// 		pLayerService->load(pEle, pProcess);
// 	}
// 	
// 
// 	/// 二维海图底层插件加载数据begin
// 	FxMap::IFxSeaMpLayerService *pSeaMapItem = seaMapLayerService(iSceneId);
// 	if (pSeaMapItem != NULL)
// 	{
// 		pSeaMapItem->clearLayers();
// 		pSeaMapItem->load(pEle,pProcess);
// 	}
// 	/// 二维海图底层插件加载数据end
// 
// 	ICore::mainWindowManager()->globalMutex()->unlock();
// 
// 	return pLayerService;
// }

void CMapLayerTreeWidget::loadLayerThreadFinishedSlot()
{
	/**************此处需要获取主窗口******************/

	//ICore::mainWindowManager()->mainWindow()->setUpdatesEnabled(true);
	
	init();
	//CLoadIndicator::removeUseCount();

	if (HGxScene::CGxControllerMgr::getInstance())
	{
		HGxScene::CGxControllerMgr::getInstance()->pauseSimulation(m_strSceneID, false);
	}
}

// bool CMapLayerTreeWidget::load(const FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess)
// {
// 	const FxCommonUtils::TiXmlElement *pRoot = pEle->FirstChildElement("maplayer_ui");
// 	if (pRoot == nullptr)
// 	{
// 		return false;
// 	}
// 	ISceneUiService *pSceneService = getService<ISceneUiService>();
// 
// 	for (const FxCommonUtils::TiXmlElement *pSceneEle = pRoot->FirstChildElement(); pSceneEle != nullptr; pSceneEle = pSceneEle->NextSiblingElement())
// 	{
// 		QString strSceneName = QString::fromUtf8(pSceneEle->Attribute("SceneName"));
// 		
// 		if (pSceneService == nullptr)
// 		{
// 			return false;
// 		}
// 		int iSceneId = -1;
// 		ISceneInterface *pInterface = pSceneService->scene(strSceneName);
// 		if (pInterface)
// 		{
// 			iSceneId = pSceneService->scene(strSceneName)->sceneId();
// 		}
// 		
// 		if (iSceneId == -1)
// 		{
// 			continue;
// 		}
//         FxMap::IFxMapLayerService *pLayerService = mapLayerService(iSceneId);
//         if (pLayerService == nullptr)
//         {
//             continue;
//         }
// 
// // 		// 调用底层，初始化场景标绘
// // 		pLayerService->load(pSceneEle, pProcess);
// // 		
// // 		// 如果是当前场景，则初始化界面
// // 		if (iSceneId == m_iSceneId)
// // 		{
// // 			init();
// // 		}
// 
// 		ICore::mainWindowManager()->mainWindow()->setUpdatesEnabled(false);
// 		QFutureWatcher<FxMap::IFxMapLayerService*> *pWatcher = new QFutureWatcher<FxMap::IFxMapLayerService*>();
// 		//pWatcher->setProperty("sceneId", iSceneId);// 将场景id存入属性中
// 		connect(pWatcher, SIGNAL(finished()), this, SLOT(loadLayerThreadFinishedSlot()));
// 		pWatcher->setFuture(QtConcurrent::run(std::bind(&CMapLayerTreeWidget::loadLayerInThread, this, iSceneId, pSceneEle, pProcess)));
// 
// 	}
// 
// 	return true;
// }
// 
// bool CMapLayerTreeWidget::save(FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess)
// {
// 	FxCommonUtils::TiXmlElement *pRoot = new FxCommonUtils::TiXmlElement("maplayer_ui");
// 	pEle->LinkEndChild(pRoot);
// 
// 	// 获取所有三维场景
// 	ISceneUiService *pSceneService = getService<ISceneUiService>();
// 	if (pSceneService == nullptr)
// 	{
// 		return false;
// 	}
// 	QList<ISceneInterface *> listMapScenes = pSceneService->mapScenes();
// 
// 	// 保存每个三维场景中的图层
// 	foreach (auto *pScene, listMapScenes)
// 	{
// 		FxCommonUtils::TiXmlElement *pSceneEle = new FxCommonUtils::TiXmlElement("scene");
// 		// modified by w00025 工作空间是utf-8编码
// 		pSceneEle->SetAttribute("SceneName", pScene->sceneName().toUtf8().data());
// 		pRoot->LinkEndChild(pSceneEle);
// 
// 		FxMap::IFxMapLayerService *pLayerService = mapLayerService(pScene->sceneId());
// 		if (pLayerService != nullptr)
// 		{
// 			pLayerService->save(pSceneEle, pProcess);
// 		}
// 
// 		/// 海图保存工作空间begin
// 		FxMap::IFxSeaMpLayerService *pSeaMapItem = seaMapLayerService(pScene->sceneId());
// 		if (pSeaMapItem != NULL)
// 		{
// 			pSeaMapItem->save(pSceneEle,pProcess);
// 		}
// 		/// 海图保存工作空间end
// 	}
// 	return true;
// }

void CMapLayerTreeWidget::moveLayerToTop()
{
	QAction act(this);
	act.setData(CMapLayerTreeWidget::MOVE_TO_TOP);
	contextMenuActionSlot(&act);
}

void CMapLayerTreeWidget::moveLayerToBottom()
{
	QAction act(this);
	act.setData(CMapLayerTreeWidget::MOVE_TO_BOTTOM);
	contextMenuActionSlot(&act);
}

void CMapLayerTreeWidget::moveLayerUp()
{
	QAction act(this);
	act.setData(CMapLayerTreeWidget::MOVE_UP);
	contextMenuActionSlot(&act);
}

void CMapLayerTreeWidget::moveLayerDown()
{
	QAction act(this);
	act.setData(CMapLayerTreeWidget::MOVE_DOWN);
	contextMenuActionSlot(&act);
}

void CMapLayerTreeWidget::deleteLayer()
{
	QAction act(this);
	act.setData(CMapLayerTreeWidget::DELETE_LAYER);
	contextMenuActionSlot(&act);
}

void CMapLayerTreeWidget::initWhenPluginStart()
{
	init();
}

QWidget* CMapLayerTreeWidget::getMapLayerPropertyMgrWgt()
{
	return m_pPropertyMgr;
}

void CMapLayerTreeWidget::addVectorLayer(const QString &strLayerPath, const QString &strLayerName)
{
	HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

	if (NULL == pLayerManager)
	{
		return;
	}

	osg::ref_ptr<HGxScene::CGxLayerModelConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createModelLayerConfig(HGxScene::SLayerOption(strLayerName.toUtf8().data(), HGxScene::DriverType::DRIVER_OGR, strLayerPath.toUtf8().data()));

	if (!pLayerCfg.valid())
	{
		return;
	}

	HGxScene::CGxGlobalLayer* pLayer = pLayerManager->createLayer(*pLayerCfg);

	if (pLayer == nullptr)
	{
		return;
	}

	pLayerManager->addLayer(pLayer);

	// 添加界面树节点
	CMapLayerItem *pLayerItem = new CMapLayerItem(CLayerConfig::LayerItem, pLayer);
	pLayerItem->setText(0, strLayerName);
	pLayerItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled);
	// 新添加的节点在最上面
	if (m_pRootItem != nullptr)
	{
		m_pRootItem->insertChild(0, pLayerItem);
		pLayerItem->updateParent(pLayerItem, true);
		m_pRootItem->setExpanded(true);
		setCurrentItem(pLayerItem);
	}
	sendModifiedEvent(true);
}

void CMapLayerTreeWidget::addMapLayerFromService(const QString &url, const QString &strLayerName, const QString &driverType, CLayerConfig::LayerType layerType, const QString &layerFormat)
{
	HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

	if (pLayerManager == nullptr)
	{
		return;
	}

 	HGxScene::CGxGlobalLayer *pLayer = nullptr;
 
 	switch (layerType)
 	{
 	case CLayerConfig::ImageLayer:
 		{
			osg::ref_ptr<HGxScene::CGxLayerImageConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createImageLayerConfig(HGxScene::SOGCLayerOption(strLayerName.toUtf8().data(), driverType.toUtf8().data(), url.toUtf8().data(), strLayerName.toUtf8().data()));
 			pLayerCfg->sourceConfig().layerFormat() = layerFormat.toUtf8().data();
 			if(!pLayerCfg.valid())
 			{
				break;
 			}

			pLayerCfg->cacheConfig().cachePath() = m_strCachePath;

 			pLayer = pLayerManager->createLayer(*pLayerCfg.get());
 			if (pLayer == nullptr)
 			{
				break;
 			}
			pLayerManager->addLayer(pLayer);
 		}
 		break;
 	case  CLayerConfig::ElevationLayer:
 		{
 			CMessageBox::information(tr("2d scene not support elevation data!"), m_pMainWindow);
			break;
 		}
 		break;
 	case CLayerConfig::VectorLayer:
 		{
 			osg::ref_ptr<HGxScene::CGxLayerModelConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createModelLayerConfig(HGxScene::SOGCLayerOption(strLayerName.toUtf8().data(), driverType.toUtf8().data(), url.toUtf8().data(), strLayerName.toUtf8().data()));
 			
			if (!pLayerCfg.valid())
			{
				break;
			}
			
			pLayer = pLayerManager->createLayer(*pLayerCfg.get());
 			
			if (pLayer == nullptr)
 			{
				break;
 			}
			pLayerManager->addLayer(pLayer);
 		}
 		break;
 	default:
 		return;
 		break;
 	}

	// 添加界面树节点
	CMapLayerItem *pLayerItem = new CMapLayerItem(CLayerConfig::LayerItem, pLayer);
	pLayerItem->setText(0, strLayerName);
	// 新添加的节点在最上面
	m_pRootItem->insertChild(0, pLayerItem);
	pLayerItem->updateParent(pLayerItem, true);
	m_pRootItem->setExpanded(true);
	setCurrentItem(pLayerItem);

	sendModifiedEvent(true);
}

void CMapLayerTreeWidget::addMapLayerFromService( const QString &strName, const SLayerPropertyStruct& stLayerStruct, CLayerConfig::LayerType layerType )
{
	HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

	if (pLayerManager == nullptr)
	{
		return;
	}

	HGxScene::CGxGlobalLayer *pLayer = nullptr;

	switch (layerType)
	{
	case CLayerConfig::ImageLayer:
	{
		osg::ref_ptr<HGxScene::CGxLayerImageConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createImageLayerConfig(HGxScene::SOGCLayerOption(stLayerStruct.strLayerName.toUtf8().data(), stLayerStruct.strDriveName.toUtf8().data(), stLayerStruct.strServerWebsite.toUtf8().data(), stLayerStruct.strLayerName.toUtf8().data()));
		if (!pLayerCfg.valid())
		{
			break;
		}

		pLayerCfg->cacheConfig().cachePath() = m_strCachePath;
		pLayer = pLayerManager->createLayer(*pLayerCfg.get());
		if (pLayer == nullptr)
		{
			break;
		}
		pLayerManager->addLayer(pLayer);
	}
	break;
	case CLayerConfig::VectorLayer:
	{
		osg::ref_ptr<HGxScene::CGxLayerModelConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createModelLayerConfig(HGxScene::SOGCLayerOption(stLayerStruct.strLayerName.toUtf8().data(), stLayerStruct.strDriveName.toUtf8().data(), stLayerStruct.strServerWebsite.toUtf8().data(), stLayerStruct.strLayerName.toUtf8().data()));
		
		if (!pLayerCfg.valid())
		{
			break;
		}
		
		pLayer = pLayerManager->createLayer(*pLayerCfg.get());
		if (pLayer == nullptr)
		{
			break;
		}
		pLayerManager->addLayer(pLayer);
	}
	break;
	case CLayerConfig::ElevationLayer:
	{
		CMessageBox::information(tr("2d scene not support elevation data!"), m_pMainWindow);
	}
	break;
	default:
		break;
	}

	// 添加界面树节点
	CMapLayerItem *pLayerItem = new CMapLayerItem(CLayerConfig::LayerItem, pLayer);
	pLayerItem->setText(0, strName);
	// 新添加的节点在最上面
	m_pRootItem->insertChild(0, pLayerItem);
	pLayerItem->updateParent(pLayerItem, true);
	m_pRootItem->setExpanded(true);
	setCurrentItem(pLayerItem);

	sendModifiedEvent(true);
}

void CMapLayerTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QTreeWidgetItem *pCurrentItem = itemAt(event->pos());
	if (pCurrentItem == nullptr)
	{
		return;
	}

	// 初始化右键菜单
	if (m_pContextMenu == nullptr)
	{
		m_pContextMenu = new QMenu(this);
		m_pContextMenu->setWindowFlags(m_pContextMenu->windowFlags() | Qt::FramelessWindowHint);
		m_pContextMenu->setAttribute(Qt::WA_TranslucentBackground);
		connect(m_pContextMenu, SIGNAL(triggered(QAction *)), this, SLOT(contextMenuActionSlot(QAction *)));
		m_pContextMenu->hide();
	}
	m_pContextMenu->clear();

	/// 弹出海图菜单 begin
	if (pCurrentItem->type() == CLayerConfig::SeaLayerCategory)
	{
/****************暂时不提供海图********************/

// 		if (m_pSeaMapRootItem != NULL)
// 		{
// 			QList<QAction*> lsAct = m_pSeaMapRootItem->GetAction(pCurrentItem);
// 			m_pContextMenu->addActions(lsAct);
// 			m_pContextMenu->popup(event->globalPos());
// 		}
		return;
	}
	/// 弹出海图菜单 end

	CLayerConfig::TreeItemType iItemType = (CLayerConfig::TreeItemType)pCurrentItem->type();
	switch (iItemType)
	{
	case CLayerConfig::TopCategory:
		{
			QAction *pAction = new QAction(tr("add data"), m_pContextMenu);
			QIcon qicon(":/images_map_layer/Resources/images_map_layer/tianjia_normal.png");
			
			pAction->setIcon(qicon);
			pAction->setData(CMapLayerTreeWidget::ADD_DATA);
			m_pContextMenu->addAction(pAction);
			m_pContextMenu->popup(event->globalPos());
		}
		break;
		// 二维图层没有分类
	case CLayerConfig::SecondCategory:
		break;
	case CLayerConfig::LayerItem:
		{
			QTreeWidgetItem *pParentItem = pCurrentItem->parent();
			if (pParentItem == nullptr)
			{
				return;
			}
			int iChildCount = pParentItem->childCount();
			QAction *pAction1 = new QAction(tr("zoom to layer range"), m_pContextMenu);
			pAction1->setData(CMapLayerTreeWidget::ZOOM_TO_LAYER);
			QAction *pAction2 = new QAction(tr("up"), m_pContextMenu);
			pAction2->setData(CMapLayerTreeWidget::MOVE_UP);
			QAction *pAction3 = new QAction(tr("down"), m_pContextMenu);
			pAction3->setData(CMapLayerTreeWidget::MOVE_DOWN);
			QAction *pAction4 = new QAction(tr("to top"), m_pContextMenu);
			pAction4->setData(CMapLayerTreeWidget::MOVE_TO_TOP);
			QAction *pAction5 = new QAction(tr("to bottom"), m_pContextMenu);
			pAction5->setData(CMapLayerTreeWidget::MOVE_TO_BOTTOM);
			QAction *pAction6 = new QAction(tr("delete"), m_pContextMenu);
			pAction6->setData(CMapLayerTreeWidget::DELETE_LAYER);
			pAction6->setShortcut(QKeySequence("Delete"));
			QAction *pAction7 = new QAction(tr("rename"), m_pContextMenu);
			pAction7->setData(CMapLayerTreeWidget::RENAME);
			pAction7->setShortcut(QKeySequence("F2"));
			QAction *pAction8 = new QAction(tr("property"), m_pContextMenu);
			pAction8->setData(CMapLayerTreeWidget::PROPERTIES);
			pAction8->setShortcut(QKeySequence("R"));
			m_pContextMenu->addAction(pAction1);
			m_pContextMenu->addAction(pAction2);
			m_pContextMenu->addAction(pAction3);
			m_pContextMenu->addAction(pAction4);
			m_pContextMenu->addAction(pAction5);
			m_pContextMenu->addAction(pAction6);
			m_pContextMenu->addAction(pAction7);
			m_pContextMenu->addAction(pAction8);
			m_pContextMenu->popup(event->globalPos());
		}
		break;
	default:
		break;
	}
}

void CMapLayerTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
	//不支持多个拖拽选中
	if (selectedItems().size() > 1)
	{
		event->setDropAction(Qt::IgnoreAction);
		return;
	}

	m_pCurDragItem = currentItem();
	if (!m_pCurDragItem->parent())
	{
		event->setDropAction(Qt::IgnoreAction);
		return;
	}
	QTreeWidget::dragEnterEvent(event);
}

void CMapLayerTreeWidget::dropEvent(QDropEvent *event)
{
	QModelIndex droppedIndex = indexAt(event->pos());

	HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

	if (pLayerManager == nullptr)
	{
		return;
	}

	if (m_pCurDragItem != nullptr)
	{
		QTreeWidgetItem *pDragParentItem = m_pCurDragItem->parent();
		if (pDragParentItem != nullptr)
		{
			if (itemFromIndex(droppedIndex.parent()) != pDragParentItem)
			{
				//如果index不存在 拖拽到最后
				droppedIndex = indexFromItem(pDragParentItem->child(pDragParentItem->childCount() - 1));
			}
			pDragParentItem->removeChild(m_pCurDragItem);
			pDragParentItem->insertChild(droppedIndex.row(), m_pCurDragItem);

 			CMapLayerItem *pMapLayerItem = dynamic_cast<CMapLayerItem *>(m_pCurDragItem);
 			HGxScene::CGxGlobalLayer* pCurrentLayer = pMapLayerItem->mapLayer();
 			if (pCurrentLayer == nullptr)
 			{
 				return;
 			}
 			HGxScene::CGxLayersSet mapLayers;
 			pLayerManager->getLayers(mapLayers);

			pLayerManager->moveLayer(pCurrentLayer, m_pRootItem->childCount() - m_pRootItem->indexOfChild(pMapLayerItem) - 1);
		}
	}
}

void CMapLayerTreeWidget::handleEvent(const ctkEvent &event)
{
	if (event.getTopic() == "event/sceneCreated")
	{
		std::string strSceneType = event.getProperty("SceneType").toString().toStdString();
		if (strSceneType == GX_MAP_SCENE_TYPE)
		{
			// 记录当前场景id
			m_strSceneID = event.getProperty("SceneID").toString().toStdString();

			bool bCreateFromFile =false/* (event.getProperty("Condition").toString() != "CreatedFromUser")*/;

// 			//z00011 增加任务数量
// 			FxCore::CLoadInfo::Instance()->addCount();
// 			//end 

			emit sceneCreatedSignal(m_strSceneID, bCreateFromFile);
		}
	}
	else if (event.getTopic() == "event/sceneSwitch")
	{
		std::string strSceneType = event.getProperty("SceneType").toString().toStdString();
		if (strSceneType == GX_MAP_SCENE_TYPE)
		{
			// 记录当前场景id
			m_strSceneID = event.getProperty("SceneID").toString().toStdString();

			emit initTreeWidgetSignal();
		}
		else
		{
			emit clearSelectSignal();
		}
	}
}

void CMapLayerTreeWidget::initLayerTypes()
{
	m_pRootItem = new CCommonItem(CLayerConfig::TopCategory);
	//m_pRootItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	m_pRootItem->setText(0, tr("map layer"));
	addTopLevelItem(m_pRootItem);
	m_pRootItem->setIcon(0, QIcon(":/images_earth_layer/Resources/images_earth_layer/root.png"));
}

void CMapLayerTreeWidget::scanDirAddLayers(const QString &filePath)
{
	QDir dir(filePath);
	if (!dir.exists())
	{
		return;
	}
	QStringList filterList;
	filterList << "*.tif" << "*.img" << "*.shp" << "*.kml";
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	dir.setNameFilters(filterList);

	QFileInfoList fileInfoList = dir.entryInfoList();
	for (int i = 0; i < fileInfoList.size(); ++i)
	{
		if (fileInfoList[i].suffix() == "tif" || fileInfoList[i].suffix() == "img")
		{
			addImageLayer(fileInfoList[i].absoluteFilePath(), fileInfoList[i].baseName());
		}
		else if (fileInfoList[i].suffix() == "shp" || fileInfoList[i].suffix() == "kml")
		{
			addVectorLayer(fileInfoList[i].absoluteFilePath(), fileInfoList[i].baseName());
		}
	}
}

void CMapLayerTreeWidget::init()
{
	HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

	if (pLayerManager == nullptr)
	{
		return;
	}

	clear();

/****************暂时不提供海图********************/
//	m_pSeaMapRootItem = NULL;

	m_pRootItem = new CCommonItem(CLayerConfig::TopCategory);
	m_pRootItem->setText(0, tr("map layer"));
	addTopLevelItem(m_pRootItem);
	m_pRootItem->setIcon(0, QIcon(":/images_map_layer/Resources/images_map_layer/root.png"));

	HGxScene::CGxLayersSet mapLayers;
	pLayerManager->getAllLayers(mapLayers);
	for (int i = 0; i < mapLayers.size(); ++i)
	{
		HGxScene::CGxGlobalLayer* pMapLayer = mapLayers[i];
		CMapLayerItem *pLayerItem = new CMapLayerItem(CLayerConfig::LayerItem, mapLayers[i]);
		pLayerItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled);
		pLayerItem->setText(0, QString::fromUtf8(pMapLayer->getName().c_str()));
		m_pRootItem->insertChild(0, pLayerItem);
		if (!pMapLayer->getVisible())
		{
			pLayerItem->setCheckState(0, Qt::Unchecked);
		}
		if (i == mapLayers.size() - 1)
		{
			pLayerItem->updateParent(pLayerItem, true);
		}
	}

/****************暂时不提供海图********************/

// 	/// 创建二维海图根节点初始化二维海图begin
// 	FxMap::IFxSeaMpLayerService * pSeaMapLayeService = seaMapLayerService(m_iSceneId);
// 
// 	if (pSeaMapLayeService != NULL)
// 	{
// 		CreatSeaMapRootItem();
// 		this->addTopLevelItem(m_pSeaMapRootItem);
// 	}

	/// 创建二维海图根节点初始化二维海图end
	expandAll();
}


void CMapLayerTreeWidget::layerNameChangedSlot(const QString &strLayerName)
{
	sendModifiedEvent(true);

	CMapLayerItem *pMapLayerItem = static_cast<CMapLayerItem *>(currentItem());
	if (pMapLayerItem)
	{
		pMapLayerItem->setText(0, strLayerName);
	}
}

void CMapLayerTreeWidget::contextMenuActionSlot(QAction *pAction)
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

	CMapLayerTreeWidget::MapLayerMenuAction menuAction = (CMapLayerTreeWidget::MapLayerMenuAction)(pAction->data().toInt());
	switch (menuAction)
	{
		// 添加数据
	case CMapLayerTreeWidget::ADD_DATA:
		{
			// 设置文件选择类型
			QStringList filters;
			filters << "shp" << "img" << "kml" << "tif";

			CGxDataResourcesUtils::getServerDataWindow2D(m_pMainWindow)->setFileFilter(filters);
			CGxDataResourcesUtils::getServerDataWindow2D(m_pMainWindow)->show();

			bool b = connect(CGxDataResourcesUtils::getServerDataWindow2D(m_pMainWindow), SIGNAL(addLayerToSceneSignal(const QString &, const QString &, const QString &, CLayerConfig::LayerType)), this, SLOT(addLayerToSceneSlot(const QString &, const QString &, const QString &, CLayerConfig::LayerType)), Qt::UniqueConnection);
			b = connect(CGxDataResourcesUtils::getServerDataWindow2D(m_pMainWindow), SIGNAL(addServerLayerToSceneSignal(const QString &, const SLayerPropertyStruct &, CLayerConfig::LayerType)), this, SLOT(addServerLayerToSceneSlot(const QString&, const SLayerPropertyStruct &, CLayerConfig::LayerType)), Qt::UniqueConnection);
			int a = 0;
		}
		break;
		// 缩放至图层范围
	case CMapLayerTreeWidget::ZOOM_TO_LAYER:
		{
			CMapLayerItem *pMapLayerItem = static_cast<CMapLayerItem *>(pCurrentItem);
			if (pMapLayerItem == nullptr)
			{
				return;
			}

			HGxScene::CGxSceneBase* pCurrentScene = HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID);

			if (NULL == pCurrentScene)
			{
				return;
			}

			HGxScene::CGxMapManipulator* pMapMani = static_cast<HGxScene::CGxMapManipulator*>(pCurrentScene->getManipulatorSystem()->getCurrentManipulator());

			if (pMapMani == nullptr)
			{
				return;
			}
			osg::BoundingBoxd layerRegin = pMapLayerItem->mapLayer()->getLayerRegion();

			if (!layerRegin.valid())
			{
				pCurrentScene->locateHome(0);
				return;
			}

			pMapMani->setViewPointByBounding(layerRegin._min.x(), layerRegin._max.x(), layerRegin._min.y(), layerRegin._max.y());

			sendModifiedEvent(true);
		}
		break;
		// 上移
	case CMapLayerTreeWidget::MOVE_UP:
		{
			HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

			if (pLayerManager == nullptr)
			{
				return;
			}
 
 			int iRow = currentIndex().row();
 
 			// 获取当前图层
 			CMapLayerItem *pCurrentMapLayerItem = dynamic_cast<CMapLayerItem *>(currentItem());
 			if (pCurrentMapLayerItem == nullptr)
 			{
 				return;
 			}
 			HGxScene::CGxGlobalLayer* pCurrentLayer = pCurrentMapLayerItem->mapLayer();
 			if (pCurrentLayer == nullptr)
 			{
 				return;
 			}
 
 			if((m_pRootItem->childCount() - m_pRootItem->indexOfChild(pCurrentItem)) >= pLayerManager->getLayersCount())
 			{
 				// "已到最上层！"
 				return;
 			}

			pLayerManager->moveLayer(pCurrentLayer, m_pRootItem->childCount() - m_pRootItem->indexOfChild(pCurrentItem));

			// 界面改动
			QTreeWidgetItem *parentItem = pCurrentItem->parent();
			int iIndex = parentItem->indexOfChild(pCurrentItem);
			QTreeWidgetItem *pChildItem = parentItem->takeChild(iIndex);
			parentItem->insertChild(iIndex - 1, pChildItem);
			parentItem->setExpanded(true);
			setCurrentItem(pChildItem);

			sendModifiedEvent(true);
		}
		break;
		// 下移
	case CMapLayerTreeWidget::MOVE_DOWN:
		{
			HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

			if (pLayerManager == nullptr)
			{
				return;
			}
 
 			// 获取当前图层
 			CMapLayerItem *pCurrentMapLayerItem = dynamic_cast<CMapLayerItem *>(currentItem());
 			if (pCurrentMapLayerItem == nullptr)
 			{
 				return;
 			}

 			HGxScene::CGxGlobalLayer* pCurrentLayer = pCurrentMapLayerItem->mapLayer();

 			if (pCurrentLayer == nullptr)
 			{
 				return;
 			}
 
 			int iTotal = m_pRootItem->childCount();
 			int iCurIndex = m_pRootItem->indexOfChild(pCurrentItem);
 			if (m_pRootItem->childCount() - 1 - m_pRootItem->indexOfChild(pCurrentItem) <= 0)
 			{
 				// "已到最底层！" ;
 				return;
 			}

 			pLayerManager->moveLayer(pCurrentLayer, m_pRootItem->childCount() - 2 - m_pRootItem->indexOfChild(pCurrentItem));

			QTreeWidgetItem *parentItem = pCurrentItem->parent();
			int iCount = parentItem->childCount();
			int iRow = currentIndex().row();
			if (iRow < iCount - 1)
			{
				QTreeWidgetItem *parentItem = pCurrentItem->parent();
				int iIndex = parentItem->indexOfChild(pCurrentItem);
				QTreeWidgetItem *pChildItem = parentItem->takeChild(iIndex);
				parentItem->insertChild(iIndex + 1, pChildItem);
				parentItem->setExpanded(true);
				setCurrentItem(pChildItem);
			}

			sendModifiedEvent(true);
		}
		break;
		// 置顶
	case CMapLayerTreeWidget::MOVE_TO_TOP:
		{
			HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

			if (pLayerManager == nullptr)
			{
				return;
			}
 
 			// 获取当前图层
 			CMapLayerItem *pCurrentMapLayerItem = dynamic_cast<CMapLayerItem *>(currentItem());
 			if (pCurrentMapLayerItem == nullptr)
 			{
 				return;
 			}
			HGxScene::CGxGlobalLayer* pCurrentLayer = pCurrentMapLayerItem->mapLayer();
 
 			if((m_pRootItem->childCount() - m_pRootItem->indexOfChild(pCurrentItem)) >= pLayerManager->getLayersCount())
 			{
 				// "已到最上层！";
 				return;
 			}

			pLayerManager->moveLayer(pCurrentLayer, m_pRootItem->childCount() - 1);

			int iRow = currentIndex().row();
			if (iRow > 0)
			{
				QTreeWidgetItem *parentItem = pCurrentItem->parent();
				int iIndex = parentItem->indexOfChild(pCurrentItem);
				QTreeWidgetItem *pChildItem = parentItem->takeChild(iIndex);
				parentItem->insertChild(0, pChildItem);
				parentItem->setExpanded(true);
				setCurrentItem(pChildItem);
			}

			sendModifiedEvent(true);
		}
		break;
		// 置底
	case CMapLayerTreeWidget::MOVE_TO_BOTTOM:
		{
			HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

			if (pLayerManager == nullptr)
			{
				return;
			}
 
 			// 获取当前图层
 			CMapLayerItem *pCurrentMapLayerItem = dynamic_cast<CMapLayerItem *>(currentItem());
 			if (pCurrentMapLayerItem == nullptr)
 			{
 				return;
 			}
 			HGxScene::CGxGlobalLayer* pCurrentLayer = pCurrentMapLayerItem->mapLayer();
 
 			if (pCurrentLayer == nullptr)
 			{
 				return;
 			}
 
 			if (m_pRootItem->childCount() - 1 - m_pRootItem->indexOfChild(pCurrentItem) <= 0)
 			{
 				// "已到最底层！"
 				return;
 			}

			pLayerManager->moveLayer(pCurrentLayer, 0);

			QTreeWidgetItem *parentItem = pCurrentItem->parent();
			int iCount = parentItem->childCount();
			int iRow = currentIndex().row();
			if (iRow < iCount - 1)
			{
				QTreeWidgetItem *parentItem = pCurrentItem->parent();
				int iIndex = parentItem->indexOfChild(pCurrentItem);
				QTreeWidgetItem *pChildItem = parentItem->takeChild(iIndex);
				parentItem->insertChild(iCount - 1, pChildItem);
				parentItem->setExpanded(true);
				setCurrentItem(pChildItem);
			}

			sendModifiedEvent(true);
		}
		break;
		// 删除
	case CMapLayerTreeWidget::DELETE_LAYER:
		{
			deleteShortCutSlot();
		}
		break;
		// 重命名
	case CMapLayerTreeWidget::RENAME:
		{
			pCurrentItem->setData(0, CurrentTextRole, pCurrentItem->text(0));
			pCurrentItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled);
			editItem(pCurrentItem, 0);
		}
		break;
		// 属性
	case CMapLayerTreeWidget::PROPERTIES:
		{
			CMapLayerItem *pLayerItem = static_cast<CMapLayerItem *>(pCurrentItem);
			if (pLayerItem == nullptr)
			{
				return;
			}

			m_pPropertyMgr->showLayerProperty(pLayerItem->mapLayer());
		}
		break;
	default:
		break;
	}
}

void CMapLayerTreeWidget::editingFinishedSlot(const QModelIndex &index)
{
	sendModifiedEvent(true);

	QTreeWidgetItem *pEditingItem = itemFromIndex(index);

	/// 判断是否海图 begin add by yq00012
	if (pEditingItem != NULL)
	{
		/// 如果是海图不做处理
		if (pEditingItem->type() == CLayerConfig::SeaLayerCategory)
		{
			return;
		}
	}
	/// 判断是否海图 begin

	pEditingItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled);

	CMapLayerItem *pMapLayerItem = static_cast<CMapLayerItem *>(pEditingItem);
	if (pMapLayerItem == nullptr)
	{
		return;
	}

	QString strTextAfterEdit = pMapLayerItem->text(0);
	if (strTextAfterEdit.isEmpty())
	{
		pMapLayerItem->setText(0, pMapLayerItem->data(0, CurrentTextRole).toString());
		return;
	}

 	// 更新树节点和底层信息
 	HGxScene::CGxGlobalLayer* pLayer = pMapLayerItem->mapLayer();
 	if (pLayer == nullptr)
 	{
 		return;
 	}
 	pLayer->setName(strTextAfterEdit.toUtf8().data());
 	m_pPropertyMgr->changeCurrentLayer(pLayer);
}

void CMapLayerTreeWidget::addLayerToSceneSlot(const QString &strName, const QString &strPath, const QString &strDriverType, CLayerConfig::LayerType layerType)
{
	// 生成金字塔缓存
	if (generatePyramid(strName, strPath, strDriverType, layerType))
	{
		return;
	}
	addLocalFileLayer(strName, strPath);
}

void CMapLayerTreeWidget::renameShortCutSlot()
{
	sendModifiedEvent(true);

	QList<QTreeWidgetItem *> listSelcItems = selectedItems();
	if (listSelcItems.size() <= 0 || listSelcItems.size() > 1)
	{
		return;
	}
	
	
	if (listSelcItems[0]->type() == CLayerConfig::SeaLayerCategory)
	{
		emit shorcutRenameItemSignal();
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
		act.setData(CMapLayerTreeWidget::RENAME);
		contextMenuActionSlot(&act);
	}
}

void CMapLayerTreeWidget::deleteShortCutSlot()
{
	sendModifiedEvent(true);

	QList<QTreeWidgetItem *> listSelcItems = selectedItems();
	if (listSelcItems.size() <= 0)
	{
		return;
	}

	/// 如果是海图 beign
	if (listSelcItems[0]->type() == CLayerConfig::SeaLayerCategory)
	{
		emit shortcutDeleteItemSignal();
		return;
	}
	/// 如果是海图 end

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
			CMapLayerItem *pMapLayerItem = static_cast<CMapLayerItem *>(pCurrentItem);
			if (pMapLayerItem)
			{
				CMessageBox::ReturnButton ret = CMessageBox::question(tr("are you sure to delete the selected layer?"), m_pMainWindow);
				if (ret == CMessageBox::Cancel)
				{
					return;
				}

				HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

				if (pLayerManager == nullptr)
				{
					return;
				}

				m_pPropertyMgr->deleteLayerProperty(pMapLayerItem->mapLayer());

				pLayerManager->deleteLayer(pMapLayerItem->mapLayer());

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
		CMessageBox::ReturnButton ret = CMessageBox::question(tr("are you sure to delete the selected layer?"), m_pMainWindow);
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
				CMapLayerItem *pMapLayerItem = static_cast<CMapLayerItem *>(pCurrentItem);
				if (pMapLayerItem)
				{

					HGxScene::CGxLayerManager* pLayerManager = getLayerManager(m_strSceneID);

					if (pLayerManager == nullptr)
					{
						return;
					}

					m_pPropertyMgr->deleteLayerProperty(pMapLayerItem->mapLayer());

 					pLayerManager->deleteLayer(pMapLayerItem->mapLayer());

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

void CMapLayerTreeWidget::propertyShortCutSlot()
{
	QList<QTreeWidgetItem *> listSelcItems = selectedItems();
	if (listSelcItems.size() <= 0 || listSelcItems.size() > 1)
	{
		return;
	}

	/// 如果是海图 beign
	if (listSelcItems[0]->type() == CLayerConfig::SeaLayerCategory)
	{
		emit shortcutPropertyItemSignal();
		return;
	}
	/// 如果是海图 end

	CCheckableTreeItem *pCurrentItem = dynamic_cast<CCheckableTreeItem *>(listSelcItems[0]);
	if (pCurrentItem == nullptr)
	{
		return;
	}
	CLayerConfig::TreeItemType iItemType = (CLayerConfig::TreeItemType)pCurrentItem->type();
	if (pCurrentItem->type() == CLayerConfig::LayerItem)
	{
		QAction act(this);
		act.setData(CMapLayerTreeWidget::PROPERTIES);
		contextMenuActionSlot(&act);
	}
}

void CMapLayerTreeWidget::itemSelectionChangedSlot()
{
// 	/// 判断是否海图 begin add by yq00012
//  	QTreeWidgetItem *pEditingItem = currentItem();
//  	if (pEditingItem != NULL)
//  	{
//  		/// 如果是海图不做处理
//  		if (pEditingItem->type() == CLayerConfig::SeaLayerCategory)
//  		{
//  			return;
//  		}
//  	}
//  	/// 判断是否海图 begin
//  
//  	CMapLayerItem *pCurrentItem = dynamic_cast<CMapLayerItem *>(currentItem());
//  
//  	if (pCurrentItem == nullptr)
//  	{
//  		m_pPropertyMgr->changeCurrentLayer(nullptr);
//  	}
//  	else
//  	{
//  		m_pPropertyMgr->changeCurrentLayer(pCurrentItem->mapLayer());
//  	}
}

void CMapLayerTreeWidget::cancelProcessSlot()
{
	CMessageBox::ReturnButton ret = CMessageBox::question(tr("whether cancel and clear the generated pyramid cache?"), m_pMainWindow);
	if (ret == CMessageBox::Confirm)
	{
		if (m_pPyramidProcess != nullptr)
		{
			m_pPyramidProcess->stopProcessing();
		}
		//关闭金字塔缓存窗口
		CMapPyramidCache::getInstance(this)->closeCacheWgt();

		if (m_pPyramidProcess != nullptr)
		{
			m_pPyramidProcess->stopProcessing();
		}
	}
}

void CMapLayerTreeWidget::pyramidProcessSlot(double dfPercent)
{
	CMapPyramidCache::getInstance()->setCacheProcess(dfPercent);
}

void CMapLayerTreeWidget::onPyramidProcessFinishedSlot(const QString &name, const QString &path, const QString &driverType, int layerType)
{
	CMapPyramidCache::getInstance()->closeCacheWgt();
	addLocalFileLayer(name, path);
}

void CMapLayerTreeWidget::sceneCreatedSlot(std::string strSceneId, bool bCreateFromFile)
{
	if (NULL == m_pMainWindow)
	{
		CGxUiFrameworkServiceInterface* pUiFrameWork = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

		m_pMainWindow = pUiFrameWork->mainWindow();
	}

	HGxScene::CGxLayerManager* pLayerManager = getLayerManager(strSceneId);

 	if (pLayerManager && !bCreateFromFile)
 	{
		{
			osg::ref_ptr<HGxScene::CGxLayerModelConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createModelLayerConfig(HGxScene::SLayerOption(tr("line_world").toUtf8().data(),
				"ogr", (HGX_FILE_CONFIG().getFullPath("shpFile/world/line_world.shp"))));
			if (pLayerCfg.valid())
			{
				pLayerCfg->featureSymbol().lineColor() = osg::Vec4f(0.588, 0.588, 0.588, 1.0);
				pLayerCfg->featureSymbol().fillColor() = osg::Vec4f(1.0, 253 / 255.0, 246 / 255.0, 1.0);
				pLayerManager->addLayer(pLayerManager->createLayer(*pLayerCfg.get()));
			}
		}
		{
			osg::ref_ptr<HGxScene::CGxLayerModelConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createModelLayerConfig(HGxScene::SLayerOption(tr("line_sheng").toUtf8().data(),
				"ogr", (HGX_FILE_CONFIG().getFullPath("shpFile/china/line_sheng.shp"))));
			if (pLayerCfg.valid())
			{
				pLayerCfg->featureSymbol().lineColor() = osg::Vec4f(156 / 255.0, 145 / 255.0, 127 / 255.0, 1.0);
				pLayerManager->addLayer(pLayerManager->createLayer(*pLayerCfg.get()));
			}
		}
 		{
			osg::ref_ptr<HGxScene::CGxLayerModelConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createModelLayerConfig(HGxScene::SLayerOption(tr("point_caption").toUtf8().data(),
 				"ogr", (HGX_FILE_CONFIG().getFullPath("shpFile/world/point_caption.shp"))));
 			if(pLayerCfg.valid())
 			{
 				pLayerCfg->featureSymbol().pointIconPath() = (HGX_FILE_CONFIG().getFullPath("texture/sysTemplate/guojia_03.png"));
 				pLayerCfg->featureSymbol().pointIconSize() = 12;
 				pLayerCfg->featureSymbol().pointIconVisible() = true;
 				pLayerCfg->featureSymbol().fontName() = (HGX_FILE_CONFIG().getFullPath("fonts/simsun.ttc"));
 				pLayerCfg->featureSymbol().fontColor() = osg::Vec4(0.0, 0.0, 0.0, 1.0);
 
				pLayerManager->addLayer(pLayerManager->createLayer(*pLayerCfg.get()));
 			}
 		}
 		{
			osg::ref_ptr<HGxScene::CGxLayerModelConfig> pLayerCfg = pLayerManager->layerConfigFactory()->createModelLayerConfig(HGxScene::SLayerOption(tr("point_sheng").toUtf8().data(),
 				"ogr", (HGX_FILE_CONFIG().getFullPath("shpFile/china/point_sheng.shp"))));
 			if(pLayerCfg.valid())
 			{
 				pLayerCfg->featureSymbol().pointIconPath() = (HGX_FILE_CONFIG().getFullPath("texture/otherScene/3-4_03.png"));
 				pLayerCfg->featureSymbol().pointIconSize() = 12;
 				pLayerCfg->featureSymbol().pointIconVisible() = true;
 				pLayerCfg->featureSymbol().fontName() = (HGX_FILE_CONFIG().getFullPath("fonts/msyh.ttf"));
 				pLayerCfg->featureSymbol().fontColor() = osg::Vec4(52/255.0, 26/255.0, 0.0, 1.0);
 				pLayerCfg->visibleConfig().maxVisibleRange() = 18682334;
 				pLayerCfg->visibleConfig().minVisibleRange() = 0.9;
 
				pLayerManager->addLayer(pLayerManager->createLayer(*pLayerCfg.get()));
 			}
 		}
 	}

	init();

// 	//z00011 增加任务数量
// 	FxCore::CLoadInfo::Instance()->removeCount();
// 	//end 
}


void CMapLayerTreeWidget::initTreeSlot()
{
	init();
}

template<typename T>
T * CMapLayerTreeWidget::getService()
{
	/************此处需要通过框架获取服务*****************/

	//return FreeXFrameworkInstance.getService<T>().get();

	return NULL;
}

void CMapLayerTreeWidget::addServerLayerToSceneSlot( const QString &strName, const SLayerPropertyStruct& stLayerStruct, CLayerConfig::LayerType layerType )
{
	addMapLayerFromService(strName, stLayerStruct, layerType);
}

   /************暂时不提供海图*****************/

// FxMap::IFxSeaMpLayerService * CMapLayerTreeWidget::seaMapLayerService(int iSceneId)
// {
// 	if (iSceneId == -1)
// 	{
// 		return NULL;
// 	}
// 
// 	FxCore::CFxServicePtr<FxScene::IFxSceneService> pSceneService = FreeXFrameworkInstance.getService<FxScene::IFxSceneService>();
// 	if(!pSceneService)
// 	{
// 		return nullptr;
// 	}
// 
// 	return pSceneService->getService<FxMap::IFxSeaMpLayerService>(iSceneId).get();
// }


void CMapLayerTreeWidget::pluginEvent( const ctkPluginEvent &event )
{
	/************暂时不提供海图*****************/

// 	QString strConstantname = "freex.seamap.layer";
// 
// 	QString strName = event.getPlugin()->getSymbolicName();
// 
// 	if (ctkPluginEvent::STOPPED == event.getType())
// 	{
// 		if (m_pSeaMapRootItem == NULL)
// 		{
// 			return;
// 		}
// 
// 		
// 		if(strName == strConstantname)
// 		{
// 			this->takeTopLevelItem(indexOfTopLevelItem(m_pSeaMapRootItem));
// 			delete m_pSeaMapRootItem;
// 			m_pSeaMapRootItem = NULL;
// 		}
// 
// 	}
// 	else if (ctkPluginEvent::STARTED == event.getType())
// 	{
// 
// 		if(strName == strConstantname)
// 		{
// 			if (m_pSeaMapRootItem == NULL && m_iSceneId != -1)
// 			{
// 				CreatSeaMapRootItem();
// 
// 				if (m_pSeaMapRootItem)
// 				{
// 					addTopLevelItem(m_pSeaMapRootItem);
// 					this->expandItem(m_pSeaMapRootItem);
// 				}
// 			}
// 		}
// 	}
}

	/************暂时不提供海图*****************/

// void CMapLayerTreeWidget::CreatSeaMapRootItem()
// {
// 	if (m_pSeaMapRootItem == NULL)
// 	{
// 		m_pSeaMapRootItem = new CSeaMapLayerRootItem(this);
// 	}
// 	m_pSeaMapRootItem->SetSceneId(m_iSceneId);
// 	m_pSeaMapRootItem->InitTreeItem();
// }
