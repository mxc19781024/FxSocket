#ifndef CSCENEOBJ_H
#define CSCENEOBJ_H

#include "scene_ui/ISceneInterface.h"

namespace HGxScene
{
	class CGxSceneBase;
}

/**
* @class	：CSceneObj
* @brief	：场景对象
* @note 
* @author	：k00026
*/
class CSceneObj : public ISceneInterface
{
	Q_OBJECT

public:
	CSceneObj(HGxScene::CGxSceneBase* pScene, QWidget *dockWidget, QObject *parent);
	~CSceneObj();
	// 场景id
	std::string sceneId() const override;

	// 场景名称
	QString sceneName() const override;

	// 场景类型
	QString sceneType() const override;

	// 场景widget
	QWidget *sceneWidget() const override;

	// dockwidget
	QWidget *dockWidget() const override;

	QWidget* getSceneRenderWgt() override;

 	// 底层场景对象
	HGxScene::CGxSceneBase* fxScene() const;

protected:
	bool eventFilter(QObject *, QEvent *);

private:
	
	HGxScene::CGxSceneBase* m_pFxScene; // 底层场景对象

	QWidget *m_pDockWgt;	// 场景所在的dock窗口
};

#endif // CSCENEOBJ_H
