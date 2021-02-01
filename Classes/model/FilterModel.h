//
//  FilterModel.h
//  屏蔽字
//
//  Created by mzp on 14-4-24.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_Filter_Model__
#define __QSMY_Filter_Model__


#include "Model.h"


class FilterModel
	: public Model
{
public:
	FilterModel();
	~FilterModel();

	virtual bool init();

	// 替换字符串中的屏蔽字
	std::string filter(const char* str);

private:
	void parseFilterData(const CSVDecoder::CSV& val);

private:
	cocos2d::CCArray* m_pFilterVec;
};



#endif	// __QSMY_Filter_Model__