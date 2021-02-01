//
//  SoulHunterPageView.h
//  狩魂一页6个弟子的视图
//
//  Created by liuxiaogang on 13-10-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoulHunterPageView__
#define __QSMY__SoulHunterPageView__

#include "AppInclude.h"
#include "AppMacros.h"
#include "components/components.h"
#include "model/SoulHunterModel.h"
#include "SoulHunter.h"
#include "SoulHunterView.h"

#define HEAD_COUNT 6

class SoulHunterWarriorView;
class SoulHunterDelegate;

class SoulHunterPageView
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{   
    CC_SYNTHESIZE(SoulHunterDelegate*, m_pDelegate, Delegate);
public:
	static SoulHunterPageView* create();
    SoulHunterPageView();
    virtual ~SoulHunterPageView();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setData(cocos2d::CCArray*);


	void showItemPage(const uint32_t did);
	void setCurrentPage(const uint32_t page);
    
private:
    void SnatchCallBack(cocos2d::CCObject* sender);
private:
    SoulHunterWarriorView   *m_pSoulHunterWarriorViews[HEAD_COUNT];
    
    cocos2d::CCArray    *m_pData;
};

#endif /* defined(__QSMY__SoulHunterPageView__) */
