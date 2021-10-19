#ifndef SCENE_DND_MANAGER_P_H
#define SCENE_DND_MANAGER_P_H

#include <QSet>

#include "public/CSceneDndManager.h"
#include "scenedndmanager_export.h"

/**
* @class	: CSceneDndManagerPrivate
* @brief	: 场景拖放管理器接口类
* @note		: 用于场景拖放，比如拖放图层进场景添加图层，拖放模型进场景添加模型等。通过ICore::sceneDndManager()将ISceneDropInterface实例注册进场景
* @author	: k00026
**/
class SCENEDNDMANAGER_EXPORT CSceneDndManagerPrivate : public CSceneDndManager
{
	Q_OBJECT

public:
	friend class ISceneDropInterface;

	/// 构造函数
	explicit CSceneDndManagerPrivate(QObject *object = nullptr);

	/// 析构函数
	~CSceneDndManagerPrivate();

	/// 单例
	static CSceneDndManagerPrivate *instance();

	/**
	* @brief	: 将拖放处理对象注册进框架
	* @return	: 注册成功返回true，否则返回false
	*/
	bool registerHandler(ISceneDropInterface *);

	/**
	* @brief	: 注销拖放处理对象
	* @note		: 请务必保证在注册的对象析构时，将拖放对象注销，否则会出错！！！
	*/
	bool unRegisterHandler(ISceneDropInterface *);

	// 判断DragEnter事件是否可drop
	bool isDroppable(QDragEnterEvent *) override;

	// 处理Drop事件
	ISceneDropInterface *dropHandler(QDropEvent *) override;

private slots:
	// 拖放对象销毁槽函数
	void dndObjDestroyedSlot(QObject *);

private:
	static CSceneDndManagerPrivate *s_pInstance;		// 单例
	QSet<ISceneDropInterface *> m_setDropHandlerObj;	// 所有drop处理对象的集合
};

#endif