#ifndef FX_SIMPLE_TOAST_PRIVATE_H
#define FX_SIMPLE_TOAST_PRIVATE_H

#include <QObject>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class CSimpleToast;

class CSimpleToastPrivate : public QObject
{
	Q_OBJECT
	Q_DECLARE_PUBLIC(CSimpleToast)

public:
	CSimpleToastPrivate(CSimpleToast *q);
	~CSimpleToastPrivate();

protected:
	bool eventFilter(QObject *, QEvent *);

private:
	void initToast(const QString &text, int style);

private slots:
	// 动画结束槽函数
	void animationStoppedSlot();

public:
	QLabel *m_pContentLbl;				// 内容
	QGraphicsOpacityEffect *m_pEffect;	// 
	QPropertyAnimation *m_pAnimation;	// 动画
	CSimpleToast *q_ptr;				// q指针
};

#endif // !FX_SIMPLE_TOAST_PRIVATE_H
