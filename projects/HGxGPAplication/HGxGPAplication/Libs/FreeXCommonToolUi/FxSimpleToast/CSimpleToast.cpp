#include <QStyleOption>
#include <QPainter>
#include <QFile>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>

#include "CSimpleToastPrivate.h"

#include "FxSimpleToast/CSimpleToast.h"



CSimpleToastPrivate::CSimpleToastPrivate(CSimpleToast *q)
	: q_ptr(q)
{
	q->setAttribute(Qt::WA_TranslucentBackground);
	q->setAttribute(Qt::WA_ShowWithoutActivating);
	q->setAttribute(Qt::WA_DeleteOnClose);
	q->installEventFilter(this);

	q->setMinimumWidth(159);
	q->setMinimumHeight(35);
	q->setMaximumHeight(35);

	QHBoxLayout *pHLayout = new QHBoxLayout(q);
	pHLayout->setContentsMargins(0, 0, 6, 0);
	pHLayout->setSpacing(6);
	m_pContentLbl = new QLabel(q);
	m_pContentLbl->setAlignment(Qt::AlignCenter);
	m_pContentLbl->setObjectName("toast_content");
	QLabel *pHolderLbl = new QLabel;
	pHolderLbl->setMinimumWidth(18);
	pHolderLbl->setMaximumWidth(18);
	pHLayout->addWidget(pHolderLbl);
	pHLayout->addWidget(m_pContentLbl);

	m_pEffect = new QGraphicsOpacityEffect(q);
	q->setGraphicsEffect(m_pEffect);

	QFile file(":/qss_simpletoast/Resources/qss_simpletoast/toast.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		q->setStyleSheet(file.readAll());
	}
}

CSimpleToastPrivate::~CSimpleToastPrivate()
{

}

bool CSimpleToastPrivate::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == q_ptr)
	{
		switch (event->type())
		{
		case QEvent::Paint:
			{
				QStyleOption opt;
				opt.init(q_ptr);
				QPainter painter(q_ptr);
				q_ptr->style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, q_ptr);
			}
			break;
		default:
			break;
		}
	}
	return QObject::eventFilter(obj, event);
}

void CSimpleToastPrivate::animationStoppedSlot()
{
	q_ptr->close();
}

void CSimpleToastPrivate::initToast(const QString &strText, int iStyle)
{
	m_pContentLbl->setText(strText);
	q_ptr->adjustSize();
	if (q_ptr->parentWidget() != nullptr)
	{
		QPoint centerPt = q_ptr->parentWidget()->rect().center() - q_ptr->rect().center();
		q_ptr->move(q_ptr->parentWidget()->mapToGlobal(QPoint(centerPt.x(), q_ptr->parentWidget()->height() * 1.0 / 36)));
	}

	m_pAnimation = new QPropertyAnimation(m_pEffect, "opacity", q_ptr);
	m_pAnimation->setDuration(2800);
	m_pAnimation->setStartValue(0.0);
	m_pAnimation->setKeyValueAt(0.1, 1.0);
	m_pAnimation->setKeyValueAt(0.88, 1.0);
	m_pAnimation->setEndValue(0.0);
	m_pAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	connect(m_pAnimation, SIGNAL(finished()), this, SLOT(animationStoppedSlot()));
}

void CSimpleToast::information(const QString &text, QWidget *parent)
{
	CSimpleToast *pToast = new CSimpleToast(parent);
	pToast->setProperty("style", "information");
	pToast->style()->unpolish(pToast);
	pToast->style()->polish(pToast);
	pToast->d_ptr->initToast(text, 0);
	pToast->show();
}

void CSimpleToast::error(const QString &text, QWidget *parent)
{
	CSimpleToast *pToast = new CSimpleToast(parent);
	pToast->setProperty("style", "error");
	pToast->style()->unpolish(pToast);
	pToast->style()->polish(pToast);
	pToast->d_ptr->initToast(text, 0);
	pToast->show();
}

CSimpleToast::CSimpleToast(QWidget *parent)
    #if WIN32
        : QWidget(parent, Qt::Window | Qt::FramelessWindowHint)
    #else if __linux__
        : QWidget(parent, Qt::Window | Qt::FramelessWindowHint)
    #endif
	, d_ptr(new CSimpleToastPrivate(this))
{
	
}

CSimpleToast::~CSimpleToast()
{

}
