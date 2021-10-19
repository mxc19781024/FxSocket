#include <QFile>
#include <QFileInfo>
#include "GxAddDataSetWindow.h"

//#include <FreeXServiceModelUtilUi/FreeXServiceModelUtilUi.h>
#include "GxServiceModelUtilUi.h"

CGxAddDataSetWindow::CGxAddDataSetWindow(QWidget *parent)
	: CFxBaseWindow(parent)
	, m_pService(NULL)
	, m_pDataService(new HGxUtil::CGxDataService)
{
	ui.setupUi(m_pCentralWgt);
	//读样式表文件
	QFile file(":/css/Resources/qss/ServerProUtils.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = file.readAll();
		m_pCentralWgt->setStyleSheet(styleSheet().append(str));
		file.close();
	}
	

	QMetaObject::connectSlotsByName(this);
	initUi();
	setWindowModality(Qt::WindowModal);
	setWindowTitle(tr("add dataset"));
}

CGxAddDataSetWindow::~CGxAddDataSetWindow()
{

}

void CGxAddDataSetWindow::showEvent( QShowEvent *event )
{
	// 窗口显示的时候初始化窗口
	ui.pDataSetWebsiteEdit->clear();
	ui.pDataSetNameEdit->setText(tr("unnamed"));
	ui.pDataSetNameEdit->setErrorMsg("normal");
	ui.pDataSetWebsiteEdit->setErrorMsg("normal");
}

void CGxAddDataSetWindow::keyPressEvent( QKeyEvent *event )
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

void CGxAddDataSetWindow::initUi()
{
	// 添加数据集驱动：flatgis，tdb，mbtiles
	ui.pDataSetDriveCBBox->addItems(QStringList()<<"flatgis"<<"tdb"<<"mbtiles");
	ui.pDataSetNameEdit->setText(tr("unnamed"));
	ui.pDataSetNameEdit->setMaxLength(20);
	ui.pOkBtn->setStyleSheet("color: #ffffff");
}

void CGxAddDataSetWindow::on_pOkBtn_clicked()
{
	if (ui.pDataSetNameEdit->text() == "")
	{
		ui.pDataSetNameEdit->setErrorMsg("red");
		return;
	}

	if (ui.pDataSetWebsiteEdit->text() == "")
	{
		ui.pDataSetWebsiteEdit->setErrorMsg("red");
		return;
	}

	bool bConnect = false;
	bool bLocal = false;


	m_pService = m_pDataService->cretaeDataService(ui.pDataSetWebsiteEdit->text().toStdString(), ui.pDataSetDriveCBBox->currentText().toStdString());

	QFileInfo info(ui.pDataSetWebsiteEdit->text());

	if (info.exists())
	{
		bConnect =true;
		bLocal = true;
	}

	if (m_pService)
	{

		if (!bLocal)
		{
			bConnect = m_pService->isConnected();
		}

		SServerPropertyStruct stServerProperty;
		stServerProperty.bConnect = bConnect;
		stServerProperty.strServerType = tr("dataSet");
		stServerProperty.strServerName = ui.pDataSetNameEdit->text();
		stServerProperty.strServerWebsite = m_pService->getServiceURL().c_str();


		SLayerPropertyStruct  stLayerProperty;
		stLayerProperty.strServerType = tr("dataSet");
		stLayerProperty.strServerName = stServerProperty.strServerName;
			stLayerProperty.strServerWebsite = stServerProperty.strServerWebsite;
		stLayerProperty.strDriveName = m_pService->getServiceType().c_str();
		stLayerProperty.strLayerType = ui.pLayerTypeCBBox->currentText();
		stLayerProperty.strLayerName = ui.pDataSetNameEdit->text();
		stServerProperty.vecLayerProperty.push_back(stLayerProperty);

		CGxServiceModelUtilUi::getInstance()->newAddLayer(stServerProperty);
	}

	close();
}

void CGxAddDataSetWindow::on_pCancelBtn_clicked()
{
	close();
}

