#include "SceneControlWidget.h"
#include "ui_CSceneControlWidget.h"
#include <QStandardItemModel>
#include <QPixmapCache>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QFile>
#include "QLinkedList"
#include "QPainter"
#include "GxPluginFwLauncherInterface.h"
#include "GxEventHelperInterface.h"
#include "GxPluginFrameworkCore.h"
#include "public\scene_ui\ISceneUiService.h"
#include "public\scene_ui\ISceneInterface.h"
#include "bdpScene\manipulator\GxManipulatorManager.h"
#include "bdpScene\scene\GxSceneBase.h"
#include "gdpScene/CommonGeo.h"
#include "gdpScene\manipulator\GxEarthManipulator.h"
#include "bdpAlgor/GxCoordConverter.h"
#include "GxPluginFwLauncherInterface.h"
#include "IDriveMgrInterface.h"
#include "GxServiceQuerier.h"
//#include "bdpObject/GxExplosionNode.h"
#include "service/event/ctkEventAdmin.h"


#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

CSceneControlWidget::CSceneControlWidget(ctkPluginContext *context, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CSceneControlWidget)
	, m_pContext(context)
	, m_pLoadDataWgt(NULL)
	, m_pSimuWidget(NULL)
	, m_bSpaceManiputor(true)
{
    ui->setupUi(this);
	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/LoginFinished", HGxFWLauncherInstance.getPluginContext());
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);
	init();
	QFile file(":/qss/resrc/qss/Scene_Control.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = styleSheet() + file.readAll();
		setStyleSheet(str);
	}

	connect(ui->SimuListBtn, SIGNAL(clicked()), this, SLOT(SlotShowSimuListWgt()));
	connect(ui->LoadDataBtn, SIGNAL(clicked()), this, SLOT(SlotShowLoadDataWgt()));
	connect(ui->SwitchManipulatorBtn,SIGNAL(clicked()), this, SLOT(SlotSwitchManipulator()));
	connect(m_pLoadDataWgt,SIGNAL(SignalSwitchMP(int)),this,SLOT(SlotSwitchMP(int)));
	connect(this, SIGNAL(SceneControlShowSignal()), this, SLOT(SceneControlShowSlot()));
}

CSceneControlWidget::~CSceneControlWidget()
{
    delete ui;
}

void CSceneControlWidget::handleEvent(const ctkEvent& event)
{
	if (event.getTopic() == "event/LoginFinished")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit SceneControlShowSignal();
	}
}

void CSceneControlWidget::SlotShowSimuListWgt()
{
	if (m_pSimuWidget)
	{
		//m_pSimuWidget->show();
		//m_pSimuWidget->move(20,150);
		HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());

		if (driveMgrInterface)
		{
			driveMgrInterface->clearUnit();
		}
	}
}

void CSceneControlWidget::SlotShowLoadDataWgt()
{
	if (m_pLoadDataWgt)
	{
		m_pLoadDataWgt->show();
		m_pLoadDataWgt->move((1920-m_pLoadDataWgt->width())/2,(1080 -m_pLoadDataWgt->height())/2 );
	}
}

void CSceneControlWidget::init()
{
	if (!m_pLoadDataWgt)
	{
		m_pLoadDataWgt = new CLoadDataWidget(m_pContext,this);
		m_pLoadDataWgt->move(500,300);
		m_pLoadDataWgt->hide();
	}
	if (!m_pSimuWidget)
	{
		m_pSimuWidget = new CSimuListWidget(m_pContext,this);
		m_pLoadDataWgt->hide();
	}
}

void CSceneControlWidget::SlotSwitchManipulator()
{
// 	getScenceService(0);
// 	if (m_spaceService == NULL)
// 	{
// 		return;
// 	}
// 	if (m_bSpaceManiputor)
// 	{
// 		m_spaceService->switchManipulator(FxSpace::E_FREEMP);
// 		ui->SwitchManipulatorBtn->setText("地球操作器");
// 		m_bSpaceManiputor = false;
// 	}
// 	else
// 	{
// 		m_spaceService->switchManipulator(FxSpace::E_SPACE_CENTRAL_BODY);
// 		ui->SwitchManipulatorBtn->setText("天体操作器");
// 		m_bSpaceManiputor = true;
// 	}
	ui->SwitchManipulatorBtn->setText("地球操作器");
	ISceneUiService* pSceneUiService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
	if (pSceneUiService)
	{
		ISceneInterface* pISceneInterface = pSceneUiService->scene(QStringLiteral("3D"));
		if (pISceneInterface)
		{
			if (pISceneInterface->fxScene())
			{
				HGxScene::CGxManipulatorManager* pGxManipulatorManager = pISceneInterface->fxScene()->getManipulatorSystem();
				if (pGxManipulatorManager)
				{
					if (!pGxManipulatorManager->activeManipulator(FREE_EARTH_MANIPULATOR_KEY))
					{
						return;
					}
					HGxScene::CGxEarthManipulator* pManipulator = static_cast<HGxScene::CGxEarthManipulator*> (pGxManipulatorManager->getCurrentManipulator());
					if (pManipulator)
					{
						pManipulator->locateHome();
					}
				}
			}
		}
	}
}

void CSceneControlWidget::getScenceService( int nSceneID )
{
// 	FxCore::CFxServiceQuerier<FxScene::IFxSceneService> sceneService;
// 	auto pScene = sceneService->getScene(nSceneID);
// 	if (pScene)
// 	{
// 		if (FxScene::SceneType_Earth != pScene->getSceneType())
// 		{
// 			return;
// 		}
// 	}
// 
// 	FxScene::CFxSceneServiceQuerier<FxSpace::IFxSpaceService> serviceProxy(nSceneID,sceneService);
// 
// 	m_spaceService = serviceProxy.get();
}

void CSceneControlWidget::SlotSwitchMP( int nIndex)
{
// 	getScenceService(0);
// 	if (m_spaceService == NULL)
// 	{
// 		return;
// 	}
// 	if (nIndex == 0)
// 	{
// 		m_spaceService->switchManipulator(FxSpace::E_SPACE_CENTRAL_BODY);
// 		ui->SwitchManipulatorBtn->setText("天体操作器");
// 		m_bSpaceManiputor = true;
// 	}
// 	else
// 	{
// 		
// 		m_spaceService->switchManipulator(FxSpace::E_FREEMP);
// 		ui->SwitchManipulatorBtn->setText("地球操作器");
// 		m_bSpaceManiputor = false;
// 	}
}

void CSceneControlWidget::SceneControlShowSlot()
{
	this->show();
	m_pLoadDataWgt->show();
}




