#include <QMovie>
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include "FxLoadIndicator/CLoadIndicator.h"
#include "bdpCommon/GxStrUtil.h"



int CLoadIndicator::useCountIncrease()
{
	return ++m_iUseCount;
}

int CLoadIndicator::useCountDecrease()
{
	return --m_iUseCount;
}

CLoadIndicator *CLoadIndicator::s_pLoadIndicator = nullptr;
QMutex *CLoadIndicator::m_pMutex = new QMutex;

void CLoadIndicator::addUseCount(QWidget *parent, QString text)
{
	if (s_pLoadIndicator == nullptr)
	{
		m_pMutex->lock();
		if (s_pLoadIndicator == nullptr)
		{
			s_pLoadIndicator = new CLoadIndicator(parent, text);
		}
		s_pLoadIndicator->show();
		QApplication::processEvents();

		m_pMutex->unlock();
	}
	s_pLoadIndicator->useCountIncrease();
}

void CLoadIndicator::removeUseCount()
{
	if (s_pLoadIndicator == nullptr)
	{
		return;
	}
	int iUseCount = s_pLoadIndicator->useCountDecrease();
	if (iUseCount <= 0)
	{
		s_pLoadIndicator->deleteLater();
		s_pLoadIndicator = nullptr;
	}
}

CLoadIndicator::CLoadIndicator(QWidget *parent, QString text)
	: QWidget(parent)
	, m_iUseCount(0)
	, m_bMousePressed(false)
{
	Q_ASSERT(parent != nullptr);

#if WIN32
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window );
#else if __linux__
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog );
#endif

	setAttribute(Qt::WA_ShowModal);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_ShowWithoutActivating);

	m_pOverlayWgt = new QWidget(this);
	m_pOverlayWgt->setGeometry(parent->rect());
	m_pOverlayWgt->setStyleSheet("background-color: rgba(0, 0, 0, 170);");

	m_pBgWgt = new QWidget(this);
	m_pBgWgt->setObjectName("load_bg");
	m_pBgWgt->setMinimumSize(200, 90);
	m_pBgWgt->setMaximumSize(200, 90);
	m_pBgWgt->move(parent->rect().center() - m_pBgWgt->rect().center());

	QLabel *pLoadImgLbl = new QLabel(m_pBgWgt);
	pLoadImgLbl->setGeometry(40, 10, 120, 50);

	//修改加载图片 l180
	pLoadImgLbl->setAlignment(Qt::AlignCenter);
	//pLoadImgLbl->setPixmap(QPixmap (":/images_loadindicator/Resources/images/loading.png"));
	QMovie *movie = new QMovie(":/images_loadindicator/Resources/images/load.gif");
	pLoadImgLbl->setMovie(movie);
	movie->start();
	

	m_pTextLbl = new QLabel(m_pBgWgt);
	m_pTextLbl->setAlignment(Qt::AlignCenter);
	m_pTextLbl->setGeometry(0, 60, 200, 20);
	//m_pTextLbl->setText("加载中……");
	m_pTextLbl->setText(text);

	QFile file(":/qss_loadindicator/Resources/qss/loadindicator.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(file.readAll());
	}
	setGeometry(parent->geometry());
}

CLoadIndicator::~CLoadIndicator()
{

}

void CLoadIndicator::setIndicateText(const QString &strIndicate)
{
	m_pTextLbl->setText(strIndicate);
}

void CLoadIndicator::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CLoadIndicator::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	if (event->buttons() | Qt::LeftButton)
	{
		m_bMousePressed = true;
		m_mousePressPt = event->pos();
	}
}

void CLoadIndicator::mouseMoveEvent(QMouseEvent *event)
{
	QWidget::mouseMoveEvent(event);
	if (parentWidget() == nullptr || !parentWidget()->isWindow())
	{
		return;
	}
	QWidget *pTopLevelWnd = parentWidget()->window();
	if (pTopLevelWnd->windowState().testFlag(Qt::WindowMaximized) || pTopLevelWnd->windowState().testFlag(Qt::WindowFullScreen))
	{
		return;
	}

	if ((event->buttons() | Qt::LeftButton) && m_bMousePressed)
	{
		QPoint mouseMovePos = event->globalPos();
		QRect availGeo = QApplication::desktop()->availableGeometry(QApplication::desktop()->screenNumber(mouseMovePos));
		mouseMovePos.setX(qBound(availGeo.x(), mouseMovePos.x(), availGeo.x() + availGeo.width()));
		mouseMovePos.setY(qBound(availGeo.y(), mouseMovePos.y(), availGeo.y() + availGeo.height()));
		pTopLevelWnd->move(mouseMovePos - m_mousePressPt);
		move(mouseMovePos - m_mousePressPt);			
	}
}

void CLoadIndicator::mouseReleaseEvent(QMouseEvent *event)
{
	QWidget::mouseReleaseEvent(event);
	m_bMousePressed = false;
}
