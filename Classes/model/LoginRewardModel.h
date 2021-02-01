//
//  LoginRewardModel.h
//  登陆奖励
//
//  created by ldr123 on 13-7-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__LoginRewardModel__
#define __QSMY__LoginRewardModel__

#include "Model.h"

//#define D_EVENT_ACTIVITY_UPDATE	"update activity"

#define MAX_LOGIN_REWARD_DAYS 3

namespace loginreward
{
	class WelcomeData : public ValueObjcet
	{
	public:
		WelcomeData()
			:m_id(0), 
			m_type(0), 
			m_dateTs(0),
			m_hourBegin(0),
			m_hourEnd(0),
			m_minBegin(0),
			m_minEnd(0),
			m_imgIndex(0),
			m_soundPath(""),
			m_content("")
		{}
	public:
		CREATE_FUNC(WelcomeData);
		D_INIT_DEFAULT(ValueObjcet);
	public:
		uint16_t	m_id;
		uint16_t	m_type;

		uint32_t	m_dateTs;
		uint16_t	m_hourBegin;
		uint16_t	m_hourEnd;
		uint16_t	m_minBegin;
		uint16_t	m_minEnd;

		uint16_t	m_imgIndex;
		std::string	m_soundPath;
		std::string	m_content;
	};


	class LoginRewardData : public ValueObjcet
	{
	public:
		LoginRewardData():price_type(0), price_number(0), icon_id(0){}
	public:
		CREATE_FUNC(LoginRewardData);
		D_INIT_DEFAULT(ValueObjcet);
	public:
		uint16_t	price_type;
		uint32_t	price_number;
		uint16_t	icon_id;
		uint16_t	days;
	};

	#define FAKE_NUM 5
	class LoginRewardResult
	{
	public:
		LoginRewardResult() : uTrueID(0),uTrueValue(0)
		{
			for (int i=0; i<FAKE_NUM; ++i)
			{
				uFakeID[i]		= 0;
				uFakeValue[i]	= 0;
			}
		}
	public:
		void clear()
		{
			uTrueID = uTrueValue = 0;

			for (int i=0; i<FAKE_NUM; ++i)
			{
				uFakeID[i]		= 0;
				uFakeValue[i]	= 0;
			}
		}
	public:
		uint16_t uTrueID;
		uint32_t uTrueValue;

		uint16_t uFakeID[FAKE_NUM];
		uint32_t uFakeValue[FAKE_NUM];
	};

	typedef bool LoginRewardDays[MAX_LOGIN_REWARD_DAYS];
}

class LoginRewardModel : public Model
{

	CC_SYNTHESIZE_READONLY(unsigned short,	loginTimes,	LoginTimes);	//连续登陆次数
	CC_SYNTHESIZE_READONLY(unsigned int,	tLoginAward,TLoginAward);	//累计登录奖励领取情况
	CC_SYNTHESIZE_READONLY(unsigned short,	loginAward,	LoginAward);	//连续登陆奖励领取情况
	CC_SYNTHESIZE_READONLY(unsigned int,	lvAward,	LvAward);		//升级奖励领取情况
	CC_SYNTHESIZE_READONLY(unsigned short,	tLoginTimes,TLoginTimes);	//登陆次数
	
	CC_SYNTHESIZE_READONLY(bool,m_bWelcomeOpen,WelcomeOpen);	//是否启用欢迎界面

public:
	typedef cocos2d::CCArray LoginRewardArr;
	LoginRewardModel();
	virtual ~LoginRewardModel();
public:
	CREATE_FUNC(LoginRewardModel);
	bool init();
public:
	void ParseActivityInfo(const Json::Value& val);
	void ParseLoginRewardData(const CSVDecoder::CSV &val);
	void ParseLoginRewardFromJson(const Json::Value& val);
	//load 欢迎界面的配表
	void LoadWelcomeData(const CSVDecoder::CSV &val);
	//获取当天的内容
	loginreward::WelcomeData*	getWelcomeData();
public:

	bool		IsHaveLoginAward();
	void		GetLoginAwardDays(loginreward::LoginRewardDays &days);
	void		getLoginAwardResult(uint16_t days, uint16_t uDropType, uint32_t uDropValue, uint16_t userLv, loginreward::LoginRewardResult &l);
protected:
	LoginRewardArr*	m_pLoginReward;
	cocos2d::CCArray*	m_pWelcomeList;
};

#endif /* defined(__QSMY__LoginRewardModel__) */
