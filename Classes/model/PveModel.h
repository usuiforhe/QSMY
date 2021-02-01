//
//  PveModel.h
//  QSMY
//
//  Created by wanghejun on 13-3-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PveModel__
#define __QSMY__PveModel__

#include "Model.h"
#include "DropModel.h"
#include "BattleModel.h"

#define D_EVENT_PVE_MAP_UPDATED         "pve map updated"
#define D_EVENT_PVE_CHAPTERS_UPDATED	"pve chapters updated"
#define D_EVENT_PVE_CHAPTER_UPDATED     "pve chapter updated"
#define D_EVENT_PVE_CHAPTER_PRES_ADD    "pve chapter pres add" //增加声望
#define D_EVENT_PVE_ELITE_UPDATED       "pve elites updated" //精英更改

#define SFX_FIGHT "fight"
#define SFX_PVE_EXPLORE_N "explore"

#define FIRST_CHAPTER_ID 1

class PveModel;
namespace pve
{    
    class ChapterInfo;
    
    
    enum PveRewardType
    {
        kPveRewardTypeNone = drop::kDropTypeNone,
        kPveRewardTypeItem = drop::kDropTypeItem,
        kPveRewardTypeEquip = drop::kDropTypeEquip,
        kPveRewardTypeWarrior = drop::kDropTypeWarrior,
        kPveRewardTypeSoul = drop::kDropTypeSoul,
		kPveRewardTypeCoin = drop::kDropTypeCoin,
		kPveRewardTypePoint = drop::kDropTypePoint
    };
    
    
    enum MapType
    {
        eTypeNormal = 0,
        eTypeLittleBoss,
        eTypeBoss
    };

	enum PveFightType
	{
		PveFightNormal = 0, //剧情副本
		PveFightElite,       //精英副本
		PveFightActivity,    //副本活动
	};
    
	enum PveFailureType
	{
		PveFailureNone = 0,
		PveFailureFormationEmpty = 1,
		PveFailureEquipEnhance,
		PveFailureWarriorPromote,
		PveFailureWarriorDevelop,
		PveFailureWarriorSkill,
		PveFailureWarriorLevel,
		PveFailureCard,
		PveFailureEquipEmpty,
		PveFailureDefault = PveFailureCard,
		PveFailureNormalSize = PveFailureEquipEmpty,
		PveFailureHigherFirst = 1001,
		PveFailureWarriorPromoteItem = PveFailureHigherFirst,
		PveFailureWarriorDevelopItem,
		PveFailureWarriorSkillItem,
		PveFailureWarriorFate,
		PveFailureHigherLast,
	};

	enum PveChapterProgress
	{
		eChapterNew = 0,//尚未全部完成
		eChapterCompete,//全部完成，未全部三星
		eChapterCleared//全部TMD三星
	};

	enum PveChapterType
	{
		PveChapterNormal=0,     //普通副本
		PveChapterActivity=1,	//活动副本
	};

	class MapDialogInfo:public battle::BattleDialogInfo
	{
	public:
		MapDialogInfo()
			:m_Direction(battle::kDialogDirectionLeft)
			,m_uWid(0)
			,m_Content("")
		{

		}
		CREATE_FUNC(MapDialogInfo);
		D_INIT_DEFAULT(battle::BattleDialogInfo);

		CC_SYNTHESIZE_READONLY(battle::DialogDirection, m_Direction, Direction);
		CC_SYNTHESIZE_READONLY(std::string, m_Content, Content);
		CC_SYNTHESIZE_READONLY(uint32_t, m_uWid, WarriorId);

		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sSound, Sound);

		friend class ::PveModel;
	};

	class SceneInfo:public ValueObjcet
	{
	public:
		SceneInfo()
			:id(0)
			,effect(0)
		{
		}
		CREATE_FUNC(SceneInfo);
		D_INIT_DEFAULT(ValueObjcet);
		uint32_t id;
		uint32_t effect;
	};

	class ChapterExtraInfo:public ValueObjcet
	{
	public:
		ChapterExtraInfo()
			:id(0)
			,pt(0.f, 0.f)
			,res(0)
			,section(0)
			,ani(0)
		{
		}
		CREATE_FUNC(ChapterExtraInfo);
		D_INIT_DEFAULT(ValueObjcet);
		uint32_t id;
		cocos2d::CCPoint pt;
		uint32_t res;
		uint32_t section;
		uint32_t ani;
	};

    //地图信息
    class MapInfo
    :public ValueObjcet
    {    
        CC_SYNTHESIZE_READONLY(uint32_t, iMapId, MapId);        
    public:
        MapInfo(uint32_t _iMapId)
        :iMapId(_iMapId)
        ,chapter_id(0)
        ,map_name("")
        ,challenge_limit(0)
        ,map_des("")
        ,fight_open(0)
        ,todayTimes(0)
        ,lastBattleTs(0)
        ,iScores(0)
        ,iTimes(0)
        ,chapterInfo(0)
        ,bUnlock(false)
        ,enemy_id(0)
		,posX(0)
		,posY(0)
		,scale(1.0)
		,eliteCoin(0)
		,eliteGold(0)
		,eliteUnlock(false)
		,fightType(0)
		{}

        static MapInfo* create(uint32_t _iMapId)
        {
            MapInfo* pRet = new MapInfo(_iMapId);
            if(pRet && pRet->init())
            {
                pRet->autorelease();
                return pRet;
            }
            CC_SAFE_DELETE(pRet);
            return NULL;
        }
        
        void setTodayTimes(uint32_t times);
        
        void updateRush(const Json::Value &val);
        
        uint32_t        chapter_id;         //所属章节id
        std::string     map_name;           //关卡名
		uint16_t        challenge_limit;    //挑战次数限制
		uint16_t		power_cost;			//消耗体力
        std::string     map_des;            //关卡描述
        uint16_t        fight_open;         //每关探索次数（几个点）
		std::vector<uint32_t>   prizes_type;         //奖励类型
		std::vector<uint32_t>        prizes_id;           //奖励ID	
		ChapterInfo*    chapterInfo;         //章节信息        
		bool            bUnlock;

        uint8_t         iScores;            //关卡评分  0 1 2 3
        uint32_t        iTimes;             //探索次数
		uint32_t		todayTimes;			//今日战斗次数
		uint32_t		lastBattleTs;		//最后战斗时间
        uint16_t        mapType;            //关卡类型（0普通，1小boss，2大boss）
        uint16_t        mapLv;              //建议等级
        
        uint32_t        enemy_id;           //头兵
		uint32_t        enemyLittle_id;      //小兵

		uint32_t        eliteCoin;       //精英铜币
		uint32_t        eliteGold;       //精英金币
		bool            eliteUnlock;      //精英解锁
		uint8_t         fightType;         //0普通副本 1精英副本

		uint32_t	posX;
		uint32_t	posY;
		float	scale;
        
        //剩余次数
        uint32_t remainTimes()
        {
            int times = challenge_limit - todayTimes;
            return MAX(times, 0);
        }
        
    };
    
    
    class PrestigeReward
    :public ValueObjcet
    {
    public:
        PrestigeReward()
        :prestige_reward_need_viplv(0)
        ,prestige_reward_type(kPveRewardTypeNone)
        ,prestige_reward_quantity(0)
        ,prestige_reward_id(0)
        {
            
        }
        
        CREATE_FUNC(PrestigeReward);
        D_INIT_DEFAULT(ValueObjcet);
        
        uint8_t                 prestige_reward_need_viplv; //章节声望奖励需要vip等级
        PveRewardType           prestige_reward_type;       //章节声望奖励类型
        uint32_t                prestige_reward_quantity;   //章节声望奖励数量
        uint32_t                prestige_reward_id;         //章节声望奖励装备id
    };
    
    //章节信息
    class ChapterInfo
    :public ValueObjcet
    {
        CC_SYNTHESIZE_READONLY(uint32_t, chapter_id, Cid);  //章节编号
    public:
        ChapterInfo(uint32_t _cid)
        :chapter_id(_cid)
        ,prestige(0)
        ,pres(0)
        ,chapter_unlock(false)
        ,chapter_box_bGet(false)
        ,lastMid(0)
        ,firstMid(0)
        ,grade_award(false)
        ,m_pPrestigeRewards(NULL)
		,great_num(0)
		,chapterType(0)
        {}
        
        static ChapterInfo* create(uint32_t _cid)
        {
            ChapterInfo* pRet = new ChapterInfo(_cid);
            if(pRet && pRet->init())
            {
                pRet->autorelease();
                return pRet;
            }
            CC_SAFE_DELETE(pRet);
            return NULL;
        }
        
        friend class ::PveModel;
        
        virtual ~ChapterInfo()
        {
            CC_SAFE_RELEASE(m_pPrestigeRewards);
        }
        
        bool init()
        {
            if(!ValueObjcet::init())return false;
            m_pPrestigeRewards = cocos2d::CCArray::create();
            CC_SAFE_RETAIN(m_pPrestigeRewards);
            return true;
        }
        
        
        bool canGetAllStarReward();
        bool canGetPrestigeReward();
        
        //获取需要显示的奖励
        pve::PrestigeReward* getAwardToShow();
        
        pve::PrestigeReward* getAwardByVipLv(uint8_t lv);
        pve::PrestigeReward* getAwardByIndex(uint32_t index);
        
        //是否所有声望奖励都被领了
        bool isReWardGottenAll();
        
        //该vip等级的vip是否被领了
        bool isPresAwardIsGotten(uint8_t vip);
        void setChpterAwardGotten(uint8_t vip);
        
        //该vip等级的vip是否能领
        bool isPresAwardCanGet(uint8_t vip);
        
        //章节最大声望值
        uint16_t getMaxPrestige();
        
		//獲取章節完成的進度
        PveChapterProgress getProgress();
        
        uint32_t getPresCount()
        {
            return m_pPrestigeRewards->count();
        }
    public:
        std::string chapter_name;		//章节名
        std::string chapter_des;		//章节描述
        uint16_t    pres;				//章节当前声望值
        bool        chapter_unlock;     //章节是否已经解锁
		uint32_t	firstMid;			//第一关卡id
		uint32_t	lastMid;			//最后关卡id
        bool        grade_award;        //是否领取三星奖励
        uint32_t    great_num;          //已经获得三星的关卡数
		int         chapterType;        //副本类型 普通副本 活动副本
        
    private:
        std::string skeletonName;
        uint32_t     chapter_box_bGet;   //章节宝箱是否已经领取
        uint16_t    prestige;			//章节最大声望值
        cocos2d::CCArray *m_pPrestigeRewards; //声望奖励
    };


	class PveFailureInfo
		:public ValueObjcet
	{
	public:
		PveFailureInfo()
			:nType(PveFailureCard)
			,sText("")
			,uScene(0)
			,uIndex(0)
			,sImage("")
			,uLevel(0)
		{

		}

		CREATE_FUNC(PveFailureInfo);
		D_INIT_DEFAULT(ValueObjcet);

		PveFailureType nType;
		std::string sText;
		uint8_t uScene;
		uint8_t uIndex;
        std::string sImage;
		uint32_t uLevel;
		uint32_t uPriority;
	};
}

class PveModel : public Model
{
public:
    typedef cocos2d::CCArray ChapterMap;    //std::vector<pve::ChapterInfo*>
    typedef cocos2d::CCArray MapInfoMap;    //std::vector<pve::MapInfo*>
	typedef cocos2d::CCDictionary FailureMap;
public:
    PveModel();
    virtual ~PveModel();
    virtual bool init();
    CC_SYNTHESIZE(pve::PveFightType,m_pFightType ,FightType);
	CC_SYNTHESIZE(int,m_pActivityID ,ActivityID);  //副本活动id
	CC_SYNTHESIZE(int,m_nActivityAwardState,ActivityAwardState);//副本活动通关领奖状态 1领取 0未领取
    /**
     *	@brief	获取章节地图数量
     *
     *	@param 	cid 	章节编号
     *
     *	@return	地图数
     */
    uint32_t getMapsCount(uint32_t cid);

    
    void getChapters();
    void getMaps(uint32_t cid);
    void getElites();
	void getElitesHandler(const Json::Value &requestData,const Json::Value &responseData);

    void updateMaps(uint32_t cid,const Json::Value &val);
    void updateMap(const Json::Value &val);
	//void parseExploreActivity(const Json::Value &val);//进游戏初始化副本活动数据
    
    void updateChapters(const Json::Value &val);
    void updateChapter(const Json::Value &val);

	void updateChapterMap(cocos2d::CCArray* chapterArr);
    
    pve::MapInfo* getMapInfo(uint32_t mid);   
    pve::MapInfo* getEliteInfo(uint32_t mid); 
    pve::MapInfo* getMapInfoByIndex(uint32_t index)
    {
        if(index>=m_Map->count()) return NULL;
        return (pve::MapInfo*)m_Map->objectAtIndex(index);
    }
    
    /**
     *	@brief	获取章节关卡列表
     *
     *	@param 	id 	章节编号
     *	@param 	vec 	关卡列表
     */
    void getMapsByChapterId(uint32_t id,MapInfoMap& vec);

    /**
     *	@brief	获取章节已解锁关卡列表
     *
     *	@param 	id  章节编号
     *	@param 	vec  已解锁关卡列表
     */
    void getUnlockedMapsByChapterId(uint32_t id,MapInfoMap& vec);
	void getUnlockedElite(MapInfoMap& vec);
	uint32_t getUnlockEliteCount();
    /**
     *	@brief	获取章节已解锁关卡列表
     *
     *	@param 	id  章节编号
     */
    uint32_t getUnlockMapsCountByChapterId(uint32_t id);
    
    pve::ChapterInfo* getChapterInfo(uint32_t cid);
    
    pve::ChapterInfo* getChapterInfoByMapId(uint32_t mid)
    {
        return getChapterInfo(getMapInfo(mid)->chapter_id);
    }
    
    pve::ChapterInfo* getChapterInfoByIndex(uint32_t index)
    {
        return (pve::ChapterInfo*)m_Chapters->objectAtIndex(index);
    }
    
	pve::ChapterInfo* getActivityChapterInfoByIndex(uint32_t index)
	{
		return (pve::ChapterInfo*)m_ActivityChapters->objectAtIndex(index);
	}
    
    uint32_t getUnlockChartersCount()
    {
        return m_UnlockChapters->count();
    }

	uint32_t getChaptersCount()
	{
		return m_Chapters->count();
	}

	uint32_t getUnlockActivityChartersCount()
	{
		return m_UnlockActivityChapters->count();
	}

	uint32_t getActivityChaptersCount()
	{
		return m_ActivityChapters->count();
	}

	//副本活动距结束时间
	unsigned int getActivityDownTime();

	void addChapterObject(pve::ChapterInfo* pChapterInfo);
    
    
    void reset();

    pve::PveFailureInfo *getFailureInfo();
	pve::PveFailureInfo *getFailureInfoHigher();
	bool isFailureReason(pve::PveFailureType eType);
private:
    void resetUnlockChapters(); 
    void unlockNewChapter(const int &cid);
    void unlockNewMap(pve::MapInfo *pMapInfo);
private:    
    void loadData();
    void parseChapter(const CSVDecoder::CSV &val);
    void parseMap(const CSVDecoder::CSV &val);
	void parseElite(const CSVDecoder::CSV &val);
	void parseFailure(const CSVDecoder::CSV &val);

	pve::PveFailureType getFailureReason();
	void freshChapterByMid(uint32_t mid); //根据最新关卡mid（剧情or活动）更新章节列表
private:
    ChapterMap*		m_Chapters;
    ChapterMap*		m_UnlockChapters;
	ChapterMap*     m_ActivityChapters;          //活动副本章节
	ChapterMap*		m_UnlockActivityChapters;    //解锁活动副本
    MapInfoMap*		m_Map;
    MapInfoMap*     m_Elite;
    
	CC_SYNTHESIZE(int, m_nFirstChapterId, FirstChapterId);//第一章的ID
	CC_SYNTHESIZE(int, m_nLastMapIDBeforeUnlock, LastMapIDBeforeUnlock);
    CC_SYNTHESIZE(int, m_nLastShowMapID, LastShowMapID);
    CC_SYNTHESIZE(int, m_nLastShowChapterID, LastShowChapterID);
	CC_SYNTHESIZE(int,m_nLastEliteID,LaseEliteID);
    
    CC_SYNTHESIZE(bool, m_bShowChapter, ShowChapter);
    CC_SYNTHESIZE(bool, m_bShowMap, ShowMap);

	CC_SYNTHESIZE(int, m_nLastShowActivityChapterID, LastShowActivityChapterID);          //活动副本
	CC_SYNTHESIZE(int, m_nLastShowActivityMapID, LastShowActivityMapID);                  //活动副本
	CC_SYNTHESIZE(int, m_nLastActivityMapIDBeforeUnlock, LastActivityMapIDBeforeUnlock);  //活动副本
	CC_SYNTHESIZE(bool, m_bShowActivityChapter, ShowActivityChapter);                     //活动副本
	CC_SYNTHESIZE(bool, m_bShowActivityMap, ShowActivityMap);                             //活动副本

	FailureMap	*m_pFailureMap;

public:
	cocos2d::CCArray* getDialog(int cid);

	bool isChapterCleared(uint32_t cid);
private:
	void parseMapDialog(const CSVDecoder::CSV &val);
	cocos2d::CCArray* createDialog(uint32_t mapId);
	cocos2d::CCDictionary* m_pMapDialogs;


	cocos2d::CCDictionary* m_pChapterExtraInfo;
	void parseChapterExtraInfo(const CSVDecoder::CSV &val);

	cocos2d::CCDictionary	*m_pChapterDrop;
	cocos2d::CCDictionary   *m_pEliteDrop;

public:
	pve::ChapterExtraInfo * getChapterExtraInfo(uint32_t id);
	
	cocos2d::CCArray	*getChapterDrop(unsigned int id);
	cocos2d::CCArray    *getEliteDrop(unsigned int id);

	bool isActivityShow();   //活动副本是否显示
	bool isCanGetActivityAward(); //活动副本通关奖励是否可领
private: 
	unsigned int m_tStartTime;
	unsigned int m_tEndTime;
	int m_ActivityMid;
};

#endif /* defined(__QSMY__PveModel__) */

