//
//  AnnounceModel.h
//  公告
//
//  Created by 汪燕军 on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__AnnounceModel__
#define __QSMY__AnnounceModel__

#include "Model.h"
#include "NavigationModel.h"


class AnnounceModel;

namespace announce {    
    /**
     *	@brief	公告信息
     */
    class AnnounceInfo : public ValueObjcet
    {
		friend class ::AnnounceModel;
    public:
        AnnounceInfo();
        virtual ~AnnounceInfo();
        
        CREATE_FUNC(AnnounceInfo);
        
        D_INIT_DEFAULT(ValueObjcet);
        
        /**
         *	@brief	解析数据
         *
         *	@param 	data 	数据
         */
        void parse(const Json::Value & data);
        
        
        bool hasPic();
        
        cocos2d::CCNode* getPic();
        
        cocos2d::CCSize getPicSize();
        
        /**
         *	@brief	是否需要导航
         *
         *	@return	是否需要导航
         */
        bool isNeedNav();
        
    private:
        
        
        /**
         *	@brief	标题
         */
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sTitle, Title);
        
        /**
         *	@brief	内容
         */
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sContent, Content);

		CC_SYNTHESIZE_READONLY(uint32_t, m_nMinDays, MinDays);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nMaxDays, MaxDays);
        
        /**
         *	@brief	导航类型
         */
        CC_SYNTHESIZE_READONLY(navigation::NavigationType, m_kNavType, NavType);
        
        /**
         *	@brief	导航标签
         */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nNavTag, NavTag);

		/**
         *	@brief	公告类型 1是维护公告，0普通公告
         */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nAnnounceType, AnnounceType);
        
        
        /**
         *	@brief	导航地址
         */
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sUrl, Url);
        
        
        /**
         *	@brief	图片路径
         *
         */
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sImg, Img);

		bool m_bNetworkImg;
    };
}

class AnnounceModel : public Model
{
public:
    AnnounceModel();
    
    virtual ~AnnounceModel();
    
    virtual bool init();
    
    /**
     *	@brief	解析公告列表
     *
     *	@param 	responseData 	响应数据
     */
    void parseAnnounces(const Json::Value & responseData);
    bool isLoginShow;
private:
    /**
     *	@brief	公告列表
     */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pAnnounceList, AnnounceList);  //登录前显示
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pAnnounceLocalList, AnnounceLocalList);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pAnnounceNewList, AnnounceNewList); //游戏内公告

	void parseAnnounceData(const CSVDecoder::CSV& val);
	void addLocalAnnounceList();
};

#endif /* defined(__QSMY__AnnounceModel__) */
