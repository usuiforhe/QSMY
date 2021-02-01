//
//  DestinyModel.h
//  天命信息
//
//  Created by OnePiece on 13-6-5.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DestinyModel__
#define __QSMY__DestinyModel__

#include "Model.h"
#include "WarriorModel.h"

//#define D_EVENT_DESTINYS_UPDATED "destinys updated"
#define D_EVENT_DESTINY_UPDATED "destiny updated"

#define kMaxLevelForAnySoulDestiny 60

#define MAX_SLOTS 6

namespace destiny
{
    class DestinyInfo;
    
    
    //天命类型
    enum DestinyRequireType
    {
        kDestinyRequireAssign = 1, //指定魂魄
        kDestinyRequireAny = 2     //任意魂魄
    };
    
    //天命奖励类型
    enum DestinyRewardType
    {
        kDestinyRewardOnce = 1, //一次性奖励
        kDestinyRewardDaily = 2,  //每日领取奖励
        kDestinyRewardProperty = 3   //属性奖励
    };
    
    enum DestinyRewardItemType
    {
        kDestinyRewardItemNone  =0, //客户端默认值
        //kDestinyRewardItemWarrior = 1,  //奖励弟子
        kDestinyRewardItemEquip = 2,    //奖励装备
        kDestinyRewardItemItem = 3,    //奖励道具
        kDestinyRewardItemPower = 4,    //奖励体力
        kDestinyRewardItemCoin = 5,     //奖励金币
        kDestinyRewardItemProperty = 6,  //奖励属性
        kDestinyRewardItemOutSideProperty = 7  //外围属性
    };
    
    //天命品质
    enum DestinyGrade
    {
        kDestinyGradeGreen = 1,    //绿色
		kDestinyGradeBlue,          //蓝色
    };
    
    class DestinySlot : public ValueObjcet
    {
    public:
        DestinySlot()
        :m_nWarriorID(0)
        ,m_nCurrentNum(0)
        ,m_nUpgradeNum(0)
        ,m_pDestinyInfo(NULL)
        {}
        
        virtual ~DestinySlot()
        {
            
        }
        
        D_INIT_DEFAULT(ValueObjcet);
        
        CREATE_FUNC(DestinySlot);
        
        void copyFrom(DestinySlot* pSlot)
        {
            m_nWarriorID = pSlot->m_nWarriorID;
            m_nCurrentNum = pSlot->m_nCurrentNum;
            m_nUpgradeNum = pSlot->m_nUpgradeNum;
            m_pDestinyInfo = pSlot->m_pDestinyInfo;
        }
        
        CC_SYNTHESIZE(uint32_t, m_nWarriorID, WarriorID);   //升级这个等级的slot需要的弟子ID 如果位0 表示任意弟子都可以用来升级这个slot
        CC_PROPERTY(uint32_t, m_nCurrentNum, CurrentNum); //这个等级的slot 当前的魂魄数量
        CC_SYNTHESIZE(uint32_t, m_nUpgradeNum, UpgradeNum); //升级这个等级的slot 需要的魂魄数量
        
    public:
        DestinyInfo *m_pDestinyInfo; //weak
    public:
        
        //这个天命slot 是否已经满级
        virtual bool isSlotUnlock();
        
        
        virtual bool isCanCollect();
    };
    
    class DestinySlotAny:public DestinySlot
    {
    public:
        DestinySlotAny()
        {
            
        }
        
        CREATE_FUNC(DestinySlotAny);
        D_INIT_DEFAULT(DestinySlot);
        
        uint32_t getLvUpgradeNum();
        uint32_t getLvCurrentNum();
        
        virtual bool isCanCollect();
    private:
        virtual uint32_t getWarriorID(){return 0;}//private
        virtual bool isSlotUnlock(){return false;};//private
    };
    
    //天命等级信息
    class DestinyUpgradeInfo : public ValueObjcet
    {
    public:
        DestinyUpgradeInfo()
        :m_pSlotInfosArray(NULL)
        ,m_nRewardItemType(kDestinyRewardItemNone)
        ,m_nRewardItemID(0)
        ,m_nRewardItemNum(0)
        ,m_pDestinyInfo(NULL)
        {
            
        }
        virtual ~DestinyUpgradeInfo()
        {
            CC_SAFE_RELEASE(m_pSlotInfosArray);
        }
        
        bool init()
        {
            if(!ValueObjcet::init()) return false;
            m_pSlotInfosArray = cocos2d::CCArray::create();
            CC_SAFE_RETAIN(m_pSlotInfosArray);
            return true;
        }
        
        CREATE_FUNC(DestinyUpgradeInfo);
        
        void copyFrom(DestinyUpgradeInfo* pDestinyUpgradeInfo)
        {
            this->m_nRewardItemType = pDestinyUpgradeInfo->m_nRewardItemType;
            this->m_nRewardItemID = pDestinyUpgradeInfo->m_nRewardItemID;
            this->m_nRewardItemNum = pDestinyUpgradeInfo->m_nRewardItemNum;
        }
        
        cocos2d::CCArray *m_pSlotInfosArray;   //天命槽信息
        
    protected:
        CC_SYNTHESIZE(DestinyRewardItemType, m_nRewardItemType, RewardItemType);   //天命奖励内容类型
        CC_SYNTHESIZE(uint32_t, m_nRewardItemID, RewardItemID);   //天命奖励内容ID
        CC_SYNTHESIZE(float, m_nRewardItemNum, RewardItemNum);  //天命奖励内容num
        
        DestinyInfo* m_pDestinyInfo;
        
    public:
        
        virtual uint32_t getRewardItemNum()
        {
            return (uint32_t)m_nRewardItemNum;
        }
        
        void addSlot(DestinySlot * pSlot)
        {
            if(!pSlot) return;
            m_pSlotInfosArray->addObject(pSlot);
        }
    };
    
    class DestinyAnyUpgradeInfo:public DestinyUpgradeInfo
    {
        DestinyAnyUpgradeInfo()
        {
            
        }
        virtual ~DestinyAnyUpgradeInfo()
        {
            
        }
    public:
        CREATE_FUNC(DestinyAnyUpgradeInfo);
        D_INIT_DEFAULT(DestinyUpgradeInfo);
        
        virtual float getRewardItemNum() const;
        virtual uint32_t getRewardItemNum();
    };
    
    //天命基本信息
    class DestinyInfo : public ValueObjcet
    {
    public:
        DestinyInfo()
        :m_nID(-1)
        ,m_sName("")
        ,m_nLevel(1)
        ,m_nMaxLevel(0)
        ,m_nRewardTime(0)
        ,m_nRequireType(kDestinyRequireAssign)
        ,m_nRewardType(kDestinyRewardOnce)
        ,m_kDestinyGrade(kDestinyGradeGreen)
        ,m_pUpgradeInfoArray(NULL)
        ,m_nLimitLevel(0)
        {
        }
        
        ~DestinyInfo()
        {
            CC_SAFE_RELEASE(m_pUpgradeInfoArray);
        }
        
        bool init()
        {
            if(!ValueObjcet::init()) return false;
            m_pUpgradeInfoArray = cocos2d::CCArray::create();
            CC_SAFE_RETAIN(m_pUpgradeInfoArray);
            return true;
        }
        
        CREATE_FUNC(DestinyInfo);
        
        
        //制作随意插的副本 用来保存临时数据
        void copyFrom(DestinyInfo* pDestinyInfo)
        {
            CCAssert(pDestinyInfo->isRequireAny(), "");
            this->m_nID = pDestinyInfo->m_nID;
            this->m_sName = pDestinyInfo->m_sName;
            this->m_nLevel = pDestinyInfo->m_nLevel;
            this->m_nMaxLevel = pDestinyInfo->m_nMaxLevel;
            this->m_nRequireType = pDestinyInfo->m_nRequireType;
            this->m_nRewardType = pDestinyInfo->m_nRewardType;
            this->m_nRewardTime = pDestinyInfo->m_nRewardTime;
            this->m_kDestinyGrade = pDestinyInfo->m_kDestinyGrade;
            this->m_nLimitLevel = pDestinyInfo->m_nLimitLevel;
            this->m_pUpgradeInfoArray->removeAllObjects();
            DestinyAnyUpgradeInfo* pDestinyUpgradeInfo = DestinyAnyUpgradeInfo::create();
            pDestinyUpgradeInfo->copyFrom(pDestinyInfo->getCurrentDestinyUpgradeInfo());
            pDestinyUpgradeInfo->m_pDestinyInfo = this;
            this->m_pUpgradeInfoArray->addObject(pDestinyUpgradeInfo);
            DestinySlotAny* pSlot = DestinySlotAny::create();
            pSlot->copyFrom(pDestinyInfo->getDestinySlotByIndex(0));
            pSlot->m_pDestinyInfo = this;
            pDestinyUpgradeInfo->addSlot(pSlot);
        }
        
    private:
        CC_SYNTHESIZE(uint32_t, m_nID, ID);                             //天命id
        CC_SYNTHESIZE(std::string, m_sName, Name);                      //天命名字
        CC_SYNTHESIZE(uint16_t, m_nLevel, Level);                       //天命等级
        
        CC_SYNTHESIZE(DestinyRequireType, m_nRequireType, RequireType); //天命需要类型
        CC_SYNTHESIZE(DestinyRewardType, m_nRewardType, RewardType);    //天命奖励类型
        CC_SYNTHESIZE(uint32_t, m_nRewardTime, RewardTime);             //最近一次的领取时间
        CC_SYNTHESIZE(DestinyGrade,m_kDestinyGrade,DestinyGrade);       //天命品质
        CC_SYNTHESIZE(uint16_t,m_nLimitLevel,LimitLevel);               //解锁等级
        cocos2d::CCArray *m_pUpgradeInfoArray;
        
        CC_PROPERTY(uint16_t, m_nMaxLevel, MaxLevel);                  //天命最大等级
        
    public:
        
        //可收集状态需要刷新（弟子或魂魄有变化）
        void setCanCollectDirty()
        {
            postNotification(D_EVENT_DESTINY_UPDATED);
        }
        
        
        //一次性领奖 能领奖的时候显示真正等级 不然显示级别+1
        uint16_t getLvDisplay()
        {
            if(isRequireAny())
            {
                return (isFullLevel() || isEqUserLevel())?m_nLevel:m_nLevel+1;
            }
            
            return isCanReward() && m_nRewardType==kDestinyRewardOnce
            ?m_nLevel
            :MIN(m_nLevel+1, m_nMaxLevel);
        }
        
        float getPercent();
        
        //是否可收集
        bool isCanCollect();
        
        //任意插的级别不能高过玩家级别 是否等于玩家等级
        bool isEqUserLevel();
        
        //是否满级
        bool isFullLevel();
        
        //是否可以领取奖励
        bool isCanReward();
        
        //是否已经领奖
        bool isReWard();
        
        //是否需要倒计时
        bool isNeedShowInterval();
        
        //是否需要领取奖励（是否是领取型奖励）
        bool isNeedReAward()
        {
            return m_nRewardType != kDestinyRewardProperty;
        }
        
        //判断是否需要任意魂魄类型
        bool isRequireAny()
        {
            return m_nRequireType == destiny::kDestinyRequireAny;
        }
        
        //增加一个等级信息的接口
        void addDestinyUpgradeInfo(DestinyUpgradeInfo *destinyUpgradeInfo)
        {
            if(! destinyUpgradeInfo) return;
            m_pUpgradeInfoArray->addObject(destinyUpgradeInfo);
        }
        
        //获取当前等级信息的接口
        DestinyUpgradeInfo * getCurrentDestinyUpgradeInfo()
        {
            return (DestinyUpgradeInfo *)m_pUpgradeInfoArray->objectAtIndex(getUpgradeIndexByLevel(m_nLevel));
        }
        
        //获取当前等级有多少个槽的接口
        uint32_t  getDestinySlotCount()
        {
            DestinyUpgradeInfo *pDestinyUpgradeInfo = getCurrentDestinyUpgradeInfo();
            return pDestinyUpgradeInfo->m_pSlotInfosArray->count();
        }
        
        //获取当前等级解锁槽的接口
        destiny::DestinySlot *getDestinySlotByIndex(uint32_t index)
        {
            DestinyUpgradeInfo *pDestinyUpgradeInfo = getCurrentDestinyUpgradeInfo();
            return (destiny::DestinySlot *)pDestinyUpgradeInfo->m_pSlotInfosArray->objectAtIndex(index);
        }
        
        int getUpgradeIndexByLevel(uint16_t level)
        {
            if(isRequireAny()) return 0;
            return MIN(level, m_nMaxLevel-1);
        }
    };
    
    class DestinyColorConfig:public ValueObjcet
    {
    public:
        CREATE_FUNC(DestinyColorConfig);
        D_INIT_DEFAULT(ValueObjcet);
        
    public:
        cocos2d::ccColor3B color;
        
    };
}

class DestinyModel : public Model
{
public:
    DestinyModel();
    virtual ~DestinyModel();
    bool init();
    void loadData();
    
private:
    cocos2d::CCDictionary *m_pDestinyDic;
    std::map<uint32_t, destiny::DestinyRequireType> m_RequireTypeMap;
    cocos2d::CCDictionary *m_DestinyColorMap;
    
public:
    
    
    void updateDestinys(const Json::Value &val);
    void updateDestiny(const Json::Value &val);
    
    
    /**
     *	@brief	领奖
     *
     *	@param 	did 	天命编号
     *	@param 	ts      领奖时间
     */
    void rewardDestiny(uint32_t did,uint32_t ts);
    
    
    cocos2d::CCArray* getDestinyListByRequireType(destiny::DestinyRequireType type);
    
    /**
     *	@brief	计算天命增加的属性
     *
     *	@param 	type 	属性类型
     *
     *	@return	
     */
    uint32_t destinyAddProps(warrior::WarriorPropType type);

        
    destiny::DestinyInfo * getDestinyInfoByIndex(destiny::DestinyRequireType type,uint32_t index)
    {
        return (destiny::DestinyInfo*)getDestinyListByRequireType(type)->objectAtIndex(index);
    }
    
    
    uint32_t getDestinyIndexByDid(uint32_t did);
    
    
    destiny::DestinyInfo * getDestinyInfoByID(uint32_t did);
    
    
    int getDestinyCount(destiny::DestinyRequireType type)
    {
        return getDestinyListByRequireType(type)->count();
    }
    
    /* 获取对应品阶的颜色值 */
    destiny::DestinyColorConfig* getDestinyColorConfig(destiny::DestinyGrade grade)
    {
        return (destiny::DestinyColorConfig*)m_DestinyColorMap->objectForKey(grade);
    }
    
    cocos2d::ccColor3B getDestinyColorByGrade(destiny::DestinyGrade grade)
	{
		destiny::DestinyColorConfig* pDestinyColorConfig = getDestinyColorConfig(grade);
		return pDestinyColorConfig?pDestinyColorConfig->color:cocos2d::ccc3(0,0,0);
	}
    
private:
    void parseDestinys(const CSVDecoder::CSV &val);
    void loadColorConfig(const CSVDecoder::CSV &val);
};

#endif /* defined(__QSMY__DestinyModel__) */
