#include "CDragTreeView.h"
#include <QStandardItem>
#include <QApplication>
#include <QFileInfo>
#include <QLineEdit>

#include <FxMessageBox/CMessageBox.h>
#include "layer_ui/CLayerManager.h"
#include "earth_layer_ui/IEarthLayerUiService.h"
#include "map_layer_ui/IMapLayerUiService.h"
#include "scene_ui/ISceneUiService.h"
#include "CLayerTypeSelectWidget.h"
#include "public/CSceneDndManager.h"
#include "CSceneDndManager_p.h"
#include <QDrag>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxUiFrameworkServiceInterface.h>

#include <gdpScene/CommonGeo.h>
#include <gdpScene/layer/GxLayerManager.h>

CDragTreeView::CDragTreeView(QWidget *parent) : QTreeView(parent)
	, m_bIsLocalFile(false)
{
	this->setAcceptDrops(false);
	this->setDragEnabled(false);

	CSceneDndManagerPrivate::instance()->registerHandler(this);
}


CDragTreeView::~CDragTreeView()
{
	CSceneDndManagerPrivate::instance()->unRegisterHandler(this);
}


 void CDragTreeView::handleDrop(std::string, const QPoint &)
 {
  	if (selectionModel())
  	{
  		selectionModel()->clearSelection();
  		selectionModel()->select(currentIndex(), QItemSelectionModel::ClearAndSelect);
  	}
  	clearFocus();
  
  	QString strLayerName = m_strLayerName;
  	QString strDriveType = m_strDriveType;
  	QString strUrl = m_strUrl;
  	bool bIsLocalFile = m_bIsLocalFile;
  	int iLayerType = -1;
  
  	CLayerConfig::LayerType layerType = (CLayerConfig::LayerType)m_iLayerType;
  
  	// 当前场景类型和场景id
	ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());

	if (NULL == pSceneService)
	{
		return;
	}

	CGxUiFrameworkServiceInterface* pUiService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	if (NULL == pUiService)
	{
		return;
	}
  
  	if (pSceneService->activeSceneType() == GX_EARTH_SCENE_TYPE)
  	{
  		if (bIsLocalFile)
  		{
  			// 获取图层服务（具体添加到的场景id是在图层插件中指定的）
  			IEarthLayerUiService *pLayerService = HGXPFCoreInstance.getService<IEarthLayerUiService>(HGxFWLauncherInstance.getPluginContext());
  			
  			QFileInfo fileInfo(strUrl);
  
  			if (pLayerService == nullptr)
  			{
  				CMessageBox::information(tr("add filed,layer plugin not load"), pUiService->mainWindow());
  				return;
  			}
  			switch (layerType)
  			{
  				// 矢量图层直接添加
  			case CLayerConfig::VectorLayer:
  				{
  					pLayerService->addVectorLayer(fileInfo.fileName(), strUrl, strDriveType);
  				}
  				break;
  			case CLayerConfig::ImageLayer:
  				{
  					pLayerService->addImageLayer(fileInfo.fileName(), strUrl, strDriveType);
  				}
  				break;
  			case CLayerConfig::ElevationLayer:
  				{
  					pLayerService->addEleLayer(fileInfo.fileName(), strUrl, strDriveType);
  				}
  				break;
  			case CLayerConfig::SeaLayer: /// 海图图层 add by yq
  				{
  					pLayerService->addSeaLayer(fileInfo.fileName(),strUrl);
  				}
  				break;
  				// 选择添加到影像还是高程
  			case CLayerConfig::UndefinedLayer:
  				{
					CLayerTypeSelectWidget::ReturnButton retBtn = CLayerTypeSelectWidget::warning(pUiService->mainWindow());
					if (retBtn == CLayerTypeSelectWidget::No)
					{
						return;
					}

  					// 添加影像
  					if (CLayerTypeSelectWidget::m_layerType == CLayerConfig::ImageLayer)
  					{
  						pLayerService->addImageLayer(fileInfo.fileName(), strUrl, strDriveType);
  					}
  					else if(CLayerTypeSelectWidget::m_layerType == CLayerConfig::ElevationLayer)
  					{
  						pLayerService->addEleLayer(fileInfo.fileName(), strUrl, strDriveType);
  					}
  				}
  				break;
  			}
  
  		}
  		else
  		{
  			IEarthLayerUiService *pLayerService = HGXPFCoreInstance.getService<IEarthLayerUiService>(HGxFWLauncherInstance.getPluginContext());
  			QFileInfo fileInfo(strUrl);
  			if (pLayerService == nullptr)
  			{
  				CMessageBox::information(tr("add filed,layer plugin not load"), pUiService->mainWindow());
  				return;
  			}
  			if (layerType == CLayerConfig::VectorLayer)
  			{
  				pLayerService->addVectorLayer(strLayerName, strUrl, strDriveType, false);
  			}
  			else if (layerType == CLayerConfig::ImageLayer)
  			{
  				pLayerService->addImageLayer(strLayerName, strUrl, strDriveType, false, m_strFormat);
  			}
  			else if(layerType == CLayerConfig::ElevationLayer)
  			{
  				pLayerService->addEleLayer(strLayerName, strUrl, strDriveType, false);
  			}
  		}
  	}
  	else if (pSceneService->activeSceneType() == GX_MAP_SCENE_TYPE)
  	{
  		// 获取图层服务
  		IMapLayerUiService *pLayerService = HGXPFCoreInstance.getService<IMapLayerUiService>(HGxFWLauncherInstance.getPluginContext());

  		if (pLayerService == nullptr)
  		{
  			CMessageBox::information(tr("add filed,layer plugin not load"), pUiService->mainWindow());
  			return;
  		}
  
  		if (bIsLocalFile)
  		{
  			// 添加本地
  			pLayerService->addMapLayerFromLocalFile(strUrl);
  		}
  		else
  		{
  			// 添加服务
  			pLayerService->addMapLayerFromService(strUrl, strLayerName, strDriveType, layerType, m_strFormat);
  		}
  	}
 }
 
 QString CDragTreeView::mimeType() const
 {
 	return "fxgis/layer_info";
 }

void CDragTreeView::mousePressEvent( QMouseEvent *event )
{
	if (event->button() == Qt::LeftButton)
	{
		m_startPos = event->pos();
	}

	QTreeView::mousePressEvent(event);
}

void CDragTreeView::mouseMoveEvent( QMouseEvent *event )
{
	int iDistance = (event->pos() - m_startPos).manhattanLength();

	if (iDistance >= QApplication::startDragDistance())
	{
		QStandardItemModel *pItemModel = (QStandardItemModel *)model();
		QStandardItem *pCurrentItem = pItemModel->itemFromIndex(currentIndex());
		QPointF hotSpot = event->pos() - m_startPos;

		if (pCurrentItem)
		{
			if (pCurrentItem->data(ItemTypeRole).toInt() == LAYER_ITEM)
			{
				if (pCurrentItem->data(ItemServerTypeRole) == LOCAL_SERVER)
				{
					QString strAbsoluteFilePath = QString(pCurrentItem->data(FilePathRole).toString() + "/" + pCurrentItem->text());
					m_strUrl = strAbsoluteFilePath;
					QFileInfo fileInfo = QFileInfo(m_strUrl);	
					QString strFileSuffix = fileInfo.suffix();

					if (fileInfo.suffix() == "shp" || fileInfo.suffix() == "kml")
					{
						m_strDriveType = "ogr";
						m_iLayerType = CLayerConfig::VectorLayer;
					}
					else if (fileInfo.suffix() == "tif" )
					{
						m_strDriveType = "gdal";
						m_iLayerType = CLayerConfig::UndefinedLayer;
					}
					else if ( fileInfo.suffix() == "img")
					{
						m_strDriveType = "gdal";
						m_iLayerType = CLayerConfig::UndefinedLayer;
					}
					else if (fileInfo.suffix() == "000")
					{
						m_strDriveType = "s57";
						m_iLayerType = CLayerConfig::SeaLayer;
					}
					else
					{
						return;
					}
					m_bIsLocalFile = true;
					m_strLayerName = fileInfo.baseName();
				}
				else
				{
					QVariant var = pCurrentItem->data(LayerProRole);
					SLayerPropertyStruct stInfo;
					stInfo = var.value<SLayerPropertyStruct>();
					m_strLayerName = stInfo.strLayerName;
					m_strDriveType = stInfo.strDriveName;
					m_strUrl = stInfo.strServerWebsite;
					m_strFormat = stInfo.strLayerFormat;

					if (stInfo.strLayerType == tr("image layer"))
					{
						m_iLayerType = CLayerConfig::ImageLayer;
					}
					else if (stInfo.strLayerType == tr("elevation layer"))
					{
						m_iLayerType = CLayerConfig::ElevationLayer;
					}
					else if (stInfo.strLayerType == tr("vector layer"))
					{
						m_iLayerType = CLayerConfig::VectorLayer;
					}
					else
					{
						m_iLayerType = CLayerConfig::UndefinedLayer;
					}

					m_bIsLocalFile = false;
				}

				QMimeData *mimeData = new QMimeData;
				QByteArray ba;
				mimeData->setData("fxgis/layer_info", ba);
				mimeData->setText(pCurrentItem->text());

				QDrag *drag = new QDrag(this);
				drag->setMimeData(mimeData);
				QPixmap pixmap = QPixmap(":/Resources/images/drag.png");
				drag->setHotSpot(QPoint(15.5, 13.5));
				drag->setPixmap(pixmap);
				Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction) ;
			}	
		}
	}
	QTreeView::mouseMoveEvent(event);
}

void CDragTreeView::mouseReleaseEvent( QMouseEvent *event )
{
	QTreeView::mouseReleaseEvent(event);
}

