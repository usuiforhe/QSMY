//
//  ChapterView.h
//  QSMY
//
//  Created by wanghejun on 13-4-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ChapterView__
#define __QSMY__ChapterView__

#define CHAPTER_MAP_MAX	29

#include "AppInclude.h"
#include "model/PveModel.h"
#include "model/UserModel.h"
#include "../common/common.h"
#include "RewardItemBox.h"

#define ANIMATION_COUNT 3

class PveDelegate;
class ChapterView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public PLWheelViewDelegate
{
	CC_SYNTHESIZE(bool, m_bFromMapList, IsFromMapList);
public:
	ChapterView();
    virtual ~ChapterView();
    CREATE_FUNC(ChapterView);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

	PveModel* GetPveModel() const {return m_pPveModel;}
public:
	virtual bool										onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler					onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler	onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName) {return NULL;}
public:
    void setDelegate(PveDelegate* pDelegate);    
    void touchChapterCell(const uint32_t &chapterID);    
    void rotateTo(const int &cid);
    void updateView();
	void onTouchEquip(cocos2d::CCObject *obj);
	void touchElite(CCObject *obj);

	void PLWheelViewTouched(PLWheelView *pView);
	void PLWheelViewCenterIdxChanged(PLWheelView *pView, unsigned int centerIdx);
	void PLWheelViewStartWheeling(PLWheelView *pView, unsigned int centerIdx);
	void PLWheelViewStopWheeling(PLWheelView *pView, unsigned int centerIdx);
	void PLWheelViewBouncePercent(PLWheelView *pView, float pct);

private:
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
    virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
private:    
    void onChaptersUpdated();    
private:
	cocos2d::CCNode*			m_pContainer;
	cocos2d::CCLabelBMFont*		m_pLblPlan;
    PveDelegate*				m_pDelegate;
    PLWheelView*				m_pView;
    PveModel*					m_pPveModel;
	cocos2d::extension::CCData*	m_pCCData;
    int                         m_nCid;

	void updateChapter(pve::ChapterInfo *pInfo);


	cocos2d::extension::CCScrollView *m_pScrBack;
	cocos2d::CCNode *m_pCCBBack;
	cocos2d::CCSprite *m_pSprBack;

	cocos2d::CCNode	*m_pNodeAllStar;

	RewardItemBox *m_pBoxPlan;

	CCNode	*m_pNodeBack;

	cocos2d::CCNode *m_pNodeBg;

	cocos2d::CCNode	*m_pNodeDrop;

	CCMenuItem *m_pBtnRush;
	CCLabelBMFont *m_RushTime;
	
	CCNode *m_pNodeRotation;
	CCNode *m_pNodeDraggingDown;
	CCNode *m_pNodeDraggingUp;

	CCMenuItem *m_pBtnPve;
	CCMenuItem *m_pBtnWonder;
	CCMenuItem *m_pBtnElite;

	CCNode	*m_pNodeFame;
	CCNode *m_pBackContainer;
	CCNode *m_pCoinTip;
	CCNode *m_pTianqiBox;
	CCMenuItemImage *m_pTianqiBtn;
	CCLabelTTF *m_pActivityTime;

	void callBackWonder(CCObject *pObj);

	unsigned int m_uCenterIdx;

	void prepareToEnter();
public:
	void prepareToExit(unsigned int cid);
private:
	void enterActionFinished();
	void actionFinished(CCObject *pObj);
	void onOpenTianqi(CCObject *obj);
private:
	CCNode *m_pMapBuild[CHAPTER_MAP_MAX];
	CCNode *m_pMapName[CHAPTER_MAP_MAX];
	CCNode *m_pMapArrow[CHAPTER_MAP_MAX];

	void updateMapPos(float dur);
	
	bool m_bNeedUpdatePos;

	void updateScale(float dt);
	void scaleToMin(float dt);
	void updateScaleMin(float dt);
	void updateActivityTime(float dt);

	CCPoint m_ptDeltaPos;
	float m_fDeltaScale;

	void showAllWithName();
	void hideOtherAndName();

	bool m_bTouching;

	sp::Armature *m_pArmatureBuilding;

	CCDictionary *m_pArrCell;

	float m_fMaxPct;
	float m_fMinPct;
	bool m_bWheeling;
	sp::Armature *m_pArmatureCell;

	void addArmatureToCell(sp::Armature *pArmature, CCTableViewCell *pCell);

	cocos2d::CCNode *m_pNodeWheel;

	sp::Armature *m_pArmatureEnv;
	unsigned int m_ActivityEndT;
};

class ChapterViewCell:
	public cocos2d::extension::CCTableViewCell,
	public cocos2d::extension::CCBMemberVariableAssigner,
	public cocos2d::extension::CCBSelectorResolver
{
public:
    ChapterViewCell();
    virtual ~ChapterViewCell();
    
    static ChapterViewCell * createChapterViewCell(ChapterView *chapterView, cocos2d::extension::CCData* pData , bool bCurrentChapter = false);
    
    virtual bool init(ChapterView *chapterView, cocos2d::extension::CCData* pData);
    
    virtual void onEnter();
    
public:
	virtual bool										onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler					onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler	onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName) {return NULL;}
public:
    void updateCell(pve::ChapterInfo * pChapterInfo);
	void onTouchArrow(CCObject *obj);
	void onTouchEquip(CCObject *obj);

	void setIsCenter(bool isCenter);
	void showTouch(bool bShow);
    
private:
    void updateView();
    
private:
    
	bool bCurrentChapter;

    ChapterView*		m_pChapterView;
	pve::ChapterInfo*	m_pChapterInfo;

// 	cocos2d::CCNode*			m_pBackground;
// 	cocos2d::CCLabelBMFont*		m_pLblPlan;
// 	cocos2d::CCMenuItemImage*	m_pArrow;
// 	cocos2d::CCLabelBMFont*		m_ChapterName;
// 	RewardItemBox*				m_pRewardItemBox;
//     
//     
//     cocos2d::CCNode*        m_pThreeStarEat;
//     cocos2d::CCNode*        m_pThreeStar;


	CCNode	*m_pNodeMapInfo;
	CCNode	*m_pNodeChapter;
	CCLabelBMFont	*m_pBMFMapName;
	CCLabelBMFont	*m_pBMFChapterName;
	CCMenuItem	*m_pBtnChapter;

	cocos2d::CCNode*			m_pImageNode;//unselected , image
	cocos2d::CCNode*			m_pAnimationNode;//onselected , animation
	cocos2d::CCNode*			m_images[ANIMATION_COUNT];
	cocos2d::CCNode*			m_animations[ANIMATION_COUNT];
	UIAnimate *m_pAnimateTouch;
};

#endif /* defined(__QSMY__ChapterView__) */
