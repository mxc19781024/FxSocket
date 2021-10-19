/**************************************************************************************************
* @file FxGanttChart.h
* @note 甘特图
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

//甘特图事件信息
struct SFxGanttChartInfoEventConstants
{
	static std::string Topic() { return "freex/GanttChartInfo/published"; }

	static std::string GanttChartInfoEvent() { return "GanttChartInfoEvent"; }
};
typedef std::vector<std::pair<double,double>> VecPari;
struct SGanttChartInfo
{
	QString                   m_strID;//对象ID
	QString                   m_strName;//对象名称
	VecPari                   m_vecTimePari;//对象事件信息
	QString                   m_strType;//事件类型
	QColor                    m_colorEvent;//绘制颜色
	SGanttChartInfo()
	{

	}
};

struct SEventInfo
{
	QString    m_strID;//事件ID
	QString    m_strType;//事件类型
	double     m_dBeginTime;//开始时间，儒略日
	double     m_dEndTime;//结束时间，儒略日
	QColor     m_colorEvent;//绘制颜色
	QString    m_strEvent;//事件描述
	bool       m_bDraw;//是否绘制，内部运算
	QRect      m_rectDraw;//绘制矩形，内部运算
	QPoint     m_point;//图标绘制位置，内部运算
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
	QString                   m_strID;//对象ID
	QString                   m_strName;//对象名称
	QMap<QString, SEventInfo> m_mapIDToEventInfo;//对象事件信息
	bool                      m_bDraw;//是否绘制，内部运算
	SObjectInfo()
		:m_bDraw(true)
	{

	}
};
class CFxGanttWidget;
class CFxGraduationWidget;
/**
	* @class CTimeAixs
	* @note 甘特图绘制
	* @author g00037
*/
class CFxGanttChart :public QWidget , public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
	/**
    * @brief 构造函数
	* @param pSimuManager [in] 仿真管理系统
	*/
	CFxGanttChart(ctkPluginContext *context,QWidget* pWidget = nullptr);

	/**
	* @brief 析构函数
	*/
	~CFxGanttChart();

	/**
	* @brief 设置开始时间
	* @param time [in] 开始时间
	*/
	void setBeginTime(double time);

	/**
	* @brief 设置总时间
	* @param nDay [in] 天
	* @param nHour [in] 时	
	* @param nMin [in] 分
	* @param nSec [in] 秒
	*/
	void setTotalTime(const int& nDay, const int& nHour, const int& nMin, const int& nSec);

	/**
	* @brief 增加对象信息
	* @param stObjectInfo [in] 对象信息
	*/
	void addObjectInfo(SObjectInfo stObjectInfo);

	/**
	* @brief 移除对象信息
	* @param stObjectInfo [in] 对象ID
	*/
	void removeObjectInfo(const QString& strObjectID);

	/**
	* @brief 增加对象事件
	* @param strObjectID [in] 对象ID
	* @param stEventInfo [in] 对象事件
	*/
	void addObjectEvent(const QString& strObjectID, SEventInfo stEventInfo);

	/**
	* @brief 移除对象事件
	* @param strObjectID [in] 对象ID
	* @param strEventID [in] 事件ID
	*/
	void removeObjectEvent(const QString& strObjectID, const QString& strEventID);
	
	/**
	* @brief 设置对象高度
	* @param m_unHeight [in] 高度
	*/
	void setObjectHeight(const unsigned int& unInterval);

	/**
	* @brief 设置甘特条高度
	* @param m_unHeight [in] 高度
	*/
	void setGanttHeight(const unsigned int& unHeight);

	/**
	* @brief 设置刻度参数
	* @param unShortX [in] 短刻度间隔
	* @param unShortY [in] 短刻度长度
	* @param unLongX [in] 长刻度间隔
	* @param unLongY [in] 长刻度长度
	* @param unBeginX [in] 刻度线起始X
	* @param unBeginY [in] 刻度线起始Y
	* @param colorGraduation [in] 刻度颜色
	*/
	void setGraduationParameter(const unsigned int& unShortX,const unsigned int& unShortY
		,const unsigned int& unLongX,const unsigned int& unLongY
		,const unsigned int& unBeginX,const unsigned int& unBeginY
		,const QColor& colorGraduation);

	/**
	* @brief 设置滚轮缩放比例
	* @param dScale [in] 缩放比例
	*/
	void setWheelScale(double dScale);

	/**
	* @brief 设置背景颜色
	* @param colorBackGround [in] 背景颜色
	*/
	void setBackGroundColor(const QColor& colorBackGround);

	/**
	* @brief 设置指针颜色
	* @param colorPointer [in] 指针颜色
	*/
	void setPointerColor(const QColor& colorPointer);

	/**
	* @brief 设置甘特条使用渐变
	* @param bGradients [in] 使用渐变
	*/
	void setGradients(bool bGradients);

	/**
	* @brief 设置显示网格线
	* @param bGrid [in] 显示网格线
	*/
	void setGrid(bool bGrid);

	/**
	* @brief 重置绘制参数
	*/
	void reset();

private:
	/**
	* @note 绘制事件
	* @param p [in] 绘制事件
	*/
	virtual void paintEvent(QPaintEvent* p);

	/**
	* @note  鼠标滑轮事件
	* @param ev [in] 鼠标滑轮事件
	*/
	virtual void wheelEvent(QWheelEvent* ev);

	/**
	* @note  鼠标按下事件
	* @param ev [in] 鼠标事件
	*/
	virtual void mousePressEvent(QMouseEvent* ev);

	/**
	* @note  鼠标释放事件
	* @param ev [in] 鼠标事件
	*/
	virtual void mouseReleaseEvent(QMouseEvent* ev);

	/**
	* @note  鼠标移动事件
	* @param ev [in] 鼠标事件
	*/
	virtual void mouseMoveEvent(QMouseEvent* ev);

	/**
	* @note  鼠标双击事件
	* @param ev [in] 鼠标事件
	*/
	virtual void mouseDoubleClickEvent(QMouseEvent* ev);

	/**
	* @note  窗口大小改变事件
	* @param ev [in] 窗口大小改变事件
	*/
	virtual void resizeEvent(QResizeEvent* ev);

private:
	/**
	* @note 更新甘特条总高度
	*/
	void updateParameter();

	void updateScrollBarByGanttDrag();

	/**
	* @note  儒略日转QDateTime
	* @param dJD [in] 儒略日
	*/
	QDateTime convertJDToDateTime(double dJD);

	/**
	* @note  秒转时分秒
	* @param nTime [in] 秒
	* @param nHour [out] 时	
	* @param nMin [out] 分
	* @param nSec [out] 秒
	*/
	void convertSecToHMS(int nTime, int& nHour, int& nMin, int& nSec);

	/**
	* @note  时分秒转字符串
	* @param nHour [in] 时	
	* @param nMin [in] 分
	* @param nSec [in] 秒
	*/
	QString HMSToString(const int& nHour, const int& nMin, const int& nSec);

	/**
	* @note  秒时间计算位置X
	* @param nTime [in] 秒
	*/
	int timeToX(double dTime);

	/**
	* @note  位置X计算秒时间
	* @param nX [in] 位置X
	*/
	double xToTime(int nX);

	/**
	* @note  儒略日转与当前开始时间的差值秒时间
	* @param dJD [in] 儒略日
	*/
	double JDToSecAtCurBeginTime(double dJD);

	/**
	* @note  与当前开始时间的差值秒时间转儒略日
	* @param dTime [in] 与当前开始时间的差值秒时间
	*/
	double SecAtCurBeginTimeToJD(double dTime);

	void updateGantt();
protected:
	/**
	* @brief	事件处理
	* @param	event [in] 事件
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
	int                        m_nShortX;           //短刻度间隔
	int                        m_nShortY;           //短刻度长度
	int                        m_nLongX;            //长刻度间隔
	int                        m_nLongY;            //长刻度长度
	int	                       m_nBeginX;           //刻度线起始X
	int	                       m_nBeginY;           //刻度线起始Y
	int	                       m_nTotalGraduation;  //总刻度
	int	                       m_nTimeGraduation;	//总时间对应刻度
	QColor                     m_colorGraduation;	//刻度颜色
	QPolygon				   m_polygonPointer;    //指针范围
	double					   m_dInitialBeginTime; //初始起始时间，儒略日
	double					   m_dCurBeginTime;	    //当前起始时间,秒
	double					   m_dInitialTotalTime; //初始总时间,秒
	double					   m_dCurTotalTime;	    //当前总时间,秒
	double                     m_dScale;            //鼠标滚轮缩放比例
	QColor                     m_colorBackGround;   //背景颜色
	QColor					   m_colorPointer;      //指针颜色
	int                        m_nObjectHeight;     //对象高度
	int						   m_nGanttY;		    //甘特条绘制起始Y
	int                        m_nGanttHeight;      //甘特条高度
	int                        m_nGanttTotalHeight; //甘特条总高度
	bool                       m_bScrollBar;        //是否显示滚动条
	QRect                      m_rectScrollBar;     //滚动条区域
	int                        m_nTotalScrollBar;   //滚动条总值
	int                        m_nCurScrollBar;     //滚动条当前值
	QPoint                     m_pointCurMouse;     //鼠标当前位置
	bool                       m_bDragHorizontal;   //水平拖动
	bool                       m_bDragVertical;     //垂直拖动
	bool                       m_bDragScrollBar;    //拖动滚动条
	bool                       m_bGradients;        //甘特条是否使用渐变
	bool                       m_bGrid;             //是否绘制网格
	bool                       m_bDragPointer;      //拖动指针
	QMap<QString, SObjectInfo> m_mapIDToObjectInfo; //时间对应对象
    QRect*						m_pRect;            //鼠标选中检测区域
	std::map<QString,QPixmap*>	m_mapImage;			//图片image
	QPoint						m_imageSize;		//iamge大小
	friend CFxGanttWidget;
	CFxGanttWidget*        m_pGantt;
	friend CFxGraduationWidget;
	CFxGraduationWidget*   m_pGraduation;
};
#endif //FX_GANTT_CHART_H