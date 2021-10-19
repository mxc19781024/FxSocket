#ifndef ISCENE_INTERFACE_H
#define ISCENE_INTERFACE_H

#include <QWidget>

namespace HGxScene
{
	class CGxSceneBase;
}

/**
* @class	: ISceneInterface
* @brief	: 单个场景对象
* @note 
* @author	: k00026
*/
class ISceneInterface : public QObject
{
public:
	ISceneInterface(QObject *parent) : QObject(parent) {}

	virtual ~ISceneInterface() {}

	/// 场景id
	virtual std::string sceneId() const = 0;

	/// 场景名称
	virtual QString sceneName() const = 0;
	
	/// 场景类型
	virtual QString sceneType() const = 0;

	/// 场景widget
	virtual QWidget *sceneWidget() const = 0;

	/// dockwidget
	virtual QWidget *dockWidget() const = 0;

	/**  
	* @brief	: 获取场景渲染窗口指针
	* @note add by w00025
	* @return	: 场景渲染窗口指针
	*/
	virtual QWidget *getSceneRenderWgt() = 0;

	/// 底层场景对象
	virtual HGxScene::CGxSceneBase* fxScene() const = 0;
};


#endif