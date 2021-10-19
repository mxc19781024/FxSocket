/**************************************************************************************************
* @file CBizDataParse.h
* @note 部署文件解析
* @author z00050
* @data 2019-9-9
**************************************************************************************************/
#ifndef BIZ_DATA_PARSE_H
#define BIZ_DATA_PARSE_H 1
#include <map>
#include <math.h>
#include <QThread>
#include <QString>
#include <QDomElement>
#include <public/common/UnitCommon.h>
#include <osg/Vec2>
#include <osg/Vec2d>
#include <osg/Vec3>
#include <osg/Vec3d>
#include <osg/Vec4>
#include <osg/Vec4d>


#include "PlatformPlugin/object/GxCameraImagingOption.h"
#include "PlatformPlugin/object/GxConeScanOption.h"
#include "PlatformPlugin/object/GxDisturbOption.h"
#include "PlatformPlugin/object/GxLinkLineOption.h"
#include "PlatformPlugin/object/GxLinkTubularOption.h"
#include "PlatformPlugin/object/GxRadarScanOption.h"
#include "PlatformPlugin/object/GxRadarBeamOption.h"
#include "PlatformPlugin/object/GxRectScanOptiong.h"
#include "PlatformPlugin/object/GxSignalJammingOption.h"
#include "PlatformPlugin/object/GxConcentricRingsOption.h"
#include "bdpCommon/GxTimeUtil.h"


/**
* @class CBizDataParse
* @brief 部署文件解析�?
* @author g00037
*/
class CBizDataParse : public QThread
{  
	Q_OBJECT
public:  
	CBizDataParse(std::string strFilePath); 
	~CBizDataParse();  

public:	
	void SetFilePath(std::string strFilePath);

	//设置装备配置信息
	void SetEquipInfoData(MapEquipInfoData   mapEquipInfoData);

	//得到仿真开始时�?
	double GetSimuStartTime();

	//得到仿真结束时间
	double GetSimuStopTime();

	//清空已有的数�?
	void ClearData();

	std::map<float, RoleCommandVec>	 GetMapIndexRoleCommandData();
	std::map<float, vecZaijiRadarState> GetMapIndexSenorCommandData();
	std::map<int, VectorDriverAircraftObjectsData> GetMapIndexToVecDriverData();
protected:
	//解析线程
	virtual void run();

	//解析
	bool elementParse(const QDomElement& ele);

	void parseObjectList(const QDomElement& ele);

	/**  
	  * @brief 读取数据驱动文件
	  * @note  读取数据驱动文件
	  * @param [in] strPath 文件路径
	  * @return 返回�?
	*/
	bool readDriverFile(std::string strPath);
	bool ReadRoleCommand(std::string strPath);
	bool ReadSenorCommand(std::string strPath);
 	void parseObjectEle(const QDomElement& ele);
	
	void parseSpacecraftObjectEle(const QDomElement& ele, SSpacecraftObjectEleData &sSpacecraftEleData);
	void parseSgp4Data(const QDomElement& ele, SSpacecraftObjectEleData &sSpacecraftEleData);
	//解析实体对象构建元素
	void parseEntityEle(const QDomElement& ele, SEntityObjectEleData &sEleData);

	//解析基础对象构建元素
	void parseBaseEle(const QDomElement& ele, SObjectEleData &sEleData);

	//解析文本对象构建元素
	void parseTextEle(const QDomElement& ele, STextObjectEleData &sEleData);

	//解析图片对象构建元素
	void parseImageEle(const QDomElement& ele, SImageObjectEleData &sEleData);

	//解析模型对象构建元素
	void parseModelEle(const QDomElement& ele, SModelObjectEleData &sEleData);

	//解析点对象构建元�?
	void parsePointEle(const QDomElement& ele, SPointObjectEleData &sEleData);

	//解析线对象构建元�?
	void parsePathEle(const QDomElement& ele, SLineObjectEleData &sEleData);


	//解析特效的显示时间列�?
	std::vector<std::pair<double,double>> parseEffectTimepari(const QDomElement& ele);

	//解析相机成像特效构建元素
	void parseCameraImgEle(const QDomElement& ele, HGxObject::CGxCameraImagingOption& sEleData);

	//解析锥形扫描雷达特效构建元素
	void parseConeScanEle(const QDomElement& ele, HGxObject::CGxConeScanOption& sEleData);

	//解析干扰特效构建元素
	void parseDisturbEle(const QDomElement& ele, HGxObject::CGxDisturbOption& sEleData);

	//解析通信线特效构建元�?
	void parseLinkLineEle(const QDomElement& ele, HGxObject::CGxLinklineOption& sEleData);

	//解析通信管道特效构建元素
	void parseLinkTubularEle(const QDomElement& ele, HGxObject::CGxLinkTubularOption& sEleData);

	//解析雷达扫描特效构建元素
	void parseRadarScanEle(const QDomElement& ele, HGxObject::CGxRadarScanOption& sEleData);

	//解析雷达波束特效构建元素
	void parseRadarBeamEle(const QDomElement& ele, HGxObject::CGxRadarBeamOption& sEleData);

	//解析矩形扫描特效构建元素
	void parseRectScanEle(const QDomElement& ele, HGxObject::CGxRectScanEle& sEleData);

	//解析信号干扰特效构建元素
	void parseSignalJammingEle(const QDomElement& ele, HGxObject::CGxSignalJammingOption& sEleData);
	void parseMapVecPari2IntensityELe(const QDomElement& ele, HGxObject::CGxSignalJammingOption& sEleData);

	//解析同心圆环特效构建元素
	void parseConcentricRing(const QDomElement& ele, HGxObject::CGxConcentricRingsOption& sEleData);


	//解析地面站构建元�?
	void parseGround(const QDomElement& ele, SNewObjectData& objectData);

	double  parseTimeText(const QString& strTime);

	QString  createTimeText(double dJD);

	//解析二维向量
	osg::Vec2  parseVec2(const QString& strVec);
	osg::Vec2d  parseVec2d(const QString& strVec);

	//创建二维向量
	QString  createVec2(osg::Vec2 vec, int prec);
	QString  createVec2d(osg::Vec2d vec, int prec);

	//解析三维向量
	osg::Vec3  parseVec3(const QString& strVec);
	osg::Vec3d  parseVec3d(const QString& strVec);

	//创建三维向量
	QString  createVec3(osg::Vec3 vec, int prec);
	QString  createVec3d(osg::Vec3d vec, int prec);

	//解析四维向量
	osg::Vec4  parseVec4(const QString& strVec);
	osg::Vec4d  parseVec4d(const QString& strVec);

	//创建四维向量
	QString  createVec4(osg::Vec4 vec, int prec);
	QString  createVec4d(osg::Vec4d vec, int prec);
signals:
	void SignalBarValue(int nValue);

	void SignalFileParseFinish(bool bState);

	void SignalsendDriverData();

private:  
	std::string		m_strFileName;     //部署文件�?
	std::string		m_strFilePath;     //部署文件路径
	double			m_dSimuBeginTime;  //仿真开始时�?
	double			m_dSimuEndTime;    //仿真结束时间
	std::string		m_strDriverPath;    //数据驱动路径
	MapEquipInfoData   m_mapEquipInfoData;//装备配置信息
	std::map<int,VectorDriverAircraftObjectsData> m_mapIndexToVecDriverData;

	VectorSpacecraftObjectsData m_vecdriverData;
	std::vector<osg::ref_ptr<HGxObject::CGxEffectEle>> m_vecEffectData;

	std::map<float, RoleCommandVec>				m_mapTimeToRoleCommand;//队形指令数据 0.1s倍速数�?
	std::map<float, vecZaijiRadarState>         m_mapTimeToZaijiRadarStatus;//载机雷达状态改�?
}; 

#endif //BIZ_DATA_PARSE_H

