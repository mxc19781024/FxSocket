#include "FxSearchEdit/CSearchEdit.h"

#include <QLabel>
#include <QAction>
#include <QPushButton>
#include <QHBoxLayout>


CSearchEdit::CSearchEdit(QWidget *parent)
	: QLineEdit(parent)
{
	setStyleSheet(".CSearchEdit{min-height:20px;max-height:20px;background:none;border-radius:0px;border-image:url(:/images_searchedit/Resources/images_searchedit/input.png)1 4 1 4;border-width: 1 4 1 4;}\
				  .CSearchEdit:hover{border-image:url(:/images_searchedit/Resources/images_searchedit/input_hover.png) 1 4 1 4;}\
				   .CSearchEdit:focus{border-image:url(:/images_searchedit/Resources/images_searchedit/input_box_click.png) 1 4 1 4;}\
				  CSearchEdit #fx_search_edit{border-image:url(:/images_searchedit/Resources/images_searchedit/search.png);}");
	setPlaceholderText(tr("serch"));

	QPushButton *pSearchBtn = new QPushButton;
	pSearchBtn->setMinimumSize(12, 12);
	pSearchBtn->setMaximumSize(12, 12);
	pSearchBtn->setObjectName("fx_search_edit");
	QHBoxLayout *pHLayout = new QHBoxLayout(this);
	pHLayout->setContentsMargins(6, 0, 6, 0);
	pHLayout->addWidget(pSearchBtn);
	pHLayout->addStretch();

	setTextMargins(14, 0 ,6 , 0);

	setContextMenuPolicy(Qt::NoContextMenu);
	setMaxLength(30);
}

CSearchEdit::~CSearchEdit()
{
}
