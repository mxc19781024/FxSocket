#include "FxMessageBox/CMessageBox.h"

#include <QEventLoop>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>


CMessageBox::ReturnButton CMessageBox::m_button = CMessageBox::Cancel;

CMessageBox::ReturnButton CMessageBox::warning(const QString &strContent, QWidget *parent)
{
	CMessageBox msgBox(parent);
	msgBox.setMessageContent(strContent);

	QVBoxLayout *pLayout = qobject_cast<QVBoxLayout *>(msgBox.layout());
	if (pLayout)
	{
		pLayout->addLayout(msgBox.m_pVLayoutForSave);
	}

	msgBox.m_pCenterWgt->setLayout(pLayout);
	msgBox.setDefaultPushBtn(CMessageBox::Save);
	msgBox.show();

	if (msgBox.m_pEvtLoop == nullptr)
	{
		msgBox.m_pEvtLoop = new QEventLoop(&msgBox);
	}
	msgBox.m_pEvtLoop->exec();

	return m_button;
}

CMessageBox::ReturnButton CMessageBox::question(const QString &strContent, QWidget *parent /*= nullptr*/)
{
	CMessageBox msgBox(parent);
	msgBox.setMessageContent(strContent);

	QVBoxLayout *pLayout = qobject_cast<QVBoxLayout *>(msgBox.layout());
	if (pLayout)
	{
		pLayout->addLayout(msgBox.m_pVLayoutForConfirm);
	}

	msgBox.m_pCenterWgt->setLayout(pLayout);
	msgBox.setDefaultPushBtn(CMessageBox::Confirm);
	msgBox.show();

	if (msgBox.m_pEvtLoop == nullptr)
	{
		msgBox.m_pEvtLoop = new QEventLoop(&msgBox);
	}
	msgBox.m_pEvtLoop->exec();

	return m_button;
}

CMessageBox::ReturnButton CMessageBox::information(const QString &strContent, QWidget *parent /*= nullptr*/)
{
	CMessageBox msgBox(parent);
	msgBox.setMessageContent(strContent);

	QVBoxLayout *pLayout = qobject_cast<QVBoxLayout *>(msgBox.layout());
	if (pLayout)
	{

		pLayout->addLayout(msgBox.m_pVLayoutForInfo);
	}

	msgBox.m_pCenterWgt->setLayout(pLayout);
	msgBox.setDefaultPushBtn(CMessageBox::Got);

	msgBox.show();


	if (msgBox.m_pEvtLoop == nullptr)
	{
		msgBox.m_pEvtLoop = new QEventLoop(&msgBox);
	}
	msgBox.m_pEvtLoop->exec();

	return m_button;
}

CMessageBox::CMessageBox(QWidget *parent)
#if WIN32
    : QWidget(parent, Qt::Window | Qt::FramelessWindowHint)
#else if __linux__
    : QWidget(parent, Qt::Window | Qt::FramelessWindowHint)
#endif
	, m_pEvtLoop(nullptr)
	, m_bMousePressed(false)
	, m_pVLayoutForConfirm(nullptr)
	, m_pVLayoutForSave(nullptr)
	, m_defaultBtn(CMessageBox::Cancel)
{
	if (parent == nullptr)
	{
		parent = QApplication::desktop();
	}
	setAttribute(Qt::WA_ShowModal);
	setAttribute(Qt::WA_TranslucentBackground);

	m_pOverlayWgt = new QWidget(this);
	m_pOverlayWgt->setObjectName("overlayWgt");
	m_pOverlayWgt->setGeometry(parent->rect());

	m_pCenterWgt = new QWidget(this);
	m_pCenterWgt->setObjectName("center_wgt");
	m_pCenterWgt->setMinimumWidth(230);
	m_pCenterWgt->setMaximumWidth(300);

	m_pCenterWgt->setMinimumHeight(151);
	m_pCenterWgt->move(parent->rect().center() - m_pCenterWgt->rect().center());

	QHBoxLayout *pHLayout = new QHBoxLayout;
	QLabel *pIconLbl = new QLabel;
	pIconLbl->setMinimumSize(30, 32);
	pIconLbl->setMaximumSize(30, 32);
	pIconLbl->setObjectName("fx_msgbox_icon");
	pHLayout->addStretch();
	pHLayout->addWidget(pIconLbl);
	pHLayout->addStretch();

	QHBoxLayout *pHLayout3 = new QHBoxLayout;
	pHLayout3->setContentsMargins(0, 12, 0, 0);
	m_pContentLbl = new QLabel;
	m_pContentLbl->setWordWrap(true);
	m_pContentLbl->setObjectName("fx_msgbox_content");
	m_pContentLbl->setMaximumWidth(140);
	m_pContentLbl->setMinimumWidth(140);
	m_pContentLbl->setAlignment(Qt::AlignHCenter);
	pHLayout3->addStretch();
	pHLayout3->addWidget(m_pContentLbl);
	pHLayout3->addStretch();

	QPushButton *pConfirmBtn = new QPushButton();
	pConfirmBtn->setMinimumSize(90, 24);
	pConfirmBtn->setObjectName("confirmBtn");
	pConfirmBtn->setText(tr("confirm"));
	pConfirmBtn->setStyleSheet("color: #3b7cff");
	QPushButton *pCancelBtn = new QPushButton();
	pCancelBtn->setMinimumSize(90, 24);
	pCancelBtn->setText(tr("cancel"));

	QPushButton *pOKBtn = new QPushButton();
	pOKBtn->setMinimumSize(90, 24);
	pOKBtn->setObjectName("confirmBtn");
	pOKBtn->setText(tr("Yes"));
	pOKBtn->setStyleSheet("color: #3b7cff");
	QPushButton *pNoBtn = new QPushButton();
	pNoBtn->setMinimumSize(90, 24);
	pNoBtn->setText(tr("No"));

	QPushButton *pSaveBtn = new QPushButton;
	pSaveBtn->setMinimumSize(90, 24);
	pSaveBtn->setObjectName("saveBtn");
	pSaveBtn->setText(tr("save"));
	pSaveBtn->setStyleSheet("color: #3b7cff");
	QPushButton *pUnSaveBtn = new QPushButton;
	pUnSaveBtn->setMinimumSize(90, 24);
	pUnSaveBtn->setText(tr("not save"));

	QPushButton *pGotBtn = new QPushButton;
	pGotBtn->setMinimumSize(90, 24);
	pGotBtn->setObjectName("gotBtn");
	pGotBtn->setText(tr("I know"));
	pGotBtn->setStyleSheet("color: #3b7cff");
	
	QFrame *pVLine = new QFrame;
	pVLine->setFrameShape(QFrame::VLine);
	pVLine->setFrameShadow(QFrame::Sunken);

	QFrame *pVLine2 = new QFrame;
	pVLine2->setFrameShape(QFrame::VLine);
	pVLine2->setFrameShadow(QFrame::Sunken);

	QHBoxLayout *pHLayout1 = new QHBoxLayout;
	pHLayout1->setContentsMargins(0, 6, 0, 0);
	//pHLayout1->addWidget(pConfirmBtn);
	pHLayout1->addWidget(pOKBtn);
	pHLayout1->addWidget(pVLine);
	pHLayout1->addWidget(pNoBtn);
	//pHLayout1->addWidget(pCancelBtn);

	QFrame *pHLine = new QFrame;
	pHLine->setFrameShape(QFrame::HLine);
	pHLine->setFrameShadow(QFrame::Sunken);

	m_pVLayoutForConfirm = new QVBoxLayout;
	m_pVLayoutForConfirm->setContentsMargins(15, 20, 15, 7);
	m_pVLayoutForConfirm->setSpacing(0);
	m_pVLayoutForConfirm->addWidget(pHLine);
	m_pVLayoutForConfirm->addLayout(pHLayout1);

	QHBoxLayout *pHLayout2 = new QHBoxLayout;
	pHLayout2->setContentsMargins(0, 6, 0, 0);
	pHLayout2->addWidget(pSaveBtn);
	pHLayout2->addWidget(pVLine);
	pHLayout2->addWidget(pUnSaveBtn);
	pHLayout2->addWidget(pVLine2);
	pHLayout2->addWidget(pCancelBtn);

	QHBoxLayout *pHLayout4 = new QHBoxLayout;
	pHLayout4->setContentsMargins(0, 6, 0, 0);
	pHLayout4->addWidget(pGotBtn);

	m_pVLayoutForSave = new QVBoxLayout;
	m_pVLayoutForSave->setContentsMargins(15, 20, 15, 7);
	m_pVLayoutForSave->setSpacing(0);
	m_pVLayoutForSave->addWidget(pHLine);
	m_pVLayoutForSave->addLayout(pHLayout2);

	m_pVLayoutForInfo = new QVBoxLayout;
	m_pVLayoutForInfo->setContentsMargins(15, 20, 15, 7);
	m_pVLayoutForInfo->setSpacing(0);
	m_pVLayoutForInfo->addWidget(pHLine);
	m_pVLayoutForInfo->addLayout(pHLayout4);

	QVBoxLayout *pVLayout = new QVBoxLayout(this);
	pVLayout->setContentsMargins(0, 19, 0, 13);
	pVLayout->setSpacing(0);

	pVLayout->addLayout(pHLayout);
	pVLayout->addLayout(pHLayout3);

	connect(pConfirmBtn, SIGNAL(clicked()), this, SLOT(onConfirmBtnClickedSlot()));
	connect(pOKBtn, SIGNAL(clicked()), this, SLOT(onConfirmBtnClickedSlot()));
	connect(pCancelBtn, SIGNAL(clicked()), this, SLOT(onCancelBtnClickedSlot()));
	connect(pNoBtn, SIGNAL(clicked()), this, SLOT(onCancelBtnClickedSlot()));
	connect(pSaveBtn, SIGNAL(clicked()), this, SLOT(onSaveBtnClickedSlot()));
	connect(pUnSaveBtn, SIGNAL(clicked()), this, SLOT(onUnSaveBtnClickedSlot()));
	connect(pGotBtn, SIGNAL(clicked()), this, SLOT(onGotBtnClickedSlot()));

	QFile file(":/qss_messagebox/Resources/qss_messagebox/messagebox.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(file.readAll());
	}

	//setGeometry(parent->geometry());
	setGeometry(QRect(parent->mapToGlobal(QPoint(0, 0)), parent->size()));
}

CMessageBox::~CMessageBox()
{

}

void CMessageBox::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	if (event->buttons() | Qt::LeftButton)
	{
		m_bMousePressed = true;
		m_mousePressPt = event->pos();
	}
}

void CMessageBox::mouseMoveEvent(QMouseEvent *event)
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

void CMessageBox::mouseReleaseEvent(QMouseEvent *event)
{
	QWidget::mouseReleaseEvent(event);
	m_bMousePressed = false;
}

void CMessageBox::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CMessageBox::keyPressEvent( QKeyEvent *event )
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Space)
	{
		switch(m_defaultBtn)
		{
		case CMessageBox::Confirm:
			{
				onConfirmBtnClickedSlot();
			}
			break;
		case CMessageBox::Save:
			{
				onSaveBtnClickedSlot();
			}
			break;
		case CMessageBox::Got:
			{
				onGotBtnClickedSlot();
			}
			break;
		default:
			break;
		}
	}
	else if(event->key() == Qt::Key_Escape)
	{
		if (m_defaultBtn == CMessageBox::Got)
		{
			onGotBtnClickedSlot();
		}
		else if (m_defaultBtn == CMessageBox::Confirm || m_defaultBtn == CMessageBox::Save)
		{
			onCancelBtnClickedSlot();
		}
	}

	QWidget::keyPressEvent(event);
}

void CMessageBox::setMessageContent(const QString &strContent)
{
	m_pContentLbl->setText(strContent);
}

void CMessageBox::setDefaultPushBtn( ReturnButton button )
{
	m_defaultBtn = button; 
}

void CMessageBox::onConfirmBtnClickedSlot()
{
	m_button = CMessageBox::Confirm;
	if (m_pEvtLoop != nullptr)
	{
		m_pEvtLoop->exit();
	}
	this->hide();
}

void CMessageBox::closeEvent(QCloseEvent *event)
{
	onCancelBtnClickedSlot();
}

void CMessageBox::onCancelBtnClickedSlot()
{
	m_button = CMessageBox::Cancel;
	if (m_pEvtLoop != nullptr)
	{
		m_pEvtLoop->exit();
	}
	this->hide();
}

void CMessageBox::onSaveBtnClickedSlot()
{
	m_button = CMessageBox::Save;
	if (m_pEvtLoop != nullptr)
	{
		m_pEvtLoop->exit();
	}
	this->hide();
}

void CMessageBox::onUnSaveBtnClickedSlot()
{
	m_button = CMessageBox::UnSave;
	if (m_pEvtLoop != nullptr)
	{
		m_pEvtLoop->exit();
	}
	this->hide();
}

void CMessageBox::onGotBtnClickedSlot()
{
	m_button = CMessageBox::Got;
	if (m_pEvtLoop != nullptr)
	{
		m_pEvtLoop->exit();
	}
	this->hide();
}

CCheckMessgaeBox::ReturnButton CCheckMessgaeBox::m_button = CCheckMessgaeBox::Got;

CCheckMessgaeBox::CCheckMessgaeBox(QWidget *parent /*= 0*/)
#if WIN32
    : QWidget(parent, Qt::Window | Qt::FramelessWindowHint)
#else if __linux__
    : QWidget(parent, Qt::Window | Qt::FramelessWindowHint)
#endif
	, m_pEvtLoop(nullptr)
	, m_pVLayoutForUnhint(nullptr)
	, m_defaultBtn(CCheckMessgaeBox::Got)
{
	if (parent == nullptr)
	{
		parent = QApplication::desktop();
	}
	setAttribute(Qt::WA_ShowModal);
	setAttribute(Qt::WA_TranslucentBackground);

	QWidget* pOverlayWgt = new QWidget(this);
	pOverlayWgt->setObjectName("overlayWgt");
	pOverlayWgt->setGeometry(parent->rect());

	QWidget* pCenterWgt = new QWidget(this);
	pCenterWgt->setObjectName("center_wgt");
	pCenterWgt->setFixedSize(417,176);
	pCenterWgt->move(parent->rect().center() - pCenterWgt->rect().center());
	QLabel *pIconLbl = new QLabel;
	pIconLbl->setObjectName("fx_msgbox_icon");
	pIconLbl->setFixedSize(QSize(97,86));
	QVBoxLayout* pIconLayout = new QVBoxLayout;
	pIconLayout->setContentsMargins(0, 0, 0, 0);
	pIconLayout->addWidget(pIconLbl);

	m_pContentLbl = new QLabel;
	m_pContentLbl->setWordWrap(true);
	m_pContentLbl->setObjectName("fx_msgbox_content");
	QVBoxLayout *pVLayoutText = new QVBoxLayout;
	pVLayoutText->setContentsMargins(0, 10, 0, 0);
	pVLayoutText->addWidget(m_pContentLbl);

	QHBoxLayout *pHLayout3 = new QHBoxLayout;
	pHLayout3->setSpacing(20);
	pHLayout3->setContentsMargins(30, 12, 40, 10);
	pHLayout3->addLayout(pIconLayout);
	pHLayout3->addLayout(pVLayoutText);

	QPushButton *pGotBtn = new QPushButton;
	pGotBtn->setMinimumSize(90, 24);
	pGotBtn->setObjectName("gotBtn");
	pGotBtn->setText(tr("I know"));
	pGotBtn->setStyleSheet("color: #3b7cff");

	QPushButton *pUnHintBtn = new QPushButton;
	pUnHintBtn->setMinimumSize(90, 24);
	pUnHintBtn->setObjectName("unHint");
	pUnHintBtn->setText(tr("no longer prompted"));

	QFrame *pVLine = new QFrame;
	pVLine->setFrameShape(QFrame::VLine);
	pVLine->setFrameShadow(QFrame::Sunken);
	
	QHBoxLayout *pHLayout1 = new QHBoxLayout;
	pHLayout1->setContentsMargins(0, 6, 0, 0);
	pHLayout1->addWidget(pUnHintBtn);
	pHLayout1->addWidget(pVLine);
	pHLayout1->addWidget(pGotBtn);

	m_pVLayoutForUnhint = new QVBoxLayout;
	m_pVLayoutForUnhint->setContentsMargins(15, 20, 15, 7);
	m_pVLayoutForUnhint->setSpacing(0);
	m_pVLayoutForUnhint->addLayout(pHLayout1);

	QVBoxLayout *pVLayout = new QVBoxLayout();
	pVLayout->setContentsMargins(0, 19, 0, 13);
	pVLayout->setSpacing(0);
	
	pVLayout->addLayout(pHLayout3);
	pVLayout->addLayout(m_pVLayoutForUnhint);

	pCenterWgt->setLayout(pVLayout);

	connect(pGotBtn, SIGNAL(clicked()), this, SLOT(onGotBtnClickedSlot()));
	connect(pUnHintBtn, SIGNAL(clicked()), this, SLOT(onUnHintBtnClickedSlot()));


	QFile file(":/qss_messagebox/Resources/qss_messagebox/checkmessagebox.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(file.readAll());
	}
	setGeometry(QRect(parent->mapToGlobal(QPoint(0, 0)), parent->size()));
}

CCheckMessgaeBox::ReturnButton CCheckMessgaeBox::checkInformation(const QString &strContent, QWidget *parent /*= nullptr*/)
{
	CCheckMessgaeBox msgBox(parent);
	msgBox.m_pContentLbl->setText(strContent + "<a href=\"http://www.freexgis.com/docs/desktop/manual\">帮助文档</a>");
	msgBox.m_pContentLbl->setOpenExternalLinks(true);

	msgBox.setDefaultPushBtn(CCheckMessgaeBox::Got);
	msgBox.show();

	if (msgBox.m_pEvtLoop == nullptr)
	{
		msgBox.m_pEvtLoop = new QEventLoop(&msgBox);
	}
	msgBox.m_pEvtLoop->exec();

	return m_button;
}

void CCheckMessgaeBox::onUnHintBtnClickedSlot()
{
	m_button = CCheckMessgaeBox::UnHint;
	if (m_pEvtLoop != nullptr)
	{
		m_pEvtLoop->exit();
	}
	this->hide();
}

void CCheckMessgaeBox::onGotBtnClickedSlot()
{
	m_button = CCheckMessgaeBox::Got;
	if (m_pEvtLoop != nullptr)
	{
		m_pEvtLoop->exit();
	}
	this->hide();
}

void CCheckMessgaeBox::setDefaultPushBtn(ReturnButton button)
{
	m_defaultBtn = button;
}

void CCheckMessgaeBox::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Space)
	{
		switch(m_defaultBtn)
		{
		case CMessageBox::Got:
			{
				onGotBtnClickedSlot();
			}
			break;
		default:
			break;
		}
	}
	else if(event->key() == Qt::Key_Escape)
	{
		if (m_defaultBtn == CMessageBox::Got)
		{
			onGotBtnClickedSlot();
		}
	}

	QWidget::keyPressEvent(event);
}

CCheckMessgaeBox::~CCheckMessgaeBox()
{

}
