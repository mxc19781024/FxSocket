#ifndef FX_COLOR_PICKER_WIDGET_H
#define FX_COLOR_PICKER_WIDGET_H

#include <QWidget>

#include "FxBaseWindow/CFxBaseWindow.h"

class FxColorPickerWidgetPrivate;

class FREEXCOMMONTOOLUI_EXPORT FxColorPickerWidget : public CFxBaseWindow
{
	Q_OBJECT

public:
	enum ButtonRole{Yes, No};
	FxColorPickerWidget(QWidget *parent = nullptr);
	~FxColorPickerWidget();
	// 设置当前颜色
	void SetCurrentColor(const QColor &color);
	// 获取当前颜色
	QColor GetCurrentColor() const;

	ButtonRole showDialog();

private:
	Q_DECLARE_PRIVATE(FxColorPickerWidget)
	Q_DISABLE_COPY(FxColorPickerWidget)
	QScopedPointer<FxColorPickerWidgetPrivate> const d_ptr;	// d指针
};

#endif
