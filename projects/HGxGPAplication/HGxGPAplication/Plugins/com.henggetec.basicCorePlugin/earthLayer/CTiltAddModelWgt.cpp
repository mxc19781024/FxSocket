#include "CTiltAddModelWgt.h"
//#include <FreeXCommonUtils/PathRegistry.h>
//#include <scene/SceneServiceUtil.h>
//#include <FreeXFramework/FreeXFramework.h>
//#include <scene/IFxSceneService.h>
#include <QFileDialog>
#include <QFile>
#include "scene_ui/ISceneUiService.h"
#include "earth_layer_ui/IEarthLayerUiService.h"
//#include "ICore.h"
//#include "CToolManager.h"
#include <QScrollBar>
#include <FxMessageBox/CMessageBox.h>
//#include <CMainWindowManager.h>

#include <bdpDB/GxRegister.h>

#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerLive.h>
#include <gdpScene/layer/GxLayerManager.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

CTiltAddModelWgt::CTiltAddModelWgt(bool bIsAddTree, HGxScene::CGxGlobalLayer* pLayer, QWidget *parent)
	: CFxBaseWindow(parent)
	, m_pLiveLayer(NULL)
	, m_bStatus(false)
	, m_bIsLoadedFile(false)
	, m_bAddTreeStatus(bIsAddTree)
{

	//读样式表文件
	QFile file(":/Resources/qss_earth_layer/earthlayerproperty.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = file.readAll();
		m_pCentralWgt->setStyleSheet(styleSheet().append(str));
		file.close();
	}
	ui.setupUi(m_pCentralWgt);

	resize(450,280);
	setWindowModality(Qt::WindowModal);
	setWindowTitle(tr("add tilt photography model"));
	setResizable(false);
	QMetaObject::connectSlotsByName(this);
	ui.m_pPathToolBtn->setToolTip(tr("browse"));
	ui.m_pHttpToolBtn->setToolTip(tr("link"));
	QRegExp rx("\\S{0,20}");
	ui.m_pNameEdit->setValidator(new QRegExpValidator(rx, this));
	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.m_pDescribeEdt->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.m_pDescribeEdt->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);

	m_pLiveLayer = static_cast<HGxScene::CGxLayerLive*>(pLayer);
	m_pLiveLayer->setName(tr("tilt photography model").toUtf8().data());

	ui.linkLabel->hide();

	initSignalAndSlot();

	//初始化为本地服务
	dataFilePushBtn(true);
}

CTiltAddModelWgt::~CTiltAddModelWgt()
{
}

void CTiltAddModelWgt::initSignalAndSlot()
{
	connect(ui.linkLabel, SIGNAL(linkActivated(QString)), this, SLOT(linkTiltToolSlot()));
	connect(ui.linkLabel, SIGNAL(linkActivated(QString)), this, SLOT(onCloseWindowSlot()));
	connect(ui.m_pNameEdit, SIGNAL(textEdited(QString)), this, SLOT(nameChangedSlot(QString)));
	connect(ui.m_pNameEdit, SIGNAL(textEdited(QString)), this, SIGNAL(layerNameChanged(QString)));
	connect(ui.m_pHttpLineEdit, SIGNAL(textEdited(QString)), this, SLOT(httpChangedSlot(QString)));
	connect(ui.m_pDescribeEdt, SIGNAL(textChanged()), this, SLOT(descChangedSlot()));
	connect(ui.m_pPathToolBtn, SIGNAL(clicked()), this, SLOT(pathBtnChangedSlot()));
	connect(ui.m_pHttpToolBtn, SIGNAL(clicked()), this, SLOT(httpBtnChangedSlot()));

	connect(ui.m_pSaveBtn, SIGNAL(clicked()), this, SLOT(saveBtnClickedSlot()));
	connect(this, SIGNAL(closeSignals()), this, SLOT(onCloseWindowSlot()));

	connect(ui.m_pDataFilePushBtn, SIGNAL(toggled(bool)), this, SLOT(dataFilePushBtn(bool)));
	connect(ui.m_pHttpDirPushBtn, SIGNAL(toggled(bool)), this, SLOT(httpDirPushBtn(bool)));

	connect(ui.m_pCancelBtn, SIGNAL(clicked()), this, SLOT(cancleBtnClickedSlot()));
	connect(ui.m_pCancelBtn, SIGNAL(clicked()), this, SLOT(onCloseWindowSlot()));

	connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(cancleBtnClickedSlot()));
}

void CTiltAddModelWgt::setWidgetInfo()
{
	if (m_pLiveLayer)
	{
		ui.m_pNameEdit->setText(QString::fromUtf8(m_pLiveLayer->getName().c_str()));
		ui.m_pDescribeEdt->setText(QString::fromUtf8(m_pLiveLayer->getDescirbe().c_str()));
	}
}

void CTiltAddModelWgt::BrowseConfigPath()
{
	QString strCaption = tr("open file");

 	QString strDir = tr(HGX_FILE_CONFIG().getDataPath().c_str());
     #if WIN32
        QString filePath = QFileDialog::getOpenFileName(this, strCaption, strDir, tr("model Files(*.fgt *.scp *.lfp)"));
     #else
        QString filePath = QFileDialog::getOpenFileName(0, strCaption, strDir, tr("model Files(*.fgt *.scp *.lfp)"));
     #endif

	if (!filePath.isEmpty())
	{
		ui.m_pPathLineEdit->setText(filePath);
		m_pLiveLayer->setTiltPath(filePath.toUtf8().data());
		m_bIsLoadedFile = true;
	}
}
void CTiltAddModelWgt::httpChangedSlot(QString strHttp)
{

}
void CTiltAddModelWgt::linkTiltToolSlot()
{
	cancleBtnClickedSlot();
	emit linkTiltToolSignal();

}
void CTiltAddModelWgt::dataFilePushBtn(bool b)
{   
	if(b)
	{
		ui.m_pStackedWgt->setCurrentIndex(0);
		ui.m_pPathLineEdit->setText(tr("support fgt,scp,lfp formats"));
		m_bIsLoadedFile = false;
	}    
	m_bIsDataFileType = b;
}
void CTiltAddModelWgt::httpDirPushBtn(bool b)
{
	if(b)
	{
		ui.m_pStackedWgt->setCurrentIndex(1);
		ui.m_pHttpTipsLabel->setText(tr("please enter network address and link"));
		m_bIsLoadedFile = false;
	} 
	m_bIsDataFileType = !b;
}
void CTiltAddModelWgt::httpBtnChangedSlot()
{
	if("" == ui.m_pHttpLineEdit->text())
	{
		return;
	}

 	m_pLiveLayer->setTiltType(HGxScene::CGxLayerLive::E_TILT_TYPE_SERVICE);
 	if(m_pLiveLayer->setTiltPath(ui.m_pHttpLineEdit->text().toStdString()))
 	{
 		ui.m_pHttpTipsLabel->setText(tr("link successd!"));
 		m_bIsLoadedFile = true;
 	}
 	else
 	{
 		ui.m_pHttpTipsLabel->setText(tr("link failed!"));
 		m_bIsLoadedFile = false;
 	}

}
void CTiltAddModelWgt::pathBtnChangedSlot()
{
	m_pLiveLayer->setTiltType(HGxScene::CGxLayerLive::E_TILT_TYPE_FILEPATH);
	BrowseConfigPath();
}

void CTiltAddModelWgt::saveBtnClickedSlot()
{
	if(!m_bIsLoadedFile)
	{
		/************此处需要获取主窗口*****************/
		int ret = CMessageBox::information(tr("the add content cannot be empty!")/*,ICore::mainWindowManager()->mainWindow()*/);
		return;
	}

	IEarthLayerUiService* pMapLayerWgtService = HGXPFCoreInstance.getService<IEarthLayerUiService>(HGxFWLauncherInstance.getPluginContext());

	if (pMapLayerWgtService)
	{
		pMapLayerWgtService->addLiveLayer(m_pLiveLayer);
	}

	setTileModelShow(false);
	emit closeSignals();
}

void CTiltAddModelWgt::cancleBtnClickedSlot()
{
	if (m_bAddTreeStatus)
	{
 		if (m_pLiveLayer)
		{
			std::string strSceneID = "";

			ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
			if (pSceneService)
			{
				strSceneID = pSceneService->activeScene();
			}

			HGxScene::CGxLayerManager::getOrCreateLayerManager(strSceneID)->deleteLayer(m_pLiveLayer);	
 		}
	}

	setTileModelShow(false);
}


void CTiltAddModelWgt::nameChangedSlot(QString strName)
{
	if (m_pLiveLayer)
	{
		if (strName.isEmpty())
		{
			m_pLiveLayer->setName(tr("tilt photography model").toUtf8().data());
		}
		else
		{
			m_pLiveLayer->setName(strName.toUtf8().data());
		}
	}
}

void CTiltAddModelWgt::descChangedSlot()
{
	std::string strDesc = ui.m_pDescribeEdt->toPlainText().toUtf8().data();

	m_pLiveLayer->setDesciribe(strDesc);
}

void CTiltAddModelWgt::setTileModelShow( bool bStatus )
{
	m_bStatus = bStatus;
}

bool CTiltAddModelWgt::getTileModelShow()
{
	return m_bStatus;
}

void CTiltAddModelWgt::setTiltModelName( QString strName )
{
	ui.m_pNameEdit->setText(strName);
}
