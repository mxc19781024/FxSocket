//#include <FreeXFramework/FreeXFramework.h>
#include <scene_ui/ISceneUiService.h>
//#include <scene/IFxSceneService.h>
//#include <map/IMapManipulatorManager.h>
//#include <earth/IFxEarthManipulatorManager.h>
//#include <earth/IFxEarthScene.h>
#include <QScrollBar>

#include "earthLayer/LayerProperty/CEarthLineLayerPropertyWgt.h"
//#include <osgEarth/GeoData>

#include <osgEarth/Viewpoint>

#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/manipulator/GxManipulatorManager.h>
#include <bdpScene/manipulator/GxBaseManipulator.h>

#include <gdpScene/CommonGeo.h>
#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerModel.h>
#include <gdpScene/scene/GxGeoContext.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

#define MAX_VISIBLE_VAL FLT_MAX

CEarthLineLayerPropertyWgt::CEarthLineLayerPropertyWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);	
	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);

	QRegExp rx("\\S{0,20}");
	ui.pLayerNameEdit->setValidator(new QRegExpValidator(rx, this));

	initSignalAndSlots();

	QFile file(":/qss_layerauxiliary/Resources/qss/layerproperty.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(styleSheet().append(file.readAll()));
	}
}

CEarthLineLayerPropertyWgt::~CEarthLineLayerPropertyWgt()
{

}

void CEarthLineLayerPropertyWgt::setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer)
{
	m_pModelLayer = static_cast<HGxScene::CGxLayerModel*>(pLayer);

	ui.pLayerNameEdit->blockSignals(true);
	ui.pAllVisiblePushBtn->blockSignals(true); 
	ui.pMinVisibleRangeSpx->blockSignals(true);
	ui.pMaxVisibleRangeSpx->blockSignals(true);
	ui.pLineColorWgt->blockSignals(true);
	ui.pLineOpacitySlider->blockSignals(true); 
	ui.pLineWidthSpx->blockSignals(true);
	ui.pLineTypeCBBox->blockSignals(true);

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

 	// 线色
 	osg::Vec4f osgColor = m_pModelLayer->getLineColor();
 	QColor lineColor = QColor::fromRgbF(osgColor.r(), osgColor.g(), osgColor.b()/*, osgColor.a()*/);
 	ui.pLineColorWgt->setCurrentColor(lineColor);
 	// 不透明度
 	ui.pLineOpacitySlider->setPercentSliderValue(qRound(osgColor.a() * 100));
 	// 线宽
 	ui.pLineWidthSpx->setValue(m_pModelLayer->getLineWidth());
 	// 线型
 	ui.pLineTypeCBBox->setLineStyle(m_pModelLayer->getLineStipple());

	ui.pLayerNameEdit->blockSignals(false);
	ui.pAllVisiblePushBtn->blockSignals(false); 
	ui.pMinVisibleRangeSpx->blockSignals(false);
	ui.pMaxVisibleRangeSpx->blockSignals(false);
	ui.pLineColorWgt->blockSignals(false);
	ui.pLineOpacitySlider->blockSignals(false); 
	ui.pLineWidthSpx->blockSignals(false);
	ui.pLineTypeCBBox->blockSignals(false);
}

HGxScene::CGxGlobalLayer* CEarthLineLayerPropertyWgt::getCurrentLayer()
{
	return m_pModelLayer;
}

//初始化信号和槽函数
void CEarthLineLayerPropertyWgt::initSignalAndSlots()
{
	connect(ui.pLayerNameEdit, SIGNAL(textChanged(QString)), this, SLOT(layerNameChangedSlot(const QString &)));	
	connect(ui.pAllVisiblePushBtn, SIGNAL(toggled(bool)), this, SLOT(allViewScopeChangedSlot(bool)));
	connect(ui.pMinVisibleRangeSpx, SIGNAL(valueChanged(double)), this, SLOT(viewScopeChangedSlot()));
	connect(ui.pMaxVisibleRangeSpx, SIGNAL(valueChanged(double)), this, SLOT(viewScopeChangedSlot()));
	connect(ui.pLineColorWgt, SIGNAL(colorChangedSignal(const QColor &)), this, SLOT(lineColorChangedSlot(const QColor &)));
	connect(ui.pLineOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(transparencyChangedSlot(int)));
	connect(ui.pLineWidthSpx, SIGNAL(valueChanged(int)), this, SLOT(lineWidthChangedSlot(int)));
	connect(ui.pLineTypeCBBox, SIGNAL(currentIndexChanged(int)), this, SLOT(lineTypeChangedSlot(int)));
	connect(ui.pMinToolBtn, SIGNAL(clicked()), this, SLOT(getCurrentViewScopeSlot()));
	connect(ui.pMaxToolBtn, SIGNAL(clicked()), this, SLOT(getCurrentViewScopeSlot()));
}

void CEarthLineLayerPropertyWgt::getCurrentViewScopeSlot()
{
	QToolButton *pToolBtn = qobject_cast<QToolButton *>(QObject::sender());

 	HGxScene::CGxSceneBase* pScene = getCurrentScene();

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
 			osg::Vec3f eye,center,up;
 			pCameraManip->getInverseMatrix().getLookAt(eye,center,up);
 			osgEarth::GeoPoint mapPoint;
 			mapPoint.fromWorld(pGeoContext->getMapSRS() , eye );
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

HGxScene::CGxSceneBase* CEarthLineLayerPropertyWgt::getCurrentScene()
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

void CEarthLineLayerPropertyWgt::layerNameChangedSlot(const QString &strLayerName)
{
	m_pModelLayer->setName(strLayerName.toUtf8().data());
	emit layerNameChangedSignal(strLayerName);
}

void CEarthLineLayerPropertyWgt::dataPathChangedSlot(const QString &strDataPath)
{

}

void CEarthLineLayerPropertyWgt::allViewScopeChangedSlot(bool bChecked)
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

void CEarthLineLayerPropertyWgt::viewScopeChangedSlot()
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

	m_pModelLayer->setVisibleRange(dMin , dMax );
}

void CEarthLineLayerPropertyWgt::lineColorChangedSlot(const QColor &lineColor)
{
	QColor curColor = ui.pLineColorWgt->getCurrentColor();
	osg::Vec4f osgLineColor(curColor.redF(), curColor.greenF(), curColor.blueF(), curColor.alphaF());
	m_pModelLayer->setLineColor(osgLineColor);
}

void CEarthLineLayerPropertyWgt::transparencyChangedSlot(int iTranparency)
{
	QColor curColor = ui.pLineColorWgt->getCurrentColor();
	double dfTran = iTranparency * 1.0 / 100;

	osg::Vec4f lineColor(curColor.redF(), curColor.greenF(), curColor.blueF(), dfTran);
	m_pModelLayer->setLineColor(lineColor);
}

void CEarthLineLayerPropertyWgt::lineWidthChangedSlot(int iLineWidth)
{
	m_pModelLayer->setLineWidth(iLineWidth);
}

void CEarthLineLayerPropertyWgt::lineTypeChangedSlot(int iLineType)
{
	m_pModelLayer->setLineStipple(ui.pLineTypeCBBox->lineStyle());
}
