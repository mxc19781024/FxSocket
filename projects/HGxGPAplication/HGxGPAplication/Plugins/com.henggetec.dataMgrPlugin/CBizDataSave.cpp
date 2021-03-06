#include <CBizDataSave.h>
#include <QFile>
#include <QIODevice>
#include <QDomDocument>
#include <QList>
#include <FreeXFramework/FxServiceQuerier.h>
#include <FreeXFramework/IFxSimulationTimer.h>

CBizDataSave::CBizDataSave(std::string strFilePath)
	:m_strFileName(strFilePath)
	,m_pBizUnitService(NULL)
{
}

CBizDataSave::~CBizDataSave()
{
}

void CBizDataSave::SetFilePath(std::string strFilePath)
{
	m_doc.clear();
	m_strFileName = strFilePath;
}

void CBizDataSave::run()
{
	if (!m_strFileName.empty())
	{
		QFile file(m_strFileName.c_str());
		if (file.open(QIODevice::WriteOnly))
		{	
			QDomElement root = m_doc.createElement("ScenarioInfo");
			m_doc.appendChild(root);
			elementCreate(root);

			QTextStream out(&file);
			out.setCodec("UTF-8");
			m_doc.save(out,4,QDomNode::EncodingFromTextStream);

			file.close();
		}	
	}
}

void CBizDataSave::elementCreate( QDomElement ele )
{
	QDomElement stEle = m_doc.createElement("ScenarioBeginTime");
	ele.appendChild(stEle);
	stEle.appendChild(m_doc.createTextNode(createTimeText(FxSimulationTimerInstance()->getLogicTimeBegin())));
	createObjectList(ele);
}

void CBizDataSave::createObjectList( QDomElement ele )
{
	if (getService())
	{
		QDomElement stEleList = m_doc.createElement("ObjectList");
		ele.appendChild(stEleList);
		ID2EntityEleMap_r mapID2EntityEle = m_pBizUnitService->getObjectList();
		foreach(auto itr,mapID2EntityEle)
		{
			createObjectEle(stEleList,itr.second.get());	
		}
	}
}

void CBizDataSave::createObjectEle( QDomElement ele, FxEntityObject::CFxEntityObjectEle* pObjectEle )
{
	FxCore::CFxServiceQuerier<FxBizUnit::IFxBizUnitService> pBizUnitService;
	if (!pBizUnitService)
	{
		return;
	}
	auto pFactory = pBizUnitService->getFactory(pObjectEle->entityType());
	if (!pFactory)
	{
		return;
	}
	auto objectEle = pFactory->saveObjectEle(m_doc,pObjectEle);
	if (pObjectEle->entityClass().c_str() == FxEntityObject::ENTITY)
	{
		createEntityEle(objectEle,pObjectEle);//实体信息
	}
	objectEle.setAttribute("Type",pObjectEle->entityType().c_str());
	objectEle.setAttribute("Class",pObjectEle->entityClass().c_str());
	ele.appendChild(objectEle);
}

void CBizDataSave::createSeaEle( QDomElement ele, FxBizUnit::CFxSeaEle* pSeaEle )
{
	if (pSeaEle)
	{
		QDomElement seaEle = m_doc.createElement("Sea");
		ele.appendChild(seaEle);
		createEntityEle(seaEle,pSeaEle);
		createSeaLineEle(seaEle,pSeaEle);
	}
}

void CBizDataSave::createAircraftEle( QDomElement ele, FxBizUnit::CFxAircraftEle* pAircraftEle )
{
	if (pAircraftEle)
	{
		QDomElement airEle = m_doc.createElement("Aircraft");
		ele.appendChild(airEle);

		createEntityEle(airEle,pAircraftEle);
		createAirLineEle(airEle,pAircraftEle);
	}
}

void CBizDataSave::createGroundEle( QDomElement ele, FxBizUnit::CFxGroundEle* pGroundEle )
{
	if (pGroundEle)
	{
		QDomElement groundEle = m_doc.createElement("Ground");
		ele.appendChild(groundEle);

		createEntityEle(groundEle,pGroundEle);
	}
}

void CBizDataSave::createSpacecraftEle( QDomElement ele, FxBizUnit::CFxSpacecraftEle* pSpacecraftEle )
{
	if (pSpacecraftEle)
	{
		QDomElement spacecraftEle = m_doc.createElement("Spacecraft");//卫星组节点
		ele.appendChild(spacecraftEle);

		createEntityEle(spacecraftEle,pSpacecraftEle);//实体信息

		QDomElement orbitModeEle = m_doc.createElement("OrbitMode");//轨道模式
		spacecraftEle.appendChild(orbitModeEle);
		orbitModeEle.appendChild(m_doc.createTextNode(QString::number(pSpacecraftEle->orbitMode())));

		createSgp4DataEle(spacecraftEle,pSpacecraftEle);//Sgp4Data
	}
}

void CBizDataSave::createRectScanEle( QDomElement ele, FxBizUnit::CFxRectScanEle* pRectScanEle )
{
	QDomElement rectScanEle = m_doc.createElement("RectScan");
	ele.appendChild(rectScanEle);

	QDomElement idEle = m_doc.createElement("ID");
	rectScanEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pRectScanEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	rectScanEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pRectScanEle->nameEle()->text())));

	QDomElement lrEle = m_doc.createElement("AngleLR");
	rectScanEle.appendChild(lrEle);
	lrEle.appendChild(m_doc.createTextNode(QString::number(pRectScanEle->angleLR(),'f',1)));

	QDomElement fbEle = m_doc.createElement("AngleFB");
	rectScanEle.appendChild(fbEle);
	fbEle.appendChild(m_doc.createTextNode(QString::number(pRectScanEle->angleFB(),'f',1)));

	QDomElement lenEle = m_doc.createElement("Lenght");
	rectScanEle.appendChild(lenEle);
	lenEle.appendChild(m_doc.createTextNode(QString::number(pRectScanEle->lenght(),'f',1)));

	QDomElement faceColorEle = m_doc.createElement("FaceColor");
	rectScanEle.appendChild(faceColorEle);
	CreateVec4(faceColorEle,pRectScanEle->faceColor(),1);

	QDomElement lineColorEle = m_doc.createElement("LineColor");
	rectScanEle.appendChild(lineColorEle);
	CreateVec4(lineColorEle,pRectScanEle->lineColor(),1);
	
	QDomElement bindEle = m_doc.createElement("BindEntity");
	rectScanEle.appendChild(bindEle);
	bindEle.appendChild(m_doc.createTextNode(QString::fromStdString(pRectScanEle->entityID())));
}

void CBizDataSave::createConeScanEle( QDomElement ele, FxBizUnit::CFxConeScanEle* pConeScanEle )
{
	QDomElement coneScanEle = m_doc.createElement("ConeScan");
	ele.appendChild(coneScanEle);

	QDomElement idEle = m_doc.createElement("ID");
	coneScanEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pConeScanEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	coneScanEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pConeScanEle->nameEle()->text())));

	QDomElement angleEle = m_doc.createElement("Angle");
	coneScanEle.appendChild(angleEle);
	angleEle.appendChild(m_doc.createTextNode(QString::number(pConeScanEle->angle(),'f',1)));

	QDomElement lenEle = m_doc.createElement("Lenght");
	coneScanEle.appendChild(lenEle);
	lenEle.appendChild(m_doc.createTextNode(QString::number(pConeScanEle->lenght(),'f',1)));

	QDomElement faceColorEle = m_doc.createElement("FaceColor");
	coneScanEle.appendChild(faceColorEle);
	CreateVec4(faceColorEle,pConeScanEle->faceColor(),1);

	QDomElement lineColorEle = m_doc.createElement("LineColor");
	coneScanEle.appendChild(lineColorEle);
	CreateVec4(lineColorEle,pConeScanEle->lineColor(),1);

	QDomElement bindEle = m_doc.createElement("BindEntity");
	coneScanEle.appendChild(bindEle);
	bindEle.appendChild(m_doc.createTextNode(QString::fromStdString(pConeScanEle->entityID())));
}

void CBizDataSave::createRadarBeamEle( QDomElement ele, FxBizUnit::CFxRadarBeamEle* pRadarBeamEle )
{
	QDomElement radarBeamEle = m_doc.createElement("RadarBeam");
	ele.appendChild(radarBeamEle);

	QDomElement idEle = m_doc.createElement("ID");
	radarBeamEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pRadarBeamEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	radarBeamEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pRadarBeamEle->nameEle()->text())));

	QDomElement lenEle = m_doc.createElement("Lenght");
	radarBeamEle.appendChild(lenEle);
	lenEle.appendChild(m_doc.createTextNode(QString::number(pRadarBeamEle->lenght(),'f',1)));

	QDomElement azimuthEle = m_doc.createElement("Azimuth");
	radarBeamEle.appendChild(azimuthEle);
	azimuthEle.appendChild(m_doc.createTextNode(QString::number(pRadarBeamEle->azimuth(),'f',1)));

	QDomElement pitchEle = m_doc.createElement("Pitch");
	radarBeamEle.appendChild(pitchEle);
	pitchEle.appendChild(m_doc.createTextNode(QString::number(pRadarBeamEle->pitch(),'f',1)));

	QDomElement angleEle = m_doc.createElement("Angle");
	radarBeamEle.appendChild(angleEle);
	angleEle.appendChild(m_doc.createTextNode(QString::number(pRadarBeamEle->angle(),'f',1)));

	QDomElement faceColorEle = m_doc.createElement("FaceColor");
	radarBeamEle.appendChild(faceColorEle);
	CreateVec4(faceColorEle,pRadarBeamEle->faceColor(),1);

	QDomElement lineColorEle = m_doc.createElement("LineColor");
	radarBeamEle.appendChild(lineColorEle);
	CreateVec4(lineColorEle,pRadarBeamEle->lineColor(),1);

	QDomElement bindEle = m_doc.createElement("BindEntity");
	radarBeamEle.appendChild(bindEle);
	bindEle.appendChild(m_doc.createTextNode(QString::fromStdString(pRadarBeamEle->entityID())));
}

void CBizDataSave::createRadarScanEle( QDomElement ele, FxBizUnit::CFxRadarScanEle* pRadarScanEle )
{
	QDomElement radarScanEle = m_doc.createElement("RadarScan");
	ele.appendChild(radarScanEle);

	QDomElement idEle = m_doc.createElement("ID");
	radarScanEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pRadarScanEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	radarScanEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pRadarScanEle->nameEle()->text())));

	QDomElement yawEle = m_doc.createElement("AngleYaw");
	radarScanEle.appendChild(yawEle);
	yawEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->angleYaw(),'f',1)));

	QDomElement pitchEle = m_doc.createElement("AnglePitch");
	radarScanEle.appendChild(pitchEle);
	pitchEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->anglePitch(),'f',1)));

	QDomElement rollEle = m_doc.createElement("AngleRoll");
	radarScanEle.appendChild(rollEle);
	rollEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->angleRoll(),'f',1)));

	QDomElement radiusEle = m_doc.createElement("Radius");
	radarScanEle.appendChild(radiusEle);
	radiusEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->radius(),'f',1)));

	QDomElement loopTimeEle = m_doc.createElement("LoopTime");
	radarScanEle.appendChild(loopTimeEle);
	loopTimeEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->loopTime(),'f',1)));

	QDomElement azMinEle = m_doc.createElement("AzMin");
	radarScanEle.appendChild(azMinEle);
	azMinEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->azMin(),'f',1)));

	QDomElement azMaxEle = m_doc.createElement("AzMax");
	radarScanEle.appendChild(azMaxEle);
	azMaxEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->azMax(),'f',1)));

	QDomElement elevMinEle = m_doc.createElement("ElevMin");
	radarScanEle.appendChild(elevMinEle);
	elevMinEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->elevMin(),'f',1)));

	QDomElement elevMaxEle = m_doc.createElement("ElevMax");
	radarScanEle.appendChild(elevMaxEle);
	elevMaxEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->elevMax(),'f',1)));

	QDomElement elfinAzMaxEle = m_doc.createElement("ElfinAzMax");
	radarScanEle.appendChild(elfinAzMaxEle);
	elfinAzMaxEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->elfinAzMax(),'f',1)));

	QDomElement elfinElevMaxEle = m_doc.createElement("ElfinElevMax");
	radarScanEle.appendChild(elfinElevMaxEle);
	elfinElevMaxEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->elfinElevMax(),'f',1)));

	QDomElement colorEle = m_doc.createElement("Color");
	radarScanEle.appendChild(colorEle);
	CreateVec4(colorEle,pRadarScanEle->color(),1);

	QDomElement lineColorEle = m_doc.createElement("LineColor");
	radarScanEle.appendChild(lineColorEle);
	CreateVec4(lineColorEle,pRadarScanEle->lineColor(),1);

	QDomElement scanTypeEle = m_doc.createElement("ScanType");
	radarScanEle.appendChild(scanTypeEle);
	scanTypeEle.appendChild(m_doc.createTextNode(QString::number(pRadarScanEle->scanType())));

	QDomElement bindEle = m_doc.createElement("BindEntity");
	radarScanEle.appendChild(bindEle);
	bindEle.appendChild(m_doc.createTextNode(QString::fromStdString(pRadarScanEle->entityID())));
}

void CBizDataSave::createLinkLineEle( QDomElement ele, FxBizUnit::CFxLinkLineEle* pLinkLineEle )
{
	QDomElement linkLineEle = m_doc.createElement("LinkLine");
	ele.appendChild(linkLineEle);

	QDomElement idEle = m_doc.createElement("ID");
	linkLineEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pLinkLineEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	linkLineEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pLinkLineEle->nameEle()->text())));

	QDomElement spEle = m_doc.createElement("SegmentPoint");
	linkLineEle.appendChild(spEle);
	spEle.appendChild(m_doc.createTextNode(QString::number(pLinkLineEle->segmentPoint())));

	QDomElement snEle = m_doc.createElement("SegmentNum");
	linkLineEle.appendChild(snEle);
	snEle.appendChild(m_doc.createTextNode(QString::number(pLinkLineEle->segmentNum())));

	QDomElement colorEle = m_doc.createElement("Color");
	linkLineEle.appendChild(colorEle);
	CreateVec4(colorEle,pLinkLineEle->color(),1);

	QDomElement sendEle = m_doc.createElement("SendEntity");
	linkLineEle.appendChild(sendEle);
	sendEle.appendChild(m_doc.createTextNode(QString::fromStdString(pLinkLineEle->sendEntityID())));

	QDomElement receiveEle = m_doc.createElement("ReceiveEntity");
	linkLineEle.appendChild(receiveEle);
	receiveEle.appendChild(m_doc.createTextNode(QString::fromStdString(pLinkLineEle->receiveEntityID())));

	QDomElement vecTimePariEle = m_doc.createElement("VecTimePari");
	linkLineEle.appendChild(vecTimePariEle);
	for (auto itr =pLinkLineEle->vecTimePari().begin() ;itr!=pLinkLineEle->vecTimePari().end();++itr)
	{
		QDomElement timePariEle = m_doc.createElement("TimePari");
		vecTimePariEle.appendChild(timePariEle);

		QDomElement stEle = m_doc.createElement("StartTime");
		timePariEle.appendChild(stEle);
		CreateTimeText(stEle,itr->first);

		QDomElement etEle = m_doc.createElement("EndTime");
		timePariEle.appendChild(etEle);
		CreateTimeText(etEle,itr->second);
	}
}

void CBizDataSave::createHighLightEle( QDomElement ele, FxBizUnit::CFxHighlightEle* pHighLightEle )
{
	QDomElement HighLightEle = m_doc.createElement("HighLight");
	ele.appendChild(HighLightEle);

	QDomElement idEle = m_doc.createElement("ID");
	HighLightEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pHighLightEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	HighLightEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pHighLightEle->nameEle()->text())));

	QDomElement colorEle = m_doc.createElement("Color");
	HighLightEle.appendChild(colorEle);
	CreateVec4(colorEle,pHighLightEle->color(),1);

	QDomElement EntityEle = m_doc.createElement("EntityID");
	HighLightEle.appendChild(EntityEle);
	EntityEle.appendChild(m_doc.createTextNode(QString::fromStdString(pHighLightEle->entityID())));

	QDomElement mapID2PariEle = m_doc.createElement("MapID2VecPari");
	HighLightEle.appendChild(mapID2PariEle);
	for (auto iter = pHighLightEle->mapID2TimePari().begin(); iter != pHighLightEle->mapID2TimePari().end();iter ++)
	{
		QDomElement GeomePariEle = m_doc.createElement("Geometry");
		mapID2PariEle.appendChild(GeomePariEle);

		QDomElement GeomeIDPariEle = m_doc.createElement("GeometryID");
		GeomePariEle.appendChild(GeomeIDPariEle);
		GeomeIDPariEle.appendChild(m_doc.createTextNode(QString::fromStdString(iter->first)));

		QDomElement vecTimePariEle = m_doc.createElement("VecTimePari");
		GeomePariEle.appendChild(vecTimePariEle);

		for(auto itr = iter->second.begin();itr != iter->second.end();itr++)
		{
			QDomElement timePariEle = m_doc.createElement("TimePari");
			vecTimePariEle.appendChild(timePariEle);

			QDomElement stEle = m_doc.createElement("StartTime");
			timePariEle.appendChild(stEle);
			CreateTimeText(stEle,itr->first);

			QDomElement etEle = m_doc.createElement("EndTime");
			timePariEle.appendChild(etEle);
			CreateTimeText(etEle,itr->second);
		}
	}
}

void CBizDataSave::createJammingEle( QDomElement ele, FxBizUnit::CFxSignalJammingEle* pJammingEle )
{
	QDomElement JammingEle = m_doc.createElement("Jamming");
	ele.appendChild(JammingEle);

	QDomElement idEle = m_doc.createElement("ID");
	JammingEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pJammingEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	JammingEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pJammingEle->nameEle()->text())));

	QDomElement EntityEle = m_doc.createElement("EntityID");
	JammingEle.appendChild(EntityEle);
	EntityEle.appendChild(m_doc.createTextNode(QString::fromStdString(pJammingEle->entityID())));

	QDomElement MapVecPari2IntensityEle = m_doc.createElement("MapVecPari2Intensity");
	JammingEle.appendChild(MapVecPari2IntensityEle);
	for (auto iter = pJammingEle->mapTimePari2Intensity().begin(); iter != pJammingEle->mapTimePari2Intensity().end();iter ++)
	{
		QDomElement VecPari2IntensityEle = m_doc.createElement("VecPari2Intensity");
		MapVecPari2IntensityEle.appendChild(VecPari2IntensityEle);

		QDomElement IntensityEle = m_doc.createElement("Intensity");
		VecPari2IntensityEle.appendChild(IntensityEle);
		IntensityEle.appendChild(m_doc.createTextNode(QString::number(iter->second)));

		QDomElement vecTimePariEle = m_doc.createElement("VecTimePari");
		VecPari2IntensityEle.appendChild(vecTimePariEle);

		QDomElement timePariEle = m_doc.createElement("TimePari");
		vecTimePariEle.appendChild(timePariEle);

		QDomElement stEle = m_doc.createElement("StartTime");
		timePariEle.appendChild(stEle);
		CreateTimeText(stEle,iter->first.first);

		QDomElement etEle = m_doc.createElement("EndTime");
		timePariEle.appendChild(etEle);
		CreateTimeText(etEle,iter->first.second);
	}
}

void CBizDataSave::createCameraImagingEle( QDomElement ele, FxBizUnit::CFxCameraImagingEle* pCameraImagingEle )
{
	QDomElement CameraImagingEle = m_doc.createElement("CameraImaging");
	ele.appendChild(CameraImagingEle);

	QDomElement idEle = m_doc.createElement("ID");
	CameraImagingEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pCameraImagingEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	CameraImagingEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pCameraImagingEle->nameEle()->text())));

	QDomElement entityEle = m_doc.createElement("Entity");
	CameraImagingEle.appendChild(entityEle);
	entityEle.appendChild(m_doc.createTextNode(QString::fromStdString(pCameraImagingEle->entityID())));

	QDomElement vecTimePariEle = m_doc.createElement("VecTimePari");
	CameraImagingEle.appendChild(vecTimePariEle);
	for (auto itr =pCameraImagingEle->vecTimePari().begin() ;itr!=pCameraImagingEle->vecTimePari().end();++itr)
	{
		QDomElement timePariEle = m_doc.createElement("TimePari");
		vecTimePariEle.appendChild(timePariEle);

		QDomElement stEle = m_doc.createElement("StartTime");
		timePariEle.appendChild(stEle);
		CreateTimeText(stEle,itr->first);

		QDomElement etEle = m_doc.createElement("EndTime");
		timePariEle.appendChild(etEle);
		CreateTimeText(etEle,itr->second);
	}
}

void CBizDataSave::createCaptureEle(QDomElement ele, FxBizUnit::CFxSignalCaptureEle* pCaptureEle)
{
	QDomElement CaptureEle = m_doc.createElement("Capture");
	ele.appendChild(CaptureEle);

	QDomElement idEle = m_doc.createElement("ID");
	CaptureEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pCaptureEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	CaptureEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pCaptureEle->nameEle()->text())));

	QDomElement EntityEle = m_doc.createElement("EntityID");
	CaptureEle.appendChild(EntityEle);
	EntityEle.appendChild(m_doc.createTextNode(QString::fromStdString(pCaptureEle->entityID())));

	QDomElement MapVecPari2CaptureEle = m_doc.createElement("MapVecPari2Capture");
	CaptureEle.appendChild(MapVecPari2CaptureEle);
	for (auto iter = pCaptureEle->mapTimePari2Capture().begin(); iter != pCaptureEle->mapTimePari2Capture().end();iter ++)
	{
		QDomElement VecPari2CaptureEle = m_doc.createElement("VecPari2Capture");
		MapVecPari2CaptureEle.appendChild(VecPari2CaptureEle);

		QDomElement IsCaptureEle = m_doc.createElement("isCapture");
		VecPari2CaptureEle.appendChild(IsCaptureEle); 
		IsCaptureEle.appendChild(m_doc.createTextNode(QString::number((iter->second).first)));

		QDomElement IntensityEle = m_doc.createElement("Intensity");
		VecPari2CaptureEle.appendChild(IntensityEle);
		IntensityEle.appendChild(m_doc.createTextNode(QString::number((iter->second).second)));

		QDomElement vecTimePariEle = m_doc.createElement("VecTimePari");
		VecPari2CaptureEle.appendChild(vecTimePariEle);

		QDomElement timePariEle = m_doc.createElement("TimePari");
		vecTimePariEle.appendChild(timePariEle);

		QDomElement stEle = m_doc.createElement("StartTime");
		timePariEle.appendChild(stEle);
		CreateTimeText(stEle,iter->first.first);

		QDomElement etEle = m_doc.createElement("EndTime");
		timePariEle.appendChild(etEle);
		CreateTimeText(etEle,iter->first.second);
	}
}

void CBizDataSave::createEnhenceEle( QDomElement ele, FxBizUnit::CFxSignalEnhanceEle* pEnhenceEle )
{
	QDomElement EnhenceEle = m_doc.createElement("Enhence");
	ele.appendChild(EnhenceEle);

	QDomElement idEle = m_doc.createElement("ID");
	EnhenceEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pEnhenceEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	EnhenceEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pEnhenceEle->nameEle()->text())));

	QDomElement EntityEle = m_doc.createElement("EntityID");
	EnhenceEle.appendChild(EntityEle);
	EntityEle.appendChild(m_doc.createTextNode(QString::fromStdString(pEnhenceEle->entityID())));

	QDomElement MapVecPari2EnhenceEle = m_doc.createElement("MapVecPari2Enhence");
	EnhenceEle.appendChild(MapVecPari2EnhenceEle);
	for (auto iter = pEnhenceEle->mapTimePari2Enhance().begin(); iter != pEnhenceEle->mapTimePari2Enhance().end();iter ++)
	{
		QDomElement VecPari2EnhenceEle = m_doc.createElement("VecPari2Enhence");
		MapVecPari2EnhenceEle.appendChild(VecPari2EnhenceEle);

		QDomElement IntensityEle = m_doc.createElement("Intensity");
		VecPari2EnhenceEle.appendChild(IntensityEle);
		IntensityEle.appendChild(m_doc.createTextNode(QString::number(iter->second)));

		QDomElement vecTimePariEle = m_doc.createElement("VecTimePari");
		VecPari2EnhenceEle.appendChild(vecTimePariEle);

		QDomElement timePariEle = m_doc.createElement("TimePari");
		vecTimePariEle.appendChild(timePariEle);

		QDomElement stEle = m_doc.createElement("StartTime");
		timePariEle.appendChild(stEle);
		CreateTimeText(stEle,iter->first.first);

		QDomElement etEle = m_doc.createElement("EndTime");
		timePariEle.appendChild(etEle);
		CreateTimeText(etEle,iter->first.second);
	}
}
void CBizDataSave::createConcentricRingsEle( QDomElement ele, FxBizUnit::CFxConcentricRingsEle* pConcentricRingsEle )
{
	QDomElement ConcentricRingsEle = m_doc.createElement("ConcentricRings");
	ele.appendChild(ConcentricRingsEle);

	QDomElement idEle = m_doc.createElement("ID");
	ConcentricRingsEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pConcentricRingsEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	ConcentricRingsEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pConcentricRingsEle->nameEle()->text())));

	QDomElement StartPointEle = m_doc.createElement("StartPoint");
	ConcentricRingsEle.appendChild(StartPointEle);
	CreateVec3(StartPointEle,pConcentricRingsEle->startPoint(),4);

	QDomElement EndPointEle = m_doc.createElement("EndPoint");
	ConcentricRingsEle.appendChild(EndPointEle);
	CreateVec3(EndPointEle,pConcentricRingsEle->endPoint(),4);

	QDomElement StartRadiusEle = m_doc.createElement("StartRadius");
	ConcentricRingsEle.appendChild(StartRadiusEle);
	StartRadiusEle.appendChild(m_doc.createTextNode(QString::number(pConcentricRingsEle->startRadius(),'f',1)));

	QDomElement EndRadiusEle = m_doc.createElement("EndRadius");
	ConcentricRingsEle.appendChild(EndRadiusEle);
	EndRadiusEle.appendChild(m_doc.createTextNode(QString::number(pConcentricRingsEle->endRadius(),'f',1)));

	QDomElement NumWaveEle = m_doc.createElement("NumWave");
	ConcentricRingsEle.appendChild(NumWaveEle);
	NumWaveEle.appendChild(m_doc.createTextNode(QString::number(pConcentricRingsEle->numWave())));

	QDomElement BaseColorEle = m_doc.createElement("BaseColor");
	ConcentricRingsEle.appendChild(BaseColorEle);
	CreateVec4(BaseColorEle,pConcentricRingsEle->baseColor(),1);

	QDomElement WaveColorEle = m_doc.createElement("WaveColor");
	ConcentricRingsEle.appendChild(WaveColorEle);
	CreateVec4(WaveColorEle,pConcentricRingsEle->waveColor(),1);

	QDomElement WaveWidthEle = m_doc.createElement("WaveWidth");
	ConcentricRingsEle.appendChild(WaveWidthEle);
	WaveWidthEle.appendChild(m_doc.createTextNode(QString::number(pConcentricRingsEle->waveWidth(),'f',1)));

	QDomElement WidthBetweenWaveEle = m_doc.createElement("WidthBetweenWave");
	ConcentricRingsEle.appendChild(WidthBetweenWaveEle);
	WidthBetweenWaveEle.appendChild(m_doc.createTextNode(QString::number(pConcentricRingsEle->widthBetweenWave(),'f',1)));

	QDomElement WaveStartOffsetEle = m_doc.createElement("WaveStartOffset");
	ConcentricRingsEle.appendChild(WaveStartOffsetEle);
	WaveStartOffsetEle.appendChild(m_doc.createTextNode(QString::number(pConcentricRingsEle->waveStartOffset(),'f',1)));

	QDomElement MoveSpeedEle = m_doc.createElement("MoveSpeed");
	ConcentricRingsEle.appendChild(MoveSpeedEle);
	MoveSpeedEle.appendChild(m_doc.createTextNode(QString::number(pConcentricRingsEle->moveSpeed(),'f',1)));

	QDomElement IsSmoothEle = m_doc.createElement("IsSmooth");
	ConcentricRingsEle.appendChild(IsSmoothEle);
	IsSmoothEle.appendChild(m_doc.createTextNode(QString::number(pConcentricRingsEle->smooth())));
	/*if (pConcentricRingsEle->smooth() == true )
	{
		IsSmoothEle.appendChild(m_doc.createTextNode("true"));
	}
	else if (pConcentricRingsEle->smooth() == false)
	{
		IsSmoothEle.appendChild(m_doc.createTextNode("false"));
	}*/

	QDomElement sendEle = m_doc.createElement("SendEntity");
	ConcentricRingsEle.appendChild(sendEle);
	sendEle.appendChild(m_doc.createTextNode(QString::fromStdString(pConcentricRingsEle->sendEntityID())));

	QDomElement receiveEle = m_doc.createElement("ReceiveEntity");
	ConcentricRingsEle.appendChild(receiveEle);
	receiveEle.appendChild(m_doc.createTextNode(QString::fromStdString(pConcentricRingsEle->receiveEntityID())));

	QDomElement vecTimePariEle = m_doc.createElement("VecTimePari");
	ConcentricRingsEle.appendChild(vecTimePariEle);
	for (auto itr =pConcentricRingsEle->vecTimePari().begin() ;itr!=pConcentricRingsEle->vecTimePari().end();++itr)
	{
		QDomElement timePariEle = m_doc.createElement("TimePari");
		vecTimePariEle.appendChild(timePariEle);

		QDomElement stEle = m_doc.createElement("StartTime");
		timePariEle.appendChild(stEle);
		CreateTimeText(stEle,itr->first);

		QDomElement etEle = m_doc.createElement("EndTime");
		timePariEle.appendChild(etEle);
		CreateTimeText(etEle,itr->second);
	}
}

void CBizDataSave::createDisturbEle( QDomElement ele, FxBizUnit::CFxDisturbEle* pDisturbEle )
{
	QDomElement DisturbEle = m_doc.createElement("Disturb");
	ele.appendChild(DisturbEle);

	QDomElement idEle = m_doc.createElement("ID");
	DisturbEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pDisturbEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	DisturbEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pDisturbEle->nameEle()->text())));

	QDomElement SendRadiusEle = m_doc.createElement("SendRadius");
	DisturbEle.appendChild(SendRadiusEle);
	SendRadiusEle.appendChild(m_doc.createTextNode(QString::number(pDisturbEle->sendRadius(),'f',1)));

	QDomElement ReceiveRadiusEle = m_doc.createElement("ReceiveRadius");
	DisturbEle.appendChild(ReceiveRadiusEle);
	ReceiveRadiusEle.appendChild(m_doc.createTextNode(QString::number(pDisturbEle->receiveRadius(),'f',1)));

	QDomElement BaseColorEle = m_doc.createElement("Color");
	DisturbEle.appendChild(BaseColorEle);
	CreateVec4(BaseColorEle,pDisturbEle->color(),1);

	QDomElement sendEle = m_doc.createElement("SendEntity");
	DisturbEle.appendChild(sendEle);
	sendEle.appendChild(m_doc.createTextNode(QString::fromStdString(pDisturbEle->sendEntityID())));

	QDomElement receiveEle = m_doc.createElement("ReceiveEntity");
	DisturbEle.appendChild(receiveEle);
	receiveEle.appendChild(m_doc.createTextNode(QString::fromStdString(pDisturbEle->receiveEntityID())));

	QDomElement vecTimePariEle = m_doc.createElement("VecTimePari");
	DisturbEle.appendChild(vecTimePariEle);
	for (auto itr =pDisturbEle->vecTimePari().begin() ;itr!=pDisturbEle->vecTimePari().end();++itr)
	{
		QDomElement timePariEle = m_doc.createElement("TimePari");
		vecTimePariEle.appendChild(timePariEle);

		QDomElement stEle = m_doc.createElement("StartTime");
		timePariEle.appendChild(stEle);
		CreateTimeText(stEle,itr->first);

		QDomElement etEle = m_doc.createElement("EndTime");
		timePariEle.appendChild(etEle);
		CreateTimeText(etEle,itr->second);
	}
}

void CBizDataSave::createCameraTraceEle( QDomElement ele, FxBizUnit::CFxCameraTraceEle* pCameraTraceEle )
{
	QDomElement cameraTraceEle = m_doc.createElement("Trace");
	ele.appendChild(cameraTraceEle);

	QDomElement idEle = m_doc.createElement("ID");
	cameraTraceEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pCameraTraceEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	cameraTraceEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pCameraTraceEle->nameEle()->text())));

	QDomElement traceListEle = m_doc.createElement("TraceList");
	cameraTraceEle.appendChild(traceListEle);
	for (auto itr =pCameraTraceEle->vecTraceObj().begin() ;itr!=pCameraTraceEle->vecTraceObj().end();++itr)
	{
		QDomElement traceObjectEle = m_doc.createElement("TraceObject");
		traceListEle.appendChild(traceObjectEle);

		QDomElement typeEle = m_doc.createElement("Type");
		traceObjectEle.appendChild(typeEle);
		typeEle.appendChild(m_doc.createTextNode(QString::number(itr->TraceType)));

		QDomElement distanceEle = m_doc.createElement("Distance");
		traceObjectEle.appendChild(distanceEle);
		distanceEle.appendChild(m_doc.createTextNode(QString::number(itr->nDistance)));

		QDomElement degreeHeadEle = m_doc.createElement("DegreeHead");
		traceObjectEle.appendChild(degreeHeadEle);
		degreeHeadEle.appendChild(m_doc.createTextNode(QString::number(itr->dDegreeHead)));

		QDomElement degreeTiltEle = m_doc.createElement("DegreeTilt");
		traceObjectEle.appendChild(degreeTiltEle);
		degreeTiltEle.appendChild(m_doc.createTextNode(QString::number(itr->dDegreeTilt)));

		QDomElement tracedIDEle = m_doc.createElement("TracedID");
		traceObjectEle.appendChild(tracedIDEle);
		tracedIDEle.appendChild(m_doc.createTextNode(QString::fromStdString(itr->strTracedID)));

		for (auto iter = itr->vecTimePari.begin() ;iter != itr->vecTimePari.end();++iter)
		{
			QDomElement timePariEle = m_doc.createElement("TimePari");
			traceObjectEle.appendChild(timePariEle);

			QDomElement stEle = m_doc.createElement("StartTime");
			timePariEle.appendChild(stEle);
			CreateTimeText(stEle,iter->first);

			QDomElement etEle = m_doc.createElement("EndTime");
			timePariEle.appendChild(etEle);
			CreateTimeText(etEle,iter->second);
		}
	}
}

void CBizDataSave::createObserveEle( QDomElement ele, FxBizUnit::CFxObserveEle* pObserveEle )
{
	QDomElement observeEle = m_doc.createElement("Observe");
	ele.appendChild(observeEle);

	QDomElement idEle = m_doc.createElement("ID");
	observeEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pObserveEle->ID())));

	QDomElement nameEle = m_doc.createElement("Name");
	observeEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pObserveEle->nameEle()->text())));

	QDomElement spEle = m_doc.createElement("SegmentPoint");
	observeEle.appendChild(spEle);
	spEle.appendChild(m_doc.createTextNode(QString::number(pObserveEle->segmentPoint())));

	QDomElement snEle = m_doc.createElement("SegmentNum");
	observeEle.appendChild(snEle);
	snEle.appendChild(m_doc.createTextNode(QString::number(pObserveEle->segmentNum())));

	QDomElement faceColorEle = m_doc.createElement("FaceColor");
	observeEle.appendChild(faceColorEle);
	CreateVec4(faceColorEle,pObserveEle->faceColor(),1);

	QDomElement lineColorEle = m_doc.createElement("LineColor");
	observeEle.appendChild(lineColorEle);
	CreateVec4(lineColorEle,pObserveEle->lineColor(),1);

	QDomElement sendEle = m_doc.createElement("SendEntity");
	observeEle.appendChild(sendEle);
	sendEle.appendChild(m_doc.createTextNode(QString::fromStdString(pObserveEle->sendEntityID())));

	QDomElement receiveEle = m_doc.createElement("ReceiveEntity");
	observeEle.appendChild(receiveEle);
	receiveEle.appendChild(m_doc.createTextNode(QString::fromStdString(pObserveEle->receiveEntityID())));
}

void CBizDataSave::createEntityEle( QDomElement ele, FxEntityObject::CFxEntityObjectEle* pEntityEle )
{
	QDomElement entityEle = m_doc.createElement("EntityEle");//实体信息组节点
	ele.appendChild(entityEle);

	QDomElement idEle = m_doc.createElement("ID");
	entityEle.appendChild(idEle);
	idEle.appendChild(m_doc.createTextNode(QString::fromStdString(pEntityEle->ID())));

	QDomElement parentIDEle = m_doc.createElement("ParentID");
	entityEle.appendChild(parentIDEle);
	parentIDEle.appendChild(m_doc.createTextNode(QString::fromStdString(pEntityEle->parentID())));

	QDomElement typeEle = m_doc.createElement("EntityType");
	entityEle.appendChild(typeEle);
	typeEle.appendChild(m_doc.createTextNode(QString::fromStdString(pEntityEle->entityType())));

	QDomElement dynamicEle = m_doc.createElement("Dynamic");
	entityEle.appendChild(dynamicEle);
	dynamicEle.appendChild(m_doc.createTextNode(QString::number(pEntityEle->dynamic())));

	QDomElement campEle = m_doc.createElement("Camp");
	entityEle.appendChild(campEle);
	campEle.appendChild(m_doc.createTextNode(QString::number(pEntityEle->camp())));

	createBaseEle(entityEle,pEntityEle);
	createTextEle(entityEle,pEntityEle->nameEle());
	createImageEle(entityEle,pEntityEle->imageEle());
	createModelEle(entityEle,pEntityEle->modelEle());
	createPointEle(entityEle,pEntityEle->pointEle());
	createPathEle(entityEle,pEntityEle->pathEle());
}

void CBizDataSave::createBaseEle( QDomElement ele, FxBaseObject::CFxObejctEle* pObjEle )
{
	QDomElement objectEle = m_doc.createElement("BaseEle");
	ele.appendChild(objectEle);

	QDomElement visibleEle = m_doc.createElement("Visible");
	objectEle.appendChild(visibleEle);
	visibleEle.appendChild(m_doc.createTextNode(QString::number(pObjEle->visible())));

	QDomElement positionIDEle = m_doc.createElement("Position");
	objectEle.appendChild(positionIDEle);
	CreateVec3d(positionIDEle,pObjEle->positionLLH(),6);

	QDomElement postureEle = m_doc.createElement("Posture");
	objectEle.appendChild(postureEle);
	CreateVec3d(postureEle,pObjEle->posture(),4);
}

void CBizDataSave::createTextEle( QDomElement ele, FxBaseObject::CFxTextObjectEle* pTextObjEle )
{
	QDomElement textEle = m_doc.createElement("TextEle");
	ele.appendChild(textEle);

	QDomElement nameEle = m_doc.createElement("Text");
	textEle.appendChild(nameEle);
	nameEle.appendChild(m_doc.createTextNode(QString::fromStdString(pTextObjEle->text())));

	QDomElement fontEle = m_doc.createElement("Font");
	textEle.appendChild(fontEle);
	
	fontEle.appendChild(m_doc.createTextNode(QString::fromStdString(DeleteDataPath(pTextObjEle->font()))));

	QDomElement fontSizeEle = m_doc.createElement("FontSize");
	textEle.appendChild(fontSizeEle);
	fontSizeEle.appendChild(m_doc.createTextNode(QString::number(pTextObjEle->fontSize(),'f',1)));

	QDomElement colorEle = m_doc.createElement("Color");
	textEle.appendChild(colorEle);
	CreateVec4(colorEle,pTextObjEle->color(),1);

	QDomElement outLineEle = m_doc.createElement("OutLine");
	textEle.appendChild(outLineEle);
	outLineEle.appendChild(m_doc.createTextNode(QString::number(pTextObjEle->outLine())));

	QDomElement outLineColorEle = m_doc.createElement("OutLineColor");
	textEle.appendChild(outLineColorEle);
	CreateVec4(outLineColorEle,pTextObjEle->outLineColor(),1);

	QDomElement offsetEle = m_doc.createElement("Offset");
	textEle.appendChild(offsetEle);
	CreateVec3d(offsetEle,pTextObjEle->offset(),4);

	QDomElement autoScaleEle = m_doc.createElement("AutoScale");
	textEle.appendChild(autoScaleEle);
	autoScaleEle.appendChild(m_doc.createTextNode(QString::number(pTextObjEle->autoScale())));

	QDomElement autoRotateEle = m_doc.createElement("AutoRotate");
	textEle.appendChild(autoRotateEle);
	autoRotateEle.appendChild(m_doc.createTextNode(QString::number(pTextObjEle->autoRotate())));
}

void CBizDataSave::createImageEle( QDomElement ele, FxBaseObject::CFxImageObjectEle* pImageObjEle )
{
	QDomElement imageEle = m_doc.createElement("ImageEle");
	ele.appendChild(imageEle);

	QDomElement filePathEle = m_doc.createElement("FilePath");
	imageEle.appendChild(filePathEle);
	filePathEle.appendChild(m_doc.createTextNode(QString::fromStdString(DeleteDataPath(pImageObjEle->filePath()))));

	QDomElement opcityEle = m_doc.createElement("Opacity");
	imageEle.appendChild(opcityEle);
	opcityEle.appendChild(m_doc.createTextNode(QString::number(pImageObjEle->opacity(),'f',1)));

	QDomElement imageSizeEle = m_doc.createElement("ImageSize");
	imageEle.appendChild(imageSizeEle);
	CreateVec2(imageSizeEle,pImageObjEle->imageSize(),0);

	QDomElement autoScaleEle = m_doc.createElement("AutoScale");
	imageEle.appendChild(autoScaleEle);
	autoScaleEle.appendChild(m_doc.createTextNode(QString::number(pImageObjEle->autoScale())));

	QDomElement autoRotateEle = m_doc.createElement("AutoRotate");
	imageEle.appendChild(autoRotateEle);
	autoRotateEle.appendChild(m_doc.createTextNode(QString::number(pImageObjEle->autoRotate())));
}

void CBizDataSave::createModelEle( QDomElement ele, FxBaseObject::CFxModelObjectEle* pModelObjEle )
{
	QDomElement modelEle = m_doc.createElement("ModelEle");
	ele.appendChild(modelEle);

	QDomElement filePathEle = m_doc.createElement("FilePath");
	modelEle.appendChild(filePathEle);
	if (pModelObjEle->filePath().size())
	{
		filePathEle.appendChild(m_doc.createTextNode(QString::fromStdString(DeleteDataPath(pModelObjEle->filePath()))));
	}
	
	else
	{
		filePathEle.appendChild(m_doc.createTextNode(""));
	}

	QDomElement positionEle = m_doc.createElement("Position");
	modelEle.appendChild(positionEle);
	CreateVec3d(positionEle,pModelObjEle->position(),4);

	QDomElement postureEle = m_doc.createElement("Posture");
	modelEle.appendChild(postureEle);
	CreateVec3d(postureEle,pModelObjEle->posture(),4);

	QDomElement scaleEle = m_doc.createElement("Scale");
	modelEle.appendChild(scaleEle);
	CreateVec3d(scaleEle,pModelObjEle->scale(),4);

	QDomElement autoScaleEle = m_doc.createElement("AutoScale");
	modelEle.appendChild(autoScaleEle);
	autoScaleEle.appendChild(m_doc.createTextNode(QString::number(pModelObjEle->autoScale())));

	QDomElement autoRotateEle = m_doc.createElement("AutoRotate");
	modelEle.appendChild(autoRotateEle);
	autoRotateEle.appendChild(m_doc.createTextNode(QString::number(pModelObjEle->autoRotate())));

	QDomElement useColorEle = m_doc.createElement("UseColor");
	modelEle.appendChild(useColorEle);
	useColorEle.appendChild(m_doc.createTextNode(QString::number(pModelObjEle->useColor())));

	QDomElement colorEle = m_doc.createElement("Color");
	modelEle.appendChild(colorEle);
	CreateVec4(colorEle,pModelObjEle->color(),1);

	QDomElement blendEle = m_doc.createElement("Blend");
	modelEle.appendChild(blendEle);
	blendEle.appendChild(m_doc.createTextNode(QString::number(pModelObjEle->blend())));
}

void CBizDataSave::createPointEle( QDomElement ele, FxBaseObject::CFxPointObjectEle* pPointObjEle )
{
	QDomElement pointEle = m_doc.createElement("PointEle");
	ele.appendChild(pointEle);

	QDomElement pointSizeEle = m_doc.createElement("PointSize");
	pointEle.appendChild(pointSizeEle);
	pointSizeEle.appendChild(m_doc.createTextNode(QString::number(pPointObjEle->pointSize())));

	QDomElement colorEle = m_doc.createElement("Color");
	pointEle.appendChild(colorEle);
	CreateVec4(colorEle,pPointObjEle->color(),1);
}

void CBizDataSave::createPathEle( QDomElement ele, FxBaseObject::CFxLineObjectEle* pLineObject )
{
	QDomElement pathEle = m_doc.createElement("PathEle");
	ele.appendChild(pathEle);

	QDomElement pointSizeEle = m_doc.createElement("PointSize");
	pathEle.appendChild(pointSizeEle);
	pointSizeEle.appendChild(m_doc.createTextNode(QString::number(pLineObject->pointSize())));

	QDomElement colorEle = m_doc.createElement("Color");
	pathEle.appendChild(colorEle);
	CreateVec4(colorEle,pLineObject->color(),1);

	QDomElement lineTypeEle = m_doc.createElement("LineType");
	pathEle.appendChild(lineTypeEle);
	lineTypeEle.appendChild(m_doc.createTextNode(QString::number(pLineObject->lineType())));

	QDomElement lineWidthEle = m_doc.createElement("LineWidth");
	pathEle.appendChild(lineWidthEle);
	lineWidthEle.appendChild(m_doc.createTextNode(QString::number(pLineObject->lineWidth(),'f',1)));

	QDomElement loopEle = m_doc.createElement("Loop");
	pathEle.appendChild(loopEle);
	loopEle.appendChild(m_doc.createTextNode(QString::number(pLineObject->loop())));

	QDomElement visibleEle = m_doc.createElement("Visible");
	pathEle.appendChild(visibleEle);
	visibleEle.appendChild(m_doc.createTextNode(QString::number(pLineObject->visible())));
}

void CBizDataSave::createSgp4DataEle( QDomElement ele, FxBizUnit::CFxSpacecraftEle* pSpacecraftEle )
{
	QDomElement sgp4Ele = m_doc.createElement("Sgp4Data");
	ele.appendChild(sgp4Ele);

	QDomElement stEle = m_doc.createElement("StartTime");
	sgp4Ele.appendChild(stEle);
	CreateTimeText(stEle,pSpacecraftEle->sgp4Data().dBeginTime);

	QDomElement etEle = m_doc.createElement("EndTime");
	sgp4Ele.appendChild(etEle);
	CreateTimeText(etEle,pSpacecraftEle->sgp4Data().dEndTime);

	QDomElement tle1 = m_doc.createElement("Tle1");
	sgp4Ele.appendChild(tle1);
	tle1.appendChild(m_doc.createTextNode(QString::fromStdString(pSpacecraftEle->sgp4Data().strLine1)));

	QDomElement tle2 = m_doc.createElement("Tle2");
	sgp4Ele.appendChild(tle2);
	tle2.appendChild(m_doc.createTextNode(QString::fromStdString(pSpacecraftEle->sgp4Data().strLine2)));

	QDomElement deltaTime = m_doc.createElement("DeltaTime");
	sgp4Ele.appendChild(deltaTime);
	deltaTime.appendChild(m_doc.createTextNode(QString::number(pSpacecraftEle->sgp4Data().dDeltaTime,'f',12)));
}

void CBizDataSave::createAirLineEle( QDomElement ele, FxBizUnit::CFxAircraftEle* pAircraftEle )
{
	QDomElement airLineEle = m_doc.createElement("AirLine");
	ele.appendChild(airLineEle);

	QDomElement stEle = m_doc.createElement("StartTime");
	airLineEle.appendChild(stEle);
	CreateTimeText(stEle,pAircraftEle->airLine().dBeginTime);

	QDomElement etEle = m_doc.createElement("EndTime");
	airLineEle.appendChild(etEle);
	CreateTimeText(etEle,pAircraftEle->airLine().dEndTime);

	QDomElement flyPathEle = m_doc.createElement("FlyPath");
	airLineEle.appendChild(flyPathEle);
	flyPathEle.appendChild(m_doc.createTextNode(QString::fromStdString(pAircraftEle->airLine().strPath)));

	QDomElement modeEle = m_doc.createElement("AirLineMode");
	airLineEle.appendChild(modeEle);
	modeEle.appendChild(m_doc.createTextNode(QString::number(pAircraftEle->airLine().nMode)));
}

void CBizDataSave::createSeaLineEle( QDomElement ele, FxBizUnit::CFxSeaEle* pSeaEle )
{
	QDomElement seaLineEle = m_doc.createElement("SeaLine");
	ele.appendChild(seaLineEle);

	QDomElement stEle = m_doc.createElement("StartTime");
	seaLineEle.appendChild(stEle);
	CreateTimeText(stEle,pSeaEle->airLine().dBeginTime);

	QDomElement etEle = m_doc.createElement("EndTime");
	seaLineEle.appendChild(etEle);
	CreateTimeText(etEle,pSeaEle->airLine().dEndTime);

	QDomElement flyPathEle = m_doc.createElement("FlyPath");
	seaLineEle.appendChild(flyPathEle);
	flyPathEle.appendChild(m_doc.createTextNode(QString::fromStdString(pSeaEle->airLine().strPath)));

	QDomElement modeEle = m_doc.createElement("SeaLineMode");
	seaLineEle.appendChild(modeEle);
	modeEle.appendChild(m_doc.createTextNode(QString::number(pSeaEle->airLine().nMode)));
}

void CBizDataSave::CreateVec2( QDomElement ele, osg::Vec2 vec, int prec)
{
	QString str = QString::number(vec.x(),'f',prec) + ","
		+ QString::number(vec.y(),'f',prec);

	ele.appendChild(m_doc.createTextNode(str));
}

void CBizDataSave::CreateVec2d( QDomElement ele, osg::Vec2d vec, int prec )
{
	QString str = QString::number(vec.x(),'lf',prec) + ","
		+ QString::number(vec.y(),'lf',prec);

	ele.appendChild(m_doc.createTextNode(str));
}

void CBizDataSave::CreateVec3( QDomElement ele, osg::Vec3 vec, int prec )
{
	QString str = QString::number(vec.x(),'f',prec) + ","
		+ QString::number(vec.y(),'f',prec) + ","
		+ QString::number(vec.z(),'f',prec);

	ele.appendChild(m_doc.createTextNode(str));
}

void CBizDataSave::CreateVec3d( QDomElement ele, osg::Vec3d vec, int prec )
{
	QString str = QString::number(vec.x(),'lf',prec) + ","
		+ QString::number(vec.y(),'lf',prec) + ","
		+ QString::number(vec.z(),'lf',prec);

	ele.appendChild(m_doc.createTextNode(str));
}

void CBizDataSave::CreateVec4( QDomElement ele, osg::Vec4 vec, int prec )
{
	QString str = QString::number(vec.x(),'f',prec) + ","
		+ QString::number(vec.y(),'f',prec) + ","
		+ QString::number(vec.z(),'f',prec) + ","
		+ QString::number(vec.w(),'f',prec);

	ele.appendChild(m_doc.createTextNode(str));
}

void CBizDataSave::CreateVec4d( QDomElement ele, osg::Vec4d vec, int prec )
{
	QString str = QString::number(vec.x(),'lf',prec) + ","
		+ QString::number(vec.y(),'lf',prec) + ","
		+ QString::number(vec.z(),'lf',prec) + ","
		+ QString::number(vec.w(),'lf',prec);

	ele.appendChild(m_doc.createTextNode(str));
}

void CBizDataSave::CreateTimeText( QDomElement ele, double dJD )
{
	int nYear, nMonth, nDay, nHour, nMin;
	double dSec;

	FxSimulationTimerInstance()->jdToCal(dJD, nYear, nMonth, nDay
		, nHour, nMin, dSec);

	QString str = QString::number(nYear) + "-" + QString::number(nMonth) + "-" + QString::number(nDay) + " "
		+ QString::number(nHour) + ":" + QString::number(nMin) + ":" + QString::number(dSec);

	ele.appendChild(m_doc.createTextNode(str));
}

std::string CBizDataSave::DeleteDataPath( std::string strPath)
{
	std::string strDataPath = FeFileReg->GetDataPath();
	std::string strTemp = "";
	for (auto iter = strPath.begin();iter != strPath.end();)
	{
		strTemp+=*iter;
		if (strTemp == strDataPath)
		{
			iter = strPath.erase(iter);
			iter = strPath.erase(iter);
			return strPath;
		}
		iter = strPath.erase(iter);
	}
}

bool CBizDataSave::getService()
{
	if (m_pBizUnitService)
	{
		return true;
	}
	FxCore::CFxServiceQuerier<FxBizUnit::IFxBizUnitService> pBizUnitService;
	if (pBizUnitService)
	{
		m_pBizUnitService = pBizUnitService.get();
		//auto serviceRef = pBizUnitService.serviceRef();
		//m_pBizUnitService = serviceRef.toService<FxBizUnit::IFxBizUnitService>();
	}
	if (m_pBizUnitService)
	{
		return true;
	}
	else
	{
		return false;
	}
}




