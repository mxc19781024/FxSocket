#include <QMetaType>
#include <QMenu>
#include <service/event/ctkEventConstants.h>
#include <CSimuListWidget.h>
#include <FxMessageBox/CMessageBox.h>
#include <qgraphicseffect.h>
#include "ui_CSimuListWidget.h"

enum EItemType
{
	UnitFactory = 0,
	UnitObject=1,
};
enum EItemDataType
{
	ItemType = Qt::UserRole,
	EntityType = Qt::UserRole+1,
	UnitObjectID = Qt::UserRole+2,
};
Q_DECLARE_METATYPE(EItemType)

CSimuListWidget::CSimuListWidget( ctkPluginContext *context, QWidget *parent /*= nullptr*/ )
	:QWidget(parent)
	,m_pContext(context)
	,m_pRemoveAction(nullptr)
	,m_pTraceAction(nullptr)
	,m_pPropertyActiton(nullptr)
	,m_pCurItem(nullptr)
	,m_pMenu(nullptr)
	//,m_pBizUnitService(nullptr)
	,m_bSimuState(true)
	,ui(new Ui::CSimuListWidget)
{
	/*QVBoxLayout* pLayout = new QVBoxLayout;
	m_pCentralWgt->setLayout(pLayout);

	QWidget* pWgt = new QWidget;
	pLayout->addWidget(pWgt);
	ui->setupUi(pWgt);
	pWgt->resize(200,740);*/
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);
	this->resize(200,740);
	init();
	initStyle();

	// 设置勾选框列宽
	ui->objectTreeWidget->setColumnWidth(0, 30);

	// 注册单元服务事件
	ctkDictionary props;
	//props.insert(ctkEventConstants::EVENT_TOPIC, FxBizUnit::SFxBizUnitEventConstants::topic().c_str());
	context->registerService<ctkEventHandler>(this, props);
}

CSimuListWidget::~CSimuListWidget()
{

}

void CSimuListWidget::init()
{
	m_pAddAction = new QAction("添加", this);
	m_pClearAction = new QAction("清空", this);
	m_pRemoveAction = new QAction("删除", this);
	m_pTraceAction = new QAction("锁定", this);
	m_pPropertyActiton = new QAction("属性", this);

	m_pMenu = new QMenu(this);
	//m_pMenu->setVisible(false);
	m_pMenu->addAction(m_pAddAction);
	m_pMenu->addAction(m_pClearAction);
	m_pMenu->addAction(m_pRemoveAction);
	m_pMenu->addAction(m_pTraceAction);
	m_pMenu->addAction(m_pPropertyActiton);

	connect(m_pAddAction, SIGNAL(triggered(bool)), this, SLOT(addClickedSlot(bool)));
	connect(m_pClearAction, SIGNAL(triggered(bool)), this, SLOT(clearClickedSlot(bool)));
	connect(m_pRemoveAction, SIGNAL(triggered(bool)), this, SLOT(removeClickedSlot(bool)));
	connect(m_pTraceAction, SIGNAL(triggered(bool)), this, SLOT(traceClickedSlot(bool)));
	connect(m_pPropertyActiton, SIGNAL(triggered(bool)), this, SLOT(propertyClickedSlot(bool)));

	connect(ui->objectTreeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(contextMenuSlot(const QPoint&)));
	connect(ui->objectTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(itemChangedSlot(QTreeWidgetItem *, int)));	
	connect(ui->CloseBtn,SIGNAL(clicked()),this,SLOT(closeWgtSlot()));
	connect(this, SIGNAL(removeServiceSignal()), this, SLOT(removeServiceSlot()));
	connect(this, SIGNAL(updateServiceSignal()), this, SLOT(updateServiceSlot()));

	if (getService())
	{
		updateTree(); //   
	}
}

void CSimuListWidget::initStyle()
{
	ui->objectTreeWidget->setStyleSheet("\
									   QTreeWidget::branch:has-children:!has-siblings:closed,\
									   QTreeWidget::branch:closed:has-children:has-siblings{border-image:none;image:url(:/images/resrc/images/arrow_close.png);}\
									   QTreeWidget::branch:open:has-children:!has-siblings,\
									   QTreeWidget::branch:open:has-children:has-siblings{border-image:none;image:url(:/images/resrc/images/arrow_open.png);}");
}

void CSimuListWidget::handleEvent( const ctkEvent& event )
{
// 	if (FxBizUnit::CFxBizUnitEvent::isBizUnitEvent(event) )
// 	{
// 		if(EventUnitServiceAdd == FxBizUnit::CFxBizUnitEvent::getEventType(event))
// 		{
// 			emit updateServiceSignal();
// 		}
// 		else if (EventUnitServiceRemove == FxBizUnit::CFxBizUnitEvent::getEventType(event))
// 		{
// 			emit removeServiceSignal();
// 		}
// 		else if (EventUnitServiceUpdate == FxBizUnit::CFxBizUnitEvent::getEventType(event))
// 		{
// 			emit updateServiceSignal();
// 		}
// 	}	
}

bool CSimuListWidget::getService()
{
// 	if (m_pBizUnitService)
// 	{
// 		return true;
// 	}
// 	FxCore::CFxServiceQuerier<FxBizUnit::IFxBizUnitService> pBizUnitService;
// 	if (pBizUnitService)
// 	{
// 		m_pBizUnitService = pBizUnitService.get();
// 	}
// 	if (m_pBizUnitService)
// 	{
// 		return true;
// 	}
// 	else
// 	{
 		return false;
// 	}
}

void CSimuListWidget::contextMenuSlot( const QPoint& point )
{
	m_pCurItem = ui->objectTreeWidget->itemAt(point);
	if (m_pCurItem)
	{
		if (m_bSimuState)
		{
			if (m_pCurItem->data(0,ItemType).value<EItemType>() == UnitObject)
			{
				m_pAddAction->setVisible(false);
				m_pClearAction->setVisible(false);
				m_pRemoveAction->setVisible(false);
				m_pTraceAction->setVisible(true);
				m_pPropertyActiton->setVisible(true);
			}
			else if (m_pCurItem->data(0,ItemType).value<EItemType>() == UnitFactory)
			{
				m_pAddAction->setVisible(false);
				m_pClearAction->setVisible(false);
				m_pRemoveAction->setVisible(false);
				m_pTraceAction->setVisible(false);
				m_pPropertyActiton->setVisible(false);
				return;
			}
		}
		else
		{
			if (m_pCurItem->data(0,ItemType).value<EItemType>() == UnitObject)
			{
				m_pAddAction->setVisible(false);
				m_pClearAction->setVisible(false);
				m_pRemoveAction->setVisible(true);
				m_pTraceAction->setVisible(true);
				m_pPropertyActiton->setVisible(true);
			}
			else if (m_pCurItem->data(0,ItemType).value<EItemType>() == UnitFactory)
			{
				m_pAddAction->setVisible(true);
				m_pClearAction->setVisible(true);
				m_pRemoveAction->setVisible(false);
				m_pTraceAction->setVisible(false);
				m_pPropertyActiton->setVisible(false);
			}
		}
		
		m_pMenu->exec(QCursor::pos());
	}
}

void CSimuListWidget::addClickedSlot( bool )
{
// 	if (getService()&&m_pCurItem->data(0,ItemType).value<EItemType>() == UnitFactory)
// 	{
// 		auto strEntityType = m_pCurItem->data(0,EntityType).toString();
// 		m_pBizUnitService->addObjectPropertyVisible(strEntityType.toStdString());
// 	}
}

void CSimuListWidget::clearClickedSlot( bool )
{
	if(nullptr == m_pCurItem||!getService()) 
		return;

	if (CMessageBox::question(tr("确认执行清空操作？"))!=CMessageBox::Confirm)
		return;

	auto strEntityType = m_pCurItem->data(0,EntityType).toString();
	if (m_pCurItem->data(0,ItemType).value<EItemType>() == UnitFactory)
	{
		m_mapType2Factory.remove(strEntityType);
	}

	while(m_pCurItem->childCount() > 0)
	{
		auto pDeleteItem = m_pCurItem->child(0);
		removeItem(pDeleteItem);
	}
	m_pCurItem = nullptr;
}

void CSimuListWidget::removeClickedSlot( bool )
{
	if (CMessageBox::question(tr("确认执行删除操作？"))!=CMessageBox::Confirm)
		return;
	removeItem(m_pCurItem);
	m_pCurItem = nullptr;
}

void CSimuListWidget::traceClickedSlot( bool )
{
// 	if (getService()&&m_pCurItem->data(0,ItemType).value<EItemType>() == UnitObject)
// 	{
// 		auto strObjectID = m_pCurItem->data(0,UnitObjectID).toString();
// 		m_pBizUnitService->traceObject(strObjectID.toStdString());
// 	}
}

void CSimuListWidget::propertyClickedSlot( bool )
{
// 	if (getService()&&m_pCurItem->data(0,ItemType).value<EItemType>() == UnitObject)
// 	{
// 		auto strObjectID = m_pCurItem->data(0,UnitObjectID).toString();
// 		m_pBizUnitService->setObjectPropertyVisible(strObjectID.toStdString());
// 	}
}

void CSimuListWidget::itemChangedSlot( QTreeWidgetItem * pItem, int nCol)
{
// 	if (getService())
// 	{
// 		Qt::CheckState state = pItem->checkState(nCol);
// 
// 		if (pItem->data(nCol,ItemType).value<EItemType>() == UnitObject)
// 		{
// 			if (state==Qt::Unchecked)
// 				m_pBizUnitService->setObjectVisible(pItem->data(nCol,UnitObjectID).toString().toStdString(), false);
// 			else if (state==Qt::Checked)
// 				m_pBizUnitService->setObjectVisible(pItem->data(nCol,UnitObjectID).toString().toStdString(), true);
// 			updateParentItem(pItem);
// 		}
// 		else if(pItem->data(nCol,ItemType).value<EItemType>() == UnitFactory)
// 		{
// 			if (state==Qt::Unchecked)
// 			{
// 				m_pBizUnitService->setObjectVisibleAllByType(pItem->data(nCol,EntityType).toString().toStdString(),false);
// 				for(int nIndex=0;nIndex<pItem->childCount();++nIndex)
// 				{
// 					QTreeWidgetItem* pChildItem = pItem->child(nIndex);
// 					pChildItem->setCheckState(nCol,state);
// 				}
// 			}
// 			else if (state==Qt::Checked)
// 			{
// 				m_pBizUnitService->setObjectVisibleAllByType(pItem->data(nCol,EntityType).toString().toStdString(),true);
// 				for(int nIndex=0;nIndex<pItem->childCount();++nIndex)
// 				{
// 					QTreeWidgetItem* pChildItem = pItem->child(nIndex);
// 					pChildItem->setCheckState(nCol,state);
// 				}
// 			}
// 		}
// 	}
}

void CSimuListWidget::removeItem( QTreeWidgetItem* pItem )
{
// 	if(nullptr == pItem||!getService()) 
// 		return;
// 	auto strEntityType = pItem->data(0,EntityType).toString();
// 	if (pItem->data(0,ItemType).value<EItemType>() == UnitObject)
// 	{
// 		auto strObjectID = pItem->data(0,UnitObjectID).toString();
// 		m_pBizUnitService->removeObject(strObjectID.toStdString());
// 		auto mapID2Item = m_mapType2Factory.value(strEntityType);
// 		mapID2Item.remove(strObjectID);
// 		m_mapType2Factory.insert(strEntityType,mapID2Item);
// 	}
// 	else if (pItem->data(0,ItemType).value<EItemType>() == UnitFactory)
// 	{
// 		m_mapID2Item.remove(strEntityType);
// 		m_mapType2Factory.remove(strEntityType);
// 	}
// 
// 	while(pItem->childCount() > 0)
// 	{
// 		auto pDeleteItem = pItem->child(0);
// 		removeItem(pDeleteItem);
// 	}
// 
// 	delete pItem;
// 	pItem = nullptr;
}

void CSimuListWidget::removeServiceSlot()
{
	if (getService())
	{
		while(ui->objectTreeWidget->topLevelItemCount()>0)
		{
			ui->objectTreeWidget->takeTopLevelItem(0);
		}
	}
}

void CSimuListWidget::updateServiceSlot()
{
	updateTree();
}

void CSimuListWidget::updateTree( const QString& strFactoryType )
{
// 	if (strFactoryType.isEmpty())
// 	{
// 		return;
// 	}
// 	if (getService())
// 	{
// 		auto pUnitItem = m_mapID2Item.value(strFactoryType,nullptr);
// 		if(!pUnitItem)
// 		{
// 			ui->objectTreeWidget->blockSignals(true);
// 			pUnitItem = new QTreeWidgetItem(ui->objectTreeWidget,QStringList(strFactoryType));
// 			pUnitItem->setData(0,ItemType,QVariant::fromValue(UnitFactory));
// 			pUnitItem->setData(0,EntityType, QVariant::fromValue(strFactoryType));
// 			pUnitItem->setExpanded(true);
// 			pUnitItem->setCheckState(0,Qt::Checked);
// 			m_mapID2Item.insert(strFactoryType,pUnitItem);
// 			ui->objectTreeWidget->blockSignals(false);
// 		}
// 
// 		auto objectList = m_pBizUnitService->getFactoryObjectList(strFactoryType.toStdString());
// 		auto mapID2Item = m_mapType2Factory.value(strFactoryType);
// 		for(auto itr=mapID2Item.begin();itr!=mapID2Item.end();)
// 		{
// 			if (!objectList.count(itr.key().toStdString()))
// 			{
// 				removeItem(itr.value());
// 				itr = mapID2Item.erase(itr);
// 			}
// 			else
// 			{
// 				++itr;
// 			}
// 		}
// 		for (auto itr=objectList.begin();itr!=objectList.end();++itr)
// 		{
// 			auto pEle = itr->second.get();
// 			if (!pEle)
// 			{
// 				continue;
// 			}
// 			ui->objectTreeWidget->blockSignals(true);
// 			auto pObjectItem = mapID2Item.value(QString::fromStdString(itr->first),nullptr);
// 			if (!pObjectItem)
// 			{
// 				pObjectItem = new QTreeWidgetItem(pUnitItem,QStringList(QString::fromStdString(itr->second->nameEle()->text())));
// 				pObjectItem->setData(0,ItemType,QVariant::fromValue(UnitObject));
// 				pObjectItem->setData(0,EntityType, QVariant::fromValue(strFactoryType));
// 				pObjectItem->setData(0,UnitObjectID, QVariant::fromValue(QString::fromStdString(itr->first)));
// 
// 				mapID2Item.insert(QString::fromStdString(itr->first),pObjectItem);
// 			}
// 			else
// 			{
// 				pObjectItem->setText(0,QString::fromStdString(itr->second->nameEle()->text()));
// 			}
// 
// 			if (itr->second->visible())
// 			{
// 				pObjectItem->setCheckState(0,Qt::Checked);
// 			}
// 			else
// 			{
// 				pObjectItem->setCheckState(0,Qt::Unchecked);
// 			}
// 			ui->objectTreeWidget->blockSignals(false);
// 		}
// 		m_mapType2Factory.insert(strFactoryType,mapID2Item);
// 	}
}

void CSimuListWidget::updateTree()
{
// 	if (getService())
// 	{
// 		foreach(auto factory,m_pBizUnitService->getFactoryList())
// 		{
// 			updateTree(factory.second->entityType().c_str());
// 		}
// 	}
}

void CSimuListWidget::updateParentItem( QTreeWidgetItem* pItem )
{
	QTreeWidgetItem* pParentItem = pItem->parent();
	if (pParentItem == nullptr)
	{
		return;
	}
	int nSelectCount = 0;
	int nChildCount = pParentItem->childCount();
	for (int i = 0; i < nChildCount; i++)
	{
		QTreeWidgetItem* pChildItem = pParentItem->child(i);
		if (pChildItem->checkState(0) == Qt::Checked)
		{
			nSelectCount++;
		}
	}
	if (nSelectCount <= 0)
	{
		pParentItem->setCheckState(0,Qt::Unchecked);
	}
	else if (nSelectCount > 0 && nSelectCount <nChildCount)
	{
		pParentItem->setCheckState(0,Qt::PartiallyChecked);
	}
	else if (nSelectCount == nChildCount)
	{
		pParentItem->setCheckState(0,Qt::Checked);
	}
}

void CSimuListWidget::closeWgtSlot()
{
	close();
}


