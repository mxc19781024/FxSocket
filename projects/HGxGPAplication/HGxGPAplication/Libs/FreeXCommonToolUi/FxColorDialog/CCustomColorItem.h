#pragma once

#include <QWidget>
#include "CSingleColorItem.h"

class CCustomColorItem : public CSingleColorItem
{
	Q_OBJECT

public:
	CCustomColorItem(const QColor &, QWidget *parent = nullptr);
	~CCustomColorItem();
	// 设置颜色
	void SetColor(const QColor &);

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
};
