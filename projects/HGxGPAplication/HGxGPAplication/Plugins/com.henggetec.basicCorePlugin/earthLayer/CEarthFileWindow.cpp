#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QDir>
#include "CEarthFileWindow.h"
#include "ui_CEarthFileWindow.h"
#include <qscrollbar.h>
//#include <FreeXServiceModelUtilUi/FreeXServiceModelUtilUi.h>

#include "public/GxServiceModelUtilUi.h"

/************暂时不提供海图*****************/
//#include "CSeaLayerDef.h"

//// 文件夹只能选择一个 文件可以选择多个 
CEarthFileSelectWindow::CEarthFileSelectWindow(QWidget *parent)
	: CFxBaseWindow(parent)
	, ui(new Ui::CEarthFileWindow)
	, m_systemModel(NULL)
{
	//读样式表文件
	QFile file(":/qss/Resources/qss/DataResWnd.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = file.readAll();
		m_pCentralWgt->setStyleSheet(styleSheet().append(str));
		file.close();
	}
	ui->setupUi(m_pCentralWgt);
	QMetaObject::connectSlotsByName(this);
	setWindowTitle(tr("add chart layer"));
	ui->pResetTBtn->setToolTip(tr("reset"));
	setAttribute(Qt::WA_ShowModal);
	(const_cast<QScrollBar *>(ui->fileSelectedView->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui->fileSelectedView->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	
	ui->fileSelectedView->setSelectionMode(QAbstractItemView::ContiguousSelection);
 	
	

	init();
	resize(600, 400);
}

CEarthFileSelectWindow::~CEarthFileSelectWindow()
{

}

void CEarthFileSelectWindow::show()
{
	///ui->pFileSeletedCBBox->setCurrentIndex(-1);
	///ui->fileSelectedView->collapseAll();
	ui->fileSelectedView->clearSelection();
	ui->fileNameEdt->clear();
	ui->fileFilterCbx->setCurrentIndex(0);

    CFxBaseWindow::show();
	QPoint point = QApplication::desktop()->rect().center();
	move(point.x() - width() / 2, point.y() - height() / 2);
}

void CEarthFileSelectWindow::keyPressEvent( QKeyEvent *event )
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Space)
	{
		confirmSlot();
	}
	else if(event->key() == Qt::Key_Escape)
	{
		close();		
	}

	CFxBaseWindow::keyPressEvent(event);
}

void CEarthFileSelectWindow::init()
{
	m_systemModel = new QFileSystemModel;
	initSignalsAndSlots();
	m_systemModel->setRootPath("");
	m_systemModel->setNameFilterDisables(false);
	ui->fileSelectedView->setModel(m_systemModel);
	ui->fileSelectedView->setAnimated(false);
	ui->fileSelectedView->setIndentation(20);
	ui->pOkBtn->setStyleSheet("color: #ffffff");
	
	QFileInfoList fileList = QDir::drives();

	for (int i = 0; i < fileList.size(); i++)
	{
		QString strName = fileList[i].absolutePath();
     	ui->pFileSeletedCBBox->addItem(strName);
	}

	ui->pFileSeletedCBBox->setCurrentIndex(-1);


	// 隐藏第三列LastModified以及第二列大小size
	ui->fileSelectedView->header()->hide();
	ui->fileSelectedView->hideColumn(1);
	ui->fileSelectedView->hideColumn(2);
	ui->fileSelectedView->hideColumn(3);

	// 为文件系统添加筛选过滤项
	/// yq add 海图000文件格式
	ui->fileFilterCbx->addItem("S57 Files(*.000)", "*.000");
}

void CEarthFileSelectWindow::initSignalsAndSlots()
{
	connect(ui->fileSelectedView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(viewClickedSlot(const QModelIndex &)));
	connect(ui->fileFilterCbx, SIGNAL(currentIndexChanged (int)), this, SLOT(fileFilterSlot(int)));
	connect(ui->pOkBtn, SIGNAL(clicked()), this, SLOT(confirmSlot()));
	connect(ui->pCancelBtn, SIGNAL(clicked()), this, SLOT(cancelSlot()));
}

void CEarthFileSelectWindow::confirmSlot()
{
	close();
	emit confirmSignals();
}

void CEarthFileSelectWindow::cancelSlot()
{
	close();
}

void CEarthFileSelectWindow::viewClickedSlot(const QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	QItemSelectionModel *pSelectModel = ui->fileSelectedView->selectionModel();
	if (pSelectModel == NULL)
	{
		return;
	}

	if (m_systemModel->hasChildren(index))
	{
		pSelectModel->clearSelection();
		pSelectModel->select(index,QItemSelectionModel::ClearAndSelect);
		ui->fileSelectedView->setSelectionMode(QAbstractItemView::SingleSelection);
	}
	else
	{
		ui->fileSelectedView->setSelectionMode(QAbstractItemView::ContiguousSelection);
	}

	QString strText;
	QModelIndexList pmodelList =  pSelectModel->selectedIndexes();

	foreach (QModelIndex index1, pmodelList)
	{
		if (index1.column() == 0)
		{
			strText += " " + m_systemModel->fileName(index1);
		}
	}

	ui->fileNameEdt->setText(strText);
}

void CEarthFileSelectWindow::fileFilterSlot(int index)
{
	QString strFilter = ui->fileFilterCbx->itemData(index).toString();
	m_filters.clear();
	m_filters << strFilter;
	m_systemModel->setNameFilters(m_filters);
}

void CEarthFileSelectWindow::on_pFileSeletedCBBox_currentIndexChanged( int index )
{
	QString strText = ui->pFileSeletedCBBox->itemText(index);
	ui->fileSelectedView->setRootIndex(m_systemModel->index(strText));
}

void CEarthFileSelectWindow::on_pResetTBtn_clicked()
{
	ui->fileSelectedView->setRootIndex(m_systemModel->index("/"));
	ui->pFileSeletedCBBox->setCurrentIndex(-1);
}

void CEarthFileSelectWindow::getSelectFilePaths( QStringList &strListPath )
{
	QItemSelectionModel *pSelectModel = ui->fileSelectedView->selectionModel();

	if (pSelectModel == NULL)
	{
		return;
	}

	QModelIndexList pmodelList =  pSelectModel->selectedIndexes();
	foreach (QModelIndex index, pmodelList)
	{
		if (index.column() == 0)
		{
			getPathByDir(m_systemModel->filePath(index),strListPath);
		}
	}
}


void CEarthFileSelectWindow::getPathByDir( const QString &strDir,  QList<QString> &strPathSet)
{
	QFileInfo filInfo(strDir);

	if (!filInfo.exists())
	{
		return;
	}

	if (!filInfo.isDir())
	{
/************暂时不提供海图*****************/

// 		/// 如果是文件
// 		if (filInfo.isFile() && filInfo.suffix() == S57LayerSuffix)
// 		{
// 			strPathSet << filInfo.absoluteFilePath();
// 		}
// 		return;
	}

	QDir dir(strDir);
	dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	QFileInfoList fileList = dir.entryInfoList();

	QString strPath;
	QString strName;
	foreach(QFileInfo fileInfo,fileList)
	{
		getPathByDir(fileInfo.absoluteFilePath(),strPathSet);
	}
}