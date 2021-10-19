#ifndef PYRAMIDCACHE_H
#define PYRAMIDCACHE_H

#include <QWidget>
#include <QObject>

/**************************************************************************************************
* @file PyramidCache 
* @note 显示金字塔缓存
* @author w00025
* @date 2019-1-22 
**************************************************************************************************/


class CEarthPyramidCacheWgt;

class CEarthPyramidCache : public QObject
{
	Q_OBJECT
public:

	static CEarthPyramidCache *getInstance(QWidget *parent = NULL)
	{
		if (!s_pRyCacheInstance)
		{
			s_pRyCacheInstance = new CEarthPyramidCache(parent);
		}
		return s_pRyCacheInstance;
	}

	/**
	* @brief 显示缓存窗口
	* @note
	*/
	void showCacheWgt();

	/**
	* @brief 显示缓存窗口并指定缓存窗口的位置
	* @note
	* @param nXpos[in] 缓存窗口的x坐标位置
	* @param nYpos[in] 缓存窗口的y坐标位置
	*/
	void showCacheWgt(int nXpos, int nYpos);


	/**
	* @brief 关闭缓存窗口
	* @note
	*/
	void closeCacheWgt();

	/**
	* @brief 设置缓存进度
	* @note
	* @param nValue[in] 缓存进度
	*/
	void setCacheProcess(double dValue);


	/**
	* @brief 设置剩余时间
	* @note
	* @param strRemainTime[in] 剩余时间
	*/
	void setRemainTime(const QString &strRemainTime);

signals:
	void cancelCacheSignal();

private:
	CEarthPyramidCache(QWidget *parent = NULL);
	~CEarthPyramidCache();

private:
	// 金字塔缓存单例
	static CEarthPyramidCache			*s_pRyCacheInstance;

	CEarthPyramidCacheWgt				*m_pRyCacheWgt;

	// 单例嵌套类用来对单例资源进行回收
	class CGarbo
	{
	public:
		~CGarbo()
		{
			if (CEarthPyramidCache::s_pRyCacheInstance)
			{
				delete CEarthPyramidCache::s_pRyCacheInstance;
			}
		}
	};
	static CGarbo s_garBo;

	//开始时间
	double m_time_start;
	//结束时间
	double m_time_end;
	
	//记录每段的步长
	int m_iEnd;
	
	//步长计数器
	int m_updateCount;

	//记录上次进度的值
	double m_nLastValue;
};

#endif // PYRAMIDCACHE_H
