//
//  AwardItemSmall.h
//  领奖ICON--用于领取多个奖励界面
//
//  Created by cll on 13-7-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__AwardItemSmall__
#define __QSMY__AwardItemSmall__

#include "AppInclude.h"
#include "model/DropModel.h"
#include "components/gui/UIAnimate.h"
#include "WarriorHeadGradeBg.h"
#include "WarriorGradeBg.h"
#include "components/label/CCRichLabelTTF.h"

class AwardItemSmall
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE_READONLY(cocos2d::CCNodeRGBA*, m_pGrade, Grade);	//品阶
    CC_SYNTHESIZE_READONLY(WarriorHeadGradeBg*, m_pGradeNode, GradeNode);
    CC_SYNTHESIZE_READONLY(WarriorGradeBg*, m_pGradeBg, GradeBg);
    
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pIconSprite, IconSprite);				//图标
	CC_SYNTHESIZE_READONLY(cocos2d::CCRichLabelTTF*, m_pItemRichName, ItemRichName);				//名
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pItemName, ItemName);				//名
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pItemNum, ItemNum);			//数量
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pItemNumTTF, ItemNumTTF);			//数量
	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pIconMenu, IconMenu);		
	
	
public:
    CREATE_FUNC(AwardItemSmall);
    AwardItemSmall();
    virtual ~AwardItemSmall();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}
	void updateView(drop::Drop *pDropItem);

	void setRotateAnimationPlay(bool bPlay);
	void setScaleAnimatePlay(bool bPlay);
private:
	cocos2d::CCNode * m_pPieceIcon;
	cocos2d::CCNode * m_pSoulMark;
	cocos2d::CCNode * m_pDressMark;

	UIAnimate *m_pAnimateRotate;
	UIAnimate *m_pAnimateScale;

	cocos2d::CCSprite *m_pIcon;

	cocos2d::CCNode * m_pGreenNode;
	cocos2d::CCNode * m_pBlueNode;
	cocos2d::CCNode * m_pPurpleNode;
	cocos2d::CCNode * m_pOrangeNode;

	cocos2d::CCNode * m_pGreenCircle;
	cocos2d::CCNode * m_pBlueCircle;
	cocos2d::CCNode * m_pPurpleCircle;
	cocos2d::CCNode * m_pOrangeCircle;
};


class AwardItemSmallLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(AwardItemSmallLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(AwardItemSmall);
};

#endif /* defined(__QSMY__AwardItemSmall__) */
