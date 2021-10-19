#ifndef FX_COLOR_PICKER_WIDGET_PRIVATE_H
#define FX_COLOR_PICKER_WIDGET_PRIVATE_H

#include <QObject>
#include <QColor>

#include "FxColorDialog/FxColorPickerWidget.h"

class CHandPushButton;
class CBasicColorArea;
class CCustomColorArea;
class CSVColorArea;
class CHColorArea;
class CPreviewColorArea;
class QSpinBox;
class QEventLoop;

class FxColorPickerWidgetPrivate : public QObject
{
	Q_OBJECT
	Q_DECLARE_PUBLIC(FxColorPickerWidget)

public:
	explicit FxColorPickerWidgetPrivate(FxColorPickerWidget *parent);
	~FxColorPickerWidgetPrivate();

private slots:
	// 色调值手动改变槽函数
	void HueChangedManuallySlot(int hue);
	// 饱和度、亮度手动改变槽函数
	void SvChangedManuallySlot(int hue, int saturation, int brightness);
	// 基本颜色项/自定义颜色项单击槽函数
	void ColorItemSelectedSlot(const QColor &color);
	// 添加到自定义颜色按钮点击槽函数
	void OnAddToCustomColorBtnClickedSlot();
	// 确定按钮点击槽函数
	void OnComfirmBtnClickedSlot();
	// 取消按钮点击槽函数
	void OnCancelBtnClickedSlot();

	void RValueChangedSlot(int r);	// r 红
	void GValueChangedSlot(int g);	// g 绿
	void BValueChangedSlot(int b);	// b 蓝

public:
	FxColorPickerWidget *q_ptr;		// q指针
	CBasicColorArea *m_pBasicColorArea;
	CCustomColorArea *m_pCustomColorArea;
	CSVColorArea *m_pSvColorArea;
	CHColorArea *m_pHColorArea;
	CPreviewColorArea *m_pPreviewColorArea;
	QSpinBox *m_pRedSpx;
	QSpinBox *m_pGreenSpx;
	QSpinBox *m_pBlueSpx;
	CHandPushButton *m_pAddToCustomBtn;
	CHandPushButton *m_pComfirmBtn;
	CHandPushButton *m_pCancelBtn;
	QEventLoop *m_pEvtLoop;
	int m_iHue;						// 色调
	int m_iSaturation;				// 饱和度
	int m_iBrightness;				// 亮度
	QColor m_color;					// 颜色
	bool m_bIsEdit;

	FxColorPickerWidget::ButtonRole m_buttonRole;		// 
};

#endif // !FX_COLOR_PICKER_WIDGET_PRIVATE_H