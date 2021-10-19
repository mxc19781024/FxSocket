#ifndef SCENE_CONTROLWIDGET_H
#define SCENE_CONTROLWIDGET_H

#include <QWidget>
#include <QHash>
#include "ctkPluginContext.h"
#include "service/event/ctkEventHandler.h"
#include "qlistwidget.h"
#include "CLoadDataWidget.h"
#include "CSimuListWidget.h"

namespace Ui {
class CSceneControlWidget;
}


class CSceneControlWidget : public QWidget, public ctkEventHandler
{
    Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
    explicit CSceneControlWidget(ctkPluginContext *context, QWidget *parent = 0);
    ~CSceneControlWidget();

protected:
	/**
	* @brief	事件处理
	* @param	event [in] 事件
	*/
	virtual void handleEvent(const ctkEvent& event);

	void init();
protected:
	/**  
	  * @note 获取天体服务
	*/
	void getScenceService(int nSceneID);

signals:
	void SceneControlShowSignal();
private Q_SLOTS:
	void SlotShowSimuListWgt();
	void SlotShowLoadDataWgt();
	void SlotSwitchManipulator();
	void SlotSwitchMP(int);
	void SceneControlShowSlot();
private:
    Ui::CSceneControlWidget *ui;
	CLoadDataWidget* m_pLoadDataWgt;
	ctkPluginContext* m_pContext;
	CSimuListWidget* m_pSimuWidget;
	bool			m_bSpaceManiputor;
};

#endif // NODEWIDGET_H
