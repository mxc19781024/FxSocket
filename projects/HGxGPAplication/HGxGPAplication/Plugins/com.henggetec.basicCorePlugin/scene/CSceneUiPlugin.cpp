#include "CSceneUiPlugin.h"

//#include "scene/IFxSceneService.h"
//#include <earth/IFxEarthScene.h>
//#include <map/IFxMapScene.h>

//#include <workspace_ui/IFxWorkSpaceService.h>
//#include <FreeXFramework/FreeXFramework.h>


//#include "ICore.h"
//#include "CSceneManager.h"
//#include "CMainWindowManager.h"
#include "CSceneObj.h"
#include <QTimer>
#include <QDebug>
//#include "FreeXFramework/FxServiceQuerier.h"

#include <QCoreApplication>

#include <gdpScene/CommonGeo.h>

#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/manipulator/GxManipulatorManager.h>
#include <bdpScene/manipulator/GxBaseManipulator.h>

#include <gdpScene/manipulator/GxEarthManipulator.h>
#include <gdpScene/manipulator/GxMapManipulator.h>
#include <gdpScene/layer/GxLayerManager.h>
#include <bdpDB/GxRegister.h>
#include <FxLoadIndicator/CLoadIndicator.h>

#include <GxEventHelperInterface.h>
#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include "GxUiFrameworkServiceInterface.h"

#include <osgEarth/Viewpoint>
#include <bdpCommon/GxStrUtil.h>
#include "osgEarth/Registry"

CSceneUiPlugin::CSceneUiPlugin(ctkPluginContext *context)
	//: m_iActiveSceneId(-1)
	: m_strActiveSceneId("")
	, m_pMainWidget(NULL)
{
	m_pContext = context;

	// 初始化场景系统
	m_pSceneCrtlManager = HGxScene::CGxControllerMgr::getInstance();

	// 定时刷新
	QTimer *pRefreshTimer = new QTimer(this);
	connect(pRefreshTimer, SIGNAL(timeout()), this, SLOT(paintFrame()));
	pRefreshTimer->start(10);

	//connect(this, SIGNAL(createSceneObjSignal()), this, SLOT(createSceneObjSlot()));

	HGxEventHelperInstance.subscribeTopic(this, "com_henggetec_uiFramework", HGxFWLauncherInstance.getPluginContext());
	HGxEventHelperInstance.subscribeTopic(this, "event/sceneCreated", HGxFWLauncherInstance.getPluginContext());
}

CSceneUiPlugin::~CSceneUiPlugin()
{

}

void CSceneUiPlugin::saveSceneDockWidget(const std::string& strSceneID, QWidget* pWidget)
{
	m_hashId2Dock.insert(strSceneID.c_str(), pWidget);
}

void CSceneUiPlugin::paintFrame()
{
	if (m_pSceneCrtlManager)
	{
		m_pSceneCrtlManager->frameSimulation();
	}
}

// int CSceneUiPlugin::createScene(const QString &strSceneName, const QString &strSceneType, bool bPlotSync)
// {
// 	// [1] 名称为空，创建失败
// 	if (strSceneName.isEmpty())
// 	{
// 		return -1;
// 	}
// 	// [2] 名称已存在，创建失败
// 	auto i = m_hashId2Scene.constBegin();
// 	while (i != m_hashId2Scene.constEnd())
// 	{
// 		if (i.value()->sceneName() == strSceneName)
// 		{
// 			return -1;
// 		}
// 		++i;
// 	}
// 
// // 	// [3] 底层创建场景
// // 	FxScene::IFxSceneService *pSceneService = getService<FxScene::IFxSceneService>();
// // 	if (pSceneService == nullptr)
// // 	{
// // 		return -1;
// // 	}
// // 	IFxScene *pScene = pSceneService->createScene(strSceneName.toUtf8().data(), strSceneType.toUtf8().data());
// // 	if (pScene == nullptr)
// // 	{
// // 		return -1;
// // 	}
// // 
// // 	// [4] 工作空间中，将场景的变化状态置为true
// // 	IFxWorkSpaceService *pWorkSpaceService = getService<IFxWorkSpaceService>();
// // 	if (pWorkSpaceService)
// // 	{
// // 		pWorkSpaceService->setWorkSpaceStatus(true);
// // 	}
// // 
// // 	// [5] 将场景窗口添加到主界面dock系统中
// // 	QString strType = "";
// // 	if (pScene->getSceneType() == "scene_earth")
// // 	{
// // 		strType = tr("3D");
// // 	}
// // 	else if (pScene->getSceneType() == "scene_map")
// // 	{
// // 		strType = tr("2D");
// // 	}
// // 
// // 	QWidget *pDockWgt = ICore::sceneManager()->createScene(pScene->getWidget(), QString::fromUtf8(pScene->getName().c_str()), strType);
// // 	connect(pDockWgt, SIGNAL(activatedSignal()), this, SLOT(dockActivatedSlot()));
// // 	connect(pDockWgt, SIGNAL(visualChangedSignal(bool)), this, SLOT(dockVisibleChangedSlot(bool)));
// // 
// // 	//ICore::mainWindowManager()->openTab(strType);
// // 
// // 	// [6] 保存dock对象
// // 	int iSceneId = pScene->getID();
// // 	CSceneObj *pSceneObj = new CSceneObj(pScene, pDockWgt, this);
// // 	m_hashId2Scene.insert(iSceneId, pSceneObj);
// // 
// // 	// [7] 发出场景创建成功事件
// // 	ctkDictionary sceneCreatedDic;
// // 	sceneCreatedDic.insert("SceneID", iSceneId);
// // 	sceneCreatedDic.insert("SceneType", pScene->getSceneType().c_str());
// // 	sceneCreatedDic.insert("Condition", "CreatedFromUser");
// // 	FreeXFrameworkInstance.sendEvent("event/sceneCreated", sceneCreatedDic);
// // 
// // 	// [8] 激活场景
// // 	activateScene(pSceneObj);
// // 
// // 	//TODO:漫游器定位功能需要重新考虑修改，目前为了兼容初始化定位和自定义定位就先这样调用(2019-9-27 w00224)
// // 	if (pScene->getSceneType() == "scene_earth")
// // 	{
// // 		pScene->asEarthScene()->locateHome();
// // 	}
// // 	else if (pScene->getSceneType() == "scene_map")
// // 	{
// // 		pScene->asMapScene()->locateHome();
// // 	}
// //	
// //	return iSceneId;
// 
// 	return 0;
// }

QWidget* CSceneUiPlugin::createScene(const std::string& strSceneID, const std::string& strSceneType, int iController, bool plotSync)
{
	if (NULL == m_pSceneCrtlManager)
	{
		return NULL;
	}

	CGxUiFrameworkServiceInterface* pUiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	if (NULL == m_pMainWidget)
	{
		m_pMainWidget = pUiFrameworkService->mainWindow();
	}

	int iRes = m_pSceneCrtlManager->createScene(strSceneID, strSceneType, iController);

	if (iRes < 0)
	{
		return NULL;
	}	

	HGxScene::CGxSceneBase* pScene = m_pSceneCrtlManager->getScene(strSceneID);

	if (NULL == pScene)
	{
		return NULL;
	}

	pScene->enableDebugInfo();

	HGxScene::CGxBaseManipulator* pManipulator = pScene->getManipulatorSystem()->getCurrentManipulator();

	// 设置中心点为北京
	HGxCommon::SGxViewPoint viewpoint;
	viewpoint.dDistance = 20000000;
	viewpoint.vecAttitude = osg::Vec3d(-90.0, 0.0, 0.0);
	viewpoint.stPoint = HGxCommon::CGxPoint(116.46, 39.92, 0);

	if (pManipulator)
	{
		pManipulator->setHome(viewpoint, 2);
		pManipulator->locateHome();
	}

	m_strActiveSceneId = strSceneID;
	m_strActiveSceneType = strSceneType;

	//异步发送事件
	ctkDictionary event;
	event.insert("SceneID", strSceneID.c_str());
	event.insert("SceneType", strSceneType.c_str());
	HGxEventHelperInstance.postEvent("event/sceneCreated", event);

	QWidget* pWgt = (QWidget*)(pScene->getWindowHandle());

	CSceneObj *pSceneObj = new CSceneObj(pScene, pWgt, this);
	m_hashId2Scene.insert(strSceneID.c_str(), pSceneObj);

		if (pScene->getSceneContext())
		{
			osg::ref_ptr<osg::Camera> m_rpCamera = new osg::Camera;
			m_rpCamera->setViewMatrix(osg::Matrix::identity());
			m_rpCamera->setRenderOrder(osg::Camera::POST_RENDER, -1);
			m_rpCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
			m_rpCamera->setAllowEventFocus(true);
			m_rpCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
			m_rpCamera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			m_rpCamera->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
			//addChild(m_rpCamera.get());

			osg::ref_ptr<osg::Geode> m_rpGeode = new osg::Geode;
			osg::ref_ptr<osg::Geometry> m_rpGeom = new osg::Geometry;
			m_rpGeom->setUseDisplayList(false);
			m_rpGeom->setUseVertexBufferObjects(true);
			m_rpGeom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

			osg::ref_ptr<osg::Vec3Array> m_rpVertexArray = new osg::Vec3Array(4);
			m_rpGeom->setVertexArray(m_rpVertexArray.get());

			osg::ref_ptr<osg::Vec2Array> rpTexCoordArray = new osg::Vec2Array(4);
			(*rpTexCoordArray)[0].set(0.0f, 0.0f);
			(*rpTexCoordArray)[1].set(1.0f, 0.0f);
			(*rpTexCoordArray)[2].set(1.0f, 1.0f);
			(*rpTexCoordArray)[3].set(0.0f, 1.0f);
			m_rpGeom->setTexCoordArray(0, rpTexCoordArray.get());

			osg::ref_ptr<osg::Texture2D>m_rpTexture = new osg::Texture2D;

			m_rpGeom->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_rpTexture.get(), osg::StateAttribute::ON);
			m_rpGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

			int nImageWidth = 1920;
			int nImageHeight = 1080;
			osg::ref_ptr<osg::Image> m_rpImage = osgDB::readImageFile(HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("/texture/topBG.png"));

			osg::Vec3d vecSize(nImageWidth, nImageHeight, 0);
			osg::Vec3d vecCorner;
			osg::Vec3d vecWidth(vecSize.x(), 0.0f, 0.0f);
			osg::Vec3d vecHeight(0.0f, vecSize.y(), 0.0f);

			if (m_rpVertexArray.valid())
			{
				(*m_rpVertexArray)[0] = vecCorner;
				(*m_rpVertexArray)[1] = vecCorner + vecWidth;
				(*m_rpVertexArray)[2] = vecCorner + vecWidth + vecHeight;
				(*m_rpVertexArray)[3] = vecCorner + vecHeight;
				m_rpVertexArray->dirty();
			}

			m_rpTexture->setImage(m_rpImage.get());
			m_rpTexture->setResizeNonPowerOfTwoHint(false);
			m_rpImage->dirty();
			m_rpGeode->addDrawable(m_rpGeom.get());
			osgEarth::Registry::shaderGenerator().run(m_rpGeode.get());
			m_rpCamera->setViewport(0, 0, nImageWidth, nImageHeight);
			m_rpCamera->setProjectionMatrixAsOrtho2D(0, nImageWidth, 0, nImageHeight);
			m_rpCamera->addChild(m_rpGeode);
			pScene->getSceneContext()->getRootNode()->addChild(m_rpCamera.get());
		}


	return (QWidget*)(pScene->getWindowHandle());
}

// void CSceneUiPlugin::createSceneFromFile(IFxScene *pScene)
// {
// 	QString strType = "";
// 	if (pScene->getSceneType() == "scene_earth")
// 	{
// 		strType = tr("3D");
// 	}
// 	else if (pScene->getSceneType() == "scene_map")
// 	{
// 		strType = tr("2D");
// 	}
// 	// 将场景窗口添加到主界面dock系统中
// 	QWidget *pDockWgt = ICore::sceneManager()->createScene(pScene->getWidget(), QString::fromUtf8(pScene->getName().c_str()), strType);
// 	connect(pDockWgt, SIGNAL(activatedSignal()), this, SLOT(dockActivatedSlot()));
// 	connect(pDockWgt, SIGNAL(visualChangedSignal(bool)), this, SLOT(dockVisibleChangedSlot(bool)));
// 
// 	// 保存场景对象
// 	int iSceneId = pScene->getID();
// 	CSceneObj *pSceneObj = new CSceneObj(pScene, pDockWgt, this);
// 	m_hashId2Scene.insert(iSceneId, pSceneObj);
// 
// 	setSceneTabInMainWindow(pScene->getSceneType().c_str());
// 
// 	// note 加载工作空间创建场景时，无需发送场景创建事件通知其他插件初始化，发送场景切换事件即可
// 
// 	// [7] 发出场景创建成功事件
// 	// g00061 2019-0927 删除注释. 该事件需要发送，场景的创建与激活应该分开处理更符合逻辑
// 	ctkDictionary sceneCreatedDic;
// 	sceneCreatedDic.insert("SceneID", iSceneId);
// 	sceneCreatedDic.insert("SceneType", pScene->getSceneType().c_str());
// 	sceneCreatedDic.insert("Condition", "CreatedFromFile");
// 	FreeXFrameworkInstance.sendEvent("event/sceneCreated", sceneCreatedDic);
// 
// 	ICore::mainWindowManager()->openTab(strType);
// 	//刷新
// 	ICore::mainWindowManager()->mainWindow()->repaint();
// 
// 	if (m_strActiveSceneType == pSceneObj->sceneType())
// 	{
// 		return;
// 	}
// 
// 	m_iActiveSceneId = pSceneObj->sceneId();
// 	m_strActiveSceneType = pSceneObj->sceneType();
// 
// 	// 发出场景切换事件
// 	ctkDictionary sceneSwitchMsg;
// 	sceneSwitchMsg.insert("SceneID", m_iActiveSceneId);
// 	sceneSwitchMsg.insert("SceneType", m_strActiveSceneType);
// 	sceneSwitchMsg.insert("Condition", "CreatedFromFile");	// 通知外面，该切换事件是在场景打开文件创建时发出的
// 
// 	FreeXFrameworkInstance.sendEvent("event/sceneSwitch", sceneSwitchMsg);
// 
// 	//TODO:漫游器定位功能需要重新考虑修改，目前为了兼容初始化定位和自定义定位就先这样调用(2019-9-27 w00224)
// 	if (pScene->getSceneType() == "scene_earth")
// 	{
// 		pScene->asEarthScene()->locateHome();
// 	}
// 	else if (pScene->getSceneType() == "scene_map")
// 	{
// 		pScene->asMapScene()->locateHome();
// 	}
// }

int CSceneUiPlugin::createSceneFromTemplate(const QString &strSceneName, const QString &strFilePath, bool bSync)
{
	// 名称为空，创建失败
	if (strSceneName.isEmpty())
	{
		return -1;
	}
	// 名称已存在，创建失败
	auto i = m_hashId2Scene.constBegin();
	while (i != m_hashId2Scene.constEnd())
	{
		if (i.value()->sceneName() == strSceneName)
		{
			return -1;
		}
		++i;
	}

	/***********************此处需要重写*********************/

// 	// 从底层加载场景
// 	FxScene::IFxSceneService *pSceneService = getService<FxScene::IFxSceneService>();
// 	if (pSceneService == nullptr)
// 	{
// 		return -1;
// 	}
// 	//x00038  modify 解决读取中文路径 strFilePath.toUtf8().data() 改为toStdString
// 	IFxScene *pScene = pSceneService->loadSceneFromFile(strFilePath.toStdString(), bSync);
// 	if (!pScene)
// 	{
// 		return -1;
// 	}
// 	pScene->setName(strSceneName.toUtf8().data());
// 
// 	QString strType = "";
// 	if (pScene->getSceneType() == "scene_earth")
// 	{
// 		strType = tr("3D");
// 	}
// 	else if (pScene->getSceneType() == "scene_map")
// 	{
// 		strType = tr("2D");
// 	}
// 
// 	// 将场景窗口添加到主界面dock系统中
// 	QWidget *pDockWgt = ICore::sceneManager()->createScene(pScene->getWidget(), QString::fromUtf8(pScene->getName().c_str()), strType);
// 	pDockWgt->setProperty("type", pScene->getSceneType().c_str());
// 	connect(pDockWgt, SIGNAL(activatedSignal()), this, SLOT(dockActivatedSlot()));
// 	connect(pDockWgt, SIGNAL(visibleChangeInDockSignal(bool)), this, SLOT(dockVisibleChangedSlot(bool)));
// 
// 	// 保存场景对象
// 	int iSceneId = pScene->getID();
// 	CSceneObj *pSceneObj = new CSceneObj(pScene, pDockWgt, this);
// 	m_hashId2Scene.insert(iSceneId, pSceneObj);
// 
// 	setSceneTabInMainWindow(pScene->getSceneType().c_str());
// 
// 	// [7] 发出场景创建成功事件
// 	ctkDictionary sceneCreatedDic;
// 	sceneCreatedDic.insert("SceneID", iSceneId);
// 	sceneCreatedDic.insert("SceneType", pScene->getSceneType().c_str());
// 	//sceneCreatedDic.insert("Condition", "CreatedFromFile");
// 	FreeXFrameworkInstance.sendEvent("event/sceneCreated", sceneCreatedDic);
// 
// 	ICore::mainWindowManager()->openTab(strType);
// 	//刷新
// 	ICore::mainWindowManager()->mainWindow()->repaint();
// 
// 	if (m_strActiveSceneType == pSceneObj->sceneType())
// 	{
// 		return pScene->getID();
// 	}
// 
// 	m_iActiveSceneId = pSceneObj->sceneId();
// 	m_strActiveSceneType = pSceneObj->sceneType();
// 
// 	// 发出场景切换事件
// 	ctkDictionary sceneSwitchMsg;
// 	sceneSwitchMsg.insert("SceneID", m_iActiveSceneId);
// 	sceneSwitchMsg.insert("SceneType", m_strActiveSceneType);
// 	//sceneSwitchMsg.insert("Condition", "CreatedFromFile");	// 通知外面，该切换事件是在场景打开文件创建时发出的
// 
// 	FreeXFrameworkInstance.sendEvent("event/sceneSwitch", sceneSwitchMsg);
// 
// 	//TODO:漫游器定位功能需要重新考虑修改，目前为了兼容初始化定位和自定义定位就先这样调用(2019-9-27 w00224)
// 	if (pScene->getSceneType() == "scene_earth")
// 	{
// 		pScene->asEarthScene()->locateHome();
// 	}
// 	else if (pScene->getSceneType() == "scene_map")
// 	{
// 		pScene->asMapScene()->locateHome();
// 	}
// 
// 	return pScene->getID();

	return 0;
}

void CSceneUiPlugin::setSceneTabInMainWindow(const QString &strSceneType)
{
	/****************此处需要获取主窗口管理**********************/

// 	if (strSceneType == "scene_earth")
// 	{
// 		ICore::mainWindowManager()->setCurrentTab(tr("3D"));
// 	}
// 	else if (strSceneType == "scene_map")
// 	{
// 		ICore::mainWindowManager()->setCurrentTab(tr("2D"));
// 	}
}

void CSceneUiPlugin::dockVisibleSet(const std::string& sceneType)
{
	if (GX_EARTH_SCENE_TYPE == sceneType)
	{

	}
	else if (GX_MAP_SCENE_TYPE == sceneType)
	{

	}
}

void CSceneUiPlugin::activateScene(ISceneInterface *pSceneObj)
{
	// 场景id没有变化，则返回
	if (m_strActiveSceneId == pSceneObj->sceneId())
	{
		return;
	}

	ctkDictionary sceneSwitchProp;
	// 记录前一个场景的场景类型和场景id
	sceneSwitchProp.insert("PreSceneType", m_strActiveSceneType.c_str());
	sceneSwitchProp.insert("PreSceneId", m_strActiveSceneId.c_str());

	m_strActiveSceneId = pSceneObj->sceneId();
	m_strActiveSceneType = pSceneObj->sceneType().toUtf8().data();

	// 记录新场景的场景类型和场景id
	sceneSwitchProp.insert("SceneType", m_strActiveSceneType.c_str());
	sceneSwitchProp.insert("SceneID", m_strActiveSceneId.c_str());

	// 发出场景切换信号(即将切换)
	HGxEventHelperInstance.postEvent("event/sceneAboutToSwitch", sceneSwitchProp);
	// 发出场景切换事件(已切换)
	HGxEventHelperInstance.postEvent("event/sceneSwitch", sceneSwitchProp);

}

bool CSceneUiPlugin::removeScene(std::string iSceneId)
{
	// 场景id不存在，删除失败
	if (!m_hashId2Scene.contains(iSceneId.c_str()))
	{
		return false;
	}

	bool bSuccess = false;

	ISceneInterface *pSceneObj = m_hashId2Scene.take(iSceneId.c_str());
	QString strSceneType = pSceneObj->sceneType();

 	// 如果删除的是当前的活动场景，则场景即将发生切换，发出场景切换信号
 	if (m_strActiveSceneId == iSceneId)
 	{
 		// 发出场景切换信号(即将切换)
 		ctkDictionary sceneSwitchProp;
 		// 记录前一个场景的场景类型和场景id
 		sceneSwitchProp.insert("PreSceneType", m_strActiveSceneType.c_str());
 		sceneSwitchProp.insert("PreSceneId", m_strActiveSceneId.c_str());
 		// note:场景即将切换信号发生在场景删除时以及点击场景切换时
		HGxEventHelperInstance.postEvent("event/sceneAboutToSwitch", sceneSwitchProp);
		QCoreApplication::processEvents();
 	}

	// 发送场景删除信号到场景工具插件，使视点同步工具置为false且为不可用状态
	// note:图层和标绘插件在删除场景时的初始化操作，已放置在sceneAboutToSwitchSignal中去处理
	// 故在标绘和图层插件中没有对场景移除信号添加相应的处理槽函数
	ctkDictionary removeDic;
	removeDic.insert("SceneType", strSceneType);
	removeDic.insert("SceneID", iSceneId.c_str());
	HGxEventHelperInstance.postEvent("event/sceneRemove", removeDic);
	
	// 删除底层场景对象
	m_pSceneCrtlManager->removeScene(iSceneId);

//  // 获取工作空间服务
//  IFxWorkSpaceService *pWorkSpaceService = getService<IFxWorkSpaceService>();
//  if (pWorkSpaceService)
//  {
//		// 将场景的变化状态置为true
//  	pWorkSpaceService->setWorkSpaceStatus(true);
//  }
// 
// 	if (strSceneType == "scene_earth")
// 	{
// 		ICore::mainWindowManager()->closeTab(tr("3D"));
// 	}
// 	else if (strSceneType == "scene_map")
// 	{
// 		ICore::mainWindowManager()->closeTab(tr("2D"));
// 	}

	// 从hash中移除场景，删除场景窗口所在的dockwidget
	delete pSceneObj;
	pSceneObj = nullptr;

	// 如果有显示的场景，则切换到该场景
	auto i = m_hashId2Scene.constBegin();
	while (i != m_hashId2Scene.constEnd())
	{
		if (i.value()->dockWidget()->isVisible())
		{
			// 场景id没有变化，则返回
			if (m_strActiveSceneId == i.value()->sceneId())
			{
				return false;
			}

			ctkDictionary sceneSwitchProp;
			m_strActiveSceneId = i.value()->sceneId();
			m_strActiveSceneType = i.value()->sceneType().toUtf8().data();

			// 记录新场景的场景类型和场景id
			sceneSwitchProp.insert("SceneType", m_strActiveSceneType.c_str());
			sceneSwitchProp.insert("SceneID", m_strActiveSceneId.c_str());
			// 发出场景切换信号(已切换)
			HGxEventHelperInstance.postEvent("event/sceneSwitch", sceneSwitchProp);

			return false;
		}
		++i;
	}

	// 没有可见的场景，则切换到没有可见场景的情况
	ctkDictionary sceneSwitchProp;
	m_strActiveSceneId = "";	// 场景id设为空
	m_strActiveSceneType = "";	// 场景类型设为空

	// 发送场景切换事件
	sceneSwitchProp.insert("SceneType", m_strActiveSceneType.c_str());
	sceneSwitchProp.insert("SceneID", m_strActiveSceneId.c_str());

	// 发出场景切换信号(已切换)
	HGxEventHelperInstance.postEvent("event/sceneSwitch", sceneSwitchProp);

	return true;
}

bool CSceneUiPlugin::setCurrentScene(std::string iSceneId)
{
	if (!m_hashId2Scene.contains(iSceneId.c_str()))
	{
		return false;
	}

	ISceneInterface *pSceneObj = m_hashId2Scene[iSceneId.c_str()];

	/****************此处需重mainWindowManager和sceneManager****************/

// 	// 显示该场景
// 	//x00038 打开tab页
// 	std::string strSceneType = pSceneObj->sceneType().toStdString();
// 	if (strSceneType == "scene_earth")
// 	{
// 		ICore::mainWindowManager()->openTab(tr("3D"));
// 	}
// 	else if (strSceneType == "scene_map")
// 	{
// 		ICore::mainWindowManager()->openTab(tr("2D"));
// 	}
// 	//end
// 
// 	ICore::sceneManager()->setSceneVisible(pSceneObj->dockWidget(), true);
// 	// 界面中切换到相应的场景
// 	ICore::sceneManager()->switchScene(pSceneObj->dockWidget());

	setSceneTabInMainWindow(pSceneObj->sceneType());


	//设置完成之后切换到当前场景（w00224 2019-10-15）
	activateScene(pSceneObj);

	return true;
}

void CSceneUiPlugin::dockActivatedSlot()
{
	// 找到场景对象
	QWidget *pDockWgt = static_cast<QWidget *>(sender());
	auto i = m_hashId2Scene.constBegin();
	while (i != m_hashId2Scene.constEnd())
	{
		if (i.value()->dockWidget() == pDockWgt)
		{
			activateScene(i.value());

			//打开对应的tab页
			setSceneTabInMainWindow(i.value()->sceneType());
			return;
		}
		++i;
	}
}

void CSceneUiPlugin::dockVisibleChangedSlot(bool bVisible)
{
	if (!bVisible)
	{
		auto i = m_hashId2Scene.constBegin();
		//x00038 窗口关闭时隐藏对应的底部tab页
		while (i != m_hashId2Scene.constEnd())
		{
			ISceneInterface* pScene = i.value();
			if (!pScene->dockWidget()->isVisible())
			{
				/********************此处需重mainWindowManager******************/

// 				if (FxScene::SceneType_Earth == pScene->sceneType().toStdString())
// 				{
// 					ICore::mainWindowManager()->closeTab(tr("3D"));
// 				}
// 				else if (FxScene::SceneType_Map == pScene->sceneType().toStdString())
// 				{
// 					ICore::mainWindowManager()->closeTab(tr("2D"));
// 				}
			}
			++i;
		}
		
		//x00038 end

		i = m_hashId2Scene.constBegin();
		// 如果有显示的场景，则切换到该场景
		
		while (i != m_hashId2Scene.constEnd())
		{
			if (i.value()->dockWidget()->isVisible())
			{
				activateScene(i.value());
				return;
			}
			++i;
		}

		// 否则，切换到没有可见场景的情况

		ctkDictionary sceneSwitchProp;

		// 记录前一个场景的场景类型和场景id
		sceneSwitchProp.insert("PreSceneType", m_strActiveSceneType.c_str());
		sceneSwitchProp.insert("PreSceneId", m_strActiveSceneId.c_str());

		m_strActiveSceneId = -1;
		m_strActiveSceneType = "";

		// 记录新场景的场景类型和场景id
		sceneSwitchProp.insert("SceneType", m_strActiveSceneType.c_str());
		sceneSwitchProp.insert("SceneID", m_strActiveSceneId.c_str());
		// 发出场景切换信号(即将切换)
		HGxEventHelperInstance.postEvent("event/sceneAboutToSwitch", sceneSwitchProp);
		// 发出场景切换信号(已切换)
		HGxEventHelperInstance.postEvent("event/sceneSwitch", sceneSwitchProp);
	}
}

void CSceneUiPlugin::sendSwitchEvents()
{
	ctkDictionary sceneSwitchMsg;
	// sceneSwitchMsg.insert("SceneID", m_iActiveSceneId);
	sceneSwitchMsg.insert("SceneID", m_strActiveSceneType.c_str());
	sceneSwitchMsg.insert("SceneType", m_strActiveSceneType.c_str());

	HGxEventHelperInstance.postEvent("event/sceneSwitch", sceneSwitchMsg);
}

void CSceneUiPlugin::createSceneObjSlot()
{
	QHash<QString, QWidget*>::iterator it = m_hashId2Dock.begin();

	for (; it != m_hashId2Dock.end(); it++)
	{
		HGxScene::CGxSceneBase* pScene = HGxScene::CGxControllerMgr::getInstance()->getScene(it.key().toStdString());

		if (pScene)
		{
			continue;
		}

		CSceneObj *pSceneObj = new CSceneObj(pScene, it.value(), this);
		m_hashId2Scene.insert(it.key(), pSceneObj);
	}
}

std::string CSceneUiPlugin::activeScene() const
{
	return m_strActiveSceneId;
}

QString CSceneUiPlugin::activeSceneType() const
{
	return m_strActiveSceneType.c_str();
}

QList<ISceneInterface *> CSceneUiPlugin::mapScenes() const 
{
	QList<ISceneInterface *> listMapScene;
	auto i = m_hashId2Scene.constBegin();
	while (i != m_hashId2Scene.constEnd())
	{
		if (i.value()->sceneType() == "scene_map")
		{
			listMapScene.append(i.value());
		}
		++i;
	}
	return listMapScene;
}

QList<ISceneInterface *> CSceneUiPlugin::earthScenes() const 
{
	QList<ISceneInterface *> listMapScene;
	auto i = m_hashId2Scene.constBegin();
	while (i != m_hashId2Scene.constEnd())
	{
		if (i.value()->sceneType() == "scene_earth")
		{
			listMapScene.append(i.value());
		}
		++i;
	}
	return listMapScene;
}

ISceneInterface *CSceneUiPlugin::scene(std::string iSceneId)
{
	if (m_hashId2Scene.contains(iSceneId.c_str()))
	{
		return m_hashId2Scene[iSceneId.c_str()];
	}
	return nullptr;
}

ISceneInterface *CSceneUiPlugin::scene(const QString &strSceneName)
{
	auto i = m_hashId2Scene.constBegin();
	while (i != m_hashId2Scene.constEnd())
	{
		if (i.value()->sceneName() == strSceneName)
		{
			return i.value();
		}
		++i;
	}
	return nullptr;
}

bool CSceneUiPlugin::closeScene(std::string unSceneID, const QString &strName /*= ""*/ )
{
	if (!m_hashId2Scene.contains(unSceneID.c_str()))
	{
		return false;
	}
	// 找到该场景
	ISceneInterface *pSceneObj = m_hashId2Scene[unSceneID.c_str()];

	/****************此处需要通过界面框架***************/
// 	// 隐藏该场景
// 	ICore::sceneManager()->setSceneVisible(pSceneObj->dockWidget(), false);

	return true;
}

bool CSceneUiPlugin::renameScene( const QString &strNewSceneName, const std::string unSceneID /*= 0*/ )
{
	// 底层场景对象修改名称
	if (!m_hashId2Scene.contains(unSceneID.c_str()))
	{
		return false;
	}

	HGxScene::CGxSceneBase* pScene = HGxScene::CGxControllerMgr::getInstance()->getScene(unSceneID);

	if (pScene)
	{
		pScene->setSceneName(strNewSceneName.toStdString());
	}

// 	// 界面窗口tab页修改名称
// 	ICore::sceneManager()->renameScene(pScene->dockWidget(), strNewSceneName);
// 
// 	// 获取工作空间服务
// 	IFxWorkSpaceService *pWorkSpaceService = getService<IFxWorkSpaceService>();
// 	if (pWorkSpaceService)
// 	{
// 		// 将场景的变化状态置为true
// 		pWorkSpaceService->setWorkSpaceStatus(true);
// 	}

	return true;
}

// bool CSceneUiPlugin::load(const FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess)
// {
// 	const FxCommonUtils::TiXmlElement *pRoot = pEle->FirstChildElement("scene_ui");
// 	if (pRoot == nullptr)
// 	{
// 		return false;
// 	}
// 	FxScene::IFxSceneService *pSceneService = getService<FxScene::IFxSceneService>();
// 	if (pSceneService == nullptr)
// 	{
// 		return false;
// 	}
// 
// 	// 调用底层接口，创建场景
// 	pSceneService->load(pRoot, pProcess);
// 
// 	// 初始化界面
// 	QList<IFxScene *> listScenes = pSceneService->getScenes();
// 	foreach(auto pScene, listScenes)
// 	{
// 		createSceneFromFile(pScene);
// 	}
// 
// 	return true;
// }
// 
// bool CSceneUiPlugin::save(FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess)
// {
// 	FxCommonUtils::TiXmlElement *pRoot = new FxCommonUtils::TiXmlElement("scene_ui");
// 	pEle->LinkEndChild(pRoot);
// 
// 	FxScene::IFxSceneService *pSceneService = getService<FxScene::IFxSceneService>();
// 	if (pSceneService == nullptr)
// 	{
// 		return false;
// 	}
// 	pSceneService->save(pRoot, pProcess);
// 
// 	return true;
// }
// 
// int CSceneUiPlugin::getDocLevel()
// {
// 	return DOC_CORE + 1;
// }

void CSceneUiPlugin::pauseRender(bool bPause)
{
	if (m_pSceneCrtlManager)
	{
		m_pSceneCrtlManager->pauseSimulation(m_strActiveSceneId, bPause);
	}
}

void CSceneUiPlugin::handleEvent(const ctkEvent &event)
{
	if (event.getTopic() == "com_henggetec_uiFramework")
	{
		QString sceneId = event.getProperty("id").toString();
// 
// 		CGxUiFrameworkServiceInterface* pServiceUI = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());
// 
// 		if (pServiceUI)
// 		{
// 			std::string strSceneType = pServiceUI->dockUserData(sceneId, Qt::UserRole).toString().toStdString();
// 
// 			if (strSceneType == GX_EARTH_SCENE_TYPE || strSceneType == GX_MAP_SCENE_TYPE)
// 			{
// 				m_strActiveSceneType = strSceneType;
// 				m_strActiveSceneId = sceneId.toStdString();
// 			}
// 		}
	}
	else if (event.getTopic() == "event/sceneCreated")
	{
		emit createSceneObjSignal();
	}
}
