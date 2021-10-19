#include "C3DRealSceneSelectWidget.h"

#include "C3DRealSceneItem.h"
#include <QStyleOption>
#include <QPainter>
#include <QScrollBar>
#include <QFile>




C3DRealSceneSelectWidget::C3DRealSceneSelectWidget(QWidget *parent)
	: QScrollArea(parent)
	, m_pCurSceneToolItem(nullptr)
{
	// 内部widget可自适应大小
	setWidgetResizable(true);
	(const_cast<QScrollBar *>(verticalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	(const_cast<QScrollBar *>(horizontalScrollBar()))->setContextMenuPolicy(Qt::NoContextMenu);
	// 初始化内部widget
	QWidget *pContainerWgt = new QWidget;
	pContainerWgt->setObjectName("fx_toolitem_container_wgt");
	setWidget(pContainerWgt);
	//m_pLayout = new CFlexibleFlowLayout(0, 0);
	
	m_pLayout = new CFlexibleFlowLayout(0, 0);
	m_pLayout->setContentsMargins(0, 0, 0, 0);
	m_pLayout->setMinimumItemWidth(80);
	pContainerWgt->setLayout(m_pLayout);
}

C3DRealSceneSelectWidget::~C3DRealSceneSelectWidget()
{

}

C3DRealSceneItem * C3DRealSceneSelectWidget::addPlotType(bool bIsCheckable, const QString &name, const QPixmap &normalPixmap
	, const QPixmap &hoverPixmap, const QPixmap &selectedPixmap)
{
	C3DRealSceneItem *pToolItem = new C3DRealSceneItem(this, bIsCheckable, name, normalPixmap, hoverPixmap, selectedPixmap);
	m_pLayout->addWidget(pToolItem);
	connect(pToolItem, SIGNAL(itemClickedSignal(bool)), this, SLOT(sceneToolClickedSlot(bool)));
	return pToolItem;
}

void C3DRealSceneSelectWidget::sceneToolClickedSlot(bool bSelected)
{
	C3DRealSceneItem *pSceneToolItem = (C3DRealSceneItem *)(sender());
	if (pSceneToolItem == nullptr)
	{
		return;
	}
	if(!pSceneToolItem->isCheckable())
		return;
	if (bSelected)
	{
		if (m_pCurSceneToolItem == nullptr)
		{
			m_pCurSceneToolItem = pSceneToolItem;
		}
		else
		{
			if (m_pCurSceneToolItem != pSceneToolItem)
			{
				m_pCurSceneToolItem->setSelected(false);
				m_pCurSceneToolItem = pSceneToolItem;
			}
		}
	}
	else
	{
		m_pCurSceneToolItem = nullptr;
	}
}

void C3DRealSceneSelectWidget::setCurrentItem( C3DRealSceneItem* pCurSceneToolItem )
{
	m_pCurSceneToolItem = pCurSceneToolItem;
}