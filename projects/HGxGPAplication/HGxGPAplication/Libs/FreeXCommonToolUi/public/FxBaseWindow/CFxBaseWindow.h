#ifndef FX_BASE_WINDOW_H
#define FX_BASE_WINDOW_H

/**
* @class	: FxWidgetSystem
* @brief	: 窗口基类
* @note		: 
* @author	: k00026
**/

#include <QWidget>
#include <QPropertyAnimation>
//#include <FreeXSysConfig.h>
#include "freexcommontoolui_export.h"

class QLabel;
class QPushButton;

#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif

class FREEXCOMMONTOOLUI_EXPORT CFxBaseWindow : public QWidget
{
	Q_OBJECT

public:
	CFxBaseWindow(QWidget *parent = nullptr);
	~CFxBaseWindow();
	void setResizable(bool);

	enum MouseStyle { NORMAL, LEFT, TOP, RIGHT, BOTTOM, TOPLEFT, TOPRIGHT, BOTTOMRIGHT, BOTTOMLEFT };

public:
	// 设置标题
	void setWindowTitle(const QString &title);

signals:
	// 点击其他窗口,闪烁提示
	void ncActiveSignal(bool bActive);

protected:
	// 事件过滤器
	bool eventFilter(QObject *watched, QEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

	/**  
	  * @note 是否窗口闪烁开关
	*/
	void setBlink(bool bBlink);

#ifdef WIN32
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#else
	bool winEvent(MSG *msg, long *result);
#endif
#endif


private:
	// 改变鼠标样式
	void changeMouseStyle(const QPoint &mousePos);

private slots:
	// 关闭窗口槽函数
	void onCloseWindowSlot();
	void ncActiveSlot( bool bActive );

protected:
	QWidget *m_pCentralWgt;			// 中心内容widget
	QPushButton *m_pCloseBtn;		// 关闭按钮

private:
	QWidget *m_pHeaderWgt;			// 顶部widget
	QLabel *m_pTitleLbl;			// 标题Label
	bool m_bResizable;				// 标记是否可缩放
	bool m_bMousePressed;			// 标记鼠标是否按下
	int m_iDragMargin;				// 拖拽缩放时的边距
	QPoint m_mousePos;				// 标记鼠标按下的坐标
	MouseStyle m_mouseStyle;		// 鼠标样式
	int m_iDragXPadding;			// 拖动时记录x边距
	int m_iDragYPadding;			// 拖动时记录y边距
	bool m_bBlink;						//是否窗口闪烁

    QPoint m_ptBottomRight;            //记录鼠标点击按下即将移动或者缩放窗口时有下角位置
};

#endif // !FX_BASE_WINDOW_H
