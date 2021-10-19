//
// Created by HG on 2020/3/26.
//

#include "EquipmentSetting.h"
#include <QStyleOption>
#include <QPainter>
#include "ui_EquipmentSetting.h"
#include "QFile"
#include "QTimer"


CEquipmentSetting::CEquipmentSetting(QWidget *parent) : QWidget(parent), ui(new Ui::EquipmentSetting)
{
    ui->setupUi(this);
	QTimer* pTimer = new QTimer();
}

CEquipmentSetting::~CEquipmentSetting()
{
    delete ui;
}

void CEquipmentSetting::setType(QString type)
{
    setProperty("type", type);
    style()->polish(this);
}

void CEquipmentSetting::setEquipmentTemperatureData(double dTM)
{
	ui->label_tmdata->setText(QString::number(dTM));
}

void CEquipmentSetting::setEquipmentPowerData(double dTM)
{
	ui->label_btdata->setText(QString::number(dTM));
}

void CEquipmentSetting::setEquipmentPressData(double dTM)
{
	ui->label_prdata->setText(QString::number(dTM));
}

void CEquipmentSetting::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
