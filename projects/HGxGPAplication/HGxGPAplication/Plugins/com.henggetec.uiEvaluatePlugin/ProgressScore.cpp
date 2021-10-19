//
// Created by HG on 2020/3/27.
//

#include "ProgressScore.h"
#include <QStyle>
#include <QPainter>
#include <QDebug>
#include <QStyleOption>

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

static float random(float min, float max) { return min + (max - min)*(float)rand() / (float)RAND_MAX; }
ProgressScore::ProgressScore(QWidget *parent)
    : QWidget(parent)
{
	setFixedSize(230,173);
}


void ProgressScore::paintEvent(QPaintEvent * event)
{
	m_listTopk << "63" << "64" << "71" << "70" << "56";
	QString value = QString::number(random(10, 100), 'f', 0);
	if (!m_listTopk.contains(value) && value.toInt() > m_listTopk[4].toInt())
	{
		m_listTopk.push_back(value);
		m_listTopk.sort();
		m_listTopk.removeAt(0);

		m_listTopk.swap(0, 4);
		m_listTopk.swap(1, 3);
	}

	QPainter painter(this);
	//画标题
	//QPainter painter(ui->EquipmentAtttibutes);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(110, 110, 110)));
	painter.drawText(QRect(135, 5, 100, 20), "分值波动情况");




	//画Topk圆弧图
	//QPainter painter(ui->EquipmentAtttibutes_2);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.translate(5, -5);

	painter.setPen(QPen(QColor(255, 122, 50), 2));
	painter.drawArc(20, 20, 170, 170, 180 * 16, 270 * 16);

	QFont font;
	//font.setBold(true);
	font.setPointSize(13);
	painter.setFont(font);
	painter.drawText(QRect(5, 0, 50, 20), Qt::AlignRight, QString("TOP1"));
	painter.drawText(QRect(66, 0, 40, 20), Qt::AlignLeft, m_listTopk[0] + QString("分"));

	painter.setPen(QPen(QColor(206, 207, 207), 2));
	painter.drawArc(35, 35, 140, 140, 225 * 16, 225 * 16);
	font.setPointSize(12);
	painter.setFont(font);
	painter.drawText(QRect(8, 20, 47, 20), Qt::AlignRight, QString("TOP2"));
	painter.drawText(QRect(66, 20, 40, 20), Qt::AlignLeft, m_listTopk[1] + QString("分"));

	painter.setPen(QPen(QColor(172, 175, 178), 2));
	painter.drawArc(50, 50, 110, 110, 270 * 16, 180 * 16);
	font.setPointSize(11);
	painter.setFont(font);
	painter.drawText(QRect(11, 40, 44, 20), Qt::AlignRight, QString("TOP3"));
	painter.drawText(QRect(66, 40, 40, 20), Qt::AlignLeft, m_listTopk[2] + QString("分"));



	painter.setPen(QPen(QColor(141, 141, 141), 2));
	painter.drawArc(65, 65, 80, 80, 315 * 16, 135 * 16);
	font.setPointSize(10);
	painter.setFont(font);
	painter.drawText(QRect(14, 60, 41, 20), Qt::AlignRight, QString("TOP4"));
	painter.drawText(QRect(66, 60, 40, 20), Qt::AlignLeft, m_listTopk[3] + QString("分"));

	painter.setPen(QPen(QColor(89, 89, 87), 2));
	painter.drawArc(80, 80, 50, 50, 360 * 16, 90 * 16);
	font.setPointSize(9);
	painter.setFont(font);
	painter.drawText(QRect(17, 78, 39, 20), Qt::AlignRight, QString("TOP5"));
	painter.drawText(QRect(66, 78, 40, 20), Qt::AlignLeft, m_listTopk[4] + QString("分"));

	QWidget::paintEvent(event);
}

void ProgressScore::resizeEvent(QResizeEvent * event)
{
    update();
}
