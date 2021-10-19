#ifndef FX_TOAST_PRIVATE_H
#define FX_TOAST_PRIVATE_H

#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>

class CToast;

class CToastPrivate : public QObject
{
	Q_OBJECT
	Q_DECLARE_PUBLIC(CToast)

public:
	CToastPrivate(CToast *q);
	~CToastPrivate();

protected:
	bool eventFilter(QObject *, QEvent *);

private:
	void initToast(const QString &title, const QString &text, int style);

private slots:
	// 关闭按钮单机槽函数
	void onCloseBtnClickedSlot();
	// 动画结束槽函数
	void animationStoppedSlot();

public:
	QLabel *m_pTitleLbl;				// 标题
	QLabel *m_pContentLbl;				// 内容
	QPushButton *m_pCloseBtn;			// 关闭按钮
	QPropertyAnimation *m_pAnimation;	// 动画
	QWidget *m_pParent;					// 父widget
	QWidget *m_pToastWgt;				// 
	CToast *q_ptr;						// q指针
};

#endif
