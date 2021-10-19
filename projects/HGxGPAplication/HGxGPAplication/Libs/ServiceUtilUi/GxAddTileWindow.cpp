#include <QFileDialog>
#include <QFile>

#include "GxAddTileWindow.h"
//#include "FreeXDataResUtilUi/CDataResourcesUtils.h"
//#include "FreeXServiceModelUtilUi/FreeXServiceModelUtilUi.h"

//#include "public/GxDataResourcesUtils.h"
#include "GxServiceModelUtilUi.h"

CGxAddTileWindow::CGxAddTileWindow(const QString& strServerType, QWidget *parent)
	: CFxBaseWindow(parent)
	, m_pService(NULL)
	, m_pDataService(new HGxUtil::CGxDataService)
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
	initUi();

	setWindowModality(Qt::WindowModal);
	setWindowTitle(tr("add tile service"));
	QMetaObject::connectSlotsByName(this);
	setCurrentServer(strServerType);


	//m_vecLayers.clear();
}

CGxAddTileWindow::~CGxAddTileWindow()
{

}

void CGxAddTileWindow::initUi()
{
	// 添加Tile服务：TMS，XYZ
	ui.pServerTypeCombo->addItems(QStringList()<<"TMS"<<"XYZ");
	ui.pServerNameEdit->setText(tr("unnamed"));
	ui.pServerNameEdit->setMaxLength(20);
	ui.pOkBtn->setStyleSheet("color: #ffffff");
}

void CGxAddTileWindow::setCurrentServer( const QString& strServerType )
{
	// 设置服务器类型
	ui.pServerTypeCombo->setCurrentIndex(ui.pServerTypeCombo->findText(strServerType));
}

void CGxAddTileWindow::showEvent( QShowEvent *event )
{
	// 窗口显示的时候初始化窗口
	ui.pServerWebsiteEdit->clear();
	ui.pServerNameEdit->setText(tr("unnamed"));
	ui.pServerNameEdit->setErrorMsg("normal");
	ui.pServerWebsiteEdit->setErrorMsg("normal");
}

void CGxAddTileWindow::keyPressEvent( QKeyEvent *event )
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

void CGxAddTileWindow::on_pOkBtn_clicked()
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
   bool bConnect = false;
   bool bLocal = false;


	QString strType = ui.pServerTypeCombo->currentText();
	if (strType == "TMS")
	{
		m_pService = m_pDataService->cretaeTMSService(ui.pServerWebsiteEdit->text().toUtf8().data());

		QFileInfo info(ui.pServerWebsiteEdit->text());

		if (info.exists())
		{
			bConnect =true;
			bLocal = true;
		}
	}
	else if (strType == "XYZ")
	{
		m_pService = m_pDataService->cretaeXYZService(ui.pServerWebsiteEdit->text().toUtf8().data());
	}

	QString ss = ui.pServerWebsiteEdit->text();
	if (m_pService)
	{
		//QCoreApplication::processEvents();

		if (!bLocal)
		{
			bConnect = m_pService->isConnected();
		}

		SServerPropertyStruct stServerProperty;
		stServerProperty.bConnect = bConnect;
		stServerProperty.strServerType = m_pService->getServiceType().c_str();
		stServerProperty.strServerName = ui.pServerNameEdit->text();
		stServerProperty.strServerWebsite = m_pService->getServiceURL().c_str();

		SLayerPropertyStruct stLayerProperty;
		stLayerProperty.strLayerType = ui.pLayerTypeCBBox->currentText();
		stLayerProperty.strLayerName = ui.pServerNameEdit->text();;
		stLayerProperty.strServerName = ui.pServerNameEdit->text();
		stLayerProperty.strServerType = ui.pServerTypeCombo->currentText();
		stLayerProperty.strServerWebsite = m_pService->getServiceURL().c_str();
		stLayerProperty.strDriveName = m_pService->getServiceType().c_str();
		stServerProperty.vecLayerProperty.push_back(stLayerProperty);

		// 点击完确定按钮后，CServerTree指针调用接口完成服务节点的添加
		CGxServiceModelUtilUi::getInstance()->newAddLayer(stServerProperty);
	}

   close();
}

void CGxAddTileWindow::on_pCancelBtn_clicked()
{
	close();
}

void CGxAddTileWindow::on_pXYZLocalTBtn_clicked()
{
	QString strTempPath;
    #if WIN32
        strTempPath = QFileDialog::getOpenFileName(this, tr("Open XML"), "", tr("XML Files (*.xml)"));
    #else
        strTempPath = QFileDialog::getOpenFileName(0, tr("Open XML"), "", tr("XML Files (*.xml)"));
    #endif

	if (!strTempPath.isEmpty())
	{
		ui.pServerWebsiteEdit->setText(strTempPath);
	}

}

void CGxAddTileWindow::on_pServerTypeCombo_currentIndexChanged( const QString &text )
{
	if (text == "TMS")
	{
		ui.pXYZLocalTBtn->show();
	}
	else if (text == "XYZ")
	{
		ui.pXYZLocalTBtn->hide();
	}
}

void CGxAddTileWindow::folderLinkSlot(const QString & strPath, int linkType)
{
}
