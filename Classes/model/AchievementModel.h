//
//  AchievementModel.h
//  公会
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__AchievementModel__
#define __QSMY__AchievementModel__

#define D_EVENT_ACHIEVEMENT_UPDATE	"achievement_update"
#define D_EVENT_ACHIEVEMENT_FLASH	"achievement_flash"
#define D_EVENT_ACHIEVEMENT_SCOREAWARD	"achievement_scoreaward"

#include "Model.h"
#include "UserModel.h"
#include "DropModel.h"

class AchievementModel;

namespace achievement {
	enum TaskState
	{
		kTaskStateClose = 0,
		kTaskStateDoing = 1,
		kTaskStateDone = 2,
	};

	enum TaskCategory
	{
		kTaskCategoryFight = 1,
		kTaskCategoryLevel = 2,
		kTaskCategoryCollect = 3,
		kTaskCategoryHonor	= 4,
	};
// 		1=副本进度
// 		2=竞技场战斗次数
// 		3=竞技场胜利次数
// 		4=除魔次数
// 		5=账号等级
// 		6=所有上阵英雄等级
// 		7=所有已装备的装备等级
// 		8=英雄数量
// 		9=装备数量
// 		10=竞技场排行榜排名
// 		11=恶鬼罗刹排行榜排名
// 		12=等级排行榜排名
// 		13=战斗力排行榜排名
// 		14=奇遇积分
	enum TaskType
	{
		kTaskTypePve = 1,
		kTaskTypePvp = 2,
		kTaskTypePvpWin = 3,
		kTaskTypeKill	= 4,
		kTaskTypeLevel	= 5,
		kTaskTypeHeroLevel	= 6,
		kTaskTypeEquipLevel	= 7,
		kTaskTypeHeroNum	= 8,
		kTaskTypeEquipNum	= 9,
		kTaskTypePvpRank	= 10,
		kTaskTypeWonderRank	= 11,
		kTaskTypeLevelRank	= 12,
		kTaskTypeFightRank	= 13,
		kTaskTypeWonder	= 14,
		kTaskTypeTower	= 21,
	};

	enum TaskColor
	{
		kTaskColorGreen = 1,
		kTaskColorBlue	= 2,
		kTaskColorPurple	= 3,
		kTaskColorOrange	= 4,
	};

	class TaskBase : public ValueObjcet
	{
		//ID	TaskID	TaskCategory	TaskType	TaskColor	CompleteID	TaskNum	TaskRewardType	TaskRewardID	RewardNum	TaskScore	TaskDescription
		CC_SYNTHESIZE_READONLY(unsigned int, m_uID, ID);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uTaskID, TaskID);
		CC_SYNTHESIZE_READONLY(TaskCategory, m_eCategory, Category);
		CC_SYNTHESIZE_READONLY(TaskType, m_eType, Type);
		CC_SYNTHESIZE_READONLY(TaskColor, m_eColor, Color);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uCompleteID, CompleteID);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uTaskNum, TaskNum);
		CC_SYNTHESIZE_READONLY(drop::DropType, m_eRewardType, RewardType);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uRewardID, RewardID);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uRewardNum, RewardNum);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uScore, Score);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sDescription, Description);

	public:
		TaskBase()
			: m_uID(0)
			, m_uTaskID(0)
			, m_eCategory(kTaskCategoryFight)
			, m_eType(kTaskTypePve)
			, m_eColor(kTaskColorGreen)
			, m_uCompleteID(0)
			, m_uTaskNum(0)
			, m_eRewardType(drop::kDropTypeNone)
			, m_uRewardID(0)
			, m_uRewardNum(0)
			, m_uScore(0)
			, m_sDescription("")
		{

		}
		~TaskBase()
		{

		}
		CREATE_FUNC(TaskBase);
		D_INIT_DEFAULT(ValueObjcet);
	protected:
		friend class ::AchievementModel;
	};

	/*成就動態信息*/
	class TaskInfo:public ValueObjcet
	{
	public:
		TaskInfo():task_id(0),state(0),taskcate(kTaskCategoryFight),baseInfo(NULL),progress(0){};
	public:
		CREATE_FUNC(TaskInfo);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::AchievementModel;

		CC_SYNTHESIZE_READONLY(TaskBase *, baseInfo, BaseInfo);
		CC_SYNTHESIZE_READONLY(unsigned int, task_id, TaskID);          //任務ID
		CC_SYNTHESIZE_READONLY(unsigned int, state, TaskState);      //任務狀態
		CC_SYNTHESIZE_READONLY(unsigned int, progress, TaskProgress);//任務進度
		CC_SYNTHESIZE_READONLY(TaskCategory, taskcate, TaskCate);//任務進度
	};

	/*成就動態信息*/
	class AwardInfo:public ValueObjcet
	{
	public:
		AwardInfo():m_index(0),m_score(0),m_type(drop::kDropTypeNone),m_id(0),m_amount(0){};
	public:
		CREATE_FUNC(AwardInfo);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::AchievementModel;
		CC_SYNTHESIZE_READONLY(unsigned int, m_index, Index);			//序号
		CC_SYNTHESIZE_READONLY(unsigned int, m_score, Score);			//积分
		CC_SYNTHESIZE_READONLY(drop::DropType, m_type, Type);		//类型
		CC_SYNTHESIZE_READONLY(unsigned int, m_id, ID);						//奖励物品ID
		CC_SYNTHESIZE_READONLY(unsigned int, m_amount, Amount);	//奖励数量
	};
}

class AchievementModel : public Model
{
	//成就积分上限
	CC_SYNTHESIZE_READONLY(unsigned int, m_uTotalScore, TotalScore);
	//玩家当前积分
	CC_SYNTHESIZE_READONLY(unsigned int, m_uCurrentScore, CurrentScore);
	//玩家当前领取index
	CC_SYNTHESIZE_READONLY(unsigned int, m_uScoreRewardId, ScoreRewardId);
	//积分奖品数据
	CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pDictAward, DictAward);
	//新成就
	CC_SYNTHESIZE(unsigned int, m_uCurrentAchievement, CurrentAchievement);
public:
	AchievementModel()
		: m_pDictAchievment(NULL)
		, m_pTaskListDict(NULL)
		, m_pDictAward(NULL)
		, m_uTotalScore(0)
		, m_uCurrentScore(0)
		, m_uScoreRewardId(0)
		, m_uCurrentAchievement(0)
	{

	}
	~AchievementModel();
	virtual bool init();

	cocos2d::CCArray *getAchievementByCategory(achievement::TaskCategory);
	uint32_t getAchievedNumByCate(achievement::TaskCategory);
	void getAward(uint32_t taskid);
	void getAwardCallBack(const Json::Value& requestData,const Json::Value& responseData);
	void reloadCallBack(const Json::Value& requestData,const Json::Value& responseData);
	void getListFromServer();
	void updateCurrentAchievement(const Json::Value &responseData);
	void onGetScoreAward(const Json::Value& requestData,const Json::Value& responseData);
	//獲取成就列表
	void parseTasks(const Json::Value &val);

	void parseOneTask(const Json::Value &val);

    virtual void reset();

private:
	void LoadAchievementData(const CSVDecoder::CSV &val);
	void LoadAwardData(const CSVDecoder::CSV &val);
	//游戏成就列表
	cocos2d::CCDictionary *m_pDictAchievment;
	//我的成就列表
	cocos2d::CCDictionary *m_pTaskListDict;  
};
#endif