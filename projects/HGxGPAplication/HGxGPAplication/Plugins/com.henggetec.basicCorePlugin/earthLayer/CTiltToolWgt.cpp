#if WIN32
#include <io.h>
#else
#include <dirent.h>
#endif
#include <QFileDialog>
#include <QFile>
#include <qscrollbar.h>

#include <osg/Group>
#include <osg/PagedLOD>
#include <osgDB/ReadFile>
#include <osg/ValueObject>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>

#if (WIN32)
#include <gdal/ogr_spatialref.h>
#include <gdal/ogrsf_frmts.h>
#else
#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>
#endif

//#include <ICore.h>
//#include <CToolManager.h>
//#include <CMainWindowManager.h>
//#include <scene/IFxSceneService.h>
//#include <FreeXCommonUtils/tinyXml/tinyxml.h>
//#include <FreeXCommonUtils/tinyXml/tinyutil.h>
//#include <FreeXCommonUtils/PathRegistry.h>
//#include <scene/SceneServiceUtil.h>
//#include <FreeXFramework/FreeXFramework.h>
#include <earth_layer_ui/IEarthLayerUiService.h>
#include <scene_ui/ISceneUiService.h>
#include <FxMessageBox/CMessageBox.h>

#include <QTextCodec>

#include "CTiltToolWgt.h"

#include <bdpCommon/GxStrUtil.h>
#include <bdpDB/GxRegister.h>
#include <bdpDB/GxTinyXml.h>

#include <gdpScene/layer/GxLayerLive.h>
#include <gdpScene/layer/GxLayerManager.h>
#include <gdpScene/layer/GxLayerConfig.h>
#include <gdpScene/layer/GxLayerConfigFactory.h>

#include "scene_ui/ISceneUiService.h"

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

CTiltToolWgt::CTiltToolWgt(QWidget *parent)
	: CFxBaseWindow(parent)
{
	//读样式表文件
	QFile file(":/qss_earth_layer/Resources/qss_earth_layer/earthlayerproperty.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = file.readAll();
		m_pCentralWgt->setStyleSheet(styleSheet().append(str));
		file.close();
	}
	ui.setupUi(m_pCentralWgt);
	setWindowModality(Qt::WindowModal);
	setWindowTitle(tr("tilt photograph format conversion"));
	setResizable(false);
	QMetaObject::connectSlotsByName(this);
	ui.m_pPathToolBtn->setToolTip(tr("browse"));
	QRegExp rx("\\S{0,20}");
	ui.m_pNameEdit->setValidator(new QRegExpValidator(rx, this));
	initWidgetInfo();
	initSignalAndSlot();

	
}

CTiltToolWgt::~CTiltToolWgt()
{
}

void CTiltToolWgt::initWidgetInfo()
{
	m_bIsLLH = true;
	m_bIsPixs = true;
	m_bIsOpenAdvance = false;
	m_strPath = "";
	ui.m_pAdvanceWidget->hide();
	setMinimumHeight(185);
	resize(450,185);
	ui.m_pChangeAxisLabel->setText(tr("geographical coordinates"));
	ui.stackedWidget->setCurrentIndex(0);
}

void CTiltToolWgt::initSignalAndSlot()
{

	connect(ui.m_pPixelPushBtn, SIGNAL(clicked(bool)), this, SLOT(pixelPushBtnSlot(bool)));
	connect(ui.m_pDistancePushBtn, SIGNAL(clicked(bool)), this, SLOT(distancePushBtnSlot(bool)));
	connect(ui.m_pAdvancedPushBtn, SIGNAL(clicked()), this, SLOT(advancedPushBtnChangedSlot()));
	connect(ui.m_pChangeAxisBtn, SIGNAL(clicked()), this, SLOT(changeAxisBtnChangedSlot()));
	connect(ui.m_pPathToolBtn, SIGNAL(clicked()), this, SLOT(pathBtnChangedSlot()));
	connect(ui.m_pSaveBtn, SIGNAL(clicked()), this, SLOT(saveBtnClickedSlot()));
	connect(this, SIGNAL(closeSignals()), this, SLOT(onCloseWindowSlot()));
	connect(ui.m_pCancelBtn, SIGNAL(clicked()), this, SLOT(onCloseWindowSlot()));
	connect(ui.m_pPathLineEdit, SIGNAL(textEdited(QString)), this, SLOT(pathChangedSlot(QString)));
}

void CTiltToolWgt::pathChangedSlot(QString path)
{
	if(path == m_strPath)
	{
		return;
	}
	QDir dir(path);
	if(!dir.exists())
	{
		return;
	}
	m_strPath = path;
	analysisFilePath(path);
}

void CTiltToolWgt::pathBtnChangedSlot()
{
	BrowseDataPath();
}

void CTiltToolWgt::BrowseDataPath()
{
	// 读取数据路径
	QString strCaption = tr("data path");

	QString strDir = tr(HGX_FILE_CONFIG().getDataPath().c_str());

#if WIN32
	QString filePath = QFileDialog::getExistingDirectory(this, strCaption, strDir);
#else
	QString filePath = QFileDialog::getExistingDirectory(0, strCaption, strDir);
#endif

	if (!filePath.isEmpty())
	{
		m_strPath = filePath;
		ui.m_pPathLineEdit->setText(filePath);
		analysisFilePath(filePath);
	}
}

void  CTiltToolWgt::analysisFilePath(const QString filePath) 
{
	
	//查找metadata.xml文件 metadata.xml在父目录中
	{
		QString strXmlName = filePath + "/../metadata.xml";
		QFile file(strXmlName);
		if(file.exists())
		{
			analysisXmlFile(QTextCodec::codecForName("GB2312")->fromUnicode(strXmlName).data());
			return;
		}
	}


	QDir dir(filePath);
	if(!dir.exists())
	{
		return;
	}
	dir.setFilter(QDir::Files | QDir::NoSymLinks);

	//查找scp文件
	{
		dir.setNameFilters(QStringList()<<QString("*.scp")); //设置文件名称过滤器，只为filters格式（后缀为.scp格式）
		QFileInfoList list =dir.entryInfoList();
		if(0 < list.count())
		{
			QString strScpName = filePath + "/" + list.at(0).fileName();
			analysisScpFile(QTextCodec::codecForName("GB2312")->fromUnicode(strScpName).data());
			return;
		}
	}
	//查找lfp文件
	{
		dir.setNameFilters(QStringList()<<QString("*.lfp")); //设置文件名称过滤器，只为filters格式（后缀为.lfp格式）
		QFileInfoList list =dir.entryInfoList();
		if(0 < list.count())
		{
			QString strLfpName = filePath + "/" + list.at(0).fileName();
			analysisLfpFile(QTextCodec::codecForName("GB2312")->fromUnicode(strLfpName).data());
			return;
		}
	}
}

void CTiltToolWgt::analysisXmlFile(const char* strXmlName)
{
 	//解析metadata.xml中的经纬度(固定格式)
 	HGxDB::CGxTiXmlDocument  document;
 	if(!document.LoadFile(HGxCommon::utf8ToCurCode(strXmlName).c_str(), HGxDB::E_TIXML_ENCODING_UTF8))
 	{
 		return;
 	}
	HGxDB::CGxTiXmlElement* eleModel = document.FirstChildElement("ModelMetadata");
 	if(!eleModel)
 	{
 		return;
 	}
	HGxDB::CGxTiXmlElement* eleSRS = eleModel->FirstChildElement("SRS");
 	if(!eleSRS)
 	{
 		return;
 	}

 	//解析出EPSG(2019-9-27 w00224)
 	QString strText  = QString("%1").arg(eleSRS->GetText());
 	QStringList strList = strText.split(":");
 	OGRSpatialReference inRs, outRs;
 	if (2 == strList.count())
 	{
 		if (!strcmp(strList.at(0).toStdString().c_str(), "EPSG"))
 		{
 			int test = strList[1].toInt();
 			inRs.importFromEPSG(strList[1].toInt());
 		}
 		//ENU格式保存的就是经纬
 		else if (!strcmp(strList.at(0).toStdString().c_str(), "ENU"))
 		{
 			QStringList enuStrList = strList.at(1).split(",");
 			if (2 == enuStrList.count())
 			{
 				ui.m_pLatSpx->setValue(enuStrList.at(0).toDouble());
 				ui.m_pLonSpx->setValue(enuStrList.at(1).toDouble());
 
 				return;
 			}
 		}
 		else
 			return;
 	}
 	//内部使用的坐标是WGS84即是对应的EPSG:4326
 	outRs.importFromEPSG(4326);
 
 	//解析出坐标
	HGxDB::CGxTiXmlElement* eleSRSOrigin = eleModel->FirstChildElement("SRSOrigin");
 	if(!eleSRSOrigin)
 	{
 		return;
 	}
 	strText  = QString("%1").arg(eleSRSOrigin->GetText());
 	strList = strText.split(",");
 	double dx = 0.0, dy = 0.0;
 	if (3 == strList.count())
 	{
 		dx = strList.at(0).toDouble();
 		dy = strList.at(1).toDouble();
 	}
 
 	//进行转化
 	OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation(&inRs, &outRs);
 	if (poCT)
 	{
 		poCT->Transform(1, &dx, &dy);
 		ui.m_pLonSpx->setValue(dx);
 		ui.m_pLatSpx->setValue(dy);
 	}
}

void CTiltToolWgt::analysisScpFile(const char* strScpName)
{
 	//解析metadata.xml中的经纬度(固定格式)
 	HGxDB::CGxTiXmlDocument document;
 	if(!document.LoadFile(HGxCommon::utf8ToCurCode(strScpName).c_str(), HGxDB::E_TIXML_ENCODING_UTF8))
 	{
 		return;
 	}
	HGxDB::CGxTiXmlElement* eleModel = document.FirstChildElement("SuperMapCache_Unicode");
 	if(!eleModel)
 	{
 		return;
 	}
	HGxDB::CGxTiXmlElement* eleSRS = eleModel->FirstChildElement("sml:Position");
 	if(!eleSRS)
 	{
 		return;
 	}
	HGxDB::CGxTiXmlElement* eleX = eleSRS->FirstChildElement("sml:X");
 	if(!eleX)
 	{
 		return;
 	}
	HGxDB::CGxTiXmlElement* eleY = eleSRS->FirstChildElement("sml:Y");
 	if(!eleY)
 	{
 		return;
 	}
	HGxDB::CGxTiXmlElement* eleZ = eleSRS->FirstChildElement("sml:Z");
 	if(!eleZ)
 	{
 		return;
 	}
 
 	QString strTextX  = QString("%1").arg(eleX->GetText());
 	QString strTextY  = QString("%1").arg(eleY->GetText());
 	QString strTextZ  = QString("%1").arg(eleZ->GetText());
 
 	ui.m_pLonSpx->setValue(strTextX.toDouble());
 	ui.m_pLatSpx->setValue(strTextY.toDouble());
 	ui.m_pHeightSpx->setValue(strTextZ.toDouble());
}

void CTiltToolWgt::analysisLfpFile(const char* strLfpName)
{
 	//解析metadata.xml中的经纬度(固定格式)
	HGxDB::CGxTiXmlDocument document;

 	if(!document.LoadFile(HGxCommon::utf8ToCurCode(strLfpName).c_str(), HGxDB::E_TIXML_ENCODING_UTF8))
 	{
 		return;
 	}
 	HGxDB::CGxTiXmlElement* eleModel = document.FirstChildElement("DataDefine");
 	if(!eleModel)
 	{
 		return;
 	}
 	//解析位置
 	{
		HGxDB::CGxTiXmlElement* elePosition = eleModel->FirstChildElement("Position");
 		if(!elePosition)
 		{
 			return;
 		}
 		QString strText  = QString("%1").arg(elePosition->GetText());
 		QStringList strList = strText.split(",");
 		if(3 == strList.count())
 		{
 			ui.m_pLonSpx->setValue(strList.at(0).toDouble());
 			ui.m_pLatSpx->setValue(strList.at(1).toDouble());
 			ui.m_pHeightSpx->setValue(strList.at(2).toDouble());
 		}
 	}
 	//解析旋转
 	{
		HGxDB::CGxTiXmlElement* eleRotation = eleModel->FirstChildElement("Rotation");
 		if(!eleRotation)
 		{
 			return;
 		}
 		QString strText  = QString("%1").arg(eleRotation->GetText());
 		QStringList strList = strText.split(",");
 		ui.m_pAngleSpx->setValue(strList.at(0).toDouble());
 	}
 	//解析缩放
 	{
		HGxDB::CGxTiXmlElement* eleScale = eleModel->FirstChildElement("Scale");
 		if(!eleScale)
 		{
 			return;
 		}
 		QString strText  = QString("%1").arg(eleScale->GetText());
 		QStringList strList = strText.split(",");
 		ui.m_pScaleSpx->setValue(strList.at(0).toDouble());
 	}
}

void genTiltModel(osg::ref_ptr<osg::Group> root, const QList <std::string>& tiltList
	, const std::string& sourceDir, const double& minSpx, const double& maxSpx
	, const osg::LOD::RangeMode& rangeMode)
{
	for (int i = 0; i < tiltList.size(); ++i)
	{
		osg::ref_ptr<osg::PagedLOD> pLod = new osg::PagedLOD;
		pLod->setRangeMode(rangeMode);

		osg::ref_ptr<osg::Node> pNode = osgDB::readNodeFile(sourceDir + "/" + tiltList.at(i));
		osg::PagedLOD* pagelod = dynamic_cast<osg::PagedLOD*>(pNode.get());

		if (pagelod)
		{
			osg::Vec3 center = pagelod->getCenter();
			float radius = pagelod->getRadius();

			if (radius <= 0)
			{
				osg::BoundingSphere bs = pNode->computeBound();
				center = bs.center();
				radius = bs.radius();
			}

			pLod->setCenter(center);
			pLod->setRadius(radius);
			pLod->setFileName(0, tiltList.at(i));
			pLod->setRange(0, minSpx, maxSpx);
			
			pLod->setName(tiltList.at(i));
			root->addChild(pLod);
		}
	}
}

void CTiltToolWgt::saveBtnClickedSlot()
{
	if(m_strPath.isEmpty())
	{
		/**************此处需要主窗口******************/
		int ret = CMessageBox::information(tr("please select data path")/*,ICore::mainWindowManager()->mainWindow()*/);
		return;
	}
	std::string strPath = m_strPath.toUtf8().data();
	QList <std::string> tiltList = getOsgbFileNames(); 

	osg::ref_ptr<osg::Group> group = new osg::Group;
	if(m_bIsPixs)
	{
		genTiltModel(group, tiltList, strPath
			, ui.m_pMinSpx->value(), ui.m_pMaxSpx->value()
			, osg::LOD::PIXEL_SIZE_ON_SCREEN);

		group->setUserValue("pixelModel", true);
	}
	else
	{
		genTiltModel(group, tiltList, strPath
			, ui.m_pMinSpx_2->value(), ui.m_pMaxSpx_2->value()
			, osg::LOD::DISTANCE_FROM_EYE_POINT);

		group->setUserValue("pixelModel", false);
	}

	osg::ref_ptr<osgDB::Options> pOpt = new osgDB::Options();
	pOpt->setDatabasePath("");//必须设为""
	group->setUserValue("srs", m_bIsLLH);//是否是地理坐标系
	
	if(m_bIsLLH)
	{
		group->setUserValue("Position", osg::Vec3d(ui.m_pLonSpx->value(), ui.m_pLatSpx->value(), ui.m_pHeightSpx->value()));
		group->setUserValue("Scale", osg::Vec3d(ui.m_pScaleSpx->value(), ui.m_pScaleSpx->value(), ui.m_pScaleSpx->value()));
		group->setUserValue("Rotate", osg::Vec3d(ui.m_pAngleSpx->value(), ui.m_pAngleSpx->value(), ui.m_pAngleSpx->value()));
	}
	else
	{
		group->setUserValue("Height", ui.m_pHighSpx->value());
	}
	QString  strOutFileName = m_strPath +"/"+ui.m_pNameEdit->text()+".fgt";
    bool bSuccess = osgDB::writeNodeFile(*group, strOutFileName.toUtf8().data(), pOpt);
	if(bSuccess)//转换成功询问是否添加到场景中
	{
		hide();

		/**************此处需要主窗口******************/
		int ret = CMessageBox::question(tr("conversion completed,whether to add into the scene")/*,ICore::mainWindowManager()->mainWindow()*/);

		if (ret == CMessageBox::Confirm)
		{
			std::string strSceneID = "";

			ISceneUiService* pSceneService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
			if (pSceneService)
			{
				strSceneID = pSceneService->activeScene();
			}

			HGxScene::CGxLayerConfig* pLayerCfg = HGxScene::CGxLayerManager::getOrCreateLayerManager(strSceneID)->layerConfigFactory()->createLiveLayerConfig();

			HGxScene::CGxLayerLive* pLiveLayer = static_cast<HGxScene::CGxLayerLive*>(HGxScene::CGxLayerManager::getOrCreateLayerManager(strSceneID)->createLayer(*pLayerCfg));

			if (pLiveLayer)
			{
				pLiveLayer->setName((ui.m_pNameEdit->text()).toUtf8().data());
				pLiveLayer->setTiltType(HGxScene::CGxLayerLive::E_TILT_TYPE_FILEPATH);

				if (pLiveLayer->setTiltPath(strOutFileName.toUtf8().data()))
				{
					HGxScene::CGxLayerManager::getOrCreateLayerManager(strSceneID)->addLayer(pLiveLayer);
				}
				else
				{
					/***************此处需要主窗口******************/
					CMessageBox::information(tr("model load failed!")/*, ICore::mainWindowManager()->mainWindow()*/);
				}
			}
		}
	}
	else//转换失败
	{
		/**************此处需要主窗口******************/
		int ret = CMessageBox::information(tr("conversion failed")/*,ICore::mainWindowManager()->mainWindow()*/);
	}
	emit closeSignals();
}

QList<std::string> CTiltToolWgt::getOsgbFileNames()
{
	//c的库函数 路径需要用本地编码
	std::string str = m_strPath.toLocal8Bit().data();
	//osgDB的函数 由于启用了use_utf_filename 需要使用utf8编码
	std::string strUtf = m_strPath.toUtf8().data();
	QList <std::string> tiltList;
#ifdef WIN32
	// 遍历目录
	_finddata_t file_info;
	std::string current_path = str + "/*";
	int handle = _findfirst(current_path.c_str(), &file_info);
	if (-1 == handle)
	{
		return tiltList;
	}

	do
	{
		// 判断是否是子目录
		if (file_info.attrib == _A_SUBDIR) //是目录
		{
			// 忽略“.”和“..”目录
			if (strcmp(file_info.name, ".") !=0 && strcmp(file_info.name, "..")!=0)
			{
				std::string nodePath = "";
				nodePath += "/";
				nodePath += file_info.name;
				nodePath += "/";
				nodePath += file_info.name;
				nodePath += ".osgb";

				// 判断文件是否存在
				if (osgDB::fileExists(strUtf + nodePath))
				{
					tiltList.push_back(nodePath);
				}
			}
		}
	}
	while (!_findnext(handle, &file_info));
	_findclose(handle);
#else
	struct stat s_buf;

	// 获取文件信息，把信息存储到s_buf中
    stat(str.c_str(), &s_buf);

	// 判断文件路径是否是目录
	if ( !S_ISDIR(s_buf.st_mode) )
	{
		return tiltList;
	}

	struct dirent *fileName;
	DIR *dir;
    dir = opendir(str.c_str());
	if ( NULL == dir )
	{
		return;
	}

	while ( (fileName = readdir(dir)) != NULL )
	{
		if ( strcmp(fileName->d_name, ".") == 0 || strcmp(fileName->d_name, "..") == 0 )
		{
			continue;
		}

		char cFilePath[200];
		bzero(cFilePath, 200);
		strcat(cFilePath, m_strPath.toStdString().c_str());
		strcat(cFilePath, "/");
		strcat(cFilePath, fileName->d_name);

		// 获取文件信息，把信息存储到s_buf中
		stat(cFilePath, &s_buf);

		// 判断文件是否是目录
		if (S_ISDIR(s_buf.st_mode))
		{
			std::string nodePath = "";
			nodePath += "/";
			nodePath += fileName->d_name;
			nodePath += "/";
			nodePath += fileName->d_name;
			nodePath += ".osgb";
			// 判断文件是否存在
			if (osgDB::fileExists(m_strPath.toStdString() + nodePath))
			{
				tiltList.push_back(nodePath);
			}
		}
	}
#endif
	return tiltList;
}

void CTiltToolWgt::advancedPushBtnChangedSlot()
{
	m_bIsOpenAdvance = !m_bIsOpenAdvance;
	if(m_bIsOpenAdvance)
	{
		ui.m_pAdvanceWidget->show();
		if(m_bIsLLH)
		{
			setMinimumHeight(461);
			resize(width(),461);
		}
		else
		{
			setMinimumHeight(348);
			resize(width(),348);
		}
	}
	else
	{
		ui.m_pAdvanceWidget->hide();
		setMinimumHeight(185);
		resize(width(),185);
	}
}

void CTiltToolWgt::changeAxisBtnChangedSlot()
{
	//还原像素或视距选中状态
	if(m_bIsPixs)
	{
		ui.m_pPixelPushBtn->setChecked(true);
	}
	else
	{
		ui.m_pDistancePushBtn->setChecked(true);
	}
	//设置坐标系
	m_bIsLLH = !m_bIsLLH;
	if(m_bIsLLH)
	{
		ui.m_pChangeAxisLabel->setText(tr("geographical coordinates"));
		ui.stackedWidget->setCurrentIndex(0);
		ui.stackedWidget->setMaximumHeight(130);
		setMinimumHeight(461);
		resize(width(),461);

	}
	else
	{
		ui.m_pChangeAxisLabel->setText(tr("world coordinates"));
		ui.stackedWidget->setCurrentIndex(1);
		ui.stackedWidget->setMaximumHeight(22);
		setMinimumHeight(348);
		resize(width(),348);

	}
}
void CTiltToolWgt::pixelPushBtnSlot(bool b)
{
	if(b)
	{
		ui.m_pStackedWgt->setCurrentIndex(1);
	}
	m_bIsPixs = b;

}
void CTiltToolWgt::distancePushBtnSlot(bool b)
{
	if(b)
	{
		ui.m_pStackedWgt->setCurrentIndex(0);
	}
	m_bIsPixs = !b;
}