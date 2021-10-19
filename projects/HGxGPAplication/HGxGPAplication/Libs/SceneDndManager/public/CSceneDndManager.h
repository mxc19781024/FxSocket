#ifndef SCENE_DND_MANAGER_H
#define SCENE_DND_MANAGER_H

#include <QObject>

class ISceneDropInterface;
class QDragEnterEvent;
class QDropEvent;

/**
* @class	: CSceneDndManager
* @brief	: 场景拖放管理器接口类
* @note		: 用于场景拖放，比如拖放图层进场景添加图层，拖放模型进场景添加模型等。通过ICore::sceneDndManager()将ISceneDropInterface实例注册进场景
* @author	: k00026
**/
class CSceneDndManager : public QObject
{
	Q_OBJECT

public:
	/// 构造函数
	CSceneDndManager(QObject *parent = 0) : QObject(parent) {}

	/// 析构函数
	virtual ~CSceneDndManager() {}

	/**
	* @brief	: 将拖放处理对象注册进框架
	* @return	: 注册成功返回true，否则返回false
	*/
	virtual bool registerHandler(ISceneDropInterface *) = 0;

	/**
	* @brief	: 注销拖放处理对象
	* @note		: 请务必保证在注册的对象析构时，将拖放对象注销，否则会出错！！！
	* @return	: 注销成功返回true，否则返回false
	*/
	virtual bool unRegisterHandler(ISceneDropInterface *) = 0;

	// 判断DragEnter事件是否可drop
	virtual bool isDroppable(QDragEnterEvent *) = 0;

	// 处理Drop事件
	virtual ISceneDropInterface *dropHandler(QDropEvent *) = 0;
};

#endif // SCENE_DND_MANAGER_H
