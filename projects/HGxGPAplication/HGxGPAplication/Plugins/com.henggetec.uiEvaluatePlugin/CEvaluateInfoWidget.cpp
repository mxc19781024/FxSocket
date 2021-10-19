#include "CEvaluateInfoWidget.h"
#include "ui_CEvaluateInfoWidget.h"
#include <QStandardItemModel>
#include <QPixmapCache>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QFile>
#include <QTimer>
#include "QLinkedList"
#include "EvaluateAttibutes.h"
#include "AlarmInfo.h"
#include "QPainter"
#include "GxPluginFwLauncherInterface.h"
#include "GxPluginFrameworkCore.h"
#include "GxEventHelperInterface.h"
#include "stdlib.h"
#include "QGraphicsWidget"
#include "QGraphicsItem"
#include "service\event\ctkEventConstants.h"
#include "FxTimePlot.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

	static float random(float min,float max) { return min+(max-min)*(float)rand()/(float)RAND_MAX; }
//调整节点的定时器触发间隔

CEvaluateInfoWidget::CEvaluateInfoWidget(ctkPluginContext *context, QWidget *parent)
	: QWidget(parent)
	, m_pContext(context)
	, ui(new Ui::CEvaluateInfoWidget)
{
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);
	ui->EquipmentAtttibutes_2->installEventFilter(this);
	ui->EquipmentAtttibutes->installEventFilter(this);
	ui->EquipmentAtttibutes_2->setFixedSize(230,185);
	connect(this, SIGNAL(EvaluateInfoShowSignal()), this, SLOT(EvaluateInfoShowSlot()));

	ctkDictionary dic;
	dic.insert(ctkEventConstants::EVENT_TOPIC, SFxAlarmInfoEventConstants::Topic().c_str());
	context->registerService<ctkEventHandler>(this, dic);

	connect(this, SIGNAL(createEventSignal(SAlarmInfo)), this, SLOT(createEventSlot(SAlarmInfo)));

	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/LoginFinished", HGxFWLauncherInstance.getPluginContext());
	m_listTopk<<"63"<<"64"<<"71"<<"70"<<"56";
	//样式表，需要注意路径
	QFile mainwndStyleFile(":/qss/resrc/qss/evaluate_info.css");
	if (mainwndStyleFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = styleSheet() + mainwndStyleFile.readAll();
		setStyleSheet(str);
	}
	InitFrameChart1();

	QTimer* pTimer = new QTimer();
	connect(pTimer, SIGNAL(timeout()), this, SLOT(TimerUpdateSlot()));
	pTimer->start(1000);
}

CEvaluateInfoWidget::~CEvaluateInfoWidget()
{
	delete ui;
}

void CEvaluateInfoWidget::createEventSlot(SAlarmInfo sInfo)
{
	m_pAlarmInfo->setInfoData(sInfo);
}

void CEvaluateInfoWidget::handleEvent(const ctkEvent& event)
{
	if (event.getTopic() == "event/LoginFinished")
	{
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit EvaluateInfoShowSignal();
	}
	else if (event.getTopic() == "event/AlarmInfo/published")
	{
		SAlarmInfo sInfo = event.getProperty(SFxAlarmInfoEventConstants::AlarmInfo().c_str()).value<SAlarmInfo>();
		//界面必须在主线程中处理
		emit createEventSignal(sInfo);
	}
}

void CEvaluateInfoWidget::InitFrameChart1()
{

	QDateTime baseDateTime(QDateTime::currentDateTime().addSecs(-120));
	 
	for (int i = 1; i < 5000; i++)
	{
	 	QDateTime datetime = baseDateTime.addSecs(i);
	 	int iValue = random(0,100);
	 	QPointF point(datetime.toMSecsSinceEpoch(), iValue);
	 	m_listpoints << point;
	 }

	FxTimePlot *pFxTimePlot = new FxTimePlot();
	int index = pFxTimePlot->addLineGraph("", "时间", "高度(KM)");
	pFxTimePlot->setTimeRange(index,QDateTime::currentDateTime(),QDateTime::currentDateTime().addDays(0.5));
	pFxTimePlot->setValueRange(index, 0, 5.0);
	pFxTimePlot->axisRect()->setRangeDrag(Qt::Horizontal);

	for (int i = 0; i < 90;++i)
	{
		pFxTimePlot->addPoint(0, QDateTime::currentDateTime().addSecs(900 * i), qrand() % 50 / 10);
	}
	ui->verticalLayout_2->setContentsMargins(-40, -10, -30, -40);
	ui->verticalLayout_2->addWidget(pFxTimePlot);
	// 	// 创建折线序列，设置折线颜色
// 	FreeXCharts::FxLineSeries *pLineSeries = new FreeXCharts::FxLineSeries;
// 	pLineSeries->setColor(QColor(219, 220, 220));
// 	pLineSeries->append(m_listpoints);
// 
// 
// 	QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, 100));
// 	linearGrad.setColorAt(0, QColor(103, 103, 107,64));
// 	linearGrad.setColorAt(1, QColor(103, 103, 107,64));
// 
// 
// 	// 创建面积序列，设置折线作为其边界线
// 	FreeXCharts::FxAreaSeries *pAreaSeries = new FreeXCharts::FxAreaSeries(pLineSeries);
// 	m_pSaveAreaSeries=pAreaSeries;
// 	//pAreaSeries->setName("序列");
// 
// 
// 	// 设置画笔，画刷
// 	pAreaSeries->setPen(Qt::NoPen);
// 	pAreaSeries->setBrush(QBrush(linearGrad));
// 
// 
// 	//// 设置关键点标记可见
// 	//pAreaSeries->setPointsVisible(false);
// 	//pAreaSeries->setPointMarkersBrush(QBrush(Qt::white));
// 	//pAreaSeries->setPointMarkersPen(QPen(QColor(255, 70, 131), 2));
// 
// 
// 	// 设置图表标题
// 	// 	chart()->setTitle("大数据量面积图");
// 	// 	chart()->setSubtitlePen(QPen(Qt::black));
// 	// 	chart()->setFont(QFont("Microsoft YaHei", 10, QFont::Bold)
// 
// 	// 创建时间轴
// 
// 	QDateTime minDateTime(QDateTime::currentDateTime().addSecs(-120));
// 	QDateTime maxDateTime(QDateTime::currentDateTime());
// 	FreeXCharts::FxDateTimeAxis *pDateTimeAxis = new FreeXCharts::FxDateTimeAxis;
// 	m_pSaveDateTimeAxis=pDateTimeAxis;
// 	pDateTimeAxis->setRange(minDateTime, maxDateTime);
// 	pDateTimeAxis->setAxisLineColor(QColor(94,92,97));
// 	pDateTimeAxis->setLabelsColor(QColor(94,92,97));
// 	pDateTimeAxis->setTickCount(3);
// 	pDateTimeAxis->setFormat("mm:ss");
// 	pDateTimeAxis->setGridLineVisible(false);
// 
// 
// 	// 创建数值轴
// 	FreeXCharts::FxValueAxis *pYAxis = new FreeXCharts::FxValueAxis;
// 	m_pSaveYAxis=pYAxis;
// 	pYAxis->setRange(0, 100);
// 	pYAxis->setAxisLineColor(QColor(94,92,97));
// 	pYAxis->setLabelsColor(QColor(94,92,97));
// 	pYAxis->setTickCount(6);
// 	//pYAxis->setTitleText("Values");
// 	pYAxis->setGridLineVisible(false);
// 	pYAxis->setAxisLineVisible(false);
// 
// 	// 创建图表对象
// 	FreeXCharts::FxChartView *m_pChartArea = new FreeXCharts::FxChartView;\
// 	m_pSaveChartArea=m_pChartArea;
// 	// 添加坐标轴
// 	m_pChartArea->chart()->setAxisX(pDateTimeAxis);
// 	m_pChartArea->chart()->setAxisY(pYAxis);
// 	//m_pChartArea->chart()->setTitle();
// 	m_pChartArea->chart()->setSubtitlePen(QPen(Qt::black));
// 	m_pChartArea->chart()->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
// 	// 添加序列
// 	m_pChartArea->chart()->addSeries(pAreaSeries);
// 	m_pChartArea->chart()->legend()->hide();
// 
// 	//m_pChartArea->chart()->setSubtitle(QString("分值波动情况"));
// 	//m_pChartArea->chart()->setSubtitlePen(QColor(110,110,110));
// 	m_pChartArea->setContentsMargins(-40,-10,-30,-40);
// 	m_pChartArea->chart()->setContentsMargins(-40,-10,-30,-40);
// 	//m_pChartArea->chart()->setMinimumSize(230,183);
// 	//m_pChartArea->setFixedSize(230,183);
// 	m_pChartArea->move(0,0);
// 
// 	m_pChartArea->setAutoFillBackground(true);
// 	m_pChartArea->chart()->setPlotAreaBackgroundVisible(true);
// 	// 将坐标轴和序列关联
// 	pAreaSeries->attachAxis(pDateTimeAxis);
// 	pAreaSeries->attachAxis(pYAxis);
// 	m_pChartArea->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
// 	// 设置反锯齿
// 
// 	// 	QGraphicsScene *psceneArea = new QGraphicsScene();
// 	// 	psceneArea->addItem(m_pChartArea);
// 	// 将图表添加到图表视图上
// 	ui->verticalLayout_2->addWidget(m_pChartArea);
// 	m_pChartArea->setRenderHint(QPainter::Antialiasing);
// 
// 	m_pChartArea->setWheelZoom();

	// 消息展示

	 m_pAlarmInfo =new CAlarmInfo();
	 m_pAlarmInfo->setFixedSize(230,138);
 	ui->horizontalLayout_5->addWidget(m_pAlarmInfo);

}

void CEvaluateInfoWidget::TimerUpdateSlot()
{
	QString value=QString::number(random(10,100),'f',0);
	if(!m_listTopk.contains(value) && value.toInt() > m_listTopk[4].toInt())
	{
		m_listTopk.push_back(value);
		m_listTopk.sort();
		m_listTopk.removeAt(0);

		m_listTopk.swap(0,4);
		m_listTopk.swap(1,3);
		update();
	}
// 	SAlarmInfo  sAlarmInfo;
// 	sAlarmInfo.level = static_cast<ELevel>(0);
// 	sAlarmInfo.time = QTime::currentTime();
// 	sAlarmInfo.data = QDate::currentDate();
// 	sAlarmInfo.info = "我方战机C完成打击任务";
// 	//界面必须在主线程中处理
// 	emit createEventSignal(sAlarmInfo);
// 	update();
// 	创建折线序列，设置折线颜色
// 		FreeXCharts::FxLineSeries *pLineSeries = new FreeXCharts::FxLineSeries;
// 		pLineSeries->setColor(QColor(219, 220, 220));
// 		pLineSeries->append(m_listpoints);
// 	
// 		QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, 100));
// 		linearGrad.setColorAt(0, QColor(103, 103, 107,64));
// 		linearGrad.setColorAt(1, QColor(103, 103, 107,64));
// 	
// 		// 创建面积序列，设置折线作为其边界线
// 		FreeXCharts::FxAreaSeries *pAreaSeries = new FreeXCharts::FxAreaSeries(pLineSeries);
// 		//pAreaSeries->setName("序列");
// 	
// 		// 设置画笔，画刷
// 		pAreaSeries->setPen(Qt::NoPen);
// 		pAreaSeries->setBrush(QBrush(linearGrad));
// 	
// 		// 创建时间轴
// 		QDateTime minDateTime(QDateTime::currentDateTime().addSecs(-120));
// 		QDateTime maxDateTime(QDateTime::currentDateTime());
// 		FreeXCharts::FxDateTimeAxis *pDateTimeAxis = new FreeXCharts::FxDateTimeAxis;
// 		pDateTimeAxis->setRange(minDateTime, maxDateTime);
// 		pDateTimeAxis->setAxisLineColor(QColor(94,92,97));
// 		pDateTimeAxis->setLabelsColor(QColor(94,92,97));
// 		pDateTimeAxis->setTickCount(3);
// 		pDateTimeAxis->setFormat("mm:ss");
// 		pDateTimeAxis->setGridLineVisible(false);
// 	
// 		m_pSaveChartArea->chart()->removeSeries(m_pSaveAreaSeries);
// 		m_pSaveChartArea->chart()->removeAxis(m_pSaveDateTimeAxis);
// 		m_pSaveChartArea->chart()->setAxisX(pDateTimeAxis);
// 		m_pSaveAreaSeries->attachAxis(pDateTimeAxis);
// 		m_pSaveChartArea->chart()->setAxisY(m_pSaveYAxis);
// 		m_pSaveChartArea->chart()->addSeries(pAreaSeries);
// 		pAreaSeries->attachAxis(pDateTimeAxis);
// 		pAreaSeries->attachAxis(m_pSaveYAxis);
// 	
// 		m_pSaveDateTimeAxis=pDateTimeAxis;
// 		m_pSaveAreaSeries=pAreaSeries;
}

void CEvaluateInfoWidget::EvaluateInfoShowSlot()
{
	this->show();
}

bool CEvaluateInfoWidget::eventFilter(QObject * obj, QEvent *ev)
{
	//if((obj == ui->EquipmentAtttibutes || obj == ui->EquipmentAtttibutes_2 )&& ev->type() == QEvent::Paint)
	//{
	//	painterTopk();
	//}

	return QWidget::eventFilter(obj,ev);
}

void CEvaluateInfoWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
}

void CEvaluateInfoWidget::painterTopk()
{
	
}
