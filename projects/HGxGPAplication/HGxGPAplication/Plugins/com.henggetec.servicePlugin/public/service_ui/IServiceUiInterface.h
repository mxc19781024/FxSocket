#ifndef ISERVICE_UI_SERVICE_H
#define ISERVICE_UI_SERVICE_H

#include <QObject>
#include <QStandardItemModel>
//#include <FreeXCore/IFxDocument.h>

class IServiceUiInterface
{
public:
	// 获取服务界面指针
	virtual QWidget *getWgt() const = 0;

	/**  
		* @brief 加载文档
		* @param pEle [in] xml节点，包含需要解析的内容
		* @param pProcess [in] 用于监测处理进度
		* @return 是否加载成功
	*/
	virtual bool load( const std::string &strFilePath = ""/*, FxCore::IFxWorkspaceProcess* pProcess=NULL*/) = 0;

	/**  
		* @brief 保存文档
		* @param pEle [in] xml节点，包含需要解析的内容
		* @param pProcess [in] 用于监测处理进度
		* @return 是否保存成功
	*/
	virtual bool save( const std::string &strFilePath = ""/*, FxCore::IFxWorkspaceProcess* pProcess=NULL*/) = 0;
};

Q_DECLARE_INTERFACE(IServiceUiInterface, "freex_service_ui_interface")

#endif
