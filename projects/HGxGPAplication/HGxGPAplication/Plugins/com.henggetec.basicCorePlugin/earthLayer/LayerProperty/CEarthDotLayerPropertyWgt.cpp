#include <QFileDialog>
//#include <FreeXFramework/FreeXFramework.h>
#include <scene_ui/ISceneUiService.h>
//#include <scene/IFxSceneService.h>
//#include <map/IMapManipulatorManager.h>
//#include <earth/IFxEarthManipulatorManager.h>
//#include <earth/IFxEarthScene.h>
#include <FxToast/CToast.h>
#include <QScrollBar>

#include <earthLayer/LayerProperty/CEarthDotLayerPropertyWgt.h>
#include <bdpCommon/GxStrUtil.h>

#include <bdpDB/GxRegister.h>

#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/manipulator/GxManipulatorManager.h>
#include <bdpScene/manipulator/GxBaseManipulator.h>

#include <gdpScene/CommonGeo.h>
#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerModel.h>
#include <gdpScene/scene/GxGeoContext.h>

#include <osgEarth/Viewpoint>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

#define MAX_VISIBLE_VAL FLT_MAX

CEarthDotLayerPropertyWgt::CEarthDotLayerPropertyWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	ui.pFontCBBox->addItem(QStringLiteral("宋体"), QVariant::fromValue(QString("fonts/simsun.ttc")));
	ui.pFontCBBox->addItem(QStringLiteral("黑体"), QVariant::fromValue(QString("fonts/simhei.ttf")));
	ui.pFontCBBox->addItem(QStringLiteral("华文楷体"), QVariant::fromValue(QString("fonts/hwkaiti.ttf")));
	ui.pFontCBBox->addItem(QStringLiteral("微软雅黑"), QVariant::fromValue(QString("fonts/msyh.ttf")));
	ui.pFontCBBox->addItem(QStringLiteral("英文字体"), QVariant::fromValue(QString("fonts/arial.ttf")));

	ui.pIconPathTBtn->setToolTip(tr("browse"));

	QRegExp rx("\\S{0,20}");
	ui.pLayerNameEdit->setValidator(new QRegExpValidator(rx, this));

	initSignalAndSlots();

	QFile file(":/qss_layerauxiliary/Resources/qss/layerproperty.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(styleSheet().append(file.readAll()));
	}
}

CEarthDotLayerPropertyWgt::~CEarthDotLayerPropertyWgt()
{

}

void CEarthDotLayerPropertyWgt::setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer)
{
	m_pModelLayer = static_cast<HGxScene::CGxLayerModel *>(pLayer);

	ui.pLayerNameEdit->blockSignals(true);
	ui.pLayerPathEdit->blockSignals(true);
	ui.pAllVisiblePushBtn->blockSignals(true);
	ui.pDotVisiblePushBtn->blockSignals(true);
	ui.pMinVisibleRangeSpx->blockSignals(true);
	ui.pMaxVisibleRangeSpx->blockSignals(true);
	ui.pFontCBBox->blockSignals(true);
	ui.pFontSizeSpx->blockSignals(true);
	ui.pFontColorWgt->blockSignals(true);
	ui.pIconPathEdit->blockSignals(true);
	ui.pIconSizeSpx->blockSignals(true);

 	// 图层名称
 	ui.pLayerNameEdit->setText(QString::fromUtf8(m_pModelLayer->getName().c_str()));
	// 数据路径
 	ui.pLayerPathEdit->setText(QString::fromUtf8(m_pModelLayer->getLayerConfig()->sourceConfig().url().value().c_str()));

	// 可见范围
	float fMinRange = 0.0, fMaxRange = 0.0;

	m_pModelLayer->getVisibleRange(fMinRange, fMaxRange);

	if (fMinRange > 0.0 || fMaxRange < MAX_VISIBLE_VAL)
	{
		//视点范围内可见
		ui.pAllVisiblePushBtn->setChecked(false);
		ui.pMinVisibleRangeSpx->setEnabled(true);
		ui.pMaxVisibleRangeSpx->setEnabled(true);
		ui.pMinToolBtn->setEnabled(true);
		ui.pMaxToolBtn->setEnabled(true);
		ui.pMinVisibleRangeSpx->setValue(fMinRange);
		ui.pMaxVisibleRangeSpx->setValue(fMaxRange);
	}
	else
	{
		//所有范围内可见
		ui.pAllVisiblePushBtn->setChecked(true);
		ui.pMinVisibleRangeSpx->setEnabled(false);
		ui.pMaxVisibleRangeSpx->setEnabled(false);
		ui.pMinToolBtn->setEnabled(false);
		ui.pMaxToolBtn->setEnabled(false);
	}
	// 字体类型
	for (int i = 0; i < ui.pFontCBBox->count(); i++)
	{
		QString strFontType = ui.pFontCBBox->itemData(i).toString();

		if (strFontType.split('/').last() == QString::fromUtf8(m_pModelLayer->getFontName().data()).split('/').last())
		{
			ui.pFontCBBox->setCurrentIndex(i);
			break;
		}
 	}

 	// 字体大小
 	ui.pFontSizeSpx->setValue(m_pModelLayer->getFontSize());
 	// 字体颜色
 	osg::Vec4f osgFontColor = m_pModelLayer->getFontColor();
 	QColor fontColor = QColor::fromRgbF(osgFontColor.r(), osgFontColor.g(), osgFontColor.b(), osgFontColor.a());
 	ui.pFontColorWgt->setCurrentColor(fontColor);
 	// 图标路径
 	ui.pIconPathEdit->setText(QString::fromUtf8(m_pModelLayer->getPointIconPath().c_str()));
 	// 图标大小
 	ui.pIconSizeSpx->setValue(m_pModelLayer->getPointIconSize());

	ui.pLayerNameEdit->blockSignals(false);
	ui.pLayerPathEdit->blockSignals(false);
	ui.pAllVisiblePushBtn->blockSignals(false);
	ui.pDotVisiblePushBtn->blockSignals(false);
	ui.pMinVisibleRangeSpx->blockSignals(false);
	ui.pMaxVisibleRangeSpx->blockSignals(false);
	ui.pFontCBBox->blockSignals(false);
	ui.pFontSizeSpx->blockSignals(false);
	ui.pFontColorWgt->blockSignals(false);
	ui.pIconPathEdit->blockSignals(false);
	ui.pIconSizeSpx->blockSignals(false);
}

HGxScene::CGxGlobalLayer* CEarthDotLayerPropertyWgt::getCurrentLayer()
{
	return m_pModelLayer;
}

//初始化信号和槽函数
void CEarthDotLayerPropertyWgt::initSignalAndSlots()
{
	connect(ui.pLayerNameEdit, SIGNAL(textChanged(QString)), this, SLOT(layerNameChangedSlot(const QString &)));	
	connect(ui.pAllVisiblePushBtn, SIGNAL(toggled(bool)), this, SLOT(allViewScopeChangedSlot(bool)));
	connect(ui.pMinVisibleRangeSpx, SIGNAL(valueChanged(double)), this, SLOT(viewScopeChangedSlot()));
	connect(ui.pMaxVisibleRangeSpx, SIGNAL(valueChanged(double)), this, SLOT(viewScopeChangedSlot()));
	connect(ui.pFontCBBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fontFamilyChangedSlot()));
	connect(ui.pFontSizeSpx, SIGNAL(valueChanged(int)), this, SLOT(fontSizeChangedSlot(int)));
	connect(ui.pFontColorWgt, SIGNAL(colorChangedSignal(const QColor &)), this, SLOT(fontColorChangedSlot(const QColor &)));
	connect(ui.pIconPathEdit, SIGNAL(textChanged(const QString &)), this, SLOT(iconPathChangedSlot(const QString &)));
	connect(ui.pIconSizeSpx, SIGNAL(valueChanged(int)), this, SLOT(iconSizeChangedSlot(int)));
	connect(ui.pIconPathTBtn, SIGNAL(pressed()),this, SLOT(selectIconPathSlot()));
	connect(ui.pMinToolBtn, SIGNAL(clicked()), this, SLOT(getCurrentViewScopeSlot()));
	connect(ui.pMaxToolBtn, SIGNAL(clicked()), this, SLOT(getCurrentViewScopeSlot()));
}

// 选择图标
void CEarthDotLayerPropertyWgt::selectIconPathSlot()
{
#if WIN32
	QString strFilePath = QFileDialog::getOpenFileName(this, tr("open file"), HGX_FILE_CONFIG().getDataPath().c_str(), tr("*.*"));
#else
	QString strFilePath = QFileDialog::getOpenFileName(0, tr("open file"), HGX_FILE_CONFIG().getDataPath().c_str(), tr("*.*"));
#endif

	if (!strFilePath.isEmpty())
	{
		ui.pIconPathEdit->setText(strFilePath);
	}
}

void CEarthDotLayerPropertyWgt::getCurrentViewScopeSlot()
{
	QToolButton *pToolBtn = qobject_cast<QToolButton *>(QObject::sender());

	HGxScene::CGxSceneBase *pScene = getCurrentScene();

	if (NULL == pScene)
	{
		return;
	}


	HGxScene::CGxGeoContext *pGeoContext = dynamic_cast<HGxScene::CGxGeoContext*> (pScene->getSceneContext());
	if (!pGeoContext)
	{
		return;
	}

	
 	// 获取视点
 	osgEarth::Viewpoint v;
	double dCurrentViewRange = 0.0;

	if (QString(pScene->getObjectType()) == GX_EARTH_SCENE_TYPE)
	{
		HGxScene::CGxManipulatorManager* pMapManipulatorMgr = getCurrentScene()->getManipulatorSystem();

		osgGA::CameraManipulator* pCameraManip = dynamic_cast<osgGA::CameraManipulator*>(pMapManipulatorMgr->getCurrentManipulator());

		if (pCameraManip)
		{
			osg::Vec3f eye, center, up;
			pCameraManip->getInverseMatrix().getLookAt(eye, center, up);
			osgEarth::GeoPoint mapPoint;
			mapPoint.fromWorld(pGeoContext->getMapSRS(), eye);
			dCurrentViewRange = mapPoint.z();
		}
	}

	if (pToolBtn)
	{
		// 调用底层接口获取当前视点范围
		if (pToolBtn->objectName() == "pMinToolBtn")
		{
			double dMaxViewRange = ui.pMaxVisibleRangeSpx->value();
			if (dCurrentViewRange <= dMaxViewRange)
			{
				ui.pMinVisibleRangeSpx->blockSignals(true);
				ui.pMinVisibleRangeSpx->setValue(dCurrentViewRange);
				double dValue = ui.pMinVisibleRangeSpx->value();
				if (dValue > dCurrentViewRange)
				{
					dCurrentViewRange = dCurrentViewRange - 0.05;
				}

				dCurrentViewRange = osg::round(dCurrentViewRange * 10) / 10.0;
				ui.pMinVisibleRangeSpx->setValue(dCurrentViewRange);
				m_pModelLayer->setVisibleRange(dCurrentViewRange, dMaxViewRange);
				ui.pMinVisibleRangeSpx->blockSignals(false);
			}
		}
		else if (pToolBtn->objectName() == "pMaxToolBtn")
		{
			double dMinViewRange = ui.pMinVisibleRangeSpx->value();
			if (dCurrentViewRange >= dMinViewRange)
			{
				ui.pMaxVisibleRangeSpx->blockSignals(true);
				ui.pMaxVisibleRangeSpx->setValue(dCurrentViewRange);
				double dValue = ui.pMaxVisibleRangeSpx->value();
				if (dValue < dCurrentViewRange)
				{
					dCurrentViewRange = dCurrentViewRange + 0.05;
				}

				dCurrentViewRange = osg::round(dCurrentViewRange * 10) / 10.0;
				ui.pMaxVisibleRangeSpx->setValue(dCurrentViewRange);
				m_pModelLayer->setVisibleRange(dMinViewRange, dCurrentViewRange);
				ui.pMaxVisibleRangeSpx->blockSignals(false);
			}
		}
	}
}

HGxScene::CGxSceneBase* CEarthDotLayerPropertyWgt::getCurrentScene()
{
	std::string strSceneID = "";

	ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
	if (pSceneService)
	{
		strSceneID = pSceneService->activeScene();
	}

	HGxScene::CGxSceneBase* pScene = HGxScene::CGxControllerMgr::getInstance()->getScene(strSceneID);

	return pScene;
}

void CEarthDotLayerPropertyWgt::layerNameChangedSlot(const QString &strLayerName)
{
	m_pModelLayer->setName(strLayerName.toUtf8().data());
	emit layerNameChangedSignal(strLayerName);
}

void CEarthDotLayerPropertyWgt::dataPathChangedSlot(const QString &strDataPath)
{

}

void CEarthDotLayerPropertyWgt::allViewScopeChangedSlot(bool bChecked)
{
	if (bChecked)
	{
		m_pModelLayer->setVisibleRange(0, MAX_VISIBLE_VAL);

		// 重置最大最小视点
		ui.pMinVisibleRangeSpx->blockSignals(true);
		ui.pMaxVisibleRangeSpx->blockSignals(true);

		ui.pMinVisibleRangeSpx->setValue(0);
		ui.pMaxVisibleRangeSpx->setValue(500000000.000000);

		ui.pMinVisibleRangeSpx->setEnabled(false);
		ui.pMaxVisibleRangeSpx->setEnabled(false);

		ui.pMinVisibleRangeSpx->blockSignals(false);
		ui.pMaxVisibleRangeSpx->blockSignals(false);
	}
	else
	{
		ui.pMinVisibleRangeSpx->blockSignals(true);
		ui.pMaxVisibleRangeSpx->blockSignals(true);
		ui.pMinVisibleRangeSpx->setEnabled(true);
		ui.pMaxVisibleRangeSpx->setEnabled(true);
		ui.pMinToolBtn->setEnabled(true);
		ui.pMaxToolBtn->setEnabled(true);
		viewScopeChangedSlot();
		ui.pMinVisibleRangeSpx->blockSignals(false);
		ui.pMaxVisibleRangeSpx->blockSignals(false);
	}
}

void CEarthDotLayerPropertyWgt::viewScopeChangedSlot()
{
	double dMin = ui.pMinVisibleRangeSpx->value();
	double dMax = ui.pMaxVisibleRangeSpx->value();
	//最小范围小于最大范围
	if (dMin < dMax - 0.000001)
	{
		m_lastMinScaleValue = dMin;
		m_lastMaxScaleValue = dMax;
	}
	else
	{
		//恢复原值
		ui.pMinVisibleRangeSpx->blockSignals(true);
		ui.pMaxVisibleRangeSpx->blockSignals(true);
		ui.pMinVisibleRangeSpx->setValue(m_lastMinScaleValue);
		ui.pMaxVisibleRangeSpx->setValue(m_lastMaxScaleValue);
		ui.pMinVisibleRangeSpx->blockSignals(false);
		ui.pMaxVisibleRangeSpx->blockSignals(false);
	}

	m_pModelLayer->setVisibleRange(dMin, dMax);
}

void CEarthDotLayerPropertyWgt::fontFamilyChangedSlot()
{
	int nIndex = ui.pFontCBBox->currentIndex();

	std::string strFontFamily = HGX_FILE_CONFIG().getFullPath(ui.pFontCBBox->itemData(nIndex).toString().toStdString());
	m_pModelLayer->setFontName(strFontFamily);
}

void CEarthDotLayerPropertyWgt::fontSizeChangedSlot(int iSize)
{
	m_pModelLayer->setFontSize(iSize);
}

void CEarthDotLayerPropertyWgt::fontColorChangedSlot(const QColor & )
{
	QColor curColor = ui.pFontColorWgt->getCurrentColor();
	osg::Vec4f fontColor(curColor.redF(), curColor.greenF(), curColor.blueF(), curColor.alphaF());
	m_pModelLayer->setFontColor(fontColor);
}

void CEarthDotLayerPropertyWgt::iconPathChangedSlot(const QString &strPath)
{
	m_pModelLayer->setPointIconPath(strPath.toUtf8().data());
}

void CEarthDotLayerPropertyWgt::iconSizeChangedSlot(int iIconSize)
{
	m_pModelLayer->setPointIconSize(iIconSize);
}
