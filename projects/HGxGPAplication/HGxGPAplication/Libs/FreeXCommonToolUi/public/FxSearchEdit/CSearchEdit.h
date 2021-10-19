#ifndef SEARCH_EDIT_H
#define SEARCH_EDIT_H

#include <QLineEdit>
#include "bdpCommon/GxStrUtil.h"
#include "freexcommontoolui_export.h"

/**
* @class	: CSearchEdit
* @brief	: 搜索框(用于图层、服务、标绘树的检索)
* @note		:
* @author	: k00026
**/
#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif
class FREEXCOMMONTOOLUI_EXPORT CSearchEdit : public QLineEdit
{
	Q_OBJECT

public:
	CSearchEdit(QWidget *parent = nullptr);
	~CSearchEdit();
};

#endif
