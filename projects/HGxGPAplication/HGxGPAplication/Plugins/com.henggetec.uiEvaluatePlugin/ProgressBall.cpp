//
// Created by HG on 2020/3/27.
//

#include "ProgressBall.h"
#include <QStyle>
#include <QPainter>
#include <QDebug>
#include <QStyleOption>

ProgressBall::ProgressBall(QWidget *parent)
    : QWidget(parent),
      m_eShape(PS_ELLIPSE), m_iBodyRadius(0), m_iProgressRadius(0), m_color(218, 109, 44), m_iValue(50),
      m_rollIndex1(0), m_rollIndex2(0),m_reset(true),m_bUseImage(false),m_pImage(nullptr)
{
	m_bUseImage = true;
	m_pImage = new QImage("E:/icon_youliang.png");
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(Roll()));
    m_timer.start(30);
}

ProgressBall::ProgressBall(ProgressShape eShape, QWidget *parent)
	: QWidget(parent)
	,m_bUseImage(false)
	,m_pImage(nullptr)
{
    m_eShape = eShape;
	m_bUseImage = true;
	m_pImage = new QImage("E:/icon_youliang.png");
}

Q_SLOT void ProgressBall::Roll()
{
    if (++m_rollIndex1 > 2 * m_iProgressRadius)
    {
        m_rollIndex1 = 0;
    }

    if (--m_rollIndex2 < 0 )
    {
        m_rollIndex2 = 2 * m_iProgressRadius;
    }

    update();
}


void ProgressBall::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QLinearGradient gradient(30,60,60,30);
	gradient.setColorAt(0,m_color);
	gradient.setColorAt(0.4,QColor(218, 109, 44));
	gradient.setColorAt(0.6,QColor(218, 109, 44));
	gradient.setColorAt(0.9,QColor(224,160,133));
	gradient.setColorAt(1,QColor(253,194,177));
	painter.setBrush(gradient);
	QRectF rect(4,4,52,52);
	painter.setPen(QPen(QBrush(gradient),4));
	painter.drawArc(rect,0,360*16);
}


//void ProgressBall::paintEvent(QPaintEvent * event)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter painter(this);
//    auto rect = style()->subElementRect(QStyle::SE_FrameContents,&opt,this);
//    if(!rect.isValid()){rect = contentsRect();}
//    style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);
//    if(m_reset)
//    {
//        // 计算波浪曲线，用A * sin(x)函数
//        // 如果不规则波浪，则可以动态改变A和x的步进
//        m_curve.clear();
//
//        m_iBodyRadius = (rect.width() > rect.height() ? rect.height() : rect.width()) / 2;
//        m_iProgressRadius = m_iBodyRadius*0.95;
//
//        // 圆形的
//        if (m_eShape == PS_ELLIPSE)
//        {
//            for (int i = 0; i < 2 * m_iProgressRadius; ++i)
//            {
//                m_curve.append(QPointF(double(i - m_iProgressRadius), m_iProgressRadius / 6.0 * sin(3.14 * i / m_iProgressRadius)));
//            }
//        }
//        else
//        {// 矩形
//            for (int i = 0; i < width(); ++i)
//            {
//                m_curve.append(QPointF(double(i - width() / 2), m_iProgressRadius / 6.0 * sin(3.14 * i / m_iProgressRadius)));
//            }
//        }
//        m_reset = false;
//    }
//    //if (m_iBodyRadius < 30)
//    //{
//        //return;
//    //}
//
//    painter.setRenderHint(QPainter::Antialiasing);
//
//    painter.translate(rect.center());
//    painter.setBrush(QColor(22, 50, 64));
//    painter.setPen(Qt::NoPen);
//
//    switch (m_eShape)
//    {
//        case PS_ELLIPSE:
//            DrawEllipse(painter);
//            break;
//        case PS_RECTANGLE:
//            DrawRectangle(painter);
//            break;
//        default:
//            break;
//    }
//}

void ProgressBall::resizeEvent(QResizeEvent * event)
{
    m_reset = true;
    update();
}

void ProgressBall::DrawEllipse(QPainter & painter)
{
    // 画背景
    painter.drawEllipse(-m_iBodyRadius, -m_iBodyRadius, 2 * m_iBodyRadius, 2 * m_iBodyRadius);

    if (m_iValue <= 0)
    {
        return;
    }

    QPolygonF curve1; // 近处波浪
    QPolygonF curve2; // 远处波浪

    // 浪高
    double rollHeight = 2 * m_iProgressRadius * (50 - m_iValue) / 100.0;

    for (int i = 0; i < 2 * m_iProgressRadius; ++i)
    {
        curve1.append(QPointF(i - m_iProgressRadius, m_curve[(m_rollIndex1 + i) % (2 * m_iProgressRadius)].y() + rollHeight));
        // 远处波浪比近处矮一半
        curve2.append(QPointF(i - m_iProgressRadius, m_curve[(m_rollIndex2 + i) % (2 * m_iProgressRadius)].y() * 0.5 + rollHeight));
    }

    // 波浪，用QPainterPath的求交功能（intersect)，波浪曲线用sin曲线

    QPainterPath ball;
    ball.addEllipse(-m_iProgressRadius, -m_iProgressRadius, 2 * m_iProgressRadius, 2 * m_iProgressRadius);

    QPainterPath progress;
    progress.moveTo(-m_iProgressRadius, m_iProgressRadius);
    progress.lineTo(-m_iProgressRadius, curve1.at(0).y());
    progress.addPolygon(curve1);
    progress.lineTo(m_iProgressRadius, m_iProgressRadius);
    progress.lineTo(-m_iProgressRadius, m_iProgressRadius);

    // 近处波浪
    QPainterPath progressBall1 = progress.intersected(ball);

    progress = QPainterPath();
    progress.moveTo(-m_iProgressRadius, m_iProgressRadius);
    progress.lineTo(-m_iProgressRadius, curve2.at(0).y());
    progress.addPolygon(curve2);
    progress.lineTo(m_iProgressRadius, m_iProgressRadius);
    progress.lineTo(-m_iProgressRadius, m_iProgressRadius);

    // 远处波浪
    QPainterPath progressBall2 = progress.intersected(ball);

    // 两个波浪重叠部分
    QPainterPath progressBall = progressBall1.intersected(progressBall2);

    // 波浪半透明
    m_color.setAlphaF(0.6);
    painter.setBrush(m_color);
    painter.drawPath(progressBall2);
    painter.drawPath(progressBall1);

    // 两个波浪重叠的地方，颜色不透明
    m_color.setAlphaF(1);
    painter.setBrush(m_color);
    painter.drawPath(progressBall);

    painter.setPen(Qt::white);
	
	int nSize = m_iBodyRadius*0.8;
	int nTextY = 0;
	if (m_bUseImage)
	{
		QRect rectImage(-nSize*0.5, -nSize, nSize, nSize);  
		painter.drawImage(rectImage,*m_pImage);
	}
    //// 进度值
    QFont font(QString::fromLocal8Bit("微软雅黑"));
    font.setPixelSize(nSize*0.6);
    painter.setFont(font);
    painter.drawText(-m_iProgressRadius, nTextY, 2 * m_iProgressRadius, nSize, Qt::AlignCenter, QString("%1%").arg(m_iValue));
}

void ProgressBall::DrawRectangle(QPainter & painter)
{
    // 画背景
    painter.drawRoundedRect(-width() / 2, -height() / 2, width(), height(), 10, 10);

    if (m_iValue <= 0)
    {
        return;
    }

    QPolygonF curve1; // 近处波浪
    QPolygonF curve2; // 远处波浪

    // 浪高
    double rollHeight = 2 * m_iProgressRadius * (50 - m_iValue) / 100.0;

    for (int i = 0; i < width(); ++i)
    {
        curve1.append(QPointF(i - width() / 2, m_curve[(m_rollIndex1 + i) % (2 * m_iProgressRadius)].y() + rollHeight));
        // 远处波浪比近处矮一半
        curve2.append(QPointF(i - width() / 2, m_curve[(m_rollIndex2 + i) % (2 * m_iProgressRadius)].y() * 0.5 + rollHeight));
    }

    // 波浪，用QPainterPath的求交功能（intersect)，波浪曲线用sin曲线

    QPainterPath roundedRect;
    roundedRect.addRoundedRect(-width() / 2, -height() / 2, width(), height(), 10, 10);

    QPainterPath progress;
    progress.moveTo(-width() / 2, height() / 2);
    progress.lineTo(-width() / 2, curve1.at(0).y());
    progress.addPolygon(curve1);
    progress.lineTo(width() / 2, height() / 2);
    progress.lineTo(-width() / 2, height() / 2);

    // 近处波浪
    QPainterPath progressBall1 = progress.intersected(roundedRect);

    progress = QPainterPath();
    progress.moveTo(-width() / 2, height() / 2);
    progress.lineTo(-width() / 2, curve2.at(0).y());
    progress.addPolygon(curve2);
    progress.lineTo(width() / 2, height() / 2);
    progress.lineTo(-width() / 2, height() / 2);

    // 远处波浪
    QPainterPath progressBall2 = progress.intersected(roundedRect);

    // 两个波浪重叠部分
    QPainterPath progressBall = progressBall1.intersected(progressBall2);

    // 波浪半透明
    m_color.setAlphaF(0.6);
    painter.setBrush(m_color);
    painter.drawPath(progressBall2);
    painter.drawPath(progressBall1);

    // 两个波浪重叠的地方，颜色不透明
    m_color.setAlphaF(1);

    // 颜色底部深，上面浅
    QLinearGradient gradient;
    gradient.setStart(QPointF(0, height() / 2));
    gradient.setFinalStop(QPointF(0, rollHeight));
    gradient.setColorAt(0, m_color.darker());
    gradient.setColorAt(1, m_color);
    painter.setBrush(gradient);
    painter.drawPath(progressBall);

    painter.setPen(Qt::white);

    // 进度值
    QFont font(QString::fromLocal8Bit("微软雅黑"));
    font.setPixelSize(18);
    painter.setFont(font);
    painter.drawText(-m_iProgressRadius, -20, 2 * m_iProgressRadius, 20, Qt::AlignCenter, QString("%1%").arg(m_iValue));
}
