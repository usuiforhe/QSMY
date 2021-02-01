//
//  NoticeModel.h
//  QSMY
//
//  Created by 汪燕军 on 13-9-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__NoticeModel__
#define __QSMY__NoticeModel__

#include "Model.h"

class NoticeModel;

namespace notice
{
    class TipsInfo:public ValueObjcet
    {
    public:
        TipsInfo()
        : ValueObjcet()
        , m_pDialogList(NULL)
        {
            
        };
        
        virtual ~TipsInfo()
        {
            CC_SAFE_RELEASE(m_pDialogList);
        };
        
        CREATE_FUNC(TipsInfo);
        
        virtual bool init()
        {
            m_pDialogList = cocos2d::CCArray::create();
            m_pDialogList->retain();
            return ValueObjcet::init();
        }
        
        cocos2d::CCString * getDialog();
        
        void addDialog(cocos2d::CCString * m_pDialog)
        {
            m_pDialogList->addObject(m_pDialog);
        }
                
    private:
        cocos2d::CCArray * m_pDialogList;
    };
    
    enum NoticeListType
    {
        kNoticeListNone         = 0,        //无
        kNoticeListNormal       = 1,        //普通队列
        kNoticeListExchangeMall = 2,        //兑换商店队列
        kNoticeListMammon       = 3         //迎财神队列
    };
    
    enum NoticeType
    {
        kNoticeNone                 = 0,    //无类型
        kNoticeSkillEnhance         = 1,    //技能强化
        kNoticeWarriorUp            = 2,    //弟子突破
        kNoticeEquipEnhance         = 3,    //装备强化
        kNoticeOpenBox              = 4,    //开宝箱
        kNoticeDrawCard             = 5,    //抽卡
        kNoticeSignUp               = 6,    //签到奖励
        kNoticeWarriorRecruit       = 7,    //魂魄招募
        kNoticeAchieveDestiny       = 8,    //达成天命
        kNoticeGetPrestigeReward    = 9,    //领取声望奖励
        kNoticeMammon               = 10,   //迎财神
        kNoticeExchange             = 11    //兑换商店兑换
    };
    
    class NoticeTemplateInfo:public ValueObjcet
    {
    public:
        NoticeTemplateInfo()
        : ValueObjcet()
        , m_kNoticeType(kNoticeNone)
        , m_sTextTemplate("")
        {
            
        }
        
        virtual ~NoticeTemplateInfo()
        {
            
        }
        
        CREATE_FUNC(NoticeTemplateInfo);
        
        D_INIT_DEFAULT(ValueObjcet);
        
        void loadLine(const CSVDecoder::CSVLine &line);
        
        cocos2d::CCString * formatContent(const char * playerName, const Json::Value & data);
        
    private:
        const char * getDropName(const Json::Value & drop);
        
    private:
        CC_SYNTHESIZE_READONLY(NoticeType, m_kNoticeType, NoticeType);
		CC_SYNTHESIZE_READONLY(bool, m_isGlobal, IsGlobal);
        std::string m_sTextTemplate;
    };
    
    class NoticeInfo:public ValueObjcet
    {
    public:
        NoticeInfo()
        :m_pText(NULL)
        {
            
        }
        
        virtual ~NoticeInfo()
        {
            CC_SAFE_RELEASE(m_pText);
        }
        
        CREATE_FUNC(NoticeInfo);
        
        D_INIT_DEFAULT(ValueObjcet);
        
        void parse(NoticeModel * pNoticeModel,const Json::Value & data, bool exceptSelf = true);
        
    private:
        CC_SYNTHESIZE_READONLY(NoticeType, m_kNoticeType, NoticeType);
        CC_SYNTHESIZE_READONLY(cocos2d::CCString * , m_pText, Text);
    };
}

class NoticeModel:public Model
{
public:    
    CREATE_FUNC(NoticeModel);
    
    NoticeModel();
    
    virtual ~NoticeModel();
    
    virtual bool init();
    
    void parseNotice(notice::NoticeListType listType, const Json::Value & responseData);
    
    void parseNotices(notice::NoticeListType listType, const Json::Value & responseData);
    
    notice::NoticeTemplateInfo * getTemplate(notice::NoticeType type);
    
    cocos2d::CCString * getNextNoticeText(notice::NoticeListType listType);
    
    
private:
	void parseTipsInfoData(const CSVDecoder::CSV &val);
    void parseNoticeTemplateData(const CSVDecoder::CSV &val);
    
    void syncFromServer();
private:
    CC_SYNTHESIZE_READONLY(notice::TipsInfo *, m_pTipsInfo, TipsInfo);
    cocos2d::CCDictionary * m_pNoticeTemplates;
    cocos2d::CCDictionary * m_pNoticeMap;
    cocos2d::CCDictionary * m_pSyncTimeMap;
    
    uint32_t m_nLastSyncTs;
};

#endif /* defined(__QSMY__NoticeModel__) */
