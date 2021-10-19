/**************************************************************************************************
* @file C3DRealScene.h
* @note 三维实景
* @author w00040
* @data 2019-5-10
**************************************************************************************************/
#ifndef C3D_REAL_SCENE_H
#define C3D_REAL_SCENE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

#include <ctkServiceRegistration.h>

#include "ui_C3DRealScene.h"
#include "C3DRealSceneSelectWidget.h"
#include "C3DRealSceneItem.h"
#include "CTiltAddModelWgt.h"
#include "CTiltToolWgt.h"

#include <service/event/ctkEventHandler.h>

struct ctkEventAdmin;
class ctkPluginContext;
class C3DRealSceneSelectWidget;

//三维实景
class C3DRealScene : public QWidget, public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)

public:
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
	C3DRealScene(ctkPluginContext *context, QWidget *parent = 0, Qt::WFlags flags = 0);
#else
	C3DRealScene(ctkPluginContext *context, QWidget *parent = 0, Qt::WindowFlags flags = 0);
#endif
	~C3DRealScene();

protected:
	// 事件处理
	void handleEvent(const ctkEvent &event) override;

protected slots:
	/**  
	  * @brief 倾斜摄影模型槽函数
	*/
	void slotTiltModel();
		/**  
	  * @brief 倾斜摄影格式转化工具槽函数
	*/
	void slotTiltTool();
private:
	Ui::C3DRealScene ui;

	// 倾斜摄影模型按钮
	C3DRealSceneItem*					m_pTiltModelBtn;
	//格式转换工具按钮
	C3DRealSceneItem*                   m_pTiltToolBtn; 
	// 插件上下文
	ctkPluginContext*					m_pContext;

	// 实景三维选择窗口
	C3DRealSceneSelectWidget*			m_pSelectWgt;

	// 实景三维工具按钮
	QVector<C3DRealSceneItem*>			m_vecToolItems;

	// 倾斜摄影属性窗
	CTiltAddModelWgt*					m_pTiltModelWgt;

	//格式转换工具
	CTiltToolWgt*                       m_pTiltToolWgt; 

	// 场景ID
	std::string							m_strSceneID;
};

#endif
