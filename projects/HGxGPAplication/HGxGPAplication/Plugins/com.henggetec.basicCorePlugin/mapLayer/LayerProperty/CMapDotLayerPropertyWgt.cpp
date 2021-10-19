#include <QFileDialog>
//#include <FreeXFramework/FreeXFramework.h>
#include <scene_ui/ISceneUiService.h>
//#include <scene/IFxSceneService.h>
//#include <map/IMapManipulatorManager.h>
//#include <map/IFxMapScene.h>
//#include <earth/IFxEarthManipulatorManager.h>
#include <FxToast/CToast.h>
#include <qscrollbar.h>

#include "CMapDotLayerPropertyWgt.h"
#include <bdpCommon/GxStrUtil.h>
#include <bdpDB/GxRegister.h>

#include <gdpScene/layer/GxGlobalLayer.h>
#include <gdpScene/layer/GxLayerModel.h>
#include <gdpScene/scene/GxGeoContext.h>
#include <gdpScene/manipulator/GxMapManipulator.h>

#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/manipulator/GxManipulatorManager.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

#define MAX_VISIBLE_VAL FLT_MAX

CMapDotLayerPropertyWgt::CMapDotLayerPropertyWgt(QWidget *parent)
	: QWidget(parent)
	, m_lastMinScaleValue(500)
	, m_lastMaxScaleValue(100000000)
{
	ui.setupUi(this);

	initScaleRange(ui.pMinVisibleCBBox);
	initScaleRange(ui.pMaxVisibleCBBox);

	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	ui.pFontCBBox->addItem(QStringLiteral("黑体"), QVariant::fromValue(QString("fonts/simhei.ttf")));
	ui.pFontCBBox->addItem(QStringLiteral("华文楷体"), QVariant::fromValue(QString("fonts/hwkaiti.ttf")));
	ui.pFontCBBox->addItem(QStringLiteral("微软雅黑"), QVariant::fromValue(QString("fonts/msyh.ttf")));
	ui.pFontCBBox->addItem(QStringLiteral("宋体"), QVariant::fromValue(QString("fonts/simsun.ttc")));
	ui.pFontCBBox->addItem(QStringLiteral("英文字体"), QVariant::fromValue(QString("fonts/arial.ttf")));

	ui.pIconPathTBtn->setToolTip(tr("browse"));
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

CMapDotLayerPropertyWgt::~CMapDotLayerPropertyWgt()
{

}

void CMapDotLayerPropertyWgt::setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer)
{
	m_pModelLayer = static_cast<HGxScene::CGxLayerModel*>(pLayer);

	if (NULL == m_pModelLayer)
	{
		return;
	}

	ui.pLayerNameEdit->blockSignals(true);
	ui.pLayerPathEdit->blockSignals(true);
	ui.pAllVisible2DPushBtn->blockSignals(true);
	ui.pDotVisible2DPushBtn->blockSignals(true);
	ui.pMinVisibleCBBox->blockSignals(true);
	ui.pMaxVisibleCBBox->blockSignals(true);
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
		ui.pAllVisible2DPushBtn->setChecked(false);
		ui.pDotVisible2DPushBtn->setChecked(true);
		ui.pMinVisibleCBBox->setEnabled(true);
		ui.pMaxVisibleCBBox->setEnabled(true);
		ui.pMin2DToolBtn->setEnabled(true);
		ui.pMax2DToolBtn->setEnabled(true);

 		HGxScene::CGxSceneBase* pScene = getCurrentScene();
 		if (pScene)
 		{
			HGxScene::CGxMapManipulator*  pMapMain = static_cast<HGxScene::CGxMapManipulator*>(pScene->getManipulatorSystem()->getCurrentManipulator());
  			if (pMapMain)
  			{
  				//+0.5实现四舍五入
  				ui.pMinVisibleCBBox->setEditText(QString("1:%1").arg((int)(pMapMain->getRange(fMinRange) + 0.5)));
  				ui.pMaxVisibleCBBox->setEditText(QString("1:%1").arg((int)(pMapMain->getRange(fMaxRange) + 0.5)));
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
	// 字体类型
	for (int i = 0; i < ui.pFontCBBox->count(); i++)
	{
		QString strFontType = ui.pFontCBBox->itemData(i).toString();

		if (strFontType.split('/').last() == QString::fromUtf8(m_pModelLayer->getFontName().c_str()).split('/').last())
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
	ui.pAllVisible2DPushBtn->blockSignals(false);
	ui.pDotVisible2DPushBtn->blockSignals(false);
	ui.pMinVisibleCBBox->blockSignals(false);
	ui.pMaxVisibleCBBox->blockSignals(false);
	ui.pFontCBBox->blockSignals(false);
	ui.pFontSizeSpx->blockSignals(false);
	ui.pFontColorWgt->blockSignals(false);
	ui.pIconPathEdit->blockSignals(false);
	ui.pIconSizeSpx->blockSignals(false);
}

HGxScene::CGxGlobalLayer* CMapDotLayerPropertyWgt::getCurrentLayer()
{
	return m_pModelLayer;
}

//初始化信号和槽函数
void CMapDotLayerPropertyWgt::initSignalAndSlots()
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

	// 字体类型
	connect(ui.pFontCBBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fontFamilyChangedSlot()));
	// 字体大小
	connect(ui.pFontSizeSpx, SIGNAL(valueChanged(int)), this, SLOT(fontSizeChangedSlot(int)));
	// 字体颜色
	connect(ui.pFontColorWgt, SIGNAL(colorChangedSignal(const QColor &)), this, SLOT(fontColorChangedSlot(const QColor &)));
	// 图标路径
	connect(ui.pIconPathEdit, SIGNAL(textChanged(const QString &)), this, SLOT(iconPathChangedSlot(const QString &)));
	// 图标大小
	connect(ui.pIconSizeSpx, SIGNAL(valueChanged(int)), this, SLOT(iconSizeChangedSlot(int)));
	// 选择图标按钮
	connect(ui.pIconPathTBtn, SIGNAL(pressed()),this, SLOT(selectIconPathSlot()));
}

void CMapDotLayerPropertyWgt::getCurrentScaleSlot()
{
	QToolButton *pToolBtn = qobject_cast<QToolButton *>(QObject::sender());

	// 初始化当前比例尺
	double dCurrentScale = 0;

	HGxScene::CGxSceneBase* pScene = getCurrentScene();
	if (pScene)
	{
		HGxScene::CGxMapManipulator*  pMapMain = static_cast<HGxScene::CGxMapManipulator*>(pScene->getManipulatorSystem()->getCurrentManipulator());
		if (pMapMain)
		{
 			// 获取当前场景下的比例尺
 			dCurrentScale = pMapMain->getCurrentMapScale();
 		}
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

void CMapDotLayerPropertyWgt::minEditTextChangedSlot(QString strText)
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

void CMapDotLayerPropertyWgt::maxEditTextChangedSlot(QString strText)
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

void CMapDotLayerPropertyWgt::selectIconPathSlot()
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

HGxScene::CGxSceneBase* CMapDotLayerPropertyWgt::getCurrentScene()
{
	std::string strSceneID = "";

	ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
	if (pSceneService)
	{
		strSceneID = pSceneService->activeScene();
	}

	return HGxScene::CGxControllerMgr::getInstance()->getScene(strSceneID);
}

void CMapDotLayerPropertyWgt::layerNameChangedSlot(const QString &strLayerName)
{
	m_pModelLayer->setName(strLayerName.toUtf8().data());
	emit layerNameChangedSignal(strLayerName);
}

void CMapDotLayerPropertyWgt::dataPathChangedSlot(const QString &strDataPath)
{

}

void CMapDotLayerPropertyWgt::scaleVisualChangedSlot(bool bChecked)
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

void CMapDotLayerPropertyWgt::fontFamilyChangedSlot()
{
	int nIndex = ui.pFontCBBox->currentIndex();

	std::string strFontFamily = (HGX_FILE_CONFIG().getFullPath(ui.pFontCBBox->itemData(nIndex).toString().toStdString()));	
	m_pModelLayer->setFontName(strFontFamily);
}

void CMapDotLayerPropertyWgt::fontSizeChangedSlot(int iSize)
{
	m_pModelLayer->setFontSize(iSize);
}

void CMapDotLayerPropertyWgt::fontColorChangedSlot(const QColor & )
{
	QColor curColor = ui.pFontColorWgt->getCurrentColor();

	osg::Vec4f fontColor(curColor.redF(), curColor.greenF(), curColor.blueF(), curColor.alphaF());
	m_pModelLayer->setFontColor(fontColor);
}

void CMapDotLayerPropertyWgt::iconPathChangedSlot(const QString &strPath)
{
	m_pModelLayer->setPointIconPath(strPath.toUtf8().data());
}

void CMapDotLayerPropertyWgt::iconSizeChangedSlot(int iIconSize)
{
	m_pModelLayer->setPointIconSize(iIconSize);
}

void CMapDotLayerPropertyWgt::initScaleRange(QComboBox *pComboBox)
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

void CMapDotLayerPropertyWgt::setVisualScale(int iMinScale, int iMaxScale)
{
	HGxScene::CGxSceneBase* pScene = getCurrentScene();
	if (pScene)
	{
		HGxScene::CGxMapManipulator*  pMapMain = static_cast<HGxScene::CGxMapManipulator*>(pScene->getManipulatorSystem()->getCurrentManipulator());
		if (pMapMain)
		{
			double dfMinVisualRange = pMapMain->getCameraHeightByRange(iMinScale * 1.0);
			double dfMaxVisualRange = pMapMain->getCameraHeightByRange(iMaxScale * 1.0);
			m_pModelLayer->setVisibleRange(dfMinVisualRange, dfMaxVisualRange);
		}
	}
}
