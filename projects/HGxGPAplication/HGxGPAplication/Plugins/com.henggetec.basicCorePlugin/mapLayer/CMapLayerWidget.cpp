#include "CMapLayerWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QStyleOption>
#include <QPushButton>
#include <QPainter>
#include "FxSearchEdit/CSearchEdit.h"
#include "FxTreeSearchTool/CTreeSearchTool.h"
#include "CMapLayerTreeWidget.h"
#include <service/event/ctkEventConstants.h>
//#include <FreeXFramework/FreeXFramework.h>



CMapLayerWidget::CMapLayerWidget(ctkPluginContext *context, QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *pVLayout = new QVBoxLayout(this);
	pVLayout->setContentsMargins(10, 8, 10, 8);
	pVLayout->setSpacing(7);
	
	CSearchEdit *pSearchEdit = new CSearchEdit;
	QFrame *pHLine = new QFrame;
	pHLine->setFrameShape(QFrame::HLine);
	pHLine->setFrameShadow(QFrame::Sunken);

	m_pLayerTreeWgt = new CMapLayerTreeWidget(context);

	QFrame *pHLine1 = new QFrame;
	pHLine1->setFrameShape(QFrame::HLine);
	pHLine1->setFrameShadow(QFrame::Sunken);

	QHBoxLayout *pHLayout = new QHBoxLayout;
	
	QPushButton *pToTopBtn = new QPushButton;
	pToTopBtn->setToolTip(tr("to top"));
	pToTopBtn->setObjectName("toTopBtn");

	QPushButton *pToBottomBtn = new QPushButton;
	pToBottomBtn->setToolTip(tr("to bottom"));
	pToBottomBtn->setObjectName("toBottomBtn");

	QPushButton *pUpBtn = new QPushButton;
	pUpBtn->setToolTip(tr("up"));
	pUpBtn->setObjectName("upBtn");

	QPushButton *pDownBtn = new QPushButton;
	pDownBtn->setToolTip(tr("down"));
	pDownBtn->setObjectName("downBtn");

	QPushButton *pDelLayerBtn = new QPushButton;
	pDelLayerBtn->setToolTip(tr("delete"));
	pDelLayerBtn->setObjectName("delBtn");

	connect(pToTopBtn, SIGNAL(clicked()), this, SLOT(toTopBtnClickedSlot()));
	connect(pToBottomBtn, SIGNAL(clicked()), this, SLOT(toBottomBtnClickedSlot()));
	connect(pUpBtn, SIGNAL(clicked()), this, SLOT(upBtnClickedSlot()));
	connect(pDownBtn, SIGNAL(clicked()), this, SLOT(downClickedSlot()));
	connect(pDelLayerBtn, SIGNAL(clicked()), this, SLOT(delBtnClickedSlot()));

	pHLayout->addWidget(pToTopBtn);
	pHLayout->addStretch();
	pHLayout->addWidget(pToBottomBtn);
	pHLayout->addStretch();
	pHLayout->addWidget(pUpBtn);
	pHLayout->addStretch();
	pHLayout->addWidget(pDownBtn);
	pHLayout->addStretch();
	pHLayout->addWidget(pDelLayerBtn);
	pHLayout->setSpacing(2);

	pVLayout->addWidget(pSearchEdit);
	pVLayout->addWidget(pHLine);
	pVLayout->addWidget(m_pLayerTreeWgt);
	pVLayout->addWidget(pHLine1);
	pVLayout->addLayout(pHLayout);

	connect(pSearchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchLayerTreeSlot(const QString &)));

	context->registerService<IMapLayerUiService>(m_pLayerTreeWgt);
	context->registerService<IMapLayerWgtService>(this);

	//context->registerService<FxCore::IFxDocument>(m_pLayerTreeWgt);

	QFile file(":/Resources/qss_map_layer/maplayer.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(file.readAll());
	}
};

CMapLayerWidget::~CMapLayerWidget()
{

};

void CMapLayerWidget::initWhenPluginStart()
{
	//m_pLayerTreeWgt->initWhenPluginStart();
}

QWidget * CMapLayerWidget::getWgt()
{
	return this;
}

QWidget* CMapLayerWidget::getLayerMgrWgt()
{
	return m_pLayerTreeWgt->getMapLayerPropertyMgrWgt();
}

void CMapLayerWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CMapLayerWidget::searchLayerTreeSlot(const QString &strKeyword)
{
	CTreeSearchTool::search(strKeyword, m_pLayerTreeWgt);
}

void CMapLayerWidget::toTopBtnClickedSlot()
{
	m_pLayerTreeWgt->moveLayerToTop();
}

void CMapLayerWidget::toBottomBtnClickedSlot()
{
	m_pLayerTreeWgt->moveLayerToBottom();
}

void CMapLayerWidget::upBtnClickedSlot()
{
	m_pLayerTreeWgt->moveLayerUp();
}

void CMapLayerWidget::downClickedSlot()
{
	m_pLayerTreeWgt->moveLayerDown();
}

void CMapLayerWidget::delBtnClickedSlot()
{
	m_pLayerTreeWgt->deleteLayer();
}