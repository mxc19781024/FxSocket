#include "FxTreeSearchTool/FuzzyCompareHelper.h"
#include "FxTreeSearchTool/FuzzyCompare.h"



CFuzzyCompare::CFuzzyCompare()
	: m_pCompareHelper(new CFuzzyCompareHelper(this))
{

}

/**
* @brief
* @note
* @param
* @param
* @return
*/
CFuzzyCompare::~CFuzzyCompare()
{

}

/**
* @brief
* @note
* @param
* @param
* @return
*/
bool CFuzzyCompare::fuzzyCompare(const QString &strInput, const QString &strOrigin)
{
	return m_pCompareHelper->fuzzyCompare(strInput, strOrigin);
}
