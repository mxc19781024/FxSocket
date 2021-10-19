#include <qscrollbar.h>

#include "CMapImageLayerPropertyWgt.h"

#include <gdpScene/layer/GxGlobalLayer.h> 
#include <gdpScene/layer/GxLayerImage.h> 

CMapImageLayerPropertyWgt::CMapImageLayerPropertyWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);

	QRegExp rx("\\S{0,20}");
	ui.pLayerNameEdit->setValidator(new QRegExpValidator(rx, this));

	/*QFile file(":/qss_layerauxiliary/Resources/qss/layerproperty.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
	setStyleSheet(styleSheet().append(file.readAll()));
	}*/
	connect(ui.pLayerNameEdit, SIGNAL(textChanged(QString)), this, SLOT(layerNameChangedSlot(const QString &)));
	connect(ui.pLayerPathEdit, SIGNAL(textChanged(QString)), this, SLOT(dataPathChangedSlot(const QString &)));
	connect(ui.pOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(transparencyChangedSlot(int)));
	connect(ui.pCacheEnableBtn, SIGNAL(toggled(bool)), this, SLOT(cacheEnableChangedSlot(bool)));

	QFile file(":/qss_layerauxiliary/Resources/qss/layerproperty.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(styleSheet().append(file.readAll()));
	}
}

CMapImageLayerPropertyWgt::~CMapImageLayerPropertyWgt()
{

}

void CMapImageLayerPropertyWgt::setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer)
{
	m_pImageLayer = static_cast<HGxScene::CGxLayerImage* >(pLayer);

	ui.pLayerNameEdit->blockSignals(true);
	ui.pLayerPathEdit->blockSignals(true);
	ui.pOpacitySlider->blockSignals(true);
	ui.pCacheEnableBtn->blockSignals(true);

	ui.pLayerNameEdit->setText(QString::fromUtf8(m_pImageLayer->getName().c_str()));
	ui.pLayerPathEdit->setText(QString::fromUtf8(m_pImageLayer->getLayerConfig()->sourceConfig().url().value().c_str()));
	ui.pOpacitySlider->setPercentSliderValue(qRound(m_pImageLayer->getOpacity() * 100));
	ui.pCacheEnableBtn->setChecked(m_pImageLayer->getCacheEnable());

	ui.pLayerNameEdit->blockSignals(false);
	ui.pLayerPathEdit->blockSignals(false);
	ui.pOpacitySlider->blockSignals(false);
	ui.pCacheEnableBtn->blockSignals(false);
}

HGxScene::CGxGlobalLayer* CMapImageLayerPropertyWgt::getCurrentLayer()
{
	return m_pImageLayer;
}

void CMapImageLayerPropertyWgt::layerNameChangedSlot(const QString &strLayerName)
{
	m_pImageLayer->setName(strLayerName.toUtf8().data());
	emit layerNameChangedSignal(strLayerName);
}

void CMapImageLayerPropertyWgt::dataPathChangedSlot(const QString &strDataPath)
{
	// 数据路径不可修改
}

void CMapImageLayerPropertyWgt::transparencyChangedSlot(int iTransparency)
{
	m_pImageLayer->setOpacity(iTransparency * 1.0 / 100);
}

void CMapImageLayerPropertyWgt::cacheEnableChangedSlot(bool bEnabled)
{
	m_pImageLayer->setCacheEnable(bEnabled);
}