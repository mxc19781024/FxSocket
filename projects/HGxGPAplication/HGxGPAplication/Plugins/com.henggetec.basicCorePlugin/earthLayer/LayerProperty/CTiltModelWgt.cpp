#include "earthLayer/LayerProperty/CTiltModelWgt.h"
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

#include <bdpDB/GxRegister.h>

#include <gdpScene/CommonGeo.h>
#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerLive.h>
#include <gdpScene/layer/GxLayerManager.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

CTiltModelWgt::CTiltModelWgt(bool bIsAddTree, HGxScene::CGxGlobalLayer* pLayer, QWidget *parent)
	: CFxBaseWindow(parent)
	, m_pLiveLayer(NULL)
	, m_bStatus(false)
	, m_bAddTreeStatus(bIsAddTree)
{
	//读样式表文件
	QFile file(":/qss_earth_layer/Resources/qss_earth_layer/earthlayerproperty.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = file.readAll();
		m_pCentralWgt->setStyleSheet(styleSheet().append(str));
		file.close();
	}
    ui.setupUi(m_pCentralWgt);
	
	setWindowModality(Qt::NonModal);
	setWindowTitle(tr("tilt photography model property"));
	ui.m_pPathToolBtn->setToolTip(tr("browse"));
	QRegExp rx("\\S{0,20}");
	ui.m_pNameEdit->setValidator(new QRegExpValidator(rx, this));
	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.m_pDescribeEdt->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.m_pDescribeEdt->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	
	//setFocusPolicy(Qt::StrongFocus);
	
	initSignalAndSlot();

	m_pLiveLayer = static_cast<HGxScene::CGxLayerLive*>(pLayer);

	if (m_bAddTreeStatus && m_pLiveLayer)
	{
 		//m_pLiveLayer = getLayerServer()->addLiveLayer();
		m_pLiveLayer->setName(tr("tilt photography model").toUtf8().data());
		resize(320,380);
		setMinimumSize(320,380);
	}
	else
	{
		ui.m_pSaveBtn->hide();
		ui.m_pCancelBtn->hide();
		ui.line_3->hide();
		ui.line_6->hide();
		ui.horizontalLayout_4->setContentsMargins(8,0,8,6);
		resize(320,386);
		setMinimumSize(320,386);
	}

	//radioBtnChangedSlot();

	updateInfo();
}

CTiltModelWgt::~CTiltModelWgt()
{
}

void CTiltModelWgt::setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer)
{
	m_pLiveLayer = static_cast<HGxScene::CGxLayerLive*>(pLayer);
	updateInfo();
}

void CTiltModelWgt::initSignalAndSlot()
{
	connect(ui.m_pNameEdit, SIGNAL(textEdited(QString)), this, SLOT(nameChangedSlot(QString)));
	connect(ui.m_pNameEdit, SIGNAL(textEdited(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
	connect(ui.m_pDescribeEdt, SIGNAL(textChanged()), this, SLOT(descChangedSlot()));
	connect(ui.m_pLonSpx, SIGNAL(valueChanged(double)), this, SLOT(lonChangedSlot(double)));
	connect(ui.m_pLatSpx, SIGNAL(valueChanged(double)), this, SLOT(latChangedSlot(double)));
	connect(ui.m_pHeightSpx, SIGNAL(valueChanged(int)), this, SLOT(heightChangedSlot(int)));
	connect(ui.m_pPathLineEdit, SIGNAL(textEdited(QString)), this, SLOT(pathChangedSlot(QString)));
	connect(ui.m_pScaleSpx, SIGNAL(valueChanged(int)), this, SLOT(scaleChangedSlot(int)));
	connect(ui.m_pRotateSpx, SIGNAL(valueChanged(double)), this, SLOT(rotateChangedSlot(double)));

	connect(ui.filePath_RadioBtn, SIGNAL(clicked()), this, SLOT(radioBtnChangedSlot()));
	connect(ui.DirPath_RadioBtn, SIGNAL(clicked()), this, SLOT(radioBtnChangedSlot()));
	connect(ui.m_pPathToolBtn, SIGNAL(clicked()), this, SLOT(pathBtnChangedSlot()));
	connect(ui.m_pSaveBtn, SIGNAL(clicked()), this, SLOT(saveBtnClickedSlot()));
	connect(ui.m_pSaveBtn, SIGNAL(clicked()), this, SLOT(onCloseWindowSlot()));

	connect(ui.m_pCancelBtn, SIGNAL(clicked()), this, SLOT(cancleBtnClickedSlot()));
	connect(ui.m_pCancelBtn, SIGNAL(clicked()), this, SLOT(onCloseWindowSlot()));
	
	connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(cancleBtnClickedSlot()));
}

void CTiltModelWgt::setWidgetInfo()
{
	if (m_pLiveLayer)
	{
		ui.m_pNameEdit->setText(QString::fromUtf8(m_pLiveLayer->getName().c_str()));
		ui.m_pDescribeEdt->setText(QString::fromUtf8(m_pLiveLayer->getDescirbe().c_str()));
		ui.m_pLonSpx->setValue(m_pLiveLayer->getPosition().x());
		ui.m_pLatSpx->setValue(m_pLiveLayer->getPosition().y());
		ui.m_pHeightSpx->setValue(m_pLiveLayer->getPosition().z());
		ui.m_pPathLineEdit->setText(m_pLiveLayer->getTiltPath().c_str());
		ui.m_pScaleSpx->setValue(m_pLiveLayer->getScale().z());
		ui.m_pRotateSpx->setValue(m_pLiveLayer->getRotate().z());
	}
}

void CTiltModelWgt::BrowseConfigPath()
{
	QString strCaption = tr("open file");
	QString strDir = tr(HGX_FILE_CONFIG().getDataPath().c_str());
	QString filePath;
#ifdef WIN32
	QFileDialog* pFileDialog = new QFileDialog(0, strCaption, strDir, tr("model Files(*.*)"));
	pFileDialog->setAttribute(Qt::WA_DeleteOnClose, true);
	filePath = pFileDialog->getOpenFileName();
	if (!filePath.isNull())
	{
		ui.m_pPathLineEdit->setText(filePath);
	}
#else
	QFileDialog fileDialog(0, strCaption, strDir, tr("model Files(*.*)"));
	if (fileDialog.exec())
	{
		filePath = fileDialog.selectedFiles().first();
		ui.m_pPathLineEdit->setText(filePath);
	}
#endif

	m_pLiveLayer->setTiltPath(filePath.toStdString());
}

void CTiltModelWgt::BrowseDataPath()
{
 	// 读取数据路径
 	QString strCaption = tr("data path");
 	QString strDir = tr(HGX_FILE_CONFIG().getDataPath().c_str());
 	QString filePath;
 
 #ifdef WIN32
 	QFileDialog* pFileDialog = new QFileDialog(0, strCaption, strDir);
 	filePath = pFileDialog->getExistingDirectory(this, strCaption, strDir);
 	if(!filePath.isNull())
 	{
 		ui.m_pPathLineEdit->setText(filePath);
 	}
 #else
 	QFileDialog fileDialog(0, strCaption, strDir);
 	fileDialog.setFileMode( QFileDialog::DirectoryOnly);
 	if (fileDialog.exec())
 	{
 		filePath = fileDialog.selectedFiles().first();
 		ui.m_pNameEdit->setText(filePath);
 	}
 #endif
	
	m_pLiveLayer->setTiltPath(filePath.toStdString());
}

void CTiltModelWgt::pathBtnChangedSlot()
{
	radioBtnChangedSlot();
	if (ui.filePath_RadioBtn->isChecked())
	{
		BrowseConfigPath();
	}
	else if (ui.DirPath_RadioBtn->isChecked())
	{
		BrowseDataPath();
	}

	setWidgetInfo();
}

void CTiltModelWgt::saveBtnClickedSlot()
{
	if (m_bAddTreeStatus)
	{
		std::string strSceneID = "";

		ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
		if (pSceneService)
		{
			strSceneID = pSceneService->activeScene();
		}

		HGxScene::CGxLayerManager::getOrCreateLayerManager(strSceneID)->addLayer(m_pLiveLayer);
	}

	setTileModelShow(false);
}

void CTiltModelWgt::cancleBtnClickedSlot()
{
	if (m_bAddTreeStatus)
	{
		std::string strSceneID = "";

		ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
		if (pSceneService)
		{
			strSceneID = pSceneService->activeScene();
		}


		if (m_pLiveLayer)
 		{
			HGxScene::CGxLayerManager::getOrCreateLayerManager(strSceneID)->deleteLayer(m_pLiveLayer);
 		}
	}
	
	setTileModelShow(false);
}

void CTiltModelWgt::radioBtnChangedSlot()
{
	if (ui.filePath_RadioBtn->isChecked())
	{
		if (m_pLiveLayer->getType() != HGxScene::CGxLayerLive::E_TILT_TYPE_FILEPATH)
		{
			m_pLiveLayer->setTiltType(HGxScene::CGxLayerLive::E_TILT_TYPE_FILEPATH);
			ui.m_pPathLineEdit->clear();
		}
	}
	else if (ui.DirPath_RadioBtn->isChecked())
	{
		if (m_pLiveLayer->getType() != HGxScene::CGxLayerLive::E_TILT_TYPE_DTATPATH)
		{
			m_pLiveLayer->setTiltType(HGxScene::CGxLayerLive::E_TILT_TYPE_DTATPATH);
			ui.m_pPathLineEdit->clear();
		}
	}
}

void CTiltModelWgt::nameChangedSlot(QString strName)
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

void CTiltModelWgt::descChangedSlot()
{
 	std::string strDesc = ui.m_pDescribeEdt->toPlainText().toUtf8().data();

	m_pLiveLayer->setDesciribe(strDesc);
}

void CTiltModelWgt::lonChangedSlot( double dLon )
{
 	osg::Vec3d vecLLH = m_pLiveLayer->getPosition();
 	vecLLH.x() = dLon;
 	m_pLiveLayer->setPosition(vecLLH);
}

void CTiltModelWgt::latChangedSlot( double dLat )
{
 	osg::Vec3d vecLLH = m_pLiveLayer->getPosition();
 	vecLLH.y() = dLat;
 	m_pLiveLayer->setPosition(vecLLH);
}

void CTiltModelWgt::heightChangedSlot( int nHeight )
{
	osg::Vec3d vecLLH = m_pLiveLayer->getPosition();
	vecLLH.z() = nHeight;
	m_pLiveLayer->setPosition(vecLLH);
}

void CTiltModelWgt::scaleChangedSlot( int nScale )
{
	m_pLiveLayer->setScale(osg::Vec3d(nScale, nScale, nScale));
}

void CTiltModelWgt::rotateChangedSlot( double dRotate )
{
	m_pLiveLayer->setRotate(osg::Vec3d(m_pLiveLayer->getRotate().x(), m_pLiveLayer->getRotate().y(), dRotate));
}

void CTiltModelWgt::pathChangedSlot(QString strPath)
{
	if (ui.filePath_RadioBtn->isChecked())
	{
		m_pLiveLayer->setTiltType(HGxScene::CGxLayerLive::E_TILT_TYPE_FILEPATH);
	}
	else if (ui.DirPath_RadioBtn->isChecked())
	{
		m_pLiveLayer->setTiltType(HGxScene::CGxLayerLive::E_TILT_TYPE_DTATPATH);
	}

	m_pLiveLayer->setTiltPath(strPath.toStdString());

	setWidgetInfo();
}

void CTiltModelWgt::setTileModelShow( bool bStatus )
{
	m_bStatus = bStatus;
}

bool CTiltModelWgt::getTileModelShow()
{
	return m_bStatus;
}

void CTiltModelWgt::setTiltModelName( QString strName )
{
	ui.m_pNameEdit->setText(strName);
}

void CTiltModelWgt::updateInfo()
{
	if (m_pLiveLayer)
	{
		blockSignals(true);
		if (m_pLiveLayer->getType() == HGxScene::CGxLayerLive::E_TILT_TYPE_FILEPATH)
		{
			ui.filePath_RadioBtn->setChecked(true);
		}
		else
		{
			ui.DirPath_RadioBtn->setChecked(true);
		}
		ui.m_pNameEdit->setText(QString::fromUtf8(m_pLiveLayer->getName().c_str()));
		ui.m_pDescribeEdt->setPlainText(QString::fromUtf8(m_pLiveLayer->getDescirbe().c_str()));
		ui.m_pLonSpx->setValue(m_pLiveLayer->getPosition().x());
		ui.m_pLatSpx->setValue(m_pLiveLayer->getPosition().y());
		ui.m_pHeightSpx->setValue(m_pLiveLayer->getPosition().z());

		if (HGxScene::CGxLayerLive::E_TILT_TYPE_FILEPATH == m_pLiveLayer->getType())
		{
			ui.filePath_RadioBtn->setChecked(true);
		}
		else if (HGxScene::CGxLayerLive::E_TILT_TYPE_DTATPATH == m_pLiveLayer->getType())
		{
			ui.DirPath_RadioBtn->setChecked(true);
		}

		ui.m_pPathLineEdit->setText(m_pLiveLayer->getTiltPath().c_str());
		ui.m_pScaleSpx->setValue(m_pLiveLayer->getScale().z());
		ui.m_pRotateSpx->setValue(m_pLiveLayer->getRotate().z());
		blockSignals(false);
	}
}
