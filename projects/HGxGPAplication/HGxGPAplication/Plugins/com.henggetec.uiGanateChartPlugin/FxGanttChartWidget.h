/**************************************************************************************************
* @file FxGanttChart.h
* @note ����ͼ
* @author g00068
* @data 2020-10-22
**************************************************************************************************/
#ifndef FX_GANTT_CHART_H
#define FX_GANTT_CHART_H 
#include <QFrame>
#include <QEvent>
#include <QPolygon>
#include <QDateTime>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QMap>
#include "service\event\ctkEventHandler.h"
#include "ctkPluginContext.h"

//����ͼ�¼���Ϣ
struct SFxGanttChartInfoEventConstants
{
	static std::string Topic() { return "freex/GanttChartInfo/published"; }

	static std::string GanttChartInfoEvent() { return "GanttChartInfoEvent"; }
};
typedef std::vector<std::pair<double,double>> VecPari;
struct SGanttChartInfo
{
	QString                   m_strID;//����ID
	QString                   m_strName;//��������
	VecPari                   m_vecTimePari;//�����¼���Ϣ
	QString                   m_strType;//�¼�����
	QColor                    m_colorEvent;//������ɫ
	SGanttChartInfo()
	{

	}
};

struct SEventInfo
{
	QString    m_strID;//�¼�ID
	QString    m_strType;//�¼�����
	double     m_dBeginTime;//��ʼʱ�䣬������
	double     m_dEndTime;//����ʱ�䣬������
	QColor     m_colorEvent;//������ɫ
	QString    m_strEvent;//�¼�����
	bool       m_bDraw;//�Ƿ���ƣ��ڲ�����
	QRect      m_rectDraw;//���ƾ��Σ��ڲ�����
	QPoint     m_point;//ͼ�����λ�ã��ڲ�����
	SEventInfo(QString strID = QString(), QString strType = QString(), double dBeginTime = 0, double dEndTime = 0, QColor colorEvent = QColor(), QString strEvent = QString())
		:m_strID(strID)
		, m_strType(strType)
		, m_dBeginTime(dBeginTime)
		, m_dEndTime(dEndTime)
		, m_colorEvent(colorEvent)
		, m_strEvent(strEvent)
		, m_bDraw(true)
	{

	}
};
struct SObjectInfo
{
	QString                   m_strID;//����ID
	QString                   m_strName;//��������
	QMap<QString, SEventInfo> m_mapIDToEventInfo;//�����¼���Ϣ
	bool                      m_bDraw;//�Ƿ���ƣ��ڲ�����
	SObjectInfo()
		:m_bDraw(true)
	{

	}
};
class CFxGanttWidget;
class CFxGraduationWidget;
/**
	* @class CTimeAixs
	* @note ����ͼ����
	* @author g00037
*/
class CFxGanttChart :public QWidget , public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
	/**
    * @brief ���캯��
	* @param pSimuManager [in] �������ϵͳ
	*/
	CFxGanttChart(ctkPluginContext *context,QWidget* pWidget = nullptr);

	/**
	* @brief ��������
	*/
	~CFxGanttChart();

	/**
	* @brief ���ÿ�ʼʱ��
	* @param time [in] ��ʼʱ��
	*/
	void setBeginTime(double time);

	/**
	* @brief ������ʱ��
	* @param nDay [in] ��
	* @param nHour [in] ʱ	
	* @param nMin [in] ��
	* @param nSec [in] ��
	*/
	void setTotalTime(const int& nDay, const int& nHour, const int& nMin, const int& nSec);

	/**
	* @brief ���Ӷ�����Ϣ
	* @param stObjectInfo [in] ������Ϣ
	*/
	void addObjectInfo(SObjectInfo stObjectInfo);

	/**
	* @brief �Ƴ�������Ϣ
	* @param stObjectInfo [in] ����ID
	*/
	void removeObjectInfo(const QString& strObjectID);

	/**
	* @brief ���Ӷ����¼�
	* @param strObjectID [in] ����ID
	* @param stEventInfo [in] �����¼�
	*/
	void addObjectEvent(const QString& strObjectID, SEventInfo stEventInfo);

	/**
	* @brief �Ƴ������¼�
	* @param strObjectID [in] ����ID
	* @param strEventID [in] �¼�ID
	*/
	void removeObjectEvent(const QString& strObjectID, const QString& strEventID);
	
	/**
	* @brief ���ö���߶�
	* @param m_unHeight [in] �߶�
	*/
	void setObjectHeight(const unsigned int& unInterval);

	/**
	* @brief ���ø������߶�
	* @param m_unHeight [in] �߶�
	*/
	void setGanttHeight(const unsigned int& unHeight);

	/**
	* @brief ���ÿ̶Ȳ���
	* @param unShortX [in] �̶̿ȼ��
	* @param unShortY [in] �̶̿ȳ���
	* @param unLongX [in] ���̶ȼ��
	* @param unLongY [in] ���̶ȳ���
	* @param unBeginX [in] �̶�����ʼX
	* @param unBeginY [in] �̶�����ʼY
	* @param colorGraduation [in] �̶���ɫ
	*/
	void setGraduationParameter(const unsigned int& unShortX,const unsigned int& unShortY
		,const unsigned int& unLongX,const unsigned int& unLongY
		,const unsigned int& unBeginX,const unsigned int& unBeginY
		,const QColor& colorGraduation);

	/**
	* @brief ���ù������ű���
	* @param dScale [in] ���ű���
	*/
	void setWheelScale(double dScale);

	/**
	* @brief ���ñ�����ɫ
	* @param colorBackGround [in] ������ɫ
	*/
	void setBackGroundColor(const QColor& colorBackGround);

	/**
	* @brief ����ָ����ɫ
	* @param colorPointer [in] ָ����ɫ
	*/
	void setPointerColor(const QColor& colorPointer);

	/**
	* @brief ���ø�����ʹ�ý���
	* @param bGradients [in] ʹ�ý���
	*/
	void setGradients(bool bGradients);

	/**
	* @brief ������ʾ������
	* @param bGrid [in] ��ʾ������
	*/
	void setGrid(bool bGrid);

	/**
	* @brief ���û��Ʋ���
	*/
	void reset();

private:
	/**
	* @note �����¼�
	* @param p [in] �����¼�
	*/
	virtual void paintEvent(QPaintEvent* p);

	/**
	* @note  ��껬���¼�
	* @param ev [in] ��껬���¼�
	*/
	virtual void wheelEvent(QWheelEvent* ev);

	/**
	* @note  ��갴���¼�
	* @param ev [in] ����¼�
	*/
	virtual void mousePressEvent(QMouseEvent* ev);

	/**
	* @note  ����ͷ��¼�
	* @param ev [in] ����¼�
	*/
	virtual void mouseReleaseEvent(QMouseEvent* ev);

	/**
	* @note  ����ƶ��¼�
	* @param ev [in] ����¼�
	*/
	virtual void mouseMoveEvent(QMouseEvent* ev);

	/**
	* @note  ���˫���¼�
	* @param ev [in] ����¼�
	*/
	virtual void mouseDoubleClickEvent(QMouseEvent* ev);

	/**
	* @note  ���ڴ�С�ı��¼�
	* @param ev [in] ���ڴ�С�ı��¼�
	*/
	virtual void resizeEvent(QResizeEvent* ev);

private:
	/**
	* @note ���¸������ܸ߶�
	*/
	void updateParameter();

	void updateScrollBarByGanttDrag();

	/**
	* @note  ������תQDateTime
	* @param dJD [in] ������
	*/
	QDateTime convertJDToDateTime(double dJD);

	/**
	* @note  ��תʱ����
	* @param nTime [in] ��
	* @param nHour [out] ʱ	
	* @param nMin [out] ��
	* @param nSec [out] ��
	*/
	void convertSecToHMS(int nTime, int& nHour, int& nMin, int& nSec);

	/**
	* @note  ʱ����ת�ַ���
	* @param nHour [in] ʱ	
	* @param nMin [in] ��
	* @param nSec [in] ��
	*/
	QString HMSToString(const int& nHour, const int& nMin, const int& nSec);

	/**
	* @note  ��ʱ�����λ��X
	* @param nTime [in] ��
	*/
	int timeToX(double dTime);

	/**
	* @note  λ��X������ʱ��
	* @param nX [in] λ��X
	*/
	double xToTime(int nX);

	/**
	* @note  ������ת�뵱ǰ��ʼʱ��Ĳ�ֵ��ʱ��
	* @param dJD [in] ������
	*/
	double JDToSecAtCurBeginTime(double dJD);

	/**
	* @note  �뵱ǰ��ʼʱ��Ĳ�ֵ��ʱ��ת������
	* @param dTime [in] �뵱ǰ��ʼʱ��Ĳ�ֵ��ʱ��
	*/
	double SecAtCurBeginTimeToJD(double dTime);

	void updateGantt();
protected:
	/**
	* @brief	�¼�����
	* @param	event [in] �¼�
	*/
	virtual void handleEvent(const ctkEvent& event);
signals:

	void addObjectEventSignal(SGanttChartInfo);
	void ganttChartShowSignal();
	void ganttChartClearSignal();
private Q_SLOTS:
	void addObjectEventSlot(SGanttChartInfo) ;
	void ganttChartShowSlot();
	void ganttChartClearSlot();
private:
	int                        m_nShortX;           //�̶̿ȼ��
	int                        m_nShortY;           //�̶̿ȳ���
	int                        m_nLongX;            //���̶ȼ��
	int                        m_nLongY;            //���̶ȳ���
	int	                       m_nBeginX;           //�̶�����ʼX
	int	                       m_nBeginY;           //�̶�����ʼY
	int	                       m_nTotalGraduation;  //�̶ܿ�
	int	                       m_nTimeGraduation;	//��ʱ���Ӧ�̶�
	QColor                     m_colorGraduation;	//�̶���ɫ
	QPolygon				   m_polygonPointer;    //ָ�뷶Χ
	double					   m_dInitialBeginTime; //��ʼ��ʼʱ�䣬������
	double					   m_dCurBeginTime;	    //��ǰ��ʼʱ��,��
	double					   m_dInitialTotalTime; //��ʼ��ʱ��,��
	double					   m_dCurTotalTime;	    //��ǰ��ʱ��,��
	double                     m_dScale;            //���������ű���
	QColor                     m_colorBackGround;   //������ɫ
	QColor					   m_colorPointer;      //ָ����ɫ
	int                        m_nObjectHeight;     //����߶�
	int						   m_nGanttY;		    //������������ʼY
	int                        m_nGanttHeight;      //�������߶�
	int                        m_nGanttTotalHeight; //�������ܸ߶�
	bool                       m_bScrollBar;        //�Ƿ���ʾ������
	QRect                      m_rectScrollBar;     //����������
	int                        m_nTotalScrollBar;   //��������ֵ
	int                        m_nCurScrollBar;     //��������ǰֵ
	QPoint                     m_pointCurMouse;     //��굱ǰλ��
	bool                       m_bDragHorizontal;   //ˮƽ�϶�
	bool                       m_bDragVertical;     //��ֱ�϶�
	bool                       m_bDragScrollBar;    //�϶�������
	bool                       m_bGradients;        //�������Ƿ�ʹ�ý���
	bool                       m_bGrid;             //�Ƿ��������
	bool                       m_bDragPointer;      //�϶�ָ��
	QMap<QString, SObjectInfo> m_mapIDToObjectInfo; //ʱ���Ӧ����
    QRect*						m_pRect;            //���ѡ�м������
	std::map<QString,QPixmap*>	m_mapImage;			//ͼƬimage
	QPoint						m_imageSize;		//iamge��С
	friend CFxGanttWidget;
	CFxGanttWidget*        m_pGantt;
	friend CFxGraduationWidget;
	CFxGraduationWidget*   m_pGraduation;
};
#endif //FX_GANTT_CHART_H