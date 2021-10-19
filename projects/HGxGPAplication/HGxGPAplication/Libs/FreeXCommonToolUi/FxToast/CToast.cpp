#include "FxToast/CToast.h"

#include "CToastPrivate.h"

#include <QStyleOption>
#include <QPainter>
#include <QFile>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>



#define TOAST_WIDTH 220
#define CONTENT_WIDTH 180
#define TOAST_MINI_HEIGHT 100

CToast* CToast::m_pInstance = NULL;

CToastPrivate::CToastPrivate(CToast *q)
	: q_ptr(q)
{
#if WIN32
    q->setWindowFlags(Qt::FramelessWindowHint | Qt::Window );
#else if __linux__
    q->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog );
#endif
	q->setAttribute(Qt::WA_TranslucentBackground);
	q->setAttribute(Qt::WA_ShowWithoutActivating);
	q->setAttribute(Qt::WA_DeleteOnClose);
	q->installEventFilter(this);

	QFile file(":/qss_toast/Resources/qss_toast/toast.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		q->setStyleSheet(file.readAll());
	}
	
	m_pToastWgt = new QWidget(q);
	m_pToastWgt->setObjectName("fx_toast_bg");
	m_pToastWgt->setMaximumWidth(TOAST_WIDTH);
	m_pToastWgt->setMinimumWidth(TOAST_WIDTH);
	m_pToastWgt->setMinimumHeight(TOAST_MINI_HEIGHT);

	m_pTitleLbl = new QLabel;
	m_pTitleLbl->setObjectName("fx_toast_title");
	m_pTitleLbl->setMinimumHeight(20);
	m_pCloseBtn = new QPushButton;
	m_pCloseBtn->setObjectName("fx_toast_close");
	m_pCloseBtn->setMaximumSize(9, 9);
	m_pCloseBtn->hide();
	connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(onCloseBtnClickedSlot()));

	QHBoxLayout *pHLayout = new QHBoxLayout;
	pHLayout->setContentsMargins(8, 4, 8, 4);
	pHLayout->setSpacing(0);
	pHLayout->addWidget(m_pTitleLbl);
	pHLayout->addStretch();
	pHLayout->addWidget(m_pCloseBtn);

	m_pContentLbl = new QLabel;
	m_pContentLbl->setMaximumWidth(CONTENT_WIDTH);
	m_pContentLbl->setMinimumWidth(CONTENT_WIDTH);
	m_pContentLbl->setWordWrap(true);
	m_pContentLbl->setObjectName("fx_toast_content");
	QHBoxLayout *pHLayout1 = new QHBoxLayout;
	pHLayout1->addStretch();
	pHLayout1->addWidget(m_pContentLbl);
	pHLayout1->addStretch();

	QVBoxLayout *pVLayout = new QVBoxLayout(m_pToastWgt);
	pVLayout->setContentsMargins(10, 7, 9, 20);
	pVLayout->setSpacing(0);
	pVLayout->addLayout(pHLayout);
	pVLayout->addLayout(pHLayout1);
}

CToastPrivate::~CToastPrivate()
{

}

bool CToastPrivate::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == q_ptr)
	{
		switch (event->type())
		{
		case QEvent::Enter:
		case QEvent::HoverEnter:
			{
				m_pAnimation->pause();
				m_pCloseBtn->show();
			}
			break;
		case QEvent::Leave:
			{
				m_pAnimation->start();
				m_pCloseBtn->hide();
			}
			break;
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

void CToastPrivate::onCloseBtnClickedSlot()
{
	q_ptr->hide();
}

void CToastPrivate::animationStoppedSlot()
{
	q_ptr->hide();
}

void CToastPrivate::initToast(const QString &strTitle, const QString &strText, int iStyle)
{
	if (m_pParent == nullptr)
	{
		q_ptr->hide();
		return;
	}

	m_pTitleLbl->setText(strTitle);
	m_pContentLbl->setText(strText);
	m_pToastWgt->adjustSize();
	QPoint toastPos = m_pParent->mapToGlobal(QPoint(m_pParent->width(), m_pParent->height())) - QPoint(m_pToastWgt->width(), m_pToastWgt->height());
	q_ptr->setGeometry(toastPos.x(), toastPos.y(), m_pToastWgt->width(), m_pToastWgt->height());

	m_pAnimation = new QPropertyAnimation(m_pToastWgt, "pos", m_pParent);
	m_pAnimation->setDuration(5000);
	QPoint sidePos = QPoint(0, m_pToastWgt->height());
	QPoint midPos = QPoint(0, 9);
	m_pAnimation->setStartValue(sidePos);
	m_pAnimation->setKeyValueAt(0.1, midPos);
	m_pAnimation->setKeyValueAt(0.85, midPos);
	m_pAnimation->setEndValue(sidePos);
	m_pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	connect(m_pAnimation, SIGNAL(finished()), this, SLOT(animationStoppedSlot()));
}

void CToast::information(const QString &title, const QString &text, QWidget *parent)
{
	if(!m_pInstance)
		{
			m_pInstance = new CToast(parent);
		}
	m_pInstance->d_ptr->initToast(title, text, 0); 
	m_pInstance->show();
}

CToast::CToast(QWidget *parent)
	: QWidget(parent)
	, d_ptr(new CToastPrivate(this))
{
#if WIN32
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window );
#else if __linux__
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog );
#endif
	d_ptr->m_pParent = parent;
}

CToast::~CToast()
{

}
