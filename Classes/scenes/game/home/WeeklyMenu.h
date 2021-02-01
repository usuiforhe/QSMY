//
//  WeeklyMenu.h
//  周活动按钮
//
//  Created by wanghejun on 13-7-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WeeklyMenu__
#define __QSMY__WeeklyMenu__

#include "AppInclude.h"
#include "../navigation/Navigation.h"
#include "../../../managers/DataManager.h"

class WeeklyMenu : public cocos2d::CCMenu
{
    
    CC_SYNTHESIZE(NavigationDelegate*, m_pDelegate, Delegate);
    
public:
    WeeklyMenu();
    virtual ~WeeklyMenu();
    static WeeklyMenu* create();
    virtual bool init();
    
public:
    virtual void addChild(cocos2d::CCNode * child);
    virtual void addChild(cocos2d::CCNode * child, int zOrder);
    virtual void addChild(cocos2d::CCNode * child, int zOrder, int tag);
    
    //virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    //virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    void onEnter();
    void onExit();

public:
    void menuCallBack(cocos2d::CCObject*);

	bool isShowing() {return m_bShowing;};

private:
    void updateNavNodes();
    void hide();
    
private:
    bool                        m_bShowing;
    cocos2d::CCSprite           *m_pBg;
};

#endif /* defined(__QSMY__WeeklyMenu__) */
