//
//  HomeMenu.h
//  首页导航
//
//  Created by wanghejun on 13-7-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__HomeMenu__
#define __QSMY__HomeMenu__

#include "AppInclude.h"
#include "../navigation/Navigation.h"
#include "../../../managers/DataManager.h"

class HomeMenu : public cocos2d::CCMenu
{
    
    CC_SYNTHESIZE(NavigationDelegate*, m_pDelegate, Delegate);
    
public:
    HomeMenu();
    virtual ~HomeMenu();
    static HomeMenu* create(navigation::NavigationNode*);
    virtual bool init(navigation::NavigationNode*);
    
public:
    virtual void addChild(cocos2d::CCNode * child);
    virtual void addChild(cocos2d::CCNode * child, int zOrder);
    virtual void addChild(cocos2d::CCNode * child, int zOrder, int tag);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    void setToggleCallBack(cocos2d::CCObject * target, cocos2d::SEL_CallFuncO selector);
    
    void onEnter();
    void onExit();

public:
    void toggleMenu(cocos2d::CCObject*);
    void menuCallBack(cocos2d::CCObject*);

	bool isShowing() {return m_bShowing;};

private:
    void updateNavNodes();
    void updateView(bool animate);
    void hide();
    
private:
    navigation::NavigationNode  *m_pNode;
    bool                        m_bShowing;
    cocos2d::CCSprite           *m_pBg;
    cocos2d::CCArray            *m_pItems;
    cocos2d::CCObject           *m_pToggleCallBackTarget;
    cocos2d::SEL_CallFuncO      m_pToggleCallBackSelector;
};

#endif /* defined(__QSMY__HomeMenu__) */
