/**************************************************************************************************
* @file CFuzzyCompareHelper
* @note 汉语词组模糊搜索help类，支持对汉语词组进行首字母，全拼的模糊匹配
* @author w00026 
* @date 创建时间（如2018-12-19） 
**************************************************************************************************/

#ifndef FuzzySearchHelper_H
#define FuzzySearchHelper_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <QTreeWidget>
//#include <FreeXSysConfig.h>

using std::vector;
using std::string;
using std::ifstream;
using std::endl;
using std::cerr;
using std::istream_iterator;
using std::pair;

class CFuzzyCompare;

class CFuzzyCompareHelper
{

public:
	CFuzzyCompareHelper(CFuzzyCompare *pFuzzyCompare);
	~CFuzzyCompareHelper();

	/**
	* @brief 模糊匹配
	* @note
	* @param strInput [in]  搜索时输入的文本内容
	* @param strOrigin [in] 文本的原始内容
	* @return 返回值为bool类型，true为匹配成功，false为匹配失败
	*/
	bool fuzzyCompare(const QString &strInput, const QString &strOrigin);

public:
	typedef unsigned short Unicode;
	typedef unsigned int UINT;
	typedef unsigned char UCHAR;

private:
	/**
	* @brief 将汉字转为拼音（首字母以及全拼）
	* @note
	* @param strOrigin [in] 文本的原始内容
	* @param strInitial [out]文本的首字母字符串
	* @param strQuanPin [out] 文本的全拼字符串
	* @return
	*/
	void getPinYin(const QString &strOrigin, QString &strInitial, QString &strQuanPin);

	/**
	* @brief 获取汉字词组的首字母拼音
	* @note	支持GBK编码，不支持如CJK编码如囧等
	* @param strChs[in]汉字词组传入
	* @param
	* @return 返回汉字词组的首字母拼音
	*/
	std::string getFirstLetter(const char* strChs);

	/**
	* @brief 判断汉字是否是unicode编码
	* @note
	* @param unicode[in] 汉字的unicode码
	* @return 是返回true，不是返回false
	*/
	bool isHanziUnicode(const Unicode unicode) const;

	/**
	* @brief 将unicode编码的汉字转换为拼音
	* @note	
	* @param hanzi_unicode[in]传入unicode编码的汉字
	* @param with_tone[in] 汉字拼音是否需要带声调
	* @return 返回词组的拼音，若为多音字则多个拼音
	*/
	std::vector<std::string> toPinyinFromUnicode(const Unicode hanzi_unicode, const bool with_tone = true) const;

	/**
	* @brief 判断是否是utf8编码
	* @note
	* @param s[in] 字符串
	* @return 是返回true，不是返回false
	*/
	bool isUtf8(const std::string& s) const;
	
	/**
	* @brief 将输入的内容转换为utf-8编码的拼音
	* @note	replace_unknown_with要起到效果的话，必须先指定replace_unknowns为true
	* @params[in]需要转换的汉字文本,
	* @param with_tone[in]汉字拼音是否需要带声调
	* @param replace_unknowns[in]是否需要用指定的字符代替无法转换出的字符
	* @param replace_unknown_with[in] 代替无法转换出字符的字符
	* @return
	*/
	std::vector<std::pair<bool, std::vector<std::string>>> toPinyinFromUtf8(const std::string& s,
		const bool with_tone = true,
		const bool replace_unknown = false,
		const std::string& replace_unknown_with = "") const;

private:
	// 模糊匹配类指针
	CFuzzyCompare*					m_pFuzzyCompare;

	// 汉字unicode编码表的起始处
	static const Unicode			begin_hanzi_unicode_;

	// 汉字unicode编码表的结尾处
	static const Unicode			end_hanzi_unicode_;

	// 带声调的拼音列表
	static const char*				pinyin_list_with_tone_[];
};


#endif
