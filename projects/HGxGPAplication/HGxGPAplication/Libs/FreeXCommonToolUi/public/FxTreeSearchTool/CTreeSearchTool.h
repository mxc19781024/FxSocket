#ifndef TREE_SEARCH_TOOL_H
#define TREE_SEARCH_TOOL_H

#include <QObject>
//#include <FreeXSysConfig.h>
#include "freexcommontoolui_export.h"

class QTreeWidget;
class QTreeView;
class CTreeSearchToolPrivate;

/**
* @class	: CTreeSearchTool
* @brief	: 树的搜索工具
* @note	
* @author	: k00026
**/
class FREEXCOMMONTOOLUI_EXPORT CTreeSearchTool : public QObject
{
	Q_OBJECT

public:
	// 搜索 QTreeWidget
	static void search(const QString &keyword = QString(), QTreeWidget *treeWidget = nullptr);

	// 搜索 QTreeView + QStandardItemModel
	static void search(const QString &keyword = QString(), QTreeView *treeView = nullptr);

	// 设置是否大小写敏感(默认大小写不敏感)
	static void setSearchCaseSensitivity(Qt::CaseSensitivity cs);

	// 设置搜索时，是否自动展开到搜索到的项(默认不会自动展开)
	static void setAutoExpand(bool expand);

	// 设置搜索模式(暂未实现)
	static void setMode();

	// 设置搜索的role(暂未实现)
	static void setSearchRole();

private:
	CTreeSearchTool(QObject *parent = nullptr);
	~CTreeSearchTool();

private:
	static QScopedPointer<CTreeSearchToolPrivate> d_ptr;
};

#endif // TREE_SEARCH_TOOL_H
