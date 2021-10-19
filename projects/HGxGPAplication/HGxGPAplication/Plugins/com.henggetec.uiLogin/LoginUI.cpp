#include "LoginUI.h""
#include <QMessageBox>
#include <QString>
#include <QPainter>
#include <QStyleOption>
#include <QFile>
#include <QDebug>
#include "GxPluginFrameworkCore.h"
#include "GxEventHelperInterface.h"
#include "GxPluginFwLauncherInterface.h"
#include "GxProcessor.h"
#include "GxUiFrameworkServiceInterface.h"
#include "GxServiceQuerier.h"
#include "bdpDB\GxLogFunc.h"
#include "gdpScene/layer/GxLayerManager.h"
#include <gdpScene/layer/GxLayerConfigFactory.h>
#include "bdpDB/GxRegister.h"
#include "bdpDB/GxLogFunc.h"
#include "bdpDB/GxTinyXml.h"
#include "QApplication"
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif
CLogin::CLogin(ctkPluginContext *context, QWidget * parent, Qt::WindowFlags flag)
	:QWidget(parent, flag)
{

	m_pUdpSocket = new QUdpSocket(this);
	m_pUdpSocket->bind(QHostAddress::LocalHost, 66666, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	connect(m_pUdpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));

	ctkDictionary dic;
	context->registerService<ctkEventHandler>(this, dic);

	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/LoginFinished", HGxFWLauncherInstance.getPluginContext());
	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/Progerss", HGxFWLauncherInstance.getPluginContext());

	//连接点击信号槽
	connect(this, SIGNAL(uiFrameworkShowSignal()), this, SLOT(slotuiFrameworkShow()));

	CGxProcessor *processor = new CGxProcessor();	//事件处理
	//连接点击信号槽
	connect(this, SIGNAL(pluginStartSignal()), processor, SLOT(slotPluginStart()));
	//连接点击信号槽
	connect(this, SIGNAL(loadProgressSignal(int, QString)), this, SLOT(slotLoadProgress(int, QString)));
}

CLogin::~CLogin()
{

}


void CLogin::handleEvent(const ctkEvent &event)
{
	if (event.getTopic() == "event/LoginFinished")
	{
		QString  strMessage;
		strMessage = QString::number(100) + "-" + "初始化成功";
		QByteArray bytArray(strMessage.toLocal8Bit());
		QUdpSocket udpsocket;
		udpsocket.writeDatagram(bytArray, QHostAddress::LocalHost, 77777);
		// 在存在一个场景时，将视点同步按钮置为不可用状态
		emit uiFrameworkShowSignal();
		//this->close();
	}
	else if(event.getTopic() == "event/Progerss")
	{
		int iValue = event.getProperty("iValue").toInt();
		QString  strMessage = event.getProperty("strMessage").toString();
		strMessage = QString::number(iValue) + "-" + strMessage;
		QByteArray bytArray(strMessage.toLocal8Bit());
		QUdpSocket udpsocket;
		udpsocket.writeDatagram(bytArray, QHostAddress::LocalHost, 77777);
// 		emit loadProgressSignal(iValue, strMessage);
// 		qApp->processEvents();
	}
}

void CLogin::slotuiFrameworkShow()
{
	HGxDB::CGxTiXmlDocument xmlDoc;

	// 0. 打开文件
	std::string strFilePath = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("Test/Example_Win32.fws");
	if (!xmlDoc.LoadFile(strFilePath.c_str(), HGxDB::E_TIXML_ENCODING_UTF8))
	{

	}

	HGxDB::CGxTiXmlElement* pEleTitle = xmlDoc.FirstChildElement("Layers");
	HGxDB::CGxTiXmlElement* pEleScene = pEleTitle->FirstChildElement("scene");

	HGxScene::CGxLayerManager *m_pLayerManager = HGxScene::CGxLayerManager::getOrCreateLayerManager("3D");
	m_pLayerManager->load(pEleScene);

	{
		osg::ref_ptr<HGxScene::CGxLayerModelConfig> pLayerCfg = m_pLayerManager->layerConfigFactory()->createModelLayerConfig(HGxScene::SLayerOption(tr("line_world").toUtf8().data(),
			"ogr", (HGX_FILE_CONFIG().getFullPath("shpFile/world/line_world.shp"))));
		if (pLayerCfg.valid())
		{
			pLayerCfg->featureSymbol().lineColor() = osg::Vec4f(0, 1, 0.588, 1);
			//pLayerCfg->featureSymbol().fillColor() = osg::Vec4f(1.0, 253 / 255.0, 246 / 255.0, 0.3);
			m_pLayerManager->addLayer(m_pLayerManager->createLayer(*pLayerCfg.get()));
		}
	}

	{
// 		osg::ref_ptr<HGxScene::CGxLayerModelConfig> pLayerCfg = m_pLayerManager->layerConfigFactory()->createModelLayerConfig(HGxScene::SLayerOption(tr("point_caption").toUtf8().data(),
// 			"ogr", (HGX_FILE_CONFIG().getFullPath("shpFile/world/point_caption.shp"))));
// 		if (pLayerCfg.valid())
// 		{
// 			pLayerCfg->featureSymbol().pointIconPath() = (HGX_FILE_CONFIG().getFullPath("texture/sysTemplate/guojia_03.png"));
// 			pLayerCfg->featureSymbol().pointIconSize() = 24;
// 			pLayerCfg->featureSymbol().pointIconVisible() = true;
// 			pLayerCfg->featureSymbol().fontName() = (HGX_FILE_CONFIG().getFullPath("fonts/simsun.ttc"));
// 			pLayerCfg->featureSymbol().fontColor() = osg::Vec4(0.0, 0.0, 0.0, 1.0);
// 			pLayerCfg->featureSymbol().fontSize() = 12;
// 
// 			m_pLayerManager->addLayer(m_pLayerManager->createLayer(*pLayerCfg.get()));
// 		}

	}
	this->close();
	HGxPFWCore::CGxServiceQuerier<CGxUiFrameworkServiceInterface> uiFrameworkService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());

	if (!uiFrameworkService/* || !uiFrameworkService.valid()*/)
	{
		qDebug() << "ui framework not valid";
		this->close();
		exit(-1);
	}

	if (uiFrameworkService)
	{
		uiFrameworkService->mainWindow()->show();
		uiFrameworkService->move(QPoint(0, 0));
	}
}

void CLogin::slotLoadProgress(int iValue, QString strMessage)
{
	SLoginData sLoginData;
	sLoginData.iValue = iValue;
	sLoginData.strMessage = strMessage.toStdString();
	int nStructSize = sizeof(SLoginData);
	int nSize = nStructSize;
	char* pData = new char[nSize];
	memcpy(pData, &sLoginData, nStructSize);
	QByteArray dataArray = QByteArray::fromRawData(pData, nSize);
	QUdpSocket udpsocket;
	udpsocket.writeDatagram(dataArray, QHostAddress::LocalHost, 77777);
}

void CLogin::readFortune()
{
	while (m_pUdpSocket->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(m_pUdpSocket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;

		m_pUdpSocket->readDatagram(datagram.data(), datagram.size(),
			&sender, &senderPort);

		if (QString(datagram) == "START_MAINWINDOW")
		{
			emit pluginStartSignal();
		}
	}
}

