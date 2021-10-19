#include "GxOGCLayerSetProWindow.h"
//#include <FreeXSysConfig.h>
#include <QScrollBar>


CGxOGCLayerSetProWindow::CGxOGCLayerSetProWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	(const_cast<QScrollBar *>(ui.scrollArea->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.scrollArea->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.pLayerList->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.pLayerList->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.pLayerInfoEdit->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.pLayerInfoEdit->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	QMetaObject::connectSlotsByName(this);
	connect(ui.pLayerList, SIGNAL(itemClicked(QListWidgetItem* )), this, SLOT(checkLayerInfoSlot(QListWidgetItem* )));
	
	// 限制不能输入空格和字数
	QRegExp rx(QString("\\S{0,").append(QString::number(20)).append("}"));
	ui.pServerNameEdit->setValidator(new QRegExpValidator(rx, ui.pServerNameEdit));
}

CGxOGCLayerSetProWindow::~CGxOGCLayerSetProWindow()
{
	
}

void CGxOGCLayerSetProWindow::updateUi()
{
	ui.pServerNameEdit->setText(m_stServerProperty.strServerName);
	ui.pServerTypeEdit->setText(m_stServerProperty.strServerType);
	ui.pServerVersionEdit->setText(m_stServerProperty.strServerVersion);
	ui.pServerWebsiteEdit->setText(m_stServerProperty.strServerWebsite);

	ui.pLayerList->clear();

	for (int i = 0; i < m_stServerProperty.vecLayerProperty.size();i++)
	{
		QListWidgetItem *pListItem = new QListWidgetItem;
		pListItem->setText(m_stServerProperty.vecLayerProperty[i].strLayerName);
		ui.pLayerList->addItem(pListItem);
	}

	//w00224 2020-6-9 查看属性的时候默认选中第一个并展示
	if(ui.pLayerList->item(0))
	{
		ui.pLayerList->item(0)->setSelected(true);
		checkLayerInfoSlot(ui.pLayerList->item(0));
	}
}

void CGxOGCLayerSetProWindow::setServerProPerty( SServerPropertyStruct stServerProPerty )
{
	m_stServerProperty = stServerProPerty;
	ui.pLayerInfoEdit->clear();
	updateUi();
}



void CGxOGCLayerSetProWindow::setServerName(const QString &strName )
{
	ui.pServerNameEdit->blockSignals(true);
	ui.pServerNameEdit->setText(strName);
	ui.pServerNameEdit->blockSignals(false);
}

void CGxOGCLayerSetProWindow::checkLayerInfoSlot(QListWidgetItem *item)
{
	for (int i = 0;i < m_stServerProperty.vecLayerProperty.size(); i++)
	{
		if (m_stServerProperty.vecLayerProperty[i].strLayerName == item->text())
		{
			ui.pLayerInfoEdit->setText(m_stServerProperty.vecLayerProperty[i].strLayerInfo);
		}
	}
}

// void CGxOGCLayerSetProWindow::on_pServerNameEdit_editingFinished()
// {
// 	QString strServerName = ui.pServerNameEdit->text();
// 	m_stServerProperty.strServerName = strServerName;
// 	emit reSetByProEditSignal(m_stServerProperty);
// }

void CGxOGCLayerSetProWindow::on_pServerNameEdit_textChanged(const QString &strText)
{
	m_stServerProperty.strServerName = strText;
	emit reSetByProEditSignal(m_stServerProperty);
}

