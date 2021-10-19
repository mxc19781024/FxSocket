#include "CEarthLayerWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include "FxSearchEdit/CSearchEdit.h"
#include "FxTreeSearchTool/CTreeSearchTool.h"
#include "CEarthLayerTreeWidget.h"
#include <service/event/ctkEventConstants.h>
//#include <FreeXFramework/FreeXFramework.h>

#include <GxEventHelperInterface.h>
#include <GxPluginFrameworkCore.h>


CEarthLayerWidget::CEarthLayerWidget(ctkPluginContext *context, QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *pVLayout = new QVBoxLayout(this);
	pVLayout->setContentsMargins(10, 8, 10, 8);
	pVLayout->setSpacing(7);
	
	//CSearchEdit *pSearchEdit = new CSearchEdit;
	CSearchEdit* pSearchEdit = new CSearchEdit;

	QFrame *pHLine = new QFrame;
	pHLine->setFrameShape(QFrame::HLine);
	pHLine->setFrameShadow(QFrame::Sunken);

	m_pLayerTreeWgt = new CEarthLayerTreeWidget(context);

	QFrame *pHLine1 = new QFrame;
	pHLine1->setFrameShape(QFrame::HLine);
	pHLine1->setFrameShadow(QFrame::Sunken);

	QHBoxLayout *pHLayout = new QHBoxLayout;
	QPushButton *pAddImgLayerBtn = new QPushButton;
	QPushButton *pAddEleLayerBtn = new QPushButton;
	QPushButton *pAddVecLayerBtn = new QPushButton;
	QPushButton *pAddSeaLayerBtn = new QPushButton;

	QPushButton *pDelLayerBtn = new QPushButton;

	connect(pAddImgLayerBtn, SIGNAL(clicked()), this, SLOT(addImgLayerBtnClickedSlot()));
	connect(pAddEleLayerBtn, SIGNAL(clicked()), this, SLOT(addEleLayerBtnClickedSlot()));
	connect(pAddVecLayerBtn, SIGNAL(clicked()), this, SLOT(addVecLayerBtnClickedSlot()));
	connect(pAddSeaLayerBtn,SIGNAL(clicked()),this,SLOT(addSeaLayerBtnClickedSlot()));
	connect(pDelLayerBtn, SIGNAL(clicked()), this, SLOT(delLayerBtnClickedSlot()));
	//按钮， 左侧图片， 右侧文字
	pAddImgLayerBtn->setText(tr("    image"));
	pAddEleLayerBtn->setText(tr("    elevation"));
	pAddVecLayerBtn->setText(tr("    vector"));
	pAddSeaLayerBtn->setText(tr("    chart"));
	pDelLayerBtn->setText(tr("    delete"));
	pDelLayerBtn->hide();
	// add szy 2021-4-16
	pAddSeaLayerBtn->hide();

	pAddImgLayerBtn->setObjectName(tr("pAddImgLayerBtn"));	
	pAddEleLayerBtn->setObjectName(tr("pAddEleLayerBtn"));	
	pAddVecLayerBtn->setObjectName(tr("pAddVecLayerBtn"));	
	pAddSeaLayerBtn->setObjectName(tr("pAddSeaLayerBtn"));
	pDelLayerBtn->setObjectName(tr("pDelLayerBtn"));

	pHLayout->addWidget(pAddImgLayerBtn);
	pHLayout->addStretch();
	pHLayout->addWidget(pAddEleLayerBtn);
	pHLayout->addStretch();
	pHLayout->addWidget(pAddVecLayerBtn);
	pHLayout->addStretch();
	pHLayout->addWidget(pAddSeaLayerBtn);
	pHLayout->addStretch();
	pHLayout->addWidget(pDelLayerBtn);
	pHLayout->setSpacing(2);

	pVLayout->addWidget(pSearchEdit);
	pVLayout->addWidget(pHLine);
	pVLayout->addWidget(m_pLayerTreeWgt);
	pVLayout->addWidget(pHLine1);
	pVLayout->addLayout(pHLayout);

	connect(pSearchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchLayerTreeSlot(const QString &)));

	context->registerService<IEarthLayerUiService>(m_pLayerTreeWgt);
	context->registerService<IEarthLayerWgtService>(this);

	//context->registerService<FxCore::IFxDocument>(m_pLayerTreeWgt);

	QFile file(":/Resources/qss_earth_layer/earthlayer.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(file.readAll());
	}
};

CEarthLayerWidget::~CEarthLayerWidget()
{

};

void CEarthLayerWidget::initWhenPluginStart()
{
	//m_pLayerTreeWgt->initWhenPluginStart();
}

QWidget* CEarthLayerWidget::getWgt()
{
	return this;
}

QWidget* CEarthLayerWidget::getLayerMgrWgt()
{
	return m_pLayerTreeWgt->getEarthLayerPropertyMgrWgt();
}

void CEarthLayerWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CEarthLayerWidget::searchLayerTreeSlot(const QString &strKeyword)
{
	CTreeSearchTool::search(strKeyword, m_pLayerTreeWgt);
}

void CEarthLayerWidget::addImgLayerBtnClickedSlot()
{
	m_pLayerTreeWgt->beginAddImgLayer();
}

void CEarthLayerWidget::addEleLayerBtnClickedSlot()
{
	m_pLayerTreeWgt->beginAddEleLayer();
}

void CEarthLayerWidget::addVecLayerBtnClickedSlot()
{
	m_pLayerTreeWgt->beginAddVecLayer();
}

void CEarthLayerWidget::delLayerBtnClickedSlot()
{
	m_pLayerTreeWgt->delLayer();
}

void CEarthLayerWidget::addSeaLayerBtnClickedSlot()
{
	m_pLayerTreeWgt->beginAddSeaLayer();
}