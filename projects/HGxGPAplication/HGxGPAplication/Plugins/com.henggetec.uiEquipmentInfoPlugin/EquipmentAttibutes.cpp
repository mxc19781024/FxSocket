//
// Created by HG on 2020/3/26.
//

#include "EquipmentAttibutes.h"
#include <QStyleOption>
#include <QPainter>
#include "ui_EquipmentAttibutes.h"
#include "QFile"
#include "QTimer"

CEquipmentAttibutes::CEquipmentAttibutes(QWidget *parent) : QWidget(parent), ui(new Ui::EquipmentAttibutes)
{
    ui->setupUi(this);
	m_pIndex = 0;
	m_dMaxOil = 0;
	m_dSpeed = 0;
// 	QFile f(":/qss/resrc/qss/all.css");
// 	f.open(QIODevice::ReadOnly);
// 	QString ss = f.readAll();
// 	setStyleSheet(ss);
//     ui->ring_label->setText(0, "10%");
//     ui->ring_label->setText(1, "20%");
//     ui->ring_label->setText(2, "30%");
//     ui->timeliness_semi_circle->SetRingType(ProgressRing::GridSemiCircle2);
//     ui->timeliness_semi_circle->SetColor(QColor("#163240"), QColor(66, 195, 249, 255));
//     ui->timeliness_semi_circle->SetScaleColor(QColor(66, 195, 249, 255));
//     ui->timeliness_semi_circle->SetValue(11);
// 
//     ui->center_stacked_widget->setCurrentWidget(ui->overall_coordination_progress_ball);
	QTimer* pTimer = new QTimer();
	connect(pTimer, SIGNAL(timeout()), this, SLOT(TimerUpdateSlot()));
	pTimer->start(1000);
	InitProgressBar();
}

CEquipmentAttibutes::~CEquipmentAttibutes()
{
    delete ui;
}

void CEquipmentAttibutes::setType(QString type)
{
    setProperty("type", type);
    style()->polish(this);
}

void CEquipmentAttibutes::setAmmunitionNumber(int nNumber)
{
	m_pAmmuBar->setProgressValue(nNumber);
}

void CEquipmentAttibutes::setMaxSpeed(double dSpeed)
{
	m_dSpeed = dSpeed;
	ui->label_speed->setText(QString::number(dSpeed) + "km/h");
}

void CEquipmentAttibutes::setMaxOil(double dMaxOil)
{
	m_dMaxOil = dMaxOil;
	
}

void CEquipmentAttibutes::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	ui->oilwidget->SetValue(((m_dMaxOil - (m_dSpeed * m_pIndex * 0.000278)*5.67) / m_dMaxOil) * 100);
}

void CEquipmentAttibutes::TimerUpdateSlot()
{
	m_pIndex++;
}

void CEquipmentAttibutes::InitProgressBar()
{
	m_pAmmuBar=new ProgressBar(20);
	QHBoxLayout* layout=new QHBoxLayout(ui->ProgressBarWidget);
	layout->setContentsMargins(0,4,0,0);
	m_pAmmuBar->setFixedWidth(170);
	m_pAmmuBar->setFixedHeight(2);
	m_pAmmuBar->setProgressValue(0.5);
	ui->zidan_number->setText(QString::number(0.5*20));
	layout->addWidget(m_pAmmuBar);
}

ProgressBar::ProgressBar( int count, QWidget* parent)
	: QWidget(parent),
	m_allCount(count)
{
	init();
}
void ProgressBar:: setProgressValue(double value)
{

	for ( int  i = 0 ; i < m_allCount ; i++)
	{
		if( i  < value/**m_allCount*/ )
		{
			m_listProgress[i]->setStyleSheet("background:rgb(255,122,50) ; max-height:2px; max-width:5px");
		}
		else
		{
			m_listProgress[i]->setStyleSheet("background:rgb(110,110,110) ; max-height:2px; max-width:5px");
		}
	}
}

void ProgressBar::init()
{
	QWidget* wgt=new QWidget(this);
	QHBoxLayout* layout=new QHBoxLayout(wgt);
	QHBoxLayout* layout_2=new QHBoxLayout(wgt);
	layout->setContentsMargins(0,0,0,0);
	layout_2->setSpacing(2);
	layout_2->setContentsMargins(0,0,0,0);

	for(int i=0 ; i < m_allCount ; i++)
	{
		QLabel*  lab = new QLabel;
		lab->setStyleSheet("background:rgb(110,110,110) ; max-height:2px; max-width:5px");
		layout_2->addWidget(lab);
		m_listProgress.push_back(lab);
	}
	layout->addLayout(layout_2);
}

void ProgressBar::setProgressCount(int value)
{

}
