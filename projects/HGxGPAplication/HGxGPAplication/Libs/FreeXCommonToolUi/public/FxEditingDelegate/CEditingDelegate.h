#ifndef EDITING_DELEGATE_H
#define EDITING_DELEGATE_H

#include <QStyledItemDelegate>
//#include <FreeXSysConfig.h>

#include "freexcommontoolui_export.h"

/**
* @class	: CEditingDelegate
* @brief	: 编辑树节点代理
* @note		: 用于树节点的重命名
* @author	: k00026
**/
#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif
class FREEXCOMMONTOOLUI_EXPORT CEditingDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	CEditingDelegate(QObject *parent = nullptr, int maxLength = 20);
	~CEditingDelegate();
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
	void editingFinishedSignal(const QModelIndex &) const;

private:
	int m_iMaxLength;
};

#endif // EDITING_DELEGATE_H
