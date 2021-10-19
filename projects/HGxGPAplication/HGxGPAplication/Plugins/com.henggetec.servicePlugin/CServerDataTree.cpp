#include <QStandardItem>
#include <QMenu>
#include <QAction>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMouseEvent>
#include <QShortcut>
#include <QFile>
#include <QPainter>
#include <QStyleOption>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

#include <service/event/ctkEventConstants.h>
#include <service/event/ctkEventAdmin.h>

#include <scene_ui/ISceneUiService.h>
#include <map_layer_ui/IMapLayerUiService.h>
#include <earth_layer_ui/IEarthLayerUiService.h>
#include <GxUiFrameworkServiceInterface.h>

#include <FxEditingDelegate/CEditingDelegate.h>

#include "public/GxServerProUtils.h"
#include "public/GxServiceProWgt.h"

#include "public/GxDataResourcesUtils.h"
#include "public/GxServerDataWindow.h"
#include "public/GxLinkFileWindow.h"
#include "public/GxServiceModelUtilUi.h"

#include <FxMessageBox/CMessageBox.h>
#include <FxSearchEdit/CSearchEdit.h>
#include "FxTreeSearchTool/CTreeSearchTool.h"
#include <FxToast/CToast.h>
#include "CServerDataTree.h"

#include <qscrollbar.h>
#include <bdpCommon/GxStrUtil.h>
#include <gdpUtil/service/GxDataService.h>
#include <gdpUtil/service/GxOGCService.h>
#include <gdpUtil/service/GxTileService.h>
#include <gdpScene/CommonGeo.h>

CServerDataTree::CServerDataTree(QWidget *parent)
	: QWidget(parent)
	, m_pItemModel(NULL)
	, m_bItemEdit(false)
	, m_pService(NULL)
	, m_bEmbed(false)
{
	ui.setupUi(this);
	(const_cast<QScrollBar *>(ui.pServerTreeView->verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(ui.pServerTreeView->horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
 	QFile file(":/qss/Resources/qss/CServerTreeView.css");
 
 	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
 	{
 		QString str = file.readAll();
 		setStyleSheet(str);
 	}
 
 	// 快捷键
 	QShortcut *pRenameSc = new QShortcut(QKeySequence("F2"), this, SLOT(reNameSlot()), nullptr, Qt::WidgetWithChildrenShortcut);
 	QShortcut *pProperty = new QShortcut(QKeySequence("R"), this, SLOT(showLayerPropertySlot()), nullptr, Qt::WidgetWithChildrenShortcut);
 	QShortcut *pDelete = new QShortcut(QKeySequence("Delete"), this, SLOT(on_pDeleteTBtn_clicked()), nullptr, Qt::WidgetWithChildrenShortcut);

	m_pService = HGxUtil::CGxDataService::getInstance();

	init();
}

CServerDataTree::~CServerDataTree()
{

}

void CServerDataTree::init()
{
	ui.pServerTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.pServerTreeView->header()->hide();
	ui.pServerTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
	CEditingDelegate *pEditingDelegate = new CEditingDelegate;
	ui.pServerTreeView->setItemDelegate(pEditingDelegate);

	m_pItemModel = CGxServiceModelUtilUi::getInstance()->getServiceModel();
	ui.pServerTreeView->setModel(m_pItemModel);

	ui.pFlashTBtn->setToolTip(tr("refresh"));
	ui.pExpandTBtn->setToolTip(tr("expand"));
	ui.pCollapseTBtn->setToolTip(tr("merge"));
	ui.pDeleteTBtn->setToolTip(tr("delete"));

	initSignalsAndSlots();
}

void CServerDataTree::initSignalsAndSlots()
{
	connect(ui.pServerTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(contextMenuSlot(const QPoint &)));

	connect(ui.pServerTreeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClickedSlot(const QModelIndex &)));
	
	connect(m_pItemModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(editItemFinshedSlot(QStandardItem *)));

	connect(ui.pSearchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchPlotTreeSlot(const QString &)));
	
	connect(CGxServiceModelUtilUi::getInstance(), SIGNAL(expandItemsSignal(const QModelIndex &)), this, SLOT(expandItemsSlot(const QModelIndex &)));

}

void CServerDataTree::searchPlotTreeSlot(const QString &keyword)
{
	CTreeSearchTool::search(keyword, ui.pServerTreeView);
}

void CServerDataTree::storageData(ETreeItemType nItemType, EServerType nServerType, QStandardItem *pItem)
{
	if (!pItem)
	{
		return;
	}
	pItem->setData(nItemType, ItemTypeRole);
	pItem->setData(nServerType,ItemServerTypeRole);
}

void CServerDataTree::storageServerProData(const SServerPropertyStruct & stServerPropertyStruct, QStandardItem *pItem)
{
	if (pItem)
	{
		QVariant var;
		var.setValue(stServerPropertyStruct); // copy s into the variant
		pItem->setData(var, ServerProRole);
	}
}

void CServerDataTree::storageLayerProData(const SLayerPropertyStruct & stLayerPropertyStruct, QStandardItem *pItem)
{
	if (pItem)
	{
		QVariant var;
		var.setValue(stLayerPropertyStruct); // copy s into the variant
		pItem->setData(var, LayerProRole);
	}
}

QStandardItem* CServerDataTree::getCurrentItem()
{
	QModelIndex currentIndex = ui.pServerTreeView->currentIndex();
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

void CServerDataTree::paintEvent( QPaintEvent *event )
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

	QWidget::paintEvent(event);
}

void CServerDataTree::contextMenuSlot(const QPoint &pos)
{
	QModelIndex index = ui.pServerTreeView->indexAt(pos);

	if (!index.isValid())
	{
		return;
	}

	QMenu menu(ui.pServerTreeView);

	menu.setWindowFlags(menu.windowFlags() | Qt::FramelessWindowHint);
	menu.setAttribute(Qt::WA_TranslucentBackground);

	// 需要的action
	QAction linkToAct(&menu);			   // 链接文件
	QIcon iconLink;
	iconLink.addPixmap(QPixmap(":/Resources/images/lianjie_hover.png"),QIcon::Normal, QIcon::On);
	iconLink.addPixmap(QPixmap(":/Resources/images/lianjie.png"),QIcon::Active, QIcon::On);
	linkToAct.setIcon(iconLink);
	
	QAction addServerAct(&menu);           // 添加服务
	QAction reFlashAct(&menu);             // 更新
	QIcon iconReflash;
	iconReflash.addPixmap(QPixmap(":/Resources/images/gengxin.png"),QIcon::Normal, QIcon::On);
	iconReflash.addPixmap(QPixmap(":/Resources/images/gengxinhover.png"),QIcon::Active, QIcon::On);
	reFlashAct.setIcon(iconReflash);

	QAction reNameAct(&menu);              // 重命名
	reNameAct.setShortcut(Qt::Key_F2);
	QAction deleteAct(&menu);              // 删除
	deleteAct.setShortcut(Qt::Key_Delete);
	QAction propertyAct(&menu);            // 属性
	propertyAct.setShortcut(Qt::Key_R);
	QAction addToSceneAct(&menu);		   // 添加到场景
	QAction addToVideoAct(&menu);           // 添加影像到场景
	QAction addToHeightAct(&menu);         // 添加高程到场景
	QAction addToVectorAct(&menu);           // 添加矢量到场景
	QAction disConnectAct(&menu);		   // 断开链接
	QAction openFolderAct(&menu);          // 打开文件位置

	QStandardItem *pCurrentItem = m_pItemModel->itemFromIndex(index);

	if (!pCurrentItem)
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

	if (!pCurrentItem->data(ItemTypeRole).isValid())
	{
		return;
	}
	// 如果item的类型是服务器类型
	if (pCurrentItem->data(ItemTypeRole).toInt() == SERVER_TYPE_ITEM)
	{
		switch(pCurrentItem->data(ItemServerTypeRole).toInt())
		{
		case DATASET_SERVER:
			{
				addServerAct.setText(tr("add dataset"));
				QIcon iconAddServer;
				iconAddServer.addPixmap(QPixmap(":/Resources/images/tianjia.png"),QIcon::Normal, QIcon::On);
				iconAddServer.addPixmap(QPixmap(":/Resources/images/tianjia_hover.png"),QIcon::Active, QIcon::On);
				addServerAct.setIcon(iconAddServer);

				connect(&addServerAct,SIGNAL(triggered()), this, SLOT(addServerSlot()));
				if (!bHGLyaer)
				{
					menu.addAction(&addServerAct);
				}
				menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			}
			break;
		case LOCAL_SERVER:
			{
				// add by wkl 20181217
				linkToAct.setText(tr("link file"));
				reFlashAct.setText(tr("refresh"));
				connect(&reFlashAct,SIGNAL(triggered()), this, SLOT(refreshFolderSlot()));
				connect(&linkToAct, SIGNAL(triggered()), this, SLOT(linkToSlot()));
				if (!bHGLyaer)
				{
					menu.addAction(&linkToAct);
				}
				menu.addAction(&reFlashAct);
				menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			}
			break;
		default:
			break;
		}
	    return;
	}

	// 如果item的类型是服务器
	if (pCurrentItem->data(ItemTypeRole).toInt() == SERVER_ITEM)
	{
		switch(pCurrentItem->data(ItemServerTypeRole).toInt()) 
		{
		case LOCAL_SERVER:
			{
				disConnectAct.setText(tr("disconnect"));
				connect(&disConnectAct,SIGNAL(triggered()), this, SLOT(disConnectSlot()));
				reFlashAct.setText(tr("refresh"));
				connect(&reFlashAct,SIGNAL(triggered()), this, SLOT(refreshFolderSlot()));
				openFolderAct.setText(tr("open floder location"));
				connect(&openFolderAct, SIGNAL(triggered()), this, SLOT(openFolderSlot()));
				menu.addAction(&reFlashAct);
				menu.addAction(&openFolderAct);
				menu.addAction(&disConnectAct);
				menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			}
			break;
		case OGC_SERVER:
			addServerAct.setText(tr("add ogc service"));
			addServerAct.setIcon(QIcon(":/Resources/images/tianjia_normal.png"));
			connect(&addServerAct, SIGNAL(triggered()), this, SLOT(addServerSlot()));
			reFlashAct.setText(tr("refresh"));
			connect(&reFlashAct, SIGNAL(triggered()), this, SLOT(reFlashServerSlot())); 
			if (!bHGLyaer)
			{
				menu.addAction(&addServerAct);
			}
			menu.addAction(&reFlashAct);
			menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			break;
		case TILE_SERVER:
			if (!bHGLyaer)
			{
			addServerAct.setText(tr("add tile service"));
			addServerAct.setIcon(QIcon(":/Resources/images/tianjia_normal.png"));
			
				menu.addAction(&addServerAct);
			
			connect(&addServerAct, SIGNAL(triggered()), this, SLOT(addServerSlot()));
			menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			}
			break;
		case DATASET_SERVER:
			break;
		default:
			break;
		}
	    return;
	}

	// 如果item的类型是图层集
	if (pCurrentItem->data(ItemTypeRole).toInt() == LAYER_SET_ITEM)
	{
		switch(pCurrentItem->data(ItemServerTypeRole).toInt()) 
		{
		case LOCAL_SERVER:
			connect(&disConnectAct,SIGNAL(triggered()), this, SLOT(disConnectSlot()));
			reFlashAct.setText(tr("refresh"));
			connect(&reFlashAct,SIGNAL(triggered()), this, SLOT(refreshFolderSlot()));
			openFolderAct.setText(tr("open floder location"));
			connect(&openFolderAct, SIGNAL(triggered()), this, SLOT(openFolderSlot()));
			menu.addAction(&reFlashAct);
			menu.addAction(&openFolderAct);
			menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			break;
		case OGC_SERVER:
			propertyAct.setText(tr("property"));
			connect(&propertyAct,SIGNAL(triggered()), this, SLOT(showLayerPropertySlot()));
			reFlashAct.setText(tr("refresh"));
			connect(&reFlashAct, SIGNAL(triggered()), this, SLOT(reFlashServerSlot())); 
			menu.addAction(&reFlashAct);
			if (!bHGLyaer)
			{
				reNameAct.setText(tr("rename"));
				connect(&reNameAct, SIGNAL(triggered()), this, SLOT(reNameSlot()));
				deleteAct.setText(tr("delete"));
				connect(&deleteAct, SIGNAL(triggered()), this, SLOT(on_pDeleteTBtn_clicked()));
				menu.addAction(&deleteAct);
				menu.addAction(&reNameAct);
			}
			menu.addAction(&propertyAct);
			menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			break;
		default:
			break;
		}
		return;
	}

	// 如果item的类型是图层
	if (pCurrentItem->data(ItemTypeRole).toInt() == LAYER_ITEM)
	{
		switch(pCurrentItem->data(ItemServerTypeRole).toInt()) 
		{
		case LOCAL_SERVER:
			{
				disConnectAct.setText(tr("disconnect"));			

				QString strAbsoluteFilePath = QString(pCurrentItem->data(FilePathRole).toString() + "/" + pCurrentItem->text());
				QFileInfo fileInfo = QFileInfo(strAbsoluteFilePath);	
				QString strFileSuffix = fileInfo.suffix();

				if (fileInfo.suffix() == "shp" || fileInfo.suffix() == "kml")
				{
					addToVectorAct.setText(tr("add to scene"));
					connect(&addToVectorAct,SIGNAL(triggered()), this, SLOT(addToSceneSlot()));
					menu.addAction(&addToVectorAct);
				}
				else if (fileInfo.suffix() == "tif" || fileInfo.suffix() == "img")
				{
					addToHeightAct.setText(tr("add elevation to scene"));
					addToVideoAct.setText(tr("add image to scene"));
					connect(&addToHeightAct,SIGNAL(triggered()), this, SLOT(addAltitudeToSceneSlot()));
					connect(&addToVideoAct,SIGNAL(triggered()), this, SLOT(addImageToSceneSlot()));
					menu.addAction(&addToHeightAct);
					menu.addAction(&addToVideoAct);
				}
				else if (fileInfo.suffix() == "000")
				{
					addToVectorAct.setText(tr("add to scene"));
					connect(&addToVectorAct,SIGNAL(triggered()), this, SLOT(addToSeaSceneSlot()));
					menu.addAction(&addToVectorAct);
				}
				connect(&disConnectAct,SIGNAL(triggered()), this, SLOT(disConnectSlot()));
				openFolderAct.setText(tr("open file location"));
				connect(&openFolderAct, SIGNAL(triggered()), this, SLOT(openFolderSlot()));
				
				menu.addAction(&openFolderAct);
				menu.addAction(&disConnectAct);
				menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			}
			break;
		case OGC_SERVER:
			addToSceneAct.setText(tr("add to scene"));
			connect(&addToSceneAct,SIGNAL(triggered()), this, SLOT(addToSceneSlot()));
			propertyAct.setText(tr("property"));
			connect(&propertyAct,SIGNAL(triggered()), this, SLOT(showLayerPropertySlot()));
			menu.addAction(&addToSceneAct);
			menu.addAction(&propertyAct);
			if (!bHGLyaer)
			{
				deleteAct.setText(tr("delete"));
				connect(&deleteAct, SIGNAL(triggered()), this, SLOT(on_pDeleteTBtn_clicked()));
				menu.addAction(&deleteAct);
			}
			menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			break;
		case TILE_SERVER:
			addToSceneAct.setText(tr("add to scene"));
			connect(&addToSceneAct,SIGNAL(triggered()), this, SLOT(addToSceneSlot()));
			propertyAct.setText(tr("property"));
			connect(&propertyAct,SIGNAL(triggered()), this, SLOT(showLayerPropertySlot()));

			menu.addAction(&addToSceneAct);
			if (!bHGLyaer)
			{
				reNameAct.setText(tr("rename"));
				connect(&reNameAct, SIGNAL(triggered()), this, SLOT(reNameSlot()));
				deleteAct.setText(tr("delete"));
				connect(&deleteAct, SIGNAL(triggered()), this, SLOT(on_pDeleteTBtn_clicked()));
				menu.addAction(&reNameAct);
				menu.addAction(&deleteAct);
			}
			menu.addAction(&propertyAct);
			menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			break;
		case DATASET_SERVER:
			addToSceneAct.setText(tr("add to scene"));
			connect(&addToSceneAct,SIGNAL(triggered()), this, SLOT(addToSceneSlot()));
			propertyAct.setText(tr("property"));
			connect(&propertyAct,SIGNAL(triggered()), this, SLOT(showLayerPropertySlot()));

			menu.addAction(&addToSceneAct);
			if (!bHGLyaer)
			{
				reNameAct.setText(tr("rename"));
				connect(&reNameAct, SIGNAL(triggered()), this, SLOT(reNameSlot()));
				deleteAct.setText(tr("delete"));
				connect(&deleteAct, SIGNAL(triggered()), this, SLOT(on_pDeleteTBtn_clicked()));
				menu.addAction(&reNameAct);
				menu.addAction(&deleteAct);
			}
			menu.addAction(&propertyAct);
			menu.exec(ui.pServerTreeView->viewport()->mapToGlobal(pos)); 
			break;
		default:
			break;
		}
	}	
}

void CServerDataTree::addServerSlot()
{
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}
	// 添加服务的有两类 OGC或者Tile和一种添加数据集驱动
	switch(pCurrentItem->data(ItemServerTypeRole).toInt())
	{
	case OGC_SERVER:
		{
			QString strName = pCurrentItem->text();
			CGxServerProUtils::getInstacne()->showAddServiceWindow(OGC, pCurrentItem->text());	     
		}
		break;
	case TILE_SERVER:
		{
			if (pCurrentItem->text() == QString::fromUtf8("TMS"))
			{
				CGxServerProUtils::getInstacne()->showAddServiceWindow(TITLE_TMS);
			}
			else if (pCurrentItem->text() == QString::fromUtf8("XYZ"))
			{
				CGxServerProUtils::getInstacne()->showAddServiceWindow(TITLE_XYZ);
			}
		}
		break;
	case DATASET_SERVER:
		{
			CGxServerProUtils::getInstacne()->showAddServiceWindow(DATASET);
		}
		break;
	default:
		break;
	}
}

void CServerDataTree::reNameSlot()
{
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}

	if (pCurrentItem->data(HGLayerRole).isValid())
	{
		//恒道云服务不可重命名
		return;
	}
	//编辑前保存当前项名字 l180
	m_strItemName = pCurrentItem->text();

	if (pCurrentItem->data(ItemTypeRole) == SERVER_TYPE_ITEM || pCurrentItem->data(ItemTypeRole) == SERVER_ITEM)
	{
		return;
	}

	if (pCurrentItem->data(ItemServerTypeRole) == OGC_SERVER  && pCurrentItem->data(ItemTypeRole) == LAYER_ITEM )
	{
		return;
	}

	m_bItemEdit = true;
	// 启动item的编辑功能
	QModelIndex currentIndex = ui.pServerTreeView->currentIndex();

	
	if (!currentIndex.isValid())
	{
		return;
	}
	ui.pServerTreeView->edit(currentIndex);
}

void CServerDataTree::reFlashServerSlot()
{
	QStandardItem *pCurrentItem = getCurrentItem();

	//恒道云服务 不存在组节点 要从根节点开始刷新
	if (pCurrentItem->data(HGLayerRole).isValid())
	{
		HGLayeritemRefresh(pCurrentItem);
	}
	else
	{
		// 更新
		if (pCurrentItem->data(ItemTypeRole).toInt() == SERVER_ITEM)
		{
			for (int nNum = 0; nNum < pCurrentItem->rowCount(); nNum++)
			{
				refreshOGCLayerset(pCurrentItem->child(nNum));
			}
		}
		else if (pCurrentItem->data(ItemTypeRole).toInt() == LAYER_SET_ITEM)
		{
			refreshOGCLayerset(pCurrentItem);
		}
	}
}

void CServerDataTree::itemDeleteSLot()
{
	// 获取当前item
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}

	QVector<QStandardItem *> vecOGCLayerSetItem = CGxServiceModelUtilUi::getInstance()->getOGCLayerItems();
	if (pCurrentItem->data(ItemTypeRole).toInt() == LAYER_SET_ITEM)
	{
		for (int i = 0; i < vecOGCLayerSetItem.size(); i++)
		{
			if (pCurrentItem == vecOGCLayerSetItem[i])
			{
				CGxServiceModelUtilUi::getInstance()->removeOGCLayerItem(i);
				break;
			}
		}
	}

	QModelIndex index = pCurrentItem->index();
	m_pItemModel->removeRow(index.row() , index.parent());
}

void CServerDataTree::showLayerPropertySlot()
{
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}

	CGxUiFrameworkServiceInterface* pUiInterface = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	if (NULL == pUiInterface)
	{
		return;
	}

	CGxServiceProWgt* pPropertyWgt = CGxServerProUtils::getInstacne()->getServiceProWindow(UNKNOW);
	if (!m_bEmbed)
	{
		m_bEmbed = true;
		//ICore::toolManager()->addPropertyWidget(pPropertyWgt);
		connect(CGxServerProUtils::getInstacne(), SIGNAL(reSetByProEditSignal(const SServerPropertyStruct &)), this, SLOT(reSetByProEditSlot(const SServerPropertyStruct &)));
		connect(CGxServerProUtils::getInstacne(), SIGNAL(reLayerSetByProEditSignal(const SLayerPropertyStruct &)), this, SLOT(reLayerSetByProEditSlot(const SLayerPropertyStruct &)));
	}

	if (pCurrentItem->data(ItemTypeRole).toInt() == LAYER_ITEM   || pCurrentItem->data(ItemTypeRole).toInt() == LAYER_SET_ITEM)
	{
		if (pCurrentItem->data(ItemTypeRole).toInt() == LAYER_SET_ITEM)
		{
			QVariant var = pCurrentItem->data(ServerProRole);
			SServerPropertyStruct stServerPropertyStruct = var.value<SServerPropertyStruct>();

			CGxServerProUtils::getInstacne()->setServerProPerty(stServerPropertyStruct);

			CGxServiceProWgt* pPropertyWgt = CGxServerProUtils::getInstacne()->getServiceProWindow(OGCSET);;
		}
		else 
		{
			QVariant var = pCurrentItem->data(LayerProRole);
			SLayerPropertyStruct stLayerPropertyStruct = var.value<SLayerPropertyStruct>();

			switch(pCurrentItem->data(ItemServerTypeRole).toInt())
			{
			case OGC_SERVER:
				{
					CGxServerProUtils::getInstacne()->setLayerProperty(OGC, stLayerPropertyStruct);

				}
				break;
			case TILE_SERVER:
				{
					CGxServerProUtils::getInstacne()->setLayerProperty(TITLE_XYZ, stLayerPropertyStruct);

					CGxServiceProWgt* pPropertyWgt = CGxServerProUtils::getInstacne()->getServiceProWindow(TITLE_XYZ);;
				}
				break;
			case DATASET_SERVER:
				{
					CGxServerProUtils::getInstacne()->setLayerProperty(DATASET, stLayerPropertyStruct);

					CGxServiceProWgt* pPropertyWgt = CGxServerProUtils::getInstacne()->getServiceProWindow(DATASET);;
				}
				break;
			default:
				{
					CGxServiceProWgt* pPropertyWgt = CGxServerProUtils::getInstacne()->getServiceProWindow(UNKNOW);;
				}
				break;
			}
		}	
	}
	else
	{
		CGxServiceProWgt* pPropertyWgt = CGxServerProUtils::getInstacne()->getServiceProWindow(UNKNOW);;
	}

	if (pUiInterface)
	{
		//W00085
// 		if (!pUiInterface->isDockExists("LayerProperty"))
// 		{
// 			pUiInterface->addDockWidget("LayerProperty", QStringLiteral("属性"), pPropertyWgt, Qtitan::DockPanelArea::RightDockPanelArea);
// 		}
// 		else
// 		{
// 			pUiInterface->showDockPanel("LayerProperty");
// 		}
	}
}

void CServerDataTree::addToSceneSlot()
{
	// 对于二三维图层而言，创建图层所需要的参数是layerName，driverName， url

	// 获取当前选中的Item
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}

	ETreeItemType eItemType = (ETreeItemType)pCurrentItem->data(ItemTypeRole).toInt();
	EServerType eServerType = (EServerType)pCurrentItem->data(ItemServerTypeRole).toInt();
	if (eItemType == LAYER_ITEM && eServerType == LOCAL_SERVER)
	{
		QFileInfo info(pCurrentItem->data(FilePathRole).toString() + "/" + pCurrentItem->text());
		QString strAbsoluteFilePath = info.absoluteFilePath();
		QString strName = info.fileName();
		QString strCurSceneType = getActiveScene();
		if (strCurSceneType == GX_EARTH_SCENE_TYPE)
		{
			// 获取图层服务
			IEarthLayerUiService* pLayerService = HGXPFCoreInstance.getService<IEarthLayerUiService>(HGxFWLauncherInstance.getPluginContext());

			if (pLayerService == nullptr)
			{
				return;
			}
			// 添加矢量
			pLayerService->addVectorLayer(strName, strAbsoluteFilePath, "ogr");
		}
		else if (strCurSceneType == GX_MAP_SCENE_TYPE)
		{
			// 获取图层服务
			IMapLayerUiService* pLayerService = HGXPFCoreInstance.getService<IMapLayerUiService>(HGxFWLauncherInstance.getPluginContext());

			if (pLayerService == nullptr)
			{
				return;
			}
			// 添加高程
			pLayerService->addMapLayerFromLocalFile(strAbsoluteFilePath);
		}
	}
	else
	{
		SLayerPropertyStruct stLayerPropertyStruct;
		QVariant var = pCurrentItem->data(LayerProRole);
		stLayerPropertyStruct = var.value<SLayerPropertyStruct>();
		QString strCurSceneType = getActiveScene();
		if (strCurSceneType == GX_EARTH_SCENE_TYPE)
		{
			// 获取图层服务
			IEarthLayerUiService *pLayerService = HGXPFCoreInstance.getService<IEarthLayerUiService>(HGxFWLauncherInstance.getPluginContext());

			if (pLayerService == nullptr)
			{
				return;
			}
			if (stLayerPropertyStruct.strLayerType == tr("image layer") || stLayerPropertyStruct.strLayerType == "image layer")
			{
				pLayerService->addImageLayer(stLayerPropertyStruct.strLayerName, stLayerPropertyStruct.strServerWebsite, stLayerPropertyStruct.strDriveName, false, stLayerPropertyStruct.strLayerFormat);
			}
			else if (stLayerPropertyStruct.strLayerType == tr("elevation layer") || stLayerPropertyStruct.strLayerType == "elevation layer")
			{
				pLayerService->addEleLayer(stLayerPropertyStruct.strLayerName, stLayerPropertyStruct.strServerWebsite, stLayerPropertyStruct.strDriveName, false);
			}
			else if (stLayerPropertyStruct.strLayerType == tr("vector layer") || stLayerPropertyStruct.strLayerType == "vector layer")
			{
				pLayerService->addVectorLayer(stLayerPropertyStruct.strLayerName, stLayerPropertyStruct.strServerWebsite, stLayerPropertyStruct.strDriveName, false );
			}
		}
		else if (strCurSceneType == GX_MAP_SCENE_TYPE)
		{
			// 获取图层服务
			IMapLayerUiService *pLayerService = HGXPFCoreInstance.getService<IMapLayerUiService>(HGxFWLauncherInstance.getPluginContext());
			if (pLayerService == nullptr)
			{
				return;
			}
			if (stLayerPropertyStruct.strLayerType == tr("image layer") || stLayerPropertyStruct.strLayerType == "image layer")
			{
				pLayerService->addMapLayerFromService(stLayerPropertyStruct.strServerWebsite, stLayerPropertyStruct.strLayerName, stLayerPropertyStruct.strDriveName,CLayerConfig::ImageLayer, stLayerPropertyStruct.strLayerFormat);
			}
			else if (stLayerPropertyStruct.strLayerType == tr("elevation layer") || stLayerPropertyStruct.strLayerType == "elevation layer")
			{
				pLayerService->addMapLayerFromService(stLayerPropertyStruct.strLayerName, stLayerPropertyStruct.strServerWebsite, stLayerPropertyStruct.strDriveName,CLayerConfig::ElevationLayer);
			}
			else if (stLayerPropertyStruct.strLayerType == tr("vector layer") || stLayerPropertyStruct.strLayerType == "vector layer")
			{
				pLayerService->addMapLayerFromService(stLayerPropertyStruct.strServerWebsite, stLayerPropertyStruct.strLayerName, stLayerPropertyStruct.strDriveName, CLayerConfig::VectorLayer );
			}
		}
	}
}

void CServerDataTree::addAltitudeToSceneSlot()
{
	// 获取当前选中的Item
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}

	QString strAbsolutePath = pCurrentItem->data(FilePathRole).toString();
	QFileInfo info(strAbsolutePath + "/" + pCurrentItem->text());

	QString strName = info.fileName();
	QString strAbsoluteFilePath = info.absoluteFilePath();

	// kzm add begin 高程
	// add note：这样写不好，因为服务是与场景和图层相独立的，直接调图层接口就产生了关联
	// 应该改为发出事件

	// 获取当前场景
	QString strCurSceneType = getActiveScene();
	if (strCurSceneType == GX_EARTH_SCENE_TYPE)
	{
		IEarthLayerUiService* pLayerService = HGXPFCoreInstance.getService<IEarthLayerUiService>(HGxFWLauncherInstance.getPluginContext());
		// 获取图层服务
		if (pLayerService == nullptr)
		{
			return;
		}
		// 添加高程
		pLayerService->addEleLayer(strName, strAbsoluteFilePath, "gdal");
	}
	else if (strCurSceneType == GX_MAP_SCENE_TYPE)
	{
		// 二维不能添加高程 l180
		//CToast::information(tr("tips"), tr("2d scene not support elevation data!"), ICore::mainWindowManager()->sceneWindow());
		CMessageBox::information(tr("2d scene not support elevation data!"), NULL);
	}
	// kzm add begin end 高程
}

void CServerDataTree::addImageToSceneSlot()
{
	// 获取当前选中的Item
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}
	// 驱动名称 flatgis
	QString strAbsoluteFilePath = pCurrentItem->data(FilePathRole).toString();
	QFileInfo info(strAbsoluteFilePath + "/" + pCurrentItem->text());

	QString strName = info.fileName();

	// kzm add begin 影像
	// add note：这样写不好，因为服务是与场景和图层相独立的，直接调图层接口就产生了关联
	// 应该改为发出事件
	// 获取当前场景
	QString strCurSceneType = getActiveScene();
	if (strCurSceneType == GX_EARTH_SCENE_TYPE)
	{
		// 获取图层服务
		IEarthLayerUiService *pLayerService = HGXPFCoreInstance.getService<IEarthLayerUiService>(HGxFWLauncherInstance.getPluginContext());

		if (pLayerService == nullptr)
		{
			return;
		}
		// 添加影像
		pLayerService->addImageLayer(strName, strAbsoluteFilePath + '/' + pCurrentItem->text(), "gdal");
	}
	else if (strCurSceneType == GX_MAP_SCENE_TYPE)
	{
		// 获取图层服务
		IMapLayerUiService *pLayerService = HGXPFCoreInstance.getService<IMapLayerUiService>(HGxFWLauncherInstance.getPluginContext());
		if (pLayerService == nullptr)
		{
			return;
		}
		// 添加影像
		pLayerService->addMapLayerFromLocalFile(info.absoluteFilePath());
	}
	// kzm add end 影像
}


void  CServerDataTree::addToSeaSceneSlot()
{
	// 对于二三维图层而言，创建图层所需要的参数是layerName，driverName， url
	// 获取当前选中的Item
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}

	ETreeItemType eItemType = (ETreeItemType)pCurrentItem->data(ItemTypeRole).toInt();
	EServerType eServerType = (EServerType)pCurrentItem->data(ItemServerTypeRole).toInt();
	if (eItemType == LAYER_ITEM && eServerType == LOCAL_SERVER)
	{
		QFileInfo info(pCurrentItem->data(FilePathRole).toString() + "/" + pCurrentItem->text());
		QString strAbsoluteFilePath = info.absoluteFilePath();
		QString strName = info.fileName();
		QString strCurSceneType = getActiveScene();
		if (strCurSceneType == GX_EARTH_SCENE_TYPE)
		{
			// 获取图层服务
			IEarthLayerUiService *pLayerService = HGXPFCoreInstance.getService<IEarthLayerUiService>(HGxFWLauncherInstance.getPluginContext());

			if (pLayerService == nullptr)
			{
				return;
			}
			// 添加矢量
			pLayerService->addSeaLayer(strName, strAbsoluteFilePath, "s57");
		}
		else if (strCurSceneType == GX_MAP_SCENE_TYPE)
		{
			// 获取图层服务
			IMapLayerUiService *pLayerService = HGXPFCoreInstance.getService<IMapLayerUiService>(HGxFWLauncherInstance.getPluginContext());
			if (pLayerService == nullptr)
			{
				return;
			}
			// 添加高程
			pLayerService->addMapLayerFromLocalFile(strAbsoluteFilePath);
		}
	}
}

void CServerDataTree::linkToSlot()
{
	CGxDataResourcesUtils::getDataResourceWindow(/*ICore::mainWindowManager()->mainWindow()*/)->show();
}

void CServerDataTree::disConnectSlot()
{
	// 获取当前item
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}

	// 获取链接类型
	ELinkType eLinkType = (ELinkType)pCurrentItem->data(LinkRole).toInt();

	if (eLinkType == FILELINK)
	{
		CGxServiceModelUtilUi::getInstance()->removeFileLink(pCurrentItem->data(FilePathRole).toString() + "/" + pCurrentItem->text());
	}
	else if (eLinkType == FOLDERLINK)
	{
		CGxServiceModelUtilUi::getInstance()->removeFileLink(pCurrentItem->text());
	}
	QModelIndex index = pCurrentItem->index();
	m_pItemModel->removeRow(index.row() , index.parent());
}

void CServerDataTree::openFolderSlot()
{
	// 获取当前item
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}
	QString filePath = pCurrentItem->data(FilePathRole).toString();
	QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void CServerDataTree::refreshFolderSlot()
{
	// 获取当前item
	QStandardItem *pCurrentItem = getCurrentItem();

	if (!pCurrentItem)
	{
		return;
	}

	QString strPath;

	if(pCurrentItem->data(ItemTypeRole).toInt() == SERVER_ITEM)
	{
		strPath = pCurrentItem->text();
		checkDirState(strPath, pCurrentItem);
	}
	else if (pCurrentItem->data(ItemTypeRole).toInt() == SERVER_TYPE_ITEM)
	{
		for (int nNum = 0; nNum < pCurrentItem->rowCount(); nNum++)
		{
			QStandardItem *pChildItem = pCurrentItem->child(nNum);
			if (pChildItem->data(ItemTypeRole).toInt() == SERVER_ITEM)
			{
				strPath = pChildItem->text();
				checkDirState(strPath, pChildItem);
			}
			else if (pChildItem->data(ItemTypeRole).toInt() == LAYER_SET_ITEM 
				  || pChildItem->data(ItemTypeRole).toInt() == LAYER_ITEM)
			{
				strPath = QString(pChildItem->data(FilePathRole).toString() + "/" + pChildItem->text());
				checkDirState(strPath, pChildItem);
			}
		}
	}
	else if (pCurrentItem->data(ItemTypeRole).toInt() == LAYER_SET_ITEM || pCurrentItem->data(ItemTypeRole).toInt() == LAYER_ITEM)
	{
		strPath = QString(pCurrentItem->data(FilePathRole).toString() + "/" + pCurrentItem->text());
		checkDirState(strPath, pCurrentItem);
	}
}

void CServerDataTree::checkDirState( const QString strPath, QStandardItem* pParentItem )
{
	if (!pParentItem)
	{
		return;
	}

	// 将子节点清空
	int nRowCount = pParentItem->rowCount();
	for (int nRow = 0; nRow < nRowCount; ++nRow)
	{
		QStandardItem *pChildItem = pParentItem->child(0);
		if (pChildItem)
		{
			QModelIndex index = pChildItem->index();
			m_pItemModel->removeRow(index.row() , index.parent());
		}
	}

	int nLinkRole = pParentItem->data(LinkRole).toInt();

	if (nLinkRole == FOLDERLINK)
	{
		QDir dir(strPath);
		// 不存在
		if (!dir.exists())
		{
			pParentItem->setIcon(QIcon(":/Resources/images/wenjianjiaduankai.png"));
			pParentItem->setData(false, ConnectTypeRole);
		}
		else
		{
			pParentItem->setIcon(QIcon(":/Resources/images/wenjianjia.png"));
			pParentItem->setData(true, ConnectTypeRole);
		}

		// 将之前链接的清空，重新加载
		CGxServiceModelUtilUi::getInstance()->recurTravDir(strPath, pParentItem);
	}
	else if (nLinkRole == FILELINK)
	{
		QFileInfo fileInfo(strPath);
		if (!fileInfo.exists())
		{
			pParentItem->setIcon(QIcon(":/Resources/images/tucengduankai.png"));
			pParentItem->setData(false, ConnectTypeRole);
		}
		else
		{
			pParentItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
			pParentItem->setData(true, ConnectTypeRole);
		}
	}
}

 void CServerDataTree::editItemFinshedSlot( QStandardItem *pItem )
 {
	 if (!m_bItemEdit)
	 {
		 return;
	 }

	 m_bItemEdit = false;

	 m_pItemModel->blockSignals(true);

	 if (pItem)
	 {
		 int nSize = pItem->text().size();
		 if (pItem->text()  == "")
		 {
			 int ret = CMessageBox::information(tr("name can be empty,please re-enter!")/*, ICore::mainWindowManager()->mainWindow()*/);

			 QVariant var = pItem->data(ServerProRole);
			 SServerPropertyStruct stServerPropertyStruct = var.value<SServerPropertyStruct>();
			 //名字为空则设置成上一次的名字 l180
			// pItem->setText(stServerPropertyStruct.strServerName);
			  pItem->setText(m_strItemName);
		 }
		 else
		 {
			 
			 int nType = pItem->data(ItemServerTypeRole).toInt();

			 EWindowType eWindowType;
			  // 对于数据集或者TMS/XYZ而言，数据集的下一级节点为图层节点 服务名称同时也是图层名称
			  if (nType == TILE_SERVER || nType == DATASET_SERVER)
			  {
				  // 将图层中的服务名称和图层名称也进行修改
				  QVariant var = pItem->data(LayerProRole);
				  SLayerPropertyStruct stLayerStruct = var.value<SLayerPropertyStruct>();
					stLayerStruct.strServerName = pItem->text();
					stLayerStruct.strLayerName = pItem->text();
					// 将更改后的信息进行存储
					storageLayerProData(stLayerStruct, pItem);
					nType == TILE_SERVER ? eWindowType = EWindowType::TITLE_TMS : eWindowType = EWindowType::DATASET;
			  }
			  else if (nType == OGC_SERVER)
			  {
				  QVariant var = pItem->data(ServerProRole);
				  SServerPropertyStruct stServerPropertyStruct = var.value<SServerPropertyStruct>();
				  stServerPropertyStruct.strServerName = pItem->text();
				  eWindowType = EWindowType::OGCSET;
				  for (int nNum = 0; nNum < stServerPropertyStruct.vecLayerProperty.size(); ++nNum)
				  {
					  SLayerPropertyStruct stLayerStruct = stServerPropertyStruct.vecLayerProperty.at(nNum);
					  stLayerStruct.strServerName = stServerPropertyStruct.strServerName;
					  stServerPropertyStruct.vecLayerProperty.at(nNum) = stLayerStruct;
					  // 将更改后的信息进行存储
					  storageServerProData(stServerPropertyStruct, pItem);
				  }
			  }

			  //showLayerPropertySlot();
		 }
		 pItem->setToolTip(pItem->text());
	 }
	 m_pItemModel->blockSignals(false);
 }

 void CServerDataTree::reSetByProEditSlot( const  SServerPropertyStruct & stServerPropertyStrut )
 {
	 QStandardItem *m_pCurrentItem = getCurrentItem();

	 if (m_pCurrentItem->data(ItemTypeRole).toInt() == LAYER_SET_ITEM)
	 {
		 m_pCurrentItem->setText(stServerPropertyStrut.strServerName);
		 QVariant var;
		 var.setValue(stServerPropertyStrut); // copy s into the variant
		 m_pCurrentItem->setData(var, ServerProRole);

		 for (int i = 0; i > -1; ++i)
		{
			QStandardItem* item = m_pCurrentItem->child(i);
			if (item)
			{
				QVariant var;
				var.setValue(stServerPropertyStrut.vecLayerProperty.at(i)); // copy s into the variant
				item->setData(var, LayerProRole);
			}
			else
			{
				break;
			}
		}
	 }
	 m_pCurrentItem->setToolTip(m_pCurrentItem->text());
 }

 void CServerDataTree::reLayerSetByProEditSlot( const SLayerPropertyStruct & stLayerPropertyStrut )
 {
	 QStandardItem *m_pCurrentItem = getCurrentItem();

	 if (m_pCurrentItem->data(ItemServerTypeRole).toInt() == DATASET_SERVER)
	 {
		 m_pCurrentItem->setText(stLayerPropertyStrut.strLayerName);
		 QVariant var;
		 var.setValue(stLayerPropertyStrut); // copy s into the variant
		 m_pCurrentItem->setData(var, LayerProRole);
		 m_pCurrentItem->setText(stLayerPropertyStrut.strLayerName);
	 }
	 else if (m_pCurrentItem->data(ItemServerTypeRole).toInt() == TILE_SERVER)
	 {
		 QVariant var;
		 var.setValue(stLayerPropertyStrut); // copy s into the variant
		 m_pCurrentItem->setData(var, LayerProRole);
		 m_pCurrentItem->setText(stLayerPropertyStrut.strLayerName);
	 }
	 else if (m_pCurrentItem->data(ItemTypeRole).toInt() == LAYER_ITEM)
	 {
		 QVariant var;
		 var.setValue(stLayerPropertyStrut); // copy s into the variant
		 m_pCurrentItem->setData(var, LayerProRole);
		 m_pCurrentItem->setText(stLayerPropertyStrut.strLayerName);
	 }

	 m_pCurrentItem->setToolTip(m_pCurrentItem->text());
 }

 void CServerDataTree::HGLayeritemRefresh(QStandardItem *pCurrentItem)
 {
	 if (!pCurrentItem)
	 {
		 return;
	 }

	 //w00224 2020-6-9 刷新前清空一下之前的
	 int nRowCount = pCurrentItem->rowCount();
	 for (int nNum = 0; nNum < nRowCount; nNum++)
	 {
		 QStandardItem *pChildItem = pCurrentItem->child(0);
		 if (pChildItem)
		 {
			 QModelIndex index = pChildItem->index();
			 m_pItemModel->removeRow(index.row() , index.parent());
		 }
	 }

	 QVariant data = pCurrentItem->data(HGServersRole);
	 QVector<SServerPropertyStruct> vecServers = data.value < QVector<SServerPropertyStruct>>();
	 QVector<SServerPropertyStruct> vecServersNew;
	 for (int i = 0; i < vecServers.size(); ++i)
	 {
		 SServerPropertyStruct stServerPropertyStruct = vecServers.at(i);

		 //w00224 2020-6-9 将上次保留的图层信息清空
		 stServerPropertyStruct.vecLayerProperty.clear();

		 SNetService stNetService;
		 stNetService.strServerName = stServerPropertyStruct.strServerName;
		 stNetService.strServerWebsite = stServerPropertyStruct.strServerWebsite;
		 stNetService.strServerType = stServerPropertyStruct.strServerType;
		 stNetService.strServerVersion = stServerPropertyStruct.strServerVersion;
		 stNetService.strUserName = stServerPropertyStruct.strUserName;
		 stNetService.strPassword = stServerPropertyStruct.strPassword;

		 getOGCServerInfo(stNetService, stServerPropertyStruct);

		 if (stServerPropertyStruct.bConnect)
		 {
			 loadOGCLayerNode(pCurrentItem, stServerPropertyStruct);
		 }
		 // 存储图层和服务的信息
		 vecServersNew.push_back(stServerPropertyStruct);
	 }
	 pCurrentItem->setData(QVariant::fromValue(vecServersNew), HGServersRole);

 }

 void CServerDataTree::on_pFlashTBtn_clicked()
 {

	 QStandardItem *pCurrentItem = getCurrentItem();

	 if (!pCurrentItem)
	 {
		// CToast::information(tr("tips"), tr("please selected the service node that need to refreshed"),   ICore::mainWindowManager()->sceneWindow());
		 CMessageBox::information(tr("please selected the service node that need to refreshed"), NULL);
		 return;
	 }

	 int nType = pCurrentItem->data(ItemTypeRole).toInt();

	 // 1.选中的是服务类型节点
	 if (nType == SERVER_TYPE_ITEM)
	 {
		 // 刷新本地服务
		 if (pCurrentItem->data(ItemServerTypeRole) == LOCAL_SERVER)
		 {
			 refreshFolderSlot();
		 }
		 // 刷新OGC服务
		 else if (pCurrentItem->data(ItemServerTypeRole) == OGC_SERVER)
		 {
			 QStandardItem *pOGCItem = m_pItemModel->item(1);
			 for (int nRow = 0; nRow < pOGCItem->rowCount(); ++nRow)
			 {
				 QStandardItem *pChildItem = pOGCItem->child(nRow);
				 for (int nNum = 0; nNum < pChildItem->rowCount(); nNum++)
				 {
					 refreshOGCLayerset(pChildItem->child(nNum));
				 }
			 }
		 }
		 // 刷新TILE服务
		 else if (pCurrentItem->data(ItemServerTypeRole) == TILE_SERVER)
		 {
			 QStandardItem *pTileItem = m_pItemModel->item(2);
			 for (int nRow = 0; nRow < pTileItem->rowCount(); ++nRow)
			 {
				refreshTileLayer(pTileItem->child(nRow));
			 }
		 }
		 // 刷新数据集服务
		 else if (pCurrentItem->data(ItemServerTypeRole) == DATASET_SERVER)
		 {
			 QStandardItem *pDatasetItem = m_pItemModel->item(3);
			 refreshDatasetLayer(pDatasetItem);
		 }
		 else //TODO:恒歌云服务的刷新需要统一处理
		 {
			 if (pCurrentItem->hasChildren() && pCurrentItem->child(0))
			 {
				 if (pCurrentItem->child(0)->data(HGLayerRole).isValid())
				 {
					 for (int nNum = 0; nNum < pCurrentItem->rowCount(); nNum++)
					 {
						 HGLayeritemRefresh(pCurrentItem->child(nNum));
					 }
				 }
			 }
		 }
		  

	 }
	 // 选中的是服务节点(如WMS/WMTS/WCS/WFS/XYZ/TMS)
	 else if (nType == SERVER_ITEM)
	 {
		 if (pCurrentItem->data(ItemServerTypeRole) == LOCAL_SERVER)
		 {
			 refreshFolderSlot();
		 }
		 else if (pCurrentItem->data(ItemServerTypeRole) == OGC_SERVER)
		 {
			 //w00224 2020-6-17 此处如果选择的组节点是恒歌云的数据，那么就只刷新它下的OGC服务
			 if (pCurrentItem->data(HGLayerRole).isValid())
			 {
				 reFlashServerSlot();
				 return;
			 }

			 for (int nNum = 0; nNum < pCurrentItem->rowCount(); nNum++)
			 {
				 refreshOGCLayerset(pCurrentItem->child(nNum));
			 }
		 }
		 else if (pCurrentItem->data(ItemServerTypeRole) == TILE_SERVER)
		 {
			 refreshTileLayer(pCurrentItem);
		 } 
	 }
	 // 选中的是图层集节点
	 else if (nType == LAYER_SET_ITEM)
	 {
		 if (pCurrentItem->data(ItemServerTypeRole) == LOCAL_SERVER)
		 {
			 refreshFolderSlot();
		 }
		 else if (pCurrentItem->data(ItemServerTypeRole) == OGC_SERVER)
		 {
			 refreshOGCLayerset(pCurrentItem);
		 }
	 }
	 // 选中的是图层节点
	 else if (nType == LAYER_ITEM)
	 {
		 if (pCurrentItem->data(ItemServerTypeRole) == TILE_SERVER)
		 {
			 // 获取树节点所保存的图层的数据
			 QVariant var = pCurrentItem->data(ServerProRole);
			 SServerPropertyStruct stServerPropertyStruct = var.value<SServerPropertyStruct>();
			 SNetService stNetService;
			 stNetService.strServerName = stServerPropertyStruct.strServerName;
			 stNetService.strServerWebsite = stServerPropertyStruct.strServerWebsite;
			 stNetService.strServerType = stServerPropertyStruct.strServerType;

			 if (stServerPropertyStruct.vecLayerProperty.size() > 0)
			 {
				 stNetService.strLayerType = stServerPropertyStruct.vecLayerProperty.at(0).strLayerType;
				 stNetService.strLayerName = stServerPropertyStruct.vecLayerProperty.at(0).strLayerName;
			 }

			SServerPropertyStruct stServerInfo;
			 getTileServerInfo(stNetService, stServerInfo);
			 if (!stServerInfo.bConnect)
			 {
				 pCurrentItem->setData(false, ConnectTypeRole);
				 pCurrentItem->setIcon(QIcon(":/Resources/images/tucengduankai.png"));
			 }
			 else
			 {
				 pCurrentItem->setData(true, ConnectTypeRole);
				 pCurrentItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
			 }
			 storageLayerProData(stServerInfo.vecLayerProperty[0], pCurrentItem);
			 storageServerProData(stServerInfo, pCurrentItem);
		 }
		 else if (pCurrentItem->data(ItemServerTypeRole) == DATASET_SERVER)
		 {
			 // 获取树节点所保存的图层的数据
			 QVariant var = pCurrentItem->data(ServerProRole);
			 SServerPropertyStruct stServerPropertyStruct = var.value<SServerPropertyStruct>();
			 SNetService stNetService;
			 stNetService.strServerName = stServerPropertyStruct.strServerName;
			 stNetService.strServerWebsite = stServerPropertyStruct.strServerWebsite;
			 stNetService.strServerType = stServerPropertyStruct.strServerType;

			 if (stServerPropertyStruct.vecLayerProperty.size() > 0)
			 {
				 stNetService.strLayerType = stServerPropertyStruct.vecLayerProperty.at(0).strLayerType;
				 stNetService.strLayerName = stServerPropertyStruct.vecLayerProperty.at(0).strLayerName;
				 stNetService.strDriverName = stServerPropertyStruct.vecLayerProperty.at(0).strDriveName;
			 }
			 SServerPropertyStruct stServerInfo;
			 getDatasetInfo(stNetService, stServerInfo);
			 if (!stServerInfo.bConnect)
			 {
				 pCurrentItem->setData(false, ConnectTypeRole);
				 pCurrentItem->setIcon(QIcon(":/Resources/images/shujuji_duankai.png"));
			 }
			 else
			 {
				 pCurrentItem->setData(true, ConnectTypeRole);
				 pCurrentItem->setIcon(QIcon(":/Resources/images/shujujismall.png"));
			 }
			 storageLayerProData(stServerInfo.vecLayerProperty[0], pCurrentItem);
			 storageServerProData(stServerInfo, pCurrentItem);
		 }
	 }
 }
	 
 void CServerDataTree::on_pExpandTBtn_clicked()
 {
	 	ui.pServerTreeView->expandAll();
 }

 void CServerDataTree::on_pCollapseTBtn_clicked()
 {
	 ui.pServerTreeView->collapseAll();	
 }

 void CServerDataTree::on_pDeleteTBtn_clicked()
 {
	 QStandardItem *pCurrentItem = getCurrentItem();

	 if (!pCurrentItem)
	 {
		 return;
	 }

	 if (pCurrentItem->data(HGLayerRole).isValid())
	 {
		 //恒道云服务不可删除
		 return;
	 }

	 if (pCurrentItem->data(ItemServerTypeRole).toInt() == NONE_SERVER)
	 {
		 CMessageBox::information(tr("this node cannot be deleted!")/*, ICore::mainWindowManager()->mainWindow()*/);
		 return;
	 }

	 int ret = -1;

	 if (pCurrentItem->data(ItemServerTypeRole).toInt() == LOCAL_SERVER)
	 {
		 if (pCurrentItem->data(ItemTypeRole).toInt() == SERVER_TYPE_ITEM)
		 {
			 CMessageBox::information(tr("this node cannot be deleted!")/*, ICore::mainWindowManager()->mainWindow()*/);
			 return;
		 }
		 else
		 {
			 ret = CMessageBox::question(tr("sure to disconnect?")/*, ICore::mainWindowManager()->mainWindow()*/);

			 if (ret == CMessageBox::Cancel)
			 {
				 return;
			 }
			 else
			 {
				 // 断开链接
				 disConnectSlot();
			 }
		 }
	 }
	 else
	 {
		 switch(pCurrentItem->data(ItemTypeRole).toInt())
		 {
		 case SERVER_TYPE_ITEM:
			 CMessageBox::information(tr("this node cannot be deleted!")/*, ICore::mainWindowManager()->mainWindow()*/);
			 return;
			 break;
		 case SERVER_ITEM:
			 CMessageBox::information(tr("this node cannot be deleted!")/*, ICore::mainWindowManager()->mainWindow()*/);
			 return;
			 break;
		 case LAYER_SET_ITEM:
			 ret = CMessageBox::question(tr("sure to delete?")/*, ICore::mainWindowManager()->mainWindow()*/);

			 if (ret == CMessageBox::Cancel)
			 {
				 return;
			 }
			 else
			 {
			     itemDeleteSLot();
			 }	
			 break;
		 case LAYER_ITEM:
			 ret = CMessageBox::question(tr("sure to delete?")/*, ICore::mainWindowManager()->mainWindow()*/);

			 if (ret == CMessageBox::Cancel)
			 {
				 return;
			 }
			 else
			 {
				 itemDeleteSLot();
			 }
			 break;
		 default:
			 break;
		 }
	 }
 }

 void CServerDataTree::expandItemsSlot( const QModelIndex & index )
 {
	 ui.pServerTreeView->expand(index);
 }

 void CServerDataTree::reflashLocalServer(const QString strPath, QStandardItem *pParentItem)
{
	if (NULL == pParentItem)
	{
		return;
	}
	 
	QDir dir(strPath);
	QStringList filters;
	QFileInfo fileInfo;
	filters << "*.shp" << "*.img" << "*.kml" << "*.tif" << ".000";

	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
	int nCount = 0;

	for (int nSize = 0; nSize < fileInfoList.size(); ++nSize)
	{
		fileInfo = fileInfoList.at(nSize);

		QString strName = fileInfo.absoluteFilePath();
		QString strData;
		if(pParentItem->data(ItemTypeRole).toInt() == SERVER_ITEM)
		{
			strData = pParentItem->text();
		}
		else if (pParentItem->data(ItemTypeRole).toInt() == LAYER_SET_ITEM || pParentItem->data(ItemTypeRole).toInt() == LAYER_ITEM)
		{
			strData = QString(pParentItem->data(FilePathRole).toString() + "/" + pParentItem->text());
		}

		if (strData == strName)
		{
			nCount++;
			break;
		}
	}

	if (nCount == 0)
	{
		//todo
		// 设置为失联状态
		pParentItem->setData(false, ConnectTypeRole);
		pParentItem->setIcon(QIcon(":/Resources/images/wenjianjiaduankai.png"));
	}

// 	for (int nNum = 0; nNum < pParentItem->rowCount(); nNum++)
// 	{
// 		reflashLocalServer(fileInfo.absoluteFilePath(), pParentItem->child(nNum));
// 	}	
}

void CServerDataTree::refreshOGCLayerset(QStandardItem* pItem)
{
	if (!pItem)
	{
		return;
	}

	// 将上一次的清空
	int nRowCount = pItem->rowCount();
	for (int nRow = 0; nRow < nRowCount; ++nRow)
	{
		QStandardItem *pChildItem = pItem->child(0);
		if (pChildItem)
		{
			QModelIndex index = pChildItem->index();
			m_pItemModel->removeRow(index.row() , index.parent());
		}
	}

	// 获取树节点所保存的图层集合的数据
	QVariant var = pItem->data(ServerProRole);
	SServerPropertyStruct stServerPropertyStruct = var.value<SServerPropertyStruct>();
	
	// 将上次保留的图层信息清空
	stServerPropertyStruct.vecLayerProperty.clear();

	SNetService stNetService;
	stNetService.strServerName = stServerPropertyStruct.strServerName;
	stNetService.strServerWebsite = stServerPropertyStruct.strServerWebsite;
	stNetService.strServerType = stServerPropertyStruct.strServerType;                                      
	stNetService.strServerVersion = stServerPropertyStruct.strServerVersion;
	stNetService.strUserName = stServerPropertyStruct.strUserName;
	stNetService.strPassword = stServerPropertyStruct.strPassword;

	getOGCServerInfo(stNetService, stServerPropertyStruct);

	if (!stServerPropertyStruct.bConnect)
	{
		// 设置为失联状态
		pItem->setData(false, ConnectTypeRole);
		pItem->setIcon(QIcon(":/Resources/images/wenjianjiaduankai.png"));
	}
	else
	{
		pItem->setData(true, ConnectTypeRole);
		pItem->setIcon(QIcon(":/Resources/images/wenjianjia.png"));
		loadOGCLayerNode(pItem, stServerPropertyStruct);
	}
	// 存储图层和服务的信息
	storageData(LAYER_SET_ITEM, OGC_SERVER, pItem);
	storageServerProData(stServerPropertyStruct, pItem);
}

void CServerDataTree::refreshTileLayer(QStandardItem* pItem)
{
	if (!pItem)
	{
		return;
	}
	int nRowCount = pItem->rowCount();
	for (int nRow = 0; nRow < nRowCount; ++nRow)
	{
		QStandardItem *pChildItem = pItem->child(nRow);
		if (pChildItem)
		{
			// 获取树节点所保存的图层的数据
			QVariant var = pChildItem->data(ServerProRole);
			SServerPropertyStruct stServerPropertyStruct = var.value<SServerPropertyStruct>();
			SNetService stNetService;
			stNetService.strServerName = stServerPropertyStruct.strServerName;
			stNetService.strServerWebsite = stServerPropertyStruct.strServerWebsite;
			stNetService.strServerType = stServerPropertyStruct.strServerType;
			if (stServerPropertyStruct.vecLayerProperty.size() > 0)
			{
				stNetService.strLayerType = stServerPropertyStruct.vecLayerProperty.at(0).strLayerType;
			}

			SServerPropertyStruct stServerInfo;
			getTileServerInfo(stNetService, stServerInfo);
			if (!stServerInfo.bConnect)
			{
				pChildItem->setData(false, ConnectTypeRole);
				pChildItem->setIcon(QIcon(":/Resources/images/tucengduankai.png"));
			}
			else
			{
				pChildItem->setData(true, ConnectTypeRole);
				pChildItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
			}
			storageLayerProData(stServerInfo.vecLayerProperty[0], pChildItem);
			storageServerProData(stServerInfo, pChildItem);
		}
	}
}

void CServerDataTree::refreshDatasetLayer(QStandardItem* pItem)
{
	if (!pItem)
	{
		return;
	}
	int nRowCount = pItem->rowCount();
	for (int nRow = 0; nRow < nRowCount; ++nRow)
	{
		QStandardItem *pChildItem = pItem->child(nRow);
		// 获取树节点所保存的图层的数据
		QVariant var = pChildItem->data(ServerProRole);
		SServerPropertyStruct stServerPropertyStruct = var.value<SServerPropertyStruct>();
		SNetService stNetService;
		stNetService.strServerName = stServerPropertyStruct.strServerName;
		stNetService.strServerWebsite = stServerPropertyStruct.strServerWebsite;
		stNetService.strServerType = stServerPropertyStruct.strServerType;

		SServerPropertyStruct stServerInfo;
		getDatasetInfo(stNetService, stServerInfo);
		if (!stServerInfo.bConnect)
		{
			pChildItem->setData(false, ConnectTypeRole);
			pChildItem->setIcon(QIcon(":/Resources/images/shujuji_duankai.png"));
		}
		else
		{
			pChildItem->setData(true, ConnectTypeRole);
			pChildItem->setIcon(QIcon(":/Resources/images/shujujismall.png"));
		}
		storageLayerProData(stServerInfo.vecLayerProperty[0], pChildItem);
		storageServerProData(stServerInfo, pChildItem);
	}
}

std::string CServerDataTree::getSceneID()
{
	std::string strSceneID = "";

	ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
	if(pSceneService)
	{
		strSceneID = pSceneService->activeScene();
	}
	return strSceneID;
}

void CServerDataTree::getLocalserviceList( QVector<SLinkInfo> &vecLinkInfo )
{
	if (!m_pItemModel)
	{
		return;
	}
	// 获取本地服务树节点
	QStandardItem *pLocalServeItem = m_pItemModel->item(0);

	if (!pLocalServeItem)
	{
		return;
	}

	for (int nRow = 0; nRow < pLocalServeItem->rowCount(); ++nRow)
	{
		QStandardItem *pChildItem = pLocalServeItem->child(nRow);
		if (pChildItem)
		{
			SLinkInfo stLinkInfo;
			ELinkType eLinkType = (ELinkType)pChildItem->data(LinkRole).toInt();
			stLinkInfo.eLinkType = eLinkType;
			if (eLinkType == FOLDERLINK)
			{
				stLinkInfo.strAbsoluteFilePath = pChildItem->text();
				stLinkInfo.strFileName = pChildItem->text();
			}
			else if (eLinkType == FILELINK)
			{
				stLinkInfo.strFileName = pChildItem->text();
				stLinkInfo.strAbsoluteFilePath = QString("%1/%2").arg(pChildItem->data(FilePathRole).toString()).arg(stLinkInfo.strFileName);
			}
			vecLinkInfo.push_back(stLinkInfo);
		}
	}

}

void CServerDataTree::getNetServiceList( ENetServiceType type, QVector<SNetService> &vecNetService )
{
	QStandardItem *pOGCServerItem = m_pItemModel->item(1);
	QStandardItem *pTileServerItem = m_pItemModel->item(2);
	QStandardItem *pDatasetItem = m_pItemModel->item(3);
	if (!pOGCServerItem && !pTileServerItem && !pDatasetItem)
	{
		return;
	}

	switch (type)
	{
	case WMS:
		{
			QStandardItem *pWMSItem = pOGCServerItem->child(0);
			getOGCServiceInfo(pWMSItem, vecNetService);
		}
		break;
	case WMTS:
		{
			QStandardItem *pWMTSItem = pOGCServerItem->child(1);
			getOGCServiceInfo(pWMTSItem, vecNetService);
		}
		break;
	case WFS:
		{
			QStandardItem *pWFSItem = pOGCServerItem->child(2);
			getOGCServiceInfo(pWFSItem, vecNetService);
		}
		break;
	case WCS:
		{
			QStandardItem *pWCSItem = pOGCServerItem->child(3);
			getOGCServiceInfo(pWCSItem, vecNetService);
		}
		break;
	case TMS:
		{
			QStandardItem *pTMSItem = pTileServerItem->child(0);
			getTileOrDatasetInfo(pTMSItem, vecNetService);

		}
		break;
	case XYZ:
		{
			QStandardItem *pXYZItem = pTileServerItem->child(1);
			getTileOrDatasetInfo(pXYZItem, vecNetService);
		}
		break;
	case DATASETS:
		{
			getTileOrDatasetInfo(pDatasetItem, vecNetService);
		}
		break;
	default:
		break;
	}
}

void CServerDataTree::getOGCServiceInfo( QStandardItem *pItem, QVector<SNetService> &vecNetService )
{
	if (!pItem)
	{
		return;
	}
	for (int nRow = 0; nRow < pItem->rowCount(); ++nRow)
	{
		QStandardItem *pChildItem = pItem->child(nRow);
		if (pChildItem)
		{
			QString ss = pChildItem->text();

			SNetService stNetService;
			QVariant var = pChildItem->data(ServerProRole);
			SServerPropertyStruct stServerPro = var.value<SServerPropertyStruct>();
			stNetService.strServerType = stServerPro.strServerType;
			stNetService.strServerName = stServerPro.strServerName;
			stNetService.strServerWebsite = stServerPro.strServerWebsite;
			stNetService.strPassword = stServerPro.strPassword;
			stNetService.strUserName = stServerPro.strUserName;
			stNetService.strServerVersion = stServerPro.strServerVersion;
			stNetService.bDefault = stServerPro.bDefault;

			vecNetService.push_back(stNetService);
		}
	}
}

void CServerDataTree::getTileOrDatasetInfo( QStandardItem *pItem, QVector<SNetService> &vecNetService )
{
	if (!pItem)
	{
		return;
	}
	for (int nRow = 0; nRow < pItem->rowCount(); ++nRow)
	{
		QStandardItem *pChildItem = pItem->child(nRow);
		if (pChildItem)
		{
			SNetService stNetService;
			QVariant var = pChildItem->data(LayerProRole);
			SLayerPropertyStruct stLayerPro = var.value<SLayerPropertyStruct>();
			stNetService.strServerType = stLayerPro.strServerType;
			stNetService.strServerName = stLayerPro.strServerName;
			stNetService.strServerWebsite = stLayerPro.strServerWebsite;
			stNetService.strLayerName = stLayerPro.strLayerName;
			stNetService.bDefault = stLayerPro.bDefault;
			
			// 注：Tile服务无驱动名称，strDriverName字段为空
			stNetService.strDriverName = stLayerPro.strDriveName;
			stNetService.strLayerType = stLayerPro.strLayerType;

			vecNetService.push_back(stNetService);
		}
	}
}

void CServerDataTree::restoreLocalServer( const QVector<SLinkInfo>& vecLinkInfo )
{
	auto iter = vecLinkInfo.begin();

	for (; iter != vecLinkInfo.end(); ++iter)
	{
		CGxServiceModelUtilUi::getInstance()->folderLink(*iter);
	}
}

void CServerDataTree::restoreNetServer( ENetServiceType type, const QVector<SNetService>& vecServiceInfo )
{
	switch (type)
	{
	case WMS:
	case WMTS:
	case WFS:
	case WCS:
		{
			auto iter = vecServiceInfo.begin();
			for (; iter != vecServiceInfo.end(); ++iter)
			{
				SServerPropertyStruct stPropertyStruct;
				getOGCServerInfo(*iter, stPropertyStruct);
				CGxServiceModelUtilUi::getInstance()->newAddLayer(stPropertyStruct);				
			}
		}
		break;
	case TMS:
	case XYZ:
		{
			auto iter = vecServiceInfo.begin();
			for (; iter != vecServiceInfo.end(); ++iter)
			{
				SServerPropertyStruct stPropertyStruct;
				getTileServerInfo(*iter, stPropertyStruct);
				CGxServiceModelUtilUi::getInstance()->newAddLayer(stPropertyStruct);		
			}
		}
		break;
	case DATASETS:
		{
			auto iter = vecServiceInfo.begin();
			for (; iter != vecServiceInfo.end(); ++iter)
			{
				SServerPropertyStruct stPropertyStruct;
				getDatasetInfo(*iter, stPropertyStruct);
				CGxServiceModelUtilUi::getInstance()->newAddLayer(stPropertyStruct);		
			}
		}
		break;
	default:
		break;
	}
}

void CServerDataTree::restoreHGNetServer(ENetServiceType type, const QVector<SNetService>& vecServiceInfo)
{
	switch (type)
	{
	case WMS:
	case WMTS:
	case WFS:
	case WCS:
	{
		auto iter = vecServiceInfo.begin();
		for (; iter != vecServiceInfo.end(); ++iter)
		{
			SServerPropertyStruct stPropertyStruct;
			getOGCServerInfo(*iter, stPropertyStruct);
			CGxServiceModelUtilUi::getInstance()->addHGLayer(stPropertyStruct);
		}
	}
	break;
	case TMS:
	case XYZ:
	{
		auto iter = vecServiceInfo.begin();
		for (; iter != vecServiceInfo.end(); ++iter)
		{
			SServerPropertyStruct stPropertyStruct;
			getTileServerInfo(*iter, stPropertyStruct);
			CGxServiceModelUtilUi::getInstance()->addHGLayer(stPropertyStruct);
		}
	}
	break;
	case DATASETS:
	{
		auto iter = vecServiceInfo.begin();
		for (; iter != vecServiceInfo.end(); ++iter)
		{
			SServerPropertyStruct stPropertyStruct;
			getDatasetInfo(*iter, stPropertyStruct);
			CGxServiceModelUtilUi::getInstance()->addHGLayer(stPropertyStruct);
		}
	}
	break;
	default:
		break;
	}
}

void CServerDataTree::getOGCServerInfo( const SNetService &stNetService, SServerPropertyStruct &stServerInfo )
{
	//服务
 	if (m_pService)
	{
		HGxUtil::CGxOGCService *pService = NULL;

		if (stNetService.strServerType.toUpper() == "WMS")
		{
			pService = m_pService->cretaeWMSService(stNetService.strServerWebsite.toUtf8().data(), stNetService.strServerVersion.toUtf8().data(),
				stNetService.strUserName.toUtf8().data(), stNetService.strPassword.toUtf8().data());
		}
		else if (stNetService.strServerType.toUpper() == "WMTS")
		{
			pService = m_pService->cretaeWMTSService(stNetService.strServerWebsite.toUtf8().data(), stNetService.strServerVersion.toUtf8().data(),
				stNetService.strUserName.toUtf8().data(), stNetService.strPassword.toUtf8().data());
		}
		else if (stNetService.strServerType.toUpper() == "WFS")
		{
			pService = m_pService->cretaeWFSService(stNetService.strServerWebsite.toUtf8().data(), stNetService.strServerVersion.toUtf8().data(),
				stNetService.strUserName.toUtf8().data(), stNetService.strPassword.toUtf8().data());
		}
		else if (stNetService.strServerType.toUpper() == "WCS")
		{
			pService = m_pService->cretaeWCSService(stNetService.strServerWebsite.toUtf8().data(), stNetService.strServerVersion.toUtf8().data(),
				stNetService.strUserName.toUtf8().data(), stNetService.strPassword.toUtf8().data());
		}
		// 服务相关信息
		stServerInfo.strServerType = stNetService.strServerType;
		stServerInfo.strServerName = stNetService.strServerName;                                         
		stServerInfo.strServerVersion = stNetService.strServerVersion;
		stServerInfo.strServerWebsite = stNetService.strServerWebsite;
		stServerInfo.bDefault = stNetService.bDefault;

		if (pService)
		{
			stServerInfo.bConnect = pService->isConnected();

			if (stServerInfo.bConnect)
			{
				pService->refresh();
				//w00224 2020-6-9 刷新的时候清空之前的
				m_serviceFormats.clear();
				//获取子图层
				m_vecLayers = pService->getServiceLayerList();
				for (int i = 0; i < pService->getServiceDataFormatList().size(); ++i)
				{
					m_serviceFormats.push_back(pService->getServiceDataFormatList().at(i).c_str());
				}
				stServerInfo.vecFormats = m_serviceFormats;

				for(int n = 0; n < m_vecLayers.size(); ++n)
				{
					// 存储图层的相关信息
					SLayerPropertyStruct stLayerProperty;
					stLayerProperty.strServerName = stNetService.strServerName;
					stLayerProperty.strServerType = pService->getServiceType().c_str();
					stLayerProperty.strServerWebsite = stServerInfo.strServerWebsite;
					stLayerProperty.strLayerName = QString::fromUtf8(m_vecLayers[n]->getName().c_str());
					stLayerProperty.strDriveName = pService->getServiceType().c_str();
					stLayerProperty.strLayerFormats = m_serviceFormats;

					if (stLayerProperty.strServerType.toUpper() == "WFS")
					{
						stLayerProperty.strLayerType = tr("vector layer");
					}
					else
					{
						stLayerProperty.strLayerType = tr("image layer");
					}
					QString strLayerInfo = tr("name:\n%1\n\nabstract:\n%2\n\npacking box:\n%3\n%4\n\nsupportEPSG、CRS:\n%5，%6\n")\
						.arg(m_vecLayers[n]->getName().c_str()).arg(tr("unknow"))\
						.arg(m_vecLayers[n]->getLowerCorner().c_str()).arg(m_vecLayers[n]->getUpperCorner().c_str())\
						.arg(m_vecLayers[n]->getEPSGCode().c_str()).arg(m_vecLayers[n]->getCRSCode().c_str());
					stLayerProperty.strLayerInfo = strLayerInfo;
					stServerInfo.vecLayerProperty.push_back(stLayerProperty);
				}
			}
		}
	}
}

void CServerDataTree::getTileServerInfo( const SNetService &stNetService, SServerPropertyStruct &stServerInfo )
{
	bool bConnect = false;
	bool bLocal = false;


	if (m_pService)
	{
		HGxUtil::CGxTileservice *pService = NULL;

		QString strType = stNetService.strServerType.toUpper();

		if (strType == "TMS")
		{
			pService = m_pService->cretaeTMSService(stNetService.strServerWebsite.toUtf8().data());
			
			QFileInfo info(stNetService.strServerWebsite);
			if (info.exists())
			{
				bConnect = true;
				bLocal = true;
			}
		}
		else if (strType == "XYZ")
		{
			pService = m_pService->cretaeXYZService(stNetService.strServerWebsite.toUtf8().data());
		}

		if (!bLocal)
		{
			bConnect = pService->isConnected();
		}
		stServerInfo.bConnect = bConnect;
		stServerInfo.strServerName = stNetService.strServerName;
		stServerInfo.strServerType = strType;
		stServerInfo.strServerWebsite = pService->getServiceURL().c_str();


		SLayerPropertyStruct stLayerProperty;
		stLayerProperty.strLayerName = stNetService.strLayerName;
		stLayerProperty.strLayerType = stNetService.strLayerType;
		stLayerProperty.strServerName = stServerInfo.strServerName;
		stLayerProperty.strServerType = pService->getServiceType().c_str();
		stLayerProperty.strServerWebsite = pService->getServiceURL().c_str();
		stLayerProperty.strDriveName = pService->getServiceType().c_str();
		stLayerProperty.bDefault = stNetService.bDefault;
		stServerInfo.vecLayerProperty.push_back(stLayerProperty);
	}
}

void CServerDataTree::getDatasetInfo( const SNetService &stNetService, SServerPropertyStruct &stServerInfo )
{
	bool bConnect = false;
	bool bLocal = false;

	//服务
	if (m_pService)
	{
		HGxUtil::CGxTileservice *pService = NULL;
		
		pService = m_pService->cretaeDataService(stNetService.strServerWebsite.toUtf8().data(),  stNetService.strDriverName.toUtf8().data());
		
		QFileInfo info(stNetService.strServerWebsite);

		if (info.exists())
		{
			bConnect = true;
			bLocal = true;
		}
		if (pService)
		{
			if (!bLocal)
			{
				bConnect = pService->isConnected();
			}
			stServerInfo.strServerType = tr("dataset");
			stServerInfo.bConnect = bConnect;
			stServerInfo.strServerName = stNetService.strServerName;
			stServerInfo.strServerWebsite = pService->getServiceURL().c_str();

// 			if (bConnect)
// 			{
			SLayerPropertyStruct  stLayerProperty;

			stLayerProperty.strLayerName = stNetService.strLayerName;
			stLayerProperty.strServerName = stNetService.strServerName;
			stLayerProperty.strServerType = tr("dataset");
			stLayerProperty.strServerWebsite = pService->getServiceURL().c_str();
			stLayerProperty.strDriveName = pService->getServiceType().c_str();
			stLayerProperty.strLayerType = stNetService.strLayerType;
			stLayerProperty.bDefault = stNetService.bDefault;

			stServerInfo.vecLayerProperty.push_back(stLayerProperty);
/*			}*/
		}
	}
}

void CServerDataTree::itemClickedSlot( const QModelIndex &index )
{
	QStandardItem *pItem = m_pItemModel->itemFromIndex(index);
	if (!pItem)
	{
		return;
	}
	int eItemType = pItem->data(ItemTypeRole).toInt();
	

	// 设置删除按钮的可用状态
	// 图层节点以及图层集合节点可以删除
	if (eItemType == LAYER_ITEM || eItemType == LAYER_SET_ITEM)
	{
		ui.pDeleteTBtn->setEnabled(true);
	}
	// 除此之外不能删除
	else
	{
		ui.pDeleteTBtn->setEnabled(false);
	}

// 	// 如果属性窗口可视的话，需要刷新属性窗口的内容
// // 	if (ICore::toolManager()->propertyWidgetCreatedAndVisible())
// // 	{
// 		showLayerPropertySlot();
// //	}
}

bool CServerDataTree::bFindLayerInServiceList( const QString &strLayerName, QStandardItem *pItem )
{
	if (!pItem)
	{
		return false;
	}
	for (int nRow = 0; nRow < pItem->rowCount(); ++nRow)
	{
		if (pItem->child(nRow)->text() == strLayerName)
		{
			return true;
		}
	}
	return false;
}

void CServerDataTree::loadOGCLayerNode( QStandardItem *pItem, const SServerPropertyStruct& stServerInfo )
{
	for (int j = 0; j < stServerInfo.vecLayerProperty.size();j++)
	{
		QStandardItem *pLayerItem = new QStandardItem;
		pLayerItem->setToolTip(stServerInfo.vecLayerProperty[j].strLayerName);
		pLayerItem->setText(stServerInfo.vecLayerProperty[j].strLayerName);
		pLayerItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
		storageData(LAYER_ITEM, OGC_SERVER, pLayerItem);
		storageLayerProData(stServerInfo.vecLayerProperty[j], pLayerItem);
		storageServerProData(stServerInfo, pLayerItem);
		pItem->appendRow(pLayerItem);
	}
}

void CServerDataTree::setNodeDisConStatus( ENetServiceType type, const SServerPropertyStruct &stPropertyStruct )
{

}

QString CServerDataTree::getActiveScene()
{
	ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
	if(!pSceneService)
	{
		return "";
	}
	return pSceneService->activeSceneType();
}
