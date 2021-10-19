/**************************************************************************************************
* @file CFuzzyCompare
* @note 汉字的模糊匹配 
* @author w00026） 
* @date 创建时间（如2018-12-19） 
**************************************************************************************************/
#include "freexcommontoolui_export.h"
//#include <FreeXSysConfig.h>
class CFuzzyCompareHelper;

class FREEXCOMMONTOOLUI_EXPORT  CFuzzyCompare
{
public:
	CFuzzyCompare();

	~CFuzzyCompare();


	/**
	* @brief 模糊匹配
	* @note
	* @param strInput [in]  搜索时输入的文本内容
	* @param strOrigin [in] 文本的原始内容
	* @return 返回值为bool类型，true为匹配成功，false为匹配失败
	*/
	bool fuzzyCompare(const QString &strInput, const QString &strOrigin);

private:
	friend class CFuzzyCompareHelper;

private:
	// 模糊搜索帮助类
	CFuzzyCompareHelper			*m_pCompareHelper;

};
