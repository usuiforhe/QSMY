//
//  Navigation.h
//  主导航菜单
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Navigation__
#define __QSMY__Navigation__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../managers/DataManager.h"



class NavigationDelegate;

class Navigation
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
public:
    Navigation();
    virtual ~Navigation();
    virtual bool init();
   
    CREATE_FUNC(Navigation);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    NavigationDelegate* getDelegate() { return m_pDelegate; }
    void setDelegate(NavigationDelegate* pDelegate) { m_pDelegate = pDelegate; }
    void setNavigateDisplay(navigation::NavigationType value);
    
    virtual void setVisible(bool b);
protected:
    void menuCallBack(CCObject *sender);
	void updateNav();
private:
    NavigationDelegate *m_pDelegate;
    NavigationModel    *m_pNavigationModel;
    CCNode             *m_pContainer;
    navigation::NavigationType m_kNav;

	sp::Armature		*m_pArmature;
};

#endif /* defined(__QSMY__Navigation__) */
