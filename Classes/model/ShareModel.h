//
//  ShareModel.h
//  分享信息
//
//  Created by heven on 14-4-29.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__ShareModel__
#define __QSMY__ShareModel__

#include "Model.h"

#define  D_EVENT_SHARE_TIMES_UPDATE "share times updated"		// 分享次数变化

class ShareModel;
namespace share
{
    class ShareInfo : public ValueObjcet
	{
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sKey, Key);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sAppKey, AppKey);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sIconPath, IconPath);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sShareUrl, ShareUrl);
	public:
		ShareInfo() {}
        
		CREATE_FUNC(ShareInfo);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::ShareModel;
	};
}

class ShareModel:public Model {
    
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pArrayForShare, ArrayForShare);
    CC_PROPERTY(uint32_t, m_nShareTimes, ShareTimes);
    
public:
    ShareModel();
    ~ShareModel();
    virtual bool init();
    
    share::ShareInfo* getShareInfoByKey(const char* key);
    
    /**
     *	@brief	是否分享能够领奖
     *
     */
    bool isCanReward();
    
private:
    void loadData();
    void loadShareData(const CSVDecoder::CSV &val);
    
};



#endif /* defined(__QSMY__ShareModel__) */
