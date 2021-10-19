// last modified 

#ifndef HANZ2PINY_H
#define HANZ2PINY_H

#include <string>
#include <vector>


class Hanz2Piny 
{
    public:
        typedef unsigned short Unicode;

    public:
        Hanz2Piny ();
        
        bool isHanziUnicode (const Unicode unicode) const;
        std::vector<std::string> toPinyinFromUnicode (const Unicode hanzi_unicode, const bool with_tone = true) const;
        
        bool isUtf8 (const std::string& s) const;

		std::vector<std::vector<std::string>> toPinyinFromUtf8(const std::string &) const;

    private:
        static const Unicode begin_hanzi_unicode_, end_hanzi_unicode_;
        static const char* pinyin_list_with_tone_[];
};

#endif

