//
//  LotteryEffect.h
//  QSMY
//
//  Created by wanghejun on 13-8-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__LotteryEffect__
#define __QSMY__LotteryEffect__

#define LOTTERY_TEN_MAX	10

#include "AppInclude.h"
#include "../common/common.h"
#include "managers/DataManager.h"
#include "DragonBone.h"
#include "CardButton.h"
#include "../common/gui/WarriorHead.h"

#include "../warrior/WarriorDelegate.h"
#include "../warrior/WarriorInfo.h"

class LotteryEffect;
class LotteryDelegate
{
	CC_SYNTHESIZE(unsigned int, m_iCurrWid, CurWarriorId);
public:
	virtual void onceMore(DPopup *view, card::CardType eType) = 0;
	//新增一个回调 处理抽卡fromlua退出界面
	virtual void onClose(){};
	virtual void playHeroSound(unsigned int wid)=0;
};

class LotteryEffect
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
,public ArmatureLoaderDelegate
,public WarriorDelegate
{
	CC_SYNTHESIZE(LotteryDelegate *, m_pDelegate, Delegate);
public:
    
    LotteryEffect();
    virtual ~LotteryEffect();
    
	static LotteryEffect* create();
	virtual bool init();

    static LotteryEffect* create(warrior::WarriorCurInfo*, card::CardType eType);
    virtual bool init(warrior::WarriorCurInfo*, card::CardType eType);
    
    static LotteryEffect* create(warrior::SoulInfo*, card::CardType eType);
    virtual bool init(warrior::SoulInfo*, card::CardType eType);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setCallBack(cocos2d::CCObject *pTarget,cocos2d::SEL_CallFuncO pCallBack);

	void loadingFinished(ArmatureLoader *pLoader);

	virtual void closeInfoView(WarriorViewType type);
	void setWarriorExistsState(bool isExist);
	virtual void onExit();

	void setData(warrior::WarriorCurInfo*, card::CardType eType);
	void setData(warrior::SoulInfo*, card::CardType eType);
private:
    void setData(warrior::WarriorBaseInfo*, card::CardType eType);
    virtual bool init(warrior::WarriorBaseInfo*, card::CardType eType);
	virtual void onBackKey(); //for android back key
    void menuCallBack(cocos2d::CCObject*);
    void closeCallBack(cocos2d::CCObject*);
	void shareCallBack(cocos2d::CCObject*);
	void checkCallBack(cocos2d::CCObject*);
    void animateEnd(sp::Armature * target, const char* event,const char* data);
    
protected:
    void callback();
protected:
    cocos2d::CCObject   *m_pTarget;
    cocos2d::SEL_CallFuncO     m_pCallBack;
    
private:
    cocos2d::CCNode         *m_pNode;
	cocos2d::CCNode			*m_pBtnNode;
    
    cocos2d::CCMenuItem     *m_pCloseButton;
//     cocos2d::CCLabelTTF     *m_pName;
//     cocos2d::CCLabelBMFont  *m_pNum;
//     
//     cocos2d::CCLabelBMFont  *m_pSpeed;
//     cocos2d::CCLabelBMFont  *m_pHp;
//     cocos2d::CCLabelBMFont  *m_pDefence;
//     cocos2d::CCLabelBMFont  *m_pAttack;
//     cocos2d::CCSprite       *m_pSoulMark;

//	DButton					*m_pShareButton;
	DButton					*m_pCheckButton;
    
private:
	sp::Armature            *m_pWarriorShow;
//	sp::Armature            *m_pLottery;

//	cocos2d::CCNode			*m_pNodeGrade[WARRIOR_GRADE_NUM];
	warrior::WarriorBaseInfo *m_pWarriorBaseInfo;
	cocos2d::CCNode			*m_pPopupNode;
	CardButton	*m_pCard;
	card::CardType	m_eType;
	DButton *m_pDBtnContinue;

	WarriorGradeBg *m_pGradeWarrior;

	CCMenuItem *m_pMenuItem;

	bool m_bReady;
	bool m_bAnimationReady;

	ArmatureLoader *m_pLoader;

	unsigned int m_uSoulNum;

	CCNode *m_pNodeOne;
	CCNode *m_pNodeTen;
	CCNode *m_pNodeBack;
	CCNode *m_pNodeGoon;
	DButton					*m_pCheckButtonTen;
	WarriorInfoView* m_pWarriorInfoView;

	void onceMoreCallBack(cocos2d::CCObject* pObj);

	void callBackAutoClose(float dt);
	void releaseSchedule();
	void resetSchedule();

	cocos2d::CCNode *m_pNodeRest;
	cocos2d::CCNode *m_pNodeNext;
	cocos2d::CCLabelBMFont *m_pBMFRest;
	cocos2d::CCNode *m_pNodeTips;

	WarriorGradeBg *m_pGrade;
	WarriorGradeBg *m_pGradeName;
	cocos2d::CCLabelTTF *m_pTTFName;
	cocos2d::CCSprite* m_pHeroOwned;
	cocos2d::CCLabelBMFont *m_pZizhiNum;
	void updateRest();
};

class LotteryEffectTen
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE(LotteryDelegate *, m_pDelegate, Delegate);
public:

	LotteryEffectTen();
	virtual ~LotteryEffectTen();

	//预先加载
	static LotteryEffectTen* create(bool special = false);
	virtual bool init();
	void setData(cocos2d::CCArray *pArr);

	static LotteryEffectTen* create(cocos2d::CCArray *pArr , bool special = false);//默认非限时英雄
	virtual bool init(cocos2d::CCArray *pArr);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setCallBack(cocos2d::CCObject *pTarget,cocos2d::SEL_CallFuncO pCallBack);
	void showWarrior();

private:
	//时候从lua调用的限时英雄
	bool m_bSpecial;

	WarriorHead *m_pWarrior[LOTTERY_TEN_MAX];
	DButton *m_pDBtnOnceMore;
	DButton *m_pDBtnTenthMore;
	cocos2d::CCMenuItem     *m_pCloseButton;

	void callBackOnce(CCObject *pObj);
	void callBackTenth(CCObject *pObj);
	void closeCallBack(cocos2d::CCObject*);

	cocos2d::CCArray *m_pArr;	sp::Armature            *m_pWarriorShow;

	unsigned int m_uIdx;

	CCNode *m_pNodeDrop;

	void animateEnd(sp::Armature * target, const char* event,const char* data);
	virtual void onBackKey(); //for android back key
	void effectCallBack(CCObject *pObj);

	cocos2d::CCObject   *m_pTarget;
	cocos2d::SEL_CallFuncO     m_pCallBack;

	cocos2d::CCNode *m_pNodeRest;
	cocos2d::CCNode *m_pNodeNext;
	cocos2d::CCLabelBMFont *m_pBMFRest;
	cocos2d::CCNode *m_pPointCost;
	cocos2d::CCNode *m_pItemCost;
	void updateRest();
};

#endif /* defined(__QSMY__LotteryEffect__) */
