//
// Created by HG on 2020/3/26.
//

#include "EvaluateAttibutes.h"
#include <QStyleOption>
#include <QPainter>
#include "ui_EvaluateAttibutes.h"
#include "QFile"
#include "QTimer"
static float random(float min,float max) { return min+(max-min)*(float)rand()/(float)RAND_MAX; }
CEvaluateAttibutes::CEvaluateAttibutes(QWidget *parent) : QWidget(parent), ui(new Ui::EvaluateAttibutes)
{
    ui->setupUi(this);
	setStyleSheet("color:rgb(110,110,110)");
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
}

CEvaluateAttibutes::~CEvaluateAttibutes()
{
    delete ui;
}

void CEvaluateAttibutes::setType(QString type)
{
    setProperty("type", type);
    style()->polish(this);
}

void CEvaluateAttibutes::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CEvaluateAttibutes::TimerUpdateSlot()
{
	ui->label_6->setText(QString::number(random(30,100),'f',0));
}