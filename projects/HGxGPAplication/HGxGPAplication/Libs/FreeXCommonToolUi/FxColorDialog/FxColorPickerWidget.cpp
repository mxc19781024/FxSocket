#include "CBasicColorArea.h"
#include "CCustomColorArea.h"
#include "CHColorArea.h"
#include "CSVColorArea.h"
#include "CPreviewColorArea.h"
#include "CHandPushButton.h"
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QFile>
#include <QEventLoop>
#include "FxColorPickerWidgetPrivate.h"


FxColorPickerWidgetPrivate::FxColorPickerWidgetPrivate(FxColorPickerWidget *parent)
	: q_ptr(parent)
	, m_bIsEdit(true)
	, m_iHue(0)
	, m_iSaturation(0)
	, m_iBrightness(0)
{
	Q_Q(FxColorPickerWidget);

	m_pBasicColorArea = new CBasicColorArea;
	m_pCustomColorArea = new CCustomColorArea;
	m_pSvColorArea = new CSVColorArea;
	m_pSvColorArea->setMinimumSize(250, 100);
	m_pHColorArea = new CHColorArea;
	m_pHColorArea->setMinimumSize(250, 24);

	QLabel *pCurColorLbl = new QLabel;
	pCurColorLbl->setText(tr("now"));
	m_pPreviewColorArea = new CPreviewColorArea;
	m_pPreviewColorArea->setMinimumSize(142, 17);
	QLabel *pNewColorLbl = new QLabel;
	pNewColorLbl->setText(tr("new"));
	pNewColorLbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	QLabel *pRedLbl = new QLabel;
	QLabel *pGreenLbl = new QLabel;
	QLabel *pBlueLbl = new QLabel;
	pRedLbl->setText(tr("red"));
	pGreenLbl->setText(tr("green)"));
	pBlueLbl->setText(tr("blue"));

	m_pRedSpx = new QSpinBox;
	m_pGreenSpx = new QSpinBox;
	m_pBlueSpx = new QSpinBox;
	m_pRedSpx->setRange(0, 255);
	m_pGreenSpx->setRange(0, 255);
	m_pBlueSpx->setRange(0, 255);

	m_pAddToCustomBtn = new CHandPushButton;
	m_pAddToCustomBtn->setText(tr("add custom"));
	m_pComfirmBtn = new CHandPushButton;
	m_pComfirmBtn->setText(tr("confirm"));
	m_pCancelBtn = new CHandPushButton;
	m_pCancelBtn->setText(tr("cancel"));

	QVBoxLayout *pLayout1 = new QVBoxLayout;
	pLayout1->addWidget(m_pBasicColorArea);
	pLayout1->addWidget(m_pCustomColorArea);
	pLayout1->setSpacing(10);
	pLayout1->setContentsMargins(12, 2, 12, 0);

	QVBoxLayout *pLayout2 = new QVBoxLayout;
	pLayout2->addWidget(m_pSvColorArea);
	pLayout2->addWidget(m_pHColorArea);
	pLayout2->setContentsMargins(2, 2, 2, 2);
	pLayout2->setSpacing(4);

	QHBoxLayout *pLayout3 = new QHBoxLayout;
	pLayout3->addWidget(pCurColorLbl);
	pLayout3->addWidget(m_pPreviewColorArea);
	pLayout3->addWidget(pNewColorLbl);
	pLayout3->setContentsMargins(12, 2, 12, 2);

	QHBoxLayout *pLayout4 = new QHBoxLayout;
	pLayout4->addWidget(pRedLbl);
	pLayout4->addWidget(m_pRedSpx);

	QHBoxLayout *pLayout5 = new QHBoxLayout;
	pLayout5->addWidget(pGreenLbl);
	pLayout5->addWidget(m_pGreenSpx);

	QHBoxLayout *pLayout6 = new QHBoxLayout;
	pLayout6->addWidget(pBlueLbl);
	pLayout6->addWidget(m_pBlueSpx);

	QVBoxLayout *pLayout7 = new QVBoxLayout;
	pLayout7->addLayout(pLayout4);
	pLayout7->addLayout(pLayout5);
	pLayout7->addLayout(pLayout6);

	QVBoxLayout *pLayout8 = new QVBoxLayout;
	pLayout8->addWidget(m_pAddToCustomBtn);
	pLayout8->addWidget(m_pComfirmBtn);
	pLayout8->addWidget(m_pCancelBtn);

	QHBoxLayout *pLayout9 = new QHBoxLayout;
	pLayout9->addLayout(pLayout7);
	pLayout9->addStretch();
	pLayout9->addLayout(pLayout8);
	pLayout9->setContentsMargins(12, 2, 12, 2);

	QVBoxLayout *pLayout10 = new QVBoxLayout(q->m_pCentralWgt);
	pLayout10->addLayout(pLayout1);
	pLayout10->addLayout(pLayout2);
	pLayout10->addLayout(pLayout3);
	pLayout10->addLayout(pLayout9);
	pLayout10->setContentsMargins(9, 9, 9, 15);

	m_pAddToCustomBtn->setObjectName("xui_colorpicker_button");
	m_pComfirmBtn->setObjectName("xui_colorpicker_button");
	m_pCancelBtn->setObjectName("xui_colorpicker_button");

	m_pEvtLoop = new QEventLoop(this);

	// 信号槽连接
	connect(m_pHColorArea, SIGNAL(HueManualChangedSignal(int)), this, SLOT(HueChangedManuallySlot(int)));
	connect(m_pSvColorArea, SIGNAL(SvManualChangedSignal(int, int, int)), this, SLOT(SvChangedManuallySlot(int, int, int)));
	connect(m_pBasicColorArea, SIGNAL(ColorItemSelcSignal(const QColor &)), this, SLOT(ColorItemSelectedSlot(const QColor &)));
	connect(m_pCustomColorArea, SIGNAL(ColorItemSelcSignal(const QColor &)), this, SLOT(ColorItemSelectedSlot(const QColor &)));
	connect(m_pAddToCustomBtn, SIGNAL(clicked()), this, SLOT(OnAddToCustomColorBtnClickedSlot()));
	connect(m_pComfirmBtn, SIGNAL(clicked()), this, SLOT(OnComfirmBtnClickedSlot()));
	connect(m_pCancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClickedSlot()));
	//connect(this, SIGNAL(close()), this, SLOT(OnCancelBtnClickedSlot()));
	connect(m_pRedSpx, SIGNAL(valueChanged(int)), this, SLOT(RValueChangedSlot(int)));
	connect(m_pGreenSpx, SIGNAL(valueChanged(int)), this, SLOT(GValueChangedSlot(int)));
	connect(m_pBlueSpx, SIGNAL(valueChanged(int)), this, SLOT(BValueChangedSlot(int)));
}


FxColorPickerWidgetPrivate::~FxColorPickerWidgetPrivate()
{

}

void FxColorPickerWidgetPrivate::HueChangedManuallySlot(int iHueValue)
{
	m_iHue = iHueValue;

	QColor color;
	color.setHsv(m_iHue, m_iSaturation, m_iBrightness);

	// 改变饱和度、亮度区域
	m_pSvColorArea->SetHue(iHueValue);

	// 改变hsv、rgb编辑区域
	m_bIsEdit = false;
	m_pRedSpx->setValue(color.red());
	m_pGreenSpx->setValue(color.green());
	m_pBlueSpx->setValue(color.blue());
	m_bIsEdit = true;

	// 改变颜色预览区域
	m_pPreviewColorArea->SetNewColor(color);
}

void FxColorPickerWidgetPrivate::SvChangedManuallySlot(int iHueValue, int iSaturationValue, int iBrightnessValue)
{
	m_iHue = iHueValue;
	m_iSaturation = iSaturationValue;
	m_iBrightness = iBrightnessValue;

	QColor color;
	color.setHsv(m_iHue, m_iSaturation, m_iBrightness);

	// 改变hsv、rgb编辑区域
	m_bIsEdit = false;
	m_pRedSpx->setValue(color.red());
	m_pGreenSpx->setValue(color.green());
	m_pBlueSpx->setValue(color.blue());
	m_bIsEdit = true;

	// 改变颜色预览区域
	m_pPreviewColorArea->SetNewColor(color);
}

void FxColorPickerWidgetPrivate::ColorItemSelectedSlot(const QColor &color)
{
	m_iHue = qBound(0, color.hue(), 360);
	m_iSaturation = color.saturation();
	m_iBrightness = color.value();

	// 改变色调区域
	m_pHColorArea->SetHueValue(m_iHue);

	// 改变饱和度、亮度区域
	m_pSvColorArea->SetHsv(m_iHue, m_iSaturation, m_iBrightness);

	// 改变颜色预览区域
	m_pPreviewColorArea->SetNewColor(color);

	// 改变hsv、rgb编辑区域
	m_bIsEdit = false;
	m_pRedSpx->setValue(color.red());
	m_pGreenSpx->setValue(color.green());
	m_pBlueSpx->setValue(color.blue());
	m_bIsEdit = true;
}

void FxColorPickerWidgetPrivate::OnAddToCustomColorBtnClickedSlot()
{
	QColor color;
	color.setHsv(m_iHue, m_iSaturation, m_iBrightness);
	m_pCustomColorArea->AddToCustomColor(color);
}

void FxColorPickerWidgetPrivate::OnComfirmBtnClickedSlot()
{
	Q_Q(FxColorPickerWidget);
	m_buttonRole = FxColorPickerWidget::Yes;
	m_color.setHsv(m_iHue, m_iSaturation, m_iBrightness);
	m_pPreviewColorArea->SetCurrentColor(m_color);
	
	if (m_pEvtLoop != NULL)
	{
		m_pEvtLoop->exit();
	}
}

void FxColorPickerWidgetPrivate::OnCancelBtnClickedSlot()
{
	Q_Q(FxColorPickerWidget);
	m_buttonRole = FxColorPickerWidget::No;
	
	if (m_pEvtLoop != NULL)
	{
		m_pEvtLoop->exit();
	}
}

void FxColorPickerWidgetPrivate::RValueChangedSlot(int r)
{
	if (!m_bIsEdit)
	{
		return;
	}
	m_bIsEdit = false;

	int iGreen = m_pGreenSpx->value();
	int iBlue = m_pBlueSpx->value();
	QColor color;
	color.setRgb(r, iGreen, iBlue);
	m_iHue = qBound(0, color.hue(), 360);
	m_iSaturation = color.saturation();
	m_iBrightness = color.value();

	// 改变色调区域
	m_pHColorArea->SetHueValue(m_iHue);

	// 改变饱和度、亮度区域
	m_pSvColorArea->SetHsv(m_iHue, m_iSaturation, m_iBrightness);

	// 改变颜色预览区域
	m_pPreviewColorArea->SetNewColor(color);

	m_bIsEdit = true;
}

void FxColorPickerWidgetPrivate::GValueChangedSlot(int g)
{
	if (!m_bIsEdit)
	{
		return;
	}
	m_bIsEdit = false;

	int iRed = m_pRedSpx->value();
	int iBlue = m_pBlueSpx->value();
	QColor color;
	color.setRgb(iRed, g, iBlue);
	m_iHue = qBound(0, color.hue(), 360);
	m_iSaturation = color.saturation();
	m_iBrightness = color.value();

	// 改变色调区域
	m_pHColorArea->SetHueValue(m_iHue);

	// 改变饱和度、亮度区域
	m_pSvColorArea->SetHsv(m_iHue, m_iSaturation, m_iBrightness);

	// 改变颜色预览区域
	m_pPreviewColorArea->SetNewColor(color);

	m_bIsEdit = true;
}

void FxColorPickerWidgetPrivate::BValueChangedSlot(int b)
{
	if (!m_bIsEdit)
	{
		return;
	}
	m_bIsEdit = false;

	int iRed = m_pRedSpx->value();
	int iGreen = m_pGreenSpx->value();
	QColor color;
	color.setRgb(iRed, iGreen, b);
	m_iHue = qBound(0, color.hue(), 360);
	m_iSaturation = color.saturation();
	m_iBrightness = color.value();

	// 改变色调区域
	m_pHColorArea->SetHueValue(m_iHue);

	// 改变饱和度、亮度区域
	m_pSvColorArea->SetHsv(m_iHue, m_iSaturation, m_iBrightness);

	// 改变颜色预览区域
	m_pPreviewColorArea->SetNewColor(color);

	m_bIsEdit = true;
}

FxColorPickerWidget::FxColorPickerWidget(QWidget *parent)
	: CFxBaseWindow(parent)
	, d_ptr(new FxColorPickerWidgetPrivate(this))
{
	QFile file(":/Resources/qss/colorpicker.css");
	if (file.open(QIODevice::Text | QIODevice::ReadOnly))
	{
		m_pCentralWgt->setStyleSheet(file.readAll());
	}
	setWindowTitle(tr("edit colors"));
	setResizable(false);
}

FxColorPickerWidget::~FxColorPickerWidget()
{
}

void FxColorPickerWidget::SetCurrentColor(const QColor &color)
{
	Q_D(FxColorPickerWidget);
	d->m_pPreviewColorArea->SetCurrentColor(color);
	d->ColorItemSelectedSlot(color);
}

QColor FxColorPickerWidget::GetCurrentColor() const
{
	Q_D(const FxColorPickerWidget);
	return d->m_color;
}

FxColorPickerWidget::ButtonRole FxColorPickerWidget::showDialog()
{
	show();
	QEventLoop evtLoop;
	d_ptr->m_pEvtLoop = &evtLoop;
	evtLoop.exec();

	d_ptr->m_pEvtLoop = nullptr;
	hide();
	return d_ptr->m_buttonRole;
}
//
//void FxColorPickerWidget::paintEvent(QPaintEvent *event)
//{
//	QWidget::paintEvent(event);
//
//	QStyleOption opt;
//	opt.init(this);
//	QPainter p(this);
//	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}
