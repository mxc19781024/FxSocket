#ifndef GxServerProUtils_H
#define GxServerProUtils_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include <QObject>
#include "Common.h"

#ifdef CSERVERPROUTILS_LIB
# define CGxServerProUtils_EXPORT Q_DECL_EXPORT
#else
# define CGxServerProUtils_EXPORT Q_DECL_IMPORT
#endif

class CGxServerTree;

class CGxServiceProWgt;
class CGxAddServiceMgr;

class CGxServerProUtils_EXPORT CGxServerProUtils : public QObject
{
	Q_OBJECT
public:
	static CGxServerProUtils *getInstacne(QWidget *parent = 0)
	{
		if (!s_pServerProUtils)
		{
			s_pServerProUtils = new CGxServerProUtils(parent);
		}
		return s_pServerProUtils;
	}

	/**  
	  * @brief  显示添加服务窗口
	  * @param type[in] 窗口类型
	  * @param strType[in] 服务类型
	*/
	void showAddServiceWindow(EWindowType type, QString strType = "");

	/**  
	  * @brief  显示添加服务窗口
	  * @param point[in] 窗口显示位置
	  * @param type[in] 窗口类型
	  * @param strType[in] 服务类型
	*/
	void showAddServiceWindow(const QPoint &point, EWindowType type, QString strType = "");


	/**  
	  * @brief 显示服务属性窗口
	  * @param type[in] 窗口类型
	*/
	void showServiceProWindow(EWindowType type);

	/**  
	  * @brief 显示服务属性窗口
	  * @return 服务窗口指针
	*/
	CGxServiceProWgt* getServiceProWindow(EWindowType type);


	/**  
	  * @brief 设置图层属性
	  * @param eType[in] 窗口类型
	  * @param stLayerProPerty[in] 图层结构体
	*/
	void setLayerProperty(EWindowType eType, const SLayerPropertyStruct &stLayerProPerty);


	/**  
	  * @brief 设置服务属性
	  * @param stServerProPerty[in] 服务结构体
	*/
	void setServerProPerty(const SServerPropertyStruct &stServerProPerty);

	/**  
	  * @brief 获取添加服务窗口
	  * @return 添加服务窗口指针
	*/
	QWidget* geteAddServiceMgr(){return m_pAddServiceMgr;}


	/**  
	  * @brief 设置服务名称
	  * @param eType[in] 窗口类型
	  * @param strServerName[in] 服务名称
	*/
	void setServerName(EWindowType eType, const QString &strServerName);

	/**  
	  * @brief 属性窗口是否隐藏
	  * @return true显示 false隐藏
	*/
	bool bProWgtVisible();

signals:
	// 通过属性编辑更改服务名称
	void reSetByProEditSignal(const SServerPropertyStruct &stServerPropertyStruct);
	// 通过属性编辑更改图层信息
	void reLayerSetByProEditSignal(const SLayerPropertyStruct &stLayerPropertyStruct);

private:
	CGxServerProUtils(QWidget *parent = 0);
	~CGxServerProUtils();

	/**  
	  * @brief 
	  * @param
	  * @param
	*/
	void init();

	/**  
	  * @brief 
	  * @param
	  * @param
	*/
	CGxServiceProWgt* createServiceProWindow(EWindowType type);

	/**  
	  * @brief 
	  * @param
	  * @param
	*/
	QWidget* createAddServiceWindow(EWindowType type, QString strType);

private:
	static CGxServerProUtils*				s_pServerProUtils;

	
	// 服务属性的窗口指针
	CGxServiceProWgt*						m_pServiceProWgt;

	// 添加服务窗口的管理类指针
	QWidget*							m_pAddServiceMgr;

	// 父窗口指针
	QWidget*							m_pParent;
	
};

#endif // GxServerProUtils_H
