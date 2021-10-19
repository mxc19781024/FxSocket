/**************************************************************************************************
* @file		: CPreviewColorArea.cpp
* @note		: 颜色预览区域实现文件
* @author	: k00026
* @data		: 2018-8
**************************************************************************************************/


#include <QStylePainter>
#include "CPreviewColorArea.h"
#include <QPaintEvent>


CPreviewColorArea::CPreviewColorArea(QWidget *parent)
	: QWidget(parent)
	, m_curColor(Qt::black)
	, m_newColor(Qt::black)
	, m_iRoundRadius(5)
{

}

CPreviewColorArea::~CPreviewColorArea()
{

}

void CPreviewColorArea::SetCurrentColor(const QColor &c)
{
	m_curColor = c;
	update();
}

void CPreviewColorArea::SetNewColor(const QColor &c)
{
	m_newColor = c;
	update();
}

void CPreviewColorArea::paintEvent(QPaintEvent *event)
{
	QRect r = event->rect();
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing);
	QPainterPath path;

	path.moveTo(m_iRoundRadius, 0);
	path.lineTo(r.width() / 2, 0);
	path.lineTo(r.width() / 2, r.height());
	path.lineTo(m_iRoundRadius, r.height());
	path.arcTo(0, r.height() - m_iRoundRadius * 2, m_iRoundRadius * 2, m_iRoundRadius * 2, 270, -90);
	path.lineTo(0, r.height() - m_iRoundRadius);
	path.lineTo(0, m_iRoundRadius);
	path.arcTo(0, 0, m_iRoundRadius * 2, m_iRoundRadius * 2, 180, -90);

	painter.setBrush(m_curColor);
	painter.drawPath(path);

	QTransform mirror(-1, 0, 0, 0, 1, 0, 0, 0, 1);
	painter.setTransform(mirror);
	painter.translate(-r.width(), 0);

	painter.setBrush(m_newColor);
	painter.drawPath(path);
}