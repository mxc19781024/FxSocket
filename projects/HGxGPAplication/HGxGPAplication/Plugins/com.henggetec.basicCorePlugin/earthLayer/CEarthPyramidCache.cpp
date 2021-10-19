﻿#include "CEarthPyramidCache.h"
#include "CEarthPyramidCacheWgt.h"
#include <time.h>



CEarthPyramidCache* CEarthPyramidCache::s_pRyCacheInstance = NULL;

CEarthPyramidCache::CEarthPyramidCache(QWidget *parent)
	: m_pRyCacheWgt(NULL)
{
	m_pRyCacheWgt = new CEarthPyramidCacheWgt(parent);
	m_iEnd = 10;
	m_updateCount=1;
	connect(m_pRyCacheWgt, SIGNAL(cancelCacheSignal()), this, SIGNAL(cancelCacheSignal()));
}

CEarthPyramidCache::~CEarthPyramidCache()
{

}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCache::showCacheWgt()
{
	m_pRyCacheWgt->show();
}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCache::showCacheWgt(int nXpos, int nYpos)
{
	m_pRyCacheWgt->move(nXpos, nYpos);
	m_pRyCacheWgt->show();
}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCache::closeCacheWgt()
{
	m_pRyCacheWgt->close();
}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCache::setCacheProcess(double dValue)
{	
	m_pRyCacheWgt->setCacheProcess(dValue*100);

	//设置步长间隔
	const int iStepPercent =50;

	//第二次加载进行值初始化
	if(dValue<m_nLastValue)
	{
		m_nLastValue = dValue;
		m_updateCount = 1;
	}

	//计算步长间隔的时间
	if(m_updateCount == 1)
	{
		m_time_start = (double)clock();
		m_iEnd = m_updateCount+iStepPercent;
		m_nLastValue = dValue;
	}
	if(m_updateCount == m_iEnd)
	{
		m_time_end=(double)clock();
		m_pRyCacheWgt->setCostTime((m_time_end-m_time_start), (dValue-m_nLastValue)*100);

		m_time_start = m_time_end;
		m_nLastValue = dValue;
		m_iEnd += iStepPercent;
	}
	m_updateCount++;

}

/**
* @brief
* @note
* @param
* @param
* @return
*/
void CEarthPyramidCache::setRemainTime(const QString &strRemainTime)
{
	m_pRyCacheWgt->setRemainTime(strRemainTime);
}
