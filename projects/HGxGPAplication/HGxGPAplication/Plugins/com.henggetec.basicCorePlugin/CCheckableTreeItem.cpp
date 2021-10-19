#include "CCheckableTreeItem.h"




CCheckableTreeItem::CCheckableTreeItem(int type)
	: QTreeWidgetItem(type)
	, m_bFlag(true)
{
	setCheckState(0, Qt::Checked);
}

CCheckableTreeItem::~CCheckableTreeItem()
{

}

void CCheckableTreeItem::setData(int column, int role, const QVariant &value)
{
	bool bCheckChanged = column == 0
		&& role == Qt::CheckStateRole
		&& data(column, role).isValid()
		&& checkState(column) != value;
	QTreeWidgetItem::setData(column, role, value);
	if (bCheckChanged)
	{
		if (m_bFlag)
		{
			bool bVisible = checkState(column) == Qt::Checked;
			setLayerVisible(bVisible);
			updateParent(this, bVisible);
			updateChild(this, bVisible);
		}
	}
}

void CCheckableTreeItem::setFlag(bool bFlag)
{
	m_bFlag = bFlag;
}

void CCheckableTreeItem::updateParent(CCheckableTreeItem *pItem, bool bVisible)
{
	if (pItem->parent() == nullptr)
	{
		return;
	}
	pItem->setFlag(false);
	CCheckableTreeItem *pParentItem = static_cast<CCheckableTreeItem *>(pItem->parent());
	if (pParentItem != nullptr)
	{
		int iCheckedCount = 0;
		int iUncheckedCount = 0;
		Qt::CheckState checkState = Qt::Unchecked;
		for (int i = 0; i < pParentItem->childCount(); ++i)
		{
			if (pParentItem->child(i)->checkState(0) == Qt::Checked)
			{
				iCheckedCount++;
			}
			else if (pParentItem->child(i)->checkState(0) == Qt::Unchecked)
			{
				iUncheckedCount++;
			}
		}
		if (iCheckedCount == pParentItem->childCount())
		{
			checkState = Qt::Checked;
			pParentItem->setLayerVisible(true);
		}
		else if (iUncheckedCount == pParentItem->childCount())
		{
			checkState = Qt::Unchecked;
			pParentItem->setLayerVisible(false);
		}
		else
		{
			checkState = Qt::PartiallyChecked;
		}
		pParentItem->setFlag(false);
		pParentItem->setCheckState(0, checkState);
		pParentItem->setFlag(true);
		updateParent(pParentItem, bVisible);
	}
	pItem->setFlag(true);
}

void CCheckableTreeItem::updateSelf()
{
	int iCheckedCount = 0;
	int iUncheckedCount = 0;
	Qt::CheckState checkState = Qt::Unchecked;
	for (int i = 0; i < childCount(); ++i)
	{
		if (child(i)->checkState(0) == Qt::Checked)
		{
			iCheckedCount++;
		}
		else if (child(i)->checkState(0) == Qt::Unchecked)
		{
			iUncheckedCount++;
		}
	}
	if (iCheckedCount == childCount())
	{
		checkState = Qt::Checked;
		setLayerVisible(true);
	}
	else if (iUncheckedCount == childCount())
	{
		checkState = Qt::Unchecked;
		setLayerVisible(false);
	}
	else
	{
		checkState = Qt::PartiallyChecked;
	}
	setFlag(false);
	setCheckState(0, checkState);
	setFlag(true);
	updateParent(this, true);
}

void CCheckableTreeItem::updateChild(CCheckableTreeItem *pItem, bool bVisible)
{
	if (pItem->childCount() == 0)
	{
		return;
	}
	QString ss = pItem->text(0);
	for (int i = 0; i < pItem->childCount(); ++i)
	{
		CCheckableTreeItem *pChildItem = static_cast<CCheckableTreeItem *>(pItem->child(i));
		if (pChildItem != nullptr)
		{
			pChildItem->setFlag(false);
			pChildItem->setCheckState(0, pItem->checkState(0));
			pChildItem->setFlag(true);
			pChildItem->setLayerVisible(bVisible);
			updateChild(pChildItem, bVisible);
		}
	}
}
