//
//  MapView.h
//  QSMY
//
//  Created by wanghejun on 13-4-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MapView__
#define __QSMY__MapView__

#include "AppInclude.h"
#include "model/PveModel.h"
#include "model/UserModel.h"
#include "../common/common.h"

#define STARS 3

class RewardItemBox;
class PveDelegate;
class MapView:
	public DLayer,
	public cocos2d::extension::CCBMemberVariableAssigner,
	public cocos2d::extension::CCBSelectorResolver,
	public cocos2d::extension::CCTableViewDataSource, 
	public cocos2d::extension::CCTableViewDelegate
	,public PLWheelViewDelegate
{
    CC_SYNTHESIZE_READONLY(pve::ChapterInfo*, m_pChapterInfo, ChapterInfo);   
	CC_SYNTHESIZE(bool, m_bFromChapter, IsFromChapter);
	CC_SYNTHESIZE(bool, m_bFromMapInfo, IsFromMapInfo);
public:
    CREATE_FUNC(MapView);
    MapView();
    virtual ~MapView();
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();    
public:
    void setDelegate(PveDelegate* pDelegate)
    {
        m_pDelegate = pDelegate;
    }
    
    void onTouchCell(cocos2d::extension::CCTableViewCell* pCell);
    virtual void update(float dt);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
    virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
	virtual bool										onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler					onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler	onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName) {return NULL;}

    void setChapterInfo(pve::ChapterInfo* pChapterInfo);
    
//     cocos2d::CCSpriteFrame* getNormalStarDisplayFrame();
//     cocos2d::CCSpriteFrame* getLightStarDisplayFrame();
private:
    void updateView();
	void updateDropInfo(unsigned int mapId);
	void onTouchArrow(cocos2d::CCObject *obj);
	void onTouchEquip(cocos2d::CCObject *obj);
    void onMapsUpdated();

	void callBackRush(cocos2d::CCObject *obj);

	void callBackAllStar(cocos2d::CCObject *obj);
	void onRushUpdated();
private:
    PveDelegate*				m_pDelegate;
    PveModel*					m_pPveModel;
    UserModel*					m_pUserModel;
    PveModel::MapInfoMap		maps;

	PLWheelView*					m_pView;
	cocos2d::CCNode*			m_pNodeChild;
// 	cocos2d::CCLabelBMFont*		m_ChapterName;
 	cocos2d::CCLabelBMFont*		m_pLblPlan;
// 	cocos2d::CCMenuItemImage*	m_pArrow;
 	RewardItemBox*              m_pRewardItemBox;
	ItemBox	*m_pBoxDrop0;
	ItemBox	*m_pBoxDrop1;

	cocos2d::CCLabelBMFont*		m_pCurrentProgress;
	cocos2d::CCNode*					m_pButtonImageNode;
	cocos2d::CCNode*					m_pButtonAnimationNode;
	cocos2d::CCNode*                    m_pButtonAnimationSW;
	cocos2d::CCNode*                    m_pNodeFame;
	cocos2d::CCNode*                    m_pNodeBack;
	cocos2d::CCNode*                    m_pNodeDrop;
	cocos2d::CCNode*                    m_pNodeJindu;
	cocos2d::CCNode*                    m_pQiyuNode;
	cocos2d::CCNode*                    m_pJuqingNode;
	cocos2d::CCNode*                    m_pDropBg;
	cocos2d::CCNode*                    m_pDropBgBoss;
	cocos2d::CCNode*                    m_pDropTitel;
	cocos2d::CCNode*                    m_pCoinTip;
	cocos2d::CCNode*                    m_pNodeQiyu;

	cocos2d::CCNode	*m_pNodeAllStar;
	cocos2d::CCMenuItem *m_pBtnAllStar;

	cocos2d::CCNode *m_pNodeBg;
	cocos2d::CCSprite *m_pSprBg;
	cocos2d::CCPoint m_ptOrigin;

	cocos2d::CCNode *m_pNodeRotation;
	CCNode *m_pNodeDraggingDown;
	CCNode *m_pNodeDraggingUp;

	CCMenuItem *m_pBtnPve;
	CCMenuItem *m_pBtnWonder;
	CCMenuItem *m_pBtnElite;

	void callBackWonder(CCObject *pObj);
	void touchElite(CCObject *pObj);

	int							m_nSelectedIndex;

	unsigned int m_uCenterIdx;
//     cocos2d::CCSprite*     m_pNormalStarSprite;
//     cocos2d::CCSprite*     m_pLightStarSprite;
    
    
    cocos2d::extension::CCData* m_pMapCellData;
//    cocos2d::extension::CCData* m_pAwardCellData;

	void prepareToEnter();

	void updateCells(float dt);

	bool m_bTouching;
public:
	void prepareToExit(unsigned int cid);
	void prepareToExitForMap(unsigned int mid);
	CCLabelBMFont   *m_RushTime;
	CCLabelBMFont getRushTime();
	void rotateTo(const int &mid);
private:
	void actionFinished(CCObject *pObj);
	void actionFinishedForMap(CCObject *pObj);
	void dropTouchEventHandler(CCObject* target);

public:
	virtual void PLWheelViewTouched(PLWheelView *pView);
	virtual void PLWheelViewCenterIdxChanged(PLWheelView *pView, unsigned int centerIdx);
	virtual void PLWheelViewStartWheeling(PLWheelView *pView, unsigned int centerIdx);
	virtual void PLWheelViewStopWheeling(PLWheelView *pView, unsigned int centerIdx);
	virtual void PLWheelViewBouncePercent(PLWheelView *pView, float pct);

private:
		float m_fMaxPct;
		float m_fMinPct;
		bool m_bWheeling;
		sp::Armature *m_pArmatureCell;

		void addArmatureToCell(sp::Armature *pArmature, CCTableViewCell *pCell);

		cocos2d::CCNode *m_pNodeWheel;

		cocos2d::CCArray *m_dropItems;
private:
	float m_nRushTimer;
};

class MapViewCell :
	public cocos2d::extension::CCTableViewCell,
	public cocos2d::extension::CCBMemberVariableAssigner,
	public cocos2d::extension::CCBSelectorResolver
{
public:
    MapViewCell();
    virtual ~MapViewCell();
    
	static MapViewCell* create(cocos2d::extension::CCData* pData, PveDelegate* pDelegate, MapView* pMapView);
	virtual bool init(cocos2d::extension::CCData* pData, PveDelegate* pDelegate, MapView* pMapView);
    virtual void onEnter();
    virtual void onExit();
public:
	virtual bool										onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler					onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler	onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName) {return NULL;}
public:
    void onRushUpdated();
	virtual void update(float dt);

	void onTouchNode(CCObject *p);
	void onRush(CCObject *p);
	void onExplore(CCObject *p);
    void updateCell(pve::MapInfo *mapInfo, bool bExpand);

	void setIsCenter(bool isCenter);
	void showTouch(bool bShow);
private:
    void updateView();
    void updateRushStatus();
private:
    float m_nRushTimer;
    
private:
	cocos2d::CCNode*		m_Node;	//weak
	
//	cocos2d::CCNode*		m_pRushNode;
//	cocos2d::CCNode*		m_pHaveTimesNode;
//	cocos2d::CCNode*		m_spIcon;
//	cocos2d::CCNode*		m_extNode;
//	cocos2d::CCLabelTTF*	m_MapBackDesc;
//	ItemBox*                m_pEquipBox1;
//	cocos2d::CCSprite*		m_pItemNone1;
//	cocos2d::CCSprite*		m_bNewMap;
//	cocos2d::CCLabelBMFont*	m_haveTimes;
//	cocos2d::CCLabelTTF*	m_pMapDesc;
//	cocos2d::CCRichLabelTTF*	m_pRichMapDesc;
//	cocos2d::CCNode*		m_NeedMoney;
//	cocos2d::CCLabelBMFont*	m_NeedMoneyNum;
//	cocos2d::extension::CCScale9Sprite*	m_pRushTimeBack;

	
//	cocos2d::CCSprite*		m_limitFight;
//	cocos2d::CCNode*        m_bossBack;
//	cocos2d::CCNode*        m_lbBack;
//	cocos2d::CCNode*        m_normalBack;

//	cocos2d::CCSprite*			m_BossIcon;
//	cocos2d::CCLabelBMFont*		m_suggestLev;
	cocos2d::CCSprite*			m_star[STARS];

	cocos2d::CCNode*			m_pImageNode;//unselected , image
	cocos2d::CCNode*			m_pAnimationNode;//onselected , animation
	cocos2d::CCNode*			m_images[STARS];
	cocos2d::CCNode*			m_animations[STARS];
    
//    cocos2d::CCNode*            m_pNewMapEffectNode;

	pve::MapInfo*           m_pMapInfo;
	PveDelegate*			m_pDelegate;
	PveModel*				m_pPveModel;
	UserModel*				m_pUserModel;
	MapView*                m_pMapView;


	CCNode	*m_pNodeMapInfo;
	CCNode	*m_pNodeChapter;
	CCLabelBMFont	*m_pBMFMapName;
	CCLabelBMFont	*m_pBMFChapterName;
	CCMenuItem	*m_pBtnMap;
	CCMenuItem  *m_pBtnMapBoss;

	CCLabelBMFont	*m_pBMFNeedPower;
	CCLabelBMFont	*m_pBMFRest;

	UIAnimate *m_pAnimateTouch;

	CCNode *m_pStarBack;
};

#pragma mark - AllStarAwardCell -

class AllStarAwardCell :
public cocos2d::extension::CCTableViewCell,
public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_RETAIN(pve::ChapterInfo*, m_pChapterInfo, ChapterInfo);
public:
    AllStarAwardCell();
    virtual ~AllStarAwardCell();
    
	static AllStarAwardCell* create(cocos2d::extension::CCData* pData, PveDelegate* pDelegate);
	virtual bool init(cocos2d::extension::CCData* pData, PveDelegate* pDelegate);
    virtual void onEnter();
    virtual void onExit();
public:
	virtual bool										onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
private:
    void updateView();
    void rewardCallBack(cocos2d::CCObject* sender);
	void replayCallBack(cocos2d::CCObject *sender);
private:
	cocos2d::CCMenuItem         *m_pReward;
    cocos2d::CCNode             *m_pGet;
	cocos2d::CCNode             *m_pGotten;

	cocos2d::CCNode				*m_pAnimate;
private:
    PveDelegate                 *m_pDelegate;

private:
	cocos2d::CCMenuItem			*m_pReplay;
};



#endif /* defined(__QSMY__MapView__) */
