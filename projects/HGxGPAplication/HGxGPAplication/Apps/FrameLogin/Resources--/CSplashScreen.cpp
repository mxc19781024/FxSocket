#include "CSplashScreen.h"

#include <QFile>
#include <QMovie>
#include <QPainter>
#include <QLocalSocket>
#include <QTextStream>
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>

#if WIN32
#include <windows.h>
#include <TlHelp32.h>
#endif

CSplashScreen::CSplashScreen(QWidget *parent)
	: QWidget(parent)
	, m_pUdpSocket(NULL)
	, m_bMousePressed(false)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	// 设置启动界面样式
	QFile file(":/css_splash_screen/Resources/startWgt.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString strStyle = file.readAll();
		setStyleSheet(strStyle);
	}
	//设置界面信息
//	QFile file1(":/txt_splash_screen/Resources/info.txt");
//	if (file1.open(QIODevice::ReadOnly | QIODevice::Text))
//	{
//#if (defined(_WIN32) && _MSC_VER < 1700)
//		ui.pTextLbl->setText(QString::fromUtf8(file1.readAll()));
//#else
//		ui.pTextLbl->setText(QString(file1.readAll()));
//#endif
//		
//	}
	// 创建UDP套接字，绑定本地LocalHost和端口号
	m_pUdpSocket = new QUdpSocket(this);
	m_pUdpSocket->bind(QHostAddress::LocalHost, 77777,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	connect(m_pUdpSocket, SIGNAL(readyRead()),
		this, SLOT(socketReayReadHandler()));

	// 播放GIF
	m_pMovie = new QMovie(":/images_splash_screen/Resources/loading.gif");
	ui.pGifLbl->setMovie(m_pMovie);
	m_pMovie->start();	
	m_pMovie->setSpeed(160);

	// 设置动画，时间2秒，GIF图右移
	m_pAnimation = new QPropertyAnimation(ui.pGifLbl, "geometry");
	m_pAnimation->setEasingCurve(QEasingCurve::InOutQuart);
	m_pAnimation->setDuration(1000);
	m_pAnimation->setStartValue(QRect(51, 44, 300, 300));
	m_pAnimation->setEndValue(QRect(width() - 300 - 51, 44, 300, 300));
	connect(m_pAnimation, SIGNAL(finished()), this, SLOT(animationFinishedSlot()));

	// 启动定时器， 用于检测主进程
	m_pTimer = new QTimer;
	connect(m_pTimer, SIGNAL(timeout()),this, SLOT(checkMainProcessSlot()));
	m_pTimer->start(100);
}

CSplashScreen::~CSplashScreen()
{
	// 关闭套接字
	m_pUdpSocket->close();
	delete m_pUdpSocket;
	m_pUdpSocket = NULL;
}

void CSplashScreen::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	if (event->buttons() | Qt::LeftButton)
	{
		m_bMousePressed = true;
		m_mousePressPt = event->pos();
	}
}

void CSplashScreen::mouseMoveEvent(QMouseEvent *event)
{
	QWidget::mouseMoveEvent(event);
	if ((event->buttons() | Qt::LeftButton) && m_bMousePressed)
	{
		QPoint mouseMovePos = event->globalPos();
		QRect availGeo = QApplication::desktop()->availableGeometry(QApplication::desktop()->screenNumber(mouseMovePos));
		mouseMovePos.setX(qBound(availGeo.x(), mouseMovePos.x(), availGeo.x() + availGeo.width()));
		mouseMovePos.setY(qBound(availGeo.y(), mouseMovePos.y(), availGeo.y() + availGeo.height()));
		move(mouseMovePos - m_mousePressPt);
	}
}

void CSplashScreen::mouseReleaseEvent(QMouseEvent *event)
{
	QWidget::mouseReleaseEvent(event);
	m_bMousePressed = false;
}

void CSplashScreen::animationFinishedSlot()
{
	// 动画结束
	if(m_pAnimation->currentValue() == m_pAnimation->endValue())
	{ 
		// 发送消息，可以进入主界面
		QUdpSocket udpSocket;
		QByteArray bytArray("START_MAINWINDOW");
		udpSocket.writeDatagram(bytArray,QHostAddress::LocalHost,66666);
		m_pTimer->stop();
		// 退出
		qApp->quit();
	}
}

void CSplashScreen::socketReayReadHandler()
{
	while (m_pUdpSocket->hasPendingDatagrams()) 
	{
		// 读取消息，判断消息是否为插件已加载完
		QByteArray datagram;
		datagram.resize(m_pUdpSocket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;

		m_pUdpSocket->readDatagram(datagram.data(), datagram.size(),
			&sender, &senderPort);
		
		qDebug() << QString(datagram);
		if (QString(datagram) == "LOAD_PLIGINS_END")
		{
			// 动画启动
			m_pAnimation->start();
			//m_pMovie->stop();
			ui.pWgt->hide();
		}
		else
		{
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
			ui.pLoadLbl->setText(QString::fromLocal8Bit(datagram));
#pragma execution_character_set("utf-8")
#else
			ui.pLoadLbl->setText(datagram);
#endif
		}
	}
}

void CSplashScreen::checkMainProcessSlot()
{
#ifdef _WIN32
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		DWORD id = 0;
		HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!Process32First(hProcessSnap, &pe32))
		{
			return;
		}
		bool bFind = false;
		while (1)
		{
			pe32.dwSize = sizeof(PROCESSENTRY32);
			if (!Process32Next(hProcessSnap, &pe32))
			{
				break;
			}
			if (strcmp(pe32.szExeFile, "FreeXGisStudio.exe") == 0)
			{
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			qApp->quit();
		}
		CloseHandle(hProcessSnap);
#else
#ifdef Q_OS_LINX
	
#endif
#endif
}
