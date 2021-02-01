//
//  ActivityModel.h
//  活动(原为登陆奖励)
//
//  created by Eci on 14-3-24.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__ActivityModel__
#define __QSMY__ActivityModel__

#include "Model.h"

#define D_EVENT_ACTIVITY_REFRESH_CPP "activity_refresh_cpp"
#define D_EVENT_ACTIVITY_SOUL_EXCHANGE_REFRESH	"activity_soul_exchange_refresh"
#define D_EVENT_ACTIVITY_ONLINE_AWARD "activity_online_award"	//在线奖励信息更新

class ActivityModel;

namespace activity
{
	enum ActivityType
	{
		ActivityTypeSoulExchange = 16,
	};

	class OnlineAwardInfo : public ValueObjcet
	{
	public:
		OnlineAwardInfo();
		~OnlineAwardInfo();
		CREATE_FUNC(OnlineAwardInfo);
		bool init();

	protected:
		friend class ::ActivityModel;

		void loadData(const CSVDecoder::CSV &val);

		CC_SYNTHESIZE_READONLY(unsigned int , m_uTime, Time);
		CC_SYNTHESIZE_READONLY(unsigned int , m_uID, ID);
		CC_SYNTHESIZE_READONLY(cocos2d::CCArray* , m_pDropList, DropList);
	};

	class SoulPointInfo : public ValueObjcet
	{
	public:
		SoulPointInfo();
		CREATE_FUNC(SoulPointInfo);
		bool init();
		bool isOpen();
		uint32_t getSoulPointFromCardAtIndex(int idx);
	protected:
		friend class ::ActivityModel;
		uint32_t m_nEndTs;
		void loadSoulPointFromCard(const CSVDecoder::CSV &val);
		CC_SYNTHESIZE_READONLY(uint32_t, m_uSoulPoint, SoulPoint);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<uint32_t>, m_vWarriorID, WarriorID);
		CC_SYNTHESIZE(int, m_uSoulPointAdded, SoulPointAdded);
	private:
		std::vector<uint32_t> m_vSoulPointFromCard;
	};
	
}

class ActivityModel : public Model
{
public:
	ActivityModel();
	virtual ~ActivityModel();

	void parseInfo(const Json::Value& val);
	void parseSoulExchange(const Json::Value& val);

	void syncFromServer();
	void loadOnlineAwardData(const CSVDecoder::CSV &val);
	void parseOnlineAward(const Json::Value &requestData, const Json::Value &responseData);
	//是否显示“在线奖励”
	bool showAward();
	//倒计时数字(0表示无倒计时)
	unsigned int getOnlineAwardCountdown();

	virtual void reset();
	//在线奖励数据是否同步
	bool isOnlineAwardSynced;
    void setSpecialActivityType(unsigned int type);
public:
	CREATE_FUNC(ActivityModel);
	bool init();
protected:
	CC_SYNTHESIZE_READONLY(activity::SoulPointInfo *, m_pSoulPointInfo, SoulPointInfo);

	//在线奖励数据
	CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pOnlineAwardList, OnlineAwardList);
	CC_SYNTHESIZE(unsigned int, m_uOnlineAwardTimes, OnlineAwardTimes);
	CC_SYNTHESIZE(unsigned int, m_uOnlineTime, m_uOnlineTime);
    
    //特殊活动编号（通过主界面特殊活动入口进入）
    CC_SYNTHESIZE_READONLY(unsigned int, m_iSpecialActivityType, SpecialActivityType);
};

#endif /* defined(__QSMY__ActivityModel__) */
