#ifndef FX_COLOR_SELECTOR_PRIVATE_H
#define FX_COLOR_SELECTOR_PRIVATE_H

#include <QWidget>

class CColorSelectWidget;
class FxColorSelector;

class FxColorSelectorPrivate : public QObject
{
	Q_OBJECT
	Q_DECLARE_PUBLIC(FxColorSelector)

public:
	FxColorSelectorPrivate(FxColorSelector *q);
	~FxColorSelectorPrivate();

protected:
	bool eventFilter(QObject *obj, QEvent *event);

private slots:
	void colorChangedSlot(const QColor &color);

public:
	FxColorSelector *q_ptr;					// q指针
	CColorSelectWidget *m_pColorSelcWgt;	// 颜色选取窗口
	QColor m_color;							// 当前颜色
};

#endif