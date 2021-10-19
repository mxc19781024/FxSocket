/**************************************************************************************************
* @file		: CSVColorArea.h
* @note		: 饱和度、亮度区域
* @author	: kzm
* @data		: 2018-8
**************************************************************************************************/


#ifndef C_SVCOLOR_AREA_H
#define C_SVCOLOR_AREA_H

#include <QWidget>
//#include <FreeXSysConfig.h>

/**
* @class	: CSVColorArea
* @brief	: 饱和度、亮度控制区域
* @note		: 饱和度、亮度控制区域
* @author	: kzm
**/
class CSVColorArea : public QWidget
{
	Q_OBJECT

public:
	explicit CSVColorArea(QWidget *parent = nullptr);
	~CSVColorArea();
	// 设置色调值
	void SetHue(int hue);
	// 设置饱和度值
	void SetSaturation(int saturation);
	// 设置亮度值
	void SetBrightness(int);
	// 设置色调、饱和度、亮度值
	void SetHsv(int, int, int);
	// 设置颜色
	void setColor(const QColor &);

protected:
	void paintEvent(QPaintEvent *);			//paint事件
	void mousePressEvent(QMouseEvent *);	//鼠标按下事件
	void mouseMoveEvent(QMouseEvent *);		//鼠标移动事件

public slots:
	void hueChangedSlot(int);

signals:
	// 手动改变饱和度和亮度信号
	void SvManualChangedSignal(int, int, int);

private:
	int m_iHue;				// 色调值
	int m_iSaturation;
	int m_iBrightness;
	int m_iBorderMargin;
};

#endif // C_SVCOLOR_AREA_H
