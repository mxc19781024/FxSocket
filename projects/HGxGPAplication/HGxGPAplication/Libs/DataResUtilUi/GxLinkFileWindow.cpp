#include <QApplication>
#include <QDesktopWidget>
#include <QFile>

#include "public/GxLinkFileWindow.h"
#include "ui_GxLinkFileWindow.h"
#include <QScrollBar>
#include "GxServiceModelUtilUi.h"




CGxLinkFileWindow::CGxLinkFileWindow(QWidget *parent)
	: CFxBaseWindow(parent)
	, ui(new Ui::CGxLinkFileWindow)
{
	ui->setupUi(m_pCentralWgt);
	QMetaObject::connectSlotsByName(this);
	setWindowTitle(tr("link file or folder"));
	ui->pResetTBtn->setToolTip(tr("reset"));
	setAttribute(Qt::WA_ShowModal);
	(const_cast<QScrollBar *>(ui->fileSelectedView->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui->fileSelectedView->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	//读样式表文件
	QFile file(":/Resources/qss/DataResWnd.css");
	if (file.open(QIODevice::ReadOnly| QIODevice::Text))
	{
		QString str = file.readAll();
		setStyleSheet(styleSheet().append(str));
		file.close();
	}

	init();

	resize(600, 400);
}

CGxLinkFileWindow::~CGxLinkFileWindow()
{

}

void CGxLinkFileWindow::show()
{
	ui->pFileSeletedCBBox->setCurrentIndex(-1);
	ui->fileSelectedView->collapseAll();
	ui->fileSelectedView->clearSelection();
	ui->fileNameEdt->clear();
	ui->fileFilterCbx->setCurrentIndex(0);
	m_strLinkInfo = "";

    CFxBaseWindow::show();
	QPoint point = QApplication::desktop()->rect().center();
	move(point.x() - width() / 2, point.y() - height() / 2);
}

void CGxLinkFileWindow::keyPressEvent( QKeyEvent *event )
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

void CGxLinkFileWindow::init()
{
	initSignalsAndSlots();
	m_systemModel.setRootPath("");
	m_systemModel.setNameFilterDisables(false);
	ui->fileSelectedView->setModel(&m_systemModel);
	ui->fileSelectedView->setAnimated(false);
	ui->fileSelectedView->setIndentation(20);
	//ui->fileSelectedView->setSortingEnabled(true);
	ui->pOkBtn->setStyleSheet("color: #ffffff");
	//setFocusPolicy(Qt::StrongFocus);

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
	ui->fileFilterCbx->addItem(tr("All Type(*.*)"), "*.*");
	ui->fileFilterCbx->addItem("Shapefile Files(*.shp)", "*.shp");
	ui->fileFilterCbx->addItem("KML Files(*.kml)", "*.kml");
	ui->fileFilterCbx->addItem("GeoTIFF Files(*.tif)", "*.tif");
	ui->fileFilterCbx->addItem("Idrisl Raster Files(*.img)", "*.img");
	/// yq add 海图000文件格式
	ui->fileFilterCbx->addItem("S57 Files(*.000)", "*.000");
}

void CGxLinkFileWindow::initSignalsAndSlots()
{
	connect(ui->fileSelectedView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(viewClickedSlot(const QModelIndex &)));
	connect(ui->fileFilterCbx, SIGNAL(currentIndexChanged (int)), this, SLOT(fileFilterSlot(int)));
	connect(ui->pOkBtn, SIGNAL(clicked()), this, SLOT(confirmSlot()));
	connect(ui->pCancelBtn, SIGNAL(clicked()), this, SLOT(cancelSlot()));
}

void CGxLinkFileWindow::confirmSlot()
{
	// 链接信息为空，链接文件窗口隐藏
	if (m_strLinkInfo.isEmpty())
	{
		close();
		return;
	}

	SLinkInfo stLinkInfo;
	stLinkInfo.strAbsoluteFilePath = m_strLinkInfo;
	stLinkInfo.strFileName = ui->fileNameEdt->text();
	bool bDir = m_systemModel.isDir(m_systemModel.index(m_strLinkInfo));

	if (bDir)
	{
		stLinkInfo.eLinkType = FOLDERLINK;
	}
	else
	{
		stLinkInfo.eLinkType = FILELINK;
	}

	//FreeXServiceModelUtilUi::getInstance()->folderLink(stLinkInfo);
	CGxServiceModelUtilUi::getInstance()->folderLink(stLinkInfo);
	close();
}

void CGxLinkFileWindow::cancelSlot()
{
	close();
}

void CGxLinkFileWindow::viewClickedSlot(const QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}

	// 记录链接的文件或者文件夹信息
	m_strLinkInfo = m_systemModel.filePath(index);

	ui->fileNameEdt->setText(m_systemModel.fileName(index));
}

void CGxLinkFileWindow::fileFilterSlot(int index)
{
	QString strFilter = ui->fileFilterCbx->itemData(index).toString();
	m_filters.clear();
	m_filters << strFilter;
	m_systemModel.setNameFilters(m_filters);
}

void CGxLinkFileWindow::on_pFileSeletedCBBox_currentIndexChanged( int index )
{
	QString strText = ui->pFileSeletedCBBox->itemText(index);

	ui->fileSelectedView->setModel(&m_systemModel);
	ui->fileSelectedView->setRootIndex(m_systemModel.index(strText));
}

void CGxLinkFileWindow::on_pResetTBtn_clicked()
{
	ui->fileSelectedView->setRootIndex(m_systemModel.index("/"));
	ui->pFileSeletedCBBox->setCurrentIndex(-1);
}

