#include "earthLayer/LayerProperty/CTiltPropertyWgt.h"
//#include <FreeXCommonUtils/PathRegistry.h>
//#include <scene/SceneServiceUtil.h>
//#include <FreeXFramework/FreeXFramework.h>
//#include <scene/IFxSceneService.h>
//#include <FreeXGeoUtil/CoordConverter.h>
#include <QFileDialog>
#include <QFile>
#include "scene_ui/ISceneUiService.h"
#include "earth_layer_ui/IEarthLayerUiService.h"
//#include "ICore.h"
//#include "CToolManager.h"
#include <QScrollBar>

#include "bdpDB/GxRegister.h"

#include "gdpScene/layer/GxGlobalLayer.h"
#include "gdpScene/layer/GxLayerLive.h"

CTiltPropertyWgt::CTiltPropertyWgt(HGxScene::CGxGlobalLayer* pLayer, QWidget *parent)
	: QWidget(parent)
	, m_pLiveLayer(NULL)
	, m_bStatus(false)
	,m_bIsLLH(true)
    ,m_bIsPixs(true)
{
    ui.setupUi(this);
	
	//setWindowModality(Qt::NonModal);
	//setWindowTitle(tr("tilt photography model property"));
	QRegExp rx("\\S{0,20}");
	ui.m_pNameEdit->setValidator(new QRegExpValidator(rx, this));
	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.m_pDescribeEdt->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.m_pDescribeEdt->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	
	m_pLiveLayer = static_cast<HGxScene::CGxLayerLive *>(pLayer);

	//setWidgetInfo();

	initSignalAndSlot();

	//读样式表文件
	QFile file(":/qss_earth_layer/Resources/qss_earth_layer/earthlayerproperty.css");
	if (file.open(QIODevice::ReadOnly| QIODevice::Text))
	{
		QString str = file.readAll();
		setStyleSheet(styleSheet().append(str));
		file.close();
	}
}

CTiltPropertyWgt::~CTiltPropertyWgt()
{
}

HGxScene::CGxGlobalLayer* CTiltPropertyWgt::getCurrentLayer()
{
	return m_pLiveLayer;
}

void CTiltPropertyWgt::setWidgetInfo()
{
	if (m_pLiveLayer)
	{
		blockSignals(true);
		ui.m_pNameEdit->setText(QString::fromUtf8(m_pLiveLayer->getName().c_str()));
		ui.m_pDescribeEdt->setPlainText(QString::fromUtf8(m_pLiveLayer->getDescirbe().c_str()));
		ui.m_pPathLineEdit->setText(m_pLiveLayer->getTiltPath().c_str());
		if (m_pLiveLayer->isLLHAxis()) {
			ui.m_pChangeAxisLabel->setText(tr("geographical coordinates"));
			ui.stackedWidget->setCurrentIndex(0);
			ui.stackedWidget->setMaximumHeight(135);
			ui.m_pLonSpx->setValue(m_pLiveLayer->getPosition().x());
			ui.m_pLatSpx->setValue(m_pLiveLayer->getPosition().y());
			ui.m_pHeightSpx->setValue(m_pLiveLayer->getPosition().z());
			ui.m_pScaleSpx->setValue(m_pLiveLayer->getScale().x());
			ui.m_pRotateSpx->setValue(m_pLiveLayer->getRotate().z());
		}
		else
		{
			ui.m_pChangeAxisLabel->setText(tr("world coordinates"));
			ui.stackedWidget->setCurrentIndex(1);
			ui.stackedWidget->setMaximumHeight(22);
			ui.m_pHighSpx->setValue(m_pLiveLayer->getXYZDisHeight());
		}
		blockSignals(false);
	}
}


void CTiltPropertyWgt::initSignalAndSlot()
{
	connect(ui.m_pNameEdit, SIGNAL(textEdited(QString)), this, SLOT(nameChangedSlot(QString)));
	connect(ui.m_pDescribeEdt, SIGNAL(textChanged()), this, SLOT(descChangedSlot()));
	connect(ui.m_pLonSpx, SIGNAL(valueChanged(double)), this, SLOT(lonChangedSlot(double)));
	connect(ui.m_pLatSpx, SIGNAL(valueChanged(double)), this, SLOT(latChangedSlot(double)));
	connect(ui.m_pHeightSpx, SIGNAL(valueChanged(int)), this, SLOT(heightChangedSlot(int)));
	connect(ui.m_pScaleSpx, SIGNAL(valueChanged(double)), this, SLOT(scaleChangedSlot(double)));
	connect(ui.m_pRotateSpx, SIGNAL(valueChanged(double)), this, SLOT(rotateChangedSlot(double)));

	connect(ui.m_pHighSpx, SIGNAL(valueChanged(double)), this, SLOT(highChangedSlot(double)));
//	connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(cancleBtnClickedSlot()));
}


void CTiltPropertyWgt::BrowseConfigPath()
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

void CTiltPropertyWgt::BrowseDataPath()
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

void CTiltPropertyWgt::highChangedSlot(double dHigh)
{
	m_pLiveLayer->setXYZDisHeight(dHigh);
}
void CTiltPropertyWgt::nameChangedSlot(QString strName)
{
	if (m_pLiveLayer)
	{
		if (strName.isEmpty())
		{
			m_pLiveLayer->setName(tr("tilt photography model").toUtf8().data());
			emit layerNameChangedSignal(tr("tilt photography model"));
		}
		else
		{
			m_pLiveLayer->setName(strName.toUtf8().data());
			emit layerNameChangedSignal(strName);
		}
	}
}

void CTiltPropertyWgt::descChangedSlot()
{
	std::string strDesc = ui.m_pDescribeEdt->toPlainText().toUtf8().data();

	m_pLiveLayer->setDesciribe(strDesc);
}

void CTiltPropertyWgt::lonChangedSlot( double dLon )
{
 	osg::Vec3d vecLLH = m_pLiveLayer->getPosition();
 	vecLLH.x() = dLon;
 	m_pLiveLayer->setPosition(vecLLH);
}

void CTiltPropertyWgt::latChangedSlot( double dLat )
{
 	osg::Vec3d vecLLH = m_pLiveLayer->getPosition();
 	vecLLH.y() = dLat;
 	m_pLiveLayer->setPosition(vecLLH);
}

void CTiltPropertyWgt::heightChangedSlot( int nHeight )
{
 	osg::Vec3d vecLLH = m_pLiveLayer->getPosition();
 	vecLLH.z() = nHeight;
 	m_pLiveLayer->setPosition(vecLLH);
}

void CTiltPropertyWgt::scaleChangedSlot( double nScale )
{
	m_pLiveLayer->setScale(osg::Vec3d(nScale, nScale, nScale));
}

void CTiltPropertyWgt::rotateChangedSlot( double dRotate )
{
	m_pLiveLayer->setRotate(osg::Vec3d(m_pLiveLayer->getRotate().x(), m_pLiveLayer->getRotate().y(), dRotate));
}

void CTiltPropertyWgt::setTileModelShow( bool bStatus )
{
	m_bStatus = bStatus;
}

bool CTiltPropertyWgt::getTileModelShow()
{
	return m_bStatus;
}

void CTiltPropertyWgt::setTiltModelName( QString strName )
{
	ui.m_pNameEdit->setText(strName);
}
void CTiltPropertyWgt::setFeLiveLayer(HGxScene::CGxGlobalLayer* pLayer)
{
	m_pLiveLayer = static_cast<HGxScene::CGxLayerLive *>(pLayer);
	setWidgetInfo();
}
