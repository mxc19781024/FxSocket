#include <QFile>
#include "GxAddOGCWindow.h"
#include <QScrollBar>
//#include <FreeXServiceModelUtilUi/FreeXServiceModelUtilUi.h>
#include "bdpCommon/GxStrUtil.h"

#include "GxServiceModelUtilUi.h"

CGxAddOGCWindow::CGxAddOGCWindow(QWidget *parent)
	: CFxBaseWindow(parent)
	, m_pService(NULL)
	, m_pDataService(new HGxUtil::CGxDataService)
	, m_bDisconnect(true)
{
	//读样式表文件
	QFile file(":/css/Resources/qss/ServerProUtils.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = file.readAll();
		m_pCentralWgt->setStyleSheet(styleSheet().append(str));
		file.close();
	}
	ui.setupUi(m_pCentralWgt);
	(const_cast<QScrollBar *>(ui.pLayerList->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.pLayerList->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	setWindowModality(Qt::WindowModal);
	setWindowTitle(tr("add ogc service"));
	
	QMetaObject::connectSlotsByName(this);

	m_vecLayers.clear();

	initUi();

	resize(600, 450);
}

CGxAddOGCWindow::~CGxAddOGCWindow()
{

}

void CGxAddOGCWindow::initUi()
{
	// 添加OGC服务：WMS，WMTS，WFS，WCS
	ui.pServerTypeCombo->addItems(QStringList()<<"WMS"<<"WMTS"<<"WFS"<<"WCS");
	ui.pServerNameEdit->setText(tr("unnamed"));
	ui.pServerNameEdit->setMaxLength(20);
	ui.pOkBtn->setStyleSheet("color: #ffffff");
}


void CGxAddOGCWindow::setCurrentServer( QString strServerType )
{
	// 设置服务器类型
	ui.pServerTypeCombo->setCurrentIndex(ui.pServerTypeCombo->findText(strServerType));
}

void CGxAddOGCWindow::showEvent( QShowEvent *event )
{
	// 窗口显示的时候初始化窗口
	ui.pServerWebsiteEdit->clear();
	ui.pLayerList->clear();
	ui.pServerNameEdit->setText(tr("unnamed"));
	ui.pServerNameEdit->setErrorMsg("normal");
	ui.pServerWebsiteEdit->setErrorMsg("normal");
}

void CGxAddOGCWindow::keyPressEvent( QKeyEvent *event )
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Space)
	{
		on_pOkBtn_clicked();
	}
	else if(event->key() == Qt::Key_Escape)
	{
		close();
	}

	CFxBaseWindow::keyPressEvent(event);
}

void CGxAddOGCWindow::on_pServerTypeCombo_currentIndexChanged( const QString &text )
{
	// 设置各服务的版本号
	if (text == "WMS")
	{
		ui.pServerVersonCombo->clear();
		ui.pServerVersonCombo->addItems(QStringList() << "1.3.0" << "1.1.1");
	}
	else if (text == "WMTS")
	{
		ui.pServerVersonCombo->clear();
		ui.pServerVersonCombo->addItems(QStringList() << "1.0.0");
	}
	else if (text == "WFS")
	{
		ui.pServerVersonCombo->clear();
		ui.pServerVersonCombo->addItems(QStringList() <<"2.0.0" << "1.0.0");
	}
	else if (text == "WCS")
	{
		ui.pServerVersonCombo->clear();
		ui.pServerVersonCombo->addItems(QStringList() <<  "2.0.1" << "1.0.0");
	}
	else
	{
	     return;
	}
}

void CGxAddOGCWindow::on_pGetLayerBtn_clicked()
{
	ui.pLayerList->clear();


	if (ui.pServerTypeCombo->currentText() == "WMS")
	{
		m_pService = m_pDataService->cretaeWMSService(ui.pServerWebsiteEdit->text().toUtf8().data(), ui.pServerVersonCombo->currentText().toUtf8().data(), ui.pUserNameEdit->text().toUtf8().data(), ui.pUserPasswordEdit->text().toUtf8().data());
	}
	else if (ui.pServerTypeCombo->currentText() == "WMTS")
	{
		m_pService = m_pDataService->cretaeWMTSService(ui.pServerWebsiteEdit->text().toUtf8().data(), ui.pServerVersonCombo->currentText().toUtf8().data(), ui.pUserNameEdit->text().toUtf8().data(), ui.pUserPasswordEdit->text().toUtf8().data());
	}
	else if (ui.pServerTypeCombo->currentText() == "WFS")
	{
		m_pService = m_pDataService->cretaeWFSService(ui.pServerWebsiteEdit->text().toUtf8().data(), ui.pServerVersonCombo->currentText().toUtf8().data(), ui.pUserNameEdit->text().toUtf8().data(), ui.pUserPasswordEdit->text().toUtf8().data());
	}
	else if (ui.pServerTypeCombo->currentText() == "WCS")
	{
		m_pService = m_pDataService->cretaeWCSService(ui.pServerWebsiteEdit->text().toUtf8().data(), ui.pServerVersonCombo->currentText().toUtf8().data(), ui.pUserNameEdit->text().toUtf8().data(), ui.pUserPasswordEdit->text().toUtf8().data());
	}
		
	if (m_pService)
	{
		QCoreApplication::processEvents();

		m_bDisconnect = m_pService->isConnected();
		if (!m_bDisconnect)
		{
			QListWidgetItem *pListItem = new  QListWidgetItem;
			pListItem->setToolTip(tr("geting layer failed,please check service address is correct or network connection is normal!"));
			pListItem->setText(tr("geting layer failed,please check service address is correct or network connection is normal!"));
			ui.pLayerList->clear();
			ui.pLayerList->addItem(pListItem);
		}
		else
		{
			m_pService->refresh(); //获取子图层
			m_vecLayers = m_pService->getServiceLayerList();
			std::vector<std::string> vecFormats = m_pService->getServiceDataFormatList();
			m_serviceFormats.clear();
			for (int i = 0; i < vecFormats.size(); ++i)
			{
				m_serviceFormats.push_back(vecFormats.at(i).c_str());
			}

			if (m_vecLayers.size() == 0)
			{
				QListWidgetItem *pListItem = new  QListWidgetItem;
				pListItem->setText(tr("geting layer failed,please check whether the layer under the service is empty!"));
				ui.pLayerList->clear();
				ui.pLayerList->addItem(pListItem);
				return;
			}

			ui.pLayerList->clear();
			for (int n = 0; n < m_vecLayers.size(); ++n)
			{
				QListWidgetItem *pListItem = new  QListWidgetItem;
				//pListItem->setText(m_vecLayers[n]->getName().c_str());
				pListItem->setText(QString::fromUtf8(m_vecLayers[n]->getName().c_str()));
				ui.pLayerList->addItem(pListItem);
			}
		}
	}
}

void CGxAddOGCWindow::on_pOkBtn_clicked()
{
	if (ui.pServerNameEdit->text() == "")
	{
		ui.pServerNameEdit->setErrorMsg("red");
		return;
	}

	if (ui.pServerWebsiteEdit->text() == "")
	{   
		ui.pServerWebsiteEdit->setErrorMsg("red");
		return;
	}

	on_pGetLayerBtn_clicked();

	SServerPropertyStruct stServerProperty;
	stServerProperty.bConnect = m_bDisconnect;
	stServerProperty.strServerType = ui.pServerTypeCombo->currentText();                                 // 服务类型
	stServerProperty.strServerName = ui.pServerNameEdit->text();                                         // 服务名称
	stServerProperty.strServerVersion = ui.pServerVersonCombo->currentText();                            // 版本信息
	stServerProperty.strServerWebsite = ui.pServerWebsiteEdit->text();									 // 服务网址
	stServerProperty.vecFormats = m_serviceFormats;														 // 服务支持的瓦片类型

 	if (stServerProperty.bConnect)
 	{
 		for (int i = 0; i < m_vecLayers.size(); i++)
 		{
 			SLayerPropertyStruct stLayerProperty;
 			stLayerProperty.strLayerName = QString::fromUtf8(m_vecLayers[i]->getName().c_str());         // 图层名称
 			stLayerProperty.strServerName = ui.pServerNameEdit->text();                                  // 服务名称
 			stLayerProperty.strServerType = ui.pServerTypeCombo->currentText();                          // 服务属性
 			stLayerProperty.strServerWebsite = stServerProperty.strServerWebsite;					     // 图层网址
 			stLayerProperty.strDriveName = stServerProperty.strServerType;							     // 图层服务类型
 			stLayerProperty.strLayerFormats = m_serviceFormats;										     // 图层支持的瓦片格式
 
 			if (stLayerProperty.strServerType == "WFS")
 			{
 				stLayerProperty.strLayerType = tr("vector layer");
 			}
 			else
 			{
 				stLayerProperty.strLayerType = tr("image layer");
 			}
 			QString strLayerInfo = QStringLiteral("名称:\n%1\n\n摘要:\n%2\n\n包装盒:\n%3\n%4\n\n支持的EPSG、CRS:\n%5，%6\n")\
 				.arg(stLayerProperty.strLayerName).arg(QStringLiteral("未知"))\
 				.arg(m_vecLayers[i]->getLowerCorner().c_str()).arg(m_vecLayers[i]->getUpperCorner().c_str())\
 				.arg(m_vecLayers[i]->getEPSGCode().c_str()).arg(m_vecLayers[i]->getCRSCode().c_str());
 			stLayerProperty.strLayerInfo = strLayerInfo;
 			stServerProperty.vecLayerProperty.push_back(stLayerProperty);
 		}
 	}
 
	CGxServiceModelUtilUi::getInstance()->newAddLayer(stServerProperty);

	close();
}

void CGxAddOGCWindow::on_pCancelBtn_clicked()
{
	close();
}
