#ifndef SCENE_DROP_INTERFACE
#define SCENE_DROP_INTERFACE

#include <QString>

/**
* @class	: ISceneDropInterface
* @brief	: 场景中拖放接口
* @note		: 
* @author	: k00026
**/
class ISceneDropInterface
{
public:
	/**
	* @brief	: 处理drop事件
	* @params	: sceneId [in] 场景id
	* @params	: sceneType [in] 场景类型，二维
	*/
	virtual void handleDrop(std::string sceneId, const QPoint &screenPos = QPoint()) = 0;

	/**
	* @brief	: 场景dragEnter接收的类型
	*/
	virtual QString mimeType() const = 0;
};

#endif
