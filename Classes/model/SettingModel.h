//
//  SettingModel.h
//  QSMY
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SettingModel__
#define __QSMY__SettingModel__

#include "Model.h"

#define D_EVENT_SETTING_MUSIC_STATUS_UPDATED "music status"
#define D_EVENT_SETTING_SFX_STATUS_UPDATED "sfx status"

class SettingModel;

namespace settings
{
    
    class GuideInfo : public ValueObjcet
    {
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_Info, Info);
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_Title, Title);

    public:
        CREATE_FUNC(GuideInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
        friend class ::SettingModel;
    };
}

class SettingModel:public Model
{
    CC_SYNTHESIZE_READONLY(bool, m_pSfxStatus, SfxStatus);    //音效状态
    CC_SYNTHESIZE_READONLY(bool, m_pMusicStatus, MusicStatus);   //音乐状态
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pGuides, Guides);
    CC_SYNTHESIZE(bool, m_enableCDKey, EnableCDKey);
    
public:
    SettingModel();
    virtual ~SettingModel();
    virtual bool init();
    
    void setSfxStatus(bool val);
    void setMusicStatus(bool val);
    
private:
    void loadData();
    void parseGuideList(const CSVDecoder::CSV &val);
    
private:
    cocos2d::CCUserDefault* m_pCCUserDefault; //weak
};

#endif /* defined(__QSMY__SettingModel__) */
