/**************************************************************************************************
* @file CBizDataSave.h
* @note 保存场景部署文件
* @author z00050
* @data 2019-9-9
**************************************************************************************************/
#ifndef BIZ_DATA_SAVE_H
#define BIZ_DATA_SAVE_H 1

#include <QThread>
#include <QDomElement> 

#include <freex_biz_unit/IFxUnitService.h>

#include <freex_entity_aircraft/AircraftCommon.h>
#include <freex_entity_ground/GroundCommon.h>
#include <freex_entity_spacecraft/SpacecraftCommon.h>

#include <freex_effect_rect_scan/RectScanCommon.h>
#include <freex_effect_cone_scan/ConeScanCommon.h>
#include <freex_effect_radar_beam/RadarBeamCommon.h>
#include <freex_effect_radar_scan/RadarScanCommon.h>
#include <freex_effect_link_line/LinkLineCommon.h>
#include <freex_effect_observe/ObserveCommon.h>
#include <freex_entity_sea/SeaCommon.h>
#include <freex_effect_highlight/HighlightCommon.h>
#include <freex_effect_concentric_rings/ConcentricRingsCommon.h>
#include <freex_effect_signal_jamming/SignalJammingCommon.h>
#include <freex_effect_camera_trace/CameraTraceCommon.h>
#include <freex_effect_camera_imaging/CameraImagingCommon.h>
#include <freex_effect_signal_enhance/SignalEnhanceCommon.h>
#include <freex_effect_signal_capture/SignalCaptureCommon.h>
#include <freex_effect_disturb/DisturbCommon.h>
/**
* @class CBizDataSave
* @brief 保存场景部署类
* @author g00037
*/
class CBizDataSave : public QThread
{  
	Q_OBJECT
public:  
	CBizDataSave(std::string strFilePath); 
	~CBizDataSave();  

public:	
	void SetFilePath(std::string strFilePath);

protected:
	//保存线程
	virtual void run();

	void elementCreate(QDomElement ele);

	void createObjectList(QDomElement ele);

	void createObjectEle(QDomElement ele, FxEntityObject::CFxEntityObjectEle* pObjectEle);

	//创建船舰对象构建元素
	void createSeaEle(QDomElement ele, FxBizUnit::CFxSeaEle* pSeaEle);

	//创建航空器对象构建元素
	void createAircraftEle(QDomElement ele, FxBizUnit::CFxAircraftEle* pAircraftEle);

	//创建地面站对象构建元素
	void createGroundEle(QDomElement ele, FxBizUnit::CFxGroundEle* pGroundEle);

	//创建卫星对象构建元素
	void createSpacecraftEle(QDomElement ele, FxBizUnit::CFxSpacecraftEle* pSpacecraftEle);
	
	//创建矩形凝视相机
	void createRectScanEle(QDomElement ele, FxBizUnit::CFxRectScanEle* pRectScanEle);

	//创建圆锥凝视相机
	void createConeScanEle(QDomElement ele, FxBizUnit::CFxConeScanEle* pConeScanEle);

	//创建相机成像特效
	void createCameraImagingEle(QDomElement ele, FxBizUnit::CFxCameraImagingEle* pCameraImagingEle);

	//创建相机追踪
	void createCameraTraceEle(QDomElement ele, FxBizUnit::CFxCameraTraceEle* pCameraTraceEle);

	//创建雷达扫描相机
	void createRadarBeamEle(QDomElement ele, FxBizUnit::CFxRadarBeamEle* pRadarBeamEle);

	//创建雷达扫描
	void createRadarScanEle(QDomElement ele, FxBizUnit::CFxRadarScanEle* pRadarScanEle);

	//创建通信线
	void createLinkLineEle(QDomElement ele, FxBizUnit::CFxLinkLineEle* pLinkLineEle);

	//创建高光特效
	void createHighLightEle(QDomElement ele, FxBizUnit::CFxHighlightEle* pHighLightEle);

	//创建干扰特效
	void createJammingEle(QDomElement ele, FxBizUnit::CFxSignalJammingEle* pJammingEle);
	
	//创建信号增强特效   
	void createEnhenceEle(QDomElement ele, FxBizUnit::CFxSignalEnhanceEle* pEnhenceEle);

	//创建信号捕获特效   
	void createCaptureEle(QDomElement ele, FxBizUnit::CFxSignalCaptureEle* pCaptureEle);

	//创建同心圆特效
	void createConcentricRingsEle(QDomElement ele, FxBizUnit::CFxConcentricRingsEle* pConcentricRingsEle);

	//创建同心圆特效
	void createDisturbEle(QDomElement ele, FxBizUnit::CFxDisturbEle* pDisturbEle);
	
	//创建观测特效
	void createObserveEle(QDomElement ele, FxBizUnit::CFxObserveEle* pObserveEle);

	//创建实体对象构建元素
	void createEntityEle(QDomElement ele, FxEntityObject::CFxEntityObjectEle*);

	//创建基础对象构建元素
	void createBaseEle(QDomElement ele, FxBaseObject::CFxObejctEle*);

	//创建文本对象构建元素
	void createTextEle(QDomElement ele, FxBaseObject::CFxTextObjectEle*);

	//创建图片对象构建元素
	void createImageEle(QDomElement ele, FxBaseObject::CFxImageObjectEle*);

	//创建模型对象构建元素
	void createModelEle(QDomElement ele, FxBaseObject::CFxModelObjectEle*);

	//创建点对象构建元素
	void createPointEle(QDomElement ele, FxBaseObject::CFxPointObjectEle*);

	//创建线对象构建元素
	void createPathEle(QDomElement ele, FxBaseObject::CFxLineObjectEle*);

	//创建卫星轨道SGP4数据
	void createSgp4DataEle(QDomElement ele, FxBizUnit::CFxSpacecraftEle* pSpacecraftEle);

	//创建飞机航线数据
	void createAirLineEle(QDomElement ele, FxBizUnit::CFxAircraftEle* pAircraftEle);

	//创建船舰航线数据
	void createSeaLineEle(QDomElement ele, FxBizUnit::CFxSeaEle* pSeaEle);

	//创建二向量
	void CreateVec2(QDomElement ele, osg::Vec2 vec, int prec);
	void CreateVec2d(QDomElement ele, osg::Vec2d vec, int prec);

	//创建三维向量
	void CreateVec3(QDomElement ele, osg::Vec3 vec, int prec);
	void CreateVec3d(QDomElement ele, osg::Vec3d vec, int prec);

	//创建四维向量
	void CreateVec4(QDomElement ele, osg::Vec4 vec, int prec);
	void CreateVec4d(QDomElement ele, osg::Vec4d vec, int prec);

	void CreateTimeText(QDomElement ele, double dJD);

	std::string DeleteDataPath(std::string);

	bool getService();

protected:
	std::string m_strFileName;

	QDomDocument m_doc;
	FxBizUnit::IFxBizUnitService* m_pBizUnitService;
}; 

#endif //BIZ_DATA_PARSE_H

