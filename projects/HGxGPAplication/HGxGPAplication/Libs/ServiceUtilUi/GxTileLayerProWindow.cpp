#include "GxTileLayerProWindow.h"
//#include <FreeXSysConfig.h>
#include <QScrollBar>


CGxTileLayerProWindow::CGxTileLayerProWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
#if WIN32
    setWindowFlags(Qt::Window );
#else if __linux__
    setWindowFlags(Qt::Dialog );
#endif
	setWindowTitle(tr("Tile layer property"));
	ui.pLayerTypeCBBox->addItems(QStringList() << tr("image layer") << tr("elevation layer"));
}

CGxTileLayerProWindow::~CGxTileLayerProWindow()
{

}

void CGxTileLayerProWindow::updateUi()
{
	ui.pLayerTypeCBBox->blockSignals(true);
    ui.pLayerTypeCBBox->setCurrentIndex(ui.pLayerTypeCBBox->findText(m_stLayerProperty.strLayerType));
    ui.pLayerTypeCBBox->blockSignals(false);
	ui.pServerNameEdit->setText(m_stLayerProperty.strLayerName);
	ui.pServerTypeEdit->setText(m_stLayerProperty.strServerType);
	ui.pServerWebsiteEdit->setText(m_stLayerProperty.strServerWebsite);
}

void CGxTileLayerProWindow::setLayerProPerty( SLayerPropertyStruct stLayerProPerty )
{
	m_stLayerProperty = stLayerProPerty;
	updateUi();
}

void CGxTileLayerProWindow::on_pLayerTypeCBBox_currentIndexChanged( const QString &strText )
{
	m_stLayerProperty.strLayerType = ui.pLayerTypeCBBox->currentText();
	emit reLayerSetByProEditSignal(m_stLayerProperty);
}

// void CGxTileLayerProWindow::on_pServerNameEdit_editingFinished()
// {
// 	QString strServerName = ui.pServerNameEdit->text();
// 	m_stLayerProperty.strLayerName = strServerName;
// 	emit reLayerSetByProEditSignal(m_stLayerProperty);
// }

void CGxTileLayerProWindow::on_pServerNameEdit_textChanged(const QString &strText)
{
	m_stLayerProperty.strLayerName = strText;
	emit reLayerSetByProEditSignal(m_stLayerProperty);
}

void CGxTileLayerProWindow::setServerName(const QString &strName)
{
	ui.pServerNameEdit->blockSignals(true);
	ui.pServerNameEdit->setText(strName);
	m_stLayerProperty.strLayerName = strName;
	ui.pServerNameEdit->blockSignals(false);
}
