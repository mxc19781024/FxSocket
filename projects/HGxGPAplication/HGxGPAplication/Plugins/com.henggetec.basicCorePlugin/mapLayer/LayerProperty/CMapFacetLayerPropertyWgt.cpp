//#include <FreeXFramework/FreeXFramework.h>
#include <scene_ui/ISceneUiService.h>
//#include <scene/IFxSceneService.h>
//#include <map/IMapManipulatorManager.h>
//#include <map/IFxMapScene.h>
//#include <earth/IFxEarthManipulatorManager.h>
#include <qscrollbar.h>
#include "osg/Math"
#include <qscrollbar.h>
#include <layer_ui/CLayerManager.h>
//#include <scene/IFxScene.h>

#include "CMapFacetLayerPropertyWgt.h"

#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerModel.h>
#include <gdpScene/manipulator/GxMapManipulator.h>

#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/manipulator/GxManipulatorManager.h>
#include <bdpScene/scene/GxControlManager.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

#define MAX_VISIBLE_VAL FLT_MAX

CMapFacetLayerPropertyWgt::CMapFacetLayerPropertyWgt(QWidget *parent)
	: QWidget(parent)
	, m_lastMinScaleValue(500)
	, m_lastMaxScaleValue(100000000)
{
	ui.setupUi(this);

	initScaleRange(ui.pMinVisibleCBBox);
	initScaleRange(ui.pMaxVisibleCBBox);

	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);

	ui.pMinVisibleCBBox->setCurrentIndex(0);
	ui.pMaxVisibleCBBox->setCurrentIndex(13);
	ui.pMinVisibleCBBox->setEditable(true);
	ui.pMaxVisibleCBBox->setEditable(true);

	ui.pMinVisibleCBBox->setValidator(new QRegExpValidator(QRegExp("^[0-9:]+$"), this));
	ui.pMaxVisibleCBBox->setValidator(new QRegExpValidator(QRegExp("^[0-9:]+$"), this));
	ui.pMinVisibleCBBox->setAutoCompletion(false);
	ui.pMaxVisibleCBBox->setAutoCompletion(false);

	QRegExp rx("\\S{0,20}");
	ui.pLayerNameEdit->setValidator(new QRegExpValidator(rx, this));

	initSignalAndSlots();

	QFile file(":/qss_layerauxiliary/Resources/qss/layerproperty.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(styleSheet().append(file.readAll()));
	}
}

CMapFacetLayerPropertyWgt::~CMapFacetLayerPropertyWgt()
{

}

void CMapFacetLayerPropertyWgt::setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer)
{
	m_pModelLayer = static_cast<HGxScene::CGxLayerModel *>(pLayer);

	ui.pLayerNameEdit->blockSignals(true);
	ui.pLayerPathEdit->blockSignals(true);
	ui.pAllVisible2DPushBtn->blockSignals(true);
	ui.pDotVisible2DPushBtn->blockSignals(true);
	ui.pMinVisibleCBBox->blockSignals(true);
	ui.pMaxVisibleCBBox->blockSignals(true);
	ui.pLineColorWgt->blockSignals(true);
	ui.pLineOpacitySlider->blockSignals(true); 
	ui.pLineWidthSpx->blockSignals(true);
	ui.pLineTypeCBBox->blockSignals(true);
	ui.pFillColorWgt->blockSignals(true);
	ui.pFillOpacitySlider->blockSignals(true);

 	// 图层名称
 	ui.pLayerNameEdit->setText(QString::fromUtf8(m_pModelLayer->getName().c_str()));
 	// 数据路径
 	ui.pLayerPathEdit->setText(QString::fromUtf8(m_pModelLayer->getLayerConfig()->sourceConfig().url().value().c_str()));
	// 可见范围
	float fMinRange = 0.0, fMaxRange = 0.0;

	m_pModelLayer->getVisibleRange(fMinRange, fMaxRange);

	if (fMinRange > 0.0 || fMaxRange < MAX_VISIBLE_VAL)
	{
		ui.pAllVisible2DPushBtn->setChecked(false);
		ui.pDotVisible2DPushBtn->setChecked(true);
		ui.pMinVisibleCBBox->setEnabled(true);
		ui.pMaxVisibleCBBox->setEnabled(true);
		ui.pMin2DToolBtn->setEnabled(true);
		ui.pMax2DToolBtn->setEnabled(true);

 		HGxScene::CGxSceneBase* pScene = getCurrentScene();
 		if (pScene)
 		{
			HGxScene::CGxMapManipulator* pMapMani = static_cast<HGxScene::CGxMapManipulator*>(pScene->getManipulatorSystem()->getCurrentManipulator());

			if (pMapMani)
			{
 				//+0.5实现四舍五入
 				ui.pMinVisibleCBBox->setEditText(QString("1:%1").arg((int)(pMapMani->getRange(fMinRange) + 0.5)));
 				ui.pMaxVisibleCBBox->setEditText(QString("1:%1").arg((int)(pMapMani->getRange(fMaxRange) + 0.5)));
 			}	
 		}
	}
	else
	{
		ui.pAllVisible2DPushBtn->setChecked(true);
		ui.pDotVisible2DPushBtn->setChecked(false);
		ui.pMinVisibleCBBox->setEnabled(false);
		ui.pMaxVisibleCBBox->setEnabled(false);
		ui.pMin2DToolBtn->setEnabled(false);
		ui.pMax2DToolBtn->setEnabled(false);
	}

	// 线色
 	osg::Vec4f osgColor = m_pModelLayer->getLineColor();
 	QColor lineColor = QColor::fromRgbF(osgColor.r(), osgColor.g(), osgColor.b(), 1);
 	ui.pLineColorWgt->setCurrentColor(lineColor);
 	// 不透明度
 	ui.pLineOpacitySlider->setPercentSliderValue(qRound(osgColor.a() * 100));
 	// 线宽
 	ui.pLineWidthSpx->setValue(m_pModelLayer->getLineWidth());
 	// 线型
 	ui.pLineTypeCBBox->setLineStyle(m_pModelLayer->getLineStipple());
 	// 填充色
 	osg::Vec4f osgFillColor = m_pModelLayer->getFillColor();
 	QColor fillColor = QColor::fromRgbF(osgFillColor.r(), osgFillColor.g(), osgFillColor.b(), 1);
 	ui.pFillColorWgt->setCurrentColor(fillColor);
 	// 填充不透明度
 	int iTransparency = (qRound(osgFillColor.a() * 100));
 	ui.pFillOpacitySlider->setPercentSliderValue(iTransparency);

	ui.pLayerNameEdit->blockSignals(false);
	ui.pLayerPathEdit->blockSignals(false);
	ui.pAllVisible2DPushBtn->blockSignals(false);
	ui.pDotVisible2DPushBtn->blockSignals(false);
	ui.pMinVisibleCBBox->blockSignals(false);
	ui.pMaxVisibleCBBox->blockSignals(false);
	ui.pLineColorWgt->blockSignals(false);
	ui.pLineOpacitySlider->blockSignals(false); 
	ui.pLineWidthSpx->blockSignals(false);
	ui.pLineTypeCBBox->blockSignals(false);
	ui.pFillColorWgt->blockSignals(false);
	ui.pFillOpacitySlider->blockSignals(false);
}

HGxScene::CGxGlobalLayer* CMapFacetLayerPropertyWgt::getCurrentLayer()
{
	return m_pModelLayer;
}

//初始化信号和槽函数
void CMapFacetLayerPropertyWgt::initSignalAndSlots()
{
	// 图层名称
	connect(ui.pLayerNameEdit, SIGNAL(textChanged(QString)), this, SLOT(layerNameChangedSlot(const QString &)));	
	
	// 是否所有比例尺范围可见按钮点击
	connect(ui.pAllVisible2DPushBtn, SIGNAL(toggled(bool)), this, SLOT(scaleVisualChangedSlot(bool)));
	// 最小比例尺改变
	connect(ui.pMinVisibleCBBox, SIGNAL(editTextChanged(QString)), this, SLOT(minEditTextChangedSlot(QString)));
	// 最大比例尺改变
	connect(ui.pMaxVisibleCBBox, SIGNAL(editTextChanged(QString)), this, SLOT(maxEditTextChangedSlot(QString)));
	// 定位最小比例尺
	connect(ui.pMin2DToolBtn, SIGNAL(clicked()), this, SLOT(getCurrentScaleSlot()));
	// 定位最大比例尺
	connect(ui.pMax2DToolBtn, SIGNAL(clicked()), this, SLOT(getCurrentScaleSlot()));

	// 线色
	connect(ui.pLineColorWgt, SIGNAL(colorChangedSignal(const QColor &)), this, SLOT(lineColorChangedSlot(const QColor &)));
	// 不透明度
	connect(ui.pLineOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(lineTranChangedSlot(int)));
	// 线宽
	connect(ui.pLineWidthSpx, SIGNAL(valueChanged(int)), this, SLOT(lineWidthChangedSlot(int)));
	// 线型
	connect(ui.pLineTypeCBBox, SIGNAL(currentIndexChanged(int)), this, SLOT(lineTypeChangedSlot(int)));
	// 填充色
	connect(ui.pFillColorWgt, SIGNAL(colorChangedSignal(const QColor &)), this, SLOT(fillColorChangedSlot(const QColor &)));
	// 不透明度
	connect(ui.pFillOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(fillTranChangedSlot(int)));
}

void CMapFacetLayerPropertyWgt::minEditTextChangedSlot(QString strText)
{
	int nMin = strText.section(':', 1, 1).trimmed().toInt();
	int nMax = ui.pMaxVisibleCBBox->currentText().section(':', 1, 1).trimmed().toInt();
	int nMinValue = 1;

	if (nMin < nMinValue || nMin > nMax)
	{
		nMin = (int)m_lastMinScaleValue;
		QString strMin = QString("1:%1").arg(nMin);
		ui.pMinVisibleCBBox->setEditText(strMin);
		m_lastMinScaleValue = nMin;
	}
	else
	{
		m_lastMinScaleValue = nMin;
		setVisualScale(nMin, nMax);
	}
}

void CMapFacetLayerPropertyWgt::maxEditTextChangedSlot(QString strText)
{
	int nMax = strText.section(':', 1, 1).trimmed().toInt();
	int nMin = ui.pMinVisibleCBBox->currentText().section(':', 1, 1).trimmed().toInt();
	int nMaxValue = ui.pMaxVisibleCBBox->itemData(ui.pMaxVisibleCBBox->count() - 1).toInt();

	if (nMax > nMaxValue || nMax < nMin)
	{
		nMax = (int)m_lastMaxScaleValue;
		m_lastMaxScaleValue = nMax;
		QString strMax = QString("1:%1").arg(nMax);
		ui.pMaxVisibleCBBox->setEditText(strMax);
		m_lastMaxScaleValue = nMax;
	}
	else 
	{
		m_lastMaxScaleValue = nMax;
		setVisualScale(nMin, nMax);
	}
}

HGxScene::CGxSceneBase* CMapFacetLayerPropertyWgt::getCurrentScene()
 {
	std::string strSceneID = "";

	ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
	if (pSceneService)
	{
		strSceneID = pSceneService->activeScene();
	}

	HGxScene::CGxSceneBase* pFxScene = HGxScene::CGxControllerMgr::getInstance()->getScene(strSceneID);

 	return pFxScene;
 }

void CMapFacetLayerPropertyWgt::layerNameChangedSlot(const QString &strLayerName)
{
	m_pModelLayer->setName(strLayerName.toUtf8().data());
	emit layerNameChangedSignal(strLayerName);
}

void CMapFacetLayerPropertyWgt::dataPathChangedSlot(const QString &path )
{

}

void CMapFacetLayerPropertyWgt::scaleVisualChangedSlot(bool bChecked)
{
	ui.pMinVisibleCBBox->blockSignals(true);
	ui.pMaxVisibleCBBox->blockSignals(true);

	if (bChecked)
	{
		ui.pMinVisibleCBBox->setCurrentIndex(0);
		ui.pMaxVisibleCBBox->setCurrentIndex(13);
		ui.pMinVisibleCBBox->setEnabled(false);
		ui.pMaxVisibleCBBox->setEnabled(false);
		ui.pMin2DToolBtn->setEnabled(false);
		ui.pMax2DToolBtn->setEnabled(false);

		m_pModelLayer->setVisibleRange(0, MAX_VISIBLE_VAL);
	}
	else
	{
		ui.pMinVisibleCBBox->setEnabled(true);
		ui.pMaxVisibleCBBox->setEnabled(true);
		ui.pMin2DToolBtn->setEnabled(true);
		ui.pMax2DToolBtn->setEnabled(true);
		int iMinScale = ui.pMinVisibleCBBox->currentText().section(':', 1, 1).trimmed().toInt();
		int iMaxScale = ui.pMaxVisibleCBBox->currentText().section(':', 1, 1).trimmed().toInt();
		setVisualScale(iMinScale, iMaxScale);
	}

	ui.pMinVisibleCBBox->blockSignals(false);
	ui.pMaxVisibleCBBox->blockSignals(false);
}

void CMapFacetLayerPropertyWgt::lineColorChangedSlot(const QColor &lineColor)
{
	double dfTran = ui.pLineOpacitySlider->value() * 1.0 / 100;

	osg::Vec4f color(lineColor.redF(), lineColor.greenF(), lineColor.blueF(), dfTran);
	m_pModelLayer->setLineColor(color);
}

void CMapFacetLayerPropertyWgt::lineTranChangedSlot(int iTranparency)
{
	QColor curColor = ui.pLineColorWgt->getCurrentColor();
	double dfTran = ui.pLineOpacitySlider->value() * 1.0 / 100;

 	osg::Vec4f lineColor(curColor.redF(), curColor.greenF(), curColor.blueF(), dfTran);
 	m_pModelLayer->setLineColor(lineColor);
}

void CMapFacetLayerPropertyWgt::lineWidthChangedSlot(int iLineWidth)
{
	m_pModelLayer->setLineWidth(iLineWidth);
}

void CMapFacetLayerPropertyWgt::lineTypeChangedSlot(int iLineType)
{
	m_pModelLayer->setLineStipple(ui.pLineTypeCBBox->lineStyle());
}

void CMapFacetLayerPropertyWgt::fillColorChangedSlot(const QColor &color )
{
	QColor curColor = ui.pFillColorWgt->getCurrentColor();
	double dfTran = ui.pFillOpacitySlider->value() * 1.0 / 100;

 	osg::Vec4f fillColor(curColor.redF(), curColor.greenF(), curColor.blueF(), dfTran);
 	m_pModelLayer->setFillColor(fillColor);
}

void CMapFacetLayerPropertyWgt::fillTranChangedSlot(int iTransparency)
{
	QColor curColor = ui.pFillColorWgt->getCurrentColor();
	double dfTran = ui.pFillOpacitySlider->value() * 1.0 / 100;

 	osg::Vec4f fillColor(curColor.redF(), curColor.greenF(), curColor.blueF(), dfTran);
 	m_pModelLayer->setFillColor(fillColor);
}

void CMapFacetLayerPropertyWgt::getCurrentScaleSlot()
{
	QToolButton *pToolBtn = qobject_cast<QToolButton *>(QObject::sender());

	// 初始化当前比例尺
	double dCurrentScale = 0;

 	HGxScene::CGxSceneBase* pScene = getCurrentScene();

	if (NULL == pScene)
	{
		return;
	}

	HGxScene::CGxMapManipulator* pMapMani = static_cast<HGxScene::CGxMapManipulator*>(pScene->getManipulatorSystem()->getCurrentManipulator());

 	if (pMapMani)
 	{
 		// 获取当前场景下的比例尺
 		dCurrentScale = pMapMani->getCurrentMapScale();
 	}

	if (pToolBtn)
	{
		// 调用底层接口获取当前比例尺
		if (pToolBtn->objectName() == "pMin2DToolBtn")
		{
			// 获取最大比例尺
			QString strMax = ui.pMaxVisibleCBBox->currentText();
			int nMaxScale = strMax.section(':', 1, 1).trimmed().toInt();

			if (dCurrentScale <= nMaxScale)
			{
				ui.pMinVisibleCBBox->setEditText(QString("1:%1").arg((int)dCurrentScale));
			}
		}
		else if (pToolBtn->objectName() == "pMax2DToolBtn")
		{
			// 获取最小比例尺
			QString strMin = ui.pMinVisibleCBBox->currentText();
			int nMinScale = strMin.section(':', 1, 1).trimmed().toInt();
			if (dCurrentScale >= nMinScale)
			{
				ui.pMaxVisibleCBBox->setEditText(QString("1:%1").arg((int)dCurrentScale));
			}
		}
	}
}

void CMapFacetLayerPropertyWgt::initScaleRange(QComboBox *pComboBox)
{
	pComboBox->addItem(QString("1:%1").arg(500), QVariant(500));
	pComboBox->addItem(QString("1:%1").arg(1000), QVariant(1000));
	pComboBox->addItem(QString("1:%1").arg(2500), QVariant(2500));
	pComboBox->addItem(QString("1:%1").arg(5000), QVariant(5000));
	pComboBox->addItem(QString("1:%1").arg(10000), QVariant(10000));
	pComboBox->addItem(QString("1:%1").arg(25000), QVariant(25000));
	pComboBox->addItem(QString("1:%1").arg(50000), QVariant(50000));
	pComboBox->addItem(QString("1:%1").arg(100000), QVariant(100000));
	pComboBox->addItem(QString("1:%1").arg(250000), QVariant(250000));
	pComboBox->addItem(QString("1:%1").arg(500000), QVariant(500000));
	pComboBox->addItem(QString("1:%1").arg(1000000), QVariant(1000000));
	pComboBox->addItem(QString("1:%1").arg(10000000), QVariant(10000000));
	pComboBox->addItem(QString("1:%1").arg(50000000), QVariant(50000000));
	pComboBox->addItem(QString("1:%1").arg(100000000), QVariant(100000000));
}

void CMapFacetLayerPropertyWgt::setVisualScale(int iMinScale, int iMaxScale)
{
	// 获取当前场景

	HGxScene::CGxSceneBase* pScene = getCurrentScene();
 	if (pScene)
 	{

		HGxScene::CGxMapManipulator* pMapMani = static_cast<HGxScene::CGxMapManipulator*>(pScene->getManipulatorSystem()->getCurrentManipulator());

		if (pMapMani)
		{
 			double dfMinVisualRange = pMapMani->getCameraHeightByRange(iMinScale * 1.0);
 			double dfMaxVisualRange = pMapMani->getCameraHeightByRange(iMaxScale * 1.0);
 			m_pModelLayer->setVisibleRange(dfMinVisualRange, dfMaxVisualRange);
 		}	
 	}
}
