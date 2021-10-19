#ifndef CPENSTYLESELECTOR_H
#define CPENSTYLESELECTOR_H

#include <QComboBox>
//#include <FreeXSysConfig.h>
#include "freexcommontoolui_export.h"

class FREEXCOMMONTOOLUI_EXPORT CPenStyleSelector : public QComboBox
{
	Q_OBJECT

public:
	CPenStyleSelector(QWidget *parent);
	~CPenStyleSelector();

	void setLineStyle(uint);

	uint lineStyle();
};

#endif // !C_PENSTYLE_SELECTOR_H
