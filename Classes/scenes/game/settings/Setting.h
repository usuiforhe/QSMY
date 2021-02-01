//
//  Setting.h
//  设置页面
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Setting__
#define __QSMY__Setting__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"


#include "SoundSetting.h"
#include "CustomerService.h"
#include "Guide.h"
#include "UserFeedback.h"

class Setting
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    //枚举不要乱改 跟ui编辑器对应的 每个都设好对应的值
    enum SettingNav
    {
        kGuide            = 0,
        kUserCenter         = 1,
        kFourm              = 2,
        kSettingSound    = 3,
        kUserFeedback       = 4,
		kCustomerService       = 5,
		kSettingCount		= 6,
		kAnnounce           = 7,
    };
    
public:
    
    Setting();
    virtual ~Setting();
    static Setting* create(NavigationDelegate* pDelegate);
    virtual bool init(NavigationDelegate* pDelegate);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
private:
    void closeCallBack(cocos2d::CCObject*);
    void navCallBack(cocos2d::CCObject*);
    void viewCallBack(cocos2d::CCObject*);
    void NavTo(SettingNav nav);
    void viewNav(SettingNav nav);
    CCNode* getView(SettingNav nav);
private:
    NavigationDelegate      *m_pDelegate;
    cocos2d::CCDictionary   *m_pNavs;
    cocos2d::CCMenuItem     *m_pSelected;
    cocos2d::CCNode         *m_pContainer;
	DButton							*m_pCloseButton;
private:
    cocos2d::CCMenu         *m_pMenu;
	CCSprite							*m_pCurrentTitle;
    cocos2d::CCMenuItemImage        *m_pCdkeyButton;
    cocos2d::CCSprite               *m_pCdKeyText;
    cocos2d::CCMenuItemImage        *m_pAccountButton;
    cocos2d::CCSprite               *m_pAccountText;
    
	std::map<SettingNav, CCSprite*> m_titleMap;
	void layout();
};

#endif /* defined(__QSMY__Setting__) */
