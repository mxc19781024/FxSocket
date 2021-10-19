/**
* @file		: GxServiceModelUtilUi.h
* @brief	: 服务模型
* @note 
* @author	: c00169
*/
#ifndef GXSERVICEMODELUTILUI_H
#define GXSERVICEMODELUTILUI_H

#include <QObject>
#include <QStandardItemModel>

//#include <FreexSysConfig.h>
#include "Common.h"

#include "servicemodelutilui_export.h"

/**
* @class	: CGxServiceModelUtilUi
* @brief	: 服务模型
* @note 
* @author	: c00169
*/
class SERVICEMODELUTILUI_EXPORT CGxServiceModelUtilUi : public QObject 
{
	Q_OBJECT

public:
	static CGxServiceModelUtilUi* getInstance();
	
	QStandardItemModel* getServiceModel(){return m_pServiceModel;}

	QVector<QStandardItem *> getOGCLayerItems() {return m_vecOGCLayerSetItem;}

	QVector<QStandardItem *> getServiceItems() {return m_vecServerItem;}

	/**  
	  * @brief 链接本地文件或者文件夹
	  * @param strPath [in] 文件或者文件夹路径
	*/
	void folderLink(const SLinkInfo &stLinkInfo);

	/**
     * @brief 新增图层
	 * @param stServerProPety [in] 新增图层所包含的内容
   */
	void newAddLayer(SServerPropertyStruct stServerProPety);

	/**  
	  * @note 添加恒道云服务图层
	*/
	void addHGLayer(SServerPropertyStruct stServerProPety);

	/** 
	* @brief 移除OGC图层节点
	*/
	void removeOGCLayerItem(int iIndex);

	/** 
	* @brief 移除文件
	*/
	void removeFileLink(const QString &strFilePath);

	//初始化恒道云服务节点
	void initCGxServer();

	/**  
	  * @brief 递归遍历文件夹
	  * @param strPath [in] 文件夹路径
	  * @param pParentItem [in] 父节点
	*/
	void recurTravDir(const QString &strPath, QStandardItem *pParentItem);

signals:
	// 展开节点信号
	void expandItemsSignal( const QModelIndex & index);

private:
	CGxServiceModelUtilUi();
	~CGxServiceModelUtilUi();

	/** 
	* @brief 存储节点信息
	*/
	void storageData(ETreeItemType nItemType, EServerType nServerType, QStandardItem *pItem);

	/** 
	* @brief 存储图层集属性信息
	*/
	void storageServerProData(const SServerPropertyStruct & stServerPropertyStruct, QStandardItem *pItem);

	/** 
	* @brief 存储图层属性信息
	*/
	void storageLayerProData(const SLayerPropertyStruct & stLayerPropertyStruct, QStandardItem *pItem);

private:
	static CGxServiceModelUtilUi			*s_pModelUtil;

	QStandardItemModel						*m_pServiceModel;

	// 链接文件/文件夹路径
	QStringList								m_strPathsList;

	// 所有的服务节点
	QVector<QStandardItem *>				m_vecServerItem;	

	// 所有恒道云服务节点
	QVector<QStandardItem *>				m_vecHGServerItem;

	// 所有OGC服务图层集节点
	QVector<QStandardItem *>				m_vecOGCLayerSetItem;
};

#endif // GXSERVICEMODELUTILUI_H
