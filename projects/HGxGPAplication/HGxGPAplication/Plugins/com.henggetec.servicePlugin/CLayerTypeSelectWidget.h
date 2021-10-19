#ifndef LAYER_TYPE_SELECT_WIDGET
#define LAYER_TYPE_SELECT_WIDGET

#include <QWidget>
#include <QRadioButton>
//#include <FreeXSysConfig.h>
#include "layer_ui/CLayerManager.h"

#include <QKeyEvent>

class QEventLoop;

class CLayerTypeSelectWidget : public QWidget
{
	Q_OBJECT
public:
	enum ReturnButton
	{
		Yes,
		No
	};

	static ReturnButton warning(QWidget *parent);

public:
	CLayerTypeSelectWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
	~CLayerTypeSelectWidget();

private:
	void showMessageBox();

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void keyPressEvent(QKeyEvent *);
	void paintEvent(QPaintEvent *);

private slots:
	void onYesBtnClickedSlot();
	void onNoBtnClickedSlot();

public:
	static CLayerConfig::LayerType m_layerType;

private:
	QEventLoop			*m_pEvtLoop;
	static ReturnButton m_button;
	bool				m_bMousePressed;
	QPoint				m_mousePressPt;
	QRadioButton		*m_pImgBtn;
	QRadioButton		*m_pEleBtn;
	QWidget				*m_pOverlayWgt;				
	QWidget				*m_pCenterWgt;					
};

#endif
