//
//  Tutorial.h
//  新手引导
//
//  Created by wanghejun on 13-8-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Tutorial__
#define __QSMY__Tutorial__

#include "AppInclude.h"
#include "components/components.h"
#include "model/TutorialModel.h"
#include "TutorialWarrior.h"

#define TURORIAL_DEBUG 0

#pragma mark - TutorialDelegate -

class TutorialDelegate {
public:
    virtual void nextStep() = 0;
};


#pragma mark - Tutorial -

class Tutorial
:public cocos2d::CCNode
,public TutorialDelegate
{
public:
    bool init();
    static Tutorial* sharedTutorial(void);
    static void purgeTutorial(void);

	//引导时手指动画的位置
	CC_SYNTHESIZE(CCPoint, m_LastFingerPosition, LastFingerPositon);

private:
    Tutorial();
    virtual ~Tutorial();
    virtual void nextStep();
private:
    TutorialModel  *m_pModel;
    
};


#pragma mark - Step -

class Step
:public DLayerModal
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    CC_SYNTHESIZE(TutorialDelegate*, m_pDelegate, Delegate);
public:
    Step();
    virtual ~Step();
    
    D_TOUCH_REGISTER_DEFAULT( DTOUCH_TUTORIAL_PRIORITY );
    
    static Step* create(tutorial::StepInfo* pStepInfo);
    virtual bool init(tutorial::StepInfo* pStepInfo);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName) = 0;
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
	virtual void onEnter();
	virtual void onExit();

protected:
        cocos2d::CCLabelTTF *m_pContent;
		cocos2d::CCSprite *m_pSpriteHead;
        cocos2d::CCSprite   *m_pTouchContinue;
        cocos2d::CCNode   *m_pLeftMark;//头像位置标记
        cocos2d::CCNode   *m_pRightMark;
        cocos2d::CCNode   *m_pLeftLabelMark;//文本框位置标记
        cocos2d::CCNode   *m_pRightLabelMark;//文本框位置标记
        cocos2d::CCNode     *m_pMaskNode;
        cocos2d::CCNode     *m_pRootNode;
		cocos2d::CCNode *m_pDialogNode;
		cocos2d::CCMenuItem* m_pTouchMenu;
protected:
    virtual void updateContentInfo(); //更新步骤信息
    virtual void loadUI() = 0;
    virtual void checkIsTrigger();  //检查是否激活
    virtual void trigger();          //激活
    virtual void nextStep();        //下一步
    virtual void finishStep();      //提交后台
	virtual void waitForWonder();
    virtual void stopSound();
protected:
    tutorial::StepInfo* m_pStepInfo;
    std::string         m_sContentId;
	uint32_t	m_nSoundID;
    std::string         m_sSoundName;

	void delayPlay(CCObject *pName);
};

#pragma mark - StepEnForce -

class StepEnForce
:public Step
{
public:
    StepEnForce();
    virtual ~StepEnForce();
    
    virtual bool init(tutorial::StepInfo* pStepInfo);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
protected:
    cocos2d::CCNode     *m_pAwContainer;
protected:
	void touchMenuCallBack(cocos2d::CCObject* obj);
    void buttonCallBack(cocos2d::CCObject* obj);
    void areaCallBack(cocos2d::CCObject* obj);
    void finishCallBack(cocos2d::CCObject* obj);
    
    virtual void nextSubStep();     //下一小步
    virtual void onStepFinished();  //当做完该步骤
    virtual void simTouch();        //模拟事件
	virtual void updateContentId();
    virtual void loadUI();
    virtual void trigger();
    virtual void nextStep();
    virtual void simTouch(cocos2d::CCMenuItem*);
	virtual void updateContentInfo();

	bool checkWhetherNeedSkipKeyStep();

	virtual void delayToNextSubStep(float t);
	void  runArrowTapAction();

protected:
	sp::Armature* m_pArrowArmature;
	bool					m_isSkipingKeyStep;
    uint8_t             m_nCurrentSubStep;
    bool                m_bSimTouch;
    cocos2d::CCPoint    m_touchPoint;
};





#pragma mark - StepPrompt -

class StepPrompt
:public Step
{
public:
    StepPrompt();
    virtual ~StepPrompt();
	 virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
protected:
    virtual void trigger();
    virtual void loadUI();
    virtual void onStepFinished();
    virtual bool init(tutorial::StepInfo* pStepInfo);
	virtual void updateContentInfo();
protected:
    void closeCallBack(cocos2d::CCObject* obj);
    void goCallBack(cocos2d::CCObject* obj);
protected:
    bool gotoNav;
	CCNode* m_pIconContainer;
	DButton*	m_pGoButton;
	DButton*	m_pCloseButton;
public:
	void closeStep() {closeCallBack(NULL);}; // for android back key
};



#endif /* defined(__QSMY__Tutorial__) */
