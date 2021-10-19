/**************************************************************************************************
* @file CBizDataSave.h
* @note ���泡�������ļ�
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
* @brief ���泡��������
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
	//�����߳�
	virtual void run();

	void elementCreate(QDomElement ele);

	void createObjectList(QDomElement ele);

	void createObjectEle(QDomElement ele, FxEntityObject::CFxEntityObjectEle* pObjectEle);

	//�����������󹹽�Ԫ��
	void createSeaEle(QDomElement ele, FxBizUnit::CFxSeaEle* pSeaEle);

	//�������������󹹽�Ԫ��
	void createAircraftEle(QDomElement ele, FxBizUnit::CFxAircraftEle* pAircraftEle);

	//��������վ���󹹽�Ԫ��
	void createGroundEle(QDomElement ele, FxBizUnit::CFxGroundEle* pGroundEle);

	//�������Ƕ��󹹽�Ԫ��
	void createSpacecraftEle(QDomElement ele, FxBizUnit::CFxSpacecraftEle* pSpacecraftEle);
	
	//���������������
	void createRectScanEle(QDomElement ele, FxBizUnit::CFxRectScanEle* pRectScanEle);

	//����Բ׶�������
	void createConeScanEle(QDomElement ele, FxBizUnit::CFxConeScanEle* pConeScanEle);

	//�������������Ч
	void createCameraImagingEle(QDomElement ele, FxBizUnit::CFxCameraImagingEle* pCameraImagingEle);

	//�������׷��
	void createCameraTraceEle(QDomElement ele, FxBizUnit::CFxCameraTraceEle* pCameraTraceEle);

	//�����״�ɨ�����
	void createRadarBeamEle(QDomElement ele, FxBizUnit::CFxRadarBeamEle* pRadarBeamEle);

	//�����״�ɨ��
	void createRadarScanEle(QDomElement ele, FxBizUnit::CFxRadarScanEle* pRadarScanEle);

	//����ͨ����
	void createLinkLineEle(QDomElement ele, FxBizUnit::CFxLinkLineEle* pLinkLineEle);

	//�����߹���Ч
	void createHighLightEle(QDomElement ele, FxBizUnit::CFxHighlightEle* pHighLightEle);

	//����������Ч
	void createJammingEle(QDomElement ele, FxBizUnit::CFxSignalJammingEle* pJammingEle);
	
	//�����ź���ǿ��Ч   
	void createEnhenceEle(QDomElement ele, FxBizUnit::CFxSignalEnhanceEle* pEnhenceEle);

	//�����źŲ�����Ч   
	void createCaptureEle(QDomElement ele, FxBizUnit::CFxSignalCaptureEle* pCaptureEle);

	//����ͬ��Բ��Ч
	void createConcentricRingsEle(QDomElement ele, FxBizUnit::CFxConcentricRingsEle* pConcentricRingsEle);

	//����ͬ��Բ��Ч
	void createDisturbEle(QDomElement ele, FxBizUnit::CFxDisturbEle* pDisturbEle);
	
	//�����۲���Ч
	void createObserveEle(QDomElement ele, FxBizUnit::CFxObserveEle* pObserveEle);

	//����ʵ����󹹽�Ԫ��
	void createEntityEle(QDomElement ele, FxEntityObject::CFxEntityObjectEle*);

	//�����������󹹽�Ԫ��
	void createBaseEle(QDomElement ele, FxBaseObject::CFxObejctEle*);

	//�����ı����󹹽�Ԫ��
	void createTextEle(QDomElement ele, FxBaseObject::CFxTextObjectEle*);

	//����ͼƬ���󹹽�Ԫ��
	void createImageEle(QDomElement ele, FxBaseObject::CFxImageObjectEle*);

	//����ģ�Ͷ��󹹽�Ԫ��
	void createModelEle(QDomElement ele, FxBaseObject::CFxModelObjectEle*);

	//��������󹹽�Ԫ��
	void createPointEle(QDomElement ele, FxBaseObject::CFxPointObjectEle*);

	//�����߶��󹹽�Ԫ��
	void createPathEle(QDomElement ele, FxBaseObject::CFxLineObjectEle*);

	//�������ǹ��SGP4����
	void createSgp4DataEle(QDomElement ele, FxBizUnit::CFxSpacecraftEle* pSpacecraftEle);

	//�����ɻ���������
	void createAirLineEle(QDomElement ele, FxBizUnit::CFxAircraftEle* pAircraftEle);

	//����������������
	void createSeaLineEle(QDomElement ele, FxBizUnit::CFxSeaEle* pSeaEle);

	//����������
	void CreateVec2(QDomElement ele, osg::Vec2 vec, int prec);
	void CreateVec2d(QDomElement ele, osg::Vec2d vec, int prec);

	//������ά����
	void CreateVec3(QDomElement ele, osg::Vec3 vec, int prec);
	void CreateVec3d(QDomElement ele, osg::Vec3d vec, int prec);

	//������ά����
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

