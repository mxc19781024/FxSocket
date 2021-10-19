#ifndef MAP_LAYER_WIDGET_H
#define MAP_LAYER_WIDGET_H

#include <QWidget>

#include "ctkPluginContext.h"

#include <map_layer_ui/IMapLayerWgtService.h>

class CMapLayerTreeWidget;

class CMapLayerWidget : public QWidget, public IMapLayerWgtService
{
	Q_OBJECT
	Q_INTERFACES(IMapLayerWgtService)

public:
	CMapLayerWidget(ctkPluginContext *context, QWidget *parent = nullptr);
	~CMapLayerWidget();
	
	// 插件启动时初始化
	void initWhenPluginStart();

	QWidget* getWgt() override;

	QWidget* getLayerMgrWgt() override;

protected:
	void paintEvent(QPaintEvent *);

private slots:
	void searchLayerTreeSlot(const QString &);

	void toTopBtnClickedSlot();
	void toBottomBtnClickedSlot();
	void upBtnClickedSlot();
	void downClickedSlot();
	void delBtnClickedSlot();

private:
	CMapLayerTreeWidget *m_pLayerTreeWgt;
};

#endif // MAP_LAYER_WIDGET_H
