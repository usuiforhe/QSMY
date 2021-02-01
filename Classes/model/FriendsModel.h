//
//  FriendsModel.h
//  QSMY
//
//  Created by Eci on 14-4-9.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__FriendsModel__
#define __QSMY__FriendsModel__

#define D_EVENT_FRIENDS_GET "friends_get"
#define D_EVENT_FRIENDS_RECOMMEND	"friends_recommend"
#define D_EVENT_FRIENDS_FOUND	"friends_found"
#define D_EVENT_FRIENDS_DUNGEON	"friends_dungeon"
#define D_EVENT_FRIENDS_REQUESTS "friends_requests"
#define D_EVENT_FRIENDS_BASIC_REFRESH	"friends_basic_refresh"
#define D_EVENT_FRIENDS_STATUS_REFRESH	"friends_status_refresh"
#define D_EVENT_FRIENDS_NEW_REQUEST	"friends_new_request"

#include "Model.h"
#include "WarriorModel.h"

class FriendsModel;

namespace friends
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
		CC_SYNTHESIZE_READONLY(uint32_t, m_nLeaderID, LeaderID);
		CC_SYNTHESIZE_READONLY(bool, m_bRequested, HasRequested);
		friend class ::FriendsModel;
	public:
		FriendBasic();
		virtual ~FriendBasic();
		void parse(const Json::Value &val);
		CREATE_FUNC(FriendBasic);
		D_INIT_DEFAULT(ValueObjcet);
		uint32_t getAvatarID();
	protected:
		uint32_t m_nAvatarID;
	};


	class FriendWarrior :public ValueObjcet
	{
		friend class ::FriendsModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nWID, WID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nUpLevel, UpLevel);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nSkillLevel, SkillLevel);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nFight, Fight);

	public:
		FriendWarrior();
		virtual ~FriendWarrior();
		void parse(const Json::Value &val);
		CREATE_FUNC(FriendWarrior);
		D_INIT_DEFAULT(ValueObjcet);
		uint32_t getAvatarID();

		static FriendWarrior *create(warrior::WarriorCurInfo *pCur);
		bool init(warrior::WarriorCurInfo *pCur);
	protected:
		uint32_t m_nAvatarID;
	};

	class FriendInfo : public FriendBasic
	{
		CC_SYNTHESIZE_READONLY(uint32_t, m_nPresentTs, PresentTs);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nReceiveTs, ReceiveTs);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nAssociateTs, AssociateTs);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nGiftState, GiftState);
		CC_SYNTHESIZE_READONLY(FriendWarrior *, m_pWarriorFront, WarriorFront);
		CC_SYNTHESIZE_READONLY(FriendWarrior *, m_pWarriorBack, WarriorBack);

		CC_SYNTHESIZE(bool, m_bFate, Fate);
		CC_SYNTHESIZE(bool, m_bDup, Dup);
		CC_SYNTHESIZE(bool, m_bRanout, Ranout);
		CC_SYNTHESIZE(bool, m_bNew, New);
		friend class ::FriendsModel;
	public:
		FriendInfo();
		virtual ~FriendInfo();
		void parse(const Json::Value &val);
		CREATE_FUNC(FriendInfo);
		D_INIT_DEFAULT(FriendBasic);

		FriendWarrior *getWarriorAtIndex(uint32_t pos);
	};

	class DungeonWarrior : public FriendWarrior
	{
		CC_SYNTHESIZE_READONLY(uint32_t, m_nPosition, Position);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nLevel, Level);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nHP, HP);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nMaxHP, MaxHP);
		CC_SYNTHESIZE(bool, m_bRevival, Revival);
		CC_SYNTHESIZE(bool, m_bEnhance, Enhance);
		CC_SYNTHESIZE(bool, m_bRecover, Recover);
		friend class ::FriendsModel;
	public:
		DungeonWarrior();
		virtual ~DungeonWarrior();
		void parse(const Json::Value &val);
		CREATE_FUNC(DungeonWarrior);
		D_INIT_DEFAULT(FriendWarrior);
		const Json::Value getBuffJson();
	};

	class Dungeon : public ValueObjcet
	{
		CC_SYNTHESIZE_READONLY(uint32_t, m_nOngoingID, OngoingID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nOngoingProgress, OngoingProgress);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nFriendID, FriendID);
		CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pClearList, ClearList);
		CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pTodayList, TodayList);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nResetCount, ResetCount);
		CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pWarriors, Warriors);
		
		CC_SYNTHESIZE_READONLY(FriendWarrior *, m_pFriendWarrior, FriendWarrior);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nMaxClear, MaxClear);
		friend class ::FriendsModel;
	public:
		Dungeon();
		virtual ~Dungeon();
		void parse(const Json::Value &val);
		CREATE_FUNC(Dungeon);
		virtual bool init();
	protected:
		std::vector<uint32_t> m_vAssistedFriendID;
	};

	class DropInfo : public ValueObjcet
	{
		CC_SYNTHESIZE_READONLY(uint32_t, m_nID, ID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDropType, DropType);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDropID, DropID);
	public:
		DropInfo();
		virtual ~DropInfo();
		void parse(const CSVDecoder::CSVLine &line);
		CREATE_FUNC(DropInfo);
		D_INIT_DEFAULT(ValueObjcet);
	};

	class BattleInfo : public ValueObjcet
	{
		CC_SYNTHESIZE_READONLY(uint32_t, m_nID, ID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nChapterID, ChapterID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nTurnOrder, TurnOrder);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nSceneID, SceneID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nEffect, Effect);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nLeaderID, LeaderID);
	public:
		BattleInfo();
		virtual ~BattleInfo();
		void parse(const CSVDecoder::CSVLine &line);
		CREATE_FUNC(BattleInfo);
		D_INIT_DEFAULT(ValueObjcet);
	};

	class ChapterInfo : public ValueObjcet
	{
		CC_SYNTHESIZE_READONLY(uint32_t, m_nID, ID);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sName, Name);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nTurns, Turns);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nFriendPos, FriendPos);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nUnlockChapter, UnlockChapter);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRushChapter, RushChapter);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDropType, DropType);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDropID, DropID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDropDetailsID, DropDetailsID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nSceneID, SceneID);
		CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pBattleList, BattleList);
		CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pDropList, DropList);
		CC_SYNTHESIZE_READONLY(bool, m_bUnlock, IsUnlock);
		CC_SYNTHESIZE_READONLY(bool, m_bDone, IsDone);
		CC_SYNTHESIZE_READONLY(bool, m_bRush, CanRush);
		friend class ::FriendsModel;
	public:
		ChapterInfo();
		virtual ~ChapterInfo();
		void parse(const CSVDecoder::CSVLine &line);
		CREATE_FUNC(ChapterInfo);
		virtual bool init();
	};
};

class FriendsModel :public Model
{
	typedef std::map<uint32_t, uint32_t> BuffMap;
	CC_SYNTHESIZE_READONLY(friends::Dungeon *, m_pDungeon, Dungeon);
	CC_SYNTHESIZE_RETAIN(friends::FriendInfo *, m_pMsgReceiver, MsgReceiver);
	CC_SYNTHESIZE_READONLY(friends::ChapterInfo *, m_pSelectedChapter, SelectedChapter);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pRemoveFriends, RemoveFriends);
	CC_SYNTHESIZE_READONLY(friends::FriendBasic *, m_pFound, Found);
	CC_SYNTHESIZE_PASS_BY_REF(BuffMap, m_mBuffItemNum, BuffItemNum);
	CC_SYNTHESIZE_READONLY(uint32_t, m_nRequestsCount, RequestsCount);
	CC_SYNTHESIZE(bool, m_bInTeam, IsInTeam);
	CC_SYNTHESIZE_READONLY(friends::ChapterInfo *, m_pLastChapter, LastChapter);
public:
    FriendsModel();
    virtual ~FriendsModel();
    virtual bool init();

	void parseFriends(const Json::Value &val);
	void parseRecommend(const Json::Value &val);
	void parseFound(const Json::Value &val);
	void parseRequests(const Json::Value &val);
	void parseDungeon(const Json::Value &val);
	void parseDungeonRush(const Json::Value &val);

	cocos2d::CCArray *getFriends(cocos2d::CCArray *pFriends);
	cocos2d::CCArray *getRecommend(cocos2d::CCArray *pRecommend);
	cocos2d::CCArray *getRequests(cocos2d::CCArray *pRequests);

	uint32_t getFriendsCount();

	void syncFriends();
	void syncRecommend();
	void syncRequests();
	void syncDungeon();

	friends::GiftState getGiftState(friends::FriendInfo *pInfo);
	
	void selectFriend(friends::FriendInfo *pInfo);

	void friendGo();
	void friendGoAndResetIfNeeded();

	uint32_t getFriendIndex();

	void friendMessage();

	bool isAssistToday(friends::FriendInfo *pInfo);
	bool isAssociateToday(friends::FriendInfo *pInfo);

	void getChapters(cocos2d::CCArray *pArr);

	void removeRequest(uint32_t nFriendID);
	void addFriend(const Json::Value &val);
	void removeFriendsServer();
	void removeFriends();
	
	void clearRequest();

	void requestSent(const Json::Value &request);

	void setSelectedChapter(friends::ChapterInfo *pChapter);

	void resetDungeon(const Json::Value &requestData);

	bool isFriend(uint32_t freindID);
	bool isFriendsFull();

	uint32_t getResetRest();

	void refreshFriendsIfNeeded();

	void giftReceived(const Json::Value &requestData);
	void giftSent(const Json::Value &requestData);

	void requestsCountGot(const Json::Value &responseData);

	friends::ChapterInfo *getChapterByID(uint32_t cid);

	void friendGiftFull(const Json::Value &requestData);

	void msgSent();
	uint32_t getMsgCD();

	const std::string getBattleName();
private:
	cocos2d::CCArray *m_pFriends;
	cocos2d::CCArray *m_pRecommend;
	cocos2d::CCArray *m_pRequests;

	bool m_bFriendsSynced;
	bool m_bRecommendSynced;
	bool m_bRequestsSynced;

	uint32_t m_nFriendsLastSynced;

	cocos2d::CCArray *m_pChapters;

	void loadDropInfo(const CSVDecoder::CSV &val, cocos2d::CCArray *pArr);
	void loadBattleInfo(const CSVDecoder::CSV &val, cocos2d::CCArray *pArr);
	void loadChapterInfo();

	uint32_t m_nMsgLastSent;
};

#endif /* defined(__QSMY__FriendsModel__) */
