#include <QFile>

#include "earthLayer/LayerProperty/CEleLayerPropertyWgt.h"

#include "gdpScene/layer/GxGlobalLayer.h"
#include "gdpScene/layer/GxLayerElevation.h"

CEleLayerPropertyWgt::CEleLayerPropertyWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QFile file(":/qss_layerauxiliary/Resources/qss/layerproperty.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(styleSheet().append(file.readAll()));
	}
	//缓存控制开关没有写槽函数，导致缓存控制失效（w00224 2019-10-10）
	connect(ui.pCacheEnableBtn, SIGNAL(toggled(bool)), this, SLOT(cacheEnableChangedSlot(bool)));

	connect(ui.pLayerNameEdit, SIGNAL(textChanged(QString)), this, SLOT(layerNameChangedSlot(const QString &)));
}

CEleLayerPropertyWgt::~CEleLayerPropertyWgt()
{

}

void CEleLayerPropertyWgt::setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer)
{
	m_pEleLayer = static_cast<HGxScene::CGxLayerElevation *>(pLayer);

	ui.pLayerNameEdit->blockSignals(true);
	ui.pLayerPathEdit->blockSignals(true);
	ui.pCacheEnableBtn->blockSignals(true);

 	ui.pLayerNameEdit->setText(QString::fromUtf8(m_pEleLayer->getName().c_str()));
	ui.pLayerPathEdit->setText(QString::fromUtf8(m_pEleLayer->getLayerConfig()->sourceConfig().url().value().c_str()));
 	ui.pCacheEnableBtn->setChecked(m_pEleLayer->getCacheEnable());

	ui.pLayerNameEdit->blockSignals(false);
	ui.pLayerPathEdit->blockSignals(false);
	ui.pCacheEnableBtn->blockSignals(false);
}

HGxScene::CGxGlobalLayer* CEleLayerPropertyWgt::getCurrentLayer()
{
	return m_pEleLayer;
}

void CEleLayerPropertyWgt::layerNameChangedSlot(const QString &strLayerName)
{
	m_pEleLayer->setName(strLayerName.toUtf8().data());
	emit layerNameChangedSignal(strLayerName);
}

void CEleLayerPropertyWgt::dataPathChangedSlot(const QString &path)
{
	// 数据路径不可修改
}

void CEleLayerPropertyWgt::cacheEnableChangedSlot(bool bEnabled)
{
	m_pEleLayer->setCacheEnable(bEnabled);
}
