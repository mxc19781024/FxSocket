#ifndef EARTH_LAYER_WIDGET_H
#define EARTH_LAYER_WIDGET_H

#include <QWidget>

#include "ctkPluginContext.h"

#include <earth_layer_ui/IEarthLayerWgtService.h>

class CEarthLayerTreeWidget;

class CEarthLayerWidget : public QWidget , public IEarthLayerWgtService
{
	Q_OBJECT
	Q_INTERFACES(IEarthLayerWgtService)

public:
	CEarthLayerWidget(ctkPluginContext *context, QWidget *parent = nullptr);
	~CEarthLayerWidget();
	void initWhenPluginStart();

	QWidget* getWgt() override;

	QWidget* getLayerMgrWgt() override;

protected:
	void paintEvent(QPaintEvent *);

private slots:
	void searchLayerTreeSlot(const QString &);
	void addImgLayerBtnClickedSlot();
	void addEleLayerBtnClickedSlot();
	void addVecLayerBtnClickedSlot();
	void addSeaLayerBtnClickedSlot();
	void delLayerBtnClickedSlot();

private:
	CEarthLayerTreeWidget *m_pLayerTreeWgt;
};

#endif
