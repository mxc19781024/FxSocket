#include "CLayerTypeSelectWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QEventLoop>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>

#include <service/event/ctkEventConstants.h>
#include <service/event/ctkEventAdmin.h>


CLayerTypeSelectWidget::ReturnButton CLayerTypeSelectWidget::m_button = CLayerTypeSelectWidget::No;

CLayerConfig::LayerType CLayerTypeSelectWidget::m_layerType = CLayerConfig::ImageLayer;

CLayerTypeSelectWidget::ReturnButton CLayerTypeSelectWidget::warning(QWidget *parent)
{
	CLayerTypeSelectWidget layerSelcWgt(parent);
	//layerSelcWgt.adjustSize();
	layerSelcWgt.show();
	if (parent != nullptr)
	{
		layerSelcWgt.move(parent->frameGeometry().topLeft() + (parent->rect().center() - layerSelcWgt.rect().center()));
	}
	layerSelcWgt.showMessageBox();
	return m_button;
}

CLayerTypeSelectWidget::CLayerTypeSelectWidget(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags)
	, m_pEvtLoop(nullptr)
	, m_bMousePressed(false)
{	
	setAttribute(Qt::WA_ShowModal);
	setAttribute(Qt::WA_TranslucentBackground);

	m_pOverlayWgt = new QWidget(this);
	m_pOverlayWgt->setGeometry(parent->rect());
	m_pOverlayWgt->setStyleSheet("background-color: rgba(0, 0, 0, 170);");

	m_pCenterWgt = new QWidget(this);
	m_pCenterWgt->setObjectName("center_wgt");
	m_pCenterWgt->setMinimumWidth(250);
	m_pCenterWgt->setMaximumWidth(250);
	m_pCenterWgt->move(parent->rect().center() - m_pCenterWgt->rect().center());
	
	m_button = CLayerTypeSelectWidget::No;

	QLabel *pTitleLbl = new QLabel;
	pTitleLbl->setText(tr("please select the layer type you wan to add"));
	QHBoxLayout *pTitleLayout = new QHBoxLayout;
	pTitleLayout->addWidget(pTitleLbl);
	pTitleLayout->addStretch();

	m_pImgBtn = new QRadioButton;
	m_pImgBtn->setText(tr("image layer"));
	m_pEleBtn = new QRadioButton;
	m_pEleBtn->setText(tr("elevation layer"));
	QFrame *pHLine = new QFrame;
	pHLine->setFrameShape(QFrame::HLine);
	pHLine->setFrameShadow(QFrame::Sunken);
	QVBoxLayout *pVLayout = new QVBoxLayout;
	m_pImgBtn->setChecked(true);
	pVLayout->setSpacing(12);
	pVLayout->addWidget(m_pImgBtn);
	pVLayout->addWidget(m_pEleBtn);
	pVLayout->addWidget(pHLine);

	QPushButton *pYesBtn = new QPushButton;
	pYesBtn->setText(tr("confirm"));
	pYesBtn->setObjectName("pYesBtn");
	pYesBtn->setMinimumSize(90, 24);
	QPushButton *pNoBtn = new QPushButton;
	pNoBtn->setText(tr("cancel"));
	pNoBtn->setObjectName("pNoBtn");
	pNoBtn->setMinimumSize(90, 24);
	QFrame *pVLine = new QFrame;
	pVLine->setFrameShape(QFrame::VLine);
	pVLine->setFrameShadow(QFrame::Sunken);
	QHBoxLayout *pHLayout = new QHBoxLayout;
	pHLayout->addWidget(pYesBtn);
	pHLayout->addWidget(pVLine);
	pHLayout->addWidget(pNoBtn);

	QVBoxLayout *pVLayout1 = new QVBoxLayout;
	pVLayout1->setContentsMargins(19, 19, 19, 19);
	pVLayout1->setSpacing(6);

	pVLayout1->addLayout(pTitleLayout);
	pVLayout1->addLayout(pVLayout);
	pVLayout1->addLayout(pVLayout);
	pVLayout1->addLayout(pHLayout);
	m_pCenterWgt->setLayout(pVLayout1);

	connect(pYesBtn, SIGNAL(clicked()), this, SLOT(onYesBtnClickedSlot()));
	connect(pNoBtn, SIGNAL(clicked()), this, SLOT(onNoBtnClickedSlot()));

	QFile file(":/qss/Resources/qss/LayerSelect.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = file.readAll();
		setStyleSheet(str);
	}

	pYesBtn->setStyleSheet("color:#3b7cff");

	setGeometry(parent->geometry());
}


CLayerTypeSelectWidget::~CLayerTypeSelectWidget()
{
}

void CLayerTypeSelectWidget::showMessageBox()
{
	if (m_pEvtLoop == nullptr)
	{
		m_pEvtLoop = new QEventLoop(this);
	}
	m_pEvtLoop->exec();
}

void CLayerTypeSelectWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() | Qt::LeftButton)
	{
		m_bMousePressed = true;
		m_mousePressPt = event->globalPos() - frameGeometry().topLeft();
	}
}

void CLayerTypeSelectWidget::mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() | Qt::LeftButton) && m_bMousePressed)
	{
		if (parentWidget ())
		{
			if (!parentWidget ()->isMaximized ())
			{
				parentWidget ()->move(mapToGlobal(event->pos() - m_mousePressPt));	
				move(mapToGlobal(event->pos() - m_mousePressPt));
			}					
		}			
	}
}

void CLayerTypeSelectWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_bMousePressed = false;
}

void CLayerTypeSelectWidget::keyPressEvent( QKeyEvent *event )
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Space)
	{
		onYesBtnClickedSlot();
	}
	else if(event->key() == Qt::Key_Escape)
	{
		onNoBtnClickedSlot();		
	}

	QWidget::keyPressEvent(event);
}

void CLayerTypeSelectWidget::onYesBtnClickedSlot()
{
	if (m_pImgBtn->isChecked())
	{
		m_layerType = CLayerConfig::ImageLayer;
	}
	else if (m_pEleBtn->isChecked())
	{
		m_layerType = CLayerConfig::ElevationLayer;
	}
	m_button = CLayerTypeSelectWidget::Yes;
	if (m_pEvtLoop != nullptr)
	{
		m_pEvtLoop->exit();
	}
	this->hide();
}

void CLayerTypeSelectWidget::onNoBtnClickedSlot()
{
	m_button = CLayerTypeSelectWidget::No;
	if (m_pEvtLoop != nullptr)
	{
		m_pEvtLoop->exit();
	}
	this->hide();
}

void CLayerTypeSelectWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}