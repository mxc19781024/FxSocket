/**************************************************************************************************
* @file UnitStructDef.h
* @note ʵ�������������ṹ�嶨��
* @author 
* @date 2021-7-10
**************************************************************************************************/
#ifndef FX_UNIT_STRUCT_DEF_H
#define FX_UNIT_STRUCT_DEF_H

#include <map>
#include <osg/Vec4>
#include <osg/Vec3d>
#include <osg/Vec2>
#include "SenorStructDef.h"

// ��Ϣ��󳤶�
#define MAX_MSG_LEN 1000
// ��Ϣ���� 
enum MsgType
{
	NewEntityMsg = 0,  // �½�ʵ��
	EntityStuMsg = 1,  // ʵ��״̬
	DetecTargetMsg = 2,// ����Ŀ��
	DestoryMsg = 3,    // ��������
	UiStartMsg = 4,    // ��ʾ�˿�ʼ����
	EnventMsg = 5,     // �¼���Ϣ	
	ViewMsg = 6,       // �ӵ���Ϣ
	TimeCommandMsg = 7,// ʱ����Ϣ
	ScreenWordMsg = 8, // ��Ļ��Ϣ	
	HitMsg = 9,        // �����¼�	
	SenorMsg = 10,     // ��������Ϣ	
	SimInfoMsg = 11,   // ������Ϣ	
	InitSimMsg = 12,   // �����ʼ����Ϣ
	AimMsg = 13,       // ��׼����
	AirSpaceMsg = 14,  // ����
	SpeSubMsg = 15     // ʵ�������Ϣ
};


// ��Ϣͷ
struct MsgHead
{
	MsgType  type;    // ��Ϣ����
	unsigned dataLen;// ����������
	double   dTime;
};

struct ComMsg
{
	MsgHead msgHead;
	char    data[MAX_MSG_LEN];
};


//���ָ�������
struct STextAttach
{
	STextAttach()
	{
		fFontSize = 16;
		//memset(strID,'\0, sizeof(strID));
		//memset(strName,'\0', sizeof(strName));
		bOutLine = 1;
		bAutoScale = 1;
		bAutoRotate = 1;
		memset(vecColor, 4, sizeof(vecColor));
		memset(strFont, 1024, sizeof(strFont));
		memset(vecOutLineColor, 4, sizeof(vecOutLineColor));
		memset(vecOffset, 3, sizeof(vecOffset));
	}
	double                  vecColor[4];       //������ɫ
	char                    strFont[1024];     //��������
	float                   fFontSize;         //������ʾ��С
	bool                    bOutLine;          //�Ƿ���ʾ�߿���
	double                  vecOutLineColor[4];//���ֱ߿�����ɫ
	double                  vecOffset[3];      //����������ĵ��ƫ����
	bool                    bAutoScale;        //�����Ƿ��Զ�����
	bool                    bAutoRotate;       //�����Ƿ��Զ���ת
};

//ͼƬ��������
struct SImageAttach
{
	double                  vecImageSize[2];   //ͼƬ��С�����ߣ�
	float                   fOpacity;          //ͼƬ͸����
	bool                    bAutoScale;        //ͼƬ�Ƿ��Զ�����
	bool                    bAutoRotate;	   //ͼƬ�Ƿ��Զ���ת
};

//ģ�͸�������
struct SModelAttach
{
	bool                    bAutoScale;        //ģ���Ƿ��Զ�����
	bool                    bAutoRotate;       //ģ���Ƿ��Զ���ת
	bool                    bUseColor;         //ģ���Ƿ�ʹ����ɫ
	double                  vecColor[4];       //ģ����ɫ
	bool					bBlend;            //ģ����ɫ���
};

//�㸽������
struct SPointAttach
{
	float        fPointSize;   //��Ĵ�С
	double       vecColor[4];  //�����ɫ
};

//β����������:����
enum ELineType
{
	E_FULLLINE = 0,
	E_IMAGINRYLINE = 1,
	E_POINTS = 2,
	E_POINTLINE = 3,
};

//β����������
struct SLineAttach
{
	float        fPointSize;   //β����ģʽ�£���Ĵ�С
	double       vecColor[4];  //β����ɫ
	ELineType    eLineType;    //β���ߵ����ͣ��㡢�ߡ����ߣ�
	float        fLineWidth;   //β���ߵ��߿�
	bool         bLoop;        //β�����Ƿ�ջ�
};

// ������Ӫ���������ԣ�
enum EnCamp
{
	RED_CAMP = 1,     // �췽
	BLUE_CAMP = 2,    // ����
	UNKNOWN_CAMP = 3  // δ֪ ����
};

// �������򣨸������ԣ�
enum EnAirspace
{
	SEA_SPACE = 1,   // ��
	GROUND_SPACE = 2,// ½
	AIR_SPACE = 3    // ��
};

// װ�����ͣ��������ԣ�
enum EnEquType
{
	AIRCAFT_EUT = 1,         //�ɻ�
	TANK_EUT = 2,            //̹��
	MISSILE_VECHICLE_EUT = 3,//�������䳵
	MISSILE_EUT = 4,         //����
	CARRIER_EUT = 5,         //��ĸ
	FRIGATE_EUT = 6,         //������
	COPTER_EUT = 7,          //ֱ����
	GROUND_EUT = 8           //����վ
};

//����������
struct SSenor
{
	RadarSenor       sRadarSenor; //�״���Ϣ
};

//ʵ���������
struct SPreUnitBsaeInfo
{
	char		            strID[64];               //ʵ��ID,�����Ծ���Ψһ��
	char	                parentID[64];            //�ϼ�ID ͨ��ָ�ӹ�ϵ
	char		            strNameText[128];        //ʵ������
	char		            strImageFilePath[1024];  //ʵ��ͼƬ·��
	char		            strModelFilePath[1024];  //ʵ��ģ��·��
	double                  vecModelScale[3];        //ʵ��ģ������
	double                  vecModelPosition[3];     //ʵ��ģ��λ�ã���γ�ߣ�
	double                  vecModelPosture[3];      //ʵ��ģ����̬����������ת��ƫ������λ���ȣ�
	bool                    bUnitVisible;            //ʵ������
	bool                    bNameVisible;            //ʵ����������
	bool                    bImageVisible;           //ʵ��ͼƬ����
	bool                    bModelVisible;           //ʵ��ģ������
	bool                    bPointVisible;           //ʵ�������
	bool                    bLineVisible;            //ʵ��β������
	bool                    bLabelVisible;           //ʵ���������
	bool                    bDynamic;                //
};

//ʵ�帽������
struct SPreUnitAttachInfo
{
	EnCamp           eCamp;       //������Ӫ���������ԣ�
	SSenor           sSSenor;     //��������Ϣ���������ԣ�
	EnAirspace       eEnAirspace; //�������򣨸������ԣ�
	EnEquType        eEnEquType;  //װ�����ͣ��������ԣ�
	STextAttach      sTextAttach; //���ָ������ԣ��������ԣ�
	SImageAttach     sImageAttach;//ͼƬ�������ԣ��������ԣ�
	SModelAttach     sModelAttach;//ģ�͸������ԣ��������ԣ�
	SPointAttach     sPointAttach;//�㸽�����ԣ��������ԣ�
	SLineAttach      sLineAttach; //�߸������ԣ��������ԣ�
}

//�½�ʵ�����������Ϣ
struct SNewUnitData
{
	SPreUnitBsaeInfo        sPreUnitBsaeInfo;  //������Ϣ������������Ϣ���ֱ�Ӵ���ʵ��
	SPreUnitAttachInfo      sPreUnitAttachInfo;//ʵ�����Ը�����Ϣ�������޸�ʵ������
};

// ״̬ʵʱλ����̬
struct SDriveUnitData
{
	SDriveUnitData()
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
	char	     strID[64];                //ʵ��ΨһId
	char	     nTargetId[64];            //������ʵ��ΨһId
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

	double dTime;          //�¼�ʱ��
	char   chDescript[200];//�¼���Ϣ
};

#include <gdpAlgor/GxAstronomyCommon.h>

//���ǹ��ģʽ
enum ESpacecraftOrbitMode
{
	SatStaticOrbit = 0,   //��̬���
	SatDynamicOrbit = 1,  //��̬���
};

//���ǹ�����ݣ����ģʽ���������
struct SSpacecraftObjectEleData
{
	ESpacecraftOrbitMode     eOrbitMode;
	HGxAlgor::SSgp4Data      stSgp4Data;
};
//����ʵ�������������(������������)
struct SNewSpacecraftObjectEleData
{
	SSpacecraftObjectEleData sSpacecraftObjectEleData;  //���ǹ��������Ϣ
	SNewUnitData             sNewUnitData;              //ʵ���������������Ϣ���ڷ�����ģʽ����������ģ��λ����Ϣ
};

typedef std::vector<std::string> VectorDestroyUnitData; //����ʵ���ID��


#endif // FX_UNIT_STRUCT_DEF_H
