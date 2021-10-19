#ifndef ISCENE_UI_SERVICE_H
#define ISCENE_UI_SERVICE_H

#include <QWidget>

class ISceneInterface;

/**
* @class	: ISceneUiService
* @brief	: 场景服务
* @note		: 
* @author	: k00026
**/
class ISceneUiService
{
public:
	/**
	* @brief	: 创建场景
	* @params	: name [in] 场景名称
	* @params	: type [in] 场景类型
	* @params	: plotSync [in] 是否开启标绘同步
	* @return	: 场景id，创建失败返回-1
	*/
	//virtual int createScene(const QString &name, const QString &type, bool plotSync = false) = 0;

	virtual void saveSceneDockWidget(const std::string& strSceneID, QWidget* pWidget) = 0;

	virtual QWidget* createScene(const std::string& strSceneID, const std::string& strSceneType, int iController = 0, bool plotSync = false) = 0;

	/**
	* @brief	: 从模板文件创建场景
	* @params	: name [in] 场景名称
	* @params	: filePath [in] 场景模板文件路径
	* @params	: plotSync [in] 是否开启标绘同步
	* @return	: 场景id，创建失败返回-1
	*/
	virtual int createSceneFromTemplate(const QString &name, const QString &filePath, bool plotSync = false) = 0;

	/**
	* @brief	: 移除场景
	* @params	: sceneId [in] 场景id
	*/
	//virtual bool removeScene(int sceneId) = 0;
	virtual bool removeScene(std::string sceneId) = 0;

	/**
	* @brief	: 设置当前场景
	* @params	: sceneId [in] 场景id
	* @return	: 切换成功返回true，否则返回false
	*/
	//virtual bool setCurrentScene(int sceneId) = 0;
	virtual bool setCurrentScene(std::string sceneId) = 0;

	/// 当前活动场景的id
	//virtual int activeScene() const = 0;
	virtual std::string activeScene() const = 0;

	/// 当前活动场景类型
	virtual QString activeSceneType() const = 0;

	/// 所有二维场景
	virtual QList<ISceneInterface *> mapScenes() const = 0;

	/// 所有三维场景
	virtual QList<ISceneInterface *> earthScenes() const = 0;

	/// 根据场景id获得场景
	//virtual ISceneInterface *scene(int sceneId) = 0;
	virtual ISceneInterface *scene(std::string sceneId) = 0;

	/// 根据场景名称获得场景
	virtual ISceneInterface *scene(const QString &sceneName) = 0;

	/**  
	* @brief 关闭场景
	* @note add by w00025
	* @param unSceneID [in] 场景ID
	* @param strName [in] 场景名称
	* @return 成功返回true，失败返回false
	*/
	//virtual bool closeScene(int sceneId, const QString &strName = "") = 0;
	virtual bool closeScene(const std::string sceneId, const QString &strName = "") = 0;

	/**  
	* @brief 对场景重命名
	* @note add by w00025
	* @param strNewSceneName[in] 新名称
	* @param unSceneID [in] 场景ID
	* @param strOldSceneName [in] 场景老名称
	* @return 成功返回true，失败返回false
	*/
	//virtual bool renameScene(const QString &strNewSceneName, const unsigned int unSceneID = 0) = 0;
	virtual bool renameScene(const QString &strNewSceneName, const std::string unSceneID = 0) = 0;

	/**  
	* @brief 暂停/开启场景渲染，避免在处理线程操作时引起资源抢占崩溃
	* @note 在线程中处理渲染相关操作时，先暂停渲染，处理完成后开启渲染
	*/
	virtual void pauseRender(bool bPause) = 0;
};

Q_DECLARE_INTERFACE(ISceneUiService, "freex_scene_ui_service")

#endif
