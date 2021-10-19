#include "CEarthPyramidCacheWgt.h"
#include <time.h>




CEarthPyramidCacheWgt::CEarthPyramidCacheWgt(QWidget *parent)
	: QWidget(parent)
	, m_bPressed(false)
	,m_nHour(0)
	,m_nMintue(0)
	,m_nSecond(0)
	,b_isStart(0)
{
	ui.setupUi(this);

#if WIN32
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window );
#else if __linux__
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog );
#endif
	setAttribute(Qt::WA_TranslucentBackground);

	m_image = QImage(":/images_earth_layer/Resources/images_earth_layer/proressBar.png");

	//读样式表文件
	QFile file(":/qss_earth_layer/Resources/qss_earth_layer/pyramidCache.css");
	if (file.open(QIODevice::ReadOnly))
	{
		setStyleSheet(file.readAll());
		file.close();
	}

	initSignalsAndSlots();
}

CEarthPyramidCacheWgt::~CEarthPyramidCacheWgt()
{

}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCacheWgt::setCacheProcess(int nValue)
{
	m_nValue = nValue;
	ui.cacheBar->setValue(nValue);

	// 如果缓存已完成
	if (nValue == ui.cacheBar->maximum())
	{
		close();
	}
}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCacheWgt::setRemainTime(const QString &strRemainTime)
{
	ui.remainTimeLbl->setText(strRemainTime);
}

void CEarthPyramidCacheWgt::setCostTime(double dCostTime, int iCostPercent)

{
	b_isStart=true;
	m_CostTime = dCostTime;
	int costtime=(int)dCostTime;
	//将耗费的总时间转换为时、分、秒
	//int dCostTime=(costtime*100/*(1-m_nValue/100))/1000;

	int dTimeNeed = costtime/iCostPercent * (100 - m_nValue);
	dTimeNeed /= 1000;

	if (dTimeNeed >= 3600)
	{
		m_nHour=dTimeNeed/3600;
		m_nMintue=dTimeNeed%3600/60;
		m_nSecond=dTimeNeed%3600%60;
	}
	else if (dTimeNeed >= 60)
	{
		m_nHour=0;
		m_nMintue=dTimeNeed/60;
		m_nSecond=dTimeNeed%60;
	}
	else
	{
		m_nHour=0;
		m_nMintue=0;
		m_nSecond=dTimeNeed;
	}

	//设置耗费的总时间
	ui.remainTimeLbl->setText(QString("%1:%2:%3").arg(m_nHour,2,10,QChar('0')).
		arg(m_nMintue,2,10,QChar('0')).arg(m_nSecond,2,10,QChar('0')));	
}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCacheWgt::mousePressEvent(QMouseEvent *event)
{
	if (event->pos().y() <= 25 && Qt::LeftButton == event->button())
	{
		m_bPressed = true;

		m_bLastPos = event->globalPos() - this->frameGeometry().topLeft();
	}
}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCacheWgt::mouseMoveEvent(QMouseEvent *event)
{
	if (!m_bPressed)
	{
		return;
	}
	move(event->globalPos() - m_bLastPos);
}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCacheWgt::mouseReleaseEvent(QMouseEvent *event)
{
	m_bPressed = false;
}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCacheWgt::initSignalsAndSlots()
{
	connect(ui.closeBtn, SIGNAL(clicked()), this, SIGNAL(cancelCacheSignal()));

	connect(ui.cacheBar, SIGNAL(valueChanged(int)), this, SLOT(valuedChangedSlot(int)));

	//定时器
	m_timer =new QTimer(this);
	connect(m_timer,SIGNAL(timeout()),this,SLOT(timerChangedSlot()));
	//m_timer->start(1000);
}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCacheWgt::valuedChangedSlot(int nValue)
{
	QString strQss = "QProgressBar{"
		"border-image:url(:/images_earth_layer/Resources/images_earth_layer/progressBg.png);"
		"border-radius:2px;}"
		"QProgressBar::chunk:enabled{"
		"border-radius:2px;"
		"background:qlineargradient(x1:0, y1:0, x2:1, y2:0";

	double v = ui.cacheBar->maximum();
	// 获取比例
	double dEndColor = static_cast<double>(nValue) / v;

	for (int i = 0; i < 100; ++i)
	{
		double dCurrent = dEndColor * i / 100.0;
		QRgb rgb = m_image.pixel((m_image.width() - 1) * dCurrent, m_image.height() / 2);
		QColor c(rgb);
		strQss.append(QString(", stop:%1 rgb(%2,%3,%4)").arg(i / 100.0).arg(c.red()).arg(c.green()).arg(c.blue()));
	}
	strQss.append(");}");
	ui.cacheBar->setStyleSheet(strQss);

	ui.valueLbl->setText(QString("%1%").arg(nValue));
	
	//设置初始默认值
	if (nValue == 0)
	{
		m_timer->start(1000);
		b_isStart=false;
		ui.remainTimeLbl->setText(tr("calculating"));
	}
	if (nValue == 99)
	{
		m_timer->stop();
	}

}

void CEarthPyramidCacheWgt::timerChangedSlot()
{
	if (b_isStart)

	{
		//进行时、分、秒的倒计时
		if (m_nSecond > 0)
		{
			m_nSecond -= 1;
		}
		else if (m_nMintue > 0)
		{
			m_nSecond = 1*60;
			m_nMintue -= 1;
			m_nSecond -= 1;

		}
		else if (m_nHour > 0)
		{
			m_nMintue = 1*60;
			m_nHour -= 1;
			m_nSecond = 1*60;
			m_nMintue -= 1;
			m_nSecond -= 1;
		}

		//更新倒计时的时间显示
		ui.remainTimeLbl->setText(QString("%1:%2:%3").arg(m_nHour,2,10,QChar('0')).
			arg(m_nMintue,2,10,QChar('0')).arg(m_nSecond,2,10,QChar('0')));	 	
	}

}
