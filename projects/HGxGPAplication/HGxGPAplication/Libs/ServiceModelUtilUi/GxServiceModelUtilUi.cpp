#include "public/GxServiceModelUtilUi.h"

#include <QFileInfo>
#include <QDir>
#include <QVariant>
#include <QVector>


CGxServiceModelUtilUi* CGxServiceModelUtilUi::s_pModelUtil = NULL;

CGxServiceModelUtilUi* CGxServiceModelUtilUi::getInstance()
{
	if(s_pModelUtil == NULL)
	{
		s_pModelUtil = new CGxServiceModelUtilUi;
	}
	return s_pModelUtil;
}


void CGxServiceModelUtilUi::folderLink( const SLinkInfo &stLinkInfo )
{
	// 如果链接的是同一文件夹或者同一文件
	for (int i = 0; i < m_strPathsList.size(); i++)
	{
		if (stLinkInfo.strAbsoluteFilePath == m_strPathsList[i])
		{
			return;
		}
	}

	// 获取到本地数据节点
	QStandardItem *pItem = m_pServiceModel->item(0);

	if (!pItem)
	{
		return;
	}

	// 如果链接的是文件
	if (stLinkInfo.eLinkType == FILELINK)
	{
		QStringList filters;
		/// yq 添加海图文件格式 
		filters << "shp" << "img" << "kml" << "tif" << "000";
		QFileInfo fileInfo(stLinkInfo.strAbsoluteFilePath);
		//如果链接的文件类型不符
		if (!filters.contains(fileInfo.suffix()))
		{
			return;
		}

		QStandardItem *pChildItem = new QStandardItem;
		pChildItem->setFlags(pChildItem->flags() & ~Qt::ItemIsEditable);
		storageData(LAYER_ITEM, LOCAL_SERVER, pChildItem);
		pChildItem->setText(fileInfo.fileName());
		// 存储文件所在的位置信息，打开文件所在位置时会遇到
		pChildItem->setData(fileInfo.absolutePath(), FilePathRole);
		pChildItem->setData(FILELINK, LinkRole);
		if (!fileInfo.exists())
		{
			pChildItem->setIcon(QIcon(":/Resources/images/tucengduankai.png"));
		}
		else
		{
			pChildItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
		}
		pItem->appendRow(pChildItem);
		
	}
	// 如果链接的是文件夹
	else if (stLinkInfo.eLinkType == FOLDERLINK)
	{
		QFileInfo fileInfo(stLinkInfo.strAbsoluteFilePath);
		QStandardItem *pChildItem = new QStandardItem;
		pChildItem->setFlags(pChildItem->flags() & ~Qt::ItemIsEditable);
		pChildItem->setText(fileInfo.absoluteFilePath());
		storageData(SERVER_ITEM, LOCAL_SERVER, pChildItem);
		pChildItem->setData(fileInfo.absolutePath(), FilePathRole);
		pChildItem->setData(FOLDERLINK, LinkRole);
		if (!fileInfo.exists())
		{
			pChildItem->setIcon(QIcon(":/Resources/images/wenjianjiaduankai.png"));
		}
		else
		{
			pChildItem->setIcon(QIcon(":/Resources/images/wenjianjia.png"));
		}
		pItem->appendRow(pChildItem);
		
		recurTravDir(stLinkInfo.strAbsoluteFilePath, pChildItem);
	}
	
	m_strPathsList << stLinkInfo.strAbsoluteFilePath;
	emit expandItemsSignal(pItem->index());
}

void CGxServiceModelUtilUi::newAddLayer( SServerPropertyStruct stServerProPety )
{
	// 遍历所有的服务的节点
	for (int i = 0; i < m_vecServerItem.size(); i++)
	{
		// 通过服务名称判断当前添加的是什么服务
		QString strText = m_vecServerItem[i]->text();
		if (stServerProPety.strServerType.toUpper() == m_vecServerItem[i]->text().toUpper())
		{
			if (m_vecServerItem[i]->data(ItemServerTypeRole).toInt() == OGC_SERVER)
			{
				// 如果是属于OGC的会有一个图层集
				QStandardItem *pLayerSetItem = new QStandardItem;
				pLayerSetItem->setToolTip(stServerProPety.strServerName);
				pLayerSetItem->setText(stServerProPety.strServerName);
				storageData(LAYER_SET_ITEM, OGC_SERVER, pLayerSetItem);
				storageServerProData(stServerProPety, pLayerSetItem);
				m_vecServerItem[i]->appendRow(pLayerSetItem);

				emit expandItemsSignal(m_vecServerItem[i]->index());
				
				m_vecOGCLayerSetItem.push_back(pLayerSetItem);
				// 判断该服务的连接状态
				if (stServerProPety.bConnect)
				{
					pLayerSetItem->setIcon(QIcon(":/Resources/images/wenjianjia.png"));
					pLayerSetItem->setData(true, ConnectTypeRole);
					// 然后再是图层节点
					for (int j = 0; j < stServerProPety.vecLayerProperty.size();j++)
					{
						QStandardItem *pLayerItem = new QStandardItem;
						pLayerItem->setToolTip(stServerProPety.vecLayerProperty[j].strLayerName);
						pLayerItem->setText(stServerProPety.vecLayerProperty[j].strLayerName);
						if (stServerProPety.bDefault)
						{
							pLayerItem->setIcon(QIcon(":/Resources/images/wenjianjia.png"));
						}
						else
						{
							pLayerItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
						}
						storageData(LAYER_ITEM, OGC_SERVER, pLayerItem);
						storageLayerProData(stServerProPety.vecLayerProperty[j], pLayerItem);
						storageServerProData(stServerProPety, pLayerItem);
						pLayerSetItem->appendRow(pLayerItem);
											
						emit expandItemsSignal(pLayerSetItem->index());
					}
				}
				else
				{
					pLayerSetItem->setIcon(QIcon(":/Resources/images/wenjianjiaduankai.png"));
					pLayerSetItem->setData(false, ConnectTypeRole);
				}
			}
			else if (m_vecServerItem[i]->data(ItemServerTypeRole).toInt() == TILE_SERVER)
			{
				// 如果是Tile直接是图层
				if (stServerProPety.vecLayerProperty.size() > 1)
				{
					return;
				}
				QStandardItem *pLayerItem = new QStandardItem;
				pLayerItem->setToolTip(stServerProPety.vecLayerProperty[0].strLayerName);
				pLayerItem->setText(stServerProPety.vecLayerProperty[0].strLayerName);

				if (stServerProPety.vecLayerProperty[0].bDefault)
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/wenjianjiaduankai.png"));
				}
				else if (stServerProPety.bConnect)
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
				}
				else
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/tucengduankai.png"));
				}

				storageData(LAYER_ITEM, TILE_SERVER, pLayerItem);
				storageLayerProData(stServerProPety.vecLayerProperty[0], pLayerItem);
				storageServerProData(stServerProPety, pLayerItem);
				m_vecServerItem[i]->appendRow(pLayerItem);
				
				emit expandItemsSignal(m_vecServerItem[i]->index());
			}
			else if (m_vecServerItem[i]->data(ItemServerTypeRole).toInt() == DATASET_SERVER)
			{
				if (stServerProPety.vecLayerProperty.size() > 1)
				{
					return;
				}

				QStandardItem *pLayerItem = new QStandardItem;
				pLayerItem->setToolTip(stServerProPety.vecLayerProperty[0].strLayerName);
				pLayerItem->setText(stServerProPety.vecLayerProperty[0].strLayerName);
				// 如果是DataSet直接是图层
				if (stServerProPety.vecLayerProperty[0].bDefault)
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/wenjianjiaduankai.png"));
				}
				else if (stServerProPety.bConnect)
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/shujujismall.png"));
				}
				else
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/shujuji_duankai.png"));
				}

				storageData(LAYER_ITEM, DATASET_SERVER, pLayerItem);
				storageLayerProData(stServerProPety.vecLayerProperty[0], pLayerItem);
				storageServerProData(stServerProPety, pLayerItem);
				m_vecServerItem[i]->appendRow(pLayerItem);
				
				emit expandItemsSignal(m_vecServerItem[i]->index());
			}
		}
	}
}

void CGxServiceModelUtilUi::addHGLayer(SServerPropertyStruct stServerProPety)
{
	// 遍历所有的服务的节点
	for (int i = 0; i < m_vecHGServerItem.size(); i++)
	{
		// 通过服务名称判断当前添加的是什么服务
		QString strText = m_vecHGServerItem[i]->text();
		if (stServerProPety.strServerType.toUpper() == m_vecHGServerItem[i]->text().toUpper())
		{
			if (m_vecHGServerItem[i]->data(ItemServerTypeRole).toInt() == OGC_SERVER)
			{
				// 如果是属于OGC的会有一个图层集
				QStandardItem *pLayerSetItem = new QStandardItem;
				pLayerSetItem->setToolTip(stServerProPety.strServerName);
				pLayerSetItem->setText(stServerProPety.strServerName);
				storageData(LAYER_SET_ITEM, OGC_SERVER, pLayerSetItem);
				storageServerProData(stServerProPety, pLayerSetItem);
				pLayerSetItem->setData(true, HGLayerRole);

				QVector<SServerPropertyStruct> vecServers;
				if (m_vecHGServerItem[i]->data(HGServersRole).isValid())
				{
					QVariant var =  m_vecHGServerItem[i]->data(HGServersRole);
					vecServers = var.value<QVector<SServerPropertyStruct>>();
				}
				vecServers.push_back(stServerProPety);
				m_vecHGServerItem[i]->setData(QVariant::fromValue(vecServers), HGServersRole);
				//m_vecHGServerItem[i]->appendRow(pLayerSetItem);

				emit expandItemsSignal(m_vecHGServerItem[i]->index());

				m_vecOGCLayerSetItem.push_back(pLayerSetItem);
				// 判断该服务的连接状态
				if (stServerProPety.bConnect)
				{
					pLayerSetItem->setIcon(QIcon(":/Resources/images/wenjianjia.png"));
					pLayerSetItem->setData(true, ConnectTypeRole);
					// 然后再是图层节点
					for (int j = 0; j < stServerProPety.vecLayerProperty.size(); j++)
					{
						QStandardItem *pLayerItem = new QStandardItem;
						pLayerItem->setToolTip(stServerProPety.vecLayerProperty[j].strLayerName);
						pLayerItem->setText(stServerProPety.vecLayerProperty[j].strLayerName);
						pLayerItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
						storageData(LAYER_ITEM, OGC_SERVER, pLayerItem);
						pLayerItem->setData(true, HGLayerRole);
						storageLayerProData(stServerProPety.vecLayerProperty[j], pLayerItem);
						storageServerProData(stServerProPety, pLayerItem);
						m_vecHGServerItem[i]->appendRow(pLayerItem);

						emit expandItemsSignal(pLayerSetItem->index());
					}
				}
				else
				{
					pLayerSetItem->setIcon(QIcon(":/Resources/images/wenjianjiaduankai.png"));
					pLayerSetItem->setData(false, ConnectTypeRole);
				}
			}
			else if (m_vecHGServerItem[i]->data(ItemServerTypeRole).toInt() == TILE_SERVER)
			{
				// 如果是Tile直接是图层
				if (stServerProPety.vecLayerProperty.size() > 1)
				{
					return;
				}
				QStandardItem *pLayerItem = new QStandardItem;
				pLayerItem->setToolTip(stServerProPety.vecLayerProperty[0].strLayerName);
				pLayerItem->setText(stServerProPety.vecLayerProperty[0].strLayerName);
				if (stServerProPety.bConnect)
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
				}
				else
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/tucengduankai.png"));
				}

				storageData(LAYER_ITEM, TILE_SERVER, pLayerItem);
				storageLayerProData(stServerProPety.vecLayerProperty[0], pLayerItem);
				storageServerProData(stServerProPety, pLayerItem);
				pLayerItem->setData(true, HGLayerRole);
				m_vecHGServerItem[i]->appendRow(pLayerItem);

				emit expandItemsSignal(m_vecHGServerItem[i]->index());
			}
			else if (m_vecHGServerItem[i]->data(ItemServerTypeRole).toInt() == DATASET_SERVER)
			{
				if (stServerProPety.vecLayerProperty.size() > 1)
				{
					return;
				}

				QStandardItem *pLayerItem = new QStandardItem;
				pLayerItem->setToolTip(stServerProPety.vecLayerProperty[0].strLayerName);
				pLayerItem->setText(stServerProPety.vecLayerProperty[0].strLayerName);
				// 如果是DataSet直接是图层
				if (stServerProPety.bConnect)
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/shujujismall.png"));
				}
				else
				{
					pLayerItem->setIcon(QIcon(":/Resources/images/shujuji_duankai.png"));
				}

				storageData(LAYER_ITEM, DATASET_SERVER, pLayerItem);
				storageLayerProData(stServerProPety.vecLayerProperty[0], pLayerItem);
				storageServerProData(stServerProPety, pLayerItem);
				pLayerItem->setData(true, HGLayerRole);
				m_vecHGServerItem[i]->appendRow(pLayerItem);

				emit expandItemsSignal(m_vecHGServerItem[i]->index());
			}
		}
	}
}

void CGxServiceModelUtilUi::removeOGCLayerItem(int iIndex)
{
	if (iIndex < m_vecOGCLayerSetItem.size() && iIndex >= 0)
	{
		m_vecOGCLayerSetItem.remove(iIndex);
	}
}
 
void CGxServiceModelUtilUi::removeFileLink( const QString &strFilePath )
{
	m_strPathsList.removeOne(strFilePath);
}

void CGxServiceModelUtilUi::initCGxServer()
{
	//QStandardItem *pCGxServeItem = new QStandardItem(tr("FreeXServer"));
	//pCGxServeItem->setIcon(QIcon(":/Resources/images/OGC.png"));

	QStandardItem *pWMSItem = new QStandardItem("WMS");
	pWMSItem->setIcon(QIcon(":/Resources/images/OGCSmall.png"));
	pWMSItem->setData(SERVER_ITEM, ItemTypeRole);
	pWMSItem->setData(OGC_SERVER, ItemServerTypeRole);
	pWMSItem->setData(true, HGLayerRole);

	QStandardItem *pWMTSItem = new QStandardItem("WMTS");
	pWMTSItem->setIcon(QIcon(":/Resources/images/OGCSmall.png"));
	pWMTSItem->setData(SERVER_ITEM, ItemTypeRole);
	pWMTSItem->setData(OGC_SERVER, ItemServerTypeRole);
	pWMTSItem->setData(true, HGLayerRole);

	QStandardItem *pWFSItem = new QStandardItem("WFS");
	pWFSItem->setIcon(QIcon(":/Resources/images/OGCSmall.png"));
	pWFSItem->setData(SERVER_ITEM, ItemTypeRole);
	pWFSItem->setData(OGC_SERVER, ItemServerTypeRole);
	pWFSItem->setData(true, HGLayerRole);

	QStandardItem *pWCSItem = new QStandardItem("WCS");
	pWCSItem->setIcon(QIcon(":/Resources/images/OGCSmall.png"));
	pWCSItem->setData(SERVER_ITEM, ItemTypeRole);
	pWCSItem->setData(OGC_SERVER, ItemServerTypeRole);
	pWCSItem->setData(true, HGLayerRole);

	//m_pServiceModel->appendRow(pCGxServeItem);

	//pCGxServeItem->appendRow(pWMSItem);
	//pCGxServeItem->appendRow(pWMTSItem);
	//pCGxServeItem->appendRow(pWFSItem);
	//pCGxServeItem->appendRow(pWCSItem);
	
	m_vecHGServerItem.push_back(pWMSItem);
	m_vecHGServerItem.push_back(pWMTSItem);
	m_vecHGServerItem.push_back(pWFSItem);
	m_vecHGServerItem.push_back(pWCSItem);
}

CGxServiceModelUtilUi::CGxServiceModelUtilUi()
{
	m_pServiceModel = new QStandardItemModel;

	QStandardItem *pLocalServeItem = new QStandardItem(tr("local service"));
	pLocalServeItem->setIcon(QIcon(":/Resources/images/bendifuwu.png"));
	pLocalServeItem->setData(SERVER_TYPE_ITEM, ItemTypeRole);
	pLocalServeItem->setData(LOCAL_SERVER, ItemServerTypeRole);

	QStandardItem *pOGCServeItem = new QStandardItem(tr("OGC service"));
	pOGCServeItem->setIcon(QIcon(":/Resources/images/OGC.png"));
	pOGCServeItem->setData(SERVER_TYPE_ITEM, ItemTypeRole);
	pOGCServeItem->setData(OGC_SERVER, ItemServerTypeRole);

	QStandardItem *pTileServeItem = new QStandardItem(tr("Tile service"));
	pTileServeItem->setIcon(QIcon(":/Resources/images/Tile.png"));
	pTileServeItem->setData(SERVER_TYPE_ITEM, ItemTypeRole);
	pTileServeItem->setData(TILE_SERVER, ItemServerTypeRole);

	QStandardItem *pDataSetItem = new QStandardItem(tr("dataset"));
	pDataSetItem->setIcon(QIcon(":/Resources/images/shujuji.png"));
	pDataSetItem->setData(SERVER_TYPE_ITEM, ItemTypeRole);
	pDataSetItem->setData(DATASET_SERVER, ItemServerTypeRole);

	m_pServiceModel->appendRow(pLocalServeItem);
	m_pServiceModel->appendRow(pOGCServeItem);
	m_pServiceModel->appendRow(pTileServeItem);
	m_pServiceModel->appendRow(pDataSetItem);

	QStandardItem *pWMSItem = new QStandardItem("WMS");
	pWMSItem->setIcon(QIcon(":/Resources/images/OGCSmall.png"));
	pWMSItem->setData(SERVER_ITEM, ItemTypeRole);
	pWMSItem->setData(OGC_SERVER, ItemServerTypeRole);

	QStandardItem *pWMTSItem = new QStandardItem("WMTS");
	pWMTSItem->setIcon(QIcon(":/Resources/images/OGCSmall.png"));
	pWMTSItem->setData(SERVER_ITEM, ItemTypeRole);
	pWMTSItem->setData(OGC_SERVER, ItemServerTypeRole);

	QStandardItem *pWFSItem = new QStandardItem("WFS");
	pWFSItem->setIcon(QIcon(":/Resources/images/OGCSmall.png"));
	pWFSItem->setData(SERVER_ITEM, ItemTypeRole);
	pWFSItem->setData(OGC_SERVER, ItemServerTypeRole);

	QStandardItem *pWCSItem = new QStandardItem("WCS");
	pWCSItem->setIcon(QIcon(":/Resources/images/OGCSmall.png"));
	pWCSItem->setData(SERVER_ITEM, ItemTypeRole);
	pWCSItem->setData(OGC_SERVER, ItemServerTypeRole);

	pOGCServeItem->appendRow(pWMSItem);
	pOGCServeItem->appendRow(pWMTSItem);
	pOGCServeItem->appendRow(pWFSItem);
	pOGCServeItem->appendRow(pWCSItem);

	m_vecServerItem.push_back(pWMSItem);
	m_vecServerItem.push_back(pWMTSItem);
	m_vecServerItem.push_back(pWFSItem);
	m_vecServerItem.push_back(pWCSItem);

	QStandardItem *pTMSItem = new QStandardItem("TMS");
	pTMSItem->setIcon(QIcon(":/Resources/images/Tilesmall.png"));
	pTMSItem->setData(SERVER_ITEM, ItemTypeRole);
	pTMSItem->setData(TILE_SERVER, ItemServerTypeRole);

	QStandardItem *pXYZItem = new QStandardItem("XYZ");
	pXYZItem->setIcon(QIcon(":/Resources/images/Tilesmall.png"));
	pXYZItem->setData(SERVER_ITEM, ItemTypeRole);
	pXYZItem->setData(TILE_SERVER, ItemServerTypeRole);

	pTileServeItem->appendRow(pTMSItem);
	pTileServeItem->appendRow(pXYZItem);

	m_vecServerItem.push_back(pTMSItem);
	m_vecServerItem.push_back(pXYZItem);

	m_vecServerItem.push_back(pDataSetItem);

	//初始化恒道云服务节点
	initCGxServer();
}

CGxServiceModelUtilUi::~CGxServiceModelUtilUi()
{

}

void CGxServiceModelUtilUi::storageData( ETreeItemType nItemType, EServerType nServerType, QStandardItem *pItem )
{
	if (!pItem)
	{
		return;
	}

	pItem->setData(nItemType, ItemTypeRole);
	pItem->setData(nServerType,ItemServerTypeRole);
}

void CGxServiceModelUtilUi::recurTravDir( const QString &strPath, QStandardItem *pParentItem )
{
	QDir dir(strPath);
	QStringList filters;
	filters << "*.shp" << "*.img" << "*.kml" << "*.tif" << "*.000";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);

	for (int nSize = 0; nSize < fileInfoList.size(); ++nSize)
	{
		QFileInfo fileInfo = fileInfoList.at(nSize);

		// 如果是目录
		if (fileInfo.isDir())
		{
			QString strName = fileInfo.absoluteFilePath();
			QDir tempDir = QDir(fileInfo.absoluteFilePath());
			QFileInfoList fileInfoList = tempDir.entryInfoList(filters, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
			// 如果目录不为空
			if (fileInfoList.size())
			{
				QStandardItem *pChildItem = new QStandardItem;
				storageData(LAYER_SET_ITEM, LOCAL_SERVER, pChildItem);
				pChildItem->setFlags(pChildItem->flags() & ~Qt::ItemIsEditable);
				pChildItem->setText(fileInfo.fileName());
				pChildItem->setData(fileInfo.absolutePath(), FilePathRole);
				pChildItem->setData(FOLDERLINK, LinkRole);
				pChildItem->setIcon(QIcon(":/Resources/images/wenjianjia.png"));
				pParentItem->appendRow(pChildItem);
				recurTravDir(fileInfo.absoluteFilePath(), pChildItem);
			}
			else
			{
				QStandardItem *pChildItem = new QStandardItem;
				storageData(LAYER_SET_ITEM, LOCAL_SERVER, pChildItem);
				pChildItem->setFlags(pChildItem->flags() & ~Qt::ItemIsEditable);
				pChildItem->setText(fileInfo.fileName());
				pChildItem->setData(fileInfo.absolutePath(), FilePathRole);
				pChildItem->setData(FOLDERLINK, LinkRole);
				pChildItem->setIcon(QIcon(":/Resources/images/wenjianjia.png"));
				pParentItem->appendRow(pChildItem);
			}
		}
		// 如果是文件
		else
		{
			QStandardItem *pChildItem = new QStandardItem;
			storageData(LAYER_ITEM, LOCAL_SERVER, pChildItem);
			pChildItem->setFlags(pChildItem->flags() & ~Qt::ItemIsEditable);
			pChildItem->setText(fileInfo.fileName());
			pChildItem->setIcon(QIcon(":/Resources/images/tuceng.png"));
			pChildItem->setData(fileInfo.absolutePath(), FilePathRole);
			pChildItem->setData(FILELINK, LinkRole);
			pParentItem->appendRow(pChildItem);
		}
	}
}

void CGxServiceModelUtilUi::storageServerProData( const SServerPropertyStruct & stServerPropertyStruct, QStandardItem *pItem )
{
	if (pItem)
	{
		QVariant var;
		var.setValue(stServerPropertyStruct); // copy s into the variant
		pItem->setData(var, ServerProRole);
	}
}

void CGxServiceModelUtilUi::storageLayerProData( const SLayerPropertyStruct & stLayerPropertyStruct, QStandardItem *pItem )
{
	if (pItem)
	{
		QVariant var;
		var.setValue(stLayerPropertyStruct); // copy s into the variant
		pItem->setData(var, LayerProRole);
	}
}
