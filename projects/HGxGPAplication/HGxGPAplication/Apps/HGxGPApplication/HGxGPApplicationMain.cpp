/*=============================================================================

  Library: CTK

  Copyright (c) German Cancer Research Center,
	Division of Medical and Biological Informatics

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#include <QApplication>

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
#include "LoginUI.h"
#include "GxProcessor.h"

using namespace HGxPFWCore;
using namespace Qtitan;
QTITAN_USE_NAMESPACE

#define  WITHOSG		1

#if WITHOSG
#include <bdpDB/GxRegister.h>
#include <osgViewer/Viewer>
#endif
#include "bdpUtil/GxSimulationMode.h"
#include "bdpUtil/GxSimulationTimer.h"
#include "bdpUtil/GxTimeManager.h"

int main(int argv, char** argc)
{
	QApplication app(argv, argc);
	// 名称
	app.setApplicationName("Demo_platform");
#if WITHOSG

	//=====================================================1.设置平台数据路径=====================================================//

	//设置位置
	QString strDataPath = "./Data";
	//获取进程间的环境 ---- 进程环境变量的保存
	QProcessEnvironment qEnv = QProcessEnvironment::systemEnvironment();

	QFileInfo qFileInfo;
	//设置文件的路径
	qFileInfo.setFile(strDataPath);

	//不是文件夹
	if (!qFileInfo.isDir())
	{
		//返回   PLATFORM_DATA  环境变量
		strDataPath = qEnv.value("PLATFORM_DATA");

		//设置   PLATFORM_DATA  环境的位置
		qFileInfo.setFile(strDataPath);

		//判断是否为目录
		if (!qFileInfo.isDir())
		{
			qDebug() << "NO Data Path";
			return false;
		}
	}

	//设置环境变量   通过环境变量找到数据格式
	HGxDB::CGxRegister::getInstance()->setDataPath(strDataPath.toStdString());

	//=====================================================2.设置平台插件路径=====================================================//

	QString strPluginPath = "./plugins";

	qFileInfo.setFile(strPluginPath);
	if (!qFileInfo.isDir())
	{
		//获取  PLATFORM_OBJECT   的地址
		strPluginPath = qEnv.value("PLATFORM_OBJECT");

		//设置环境变量地址
		qFileInfo.setFile(strPluginPath);

		if (!qFileInfo.isDir())
		{
			qDebug() << "NO Plugin Path";
			return false;
		}
	}

	//===============================================3.给平台设置插件的环境变量地址================================================//
	HGxDB::CGxRegister::getInstance()->setPluginPath(strPluginPath.toStdString());

	{
		//启动外部程序，并可以通信
		//QProcess process;
		//启动
		//process.start("FrameLogin.exe");
	}

	//=========================================================4.登录===========================================================//

	CLogin login;
	login.show();

	//查看平台注册器的默认路径是否配置成功
	bool bRes = HGxDB::CGxRegister::getInstance()->getConfig().isValid();
	if (bRes)
	{
		qDebug() << " success ";
	}
	else
	{
		qDebug() << " no success ";
	}

	{
		// 	/// 设置翻译系统对话框为中文
		// 	QTranslator *tran = new QTranslator;
		// 	bool bOk = tran->load(strDataPath + "/translations/qt_zh_CN.qm");
		// 	if (bOk)
		// 	{
		// 		qApp->installTranslator(tran);
		// 	}
		// 
		// 	QTranslator *tranChinese = new QTranslator;
		// 	bOk = tranChinese->load(strDataPath + "/translations/Chinese.qm");
		// 	if (bOk)
		// 	{
		// 		qApp->installTranslator(tranChinese);
		// 	}
	}

	//===================================================5.给平台设置插件的地址====================================================//
	//框架核心接口类                        添加                      插件路径       绝对路径 
	CGxPluginFrameworkCore::fwLauncher()->addSearchPath(QDir("./plugins/Release").absolutePath());

	//CGxPluginFrameworkCore::fwLauncher()->addSearchPath(QDir("./plugins").absolutePath());
	//CGxPluginFrameworkCore::fwLauncher()->start("com_henggetec_uiLogin", ctkPlugin::START_TRANSIENT, CGxPluginFrameworkCore::fwLauncher()->getPluginContext());

	//加载全部插件
	CGxPluginFrameworkCore::fwLauncher()->startAll();

	//==================================================6.创建并设置模拟仿真器====================================================//

	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulationMode(HGxUtil::E_DEDUCE_MODE);

	//====================================================7.获取界面框架服务======================================================//

	CGxServiceQuerier<CGxUiFrameworkServiceInterface> uiFrameworkService(CGxPluginFrameworkCore::fwLauncher()->getPluginContext());

	if (!uiFrameworkService/* || !uiFrameworkService.valid()*/)
	{
		qDebug() << "ui framework not valid";
		exit(-1);
	}

	//W00085
	//设置自定义样式
		//uiFrameworkService->setCustomStyle(CGxGuiStyle::Dark);
		//if (uiFrameworkService->menuBar())
		//{
			//uiFrameworkService->menuBar()->addMenu(uiFrameworkService->dockMenu());
			//uiFrameworkService->menuBar()->addMenu(uiFrameworkService->styleMenu());
		//}
	//uiFrameworkService->mainWindow()->setWindowIcon(QIcon());
	//uiFrameworkService->mainWindow()->setWindowTitle(QStringLiteral("地理信息平台"));

	uiFrameworkService->show();

	//===================================================8.服务框架设置=========================================================//

	CGxProcessor processor;

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
	return app.exec();

}
