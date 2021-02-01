//
//  MailModel.h
//
//  邮件信息
//  Created by cll on 13-7-17.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MailModel__
#define __QSMY__MailModel__

#include "Model.h"

#define  D_EVENT_MAILLIST_UPDATE                "update mailList"       //更新邮件列表
#define  D_EVENT_MAIL_UPDATE                    "update mail"           //更新邮件信息
#define  D_EVENT_MAIL_NEW_COUNT_UPDATE          "new mail count update" //新邮件数变化
#define  D_EVENT_MAIL_NEW_COUNT_SYNC            "new mail count sync"   //同步新邮件数

namespace mail
{
    
	//邮件类型
    enum MailType
    {
        kMailTypeNone=0,
        kMailTypeNormal = 1,				//普通邮件
        kMailTypeRechargeAward = 2,			//充值返现
        kMailTypePayAward = 3,				//消费返现
        kMailTypeExchangeAward = 4,			//兑换商店
        kMailTypeTotalRechargeAward = 5,	//累计充值奖励
		kMailTypeTowerRankAward = 6,		//千机楼排名奖励
		kMailTypeMatchBetAward = 7,			//杯赛竞猜奖励
        kMailTypeFromUser = 8,              //玩家发送的留言
        kMailTypePvp = 9,                   //论剑被干掉
        kMailTypeSoulHunter = 10,            //狩魂被抢魂魄
		kMailTypeGuildJoin = 11,				//加入公会
		kMailTypeGuildExit = 12,				//被踢出公会
		kMailTypeGuildPosition = 13,			//改变职位
		kMailTypeGuildMember = 14,			//取消助理
		kMailTypeGuildBossDaily = 15,		//苍龙现日结算
		kMailTypeGuildBossWeekly = 16,		//苍龙现周结算
		kMailTypeScore = 17,					//评分奖励
		kMailTypeTeam = 18,					//副本奖励
		kMailTypeFromFriend = 19,			//好友邮件
		kMailTypeRechargeGain = 20,              //冲级奖励
		kMailTypeLimiteHero = 21,             //限时英雄
        kMailTypeWonderBoss = 22,            //奇遇boss奖励
        kMailTypeWonderRank = 23,            //奇遇排行奖励
    };

	enum MailTag
	{
		kMailTagGain = 0,       //奖励
		kMailTagEvent,          //事件
		kMailTagCount
	};

	//邮件格式  本地配置
	class Format:public ValueObjcet
	{
		public:
			Format():mail_type(kMailTypeNone),format_str("")
			{
			};
        
			virtual ~Format()
			{
			};

			CREATE_FUNC(Format);
			D_INIT_DEFAULT(ValueObjcet);

		public:
		
			MailType	mail_type;				//邮件类型
			std::string format_str;				//邮件描述段
	};
    

	//邮件信息
    class MailInfo:public ValueObjcet
    {
	public:
        MailInfo():id(0),mail_type(kMailTypeNone),title(""),content(""),att_flag(false),is_read(false),arAtt(NULL)
			,sender_id(0),m_uHeight(0.0f)
        {
        };
        
		virtual ~MailInfo()
		{
			CC_SAFE_RELEASE(arAtt);
		};

		CREATE_FUNC(MailInfo);

		bool init()
		{
            if(!ValueObjcet::init()) return false;
			arAtt = cocos2d::CCArray::create();
			CC_SAFE_RETAIN(arAtt);

			return true;
		};
        
        //判断邮件是否为新邮件
        bool isNew()
        {
            return !is_read || (checkHasAtt() && att_flag);
        }

		//是否领取附件
		bool checkAttAccepted()
		{
			return !att_flag;
		}

		//是否有附件
		bool checkHasAtt()
		{
			return arAtt->count()>0;
		};

	 public:

		 CC_SYNTHESIZE(float, m_uHeight, Height);
		uint32_t	id;						//邮件ID
        MailType	mail_type;				//邮件类型
        std::string	title;					//邮件标题
		std::string	content;				//邮件内容
        bool		att_flag;				//是否有附件	0无1有   --客户端用于判断附件是否领取
		bool		is_read;				//是否已读		0未读 1已读
		
		cocos2d::CCArray *arAtt;			//附件内容
        std::string send_user;              //发件人
		uint32_t sender_id;					//发件人ID

		uint32_t mail_ts;					//邮件时间
    };
    
}

class MailModel : public Model
{
public:
	typedef cocos2d::CCArray MailList;
	typedef cocos2d::CCDictionary MailFormatMap;

	CC_SYNTHESIZE_READONLY(uint32_t, m_iSystemNewNum, SystemNewNum); //事件
	CC_SYNTHESIZE_READONLY(uint32_t, m_iFriendNewNum, FriendNewNum);
	CC_SYNTHESIZE_READONLY(uint32_t, m_inormalAttNum, NormalAttNum); //附件

	CC_SYNTHESIZE_READONLY(MailList *, m_pSysMailList, SysMailList);
	CC_SYNTHESIZE_READONLY(MailList *, m_pUserMailList, UserMailList);
	CC_SYNTHESIZE_READONLY(MailList *, m_pAllMailList, AllMailList);
	CC_SYNTHESIZE_READONLY(MailList *, m_pMaillGain, MaillGain); //奖励
	CC_SYNTHESIZE_READONLY(MailList *, m_pMaillEvent, MaillEvent); //事件

	CC_SYNTHESIZE_READONLY(bool, m_bHasAttachment, HasAttachment);
public:
    
    MailModel();
    virtual ~MailModel();
    virtual bool init();
   
	void parseMails(const Json::Value &val);
	mail::MailInfo * parseMail(const Json::Value &val, mail::MailInfo * pMail = NULL);
    
    /**
     *	@brief	设置新邮件数 只能后台同步调用！
     *
     *	@param 	val 邮件数
     */
    void setNewMailsNum(uint32_t sys, uint32_t friendNum,uint32_t normalAtt);

	
	//获得邮件列表

	mail::MailInfo* getMailByID(uint32_t id);

	//将所有邮件设为已读
	void setAllMailsReaded();
	void setAllFriendMailsReaded();

    //设置附件已接收
    void setMailAttAccepted(mail::MailInfo* pMail);
	//一键领取
	void setAllMailAttAccepted(const Json::Value &val);
	//确认竞技场积分邮件
	void readSystemMail(mail::MailInfo* pMailInfo);

	void mailDeleted(const Json::Value &val);
	void friendMailDeleted(const Json::Value &val);

	uint32_t getNewMailsNum();
	void deleteEventMail();
private:
	void loadMailFormatData(const CSVDecoder::CSV &val);
	mail::Format * getMailFormatByType(mail::MailType mType);
	void formatMailContent(mail::MailInfo * pMail,const Json::Value& params);
	void addMailInorder(mail::MailInfo * pMail, MailList *pList);
    void getNewMails(float dt);
private:
	MailFormatMap * m_pMailFormatMap;
    time_t updateTime;
};

#endif /* defined(__QSMY__MailModel__) */
