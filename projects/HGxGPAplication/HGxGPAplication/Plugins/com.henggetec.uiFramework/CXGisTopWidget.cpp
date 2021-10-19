#include "CXGisTopWidget.h"
#include <QtCore/QCoreApplication>
#include <QToolTip>
#include "QTextFormat"
#include "QTextEdit"
#include "QFile"
#include "QDateTime"
#include "QTimer"
#include "qfontdatabase.h"
#include "bdpDB/GxRegister.h"
#include "QPainter"
#include "GxPluginFwLauncherInterface.h"

namespace HGxGui {
#define TOPMAINWINDOW_STYLE_PATH ":/qss_mainwindow/Resources/qss_mainwindow/topmainstyle.css"

	CXGisTopWidget::CXGisTopWidget(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
		setAttribute(Qt::WA_TranslucentBackground);
		setAttribute(Qt::WA_Hover);
	}

	CXGisTopWidget::~CXGisTopWidget()
	{

	}

	void CXGisTopWidget::setMaxBtnText(QString text)
	{
		//ui.m_pMaxBtn->setToolTip(text);
	}

	void CXGisTopWidget::setAppType(HGxGui::EAppType eAppType)
	{

		InitNormal();

		ui.logo->setStyleSheet("QPushButton{border-image: url(:/images_mainwindow/Resources/images_mainwindow/logo.png);}");
		ui.exit->setStyleSheet("QPushButton{border-image: url(:/images_mainwindow/Resources/images_mainwindow/guanbi.png);}");
		ui.platform_name->setStyleSheet("color:rgba(255,255,255,255);font-family:微软雅黑;font-size:18px;}");
// 		QFile mainwndStyleFile(":/qss_mainwindow/Resources/qss_mainwindow/topstyle.css");
// 		if (mainwndStyleFile.open(QIODevice::ReadOnly | QIODevice::Text))
// 		{
// 			QString strSheet = mainwndStyleFile.readAll();
// 			setStyleSheet(strSheet);
// 		}
	}
	void CXGisTopWidget::paintEvent(QPaintEvent *event)
	{
		QWidget::paintEvent(event);

		QStyleOption opt;
		opt.init(this);
		QPainter painter(this);
		style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	}
	bool CXGisTopWidget::InitNormal()
	{
		initSignalsAndSlots();
		m_pTimer = new QTimer();
		connect(m_pTimer, SIGNAL(timeout()), this, SLOT(SlotUpdataDataTime()));
		m_pTimer->start();
		// 时间字体
		QString m_strSturkopf;
		QString str = QString::fromLocal8Bit(HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("fonts/Let_s_go_Digital_Regular.ttf").data());
		int fontID = QFontDatabase::addApplicationFont(str);
		if (fontID != -1)
		{
			m_strSturkopf = QFontDatabase::applicationFontFamilies(fontID).at(0);
		}

		ui.curTime->setStyleSheet("background: transparent; color:rgba(255,255,255,255);font-size:18px;");
		ui.curTime->setAlignment(Qt::AlignCenter);
		ui.curTime->setFont(QFont(m_strSturkopf));
		return true;
	}

	void CXGisTopWidget::SlotUpdataDataTime()
	{
		QDateTime current_DateTime = QDateTime::currentDateTime();
		ui.curTime->setText(current_DateTime.toString("yyyy/MM/dd hh:mm:ss"));
	}

	bool CXGisTopWidget::InitBigScreen()
	{
		//ui.label_title->setText();
		//ui.frame_left->hide();
		//ui.frame_right->hide();
		return true;
	}

	void CXGisTopWidget::initSignalsAndSlots()
	{
		// 	// 最小化按钮单击
		//connect(ui.logo, SIGNAL(clicked()), this, SLOT(MenuWidgetBtnClickedSlot()));
		// 	// 关闭按钮单击
		connect(ui.exit, SIGNAL(clicked()), this, SLOT(closeBtnClickedSlot()));
	}

	void CXGisTopWidget::closeBtnClickedSlot()
	{
		emit closeAppSignal();
	}

	void CXGisTopWidget::Btn3DClickedSlot()
	{

	}

	void CXGisTopWidget::Btn2DClickedSlot()
	{

	}

	void CXGisTopWidget::Btn2D3DClickedSlot()
	{

	}

}


