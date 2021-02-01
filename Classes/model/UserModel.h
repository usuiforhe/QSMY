//
//  UserModel.h
//
//  Created by cll on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__UserModel__
#define __QSMY__UserModel__

#include "Model.h"
#include "DropModel.h"

#define	D_EVENT_USERINFO_UPDATE     "update useinfo"
#define D_EVENT_USER_LEVELUP        "user levelup"
#define D_EVENT_USER_VIP_LEVELUP	"user viplevelup"
#define D_EVENT_PVE_RUSH_TIME       "pve rush time"

#define D_EVENT_RESET_SCHEDULE		"reset schedule"
#define D_EVENT_GET_PRIVILEGES		"get privileges"

#define K_POWER_REFRESH_INTERVAL	(D_CONFIG_UINT_FOR_KEY(USER_POWER_REFRESH_INTERVAL))
#define K_ENDURANCE_REFRESH_INTERVAL (D_CONFIG_UINT_FOR_KEY(ENDUR_REFRESH_INTERVAL))

class UserModel;
namespace user
{
	/*首充信息*/
	class FirstPunchInfo:public ValueObjcet
	{
	public:
		FirstPunchInfo()
			:m_nIndex(0)
			,m_nType(drop::kDropTypeNone)
			,m_nID(0)
			,m_nNum(0)
		{

		}
		CREATE_FUNC(FirstPunchInfo);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::UserModel;

		CC_SYNTHESIZE_READONLY(unsigned int, m_nIndex, Index);//tolua
		CC_SYNTHESIZE_READONLY(drop::DropType, m_nType, Type);//tolua
		CC_SYNTHESIZE_READONLY(unsigned int, m_nID, ID);//tolua
		CC_SYNTHESIZE_READONLY(unsigned int, m_nNum, Num);//tolua
	};

	/*VIP描述信息*/
	class VipInfo:public ValueObjcet
	{
	public:
        VipInfo()
        :vip_lv(0)
        ,point(0)
        ,vip_desc("")
        ,rmb(0)
        ,addPowerCountMax(1)
        ,prestige_ratio(1.f)
        ,vip_item_id(0)
        ,vip_box_id(0)
        {
            
        }
		CREATE_FUNC(VipInfo);
		D_INIT_DEFAULT(ValueObjcet);
        friend class ::UserModel;
        
		CC_SYNTHESIZE_READONLY(unsigned char, vip_lv, VipLv);//vip等级
		CC_SYNTHESIZE_READONLY(unsigned int, point, Point);//累计充值点券
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, vip_desc, VipDesc);//vip描述
        CC_SYNTHESIZE_READONLY(unsigned int, rmb, Rmb);//累计充值rmb
        CC_SYNTHESIZE_READONLY(unsigned char, addPowerCountMax, AddPowerCountMax);//增加体力次数
        CC_SYNTHESIZE_READONLY(float, prestige_ratio, PrestigeRatio);//领奖需求最大声望系数
        CC_SYNTHESIZE_READONLY(unsigned int, vip_item_id, VipItemId);//对应的vip道具编号，用于vip图标显示等
        CC_SYNTHESIZE_READONLY(unsigned int, vip_box_id, VipBoxId);//对应的vip礼包商品编号，用于显示礼包图标 或 购买礼包等
		CC_SYNTHESIZE_READONLY(unsigned int, m_nPvpMax, PvpMax);//tolua
		CC_SYNTHESIZE_READONLY(unsigned int, m_nPractiseMax, PractiseMax);//tolua
		CC_SYNTHESIZE_READONLY(unsigned int, m_nFriendsMax, FriendsMax);//tolua
		CC_SYNTHESIZE_READONLY(unsigned int, m_nTeamResetMax, TeamResetMax);//tolua
	};
    
	/*本地配置基本信息*/
	class UserBase:public ValueObjcet
	{
	public:
        
        CREATE_FUNC(UserBase);
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::UserModel;
        
        CC_SYNTHESIZE_READONLY(unsigned int, userLv, UserLv);       //等级
        CC_SYNTHESIZE_READONLY(unsigned int, maxExp, MaxExp);       //当前等级对应最大经验
        CC_SYNTHESIZE_READONLY(unsigned int, totalExp, TotalExp);   //当前等级累计经验
        CC_SYNTHESIZE_READONLY(unsigned int, maxPower, MaxPower);   //最大体力
		CC_SYNTHESIZE_READONLY(unsigned int, maxEndu, MaxEndu);   //最大 耐力
	};
    
    
	/*用户升级奖励信息*/
	class UserUpAward:public ValueObjcet
	{
	public:
        
		CREATE_FUNC(UserUpAward);
		D_INIT_DEFAULT(ValueObjcet);
        friend class ::UserModel;
        
        CC_SYNTHESIZE_READONLY(unsigned int, userLv, UserLv);//等级
        CC_SYNTHESIZE_READONLY(unsigned int, points, Points);//当前等级对应最大经验
        CC_SYNTHESIZE_READONLY(unsigned int, coins,  Coins);//当前等级累计经验
        CC_SYNTHESIZE_READONLY(unsigned char, unlockPosNum, UnlockPosNum);//可上阵弟子数
	};
    
	/*用户当前信息*/
    class UserInfo:public ValueObjcet
    {
        CC_SYNTHESIZE_READONLY(unsigned char, iVipLevel, VipLevel); //VIP等级
    public:
        UserInfo()
        :iUserId(0)
        ,iUserLv(0)
        ,iUserExp(0)
        ,iPoints(0)
        ,iCoins(0)
        ,iPower(0)
        ,iEndurance(0)
		,iVipLevel(1)
        ,mid(0)
		,lastRush(0)
        ,amount(0)
        ,vip_bag(0)
        ,pow_times(0)
        ,reg_ts(0)
        ,charged(false)
        ,monthcard(false)
		,m_nUpdatePowerTs(0)
		,m_bLocked(false)
        {
            
        }
        
        friend class ::UserModel;
        
        
        bool hasPower()
        {
            return iPower>0;
        }
        
        
       
        
        void setVipLevel(unsigned char lv)
        {
            if(iVipLevel == lv) return;
            bool levelup = lv>iVipLevel;
            iVipLevel = lv;
            if(levelup)
            {
                postNotification(D_EVENT_USER_VIP_LEVELUP);
                NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_USER_VIP_LEVELUP);
            }
            postNotification(D_EVENT_USERINFO_UPDATE);
        }
        
        /**
         *	@brief	是否花过钱
         *
         *	@return
         */
        bool isPayed()
        {
            return charged;
        };
        
		//当前vip级别的礼包是否购买
		bool hasBoughtVipBag(unsigned char vipLevel)
		{
			return (vip_bag&(1<<(vipLevel-1)))>0;
		};
        
        /**
         *	@brief	获取还能买体力次数
         *
         *	@return	
         */
        uint32_t getRemainBuyPowTimes();

        
		/* 刷新用户点券信息 */
        void updatePoints(unsigned int point, float fDelayTime=0.f);
		void updateCoins(unsigned int coin, float fDelayTime=0.f);
        
        /* 用户点券信息变化 point>0 获得 point <0 消耗*/
		void changePoints(int point, float fDelayTime=0.f);
		void changeCoins(int coin, float fDelayTime=0.f);
		void addExp(unsigned int exp, float fDelayTime=0.f);
		void changePower(int power, bool bAdd=true, float fDelayTime=0.f);
		void setPower(int power, float fDelayTime=0.f);

		//竞技场打完之后减耐力
		void changeEnduranceBy(int endurance = -2);
		void getUpAward();	//获得升级奖励
        
        /* 判断等级是否到达最大值*/
        bool checkLevelLimit();
        
        /**
         * 获取扫荡cd
         */
        int getRushCD();
        
        int getRushCDCost();
        
        CREATE_FUNC(UserInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
        CC_SYNTHESIZE_READONLY(unsigned int, iUserId, UserId);              //自增ID
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, sPlatId, PlatId);   //玩家注册ID
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, sUserName, UserName);   //玩家游戏名
        CC_SYNTHESIZE_READONLY(unsigned short, iUserLv, UserLv);          //等级
        CC_SYNTHESIZE_READONLY(unsigned int, iUserExp, UserExp);        //玩家的累计经验
        CC_SYNTHESIZE_READONLY(unsigned int, iPoints, Points);          //点券
        CC_SYNTHESIZE_READONLY(unsigned int, iPower, Power);            //当前体力
		CC_SYNTHESIZE_READONLY(unsigned int, iEndurance, Endurance);     //当前耐力
        CC_SYNTHESIZE_READONLY(unsigned int, iCoins, Coins);            //游戏币
		CC_SYNTHESIZE(unsigned int, mid, Mid);                          //最新关卡编号
        CC_SYNTHESIZE_READONLY(unsigned int, lastRush, LastRush);       //最后扫荡时间
       
        
		CC_SYNTHESIZE_READONLY(unsigned int, vip_bag, VipBag);			//VIP礼包购买情况 13位的2进制数 目前VIP等级最大13        
        CC_SYNTHESIZE_READONLY(unsigned int, amount, Amount);           //已充值rmb数
        CC_SYNTHESIZE_READONLY(unsigned int, pow_times, PowTimes);      //体力已经恢复次数
        
        CC_SYNTHESIZE_READONLY(unsigned int, reg_ts, RegTs);//注册时间
        CC_SYNTHESIZE_READONLY(bool, monthcard, MonthCard); //月卡用户

		CC_SYNTHESIZE_READONLY(uint32_t, m_nUpdatePowerTs, UpdatePowerTs);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nUpdateEnduTs, UpdateEnduTs);//最后一次更新耐力值时间

		//参数表示调用是否可能会在客户端本地改变体力（耐力），倒计时用
		uint32_t getPowerRecTime( bool bChange = true );
		uint32_t getPowerFullTime();

		//参数表示调用是否可能会在客户端本地改变体力（耐力），倒计时用
		uint32_t getEnduRecTime( bool bChange = true );
		uint32_t getEnduFullTime();

		CC_SYNTHESIZE_READONLY(bool, m_bLocked, Locked);
        
		void setLocked(bool locked, float fDelay = 0.f);

    private:
        bool            charged;    //是否已充值过 0:未充值过 1:已充值过
    };

	enum PrivilegeType
	{
		PrivilegeTypeNone = 0,
		PrivilegeTypeAutoEnhance = 1 << 0,
		PrivilegeTypeRush = 1 << 1,
		PrivilegeTypeBuyExplore = 1 << 2,
		PrivilegeTypeSpeed = 1 << 3,
	};

	class Privilege : public ValueObjcet
	{
	public:
		CREATE_FUNC(Privilege);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::UserModel;
	protected:
		CC_SYNTHESIZE_READONLY(uint32_t, m_nID, ID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nEndTs, EndTs);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sContent, Content);
		CC_SYNTHESIZE_READONLY(uint32_t, m_ePrivilegeType, PrivilegeType);
		CC_SYNTHESIZE_READONLY(bool, m_bEnabled, Enabled);
	private:
	};
	
	/*副本活动信息*/
	class ExporeActivityInfo:public ValueObjcet
	{
	public:
		ExporeActivityInfo()
			:m_ExploreActivityId(-1)
			,m_StartTs(0)
			,m_EndTs(0)
			,m_AwardState(0)
			,m_ActivityMid(0)
		{

		}
		friend class ::UserModel;
		CREATE_FUNC(ExporeActivityInfo);
		D_INIT_DEFAULT(ValueObjcet);
		//{"id":267,"type":43,"startTs":1425139200,"endTs":1427212800,"exploreActivityId":1}
		CC_SYNTHESIZE_READONLY(unsigned int, m_StartTs, StartTs);
		CC_SYNTHESIZE_READONLY(unsigned int, m_EndTs, EndTs);
		CC_SYNTHESIZE_READONLY(int, m_ExploreActivityId, ExploreActivityId);
		CC_SYNTHESIZE_READONLY(int, m_AwardState, AwardState); //1领取 0未领取
		CC_SYNTHESIZE_READONLY(int, m_ActivityMid, ActivityMid); //副本活动最后挑战关卡id
	};
}

class UserModel : public Model
{
    typedef cocos2d::CCDictionary UserBaseMap;
	typedef cocos2d::CCDictionary UserUpAwardMap;
    
	CC_SYNTHESIZE(bool, m_bLogin, Login); //玩家是否已经登录
    
public:
	UserModel();
    virtual ~UserModel();
	virtual bool init();
    
    
    /**
     *	@brief	解析玩家信息
     *
     *	@param 	val 	新信息内容
     *	@param 	isLevelUp 	玩家是否升级
     */
    void parseUserInfo(const Json::Value &val ,bool* isLevelUp = NULL);
    
	user::UserInfo * getUserInfo()
	{
		return m_userInfo;
	}
    
	/* uid是否是当前user*/
    bool checkCurrentUser(unsigned int uid)
	{
		return m_userInfo->iUserId == uid;
	}
    
    /**
     *	@brief	获取用户升级信息
     *
     *	@param 	lv 	级别
     *	@return	升级信息
     */
    user::UserBase* getUserBaseByLv(unsigned int lv);
    
	//获取用户升级奖励信息
	user::UserUpAward* getUserUpAwardByLv(unsigned int lv);
    
    
    cocos2d::CCArray* getVipInfos()
    {
        return m_vipInfos;
    }

	cocos2d::CCArray* getFirstPunchInfos()
	{
		return m_firstPunchInfos;
	}
    
    unsigned char getMaxVipLevel()
    {
        return m_nMaxVipLv;
    }
    
    user::VipInfo* getVipInfoByVipLevel(unsigned char level);
    
    unsigned int getToVipLevelCost(unsigned char level);

	user::ExporeActivityInfo* getExploreActivityInfo()
	{
		return m_ExploreActivityInfo;
	}
private:
    
    void onBuyMonthCard();

    //void loadCookie();
	void loadUserUpAwardData(const CSVDecoder::CSV &val);
	void loadUserLvExpData(const CSVDecoder::CSV &val);
    void loadVipInfoData(const CSVDecoder::CSV &val);
	void loadFirstPunchInfoData(const CSVDecoder::CSV &val);
	user::UserInfo * parseUser(const Json::Value &val, user::UserInfo * pUserInfo = NULL);
    void updateUserInfo(float dt);

	void resetSchedule();
    
private:
    UserBaseMap *m_userBaseMap;				//等级经验配置信息
	UserUpAwardMap *m_userUpAwardMap;		//升级奖励信息
	user::UserInfo * m_userInfo;			//当前用户信息
    
	cocos2d::CCArray *m_vipInfos;			//vip信息列表
	cocos2d::CCArray *m_firstPunchInfos;			//首充

    uint8_t           m_nMaxVipLv;          //vip等级最大值
	user::ExporeActivityInfo *m_ExploreActivityInfo;
public:
	void parsePrivileges(const Json::Value &val);
	void loadPrivileges(const CSVDecoder::CSV &val);
	void resetPrivileges();
	void getPrivileges(cocos2d::CCArray *pArr);
	bool checkPrivilegeType(user::PrivilegeType eType);
	void refreshPrivileges(float dt);
	void parseExploreActivity(const Json::Value &val);
protected:
	CC_SYNTHESIZE_READONLY(bool, m_bPrivilegesSynced, PrivilegesSynced);
private:
	cocos2d::CCDictionary *m_pDictPrivileges;
	uint32_t m_nNearestEndTs;
};



#endif /* defined(__QSMY__UserModel__) */
