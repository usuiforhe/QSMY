//
//  Activeness.h
//  活跃度模块
//
//  Created by cll on 13-6-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Activeness__
#define __QSMY__Activeness__

#include "AppInclude.h"
#include "components/components.h"
#include "model/ActivenessModel.h"
#include "ActivenessAwardView.h"
#include "../Game.h"

using namespace activeness;

#define ACTIVENESS_PROGRESS_NUM 4
#define ACTIVENESS_PROGRESS_STATE 2

struct ActivenessDelegate
{
    virtual void showAwardView(activeness::AwardInfo *pAwardInfo) = 0;
	virtual void closeAwardView() = 0;
};


class ActivenessTableView;
class ActivenessAwardView;
class ActivenessBox;

class Activeness
:public DLayer
,public ActivenessDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public BackKeyInterface
{
public:
    Activeness();
    ~Activeness();
    
    CREATE_FUNC(Activeness);
    bool init();
    
    void onEnter();
    void onExit();
    
    void updateView();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){ return NULL;};

	
private:
    void closeCallBack(cocos2d::CCObject * sender);
    void getRewardCallback(cocos2d::CCObject * sender);
    
    virtual void showAwardView(activeness::AwardInfo *pAwardInfo);
private:
    cocos2d::CCNode  * m_pContainer;
    ActivenessTableView * m_pActivenessTableView;
    
    cocos2d::CCArray * m_pBoxes;
    cocos2d::CCLabelBMFont * m_pPoint;
    
	cocos2d::CCArray * m_pAwards;

	int m_nIndex;

	cocos2d::CCMenuItem    *m_pButton[ACTIVENESS_PROGRESS_NUM];
	cocos2d::CCLabelBMFont *m_pLevelPoint[ACTIVENESS_PROGRESS_NUM];
	cocos2d::CCLabelBMFont *m_pTextLeft;
    cocos2d::CCNode        *m_pCanGet[ACTIVENESS_PROGRESS_NUM];
    cocos2d::CCNode *m_pProgressDotContainer[ACTIVENESS_PROGRESS_NUM];
    DButton                *m_pDBtnClose;
    cocos2d::CCSprite      *m_pProgressState[ACTIVENESS_PROGRESS_STATE];
    cocos2d::CCNode        *m_pBoxAvailable[ACTIVENESS_PROGRESS_NUM];
	sp::Armature		   *m_pCanGetAni[ACTIVENESS_PROGRESS_NUM];
	sp::Armature		   *m_pProgressAni[ACTIVENESS_PROGRESS_NUM];
	sp::Armature		   *m_pBoxAvailableAni[ACTIVENESS_PROGRESS_NUM];
    cocos2d::CCNode        *m_pRewardGetted[ACTIVENESS_PROGRESS_NUM];

	void pageCallBack(cocos2d::CCObject * sender);
	void prevPageCallBack(cocos2d::CCObject * sender);
	void nextPageCallBack(cocos2d::CCObject * sender);

	void updatePage();

	DSlideView   *m_pSlideView;
	cocos2d::CCNode * m_pBoxContainer;

	ActivenessAwardView *m_pAwardView;
    ActivenessModel     *m_pActivenessModel;
	virtual void closeAwardView();
public:
	virtual void onBackKey();
};

#endif /* defined(__QSMY__Activeness__) */
