#include "GxOGCLayerProWindow.h"
#include <QScrollBar>
//#include <FreeXSysConfig.h>


CGxOGCLayerProWindow::CGxOGCLayerProWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QMetaObject::connectSlotsByName(this);
	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.pLayerInfoEdit->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.pLayerInfoEdit->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	// 限制不能输入空格和字数
	QRegExp rx(QString("\\S{0,").append(QString::number(20)).append("}"));
	ui.pServerNameEdit->setValidator(new QRegExpValidator(rx, ui.pServerNameEdit));
	connect(ui.pDataFormatBBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(formatComboBoxChangedSlot(QString)));
}

CGxOGCLayerProWindow::~CGxOGCLayerProWindow()
{

}

void CGxOGCLayerProWindow::updateUi()
{
	ui.pServerNameEdit->setText(m_stLayerProperty.strLayerName);
	ui.pServerTypeEdit->setText(m_stLayerProperty.strServerType);
	ui.pServerWebsiteEdit->setText(m_stLayerProperty.strServerWebsite);
	ui.pLayerInfoEdit->setText(m_stLayerProperty.strLayerInfo);
	
	if (m_stLayerProperty.strServerType == "WFS")
	{
		ui.pDataFormatBBox->setVisible(false);
		ui.label_dataFormat->setVisible(false);
		return;
	}
	else
	{
		ui.label_dataFormat->setVisible(true);
		ui.pDataFormatBBox->setVisible(true);
	}

	//如果当前图层的格式是空，说明没有设置过，就先初始化当前支持的所有格式
	ui.pDataFormatBBox->clear();
	if (m_stLayerProperty.strLayerFormat.isEmpty())
	{
		for (int i = 0; i < m_stLayerProperty.strLayerFormats.size(); i++)
		{
			ui.pDataFormatBBox->addItem(m_stLayerProperty.strLayerFormats.at(i));
		}
	}
	else
	{
		ui.pDataFormatBBox->addItem(m_stLayerProperty.strLayerFormat);
		for (int i = 0; i < m_stLayerProperty.strLayerFormats.size(); i++)
		{
			if (m_stLayerProperty.strLayerFormats.at(i) == m_stLayerProperty.strLayerFormat)
			{
				continue;
			}
			ui.pDataFormatBBox->addItem(m_stLayerProperty.strLayerFormats.at(i));
		}
	}

}

void CGxOGCLayerProWindow::setLayerProPerty( SLayerPropertyStruct stLayerProPerty)
{
	m_stLayerProperty = stLayerProPerty;
	updateUi();
}

void CGxOGCLayerProWindow::formatComboBoxChangedSlot(QString strFormat)
{
	if (strFormat.isEmpty())
	{
		return;
	}

	m_stLayerProperty.strLayerFormat = strFormat;

	emit reLayerSetByProEditSignal(m_stLayerProperty);
}

// void CGxOGCLayerProWindow::on_pServerNameEdit_editingFinished()
// {
// 	QString strServerName = ui.pServerNameEdit->text();
// 	m_stLayerProperty.strLayerName = strServerName;
// 	emit reLayerSetByProEditSignal(m_stLayerProperty);
// }

// void CGxOGCLayerProWindow::on_pServerWebsiteEdit_editingFinished()
// {
// 	QString strServerName = ui.pServerWebsiteEdit->text();
// 	m_stLayerProperty.strServerWebsite = strServerName;
// 	emit reLayerSetByProEditSignal(m_stLayerProperty);
// }

