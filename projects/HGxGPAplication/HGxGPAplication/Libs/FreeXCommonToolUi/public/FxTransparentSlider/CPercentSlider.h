#ifndef CPERCENTSLIDER_H
#define CPERCENTSLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QLabel>
//#include <FreeXSysConfig.h>

#include "freexcommontoolui_export.h"

class CHandleLabel;
#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif
class FREEXCOMMONTOOLUI_EXPORT CPercentSlider : public QSlider
{
	Q_OBJECT

public:
	CPercentSlider(QWidget *parent = nullptr);
	~CPercentSlider();

	// 设置Slider的值
	void setPercentSliderValue(int iValue);

private slots:
	void editingFinishedSlot();
	void valueChangedSlot(int value);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
private:
	CHandleLabel* m_displayLabel;

};

#endif // CPERCENTSLIDER_H
