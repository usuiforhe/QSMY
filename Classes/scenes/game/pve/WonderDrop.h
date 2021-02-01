//
//  WonderDrop.h
// 奇遇掉落
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__WonderDrop__
#define __QSMY__WonderDrop__

#define CHEST_BOX_COLOR_NUM	3

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../common/gui/AwardItemSmall.h"
#include "../common/gui/WarriorGradeBg.h"

#include "Pve.h"
enum ChestColor{
	kGoldColor,
	kSiverColor,
	kCopperColor
};

enum ChestState{
	kOpenState,
	kCloseState
};

class WonderDrop
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(Pve *, m_pDelegate, Delegate);
	CC_SYNTHESIZE(uint32_t, m_uIdx, Index);
	CC_SYNTHESIZE(bool, m_bIsDone, IsDone);
public:
    
    WonderDrop();
    virtual ~WonderDrop();
	CREATE_FUNC(WonderDrop);
	virtual bool init();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };
    
	virtual void onEnter();
	virtual void onExit();

	void setData(settlement::WonderInfo *pData);
	D_POPUP_PRIORITY(DPOPUP_WONDER_DROP_PRIORITY);

private:

	void closeCallBack(CCObject* obj);
	void leftCallBack(CCObject *obj);
	void rightCallBack(CCObject *obj);
	void onMessageBoxComfirm(MessageType type,DMessageBox*target);

	void updateView();
	void showChestState(uint8_t color, ChestState state);

	cocos2d::CCNode *m_pNodeType[settlement::kWonderTypeMax - 1];

	cocos2d::CCMenuItem *m_pBtnLeft;
	cocos2d::CCMenuItem *m_pBtnRight;
	cocos2d::CCMenuItem *m_pBtnThird;

	cocos2d::CCNode *m_pLeftLabel7;
	cocos2d::CCNode *m_pRightLabel7;
    cocos2d::CCNode *m_pNode3LaterLabel;
    cocos2d::CCNode *m_pNode2Btns;
    DButton         *m_pNode2LeftBtn;
    DButton         *m_pNode2RightBtn;

	cocos2d::CCLabelBMFont *m_pBMFTime;
//	cocos2d::CCNode *m_pNodeIcon;
//	cocos2d::CCLabelTTF *m_pTTFName;
	AwardItemSmall *m_pNodeItem;
	cocos2d::CCNode *m_pNodeHead;
	cocos2d::CCNode *m_pNode3Btns;
	cocos2d::CCNode *m_pNode1Btn;
	CCRichLabelTTF *m_pRichDescription;
    cocos2d::CCLabelTTF *m_pWarriorName;
	cocos2d::CCLabelBMFont *m_pBMFExp;
	cocos2d::CCLabelBMFont *m_pPoint;
    cocos2d::CCNode     *m_pUserPointsNode;
    
    //boss
    WarriorGradeBg*         m_pBossGrade;
    cocos2d::CCNode*        m_pBossContainer;
    cocos2d::CCLabelTTF*    m_pBossName;
    cocos2d::CCLabelBMFont* m_pBossLevel;

	settlement::WonderInfo *m_pData;

	virtual void update(float dt);

	void wonderCallBack(const Json::Value &,const Json::Value &responseData);

	void closeSelf();
	void animateBeforeClose();
	void nodeScaled();

	float m_fDisposeTime;

	CCNode *m_pRoot;
	void animateEnd(sp::Armature * target, const char* event,const char* data);
	sp::Armature *m_pArmature;

	bool m_bAnimating;

	WarriorGradeBg *m_pGradeWarrior;

	cocos2d::CCNode *m_pBoxColor[CHEST_BOX_COLOR_NUM];
	cocos2d::CCNode *m_pBoxState[CHEST_BOX_COLOR_NUM*2];
	uint8_t m_ichestGrade;
};

#endif /* defined(__QSMY__WonderDrop__) */
