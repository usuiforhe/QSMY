//
//  NavigationModel.h
//  导航数据配置
//
//  Created by wanghejun on 13-7-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__NavigationModel__
#define __QSMY__NavigationModel__

#include "Model.h"


#define D_EVENT_NAV_LOCK_STATUS_UPDATE "nva lock update"

class NavigationModel;

namespace navigation
{
    /**
     *
     *  因为要在编辑器填和配置表中用到枚举值
     *  枚举后面都加上固定值，一经添加不要修改值 以免和编辑器配置表对应出错！！！只能增减
     *
     */
    enum NavigationType 
    {
        kNavNIL             = 0,        //无
        kNavHome            = 1,        //首页
        kNavFormation       = 2,        //阵容
        kNavPve             = 3,        //pve
        kNavExperience      = 4,        //历练
        kNavWonder          = 5,        //奇遇（活动）
		kNavMall            = 6,        //市集（商城）
		kNavTeam			= 7,		//组队
        kNavWarrior         = 8,        //弟子
        kNavInventory       = 9,        //背包（道具）
        kNavDestiny         = 10,       //天命
        kNavEquip           = 11,       //装备
        kNavLadder          = 12,       //论剑 pvp
        //kNavBook            = 13,       //武林谱（图鉴）
        //kNavAnnounce        = 14,       //公告
        kNavSetting         = 15,       //设置
        kNavTreasure        = 16,       //心法
        kNavLottery         = 17,       //抽卡
        kNavMatch           = 18,       //杯赛
        kNavTower           = 19,       //千层塔/千机楼
        kNavActiveness      = 20,       //活跃度
        kNavChangeFormation = 21,       //布阵
        kNavSoul		    = 22,       //魂魄
		kNavActivity        = 23,       //活动
        kNavMail            = 24,       //邮件
        kNavWish            = 25,       //许愿
        kNavPrize           = 26,       //奖励页
        kNavChargePrize     = 27,       //累充奖励页（临时）
        kNavStrategy        = 28,       //掌中宝
		kNavSoulHunter      = 29,       //狩魂
		kNavGuild		    = 30,       //公会
		kNavGuildBoss		= 31,       //苍龙现
		kNavFriends			= 32,		//好友
		kNavAchievement		= 33,		//成就
		kNavSignin			= 34,		//签到
		kNavAccount			= 35,		//储值
		kNavVip	= 36,					//vip特权
		kNavBurn	= 37,	//炼化
		kNavMallLottery	= 38,	//商城进抽卡
		kNavElite           = 39, //精英赛
		kNavRank            =40,  //排行榜
		kNavGuildTomb			=41,	//魔界探险
		kNavRebirth           =42,  //重生
		kNavEquipBurn		=43,	//装备炼化
		kNavWeeklyAward		= 44,		//周活动奖励
		kNavChess			= 45,		//墨攻棋阵
		kNavMirage			= 46,		//蜃楼东渡
    };
    
    /**
     *
     *  导航的一级菜单
     *
     */
    enum NavigationCat
    {
        KNavCatRoot       = 0,   //根
        KNavCatPve        = 1,   //pve
        KNavCatPvp        = 2,   //pvp
        KNavCatChallenge  = 3,   //挑战
        KNavCatInventory  = 4,   //仓库
        KNavCatFormation  = 5,   //阵容
        KNavCatWarrior    = 6,   //弟子管理 有间客栈
        KNavCatUnion      = 7,   //帮会
    };
    
    
    
    
    
    /**
     *	@brief	导航节点只用于确定导航信息之间的关系和显示 与模块无关
     */
    class NavigationNode:public ValueObjcet

    {
        
    public:
        
        friend class ::NavigationModel;
        
        CREATE_FUNC(NavigationNode);
        
        NavigationNode()
        :m_pChildren(NULL)
        ,m_bUnlocked(false)
        ,m_nId(KNavCatRoot)
        ,m_kNavigationType(kNavNIL)
        ,m_pParent(NULL)
        ,unLockLevel(0)
        {
            
        }
        
        virtual ~NavigationNode()
        {
            CC_SAFE_RELEASE(m_pChildren);
        }
        
        virtual bool init()
        {
            if(!ValueObjcet::init()) return false;
            m_pChildren = cocos2d::CCArray::create();
            CC_SAFE_RETAIN(m_pChildren);
            return true;
        }
        
        cocos2d::CCArray* getChildren()
        {
            return m_pChildren;
        }
        
        void addChild(NavigationNode* pNode)
        {
            if(m_pChildren->indexOfObject(pNode)!=CC_INVALID_INDEX) return;
            m_pChildren->addObject(pNode);
        }
        
        bool isUnlocked();
        uint32_t getUnlockLevel();
        
    public:
        
        uint8_t m_nId;      //节点编号 用于确定节点之间的关系 
        
        
        NavigationType m_kNavigationType;   //导航与模块之间的对应关系
        NavigationNode *m_pParent; //weak   //父节点
        
    private:
        uint32_t unLockLevel;   //解锁等级
        bool m_bUnlocked;   //是否已经解锁
        cocos2d::CCArray * m_pChildren; //子级们
    };
}

class NavigationModel:public Model {
    
public:
    NavigationModel();
    virtual ~NavigationModel();
    
    virtual bool init();
    
    
    /**
     *	@brief	获取节点子级
     *
     *	@param 	id 	编号
     *
     *	@return	节点子级数组
     */
    cocos2d::CCArray* getNavigationNodeChildrenById(uint8_t id);
    
    cocos2d::CCArray* getNavigationRootChildren();
    
    /**
     *	@brief	获取节点信息
     *
     *	@param 	id 	编号
     *
     *	@return	节点信息
     */
    navigation::NavigationNode* getNavigationNodeById(uint8_t id);
    
    navigation::NavigationNode* getNavigationRoot();
    
    /**
     *	@brief	获取兄弟节点
     *
     *	@param 	id 	节点编号
     *
     *	@return	兄弟节点们 包括自己
     */
    cocos2d::CCArray* getNavigationBrothersById(uint8_t id);

    
    cocos2d::CCArray* getNavigationBrothersByNavType(navigation::NavigationType tag);
    
    navigation::NavigationNode* getNavigationNodeByNavType(navigation::NavigationType tag);

	void updateNavigationUnlockStatus();
    
    void setUnlockStatus(const std::vector<uint8_t> &ids,bool status);
    
    void setUnlockStatus(uint8_t id,bool status);
    
    
private:
    void setUnlockStatus(uint8_t id,bool status,bool notifyNavsUpdate);
    
    void loadNavLocalData();
    void parseNavData(const CSVDecoder::CSV &val);
    void addToParent(navigation::NavigationNode*  pNode,uint8_t parentId);
    navigation::NavigationNode*  getNavNode(uint8_t id);
private:
    cocos2d::CCDictionary            * m_pNavigationNodes;
};

#endif /* defined(__QSMY__NavigationModel__) */
