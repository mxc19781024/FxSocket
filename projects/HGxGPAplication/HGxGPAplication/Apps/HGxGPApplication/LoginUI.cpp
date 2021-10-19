#include "LoginUI.h""
#include <QMessageBox>
#include <QString>
#include <QPainter>
#include <QFile>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxServiceQuerier.h>

#include "GxUiFrameworkServiceInterface.h"
#include <QDir>
#include <QtWidgets/QtWidgets>
#include <QDebug>
#include "QtitanDocking.h"
#include "GxProcessor.h"

#include <QProcessEnvironment>
#include <QFileInfo>
#include <FxLoadIndicator/CLoadIndicator.h>
#include <cstdio>

using namespace HGxPFWCore;
using namespace Qtitan;
QTITAN_USE_NAMESPACE

#define  WITHOSG		1

#if WITHOSG
#include <bdpDB/GxRegister.h>
#include <osgViewer/Viewer>
#endif

CLogin::CLogin( QWidget * parent, Qt::WindowFlags flag)
	:QWidget(parent, flag)
	,m_pInput(NULL)
	,m_pTitle(NULL)
	,m_pLE_User(NULL)
	,m_pLE_Passwd(NULL)
	,m_pButton_Login(NULL)
	,m_pButton_UsrImage(NULL)
	,m_pLoginContext(NULL)
	,m_pButton_Exit(NULL)
{
	//添加样式表
	QFile file("D:\Projects\HGxGPAplication\Build_\Apps\HGxGPApplication/login.qss");
	QFileInfo fileInfo(file);
	qDebug() << fileInfo.absoluteFilePath() << "HHHHHHHHHHHHHHHHH";
	file.open(QFile::ReadOnly | QFile::Text);
	char buffer[65536];

	FILE* fp = fopen("D:/Projects/HGxGPAplication/Build_/Apps/HGxGPApplication/login.qss", "r+");
	fread(buffer, sizeof(buffer), 1, fp);
	//printf("%s\n", buffer);

	QByteArray array(buffer);
	//QString string = QString(QLatin1String(buffer));
	QString string(array);
	qDebug() << string << "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";
	this->setStyleSheet(string);
	//if (file.isOpen())
	//{
	//	qDebug() << file.readAll() << "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL";
	//	this->setStyleSheet(file.readAll());

	//}
	//初始化窗口
	setupUi();
	//设置用户头像
	//setUsrImage(":/icon/usr.png");
	//调整登录信息框的位置
	setInputPosition(270,100);
}

CLogin::~CLogin()
{

}

void CLogin::setupUi()
{
	setObjectName("LoginWidget");
	//设置窗体大小
	setMaximumSize(600,400);
	setMinimumSize(600,400);
	//设置窗口背景图
	setBackGroundImage(":/image/2.jpg");
	//去掉窗口标题栏
	setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);

	//添加组件盒子
	m_pInput = new QWidget(this);
	m_pInput->setObjectName(QString("InPut"));
	//m_pInput->setGeometry(0,0, 250, 350);

	//添加用户头像框
	{
		/*m_pButton_UsrImage = new QPushButton(m_pInput);
		m_pButton_UsrImage->setObjectName("UsrImage");
		m_pButton_UsrImage->setFocusPolicy(Qt::NoFocus);
		m_pButton_UsrImage->setFlat(true);
		m_pButton_UsrImage->setGeometry(75,0,100,100);
		m_pButton_UsrImage->setIconSize(QSize(100,100));*/
	}
	
	//添加登录内容
	{
		m_pLoginContext = new QWidget(m_pInput);
		m_pLoginContext->setObjectName("LoginContext");
		//m_pLoginContext->lower();
		m_pLoginContext->setGeometry(0,0,250,220);

		//标题
		m_pTitle = new QLabel(m_pLoginContext);
		m_pTitle->setObjectName("Title");
		m_pTitle->setAlignment(Qt::AlignCenter);
		m_pTitle->setText(QString::fromLocal8Bit("登录"));

		//用户名
		m_pLE_User = new QLineEdit(m_pLoginContext);
		m_pLE_User->setPlaceholderText(QString::fromLocal8Bit("Username"));
		m_pLE_User->setFixedSize(210,30);
		m_pLE_User->setTextMargins(30, 0, 0, 0);
		//添加图标
		{
			QPushButton* pButton = new QPushButton(m_pLE_User);
			pButton->setIcon(QIcon(":/icon/usr.png"));
			pButton->setIconSize(QSize(20,30));
			pButton->setFocusPolicy(Qt::NoFocus);
			pButton->setStyleSheet("border:none");
			pButton->setFlat(true);
		}

		//密码
		m_pLE_Passwd = new QLineEdit(m_pLoginContext);
		m_pLE_Passwd->setPlaceholderText(QString::fromLocal8Bit("Password"));
		m_pLE_Passwd->setFixedSize(210,30);
		m_pLE_Passwd->setEchoMode(QLineEdit::Password);
		m_pLE_Passwd->setTextMargins(30, 0, 0, 0);
		//添加图标
		{
			QPushButton* pButton = new QPushButton(m_pLE_Passwd);
			pButton->setIcon(QIcon(":/icon/passwd.png"));
			pButton->setIconSize(QSize(20,30));
			pButton->setFocusPolicy(Qt::NoFocus);
			pButton->setStyleSheet("border:none");
			pButton->setFlat(true);
		}

		//登录按钮
		m_pButton_Login = new QPushButton(m_pLoginContext);
		m_pButton_Login->setObjectName("LoginButton");
		m_pButton_Login->setText(QString::fromLocal8Bit("确认"));
		m_pButton_Login->setFixedSize(100,30);
		//连接点击信号槽
		connect(m_pButton_Login, SIGNAL(clicked()), this, SLOT(slotLoginButtonClicked()));

		//退出按钮
		m_pButton_Exit = new QPushButton(m_pLoginContext);
		m_pButton_Exit->setObjectName("ExitButton");
		m_pButton_Exit->setText(QString::fromLocal8Bit("退出"));
		m_pButton_Exit->setFixedSize(100,30);
		//连接点击信号槽
		connect(m_pButton_Exit, SIGNAL(clicked()), this, SLOT(slotExitButtonClicked()));
	}

	//添加布局
	{
		QVBoxLayout* pVBoxLayout = new QVBoxLayout(m_pLoginContext);
		m_pLoginContext->setLayout(pVBoxLayout);
		{
			QSpacerItem* pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer1 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QHBoxLayout* pHBoxLayout = new QHBoxLayout();
			pHBoxLayout->addItem(pSpacer1);
			pHBoxLayout->addWidget(m_pTitle);
			pHBoxLayout->addItem(pSpacer2);
			pVBoxLayout->addItem(pSpacer);
			pVBoxLayout->addItem(pHBoxLayout);
		}
		{
			QSpacerItem* pSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QHBoxLayout* pHBoxLayout = new QHBoxLayout();
			pHBoxLayout->addItem(pSpacer1);
			pHBoxLayout->addWidget(m_pLE_User);
			pHBoxLayout->addItem(pSpacer2);
			pVBoxLayout->addItem(pSpacer);
			pVBoxLayout->addItem(pHBoxLayout);
		}
		{
			QSpacerItem* pSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QHBoxLayout* pHBoxLayout = new QHBoxLayout();
			pHBoxLayout->addItem(pSpacer1);
			pHBoxLayout->addWidget(m_pLE_Passwd);
			pHBoxLayout->addItem(pSpacer2);
			pVBoxLayout->addItem(pSpacer);
			pVBoxLayout->addItem(pHBoxLayout);
		}
		{
			QSpacerItem* pSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QHBoxLayout* pHBoxLayout = new QHBoxLayout();
			pHBoxLayout->addItem(pSpacer1);
			pHBoxLayout->addWidget(m_pButton_Login);
			pHBoxLayout->addItem(pSpacer3);
			pHBoxLayout->addWidget(m_pButton_Exit);
			pHBoxLayout->addItem(pSpacer2);
			pVBoxLayout->addItem(pSpacer);
			pVBoxLayout->addItem(pHBoxLayout);
		}
		{
			/*QSpacerItem* pSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QHBoxLayout* pHBoxLayout = new QHBoxLayout();
			pHBoxLayout->addItem(pSpacer1);
			pHBoxLayout->addWidget(m_pButton_Exit);
			pHBoxLayout->addItem(pSpacer2);
			pVBoxLayout->addItem(pSpacer);
			pVBoxLayout->addItem(pHBoxLayout);*/
		}
		{
			QSpacerItem* pSpacer = new QSpacerItem(40, 55, QSizePolicy::Expanding, QSizePolicy::Minimum);
			pVBoxLayout->addItem(pSpacer);
		}
	}
}

CLogin::SLoginInfo CLogin::getLoginInfo()
{
	SLoginInfo UsrInfo;
	UsrInfo.strUsrName = m_pLE_User->text().toLocal8Bit();
	UsrInfo.strPasswd = m_pLE_Passwd->text().toLocal8Bit();
	return UsrInfo;
}

void CLogin::slotLoginButtonClicked()
{
	//检测用户名或密码是否为空
	if (checkLoginData())
	{
		SLoginInfo a = getLoginInfo();
		//验证通过，添加操作
		if (a.strUsrName == "admin" && a.strPasswd == "admin")
		{
			InitStartPlugin();
			//this->close();
		}else if (a.strUsrName != "admin")//用户名不正确
		{
			QMessageBox* pMessage = new QMessageBox( QString::fromLocal8Bit("警告！")
				,QString::fromLocal8Bit("用户名有误，未找到该用户！")
				,QMessageBox::Warning
				,QMessageBox::Ok | QMessageBox::Default
				,0
				,0);
			if (pMessage->exec())
			{
				m_pLE_User->setFocus();
			}
		}else if (a.strPasswd != "admin")//密码不正确
		{
			QMessageBox* pMessage = new QMessageBox( QString::fromLocal8Bit("警告！")
				,QString::fromLocal8Bit("密码错误，请注意区分大小写！")
				,QMessageBox::Warning
				,QMessageBox::Ok | QMessageBox::Default
				,0
				,0);
			if (pMessage->exec())
			{
				m_pLE_Passwd->setFocus();
			}
		}
	}
}

bool CLogin::checkLoginData()
{
	//检查用户名和密码是否为空
	if (m_pLE_User->text().isEmpty() || m_pLE_Passwd->text().isEmpty())
	{
		QMessageBox* pMessage = new QMessageBox( QString::fromLocal8Bit("警告！")
			,QString::fromLocal8Bit("用户名或密码不能为空！")
			,QMessageBox::Warning
			,QMessageBox::Ok | QMessageBox::Default
			,0
			,0);
		if (pMessage->exec())
		{
			if (m_pLE_User->text().isEmpty())
			{
				m_pLE_User->setFocus();
			}
			else if(m_pLE_User->text().isEmpty())
			{
				m_pLE_Passwd->setFocus();
			}
		}
		return false;
	}
	//密码位数检测
	if (m_pLE_Passwd->text().size() < 4)
	{
		QMessageBox* pMessage = new QMessageBox( QString::fromLocal8Bit("警告！")
			,QString::fromLocal8Bit("您输入的密码位数有误，请重新输入！")
			,QMessageBox::Warning
			,QMessageBox::Ok | QMessageBox::Default
			,0
			,0);
		pMessage->exec();
		m_pLE_Passwd->clear();
		m_pLE_Passwd->setFocus();
		return false;
	}
	return true;
}

void CLogin::setBackGroundImage( QString strPath )
{
	//添加背景图
	//this->setAutoFillBackground(true);
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window
		,QBrush(QPixmap(strPath).scaled(this->width(), this->height(), Qt::IgnoreAspectRatio))
		);
	this->setPalette(palette);
	this->setAutoFillBackground(true);
}

void CLogin::setInputPosition( int x, int y)
{
	//设置登录输入框的位置
	m_pInput->move(x, y);
}

void CLogin::setUsrImage( QString strPath )
{
	//设置用户头像
	QPixmap image(strPath);
	//设置所需图片的大小
	QRect rect(0,0,98,98);
	//获取圆形图像
	m_pButton_UsrImage->setIcon(QIcon(getRoundImage(image, rect, rect.width())));
}

QPixmap CLogin::getRoundImage( const QPixmap& image, QRect rect, int radius )
{
	//不处理空数据或错误数据
	if (image.isNull())
	{
		return image;
	}
	int imageWidth = rect.width();
	int imageHeight = rect.height();

	//处理大尺寸的图片，保证图片显示区域的完整
	QPixmap newPixMap = image.scaled(imageWidth
		,(imageHeight == 0 ? imageWidth : imageHeight)
		,Qt::IgnoreAspectRatio
		,Qt::SmoothTransformation
		);
	QPixmap destImage(imageWidth, imageHeight);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	//抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	//图片平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	//将图片裁剪为圆角
	QPainterPath path;
	path.addRoundedRect(rect, radius, radius);
	painter.setClipPath(path); 
	painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
	return destImage;
}

void CLogin::InitStartPlugin()
{
#if WITHOSG
	// 读取配置
	QString strDataPath = "./Data";
	QProcessEnvironment qEnv = QProcessEnvironment::systemEnvironment();

	QFileInfo qFileInfo;
	qFileInfo.setFile(strDataPath);

	if (!qFileInfo.isDir())
	{
		strDataPath = qEnv.value("PLATFORM_DATA");
		qFileInfo.setFile(strDataPath);

		if (!qFileInfo.isDir())
		{
			qDebug() << "NO Data Path";
			return ;
		}
	}

	HGxDB::CGxRegister::getInstance()->setDataPath(strDataPath.toStdString());

	QString strPluginPath = "./PlatformPlugin";
	qFileInfo.setFile(strPluginPath);
	if (!qFileInfo.isDir())
	{
		strPluginPath = qEnv.value("PLATFORM_OBJECT");
		qFileInfo.setFile(strPluginPath);

		if (!qFileInfo.isDir())
		{
			qDebug() << "NO Plugin Path";
			return ;
		}
	}

	HGxDB::CGxRegister::getInstance()->setPluginPath(strPluginPath.toStdString());

	bool bRes = HGxDB::CGxRegister::getInstance()->getConfig().isValid();

	/// 设置翻译系统对话框为中文
	QTranslator *tran = new QTranslator;
	bool bOk = tran->load(strDataPath + "/translations/qt_zh_CN.qm");
	if (bOk)
	{
		qApp->installTranslator(tran);
	}

	QTranslator *tranChinese = new QTranslator;
	bOk = tranChinese->load(strDataPath + "/translations/Chinese.qm");
	if (bOk)
	{
		qApp->installTranslator(tranChinese);
	}

	osgViewer::Viewer viewer;

	CGxPluginFrameworkCore::fwLauncher()->addSearchPath(QDir("./plugins/Release").absolutePath());
	CGxPluginFrameworkCore::fwLauncher()->addSearchPath(QDir("./plugins/Release").absolutePath());
	CGxPluginFrameworkCore::fwLauncher()->startAll();
	CGxServiceQuerier<CGxUiFrameworkServiceInterface> uiFrameworkService(CGxPluginFrameworkCore::fwLauncher()->getPluginContext());

	if (!uiFrameworkService/* || !uiFrameworkService.valid()*/)
	{
		qDebug() << "ui framework not valid";
		exit(-1);
	}
	//W00085
	//设置自定义样式
	//  	uiFrameworkService->setCustomStyle(CGxGuiStyle::Dark);
	//   	if (uiFrameworkService->menuBar())
	//   	{
	//   		uiFrameworkService->menuBar()->addMenu(uiFrameworkService->dockMenu());
	//   		//uiFrameworkService->menuBar()->addMenu(uiFrameworkService->styleMenu());
	//   	}
	//uiFrameworkService->mainWindow()->setWindowIcon(QIcon());
	//uiFrameworkService->mainWindow()->setWindowTitle(QStringLiteral("地理信息平台"));

	uiFrameworkService->show();

	CGxProcessor processor;	//事件处理

#elif 0
	DockMainWindow mainwindow;
	DockWidgetPanel* panel = mainwindow.addDockPanel("abc", Qtitan::DockPanelArea::AllDockPanelAreas);
	panel->setWidget(new QLabel("abc"));
	panel->setFloat(true);

	DockWidgetPanel* panel1 = mainwindow.addDockPanel("def", Qtitan::AllDockPanelAreas);
	panel1->setFloat(true);
	panel1->setWidget(new QLabel("def"));

	DockDocumentPanel* doc0 = mainwindow.addDocumentPanel("abc.h");
	doc0->setWidget(new QTableWidget(5, 2));

	//   DockDocumentPanel* doc1 = mainwindow.addDocumentPanel("abc.cpp");
	//   doc1->setWidget(new QTableWidget(2, 5));

	mainwindow.show();

#else 

	DockMainWindow mainwindow;
	//DockWidgetPanel* panel = mainwindow.addDockPanel("abc", Qtitan::DockPanelArea::LeftDockPanelArea);
	DockWidgetPanel* panel = new DockWidgetPanel("abc", mainwindow.dockPanelManager());
	panel->setWidget(new QLabel("abc"));
	mainwindow.dockPanelManager()->insertDockPanel(panel, Qtitan::LeftDockPanelArea);

	//mainwindow.addDockPanel()

	//DockWidgetPanel* panel1 = mainwindow.addDockPanel("def", Qtitan::AllDockPanelAreas);


	DockWidgetPanel* panel1 = new DockWidgetPanel("def", mainwindow.dockPanelManager());
	panel1->setWidget(new QLabel("def"));
	mainwindow.dockPanelManager()->insertDockPanel(panel1, Qtitan::LeftDockPanelArea);
	panel1->setFloat(true);

	mainwindow.resize(1000, 800);
	mainwindow.show();


#endif
}

void CLogin::setInputAligenCenter()
{
	//设置登录输入框的大小及位置
	m_pInput->setGeometry(0, 0, 250, 350);
	
	QPoint globalPos = m_pInput->parentWidget()->mapToGlobal(QPoint(0,0));//父窗口绝对坐标
	int x = globalPos.x() + (m_pInput->parentWidget()->width() - m_pInput->width()) / 2;
	int y = globalPos.y() + (m_pInput->parentWidget()->height() - m_pInput->height()) / 2;
	m_pInput->move(x, y);
}

void CLogin::slotExitButtonClicked()
{
	this->close();
}

void CLogin::mousePressEvent( QMouseEvent *e )
{
	//鼠标左键释放
	if (e->button() == Qt::LeftButton)
	{
		last = e->globalPos();
	}
}

void CLogin::mouseMoveEvent( QMouseEvent *e )
{
	//鼠标左键按下并移动
	if(e->buttons() == Qt::LeftButton)
	{
		int dx = e->globalX() - last.x();
		int dy = e->globalY() - last.y();
		last = e->globalPos();
		move(x() + dx, y() + dy);
	}
}

void CLogin::mouseReleaseEvent( QMouseEvent *e )
{
	//鼠标左键按下
	if (e->button() == Qt::LeftButton)
	{
		int dx = e->globalX() - last.x();
		int dy = e->globalY() - last.y();
		move(x() + dx, y() + dy);
	}
}

