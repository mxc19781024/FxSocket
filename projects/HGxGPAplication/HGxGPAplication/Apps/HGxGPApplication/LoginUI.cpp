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
	//�����ʽ��
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
	//��ʼ������
	setupUi();
	//�����û�ͷ��
	//setUsrImage(":/icon/usr.png");
	//������¼��Ϣ���λ��
	setInputPosition(270,100);
}

CLogin::~CLogin()
{

}

void CLogin::setupUi()
{
	setObjectName("LoginWidget");
	//���ô����С
	setMaximumSize(600,400);
	setMinimumSize(600,400);
	//���ô��ڱ���ͼ
	setBackGroundImage(":/image/2.jpg");
	//ȥ�����ڱ�����
	setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);

	//����������
	m_pInput = new QWidget(this);
	m_pInput->setObjectName(QString("InPut"));
	//m_pInput->setGeometry(0,0, 250, 350);

	//����û�ͷ���
	{
		/*m_pButton_UsrImage = new QPushButton(m_pInput);
		m_pButton_UsrImage->setObjectName("UsrImage");
		m_pButton_UsrImage->setFocusPolicy(Qt::NoFocus);
		m_pButton_UsrImage->setFlat(true);
		m_pButton_UsrImage->setGeometry(75,0,100,100);
		m_pButton_UsrImage->setIconSize(QSize(100,100));*/
	}
	
	//��ӵ�¼����
	{
		m_pLoginContext = new QWidget(m_pInput);
		m_pLoginContext->setObjectName("LoginContext");
		//m_pLoginContext->lower();
		m_pLoginContext->setGeometry(0,0,250,220);

		//����
		m_pTitle = new QLabel(m_pLoginContext);
		m_pTitle->setObjectName("Title");
		m_pTitle->setAlignment(Qt::AlignCenter);
		m_pTitle->setText(QString::fromLocal8Bit("��¼"));

		//�û���
		m_pLE_User = new QLineEdit(m_pLoginContext);
		m_pLE_User->setPlaceholderText(QString::fromLocal8Bit("Username"));
		m_pLE_User->setFixedSize(210,30);
		m_pLE_User->setTextMargins(30, 0, 0, 0);
		//���ͼ��
		{
			QPushButton* pButton = new QPushButton(m_pLE_User);
			pButton->setIcon(QIcon(":/icon/usr.png"));
			pButton->setIconSize(QSize(20,30));
			pButton->setFocusPolicy(Qt::NoFocus);
			pButton->setStyleSheet("border:none");
			pButton->setFlat(true);
		}

		//����
		m_pLE_Passwd = new QLineEdit(m_pLoginContext);
		m_pLE_Passwd->setPlaceholderText(QString::fromLocal8Bit("Password"));
		m_pLE_Passwd->setFixedSize(210,30);
		m_pLE_Passwd->setEchoMode(QLineEdit::Password);
		m_pLE_Passwd->setTextMargins(30, 0, 0, 0);
		//���ͼ��
		{
			QPushButton* pButton = new QPushButton(m_pLE_Passwd);
			pButton->setIcon(QIcon(":/icon/passwd.png"));
			pButton->setIconSize(QSize(20,30));
			pButton->setFocusPolicy(Qt::NoFocus);
			pButton->setStyleSheet("border:none");
			pButton->setFlat(true);
		}

		//��¼��ť
		m_pButton_Login = new QPushButton(m_pLoginContext);
		m_pButton_Login->setObjectName("LoginButton");
		m_pButton_Login->setText(QString::fromLocal8Bit("ȷ��"));
		m_pButton_Login->setFixedSize(100,30);
		//���ӵ���źŲ�
		connect(m_pButton_Login, SIGNAL(clicked()), this, SLOT(slotLoginButtonClicked()));

		//�˳���ť
		m_pButton_Exit = new QPushButton(m_pLoginContext);
		m_pButton_Exit->setObjectName("ExitButton");
		m_pButton_Exit->setText(QString::fromLocal8Bit("�˳�"));
		m_pButton_Exit->setFixedSize(100,30);
		//���ӵ���źŲ�
		connect(m_pButton_Exit, SIGNAL(clicked()), this, SLOT(slotExitButtonClicked()));
	}

	//��Ӳ���
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
	//����û����������Ƿ�Ϊ��
	if (checkLoginData())
	{
		SLoginInfo a = getLoginInfo();
		//��֤ͨ������Ӳ���
		if (a.strUsrName == "admin" && a.strPasswd == "admin")
		{
			InitStartPlugin();
			//this->close();
		}else if (a.strUsrName != "admin")//�û�������ȷ
		{
			QMessageBox* pMessage = new QMessageBox( QString::fromLocal8Bit("���棡")
				,QString::fromLocal8Bit("�û�������δ�ҵ����û���")
				,QMessageBox::Warning
				,QMessageBox::Ok | QMessageBox::Default
				,0
				,0);
			if (pMessage->exec())
			{
				m_pLE_User->setFocus();
			}
		}else if (a.strPasswd != "admin")//���벻��ȷ
		{
			QMessageBox* pMessage = new QMessageBox( QString::fromLocal8Bit("���棡")
				,QString::fromLocal8Bit("���������ע�����ִ�Сд��")
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
	//����û����������Ƿ�Ϊ��
	if (m_pLE_User->text().isEmpty() || m_pLE_Passwd->text().isEmpty())
	{
		QMessageBox* pMessage = new QMessageBox( QString::fromLocal8Bit("���棡")
			,QString::fromLocal8Bit("�û��������벻��Ϊ�գ�")
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
	//����λ�����
	if (m_pLE_Passwd->text().size() < 4)
	{
		QMessageBox* pMessage = new QMessageBox( QString::fromLocal8Bit("���棡")
			,QString::fromLocal8Bit("�����������λ���������������룡")
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
	//��ӱ���ͼ
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
	//���õ�¼������λ��
	m_pInput->move(x, y);
}

void CLogin::setUsrImage( QString strPath )
{
	//�����û�ͷ��
	QPixmap image(strPath);
	//��������ͼƬ�Ĵ�С
	QRect rect(0,0,98,98);
	//��ȡԲ��ͼ��
	m_pButton_UsrImage->setIcon(QIcon(getRoundImage(image, rect, rect.width())));
}

QPixmap CLogin::getRoundImage( const QPixmap& image, QRect rect, int radius )
{
	//����������ݻ��������
	if (image.isNull())
	{
		return image;
	}
	int imageWidth = rect.width();
	int imageHeight = rect.height();

	//�����ߴ��ͼƬ����֤ͼƬ��ʾ���������
	QPixmap newPixMap = image.scaled(imageWidth
		,(imageHeight == 0 ? imageWidth : imageHeight)
		,Qt::IgnoreAspectRatio
		,Qt::SmoothTransformation
		);
	QPixmap destImage(imageWidth, imageHeight);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	//�����
	painter.setRenderHints(QPainter::Antialiasing, true);
	//ͼƬƽ������
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	//��ͼƬ�ü�ΪԲ��
	QPainterPath path;
	path.addRoundedRect(rect, radius, radius);
	painter.setClipPath(path); 
	painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
	return destImage;
}

void CLogin::InitStartPlugin()
{
#if WITHOSG
	// ��ȡ����
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

	/// ���÷���ϵͳ�Ի���Ϊ����
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
	//�����Զ�����ʽ
	//  	uiFrameworkService->setCustomStyle(CGxGuiStyle::Dark);
	//   	if (uiFrameworkService->menuBar())
	//   	{
	//   		uiFrameworkService->menuBar()->addMenu(uiFrameworkService->dockMenu());
	//   		//uiFrameworkService->menuBar()->addMenu(uiFrameworkService->styleMenu());
	//   	}
	//uiFrameworkService->mainWindow()->setWindowIcon(QIcon());
	//uiFrameworkService->mainWindow()->setWindowTitle(QStringLiteral("������Ϣƽ̨"));

	uiFrameworkService->show();

	CGxProcessor processor;	//�¼�����

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
	//���õ�¼�����Ĵ�С��λ��
	m_pInput->setGeometry(0, 0, 250, 350);
	
	QPoint globalPos = m_pInput->parentWidget()->mapToGlobal(QPoint(0,0));//�����ھ�������
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
	//�������ͷ�
	if (e->button() == Qt::LeftButton)
	{
		last = e->globalPos();
	}
}

void CLogin::mouseMoveEvent( QMouseEvent *e )
{
	//���������²��ƶ�
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
	//����������
	if (e->button() == Qt::LeftButton)
	{
		int dx = e->globalX() - last.x();
		int dy = e->globalY() - last.y();
		move(x() + dx, y() + dy);
	}
}

