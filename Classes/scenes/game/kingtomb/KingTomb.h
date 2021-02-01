//
//  KingTomb.h
//  LibQSMY
//  皇陵宝藏
//  Created by jk on 14-8-1.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#ifndef __LibQSMY__KingTomb__
#define __LibQSMY__KingTomb__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "model/KingTombModel.h"
#include "managers/GameManager.h"
#include "../common/gui/ItemBox.h"
#include "KingTombRule.h"

#include "KingTombfight.h"
#include "KingTombGuildList.h"


class KingTombHistory;

using namespace cocos2d;


#define TOMBS_NUM_UNIT	6

class GuildKingTombDelegate
{
public:
	//用于探险界面 弹框显示在 公会系统顶层（底部导行以上）
	virtual void openTopPopup(cocos2d::CCLayer*) = 0;
};

class KingTomb
: public DLayer
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCTableViewDelegate
, public KingTombDelegate
,public UIAnimateDelegate

{
    
public:
	KingTomb();
	~KingTomb();
	CREATE_FUNC( KingTomb ) ;
	virtual bool init() ;
    virtual void onEnter();
	virtual void onExit() ;
    
    
public:
    
    virtual bool onAssignCCBMemberVariable( cocos2d::CCObject *pTarget,
                                           const char *pMemberVariableName, cocos2d::CCNode *pNode ) ;
    virtual void openFightPanel();
    virtual void openGuildListPanel(uint32_t index);
    
#pragma mark CCTableViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){}
    //    virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);
    
#pragma mark CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {return NULL;};
    
#pragma mark UIAnimateDelegate
	virtual void animationComplete(UIAnimate *pAnimate);

	//add 动画播放完成
	void animationOver();
	//触摸
	void hideCallBack(CCObject*);

	CC_SYNTHESIZE(GuildKingTombDelegate *, m_pDelegate, Delegate);	
private:

	CCMenuItem*		m_pMenuHide;
    DButton * m_pQiangButton;
    DButton * m_pSearchButtonFirst;
    DButton * m_pSearchButton;
    DButton *m_pCompleteBtn;
    DButton *m_pSpeedUpBtn;
    DButton *m_pRuleBtn;
	DButton *m_pHistoryBtn;
	DButton	*m_pRankBtn;

    CCLabelBMFont *m_pSpeedNumBmTf;
    CCLabelBMFont *m_pGetNumBmTf;
    CCLabelBMFont *m_pVipBmTf;
    CCLabelBMFont *m_pYuanBMTf;
    CCLabelBMFont *m_pNumBMTf1;
    CCLabelBMFont *m_pNumBMTf;
    CCLabelBMFont *m_pTimeBmTf;
    CCRichLabelTTF *m_pAwardName;
    CCLabelBMFont *m_pYuanBMFindTf2;
    CCLabelBMFont *m_pYuanBMFindTf1;
    CCLabelBMFont *m_pItemNum;    
    CCLabelBMFont *m_pItemSpeed;
    
	CCNode*		m_pBoxNodeLeft;
	CCNode*		m_pBoxNodeRight;
    CCNode* m_pVip;
    CCNode *m_BtnStateFriest;
    CCNode *m_BtnState;
    CCNode *m_pContainer;
    CCNode* m_pSpeedUp;
    CCNode* m_pFisish;
    CCNode* p_pOutPut;
    CCNode* m_pInfos;
    CCNode* m_pFindYuan2;
    CCNode* m_pFindYuan1;
    
    CCNode * m_punOpenTip;
    CCNode * m_pDoneTip;
    CCNode *m_pKingTombBack;
	CCNode *m_pKingTombFore;
    CCNode * m_pIsOkImg;
    CCNode * m_pSnatch;
    CCNode * m_pFont_free1;
    CCNode * m_pFont_free2;
    CCNode * m_pSpeedYuan;
    CCNode * m_pBtnBG ;
    CCNode * m_fightItemImg;
    //CCNode *m_pD1;
    //CCNode *m_pD2;
    //CCNode *m_pD3;
    //CCNode *m_pD4;
    //CCNode *m_pD5;
    //CCNode *m_pD6;

	CCMenuItemImage*	m_pD[TOMBS_NUM_UNIT];

    CCNode *m_pReward;
    CCNode *m_pItemNone;
    
    ItemBox *m_pItemIconLeft;
    ItemBox *m_pItemIcon;
    UIAnimate*     m_pAnimate;
    UIAnimate *m_pSuccessAim;
    UIAnimate *m_pDefeatAim;
    
    
    void updateView();
    
    void updata(float dt);
    
    DSlideView   *m_pSlideView;
    KingTombModel *m_pmodel;
    cocos2d::CCArray * m_pTombs;
    
    void pageCallBack(cocos2d::CCObject *sender);
    void historyBtnCallBack(cocos2d::CCObject *sender);
	void rankBtnCallBack(cocos2d::CCObject *sender);
    void ruleBtnCallBack(cocos2d::CCObject *sender);
    void getRewordBtnCallBack(cocos2d::CCObject *sender);
    void speedUpBtnCallBack(cocos2d::CCObject *sender);
    void fightBtnCallBack(cocos2d::CCObject *sender);
    void searchBtnCallBack(cocos2d::CCObject *sender);
    
    kingTomb::kingTombInfo * m_pInfo;
    
    void upDataAimView();
    
    void upDataBtnView();
    
    void upDataItemView();
    void upDataSpeedNumView();
    void upDataOutPutView();
    void upDataSpeedBtnView();
    
    void upDataViewIsNone();
    
    CCNode *m_pLayer;
    
    uint32_t m_pSearchTimes;
    uint32_t m_index;
    
    bool m_pAnimatePlay;
    
    bool m_pIsUpData;
    
    void setOutPutNumBmTf(uint32_t num);
    
};


#endif /* defined(__LibQSMY__KingTomb__) */
