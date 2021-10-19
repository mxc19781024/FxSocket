#ifndef CSCENEUIPLUGIN_H
#define CSCENEUIPLUGIN_H

#include <QObject>
#include <QMutex>
#include <QTimer>

#include <ctkPluginContext.h>
#include <scene_ui/ISceneUiService.h>
//#include <scene/IFxScene.h>
//#include <FreeXCore/IFxDocument.h>
#include "service/event/ctkEventAdmin.h"
#include "service/event/ctkEventConstants.h"

#include <osg/ref_ptr>

#include <service/event/ctkEventHandler.h>

namespace HGxScene
{
	class CGxControllerMgr;
	class CGxSceneBase;
	class CGxLayerManager;
}

class CSceneUiPlugin : public QObject,  public ISceneUiService/*, public FxCore::IFxDocument*/, public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ISceneUiService)
	//Q_INTERFACES(FxCore::IFxDocument)
	Q_INTERFACES(ctkEventHandler)

public:
	CSceneUiPlugin(ctkPluginContext *context);
	~CSceneUiPlugin();

	// 创建场景
	//int createScene(const QString &name, const QString &type, bool plotSync = false) override;

	void saveSceneDockWidget(const std::string& strSceneID, QWidget* pWidget) override;

	QWidget* createScene(const std::string& strSceneID, const std::string& strSceneType, int iController = 0, bool plotSync = false) override;
	
	// 从模板文件创建场景
	int createSceneFromTemplate(const QString &name, const QString &filePath, bool bSync = false);

	// 移除场景
	bool removeScene(std::string) override;

	// 设置当前场景
	bool setCurrentScene(std::string sceneId);

	// 当前活动场景的id
	std::string activeScene() const override;

	// 当前活动场景类型
	QString activeSceneType() const override;

	// 所有二维场景
	QList<ISceneInterface *> mapScenes() const override;

	// 所有三维场景
	QList<ISceneInterface *> earthScenes() const override;

	// 根据场景id获得场景
	ISceneInterface *scene(std::string sceneId) override;

	// 根据场景名称获得场景
	ISceneInterface *scene(const QString &sceneName) override;

	// 关闭场景
	bool closeScene(const std::string iSceneId, const QString &strName = "");

	// 重命名场景
	bool renameScene(const QString &strNewSceneName, const std::string unSceneID = 0);

// 	// 从文档中加载场景
// 	bool load(const FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess) override;
// 
// 	// 将场景保存到文档中
// 	bool save(FxCommonUtils::TiXmlElement *pEle, FxCore::IFxWorkspaceProcess *pProcess) override;
// 
// 	// 设置文档级别
// 	int getDocLevel();

	// 激活场景
	void activateScene(ISceneInterface *);

	/**  
	* @brief 暂停/开启场景渲染，避免在处理线程操作时引起资源抢占崩溃
	* @note 在线程中处理渲染相关操作时，先暂停渲染，处理完成后开启渲染
	*/
	virtual void pauseRender(bool bPause);

protected:
	// 事件处理
	void handleEvent(const ctkEvent &event) override;

private:

	/*********此处需要保留*************/
	//void createSceneFromFile(IFxScene *);

	void setSceneTabInMainWindow(const QString &);

	void dockVisibleSet(const std::string& sceneType);

signals:
	// 场景切换信号(即将切换)
	// note:场景即将切换信号发生在场景删除时以及点击场景切换时，发送的是上一个场景的
	// 信息
	void sceneAboutToSwitchSignal(const ctkDictionary &);

	// 场景切换信号(已切换)
	// note:场景即将切换信号发生在场景删除时以及点击场景切换时，发送的是当前切换的场景的
	// 信息
	void sceneSwitchedSignal(const ctkDictionary &);
	
	// 场景删除信号
	// note:图层和标绘插件在删除场景时的初始化操作，已放置在sceneAboutToSwitchSignal中去处理
	// 故在标绘和图层插件中没有对场景移除信号添加相应的处理槽函数
	void sceneRemoveSignal(const ctkDictionary &);

	void createSceneObjSignal();

private slots:
	// dock(场景)激活槽函数
	void dockActivatedSlot();

	// dock(场景)显隐状态改变槽函数
	void dockVisibleChangedSlot(bool);

	// 场景刷新
	void paintFrame();
	
	void sendSwitchEvents();

	void createSceneObjSlot();

private:
	// 插件上下文
	ctkPluginContext *m_pContext;

	// 当前活动场景的id
	//int m_iActiveSceneId;
	std::string m_strActiveSceneId;

	// 当前活动场景的类型
	//QString m_strActiveSceneType;
	std::string m_strActiveSceneType;

	// 场景id - 场景接口
	QHash<QString, ISceneInterface *> m_hashId2Scene;

	QHash<QString, QWidget*> m_hashId2Dock;

	// 场景管理器
	osg::ref_ptr<HGxScene::CGxControllerMgr>  m_pSceneCrtlManager;

	/// 主窗口
	QWidget* m_pMainWidget;
};

#endif // CSCENEUIPLUGIN_H
