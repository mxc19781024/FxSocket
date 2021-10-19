#include <QPainter>
#include <QPixMap>
#include <QTimer>
#include <QBoxLayout>
#include <FxGanttChartWidget.h>
#include <FxGraduationWidget.h>
#include <FxGanttWidget.h>
#include <QStyleOption>
#include <service\event\ctkEventConstants.h>
#include <QMetaType>
#include "bdpUtil\GxSimulationTimer.h"
#include "bdpUtil\GxTimeManager.h"
#include "GxPluginFwLauncherInterface.h"
#include "GxPluginFrameworkCore.h"
#include "GxEventHelperInterface.h"

#define DayToSec 86400
#define HourToSec 3600
#define MinToSec 60
#define SimuTimeToSec 86400

Q_DECLARE_METATYPE(SGanttChartInfo)

CFxGanttChart::CFxGanttChart(ctkPluginContext *context,QWidget* pWidget)
	:QWidget(pWidget)
{
	setObjectName("ImageFrame");
	m_pGantt = new CFxGanttWidget(this);
	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/LoginFinished", HGxFWLauncherInstance.getPluginContext());
	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/clearScene", HGxFWLauncherInstance.getPluginContext());
	m_pGraduation = new CFxGraduationWidget(this);
	QVBoxLayout* pVLayout = new QVBoxLayout;
	pVLayout->setMargin(0);
	pVLayout->setSpacing(0);
	pVLayout->setContentsMargins(0,0,0,20);
	this->setLayout(pVLayout);
	pVLayout->addWidget(m_pGraduation);
	pVLayout->addWidget(m_pGantt);

	reset();
	this->setMinimumSize(m_nBeginX + m_nLongX,m_nLongY*2);
	//this->setAutoFillBackground(false);
//	{
	//m_pGantt->setStyleSheet("{background: transparent;}");
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
//	this->setStyleSheet(" background-color: rgba(255, 0, 0, 5);");
// 	setAttribute(Qt::WA_Hover);
	//setWindowOpacity(0.0);
	//this->setAutoFillBackground(false);
	/*QFile file(":/qss/resrc/qss/ganate_chart.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
	QString str = file.readAll();
	setStyleSheet(str);
	}*/
	this->setStyleSheet("QWidget#ImageFrame{border-image: url(:/images/resrc/images/dikuang.png);}");
	QTimer* pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), m_pGantt, SLOT(update()));
	connect(pTimer, SIGNAL(timeout()), m_pGraduation, SLOT(update()));
	pTimer->start(100);

	setBeginTime(HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent());
	setTotalTime(2,0,0,0);
	ctkDictionary dic;
	dic.insert(ctkEventConstants::EVENT_TOPIC, SFxGanttChartInfoEventConstants::Topic().c_str());
	context->registerService<ctkEventHandler>(this, dic);
	connect(this, SIGNAL(addObjectEventSignal(SGanttChartInfo)), this, SLOT(addObjectEventSlot(SGanttChartInfo)));

    m_mapImage.insert(std::make_pair("1",new QPixmap(":/images/resrc/images/1.png")));
	m_mapImage.insert(std::make_pair("2",new QPixmap(":/images/resrc/images/2.png")));
	m_mapImage.insert(std::make_pair("3",new QPixmap(":/images/resrc/images/3.png")));
	m_mapImage.insert(std::make_pair("4",new QPixmap(":/images/resrc/images/4.png")));
	m_mapImage.insert(std::make_pair("5",new QPixmap(":/images/resrc/images/5.png")));
	m_mapImage.insert(std::make_pair("6",new QPixmap(":/images/resrc/images/6.png")));
	m_mapImage.insert(std::make_pair("7",new QPixmap(":/images/resrc/images/7.png")));

	m_imageSize.setX(m_mapImage.begin()->second->width());
	m_imageSize.setY(m_mapImage.begin()->second->height());

	m_pRect = new QRect(QPoint(0,0),m_imageSize);//鼠标选中检测区域初始化

	connect(this, SIGNAL(ganttChartShowSignal()), this, SLOT(ganttChartShowSlot()));
	connect(this, SIGNAL(ganttChartClearSignal()), this, SLOT(ganttChartClearSlot()));
}

CFxGanttChart::~CFxGanttChart()
{
}

void CFxGanttChart::setBeginTime(double time)
{
	m_dInitialBeginTime = time;
	updateGantt();
}

void CFxGanttChart::setTotalTime(const int& nDay, const int& nHour, const int& nMin, const int& nSec)
{
	m_dCurTotalTime = m_dInitialTotalTime = nDay*DayToSec + nHour * HourToSec + nMin * MinToSec + nSec;
	updateGantt();
}

void CFxGanttChart::addObjectInfo(SObjectInfo stObjectInfo)
{
	m_mapIDToObjectInfo.insert(stObjectInfo.m_strID, stObjectInfo);
	updateGantt();
}

void CFxGanttChart::removeObjectInfo(const QString& strObjectID)
{
	m_mapIDToObjectInfo.remove(strObjectID);
	updateGantt();
}

void CFxGanttChart::addObjectEvent(const QString& strObjectID, SEventInfo stEventInfo)
{
	if (m_mapIDToObjectInfo.count(strObjectID))
	{
		m_mapIDToObjectInfo[strObjectID].m_mapIDToEventInfo.insert(stEventInfo.m_strID, stEventInfo);
		updateGantt();
	}
}

void CFxGanttChart::removeObjectEvent(const QString& strObjectID, const QString& strEventID)
{
	if (m_mapIDToObjectInfo.count(strObjectID))
	{
		m_mapIDToObjectInfo[strObjectID].m_mapIDToEventInfo.remove(strEventID);
		updateGantt();
	}
}

void CFxGanttChart::setGanttHeight( const unsigned int& unHeight )
{
	m_nGanttHeight = unHeight;
	if (m_nObjectHeight < m_nGanttHeight)
	{
		m_nObjectHeight = m_nGanttHeight;
	}
	updateGantt();
}

void CFxGanttChart::setObjectHeight( const unsigned int& unInterval )
{
	m_nObjectHeight = unInterval;
	if (m_nObjectHeight < m_nGanttHeight)
	{
		m_nGanttHeight = m_nObjectHeight;
	}
	updateGantt();
}

void CFxGanttChart::setGraduationParameter( const unsigned int& unShortX,const unsigned int& unShortY ,const unsigned int& unLongX,const unsigned int& unLongY ,const unsigned int& unBeginX,const unsigned int& unBeginY ,const QColor& colorGraduation )
{
	m_nShortX = unShortX;
	m_nShortY = unShortY;
	m_nLongX = unLongX;
	m_nLongY = unLongY;
	m_nBeginX = unBeginX;
	m_nBeginY = unBeginY;
	m_colorGraduation = colorGraduation;
	updateGantt();
}

void CFxGanttChart::setWheelScale( double dScale )
{
	m_dScale  = dScale;
}

void CFxGanttChart::setBackGroundColor( const QColor& colorBackGround )
{
	m_colorBackGround = colorBackGround;
	updateGantt();
}

void CFxGanttChart::setPointerColor( const QColor& colorPointer )
{
	m_colorPointer = colorPointer;
	updateGantt();
}

void CFxGanttChart::setGradients( bool bGradients )
{
	m_bGradients = bGradients;
	updateGantt();
}

void CFxGanttChart::setGrid( bool bGrid )
{
	m_bGrid = bGrid;
	updateGantt();
}

void CFxGanttChart::reset()
{
	m_nShortX = 30;
	m_nShortY = 10;
	m_nLongX = 150;
	m_nLongY = 15;
	m_nBeginX = 150;
	m_nBeginY = 38;
	m_pGraduation->setMinimumWidth(m_nBeginX + m_nLongX);
	m_pGraduation->setFixedHeight(m_nBeginY);
	m_pGantt->setMinimumSize(m_nBeginX + m_nLongX,m_nBeginY);
	m_nTotalGraduation = 150;
	m_nTimeGraduation = 150;
	m_colorGraduation = QColor(105, 106, 105);
	m_polygonPointer.putPoints(0, 3
		,m_nBeginX - 5, m_nBeginY - 15
		,m_nBeginX, m_pGantt->height()
		,m_nBeginX + 5, m_nBeginY - 15);
	m_dInitialBeginTime = 0;
	m_dCurBeginTime = 0;		
	m_dInitialTotalTime = m_nLongX;
	m_dCurTotalTime = m_nLongX;		
	m_dScale = 1.5;           
	m_colorBackGround = QColor(102, 204, 255);  
	m_colorPointer = QColor(255, 0, 0);
	m_nGanttY = 0;		     
	m_nObjectHeight = 24;
	m_nGanttHeight = 10;     
	m_nGanttTotalHeight = 0;
	m_bScrollBar = false;
	m_rectScrollBar = QRect(this->width()-10,1,0,0);   //滚动条区域
	m_nTotalScrollBar = m_pGantt->height() - m_rectScrollBar.height();  //滚动条总值
	m_nCurScrollBar = 0;     //滚动条当前值
	m_pointCurMouse = QPoint(0,0);    
	m_bDragHorizontal = false;  
	m_bDragVertical = false;    
	m_bDragScrollBar = false;
	m_bGradients = false;    
	m_bGrid = true;        
	m_bDragPointer = false;       
	m_mapIDToObjectInfo.clear();
}

void CFxGanttChart::updateParameter()
{
	m_nGanttTotalHeight = m_mapIDToObjectInfo.size()*m_nObjectHeight;
	m_nTotalScrollBar = m_pGantt->height() - m_rectScrollBar.height();
}

void CFxGanttChart::updateScrollBarByGanttDrag()
{
	m_nTotalScrollBar = m_pGantt->height() - m_rectScrollBar.height();
	int nCurTotalHeight = m_nGanttTotalHeight-m_pGantt->height();
	int nDiff=0;
	if (nCurTotalHeight!=0)
	{
		int nNewY = m_nTotalScrollBar*abs(m_nGanttY)/nCurTotalHeight;
		nNewY = max(min(nNewY,m_nTotalScrollBar),0);
		nDiff = nNewY - m_nCurScrollBar;
		m_nCurScrollBar = nNewY;
	}
	m_rectScrollBar.translate(this->width()-m_rectScrollBar.width()-m_rectScrollBar.x(),nDiff);
}
void CFxGanttChart::paintEvent(QPaintEvent* p)
{
	QPainter painter(this);
	//painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/images/resrc/images/dikuang.png"));
	updateParameter();
	///QFrame::paintEvent(p);

	QStyleOption opt;
	opt.init(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CFxGanttChart::wheelEvent(QWheelEvent* ev)
{
	if (m_nBeginX < ev->pos().x())
	{
		if (ev->delta() > 0)//放大
		{
			double dNewTime = m_dCurTotalTime/m_dScale;
			if (dNewTime > m_nTimeGraduation)
			{
				double dRadio = double(ev->x() - m_nBeginX) / double(m_nTimeGraduation);
				m_dCurBeginTime += (m_dCurTotalTime-dNewTime)*dRadio;
				m_dCurTotalTime = dNewTime;
			}
		}
		else if (ev->delta() < 0)//缩小     
		{
			double dNewTime = m_dCurTotalTime*m_dScale;
			if (dNewTime > m_dInitialTotalTime)
			{
				dNewTime = m_dInitialTotalTime;
			}
			double dRadio = double(ev->x() - m_nBeginX) / double(m_nTimeGraduation);
			m_dCurBeginTime += (m_dCurTotalTime-dNewTime)*dRadio;
			m_dCurTotalTime = dNewTime;

			if (m_dCurBeginTime < 0)
			{
				m_dCurBeginTime = 0;
			}
			if ((m_dCurBeginTime + m_dCurTotalTime) > m_dInitialTotalTime)
			{
				m_dCurBeginTime = m_dInitialTotalTime - m_dCurTotalTime;
			}
		}
		updateGantt();
	}
	QWidget::wheelEvent(ev);
}

void CFxGanttChart::mousePressEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		m_pointCurMouse = ev->globalPos();	
		for (auto itrObject = m_mapIDToObjectInfo.begin(); itrObject != m_mapIDToObjectInfo.end(); ++itrObject)
		{
			if (!itrObject->m_bDraw)
			{
				continue;
			}
			for (auto itrEvent = itrObject->m_mapIDToEventInfo.begin(); itrEvent != itrObject->m_mapIDToEventInfo.end(); ++itrEvent)
			{
				if (!itrEvent->m_bDraw)
				{
					continue;
				}
				if (itrEvent->m_rectDraw.contains(m_pGantt->mapFromGlobal(m_pointCurMouse)))//是否在该甘特条区域内
				{
					//处理甘特条单击操作
					updateGantt();
					return;
				}
				else if(this->mapFromGlobal(m_pointCurMouse).y()<m_nBeginY)
				{
					QPoint imagePoint = itrEvent->m_point;
					QPoint evPoint = m_pGraduation->mapFromGlobal(m_pointCurMouse) - imagePoint;
					if (m_pRect->contains(evPoint)&&(imagePoint.x()+ m_imageSize.x()/2)>=m_nBeginX&&(imagePoint.x()+ m_imageSize.x()/2)<=width())//是否在该图标区域内
					{
						int nAlpha = qAlpha(m_mapImage.begin()->second->toImage().convertToFormat(QImage::Format_ARGB6666_Premultiplied).pixel(evPoint));
						if (nAlpha!=0)//是否选中非透明部分
						{
							//处理图标单击操作
							updateGantt();
							return;
						}
					}
				}
			}
		}
	}
	QWidget::mousePressEvent(ev);
}

void CFxGanttChart::mouseReleaseEvent(QMouseEvent* ev)
{
	QWidget::mouseReleaseEvent(ev);
}

void CFxGanttChart::mouseMoveEvent(QMouseEvent* ev)
{
	if (m_bDragPointer)
	{
		double dTime = JDToSecAtCurBeginTime(HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent());
		//int nDrawX = timeToX(dTime);
		//nDrawX += (ev->globalPos().x() - m_pointCurMouse.x());
		dTime += m_dCurTotalTime*(ev->globalPos().x() - m_pointCurMouse.x()) / m_nTimeGraduation;
		HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeCurrent(SecAtCurBeginTimeToJD(dTime));
	}	
	else if (m_bDragScrollBar)
	{
		int nDiff = ev->globalPos().y() - m_pointCurMouse.y();
		if ((m_nCurScrollBar + nDiff)<0)
		{
			nDiff = - m_nCurScrollBar;
		}
		if ((m_nCurScrollBar + nDiff)>m_nTotalScrollBar)
		{
			nDiff = m_nTotalScrollBar - m_nCurScrollBar;
		}
		m_nCurScrollBar += nDiff;
		m_rectScrollBar.translate(0,nDiff);
		m_nGanttY = (m_pGantt->height()-m_nGanttTotalHeight)*m_nCurScrollBar/m_nTotalScrollBar;
	}
	else
	{
		if (m_bDragHorizontal)
		{
			m_dCurBeginTime = m_dCurBeginTime + m_dCurTotalTime*(m_pointCurMouse.x() - ev->globalPos().x()) / m_nTimeGraduation;
			if (m_dCurBeginTime < 0)
			{
				m_dCurBeginTime = 0;
			}
			if ((m_dCurBeginTime + m_dCurTotalTime) > (m_dInitialTotalTime))
			{
				m_dCurBeginTime = m_dInitialTotalTime - m_dCurTotalTime;
			}
		}
		if (m_bDragVertical)
		{
			m_nGanttY = m_nGanttY + (ev->globalPos().y() - m_pointCurMouse.y());
			if (m_pGantt->height() - m_nGanttY > m_nGanttTotalHeight)
			{
				m_nGanttY = m_pGantt->height() - m_nGanttTotalHeight;
			}
			if (m_nGanttY > 0)
			{
				m_nGanttY = 0;
			}
			updateScrollBarByGanttDrag();
		}
	}
	m_pointCurMouse = ev->globalPos();
	updateGantt();
	QWidget::mouseMoveEvent(ev);
}

void CFxGanttChart::mouseDoubleClickEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		for (auto itrObject = m_mapIDToObjectInfo.begin(); itrObject != m_mapIDToObjectInfo.end(); ++itrObject)
		{
			if (!itrObject->m_bDraw)
			{
				continue;
			}
			for (auto itrEvent = itrObject->m_mapIDToEventInfo.begin(); itrEvent != itrObject->m_mapIDToEventInfo.end(); ++itrEvent)
			{
				if (!itrEvent->m_bDraw)
				{
					continue;
				}
				if (itrEvent->m_rectDraw.contains(m_pGantt->mapFromGlobal(m_pointCurMouse)))//是否在该图标区域内
				{
					HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeCurrent(itrEvent->m_dBeginTime);
					updateGantt();
					return;
				}
				else if(this->mapFromGlobal(m_pointCurMouse).y()<m_nBeginY)
				{
					QPoint imagePoint = itrEvent->m_point;
					QPoint evPoint = m_pGraduation->mapFromGlobal(m_pointCurMouse) - imagePoint;
					if (m_pRect->contains(evPoint)&&(imagePoint.x()+ m_imageSize.x()/2)>=m_nBeginX&&(imagePoint.x()+ m_imageSize.x()/2)<=width())//是否在该图标区域内
					{
						int nAlpha = qAlpha(m_mapImage.begin()->second->toImage().convertToFormat(QImage::Format_ARGB6666_Premultiplied).pixel(evPoint));
						if (nAlpha!=0)//是否选中非透明部分
						{
							HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeCurrent(itrEvent->m_dBeginTime);//处理图标双击操作
							updateGantt();
							return;
						}
					}
				}
			}
		}
		if (this->mapFromGlobal(m_pointCurMouse).x()>m_nBeginX)
		{
			HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeCurrent(SecAtCurBeginTimeToJD(xToTime(this->mapFromGlobal(m_pointCurMouse).x())));
		}
	}
	QWidget::mouseDoubleClickEvent(ev);
}

void CFxGanttChart::resizeEvent(QResizeEvent* ev)
{
	if ((this->height()) > (m_nGanttTotalHeight + m_nGanttY))
	{
		m_nGanttY = this->height() - m_nGanttTotalHeight;
		if (this->height() - m_nGanttY > m_nGanttTotalHeight)
		{
			m_nGanttY = this->height() - m_nGanttTotalHeight;
		}
		if (m_nGanttY > 0)
		{
			m_nGanttY = 0;
		}
	}
	updateScrollBarByGanttDrag();
	updateGantt();
	QWidget::resizeEvent(ev);
}

QDateTime CFxGanttChart::convertJDToDateTime(double dJD)
{
	int nYear, nDay, nMonth, nHour, nMin;
	double dSec;
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->jdToCal(dJD, nYear, nMonth, nDay, nHour, nMin, dSec);
	return QDateTime(QDate(nYear, nMonth, nDay), QTime(nHour, nMin, dSec));
}

void CFxGanttChart::convertSecToHMS(int nTime, int& nHour, int& nMin, int& nSec)
{
	nSec = nTime % 60;
	nMin = nTime / 60;
	nHour = nMin / 60;
	nMin = nMin % 60;
}

QString CFxGanttChart::HMSToString(const int& nHour, const int& nMin, const int& nSec)
{
	QString strTime;
	strTime.append(QString::number(nHour)).append(":");
	strTime.append(QString::number(nMin).rightJustified(2, QLatin1Char('0'), true)).append(":");
	strTime.append(QString::number(nSec).rightJustified(2, QLatin1Char('0'), true));
	return strTime;
}

int CFxGanttChart::timeToX(double dTime)
{
	return m_nBeginX + m_nTimeGraduation*dTime / m_dCurTotalTime;
}

double CFxGanttChart::xToTime( int nX )
{
	return ((nX - m_nBeginX) * m_dCurTotalTime ) / m_nTimeGraduation;
}

double CFxGanttChart::JDToSecAtCurBeginTime(double dJD)
{
	return (dJD - m_dInitialBeginTime)*SimuTimeToSec - m_dCurBeginTime;
}

double CFxGanttChart::SecAtCurBeginTimeToJD( double dTime )
{
	return (dTime + m_dCurBeginTime) / SimuTimeToSec + m_dInitialBeginTime;
}

void CFxGanttChart::updateGantt()
{
	m_pGantt->update();
	m_pGraduation->update();
}

void CFxGanttChart::handleEvent( const ctkEvent& event )
{
	if (event.getTopic() == "event/LoginFinished")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit ganttChartShowSignal();
	}
	if (event.getTopic() == "event/clearScene")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit ganttChartClearSignal();
	}
	else
	{
		SGanttChartInfo sInfo = event.getProperty(SFxGanttChartInfoEventConstants::GanttChartInfoEvent().c_str()).value<SGanttChartInfo>();
		//界面必须在主线程中处理
		emit addObjectEventSignal(sInfo);
	}
}

void CFxGanttChart::addObjectEventSlot( SGanttChartInfo sObjectInfo)
{
	SObjectInfo stObjectInfo;
	stObjectInfo.m_strID = sObjectInfo.m_strID;
	stObjectInfo.m_strName = sObjectInfo.m_strName;
	int j = 0;
	for (auto itr=sObjectInfo.m_vecTimePari.begin();itr!=sObjectInfo.m_vecTimePari.end();++itr)
	{
		SEventInfo stEventInfo;
		stEventInfo.m_strID = QString::number(j);
		stEventInfo.m_dBeginTime = itr->first;
		stEventInfo.m_dEndTime = itr->second;
		stEventInfo.m_colorEvent = QColor(255, 128, 0);
		stEventInfo.m_strType = QString::number(1);
		stObjectInfo.m_mapIDToEventInfo.insert(stEventInfo.m_strID, stEventInfo);
		++j;
	}
	setGraduationParameter(30,10,150,15,130,40,QColor(105, 106, 105));
	double dCurTime = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent();
	int nCurTime= dCurTime;
	double dCurrentTime;
	if (dCurTime - nCurTime >= 0.5)
	{
		dCurrentTime = int(dCurTime) + 0.5;
	}
	else
	{
		dCurrentTime = int(dCurTime) - 0.5;
	}
	setBeginTime(dCurrentTime);
	addObjectInfo(stObjectInfo);
	setGanttHeight(4);
	setPointerColor(QColor(255, 0, 0));
	setGrid(false);
}

void CFxGanttChart::ganttChartShowSlot()
{
	this->show();
}

void CFxGanttChart::ganttChartClearSlot()
{
	m_mapIDToObjectInfo.clear();
	updateGantt();
}
