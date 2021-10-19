#ifndef CCHECKABELTREEITEM_H
#define CCHECKABELTREEITEM_H

#include <QTreeWidgetItem>

class CCheckableTreeItem : public QTreeWidgetItem
{
public:
	CCheckableTreeItem(int type = Type);
	~CCheckableTreeItem();
	virtual void setLayerVisible(bool) = 0;
	void setData(int column, int role, const QVariant &value) override;
	void setFlag(bool);
	void updateParent(CCheckableTreeItem *, bool);
	void updateSelf();

private:
	void updateChild(CCheckableTreeItem *, bool);

private:
	bool m_bFlag;
};

#endif // CCHECKABELTREEITEM_H
