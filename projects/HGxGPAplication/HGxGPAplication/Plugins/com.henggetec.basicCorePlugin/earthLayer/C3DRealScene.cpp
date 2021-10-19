#include <ctkPluginContext.h>
//#include <ICore.h>
//#include <CMainWindowManager.h>
#include "C3DRealScene.h"

#include <gdpScene/CommonGeo.h>
#include <gdpScene/layer/GxLayerManager.h>
#include <gdpScene/layer/GxLayerConfig.h>
#include <gdpScene/layer/GxLayerLive.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>

#include "GxUiFrameworkServiceInterface.h"

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
C3DRealScene::C3DRealScene(ctkPluginContext *context, QWidget *parent, Qt::WFlags flags)
#else
C3DRealScene::C3DRealScene(ctkPluginContext *context, QWidget *parent, Qt::WindowFlags flags)
#endif
	: QWidget(parent, flags)
	, m_pContext(context)
	, m_pSelectWgt(NULL)
	, m_pTiltModelWgt(NULL)
	, m_pTiltToolWgt(NULL)
	, m_strSceneID("")
{
	ui.setupUi(this);

	m_vecToolItems.clear();

	// 初始化界面
	m_pSelectWgt = new C3DRealSceneSelectWidget(this);
	QVBoxLayout *pToolLayOut = new QVBoxLayout(ui.toolWgt);
	pToolLayOut->setContentsMargins(0,0,0,0);
	pToolLayOut->addWidget(m_pSelectWgt);
	ui.toolWgt->setLayout(pToolLayOut);

	//加入工具按钮并设置样式
	{
		QPixmap Pixmap1_n(":/Resources/images_earth_layer/qingxiesheying_normal.png");
		QPixmap Pixmap1_h(":/Resources/images_earth_layer/qingxiesheying_hover.png");
		QPixmap Pixmap1_s(":/Resources/images_earth_layer/qingxiesheying_press.png");
		m_pTiltModelBtn = m_pSelectWgt->addPlotType(false, tr("tilt photography"), Pixmap1_n, Pixmap1_h, Pixmap1_s);
		m_vecToolItems.push_back(m_pTiltModelBtn);
		connect(m_pTiltModelBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(slotTiltModel()));
	}
	{
		QPixmap Pixmap1_n(":/Resources/images_earth_layer/zhuanhuan_normal.png");
		QPixmap Pixmap1_h(":/Resources/images_earth_layer/zhuanhuan_hover.png");
		QPixmap Pixmap1_s(":/Resources/images_earth_layer/zhuanhuan_press.png");
		m_pTiltToolBtn = m_pSelectWgt->addPlotType(false, tr("conversion tool"), Pixmap1_n, Pixmap1_h, Pixmap1_s);
		m_vecToolItems.push_back(m_pTiltToolBtn);
		connect(m_pTiltToolBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(slotTiltTool()));
	}
	m_pTiltToolBtn->hide();

	QFile file(":/Resources/qss_earth_layer/layerselector_widget.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(file.readAll());
	}

	HGxEventHelperInstance.subscribeTopic(this, "event/sceneCreated", HGxFWLauncherInstance.getPluginContext());
	HGxEventHelperInstance.subscribeTopic(this, "event/sceneSwitch", HGxFWLauncherInstance.getPluginContext());
}

C3DRealScene::~C3DRealScene()
{

}

void C3DRealScene::handleEvent(const ctkEvent &event)
{
	if (event.getTopic() == "event/sceneCreated")
	{
		std::string strSceneType = event.getProperty("SceneType").toString().toStdString();
		if (strSceneType == GX_EARTH_SCENE_TYPE)
		{
			// 记录当前场景id
			m_strSceneID = event.getProperty("SceneID").toString().toStdString();
		}
	}
	else if (event.getTopic() == "event/sceneSwitch")
	{
		std::string strSceneType = event.getProperty("SceneType").toString().toStdString();
		if (strSceneType == GX_EARTH_SCENE_TYPE)
		{
			// 记录当前场景id
			m_strSceneID = event.getProperty("SceneID").toString().toStdString();
		}
	}
}

void C3DRealScene::slotTiltModel()
{
	if (NULL == m_pTiltModelWgt)
	{
		CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());
		if (NULL == uiFrameworkService)
		{
			return;
		}

		osg::ref_ptr<HGxScene::CGxLayerLiveConfig> pLayerConfig = new HGxScene::CGxLayerLiveConfig;
		HGxScene::CGxLayerLive* pLayer = static_cast<HGxScene::CGxLayerLive*>(HGxScene::CGxLayerManager::getOrCreateLayerManager(m_strSceneID)->createLayer(*pLayerConfig.get()));
		m_pTiltModelWgt = new CTiltAddModelWgt(true, pLayer, parentWidget() ? parentWidget() : uiFrameworkService->mainWindow());
		connect(m_pTiltModelWgt, SIGNAL(linkTiltToolSignal()), this, SLOT(slotTiltTool()));
		m_pTiltModelWgt->setTileModelShow(true);
		m_pTiltModelWgt->show();
	}
	else
	{
		bool bStatus = m_pTiltModelWgt->getTileModelShow();
		if (!bStatus)
		{
			if (m_pTiltModelWgt)
			{
				disconnect(m_pTiltModelWgt, SIGNAL(linkTiltToolSignal()), this, SLOT(slotTiltTool()));
				delete m_pTiltModelWgt;
				m_pTiltModelWgt = NULL;
			}
	
			CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());
			if (NULL == uiFrameworkService)
			{
				return;
			}

			osg::ref_ptr<HGxScene::CGxLayerLiveConfig> pLiveLayerConfig = new HGxScene::CGxLayerLiveConfig;
			HGxScene::CGxGlobalLayer* pLiveLayer = HGxScene::CGxLayerManager::getOrCreateLayerManager(m_strSceneID)->createLayer(*pLiveLayerConfig.get());
			m_pTiltModelWgt = new CTiltAddModelWgt(true, pLiveLayer, parentWidget() ? parentWidget() : uiFrameworkService->mainWindow());
			connect(m_pTiltModelWgt, SIGNAL(linkTiltToolSignal()), this, SLOT(slotTiltTool()));
			m_pTiltModelWgt->setTileModelShow(true);
			m_pTiltModelWgt->show();
			
		}
	}
}
void C3DRealScene::slotTiltTool()
{
	if (NULL == m_pTiltToolWgt)
	{
		/***********************此处需要获取主窗口**************************/	
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

		/***********************此处需要获取主窗口**************************/
		m_pTiltToolWgt = new CTiltToolWgt(/*parentWidget() ? parentWidget() : ICore::mainWindowManager()->mainWindow()*/);
		m_pTiltToolWgt->show();

	}
}