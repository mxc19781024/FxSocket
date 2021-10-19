/**************************************************************************************************
* @file ctkFxPlugin.h
* @note FreeX 插件实现，处理插件的启动与停止
* @author g00034
* @data 2018-11-29
**************************************************************************************************/

#ifndef CTK_FXPLUGIN_P_H
#define CTK_FXPLUGIN_P_H

#include <ctkPluginActivator.h>
#include <CBizDataInterface.h>

/**
  * @class CFreeXPluginActivator
  * @brief FreeX 插件激活类
  * @author g00034
*/


class CFreeXPluginActivator : public QObject, public ctkPluginActivator
{
	Q_OBJECT
	Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	Q_PLUGIN_METADATA(IID "freex_plugin_bizdata_mgr_service")
#endif

public:
	CFreeXPluginActivator();

	/**  
	  * @brief 插件开启调用
	  * @param context [in] 插件上下文
	*/
	void start(ctkPluginContext* context);

	/**  
	  * @brief 插件停止调用
	  * @param context [in] 插件上下文
	*/
	void stop(ctkPluginContext* context);

private:
	// 插件服务对象
	CBizDataInterface* m_pBizDataPlugin;

}; 

#endif // CTK_FXPLUGIN_P_H
