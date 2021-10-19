/**************************************************************************************************
* @file UnitStructDef.h
* @note ʵ�������������ṹ�嶨��
* @author 
* @date 2019-8-26
**************************************************************************************************/
#ifndef FX_UNIT_STRUCT_DEF_H
#define FX_UNIT_STRUCT_DEF_H

#include <map>
#include <osg/Vec4>
#include <osg/Vec3d>
#include <osg/Vec2>
#include "StructDef.h"

//��Ԫ��ɫ������ָ��
struct RoleCommand
{
	short C1;//����ָ�1:��λ��2���Կ���3���״�Эͬ��7:���Σ�10������ָ���ػ���ȷ������������ϵͳ�ͽ���������  ��ֵ50ʱΪ�������ָ��Ŵ���
	short C2;//����ָ�0���أ�1��������2:������4:������5����������
	std::string recplatfmID;//����ƽ̨
	std::string WorkState;//
	double SimTimeSec;	//�´�ָ���ϵͳ����ʱ�䣬��
	std::string strNumbCo;//�����ڷɻ���
};
//����ָ���
typedef std::vector<RoleCommand> RoleCommandVec;

	//�ػ��״�״̬��¼
	struct ZaijiRadarState
	{
		double SimuTime;	//����ʱ�䣬��
		short ValidRadiatFlag;//�з����־��0�ޣ�1��
		short Shapetype;	//�������ͣ�1��˫Բ׶�ṹ��2��˫Բ��ֱ��ṹ
		std::string IDbelong;//�״����ڷɻ��ķɻ���ʶ��
		float yawCurline;	//�״ﲨ��˲ʱָ�򣬷�λ�ǣ��Ի�������Ϊ���ģ������ȣ�˳ʱ������ֵ��-pi��+pi]��
		float picthCurline;	//�״ﲨ��˲ʱָ��ˮƽ�����ǣ����ȣ���-pi/2��+pi/2]
		float yawCnline;	//�״������߼����꣬��λ�ǣ�ֵ��-pi��+pi]�����״������ߵļ�����
		float picthCnline;	//�״������߼����꣬���ǣ���-pi/2��+pi/2]
		float BeamNearDis;//�ٽ��磬��
		float dectin;	//���磬��
		float BeamMidDis;//�н磬��
		float dectout;	//Զ�磬�ס������ڻ�׼RCS=1ƽ��ʱ��ֵ������-�ס�
		float alpha;	//˫Բ׶�ṹʱ���붥׶�ǣ�[0��pi)��
		//˫Բ��ֱƽ���ṹʱ��Ϊˮƽ�ǣ�[0��pi/2��
		float beta;		//˫Բ׶�ṹʱ����Ч��˫Բ��ֱƽ���ṹʱ��Ϊ��ֱ��,[0,pi/2)��	
	};
	//�ػ��״�״̬����
	typedef std::vector<ZaijiRadarState> vecZaijiRadarState;

//װ�����˳̶�
enum EDegreeDamage
{

};

//����װ������
struct SPreEquipCap
{
	double      dHit;       //װ���������
	double      dCruise;    //װ��Ѳ������
	double      dPerformance;//װ������
	double      dEndurance;  //װ����������
	double      dCoordinate; //װ��Э������
	double      dProbe;      //װ��̽������
};

//����װ��������Ϣ
struct SPreEquipInfo
{
	std::string name;            //װ������
	std::string strtype;         //װ������
	std::string description;     //װ������
	std::string model_path;      //װ����άģ��·��
	std::string image_path;      //װ����άͼƬ·��
	double      maxVoyage;       //װ����󺽳�
	double      maxSpeed;        //װ������ٶ�
	double      maxOil;          //װ���������
	int         ammunitionNumber;//װ��Я����ҩ����
	std::string combatCapability;//��ս����
	std::string degreeDamage;    //װ�����˳̶�
	SPreEquipCap sPreEquipCap;   //��ս����
};
//װ����Ԫ���ݼ���
typedef std::map<std::string,SPreEquipInfo> MapEquipInfoData;

//����������
struct SSenor
{
	// �״�
	Senor senor[SENOR_MAX_NUM];
};

//����ʵ�������������
struct SNewObjectData
{
	SNewObjectData()
	{
		dTime = 0.0;
		//memset(strID,'\0, sizeof(strID));
		//memset(strName,'\0', sizeof(strName));
		nameVisible = 1;
		nameFontSize = 20.0;
		memset(nameColor, 1, sizeof(nameColor));
		//memset(strModelFilePath,'\0', sizeof(strModelFilePath));
		nType = 0;
		modelAuto = 1;
		nDriveStyle = 0;
		memset(modelColor, 1, sizeof(modelColor));
		//memset(strImageFilePath,'\0', sizeof(strImageFilePath));	
		memset(imageImageSize, 64, sizeof(imageImageSize));
		memset(modelScale, 10, sizeof(modelScale));
		memset(vecPositionLLH, 0, sizeof(vecPositionLLH));
		memset(vecPosture, 0, sizeof(vecPosture));
	}
	double		 dTime;
	char	     strID[64];            //ʵ��ΨһId
	char	     parentID[64];         //�ϼ�ID ͨ��ָ�ӹ�ϵ
	char	     strName[128];         //ʵ������
	int          nameVisible;		   //ʵ��������Ӱ��0������ʾ��1����ʾ
	int          nameFontSize;		   //ʵ�������ֺ�
	double	     nameColor[4];		   //ʵ��������ɫ

	EnCamp enCamp;// ��Ӫ 1-�췽 2-���� 3-����	 
	EnAirspace enSpaceType;// ʵ���������� 1-����װ�� 2-½��װ�� 3-����װ��	
	EnEquType enEquType;// 1-�ɻ� 2-̹�� 3-�������䳵 4-���� 5-��ĸ 6-������	
	SSenor    sSenor;// ������
	EnConeState enCommuState;// ͨ��״̬ 1-��ͨ 2-�Ͽ�				 
	SepareSubInfo sepInfo[MAX_SEPSUB_NUM];// ����ʵ��id
	SPreEquipInfo sPreEquipInfo;      //ʵ��������Ϣ


	char   	     strModelFilePath[512];//ʵ��ģ��·��
	int          nType;                //ʵ�����ͣ�0���׷���1���췽��2������
	int          modelAuto;			   //ʵ���Ƿ��Զ����ţ�0����ֹ���ţ�1����������
	double	     modelColor[4];		   //ʵ��Ⱦɫ��ɫ
	char   	     strImageFilePath[512];//ʵ��ģ��ͼƬ·��
	double	     imageImageSize[2];	   //ʵ��ģ��ͼƬ��С

	double       modelScale[3];		   //ʵ��ģ�����Ŵ�С
	double       vecPositionLLH[3];    //ʵ�嵱ǰλ�ã� ��γ�ߣ�
	double       vecPosture[3];        //ʵ�嵱ǰ��̬,ƫ�������������

	int          nDriveStyle;          //ʵ��������ʽ

};

// ״̬ʵʱλ����̬
struct SDriveObjectData
{
	SDriveObjectData()
	{
		dTime = 0.0;
		nDriveStyle = 0;
		memset(modelColor, 1, sizeof(modelColor));
		memset(vecPositionLLH, 0, sizeof(vecPositionLLH));
		memset(vecPosture, 0, sizeof(vecPosture));
	}
	double		 dTime;
	char	     strID[64];            //ʵ��ΨһId

	EnCamp enCamp;// ��Ӫ 1-�췽 2-���� 3-����	 
	EnAirspace enSpaceType;// ʵ���������� 1-����װ�� 2-½��װ�� 3-����װ��	
	EnEquType enEquType;// 1-�ɻ� 2-̹�� 3-�������䳵 4-���� 5-��ĸ 6-������	
	SSenor    sSenor;// ������
	EnConeState enCommuState;// ͨ��״̬ 1-��ͨ 2-�Ͽ�				 
	SepareSubInfo sepInfo[MAX_SEPSUB_NUM];// ����ʵ��id

	double	     modelColor[4];		   //ʵ��Ⱦɫ��ɫ
	double       vecPositionLLH[3];    //ʵ�嵱ǰλ�ã� ��γ�ߣ�
	double       vecPosture[3];        //ʵ�嵱ǰ��̬,ƫ�������������
	int          nDriveStyle;          //ʵ��������ʽ

};

//����ʵ�������������
struct SDriverAircraftObjectData
{
	SDriverAircraftObjectData()
	{
		dTime = 0.0; 
		//memset(strID,'\0, sizeof(strID));
		//memset(strName,'\0', sizeof(strName));
		nameVisible = 1;		  
		nameFontSize = 20.0;		  
		memset(nameColor,1, sizeof(nameColor));
		//memset(strModelFilePath,'\0', sizeof(strModelFilePath));
		nType = 0;               
		modelAuto = 1;		
		nDriveStyle = 0;
		memset(modelColor,1, sizeof(modelColor));
		//memset(strImageFilePath,'\0', sizeof(strImageFilePath));	
		memset(imageImageSize,64, sizeof(imageImageSize));
		memset(modelScale,10, sizeof(modelScale));
		memset(vecPositionLLH,0, sizeof(vecPositionLLH));
		memset(vecPosture,0, sizeof(vecPosture));
	}
	double		 dTime; 
	char	     strID[64];            //ʵ��ΨһId
	char	     parentID[64];         //�ϼ�ID ͨ��ָ�ӹ�ϵ
	char	     strName[128];         //ʵ������
	int          nameVisible;		   //ʵ��������Ӱ��0������ʾ��1����ʾ
	int          nameFontSize;		   //ʵ�������ֺ�
	double	     nameColor[4];		   //ʵ��������ɫ
									   
	EnCamp enCamp;// ��Ӫ 1-�췽 2-���� 3-����	 
	EnAirspace enSpaceType;// ʵ���������� 1-����װ�� 2-½��װ�� 3-����װ��	
	EnEquType enEquType;// 1-�ɻ� 2-̹�� 3-�������䳵 4-���� 5-��ĸ 6-������	
	SSenor    sSenor;// ������
	EnConeState enCommuState;// ͨ��״̬ 1-��ͨ 2-�Ͽ�				 
	SepareSubInfo sepInfo[MAX_SEPSUB_NUM];// ����ʵ��id
	SPreEquipInfo sPreEquipInfo;      //ʵ��������Ϣ


	char   	     strModelFilePath[512];//ʵ��ģ��·��
	int          nType;                //ʵ�����ͣ�0���׷���1���췽��2������
	int          modelAuto;			   //ʵ���Ƿ��Զ����ţ�0����ֹ���ţ�1����������
	double	     modelColor[4];		   //ʵ��Ⱦɫ��ɫ
	char   	     strImageFilePath[512];//ʵ��ģ��ͼƬ·��
	double	     imageImageSize[2];	   //ʵ��ģ��ͼƬ��С

	double       modelScale[3];		   //ʵ��ģ�����Ŵ�С
	double       vecPositionLLH[3];    //ʵ�嵱ǰλ�ã� ��γ�ߣ�
	double       vecPosture[3];        //ʵ�嵱ǰ��̬,ƫ�������������
	
	int          nDriveStyle;          //ʵ��������ʽ

};

// ��������
struct HitData
{
	HitData()
	{
		dTime = 0;
	}

	~HitData()
	{
		dTime = 0;
	}

	double		 dTime;
	char	     strID[64];            //ʵ��ΨһId
	char	     nTargetId[64];            //ʵ��ΨһId
};

// �¼�����
struct EventData
{
	EventData()
	{
		dTime = 0;
		memset(chDescript, 0, 200);
	}

	~EventData()
	{
		dTime = 0;
		memset(chDescript, 0, 200);
	}

	double dTime;
	char chDescript[200];
};

// enum ECamp
// {
// 	InitObject = 0,
// 	RedObject = 1,
// 	BlueObject = 2,
// };


struct SObjectEleData
{
	bool                    bVisible;
	osg::Vec3d		        vecPositionLLH;
	osg::Vec3d		        vecPosture;
};

struct STextObjectEleData
{
	std::string		        strText;      //ʵ������
	osg::Vec4               vecColor;
	std::string             strFont;
	float                   fFontSize;
	bool                    bOutLine;
	osg::Vec4               vecOutLineColor;
	osg::Vec3d              vecOffset;
	bool                    bAutoScale;
	bool                    bAutoRotate;
};

struct SImageObjectEleData
{
	std::string		        strImageFilePath;   //ʵ��ͼƬ
	osg::Vec2               vecImageSize;
	float                   fOpacity;
	bool                    bAutoScale;
	bool                    bAutoRotate;
};

/**
* @brief ģ������
*/
enum EModelType
{
	GENARAL = 0,
	ANIMATIONPATH = 1,
	COMBINED = 2,
};
struct SModelObjectEleData
{
	std::string		        strFilePath;   //ʵ��ģ��
	osg::Vec3d              vecScale;//ģ������
	osg::Vec3d              vecPosition;//ģ������
	osg::Vec3d              vecPosture;//ģ������
	bool                    bAutoScale;
	bool                    bAutoRotate;
	bool                    bUseColor;
	osg::Vec4               vecColor;
	EModelType				eModelType;
	bool					bBlend; //��ɫ���
};

struct SPointObjectEleData
{
	float        fPointSize;
	osg::Vec4    vecColor;
};

/**
* @brief ����
*/
enum ELineType
{
	E_FULLLINE = 0,
	E_IMAGINRYLINE = 1,
	E_POINTS = 2,
	E_POINTLINE = 3,
};

struct SLineObjectEleData
{
	bool         bVisible;
	float        fPointSize;
	osg::Vec4    vecColor;
	ELineType    eLineType;
	float        fLineWidth;
	bool         bLoop;

};

#include <gdpAlgor/GxAstronomyCommon.h>

enum ESpacecraftOrbitMode
{
	SatStaticOrbit = 0,
	SatDynamicOrbit = 1,
};
struct SSpacecraftObjectEleData
{
	ESpacecraftOrbitMode     eOrbitMode;
	HGxAlgor::SSgp4Data      stSgp4Data;
};

struct SEntityObjectEleData
{
	std::string		        strID;	    //ʵ��ID
	std::string		        strParentID;//���ڵ�ID
	std::string             strEntityType;    //ʵ������
	std::string             strEntityClass;    //ʵ������
	bool                    bDynamic;
	EnCamp                  eCamp;      //��Ӫ
	SObjectEleData          sObjectEleData;
	STextObjectEleData      sTextObjectEleData;
	SImageObjectEleData     sImageObjectEleData;
	SModelObjectEleData     sModelObjectEleData;
	SPointObjectEleData     sPointObjectEleData;
	SLineObjectEleData      sLineObjectEleData;
};

//����ʵ�������������
struct SNewSpacecraftObjectEleData
{
	SSpacecraftObjectEleData sSpacecraftObjectEleData;
	SEntityObjectEleData sEntityObjectEleData;
};

typedef std::vector<std::string> VectorDestroyUnitData;

//ʵ������������ݼ���
typedef std::vector<SDriverAircraftObjectData> VectorDriverAircraftObjectsData;
//ʵ������������ݼ���
typedef std::vector<SNewSpacecraftObjectEleData> VectorSpacecraftObjectsData;
//ʵ������������ݼ���
typedef std::vector<SDriverAircraftObjectData> VectorDriverAircraftObjectsData;
//������ʵ������������ݼ���
typedef std::vector<SDriverAircraftObjectData> VectorDriverSpacecraftObjectsData;


#endif // FX_EFFECT_COMMON_H
