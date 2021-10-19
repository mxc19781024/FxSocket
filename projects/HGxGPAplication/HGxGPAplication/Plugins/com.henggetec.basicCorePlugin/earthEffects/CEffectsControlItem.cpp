#include "CEffectsControlItem.h"




CEffectsControlItem::CEffectsControlItem(QWidget *parent)
	: QWidget(parent)
	, m_strName("")
{
	ui.setupUi(this);

	connect(ui.switchPushBtn, SIGNAL(toggled(bool)), this, SLOT(btnSwitchedSlot(bool)));
}

CEffectsControlItem::~CEffectsControlItem()
{

}

void CEffectsControlItem::setSwitchOn( bool bChecked )
{
	// 只能控制界面中开关的状态，不会自动发出信号调用底层接口进行控制
	ui.switchPushBtn->blockSignals(true);
	ui.switchPushBtn->setChecked(bChecked);
	ui.switchPushBtn->blockSignals(false);
}

bool CEffectsControlItem::getSwitchState()
{
	return ui.switchPushBtn->isChecked();
}

void CEffectsControlItem::setBtnToolTip( QString &strTip )
{
	ui.switchPushBtn->setToolTip(strTip);
}

void CEffectsControlItem::setItemName(const QString &strName, const QString &strIconName)
{
	ui.nameLbl->setText(strName);
	ui.iconWgt->setStyleSheet(QString("border-image:url('%1%2.png')").arg(IMAGE_PATH).arg(strIconName));
	m_strName = strName;
}

void CEffectsControlItem::btnSwitchedSlot(bool bChecked)
{
	emit ItemChecked(m_strName, bChecked);
}
