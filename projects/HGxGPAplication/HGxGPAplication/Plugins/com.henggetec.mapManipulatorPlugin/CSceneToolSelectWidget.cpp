#include "CSceneToolSelectWidget.h"

#include "CSceneToolItem.h"
#include <QStyleOption>
#include <QPainter>
#include <QScrollBar>
#include <QFile>

CSceneToolSelectWidget::CSceneToolSelectWidget(QWidget *parent)
	: QScrollArea(parent)
	, m_pCurSceneToolItem(nullptr)
{
	// 内部widget可自适应大小
	setWidgetResizable(true);

	// 初始化内部widget
	QWidget *pContainerWgt = new QWidget;
	pContainerWgt->setObjectName("fx_toolitem_container_wgt");
	setWidget(pContainerWgt);
	m_pLayout = new CFlexibleFlowLayout(0, 0);
	m_pLayout->setContentsMargins(0, 0, 0, 0);
	m_pLayout->setMinimumItemWidth(80);
	pContainerWgt->setLayout(m_pLayout);
}

CSceneToolSelectWidget::~CSceneToolSelectWidget()
{

}

CSceneToolItem * CSceneToolSelectWidget::addPlotType(bool bIsCheckable, const QString &name, const QPixmap &normalPixmap
	, const QPixmap &hoverPixmap, const QPixmap &selectedPixmap)
{
	CSceneToolItem *pToolItem = new CSceneToolItem(this, bIsCheckable, name, normalPixmap, hoverPixmap, selectedPixmap);
	m_pLayout->addWidget(pToolItem);
	connect(pToolItem, SIGNAL(itemClickedSignal(bool)), this, SLOT(sceneToolClickedSlot(bool)));
	return pToolItem;
}

void CSceneToolSelectWidget::sceneToolClickedSlot(bool bSelected)
{
	CSceneToolItem *pSceneToolItem = (CSceneToolItem *)(sender());
	if (pSceneToolItem == nullptr)
	{
		return;
	}
	if(!pSceneToolItem->isCheckable())
	{
		return;
	}
	else
	{
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
	}
}

void CSceneToolSelectWidget::setCurrentItem( CSceneToolItem* pCurSceneToolItem )
{
	m_pCurSceneToolItem = pCurSceneToolItem;
}
