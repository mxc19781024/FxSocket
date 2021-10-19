#ifndef FX_COLOR_SELECTOR_H
#define FX_COLOR_SELECTOR_H

#include <QWidget>
//#include <FreeXSysConfig.h>

#include "freexcommontoolui_export.h"

class FxColorSelectorPrivate;
#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif
class FREEXCOMMONTOOLUI_EXPORT FxColorSelector : public QWidget
{
	Q_OBJECT

public:
	FxColorSelector(QWidget *parent = nullptr, const QColor &color = QColor("#ffffff"));
	~FxColorSelector();

	// 设置当前颜色
	void setCurrentColor(const QColor &);

	// 获取当前颜色
	QColor getCurrentColor() const;

signals:
	// 颜色改变发出信号
	void colorChangedSignal(const QColor &);

private:
	Q_DECLARE_PRIVATE(FxColorSelector)
	Q_DISABLE_COPY(FxColorSelector)						// 禁止复制
	const QScopedPointer<FxColorSelectorPrivate> d_ptr;	// d指针
};

#endif // FX_COLOR_SELECTOR_H
