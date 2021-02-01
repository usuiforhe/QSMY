//
//  HeroShowView.h
//  首页上场单位展示
//
//  Created by wanghejun on 13-7-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__HeroShowView__
#define __QSMY__HeroShowView__

#include "AppInclude.h"
#include "../navigation/Navigation.h"
#include "managers/DataManager.h"
#include "../common/common.h"
#include "DragonBone.h"

class HeroShowView
:public DLayer
,public DPageViewDataSource
,public cocos2d::extension::CCScrollViewDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE(NavigationDelegate*, m_pDelegate, Delegate);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCNode*, m_pDialogEffect, DialogEffect);
public:
    
    CREATE_FUNC(HeroShowView);
    bool init();
    
    HeroShowView();
    virtual ~HeroShowView();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual float cellSizeForTable(DPageView *view);
    
    virtual cocos2d::CCNode* cellAtIndex(DPageView *view, unsigned int idx);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
    
    virtual uint32_t numberOfCellsInTableView(DPageView *view);
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
	void hideHeroTalk();
private:
    
    void updateInfo();
    void onFormationUpdate();
    void viewCallback(cocos2d::CCObject *sender);
    
private:
    FormationModel                  *m_pFormationModel;
    DPageViewLoop                   *m_pDPageViewLoop;
    cocos2d::CCArray                *m_pItems;
    
    cocos2d::CCLabelBMFont                   *m_pLevel;
    cocos2d::CCLabelTTF                      *m_pName;
    cocos2d::CCSprite                        *m_pGrade;
    
    WarriorTypeMark                          *m_pWarriorTypeMark;
    
    uint16_t                        m_nCurrentPage;
    
    sp::Armature                    *m_pDialog;

	cocos2d::CCNode                 *m_pFormationContainer;

	cocos2d::CCLabelTTF * m_pLabelTalk;

	bool		m_bTalking;
};




#endif /* defined(__QSMY__HeroShowView__) */
