#include <QStandardItem>
#include <QMenu>
#include <QAction>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>

#include "public/GxServerDataWindow.h"
#include "GxServerProUtils.h"
#include <public/GxLinkFileWindow.h>
#include <public/GxDataResourcesUtils.h>
#include <GxServiceProWgt.h>
#include <GxServiceModelUtilUi.h>
#include <FxMessageBox/CMessageBox.h>

// kzm added
#include "scene_ui/ISceneUiService.h"
#include "map_layer_ui/IMapLayerUiService.h"
#include "earth_layer_ui/IEarthLayerUiService.h"

#include "FxEditingDelegate/CEditingDelegate.h"
#include "public/GxDataResourcesUtils.h"

#include <QScrollBar>
#include "ui_GxServerDataWindow.h"

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>
#include "GxUiFrameworkServiceInterface.h"

CGxServerDataWindow::CGxServerDataWindow(QWidget *parent)
	: CFxBaseWindow(parent)
	, ui(new Ui::CGxServerDataWindow)
	, m_pItemModel(NULL)
	, m_bLinkFiles(false)
	, m_eCurLayerType(CLayerConfig::UndefinedLayer)
{
	ui->setupUi(m_pCentralWgt);
	QFile file(":/Resources/qss/DataResWnd.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = file.readAll();
		m_pCentralWgt->setStyleSheet(styleSheet().append(str));
		//setStyleSheet(styleSheet().append(str));
		file.close();
	}


	QMetaObject::connectSlotsByName(this);
	setWindowTitle(tr("add data"));
	setAttribute(Qt::WA_ShowModal);
	ui->pResetTBtn->setToolTip(tr("reset"));
	(const_cast<QScrollBar *>(ui->fileSelectedView->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui->fileSelectedView->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	//读样式表文件


	//x00038 重设界面大小
	resize(600, 400);
	//end
	init();
}

CGxServerDataWindow::~CGxServerDataWindow()
{

}

void CGxServerDataWindow::show()
{	
	ui->pFileSeletedCBBox->setCurrentIndex(-1);
	ui->fileSelectedView->collapseAll();
	ui->fileSelectedView->clearSelection();
	ui->fileNameEdt->clear();	
	on_pResetTBtn_clicked();
	filterAllFiles();
    CFxBaseWindow::show();
	QPoint point = QApplication::desktop()->rect().center();
	move(point.x() - width() / 2, point.y() - height() / 2);
}

void CGxServerDataWindow::keyPressEvent( QKeyEvent *event )
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

void CGxServerDataWindow::init()
{
	// 添加服务树结构
	ui->pFileSeletedCBBox->addItem(tr("local service"));
	ui->pFileSeletedCBBox->addItem(tr("OGC service"));
	ui->pFileSeletedCBBox->addItem(tr("Tile service"));
	ui->pFileSeletedCBBox->addItem(tr("dataset"));

	ui->fileSelectedView->header()->hide();
	ui->fileSelectedView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->fileSelectedView->setContextMenuPolicy(Qt::CustomContextMenu);

	ui->pOkBtn->setStyleSheet("color:#ffffff");

	//m_pItemModel = FreeXServiceModelUtilUi::getInstance()->getServiceModel();
	m_pItemModel = CGxServiceModelUtilUi::getInstance()->getServiceModel();

	if (m_pItemModel)
	{
		ui->fileSelectedView->setModel(m_pItemModel);
	}

	initSignalsAndSlots();
}

void CGxServerDataWindow::initSignalsAndSlots()
{
	connect(ui->fileSelectedView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(viewClickedSlot(const QModelIndex &)));

	connect(ui->pFileSeletedCBBox, SIGNAL(currentIndexChanged(int)), this, SLOT(switchFileViewSlot(int)));

	connect(ui->fileSelectedView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(contextMenuSlot(const QPoint &)));

	connect(ui->pOkBtn, SIGNAL(clicked()), this, SLOT(confirmSlot()));

	connect(ui->pCancelBtn, SIGNAL(clicked()), this, SLOT(cancelSlot()));

	//connect(FreeXServiceModelUtilUi::getInstance(), SIGNAL(expandItemsSignal(const QModelIndex &)), this, SLOT(expandItemsSlot(const QModelIndex &)));
	connect(CGxServiceModelUtilUi::getInstance(), SIGNAL(expandItemsSignal(const QModelIndex &)), this, SLOT(expandItemsSlot(const QModelIndex &)));
}

QStandardItem* CGxServerDataWindow::getCurrentItem()
{
	QModelIndex currentIndex = ui->fileSelectedView->currentIndex();
	if (!currentIndex.isValid())
	{
		return NULL;
	}
	QStandardItem *pCurrentItem = m_pItemModel->itemFromIndex(currentIndex);

	if (!pCurrentItem)
	{
		return NULL;
	}
	return pCurrentItem;
}

void CGxServerDataWindow::switchFileViewSlot(int index)
{
	//无选中状态不作处理 l180
	if(index < 0)
	{
		return;
	}
	// 获取当前选中的文本
	QString strText = ui->pFileSeletedCBBox->itemText(index);

	int nRootCount = m_pItemModel->rowCount();
	for (int nNum = 0; nNum < nRootCount; ++nNum)
	{
		QModelIndex index = m_pItemModel->index(nNum, 0);

		QString strRootItemText = index.data(Qt::DisplayRole).toString();

		if (strRootItemText == strText)
		{
			ui->fileSelectedView->setRowHidden(index.row(), index.parent(), false);
		}
		else
		{
			ui->fileSelectedView->setRowHidden(index.row(), index.parent(), true);
		}
	}
}

void CGxServerDataWindow::viewClickedSlot(const QModelIndex &index)
{
	QString strItemText = index.data(Qt::DisplayRole).toString();
	ui->fileNameEdt->setText(strItemText);
}

void CGxServerDataWindow::contextMenuSlot(const QPoint &pos)
{
	QModelIndex index = ui->fileSelectedView->indexAt(pos);

	if (!index.isValid())
	{
		return;
	}

	QMenu menu(ui->fileSelectedView);
	menu.setWindowFlags(menu.windowFlags() | Qt::FramelessWindowHint);
	menu.setAttribute(Qt::WA_TranslucentBackground);

	// 需要的action
	QAction linkToAct(&menu);			   // 链接文件
	QAction addServerAct(&menu);           // 添加服务

	QStandardItem *pCurrentItem = m_pItemModel->itemFromIndex(index);

	if (!pCurrentItem)
	{
		return;
	}

	if (!pCurrentItem->data(ItemTypeRole).isValid())
	{
		return;
	}

	//恒道云服务 只能刷新不能删除修改
	bool bHGLyaer = false;
	QVariant HGData = pCurrentItem->data(HGLayerRole); 
	if (HGData.isValid())
	{
		bHGLyaer = true;
	}

	// 如果item的类型是服务器类型
	if (pCurrentItem->data(ItemTypeRole).toInt() == SERVER_TYPE_ITEM)
	{
		switch(pCurrentItem->data(ItemServerTypeRole).toInt())
		{
		case DATASET_SERVER:
			{
				if (!bHGLyaer)
				{
					addServerAct.setText(tr("add dataset"));
					addServerAct.setIcon(QIcon(":/Resources/images/tianjia_normal.png"));
					connect(&addServerAct, SIGNAL(triggered()), this, SLOT(addServerSlot()));
					menu.addAction(&addServerAct);
					menu.exec(ui->fileSelectedView->viewport()->mapToGlobal(pos));
				}
			}
			break;
		case LOCAL_SERVER:
			{
				if (!bHGLyaer)
				{
					// add by wkl 20181217
					linkToAct.setText(tr("link file"));
					connect(&linkToAct, SIGNAL(triggered()), this, SLOT(linkToSlot()));
					menu.addAction(&linkToAct);
					menu.exec(ui->fileSelectedView->viewport()->mapToGlobal(pos));
				}
			}
			break;
		default:
			break;
		}
	}

	// 如果item的类型是服务器
	if (pCurrentItem->data(ItemTypeRole).toInt() == SERVER_ITEM)
	{
		switch(pCurrentItem->data(ItemServerTypeRole).toInt()) 
		{
		case OGC_SERVER:
			if (!bHGLyaer)
			{
				addServerAct.setText(tr("add ogc service"));
				addServerAct.setIcon(QIcon(":/Resources/images/tianjia_normal.png"));
				connect(&addServerAct, SIGNAL(triggered()), this, SLOT(addServerSlot()));
				menu.addAction(&addServerAct);
				menu.exec(ui->fileSelectedView->viewport()->mapToGlobal(pos));
			}
			break;
		case TILE_SERVER:
			if (!bHGLyaer)
			{
				addServerAct.setText(tr("add tile service"));
				addServerAct.setIcon(QIcon(":/Resources/images/tianjia_normal.png"));
				menu.addAction(&addServerAct);
				connect(&addServerAct, SIGNAL(triggered()), this, SLOT(addServerSlot()));
				menu.exec(ui->fileSelectedView->viewport()->mapToGlobal(pos));
			}
			break;
		case DATASET_SERVER:
			break;
		default:
			break;
		}	
	}
}

void CGxServerDataWindow::addServerSlot()
{
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}

	QPoint point = QPoint(x() + width(), y());

	// 添加服务的有两类 OGC或者Tile和一种添加数据集驱动
	switch(pCurrentItem->data(ItemServerTypeRole).toInt())
	{
	case OGC_SERVER:
		{
			CGxServerProUtils::getInstacne()->showAddServiceWindow(point, OGC, pCurrentItem->text());
		}
		break;
	case TILE_SERVER:
		{
			if (pCurrentItem->text() == QString::fromLocal8Bit("TMS"))
			{
				CGxServerProUtils::getInstacne()->showAddServiceWindow(point, TITLE_TMS);
			}
			else if (pCurrentItem->text() == QString::fromLocal8Bit("XYZ"))
			{
				CGxServerProUtils::getInstacne()->showAddServiceWindow(point, TITLE_XYZ);
			}
		}
		break;
	case DATASET_SERVER:
		{
			CGxServerProUtils::getInstacne()->showAddServiceWindow(point, DATASET);
		}
		break;
	default:
		break;
	}
}

void CGxServerDataWindow::linkToSlot()
{
	m_bLinkFiles = true;

	CGxUiFrameworkServiceInterface* pUiFrameWork = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	if (pUiFrameWork)
	{
		CGxDataResourcesUtils::getDataResourceWindow(pUiFrameWork->mainWindow())->show();
		CGxDataResourcesUtils::getDataResourceWindow(pUiFrameWork->mainWindow())->move(x() + width(), y());
	}
}

void CGxServerDataWindow::on_pResetTBtn_clicked()
{
	int nRootCount = m_pItemModel->rowCount();
	for (int nNum = 0; nNum < nRootCount; ++nNum)
	{
		QModelIndex index = m_pItemModel->index(nNum, 0);
		ui->fileSelectedView->setRowHidden(index.row(), index.parent(), false);
	}
	//撤销后还原为无选中状态 l180
	ui->pFileSeletedCBBox->setCurrentIndex(-1);
}

void CGxServerDataWindow::expandItemsSlot( const QModelIndex & index )
{
	if (!isHidden())
	{
		filterAllFiles();
	}	
}

void CGxServerDataWindow::confirmSlot()
{
	//如果名字为空，即没有选中则不创建 l180
	if("" == ui->fileNameEdt->text())
	{
		/*******************************此处需要获取主窗口**************************/

	   CMessageBox::information(tr("please select layer to add")/*, ICore::mainWindowManager()->mainWindow()*/);
	   return;
	}
	// 获取当前选中的Item
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		/*******************************此处需要获取主窗口**************************/

		CMessageBox::information(tr("please select layer to add")/*, ICore::mainWindowManager()->mainWindow()*/);
		return;
	}

	ETreeItemType eItemType = (ETreeItemType)pCurrentItem->data(ItemTypeRole).toInt();
	EServerType eServerType = (EServerType)pCurrentItem->data(ItemServerTypeRole).toInt();
	if (eItemType == LAYER_ITEM )
	{
		if (eServerType == LOCAL_SERVER)
		{
			QFileInfo info(pCurrentItem->data(FilePathRole).toString() + "/" + pCurrentItem->text());
			QString strAbsoluteFilePath = info.absoluteFilePath();
			QString strDriveType;
			CLayerConfig::LayerType  layerType;
			//QString strName = info.baseName();
			// modified by w00025
			QString strName = ui->fileNameEdt->text();
			if (info.suffix() == "shp" || info.suffix() == "kml")
			{
				strDriveType = "ogr";
				layerType = CLayerConfig::VectorLayer;
			}
			else if (info.suffix() == "tif" )
			{
				strDriveType = "gdal";
				layerType = CLayerConfig::ImageLayer;
			}
			else if (info.suffix() == "img")
			{
				strDriveType = "gdal";
				layerType = CLayerConfig::ImageLayer;
			}
			else if (info.suffix() == "000")
			{
				strDriveType = "s57";
			}
			else
			{
				layerType = CLayerConfig::UndefinedLayer;
			}

			// 某些数据类型可以作为多种图层类型添加（如:tif），图层类型由外部指定（解决右键无法添加高程图层问题 fixed by g00034）
			if(CLayerConfig::UndefinedLayer != m_eCurLayerType)
			{
				layerType = m_eCurLayerType;
				m_eCurLayerType = CLayerConfig::UndefinedLayer;
			}

			// modified by w00025
			emit addLayerToSceneSignal(strName, strAbsoluteFilePath, strDriveType, layerType);	
		}
		else 
		{
			SLayerPropertyStruct stLayerPropertyStruct;
			QVariant var = pCurrentItem->data(LayerProRole);
			stLayerPropertyStruct = var.value<SLayerPropertyStruct>();
			CLayerConfig::LayerType  layerType;

			if (stLayerPropertyStruct.strLayerType == tr("image layer"))
			{
				layerType = CLayerConfig::ImageLayer;
			}
			else if (stLayerPropertyStruct.strLayerType == tr("elevation layer"))
			{
				layerType = CLayerConfig::ElevationLayer;
			}
			else if (stLayerPropertyStruct.strLayerType == tr("vector layer"))
			{
				layerType = CLayerConfig::VectorLayer;
			}
			// modified by w00025
			emit addServerLayerToSceneSignal(ui->fileNameEdt->text(), stLayerPropertyStruct, layerType);
			//emit addLayerToSceneSignal(stLayerPropertyStruct.strLayerName, stLayerPropertyStruct.strServerWebsite, stLayerPropertyStruct.strDriveName, layerType);
	    }	
	}
	else
	{
		// 此处需要获取主窗口

		CMessageBox::information(tr("please reselect layer to aa")/*, ICore::mainWindowManager()->mainWindow()*/);
		return;
	}

	close();
}

void CGxServerDataWindow::cancelSlot()
{
	close();
}

void CGxServerDataWindow::filterAllFiles()
{
	// 获取到本地数据节点
	QStandardItem *pItem = m_pItemModel->item(0);

	// 存在文件类型限制，过滤
	if (m_listFilefilters.count() > 0)
	{
		filterFile(pItem);
	}

	if (m_bLinkFiles)
	{
		m_bLinkFiles = false;
		ui->fileSelectedView->expand(pItem->index());
	}
}

void CGxServerDataWindow::filterFile( QStandardItem* pItem )
{
	for ( int i = 0;i < pItem->rowCount(); i++)
	{
		QStandardItem *pChildItem = pItem->child(i);
		if (pChildItem->rowCount() > 0)
		{
			filterFile(pChildItem);
		}
		else
		{
			//文件链接			
			if(pChildItem->data(LinkRole) == FILELINK)
			{
				QString strSuffix = pChildItem->text().split('.').last();
				if (m_listFilefilters.contains(strSuffix))
				{
					ui->fileSelectedView->setRowHidden(m_pItemModel->indexFromItem(pChildItem).row(), m_pItemModel->indexFromItem(pChildItem).parent(), false);
				}
				else
				{
					ui->fileSelectedView->setRowHidden(m_pItemModel->indexFromItem(pChildItem).row(), m_pItemModel->indexFromItem(pChildItem).parent(), true);
				}
			}			
		}
	}
}
