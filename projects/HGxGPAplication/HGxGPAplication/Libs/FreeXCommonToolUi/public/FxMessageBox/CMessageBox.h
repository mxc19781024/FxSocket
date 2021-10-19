#ifndef FX_MESSAGEBOX_H
#define FX_MESSAGEBOX_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "bdpCommon/GxStrUtil.h"

#include "freexcommontoolui_export.h"

class QEventLoop;
class QVBoxLayout;

/**
* @class	: CMessageBox
* @brief	: 消息对话框
* @note		: 
* @author	: k00026
**/
#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif
class FREEXCOMMONTOOLUI_EXPORT CMessageBox : public QWidget
{
	Q_OBJECT

public:
	enum ReturnButton
	{
		Confirm,	// 确定
		Cancel,		// 取消
		Save,		// 保存
		UnSave,		// 未保存
		Got		    // 知道了
	};

	static ReturnButton warning(const QString &content, QWidget *parent = nullptr);

	static ReturnButton question(const QString &content, QWidget *parent = nullptr);

	static ReturnButton information(const QString &content, QWidget *parent = nullptr);

	void setMessageContent(const QString &content);
	void setDefaultPushBtn(ReturnButton button);
public:
	CMessageBox(QWidget *parent = 0);
	~CMessageBox();

protected:
	// 鼠标事件
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *)override;
	void keyPressEvent(QKeyEvent *)override;
	//为了处理Alt+F4重写该方法，内部处理只是取消当前的MessageBox（w00224 2019-10-8）
	void closeEvent(QCloseEvent *)override;

private:
	

private slots:
	// 确定按钮单击槽函数
	void onConfirmBtnClickedSlot();
	// 取消按钮单击槽函数
	void onCancelBtnClickedSlot();
	// 保存按钮单击槽函数
	void onSaveBtnClickedSlot();
	// 未保存按钮单击槽函数
	void onUnSaveBtnClickedSlot();
	// 知道了按钮单击槽函数
	void onGotBtnClickedSlot();

public:
	QVBoxLayout	*m_pVLayoutForConfirm;	// 确认/取消按钮组合
	QEventLoop	*m_pEvtLoop;			// 事件循环
	QWidget *m_pCenterWgt;				// 内容
	static ReturnButton m_button;		// 返回值

private:
	ReturnButton		m_defaultBtn;	// 默认值
	QLabel *m_pContentLbl;				// 内容Label
	bool m_bMousePressed;				// 标记鼠标是否按下
	QPoint m_mousePressPt;				// 标记鼠标按下的位置
	QVBoxLayout	*m_pVLayoutForSave;		// 保存/不保存/取消按钮组合
	QVBoxLayout	*m_pVLayoutForInfo;		// 知道了按钮
	QWidget *m_pOverlayWgt;				// 背景
	
};


class FREEXCOMMONTOOLUI_EXPORT CCheckMessgaeBox: public QWidget
{	
	Q_OBJECT
public:
	enum ReturnButton
	{
		Got,		// 知道了
		UnHint		// 不再提示
	};

public:
	static ReturnButton checkInformation(const QString &content, QWidget *parent = nullptr);
	
	void setDefaultPushBtn(ReturnButton button);

public:
	CCheckMessgaeBox(QWidget *parent = 0);
	~CCheckMessgaeBox();

private slots:
	// 不再提示按钮单击槽函数 w00224 2019-12-26
	void onUnHintBtnClickedSlot();
	// 知道了按钮单击槽函数
	void onGotBtnClickedSlot();

protected:
	void keyPressEvent(QKeyEvent *)override;

public:
	static ReturnButton m_button;		// 返回值

private:
	QVBoxLayout	*m_pVLayoutForUnhint;	// 不再提示/确定按钮组合
	QEventLoop	*m_pEvtLoop;			// 事件循环
	QLabel *m_pContentLbl;				// 内容Label
	ReturnButton	m_defaultBtn;		// 默认值

	bool m_bMousePressed;				// 标记鼠标是否按下
	QPoint m_mousePressPt;				// 标记鼠标按下的位置
};

#endif // FX_MESSAGEBOX_H
