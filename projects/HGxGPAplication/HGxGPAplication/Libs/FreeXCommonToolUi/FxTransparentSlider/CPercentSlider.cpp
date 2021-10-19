#include "FxTransparentSlider/CPercentSlider.h"
#include <QPainter>
#include "CHandleLabel.h"
#include <QStyleOption>
#include <QProxyStyle>



CPercentSlider::CPercentSlider(QWidget *parent)
	: QSlider(parent)
{
	setObjectName("CPercentSlider");
	this->setMinimum(0);
	this->setMaximum(100);
	//this->setValue(2);
	this->setPageStep(0);
	m_displayLabel = new CHandleLabel(this);
	m_displayLabel->setFixedSize(28,12);
	m_displayLabel->setStyleSheet("color:#3b7cff; border:1px ; background:white; font-size:10px; border-radius:4px;");
	connect(m_displayLabel, SIGNAL(editingFinished()), this, SLOT(editingFinishedSlot( )));
	// 设置游标背景为白色
	m_displayLabel->setAutoFillBackground(true);
	QPalette palette;
	palette.setColor(QPalette::Background, Qt::white);
	m_displayLabel->setPalette(palette);
	m_displayLabel->setAlignment(Qt::AlignCenter);
	m_displayLabel->setText(QString::number(this->value()) + "%");


	//m_displayLabel->move(0, 5);
	
	setStyleSheet( "QSlider{max-width:1000px;} \
		            QSlider::groove:horizontal {border: 0px solid #bbb;border-radius:3px;}\
				   QSlider::handle:horizontal {width: 30px;height:8px;margin:0px 0; border-radius:3px;}\
				   QSlider::add-page:horizontal {background:#ffffff;margin-top:10px;margin-bottom:10px;}\
				   QSlider::sub-page:horizontal {background:#3b7cff;margin-top:10px;margin-bottom:10px;} ");

	connect(this, SIGNAL(valueChanged(int )), this, SLOT(valueChangedSlot(int )));
	//setStyle(new QProxyStyle);
}

CPercentSlider::~CPercentSlider()
{

}

void CPercentSlider::setPercentSliderValue(int iValue)
{
	setValue(iValue);
	m_displayLabel->setText(QString::number(this->value())+ "%");
    int i = (this->width() - m_displayLabel->width()) * this->value() / (this->maximum() - this->minimum());
    int iH = (this->height() - m_displayLabel->height())/2;
    m_displayLabel->move(i, iH);
}

void CPercentSlider::editingFinishedSlot()
{
	QString strText = m_displayLabel->text();
	strText.chop(1);
	int iValue =  strText.toInt();
	setValue(iValue);
}

void CPercentSlider::valueChangedSlot(int value)
{
    int i = (this->width() - m_displayLabel->width()) * this->value() / (this->maximum() - this->minimum());
    int iH = (this->height() - m_displayLabel->height())/2;
    m_displayLabel->move(i, iH);
	m_displayLabel->setText(QString::number(this->value())+ "%");
}

void CPercentSlider::mousePressEvent( QMouseEvent *event )
{
	m_displayLabel->setText(QString::number(this->value())+ "%");
	QSlider::mousePressEvent(event);
}

void CPercentSlider::mouseMoveEvent( QMouseEvent *event )
{
    int i = (this->width() - m_displayLabel->width()) * this->value() / (this->maximum() - this->minimum());
    int iH = (this->height() - m_displayLabel->height())/2;
    m_displayLabel->move(i, iH);
	QSlider::mouseMoveEvent(event);
}

void CPercentSlider::mouseReleaseEvent( QMouseEvent *event )
{
	QSlider::mouseReleaseEvent(event);
}

void CPercentSlider::paintEvent( QPaintEvent *event )
{
	//QSlider::paintEvent(event);
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

	QSlider::paintEvent(event);
}

void CPercentSlider::resizeEvent( QResizeEvent *event )
{
	int i = (this->width() - m_displayLabel->width()) * this->value() / (this->maximum() - this->minimum());
    int iH = (this->height() - m_displayLabel->height())/2;
    m_displayLabel->move(i, iH);
}

