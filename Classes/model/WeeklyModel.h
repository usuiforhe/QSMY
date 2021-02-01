//
//  WeeklyModel.h
//  周活动
//
//  created by Eci on 14-5-28.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__WeeklyModel__
#define __QSMY__WeeklyModel__

#include "Model.h"
#include "NavigationModel.h"

class WeeklyModel;

namespace weekly
{
	class WeeklyTypeInfo : public ValueObjcet
	{
		CC_SYNTHESIZE(unsigned int, m_uNav, Nav);
	public:
		WeeklyTypeInfo();
		~WeeklyTypeInfo();
		CREATE_FUNC(WeeklyTypeInfo);
		D_INIT_DEFAULT(ValueObjcet);

		bool isActive();
	protected:
		friend class ::WeeklyModel;
		unsigned int m_uType;
		bool m_bResident;

		bool m_bActive;

		unsigned int m_uWeather;
	};
}

class WeeklyModel : public Model
{
	CC_SYNTHESIZE(unsigned int, m_uOpenTs, OpenTs);
public:
	WeeklyModel();
	virtual ~WeeklyModel();

	void loadWeeklyTypeInfo(const CSVDecoder::CSV &val);
	void loadWeeklyWeatherEffect(const CSVDecoder::CSV &val);

	void parseInfo(const Json::Value& val);
	void parseWeekly(const Json::Value& val);
	void parseOneWeekly(const Json::Value& val);

	virtual void reset();

	bool isWeekly(navigation::NavigationType eType);
	bool isWeeklyActive(navigation::NavigationType eType);

	uint32_t getWeatherEffect(unsigned int weeklyType);

	bool isOpen();

	unsigned int getDaysToUnlock();
public:
	CREATE_FUNC(WeeklyModel);
	bool init();
public:
	cocos2d::CCDictionary* getWeeklyList()
	{
		return m_pWeeklyList;
	}
protected:
private:
	cocos2d::CCDictionary *m_pWeeklyList;
	cocos2d::CCDictionary *m_pWeatherEffect;
};

#endif /* defined(__QSMY__WeeklyModel__) */
