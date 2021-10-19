#include "LoginUI.h""
#include <QMessageBox>
#include <QString>
#include <QPainter>
#include <QStyleOption>
#include <QFile>
#include <QDebug>
#include "QStyle"

#if WIN32
#include <windows.h>
#include <TlHelp32.h>
#endif
#include "QCoreApplication"


CLogin::CLogin(QWidget * parent, Qt::WindowFlags flag)
	:QWidget(parent, flag)
	,m_pInput(NULL)
	,m_pTitle(NULL)
	,m_pLE_User(NULL)
	,m_pLE_Passwd(NULL)
	,m_pButton_Login(NULL)
	,m_pButton_UsrImage(NULL)
	,m_pLoginContext(NULL)
	,m_pButton_Exit(NULL)
	,m_pHalvingLine(NULL)
{
	//添加样式表
	QFile file(":/Resource/login.css");
	if (file.open(QFile::ReadOnly | QFile::Text))
	{
		this->setStyleSheet(file.readAll());
		file.close();
	}
	//初始化窗口
	setupUi();

	//调整登录信息框的位置
	setInputPosition(410,100);

	//设置进度条的值
	setProgerssBarValue(24);

	//设置进度条上方提示信息
	setPromptMessage("loading");

	//连接点击信号槽
	connect(this, SIGNAL(uiFrameworkShowSignal()), this, SLOT(slotuiFrameworkShow()));
	//连接点击信号槽
	connect(this, SIGNAL(loadProgressSignal(int, QString)), this, SLOT(slotLoadProgress(int, QString)));

	// 创建UDP套接字，绑定本地LocalHost和端口号
	m_pUdpSocket = new QUdpSocket(this);
	m_pUdpSocket->bind(QHostAddress::LocalHost, 77777, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	connect(m_pUdpSocket, SIGNAL(readyRead()),
		this, SLOT(socketReayReadHandler()));

	// 启动定时器， 用于检测主进程
	m_pTimer = new QTimer;
	//函数？？？

	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(checkMainProcessSlot()));
	m_pTimer->start(100);
}

CLogin::~CLogin()
{

}

void CLogin::setupUi()
{
	setObjectName("LoginWidget");
	//设置窗体大小
	setMaximumSize(813,475);
	setMinimumSize(813,475);
	//设置背景
	setAttribute(Qt::WA_TranslucentBackground);
	//去掉窗口标题栏
	setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
	//获得焦点锁定
	setFocus(Qt::MouseFocusReason);

	//添加组件盒子 ---登录窗口widget
	m_pInput = new QWidget(this);
	m_pInput->setObjectName(QString("InPut"));
	m_pInput->setMaximumSize(330,284);
	m_pInput->setMinimumSize(330,284);

	//添加公司logo
	{
		m_pLogo = new QLabel(this);
		m_pLogo->setGeometry(20, 20, 89.6, 22.4);
		m_pLogo->setObjectName("Logo");
		QImage ImgLogo(":/Resource/image/logo.png");
		QPixmap Pixmap = QPixmap::fromImage(ImgLogo);
		//将图片按照label的大小进行设置
		QPixmap fitPixmap = Pixmap.scaled(m_pLogo->width(), m_pLogo->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		m_pLogo->setPixmap(fitPixmap);
	}

	//添加用户头像框
	{
		//m_pButton_UsrImage = new QPushButton(m_pInput);
		//m_pButton_UsrImage->setObjectName("UsrImage");
		//m_pButton_UsrImage->setFocusPolicy(Qt::NoFocus);
		//m_pButton_UsrImage->setFlat(true);
		//m_pButton_UsrImage->setGeometry(75,0,100,100);
		//m_pButton_UsrImage->setIconSize(QSize(100,100));
	}
	
	//添加登录内容
	{
		//登录widget
		m_pLoginContext = new QWidget(m_pInput);
		m_pLoginContext->setObjectName("LoginContext");
		m_pLoginContext->setGeometry(3,2,320,278);

		//标题 -- 加入登录widget中  对正中心
		m_pTitle = new QLabel(m_pLoginContext);
		m_pTitle->setObjectName("Title");
		m_pTitle->setAlignment(Qt::AlignCenter);
		m_pTitle->setText(QString::fromLocal8Bit("登录"));

		//分割线  --加入登录widget中   对正中心
		m_pHalvingLine = new QLabel(m_pLoginContext);
		m_pHalvingLine->setObjectName("HalvingLine");
		m_pHalvingLine->setAlignment(Qt::AlignCenter);
		m_pHalvingLine->setMaximumSize(154,1);
		m_pHalvingLine->setMinimumSize(154,1);

		//用户名    --加入登录widget中   设置边限左30
		m_pLE_User = new QLineEdit(m_pLoginContext);
		m_pLE_User->setText(QString::fromLocal8Bit("admin"));
		m_pLE_User->setPlaceholderText(QString::fromLocal8Bit("用户名"));
		//设置最大最小大小都为242,38
		m_pLE_User->setFixedSize(242,38);
		//设置边限30
		m_pLE_User->setTextMargins(30, 0, 0, 0);
		//添加图标
		{
			QPushButton* pButton = new QPushButton(m_pLE_User);
			//设置位置大小
			pButton->setGeometry(5, 10, 20, 20);
			//设置图片
			pButton->setIcon(QIcon(":/Resource/image/yonghutubiao.png"));
			//设置大小
			pButton->setIconSize(QSize(20,20));
			//取消焦点锁定
			pButton->setFocusPolicy(Qt::NoFocus);
			//边框取消
			pButton->setStyleSheet("border:none");
			pButton->setFlat(true);
		}

		//同样操作适用于下边的密码配置

		//密码
		m_pLE_Passwd = new QLineEdit(m_pLoginContext);
		m_pLE_Passwd->setPlaceholderText(QString::fromLocal8Bit("输入密码"));
		m_pLE_Passwd->setFixedSize(242,38);
		//设置密码填入方式
		m_pLE_Passwd->setEchoMode(QLineEdit::Password);
		m_pLE_Passwd->setTextMargins(30, 0, 0, 0);
		//添加图标
		{
			QPushButton* pButton = new QPushButton(m_pLE_Passwd);
			pButton->setGeometry(5, 10, 20, 20);
			pButton->setIcon(QIcon(":/Resource/image/mimatubiao.png"));
			pButton->setIconSize(QSize(20,20));
			pButton->setFocusPolicy(Qt::NoFocus);
			pButton->setStyleSheet("border:none");
			pButton->setFlat(true);
		}

		//登录按钮
		m_pButton_Login = new QPushButton(m_pLoginContext);
		m_pButton_Login->setObjectName("LoginButton");
		m_pButton_Login->setText(QString::fromLocal8Bit("确认"));
		m_pButton_Login->setFixedSize(110,31);
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
		//垂直布局 --- 登录widget
		QVBoxLayout* pVBoxLayout = new QVBoxLayout(m_pLoginContext);
		//设置垂直布局到登录widget
		m_pLoginContext->setLayout(pVBoxLayout);
		{
			/*

			  ---放置方式
			         空白
			  空白   登录   空白

			*/

			//空白空间--填充                        w    h               水平大小策略             垂直大小策略
			QSpacerItem* pSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer1 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer2 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

			//水平布局
			QHBoxLayout* pHBoxLayout = new QHBoxLayout();

			//放置空白区间
			pHBoxLayout->addItem(pSpacer1);

			//放置 登录 标题
			pHBoxLayout->addWidget(m_pTitle);

			//添加空白
			pHBoxLayout->addItem(pSpacer2);

			//垂直布局加空白
			pVBoxLayout->addItem(pSpacer);

			//添加到垂直布局中
			pVBoxLayout->addItem(pHBoxLayout);
		}
		{
			/**放置方式：

				空白布局  分割线  空白布局  
				        空白布局
			*/

			//填充空白
			QSpacerItem* pSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer1 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer2 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			
			//水平布局
			QHBoxLayout* pHBoxLayout = new QHBoxLayout();

			//水平布局中添加空白布局
			pHBoxLayout->addItem(pSpacer1);
			//添加分割线
			pHBoxLayout->addWidget(m_pHalvingLine);
			//水平布局中添加空白布局
			pHBoxLayout->addItem(pSpacer2);

			//添加空白
			pVBoxLayout->addItem(pSpacer);

			//添加垂直布局
			pVBoxLayout->addItem(pHBoxLayout);
		}
		{
			/**
				放置方式：

				空白布局  用户名  空白布局
						 空白布局
			*/

			//填充空白
			QSpacerItem* pSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
			QSpacerItem* pSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

			//水平布局
			QHBoxLayout* pHBoxLayout = new QHBoxLayout();

			//添加空白
			pHBoxLayout->addItem(pSpacer1);

			//添加用户名
			pHBoxLayout->addWidget(m_pLE_User);

			//添加空白
			pHBoxLayout->addItem(pSpacer2);

			//添加空白
			pVBoxLayout->addItem(pSpacer);

			//添加垂直布局
			pVBoxLayout->addItem(pHBoxLayout);
		}
		{
			/**
			放置方式：

					空白布局  密码  空白布局
			                空白布局
			*/
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
		{/**
		   放置方式：
				空白布局  登录按钮  空白布局  退出按钮  空白布局
					               空白布局
		*/
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
		{/**
		    放置方式：
		                             空白布局
		 */
			QSpacerItem* pSpacer = new QSpacerItem(40, 30, QSizePolicy::Expanding, QSizePolicy::Minimum);
			pVBoxLayout->addItem(pSpacer);
		}
	}

	//添加进度条
	{
		//显示进度条上方的文字 loading  
		m_pProgerssText = new QLabel(this);
		m_pProgerssText->setGeometry(0, 420, 813, 30);
		m_pProgerssText->setObjectName("ProgerssText");
		m_pProgerssText->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		//进度条
		m_pProgressBar = new QProgressBar(this);
		m_pProgressBar->setObjectName("SystemProgressBar");
		m_pProgressBar->setGeometry(140, 450, 526, 12);
		m_pProgressBar->hide();
		m_pProgerssText->hide();
		/*m_pProgressBar->close();
		m_pProgerssText->close();*/
	}

	//调整登录信息框的位置
	setInputPosition(410,100);
}

CLogin::SLoginInfo CLogin::getLoginInfo()
{
	SLoginInfo UsrInfo;
	UsrInfo.strUsrName = "admin";//m_pLE_User->text().toLocal8Bit();
	UsrInfo.strPasswd = "admin";//m_pLE_Passwd->text().toLocal8Bit();
	return UsrInfo;
}

void CLogin::slotLoginButtonClicked()
{
	m_pInput->close();
	m_pProgressBar->show();
	m_pProgerssText->show();
	// 发送消息，可以进入主界面
	QUdpSocket udpSocket;
	QByteArray bytArray("START_MAINWINDOW");
	udpSocket.writeDatagram(bytArray, QHostAddress::LocalHost, 66666);
	//检测用户名或密码是否为空
	if (checkLoginData())
	{
		SLoginInfo a = getLoginInfo();
		//验证通过，添加操作
		if (a.strUsrName == "admin" && a.strPasswd == "admin")
		{
			//异步发送事件

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
	QPalette palette = this->palette();
	//设置笔刷
	palette.setBrush(QPalette::Window
		,QBrush(QPixmap(strPath).scaled(this->width(), this->height(), Qt::IgnoreAspectRatio))
		);
	//设置
	this->setPalette(palette);
	//支持小部分窗口的背景
	this->setAutoFillBackground(true);
}

void CLogin::setInputPosition( int x, int y)
{
	//设置登录输入框的位置-----widget
	m_pInput->move(x, y);
}

void CLogin::setUsrImage( QString strPath)
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
	/*QPixmap newPixMap = image.scaled(imageWidth,(imageHeight == 0 ? imageWidth : imageHeight)
		,Qt::IgnoreAspectRatio	,Qt::SmoothTransformation	);*/

	//Qt::IgnoreAspectRatio 此枚举类型定义了缩放矩形时长宽比的情况。  不需要三目运算符
	QPixmap newPixMap = image.scaled(imageWidth, imageHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);



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

// void CLogin::handleEvent(const ctkEvent &event)
// {
// 	if (event.getTopic() == "event/LoginFinished")
// 	{
// 		// 在存在一个场景时，将视点同步按钮置为不可用状态
// 		emit uiFrameworkShowSignal();
// 		//this->close();
// 	}
// 	else if(event.getTopic() == "event/Progerss")
// 	{
// 		int iValue = event.getProperty("iValue").toInt();
// 		QString  strMessage = event.getProperty("strMessage").toString();
// 		emit loadProgressSignal(iValue, strMessage);
// 	}
// }

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

void CLogin::socketReayReadHandler()
{
	while (m_pUdpSocket->hasPendingDatagrams())
	{
		// 读取消息，判断消息是否为插件已加载完
		QByteArray datagram;
		//设置缓冲的大小
		datagram.resize(m_pUdpSocket->pendingDatagramSize());
		//地址
		QHostAddress sender;
		//发送端口
		quint16 senderPort;
		//							读取的数据       读取的大小       地址      端口
		m_pUdpSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
		//byteArray转QString
		QString qdata = QString::fromLocal8Bit(datagram);

		//数据切割
		QStringList list = qdata.split("-");

		//查看数据的完整性
		if (list.size() == 2)
		{
			//设置进度条
			setProgerssBarValue(list.at(0).toInt());

			//进度条显示文字
			QString str = list.at(1);
			m_pProgerssText->setText(str);//setPromptMessage(list.at(0));

			//进度条拉满触发机制
			if (list.at(0).toInt() == 100)
			{
				qApp->quit();
			}
		}
#pragma execution_character_set("utf-8")
#else
		ui.pLoadLbl->setText(datagram);
#endif
	}
}
/************************************************************************/
/* typedef struct tagPROCESSENTRY32
{
	DWORD   dwSize;					//结构的大小，以字节为单位。在调用Process32First函数之前，将该成员设置为sizeof(PROCESSENTRY32)。如果不初始化dwSize, Process32First会失败。
	DWORD   cntUsage;				//不再使用此成员，并始终设置为零。
	DWORD   th32ProcessID;          // 进程标识符		
	ULONG_PTR th32DefaultHeapID;	//不再使用此成员，并始终设置为零。
	DWORD   th32ModuleID;           //不再使用此成员，并始终设置为零。
	DWORD   cntThreads;				//进程启动的执行线程数。
	DWORD   th32ParentProcessID;    // 创建该进程的进程的标识符(它的父进程)。
	LONG    pcPriClassBase;         // 由该进程创建的任何线程的基本优先级。
	DWORD   dwFlags;				//不再使用此成员，并始终设置为零
	CHAR    szExeFile[MAX_PATH];    // Path进程的可执行文件的名称。要检索可执行文件的完整路径，调用Module32First函数并检查返回的MODULEENTRY32结构的szExePath成员。但是，如果调用进程是32位进程，则必须调用QueryFullProcessImageName函数来检索64位进程的可执行文件的完整路径
} PROCESSENTRY32;                                                                     */
/************************************************************************/

void CLogin::checkMainProcessSlot()
{
#ifdef _WIN32
	//描述在快照在系统地址空间中的流程列表中的条目。
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	DWORD id = 0; 
	//句柄指针                                      包括系统中的所有进程  此参数可以为零以指示当前过程
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//检索系统快照中遇到的第一个过程的信息
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
		
		if (strcmp(pe32.szExeFile, "HGxGPApplication.exe") == 0)
		{
			bFind = true;
			break;
		}
	}
	if (!bFind)
	{
		//qApp->quit();
	}
	CloseHandle(hProcessSnap);
#else
#ifdef Q_OS_LINX

#endif
#endif
}

void CLogin::mousePressEvent(QMouseEvent *e)
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

void CLogin::paintEvent( QPaintEvent* event )
{
	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);//抗锯齿
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CLogin::setProgerssBarValue( int iValue )
{
	m_pProgressBar->setValue(iValue);
}

void CLogin::setPromptMessage(QString strMessage)
{
	m_pProgerssText->setText(strMessage);
}

