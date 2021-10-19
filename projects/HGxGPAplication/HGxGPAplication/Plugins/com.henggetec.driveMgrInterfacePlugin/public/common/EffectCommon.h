/**************************************************************************************************
* @file Common.h
* @note 常量、结构体定义
* @author 
* @date 2019-8-26
**************************************************************************************************/
#ifndef FX_EFFECT_COMMON_H
#define FX_EFFECT_COMMON_H
#include <FreeXAstronomy/Common.h>
#include <scene/SceneServiceUtil.h>
#include <FreeXEntityObject/EntityObject.h>
#include <freex_biz_unit/IFxUnitSceneService.h>
namespace FxBizUnit
{
	typedef std::vector<std::pair<double,double>> VecPari;
	typedef std::map<unsigned int,osg::observer_ptr<FxEntityObject::CFxEntityObject>> mapSceneEntity;
	class CFxEffectEle : public FxEntityObject::CFxEntityObjectEle
	{
	public:

		CFxEffectEle()
			:FxEntityObject::CFxEntityObjectEle()
			,m_bUseShader(false)
		{
			m_strEntityClass = "Effect";
		}

		virtual ~CFxEffectEle(){}

		virtual CFxEffectEle* clone(){return new CFxEffectEle;}

		virtual CFxEffectEle* cloneThis(){return new CFxEffectEle(*this);}
		
		virtual void update()
		{
			FxEntityObject::CFxEntityObjectEle::update();
			if (m_opParentEle.valid())
			{
				auto pParentEle = dynamic_cast<CFxEffectEle*>(m_opParentEle.get());
				if (pParentEle)
				{
					this->vecTimePari() = pParentEle->vecTimePari();
					this->useShader() = pParentEle->useShader();
				}
			}
		}

		VecPari& vecTimePari(){return m_vecTimePari;}

		bool& useShader(){return m_bUseShader;}

	protected:
		VecPari m_vecTimePari;
		bool    m_bUseShader;
	};

	class CFxEffectBase : public FxEntityObject::CFxEntityObject
	{
		
	public:
		CFxEffectBase(const unsigned int& unSceneID,FxEntityObject::CFxEntityObjectEle* pEle, FxEntityObject::CFxEntityObjectData* pObjectData,osgEarth::MapNode* pMapNode,osgViewer::View* pView)
			:FxEntityObject::CFxEntityObject(unSceneID,pEle,pObjectData,pMapNode,pView)
			,m_opEffectEle(dynamic_cast<CFxEffectEle*>(pEle))
			,m_pUnitService(nullptr)
		{
			FxScene::CFxSceneServiceQuerier<IFxSceneUnitService> unitService(unSceneID);
			if (unitService)
			{
				m_pUnitService = unitService.get();
				/*auto serviceRef = unitService.serviceRef();
				if (serviceRef)
				{
					m_pUnitService = serviceRef.toService<IFxSceneUnitService>();
				}*/
			}
		}

	protected:
		virtual void updateObjectEle()
		{

		}

		virtual void traverse(osg::NodeVisitor& nv)
		{
			bool bIn = true;
			auto vecTimePair = &m_opEffectEle->vecTimePari();
			if (vecTimePair->size()>0)
			{
				bIn = false;
			}
			for (auto itr=vecTimePair->begin();itr!=vecTimePair->end();++itr)
			{
				if (s_dCurUTC_JD>=std::min(itr->first,itr->second)&&s_dCurUTC_JD<std::max(itr->first,itr->second))
				{
					bIn = true;
					break;
				}
			}
			if (bIn)
			{
				FxEntityObject::CFxEntityObject::traverse(nv);
			}
		}

	protected:
		osg::observer_ptr<CFxEffectEle>  m_opEffectEle;
		IFxSceneUnitService*             m_pUnitService;
	};
	
}

//todo:
struct SRadarParam
{
    osg::Vec4d   vecColor;
    bool		 bDrawRange;
    bool		 bDrawScan;
    double		 dRadius;
    double		 dScanSpeed;
    double		 dAzimuthBegin;
    double       dAzimuthEnd;
    double		 dPitchBegin;
    double		 dPitchEnd;

    
    double       dleftAngle;
    double       dRightAngle;
    double       dUpAngle;
    double       dDownAngle;
};
enum ERadarScanType
{
	VERTICAL_SCANTYPE,
	HORIZONTAL_SCANTYPE,
	ROTATION_SCANTYPE,
	STATIC_SCANTYPE,
	SWING_SCANTYPE,
};
enum EffectType
{

};
typedef std::pair<double, double> PairStartTime2EndTime;
typedef std::vector<PairStartTime2EndTime> VectorPairTimes;
//传感器信息
	struct SSensorInfo
	{
		std::string strName;
		std::string strID;
		std::string strSender;
		EffectType  eEffectType;
		osg::Vec2d	vec2dSensorPara;
		osg::Vec3d  vecSensorColor;

        //todo:
        SRadarParam sRadarPara;
		
	};

	struct SActInfo
	{
		SActInfo()
		{
			strReceiver = "";
			strActId = "";
		}
		SSensorInfo sSensorInfo;		
		std::string	strReceiver;
		std::string	strActId;
		VectorPairTimes vecTimes;

        //todo:
        osg::Vec3d   vec3dPosture;
	};

#endif // FX_EFFECT_COMMON_H
