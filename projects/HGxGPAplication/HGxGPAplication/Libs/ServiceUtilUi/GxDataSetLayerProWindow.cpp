#include "GxDataSetLayerProWindow.h"
#include <QScrollBar>
//#include <FreeXSysConfig.h>


CGxDataSetLayerProWindow::CGxDataSetLayerProWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	QMetaObject::connectSlotsByName(this);
	ui.pLayerTypeCBBox->addItems(QStringList() << tr("image layer") << tr("elevation layer"));

	// 限制不能输入空格和字数
	QRegExp rx(QString("\\S{0,").append(QString::number(20)).append("}"));
	ui.pDataSetNameEdit->setValidator(new QRegExpValidator(rx, ui.pDataSetNameEdit));
}

CGxDataSetLayerProWindow::~CGxDataSetLayerProWindow()
{

}

void CGxDataSetLayerProWindow::updateUi()
{
	ui.pLayerTypeCBBox->blockSignals(true);
	ui.pLayerTypeCBBox->setCurrentIndex(ui.pLayerTypeCBBox->findText(m_stLayerProperty.strLayerType));
	ui.pLayerTypeCBBox->blockSignals(false);
	ui.pDataSetNameEdit->setText(m_stLayerProperty.strLayerName);
	ui.pDataSetDriveEdit->setText(m_stLayerProperty.strDriveName);
	ui.pDataSetWebsiteEdit->setText(m_stLayerProperty.strServerWebsite);
}

void CGxDataSetLayerProWindow::setLayerProPerty(SLayerPropertyStruct stLayerProPerty)
{
	m_stLayerProperty = stLayerProPerty;
	updateUi();
}

void CGxDataSetLayerProWindow::setServerName(const QString &strName)
{
	ui.pDataSetNameEdit->blockSignals(true);
	ui.pDataSetNameEdit->setText(strName);
	ui.pDataSetNameEdit->blockSignals(false);
}

void CGxDataSetLayerProWindow::on_pLayerTypeCBBox_currentIndexChanged( const QString &strText )
{
	m_stLayerProperty.strLayerType = ui.pLayerTypeCBBox->currentText();
	emit reLayerSetByProEditSignal(m_stLayerProperty);
}

// void CGxDataSetLayerProWindow::on_pDataSetNameEdit_editingFinished()
// {
// 	QString strLayerName = ui.pDataSetNameEdit->text();
// 	m_stLayerProperty.strLayerName = strLayerName;
// 	emit reLayerSetByProEditSignal(m_stLayerProperty);
// }

void CGxDataSetLayerProWindow::on_pDataSetNameEdit_textChanged( const QString &strText )
{
	m_stLayerProperty.strLayerName = strText;
	emit reLayerSetByProEditSignal(m_stLayerProperty);
}
