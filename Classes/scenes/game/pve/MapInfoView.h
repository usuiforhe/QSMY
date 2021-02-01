//
//  MapInfoView.h
//  QSMY
//
//  Created by wanghejun on 13-4-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MapInfoView__
#define __QSMY__MapInfoView__

#include "AppInclude.h"
#include "model/PveModel.h"
#include "model/UserModel.h"
#include "DragonBone.h"
#include "../common/common.h"
#include "MapProgress.h"


#define STEP_COUNT	5
#define PATH_COUNT	STEP_COUNT - 1

class RewardItemBox;
class PveDelegate;
class MapInfoView : 
	public DLayer,
	public cocos2d::extension::CCBMemberVariableAssigner,
	public cocos2d::extension::CCBSelectorResolver
	,public DPopupDelegate
{
public:
    MapInfoView();
    ~MapInfoView();
    CREATE_FUNC(MapInfoView);
    virtual bool init();
    
    virtual void onExit();
    virtual void onEnter();
    
private:
    void binding();
    void unbinding();
    void showPresAdd();
    void removePresEffect(sp::Armature * target, const char* event,const char* data);
private:
    CC_SYNTHESIZE_READONLY(pve::MapInfo*, m_pMapInfo, MapInfo);
    CC_SYNTHESIZE_READONLY(pve::ChapterInfo*, m_pChapterInfo, ChapterInfo);
    
    PveDelegate*			m_pDelegate;    
//	cocos2d::CCLabelBMFont*	m_ChapterName;
	RewardItemBox*			m_pRewardItemBox;
//	cocos2d::CCNode*		m_EquipGet;
//    cocos2d::CCSprite*      m_EquipGotten;
	cocos2d::CCLabelBMFont*	m_pLblPlan;
//	cocos2d::CCSprite*		m_pBackground;
//	cocos2d::CCLabelTTF*	m_mapName;
//    cocos2d::CCRichLabelTTF*m_pRichMapName;
//	cocos2d::CCNode*		m_pItemAward;
//	ItemBox*                m_pEquipBox1;
//    cocos2d::CCNode*                    m_pPresEffectNode;
    
   DEffectLayer            *m_pEffectLayer;
   cocos2d::CCNode*        m_pExplorerEffect;
   cocos2d::CCNode*        m_pButtonAnimationSW;
//    cocos2d::CCNode*        m_pEffectNode;
//	MapProgress*			m_pProgress;
//	cocos2d::CCNode *m_pDialogContainer;
	StoryTalk *m_pDialog;
	DPopup *m_pDialogPopup;
    
private:
//    sp::Armature                        *m_pPresEffect;
    int                                 m_nFightNum;
    int                                 m_nFightOpen;
    int                                 m_nMapType;
    

private:    
    void updateView();
    void onMapInfoUpdated();
    void removeNode(CCNode *obj);
	void onTouchArrow(cocos2d::CCObject *obj);
	void onTouchExplore(cocos2d::CCObject *obj);
	void onTouchItem1(cocos2d::CCObject *obj);
	void onTouchItem2(cocos2d::CCObject *obj);
	void onTouchItem3(cocos2d::CCObject *obj);
    
    void AdjustExplorePoint(int nFightNum, int nFightOpen, int nMapType);
public:
    void setDelegate(PveDelegate* pDelegate)
    {
        m_pDelegate = pDelegate;
    }
    
    void setMapInfo(pve::MapInfo *pMapInfo);
    void displayExploreAnimation();
    
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName) {return NULL;}
 
#pragma mark - Accelerate -
public:
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
private:
    cocos2d::CCPoint                    oPos;
    float                               oScale;

public:
	void playDialog(pve::MapDialogInfo* pInfo);
	void playNextDialog(CCObject *sender);

private:
	uint32_t m_nSoundID;
public:
#pragma mark - DPopupDelegate;
	virtual void onPopupClose(DPopup*);
	virtual void onPopupBack(DPopup* pPopup);

	CC_SYNTHESIZE(bool, m_bShowingDialog, isShowingDialog);
//////////////////////////////////////////////////////////////////////



private:
	cocos2d::CCMenuItem	*m_pBtnProgress[STEP_COUNT];
	cocos2d::CCMenuItem *m_pBtnPath[PATH_COUNT];

	cocos2d::CCNode	*m_pNodeGot;
	cocos2d::CCLabelBMFont *m_pBMFCoins;
	cocos2d::CCLabelBMFont *m_pBMFExp;

	cocos2d::CCNode *m_pNodeFight;
	CCLabelTTF *m_pTTFDes;
	CCRichLabelTTF* m_pSuggestLevel;
	cocos2d::CCNode *m_pNodeViewOrEvade;

	CCNode	*m_pNodeHead;

	CCNode	*m_pIconContainer;

	CCNode	*m_pNodeExplore;
	UIAnimate *m_pAnimateResult;

public:
	void showResult();
	void hideResult();

	void showBattleIfNeeded();

private:
	unsigned int m_uFirstStep;
	CCMenuItem *m_pBtnPve;
	CCMenuItem *m_pBtnWonder;
	CCMenuItem *m_pBtnElite;
	void callBackWonder(CCObject *pObj);
	void exploreEnd(CCObject *pObj);
	void touchElite(CCObject *pObj);
};

#endif /* defined(__QSMY__MapInfoView__) */
