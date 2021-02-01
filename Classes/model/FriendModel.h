//
//  FriendModel.h
//  QSMY
//
//  Created by Eci on 14-4-9.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__FriendModel__
#define __QSMY__FriendModel__

//推荐，申请
#define D_EVENT_FRIEND_RECOMMEND	"friend_recommend"
//推荐后申请按钮改变
#define D_EVENT_FRIEND_BASIC_REFRESH	"friend_basic_refresh"
//赠送成功后更新赠送时间
#define D_EVENT_FRIEND_INFO_REFRESH	"friend_info_refresh"
//好友列表
#define D_EVENT_FRIEND_LIST	"friend_list"
//GIFT列表更新
#define D_EVENT_FRIEND_GIFT_LIST	"friend_gift_list"

/************************************************************************/
/* 语言包相关                                                                     */
/************************************************************************/
#define FRIEND_TXT_TYPEEMPTY "friendtips1"
#define FRIEND_TXT_PLACEHOLDER "friendtips2"
#define FRIEND_TXT_APPLYSUCCESS "friendtips3"
#define FRIEND_TXT_GIFTSEND "friendtips4"
#define FRIEND_TXT_GIFTGET "friendtips5"
#define FRIEND_TXT_GIFTSENDGET "friendtips6"
#define FRIEND_TXT_ADDSUCCESS "friendtips7"
#define FRIEND_TXT_DELETESUCCESS "friendtips8"
//在线状态
#define FRIEND_STATE_ONLINE "friendtips9"
#define FRIEND_STATE_TODAY "friendtips10"
#define FRIEND_STATE_ONEDAY "friendtips11"
#define FRIEND_STATE_TWODAY "friendtips12"
#define FRIEND_STATE_THREEDAY "friendtips13"
#define FRIEND_STATE_WEEK "friendtips14"
#define FRIEND_STATE_TWOWEEK "friendtips15"
#define FRIEND_STATE_LONG "friendtips16"

#include "Model.h"
#include "WarriorModel.h"

class FriendModel;

namespace partner
{
	enum GiftState
	{
		kGiftStatePresent = 0,
		kGiftStateReceive = 1,
		kGiftStateFormation = 2,
		kGiftStateReturn = 3,
	};

	class FriendBasic : public ValueObjcet
	{
		CC_SYNTHESIZE_READONLY(uint32_t, m_nFriendID, FriendID);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sName, Name);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nLevel, Level);
		CC_SYNTHESIZE_READONLY(bool, m_bRequested, HasRequested);
		//推荐（false）新的申请（true）
		CC_SYNTHESIZE_READONLY(bool, m_bNewRequest, NewRequest);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nOffTime, OffTime);
		//战斗力
		CC_SYNTHESIZE_READONLY(uint32_t, m_nFight, Fight);
		friend class ::FriendModel;
	public:
		FriendBasic();
		virtual ~FriendBasic();
		void parse(const Json::Value &val);
		CREATE_FUNC(FriendBasic);
		D_INIT_DEFAULT(ValueObjcet);
	protected:
		uint32_t m_nAvatarID;
	};

	class FriendInfo : public FriendBasic
	{
		//我向好友赠送体力的发货时间
		CC_SYNTHESIZE_READONLY(uint32_t, m_nPresentTs, PresentTs);
		//好友赠送我体力的发货时间
		CC_SYNTHESIZE_READONLY(uint32_t, m_nReceiveTs, ReceiveTs);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nAssociateTs, AssociateTs);
		//0表示未领，1表示领过了
		CC_SYNTHESIZE_READONLY(uint32_t, m_nGiftState, GiftState);

		CC_SYNTHESIZE(bool, m_bFate, Fate);
		CC_SYNTHESIZE(bool, m_bDup, Dup);
		CC_SYNTHESIZE(bool, m_bRanout, Ranout);
		CC_SYNTHESIZE(bool, m_bNew, New);
		friend class ::FriendModel;
	public:
		FriendInfo();
		virtual ~FriendInfo();
		void parse(const Json::Value &val);
		CREATE_FUNC(FriendInfo);
		D_INIT_DEFAULT(FriendBasic);

	};
};

class FriendModel :public Model
{

public:
    FriendModel();
    virtual ~FriendModel();
    virtual bool init();

	void onEnter();

	void syncRecommend();
	void syncFriend();

	///////////////////////////////network///////////////////////////////
	void parseFriend(const Json::Value &val);
	void parseRecommend(const Json::Value &val);
	void parseRequest(const Json::Value &val);
	void requestsCountGot(const Json::Value &responseData);
	//好友申请与响应
	void doApply(unsigned int fid);
	void onApply(const Json::Value &requestData,const Json::Value &responseData);

	void removeRequest(uint32_t nFriendID);
	void addFriend(const Json::Value &val);
	void onReceiveGift(const Json::Value &val);
	void onReceiveAndSendGiftAll();
	void removeFriends(const Json::Value &val);
	void onSendGift(const Json::Value &requestData,const Json::Value &responseData);

	//返回列表数据
	cocos2d::CCArray *getFriendList(cocos2d::CCArray *pFriend);
	cocos2d::CCArray *getRecommend(cocos2d::CCArray *pRecommend);
	cocos2d::CCArray *getGiftList(cocos2d::CCArray *pFriend);
	
	partner::GiftState getGiftState(partner::FriendInfo *pInfo);

	const char	*getStateString(unsigned int offtime);
	//推荐了几次好友
	CC_SYNTHESIZE_READONLY(uint8_t, m_uRecommendTimes, RecommendTimes);
	//忽略所有申请
	void clearRequest();

	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pFriend, FriendList);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pRecommend, RecommendList);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pRequest, RequestList);
private:

	bool m_bRecommendSynced;
	bool m_bFriendSynced;
	uint32_t	m_uFriendSyncedTime;
};

#endif /* defined(__QSMY__FriendModel__) */
