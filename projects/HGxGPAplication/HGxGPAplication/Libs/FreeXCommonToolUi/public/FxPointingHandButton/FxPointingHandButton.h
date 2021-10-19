#ifndef FXPOINTINGHANDBUTTON_H
#define FXPOINTINGHANDBUTTON_H

#include <QPushButton>
#include "freexcommontoolui_export.h"

class FREEXCOMMONTOOLUI_EXPORT FxPointingHandButton : public QPushButton
{
	Q_OBJECT

public:
	FxPointingHandButton(QWidget *parent);
	~FxPointingHandButton();

protected:
	void enterEvent(QEvent *event);

	void leaveEvent(QEvent *event);

private:
	
};

#endif // FXPOINTINGHANDBUTTON_H
