//
//  PlatformModel.h
//  QSMY
//
//  Created by wanghejun on 13-9-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PlatformModel__
#define __QSMY__PlatformModel__

#include "Model.h"

#define D_EVENT_PLATFORM_SERVER_CHANGED "on select server"
#define D_EVENT_PLATFORM_ON_GET_SERVER_LIST "on get server list"
#define D_EVENT_PLATFORM_ON_GET_MY_SERVER_LIST "on get my server list"

#define D_EVENT_LOGINED_USER_CHANGED "on logined user"

class PlatformModel;
namespace platform
{
    enum ServerStatus
    {
        kServerStatusNone = 0,  //无
        kServerStatusFire,  //火爆
        kServerStatusNew,   //新区
		kServerStatusMaintain   //维护
    };
    
    class PlatformConfig
    {
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_user, User);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_iap, IAP);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_order, OrderMethod);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_platform, Platform);
        //CC_SYNTHESIZE_READONLY(uint32_t, m_uPlatformId, PlatformId);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_fourmURL, FourmURL);
        
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_versionURL, VersionURL);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_checkVersionURL, CheckVersionURL);
        //CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_maintainUrl, MaintainUrl);
        //CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_loginSetTokken, LoginSetTokken);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_syzzb, SyzzbURL);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_ipaURL, IPAURL);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_channelId, ChannelId);
		CC_SYNTHESIZE_READONLY(bool, m_bHideService, IsHideService);
		CC_SYNTHESIZE_READONLY(int, m_iSettingConfig, SettingConfig);
		CC_SYNTHESIZE_READONLY(bool, m_bStrategyHidden, StrategyHidden);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sServerPlatformId, ServerPlatformId);
        
    
        std::string m_serverURL;
        std::string m_announceURL;
        std::string m_platformURL;
		std::string m_pAuthUrl;

        friend class ::PlatformModel;
    };
    
    class ServerInfo:public ValueObjcet
    {
        CC_SYNTHESIZE_READONLY(uint32_t, m_nId, ID);//用来拼接域名
		CC_SYNTHESIZE_READONLY(uint32_t, m_nSId, SID);//用来客户端显示
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_Name, Name);
        CC_SYNTHESIZE_READONLY(ServerStatus, m_Status, Status);
        
    public:
        ServerInfo()
        :m_nId(0)
        ,m_Name("")
        ,m_Status(kServerStatusNone)
        {
            
        }
        CREATE_FUNC(ServerInfo);
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::PlatformModel;
    };
    
    class RechargeInfo:public ValueObjcet
    {
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sIdentifier, Identifier);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_nPrice, Price);
        CC_SYNTHESIZE_READONLY(uint32_t, m_nPoint, Point);
        CC_SYNTHESIZE_READONLY(float, m_nExtRate, ExtRate);
        CC_SYNTHESIZE_READONLY(uint32_t, m_nMonthCardId, MonthCardId);
        CC_SYNTHESIZE_READONLY(uint32_t, m_nResId, ResId);
        //add 用户充值界面显示 买多少 和 送多少
		CC_SYNTHESIZE_READONLY(uint32_t, m_nShowPoint, ShowPoint);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nShowExt, ShowExt);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nShowExtFirst, ShowExtFirst);
        CC_SYNTHESIZE_READONLY(std::string, m_des, Des);
		//新增月卡相关
		CC_SYNTHESIZE_READONLY(uint32_t, m_nMCPointFirst, MCPointFirst);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nMCPoint, MCPoint);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nMCTimes, MCTimes);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nMCDay, MCDay);
    public:
        RechargeInfo()
        :m_nPrice("")
        ,m_des("")
        ,m_nPoint(0)
        ,m_nExtRate(0.0f)
        ,m_nMonthCardId(0)
        ,m_nResId(0)
		,m_nShowPoint(0)
		,m_nShowExt(0)
		,m_nShowExtFirst(0)
		,m_nMCPointFirst(0)
		,m_nMCPoint(0)
		,m_nMCTimes(0)
		,m_nMCDay(0)
        {
            
        }
        
        std::string getRechargeName()
        {
            if(isMonthCard())
            {
                return D_LOCAL_STRING("monthCardDeposit%s%d%d",this->getPrice().c_str(),this->getPoint(),(int)getExtRate());
            }
            if(getExtRate()>0)
            {
                return D_LOCAL_STRING("deposit%s%d%d",this->getPrice().c_str(),this->getPoint(),(int)getExtRate());
            }
            return D_LOCAL_STRING("deposit%s%d",this->getPrice().c_str(),this->getPoint());
        }
        
        CREATE_FUNC(RechargeInfo);
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::PlatformModel;
        
        /**
         *	@brief	判定是否是月卡
         *
         *	@return	是否是月卡
         */
        bool isMonthCard()

        {
            return m_nMonthCardId>0;
        }

    };

	class UserLoginInfo : public ValueObjcet
	{
	public:
		CREATE_FUNC(UserLoginInfo);
		D_INIT_DEFAULT(ValueObjcet);
		CC_SYNTHESIZE(int, m_nLoginType, LogType);
		CC_SYNTHESIZE(std::string, m_pLoginName, LoginName);
		CC_SYNTHESIZE(std::string, m_pPassword, Password);
		CC_SYNTHESIZE(std::string, m_pEmail, Email);
        CC_SYNTHESIZE(std::string, m_facebookName, FacebookName);
		CC_SYNTHESIZE(std::string, m_sUniqueId, UniqueId);
		CC_SYNTHESIZE(std::string, m_sPlatId, PlatId);

		UserLoginInfo()
			:m_nLoginType(0),
			m_pLoginName(""),
			m_pPassword(""),
			m_pEmail(""),
			m_facebookName(""),
			m_sUniqueId(""),
			m_sPlatId("")
		{

		}


		friend class ::PlatformModel;
	};
}

class PlatformModel :public Model
{
	typedef std::map<std::string, std::string> DeveloperInfo;
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pServerList, ServerList);
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pMyServerList, MyServerList);
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pRechargeList, RechargeList);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(platform::PlatformConfig, m_PlatformConfig, Config);
    CC_SYNTHESIZE(platform::RechargeInfo*, m_pRechargeInfo, RechargeInfo);
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(DeveloperInfo, m_mParameter, Parameter);
	CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pDictForAnalytics, DictForAnalytics);
	CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pDictForAds, DictForAds);
	CC_SYNTHESIZE(cocos2d::CCArray *, m_pLoginedUsers, LoginedUsers);//客户端保存的已经登录用户名

public:
    PlatformModel();
    virtual ~PlatformModel();
    virtual bool init();
    
    platform::ServerInfo* getCurrentServer();
    void setCurrentServer(platform::ServerInfo*);
    platform::ServerInfo* getServerInfoById(uint32_t id);

    void getChargeOrder();
    
    /**
     *	@brief	同步服务器列表
     *
     */
	void syncServerList();
    
    /**
     *	@brief	同步我最近服务器列表
     *
     */
    void syncMyServerList();
    
    /**
     *	@brief	保存当前服务器到列表
     *
     */
    void saveMyCurrentServer();
    

	bool getSettingVisibleByIndex(int idx);

	void saveLoginedUser(const char* userName, const char* password, int iLoginType, const char* email=NULL, const char* facebookUserName = NULL, const char* uniqueId = NULL, const char* platId=NULL);
	platform::UserLoginInfo* getLoginedUserByName(const char* platId, int iLoginType);
	void removeLoginedUser(platform::UserLoginInfo*);
	void removeLoginedUser(const char* userName, int iLoginType, bool triggerNofi=true);
	void saveToClient();
#pragma mark - 平台配置 -
public:
    std::string getServerURL();
    std::string getAnnounceURL();
    std::string getPlatformURL();
    std::string getMaintainURL();
	std::string getAuthURL();
    
private:

	void loadLoginedUsers(const std::string);
	
    void buy(uint32_t billno,const std::string &token);
    
    void onSaveMyCurrentServer(const Json::Value &requestData,const Json::Value &responseData);
    void onGetChargeOrder(const Json::Value &requestData, const Json::Value &responseData);
    void loadData();
    void parseConfig(const CSVDecoder::CSV &val);
    
    void parseServerList(const Json::Value &requestData,const Json::Value &responseData);
    void parseMyServerList(const Json::Value &requestData,const Json::Value &responseData);
    
    void parseRechargeList(const CSVDecoder::CSV &val);

	void parseDeveloperInfo(const CSVDecoder::CSV &val, DeveloperInfo &mmap);

	void loadAnalyticsData(const CSVDecoder::CSV &val);

	void loadAdsData(const CSVDecoder::CSV &val);

    
private:
    bool isMyServerListSynced;

    
};

#endif /* defined(__QSMY__PlatformModel__) */
