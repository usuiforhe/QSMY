//
//  StringUtil.h
//  QSMY
//
//  Created by wanghejun on 13-4-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef QSMY_StringUtil_h
#define QSMY_StringUtil_h
#include <string>
#include <vector>
#include <sstream>


// string toolkit

static inline void split(std::string src, const char* token, std::vector<uint16_t>& vect)
{
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = src.find(token, nbegin);
        if(nend == -1)
            vect.push_back(atoi(src.substr(nbegin, src.length()-nbegin).c_str()));
        else
            vect.push_back(atoi(src.substr(nbegin, nend-nbegin).c_str()));
        nbegin = nend + strlen(token);
    }
}

// string toolkit
static inline void split(std::string src, const char* token, std::vector<uint32_t>& vect)
{
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = src.find(token, nbegin);
        if(nend == -1)
            vect.push_back(atoi(src.substr(nbegin, src.length()-nbegin).c_str()));
        else
            vect.push_back(atoi(src.substr(nbegin, nend-nbegin).c_str()));
        nbegin = nend + strlen(token);
    }
}

// string toolkit
static inline void split(std::string src, const char* token, std::vector<std::string>& vect)
{
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = src.find(token, nbegin);
        if(nend == -1)
            vect.push_back(src.substr(nbegin, src.length()-nbegin));
        else
            vect.push_back(src.substr(nbegin, nend-nbegin));
        nbegin = nend + strlen(token);
    }
}

template <typename _Tp>
static inline std::string join(std::vector<_Tp>& vect,const char* token)
{
    if(vect.size()==0) return "";
    if(vect.size()==1)
    {
        std::stringstream stream;
        stream<<vect.at(0);
        return stream.str();
    }
    std::string ret;    
    typename std::vector<_Tp>::iterator itr;
    for (itr=vect.begin(); itr<vect.end()-1; ++itr)
    {
        std::stringstream stream;
        stream<<*itr;
        ret.append(stream.str());
        ret.append(token);
    }
    if(itr!=vect.end())
    {
        std::stringstream stream;
        stream<<*itr;
        ret.append(stream.str());
    }
    return ret;
}


static inline void replace(std::string &strBase, std::string strSrc,std::string strDes)
{
    std::string::size_type pos = 0;
    std::string::size_type srcLen = strSrc.size();
    std::string::size_type desLen = strDes.size();
    pos = strBase.find(strSrc, pos);
    while (pos!=std::string::npos) {
        strBase.replace(pos, srcLen, strDes);
        pos = strBase.find(strSrc, pos+desLen);
    }
}

static size_t string_replace_char(std::string& str, char which, char to)
{
	size_t num = 0;
	for ( size_t i = 0; i < str.size(); i++ )
	{
		if ( str[i] == which )
		{
			str[i] = to;
			num++;
		}
	}
	return num;
}


static inline size_t stringCount(std::string & str, const char* token)
{
    std::string::size_type pos = 0;
    std::string::size_type len = strlen(token);
    pos = str.find(token, pos);
    size_t total(0);
    while(pos != std::string::npos)
    {
        ++total;
        pos = str.find(token, pos + len);
    }
    
    return total;
}

#define UTFLEN(x)  utf8_look_for_table[(x)]

static unsigned char utf8_look_for_table[] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

static int getUtf8Length(const char *str)
{
	int clen = strlen(str);
	int len = 0;

	for(char *ptr = const_cast<char*>(str);
		*ptr!=0&&len<clen;
		len++, ptr+=UTFLEN((unsigned char)*ptr));

	return len;
}

#endif
