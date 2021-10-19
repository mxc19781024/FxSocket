#ifndef C_SIGNLE_COLOR_ITEM_H
#define C_SIGNLE_COLOR_ITEM_H

#include <QWidget>
////#include <FreeXSysConfig.h>

class CSingleColorPickerItemPrivate;

class CSingleColorPickerItem : public QWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(CSingleColorPickerItem)

public:
	CSingleColorPickerItem(const QColor &, QWidget *parent = 0);
	~CSingleColorPickerItem();
	// 获得颜色
	QColor GetColor();
	// 设置颜色
	void SetColor(const QColor &);

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

signals:
	void ItemClickedSignal(const QColor &);

private:
	Q_DISABLE_COPY(CSingleColorPickerItem)			// 禁止复制
	CSingleColorPickerItemPrivate * const d_ptr;		// d指针
};

#endif // C_SIGNLE_COLOR_ITEM_H
