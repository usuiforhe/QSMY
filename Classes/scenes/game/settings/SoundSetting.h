//
//  SoundSetting.h
//  声音设置
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoundSetting__
#define __QSMY__SoundSetting__

#include "AppInclude.h"
#include "components/components.h"
#include "model/SettingModel.h"

class SoundSetting
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
    //枚举不要乱改 跟ui编辑器对应的 每个都设好对应的值
    enum
    {
        kMusic              = 0,
        kSfx                = 1,
    };
    
public:
    SoundSetting();
    virtual ~SoundSetting();
    CREATE_FUNC(SoundSetting);
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    virtual void onEnter();
    virtual void onExit();
private:
    SettingModel* m_pSettingModel;
	cocos2d::CCNode* m_pMusicButtonContainer;
	cocos2d::CCNode* m_pSfxButtonContainer;
private:
    void updateMusicStatus(CCObject* sender, CCControlEvent controlEvent);
    void updateSfxStatus(CCObject* sender, CCControlEvent controlEvent);

    void menuCallBack(cocos2d::CCObject*);
private:
};

#endif /* defined(__QSMY__SoundSetting__) */
