//
//  Tutorial.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "AppInclude.h"
#include "Tutorial.h"
#include "utils/TimerUtil.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "managers/SoundManager.h"
#include "managers/PlatformManager.h"
#include "../Game.h"
#include "./steps/StepFormation.h"
#include "./steps/StepSelectWarrior.h"
#include "./steps/StepPve.h"
#include "./steps/InputName.h"
#include "./steps/StepEquip.h"
#include "./steps/StepFirstBattle.h"
#include "./steps/StepSecondPve.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const CCSize g_fingerSize = CCSizeMake(87.0f, 84.0f);
static Tutorial *s_SharedTutorial = NULL;

Tutorial* Tutorial::sharedTutorial(void)
{
    if (!s_SharedTutorial)
    {
        s_SharedTutorial = new Tutorial();
        s_SharedTutorial->init();
    }
    
    return s_SharedTutorial;
}


void Tutorial::purgeTutorial(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedTutorial);
}

Tutorial::Tutorial()
:m_pModel(NULL)
,m_LastFingerPosition(CCPointZero)
{

}

Tutorial::~Tutorial()
{
    //SAFE_RELEASE_UNBINDING_ALL(m_pModel, this);
    CC_SAFE_RELEASE(m_pModel);
}

bool Tutorial::init()
{
    m_pModel = DM_GET_TUTORIAL_MODEL;
    CC_SAFE_RETAIN(m_pModel);
    //BINDING_EXEC(m_pModel, this, Tutorial::nextStep, D_EVENT_TUTORIAL_STEP_COMPLETE);
    nextStep();
    return true;
}

void Tutorial::nextStep()
{
    if(m_pModel->isAllStepComplete())
    {
        purgeTutorial();
        return;
    }
    removeAllChildren();
    Step* pStep = Step::create(m_pModel->getNextStep());
    if(pStep)
    {
        pStep->setDelegate(this);
        addChild(pStep);  
    }
}

//MARK: - Step -

Step::Step()
:m_pStepInfo(NULL)
,m_pDelegate(NULL)
,m_pContent(NULL)
,m_pSpriteHead(NULL)
,m_pTouchMenu(NULL)
,m_pLeftMark(NULL)
,m_pRightMark(NULL)
,m_pMaskNode(NULL)
,m_pRootNode(NULL)
,m_pLeftLabelMark(NULL)
,m_pRightLabelMark(NULL)
,m_nSoundID(0)
,m_pDialogNode(NULL)
,m_pTouchContinue(NULL)
,m_sSoundName("")
{
    
}

Step::~Step()
{
    switch (m_pStepInfo->getTriggerType()) {
        case tutorial::kTutorialTriggerTypeMapComplete:
			 UNBINDING_ALL(DM_GET_PVE_MODEL->getMapInfo(m_pStepInfo->getTriggerCondition()), this);
			 break;
		case tutorial::kTutorialTriggerTypeReputation:
			UNBINDING_ALL(DM_GET_PVE_MODEL->getChapterInfo(m_pStepInfo->getTriggerCondition()), this);
            break;
        case tutorial::kTutorialTriggerTypeCompleteStep:
            //DM_GET_TUTORIAL_MODEL->getLastStepId() >= trigger_condition;
            break;
        case tutorial::kTutorialTriggerTypeUserLvel:
            NotificationCenter::sharedNotificationCenter()->removeObserver(this, D_EVENT_CLOSE_LEVELUP_POPUP);
            break;
        default:
            break;
    }

	NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);

    CC_SAFE_RELEASE(m_pContent);
	CC_SAFE_RELEASE(m_pSpriteHead);
    CC_SAFE_RELEASE(m_pStepInfo);
	CC_SAFE_RELEASE(m_pDialogNode);
	CC_SAFE_RELEASE(m_pTouchMenu);
    CC_SAFE_RELEASE(m_pLeftMark);
    CC_SAFE_RELEASE(m_pRightMark);
    CC_SAFE_RELEASE(m_pMaskNode);
    CC_SAFE_RELEASE(m_pRootNode);
    CC_SAFE_RELEASE(m_pLeftLabelMark);
    CC_SAFE_RELEASE(m_pRightLabelMark);
    CC_SAFE_RELEASE(m_pTouchContinue);
    
    if (m_sSoundName.length() > 0) {
        SoundManager::sharedSoundManager()->unloadEffect(ccs(m_sSoundName));
    }
}

Step* Step::create(tutorial::StepInfo* pStepInfo)
{
    Step* pRet = NULL;
    if(!pStepInfo) return NULL;
    switch (pStepInfo->getType())
    {
        case tutorial::kTutorialTypePrompt:
            pRet = new StepPrompt();
            break;
        case tutorial::kTutorialTypeEnforce:
            switch (pStepInfo->getID()) {
                case 1000:
                    pRet = new InputName();
                    break;
				case 1010:
					pRet = new StepPve();
					break;
				case 510:
					pRet = new StepSelectWarrior();
					break;
				case 1110:
					pRet = new StepSecondPve();
					break;
					/*
					case 1030:
					pRet = new StepFormation();
					break;
					case 1050:
					pRet = new StepEquip();
					break;
					*/
				case G_FIRST_BATTLE_ID:
					pRet = new StepFirstBattle();
					break;
                default:
                    pRet = new StepEnForce();
                    break;
            }
            break;
        default:
            
            break;
    }
    
    if(pRet && pRet->init(pStepInfo))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool Step::init(tutorial::StepInfo* pStepInfo)
{
    if(!DLayerModal::init()) return false;
    CC_SAFE_RETAIN(pStepInfo);
    CC_SAFE_RELEASE(m_pStepInfo);
    m_pStepInfo = pStepInfo;
    setVisible(false);
    return true;
}
void Step::checkIsTrigger()
{
    if(m_pStepInfo->isTrigger())
    {
        UNBINDING_ALL(DM_GET_PVE_MODEL->getMapInfo(m_pStepInfo->getTriggerCondition()), this);
        NotificationCenter::sharedNotificationCenter()->removeObserver(this, D_EVENT_CLOSE_LEVELUP_POPUP);
        trigger();
    }else
    {
        switch (m_pStepInfo->getTriggerType()) {
			case tutorial::kTutorialTriggerTypeFirst3Star:
				NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Step::checkIsTrigger),D_EVENT_END_BATTLE);
				break;
			case tutorial::kTutorialTriggerTypeReputation:
               BINDING_ONLY(DM_GET_PVE_MODEL->getChapterInfo(m_pStepInfo->getTriggerCondition()),this,Step::checkIsTrigger, D_EVENT_PVE_CHAPTER_UPDATED);
                break;
            case tutorial::kTutorialTriggerTypeCompleteStep:
                //DM_GET_TUTORIAL_MODEL->getLastStepId() >= trigger_condition;
                break;
            case tutorial::kTutorialTriggerTypeUserLvel:
                NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(Step::checkIsTrigger), D_EVENT_CLOSE_LEVELUP_POPUP);
                break;
			case tutorial::kTutorialTriggerTypeMapComplete:
				 BINDING_ONLY(DM_GET_PVE_MODEL->getMapInfo(m_pStepInfo->getTriggerCondition()),this,Step::checkIsTrigger, D_EVENT_PVE_MAP_UPDATED);
				break;
			case tutorial::kTutorialTriggerTypeFirsrWonder:
				 NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(Step::waitForWonder), D_EVENT_GET_WONDER);
				break;
			case tutorial::kTutorialTriggerTypeFirstSignin:
				NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(Step::checkIsTrigger), D_EVENT_GAME_VIEW_CHANGED);
				break;
            default:
                break;
        }
    }
}

void Step::waitForWonder()
{
	DM_GET_TUTORIAL_MODEL->setHasWonder(true);
	checkIsTrigger();
}

#define SOUND_DELAY_TAG 678

void Step::stopSound()
{
    stopActionByTag(SOUND_DELAY_TAG);
    if (m_nSoundID) {
        SoundManager::sharedSoundManager()->stopSFX(m_nSoundID);
        m_nSoundID = 0;
    }
}

void Step::nextStep()
{
    if(!m_pStepInfo->isComplete()) //如果没有关键步骤使其完成 则在最后一部完成
    {
        finishStep();
        return;
    }
    if(m_pDelegate) m_pDelegate->nextStep();
}

void Step::trigger()
{
    setVisible(true);
}

void Step::finishStep()
{
    HTTP_CLIENT->finishRookie(m_pStepInfo->getID());
}

bool Step::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContent", CCLabelTTF *, this->m_pContent);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteHead", CCSprite *, this->m_pSpriteHead);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDialogNode", CCNode*, this->m_pDialogNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchMenu", CCMenuItem*, this->m_pTouchMenu);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLeftMark", CCNode*, this->m_pLeftMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRightMark", CCNode*, this->m_pRightMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMaskNode", CCNode*, this->m_pMaskNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRootNode", CCNode*, this->m_pRootNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLeftLabelMark", CCNode*, this->m_pLeftLabelMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRightLabelMark", CCNode*, this->m_pRightLabelMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchContinue", CCSprite*, this->m_pTouchContinue);
    return false;
}



void Step::updateContentInfo()
{
	stopSound();
    if (m_sSoundName.length() > 0) {
        SoundManager::sharedSoundManager()->unloadEffect(ccs(m_sSoundName));
        m_sSoundName = "";
    }

	tutorial::StepDialog *pDialog = DM_GET_TUTORIAL_MODEL->getStepDialog(m_sContentId.c_str());
    if (!pDialog)
    {
        return;
	}

	if (m_pDialogNode && m_pStepInfo->getType() == tutorial::kTutorialTypeEnforce)
	{
		m_pDialogNode->setPosition(pDialog->getDialogPosition());
	}

	if (m_pSpriteHead)
	{
		m_pSpriteHead->setDisplayFrame(ResourceManager::sharedResourceManager()->getTutorialEmotion(pDialog->getEmotionID())->displayFrame());
	}
    
    if (m_pLeftMark && m_pRightMark && m_pLeftLabelMark && m_pRightLabelMark) {
        CCPoint headPosition = pDialog->getProfileSide() == tutorial::kTutorialProfileSideRight ? m_pRightMark->getPosition() : m_pLeftMark->getPosition();
        m_pSpriteHead->setPosition(headPosition);
		m_pSpriteHead->setFlipX(pDialog->getProfileSide() == tutorial::kTutorialProfileSideRight);
        CCPoint labelPosition = pDialog->getProfileSide() == tutorial::kTutorialProfileSideLeft ? m_pRightLabelMark->getPosition() : m_pLeftLabelMark->getPosition();
        m_pContent->setPosition(labelPosition);
    }
    
    if(m_pContent)
    {
        CCRichLabelTTF* m_pRichLabel = CCRichLabelTTF::create();
        replaceLabel(m_pContent,m_pRichLabel);
        m_pRichLabel->setString(CCString::createWithFormat(DM_GET_TUTORIAL_MODEL->getContent(m_sContentId.c_str()),DM_GET_USER_MODEL->getUserInfo()->getUserName().c_str())->getCString());
#if TURORIAL_DEBUG
		std::string content(m_pRichLabel->getString());
		content.append(m_sContentId);
		m_pRichLabel->setString(content.c_str());
#endif
    }
    
    m_sSoundName.assign(CCString::createWithFormat("tutorial/%s",pDialog->getResID().c_str())->getCString());
    SoundManager::sharedSoundManager()->preloadEffect(ccs(m_sSoundName));
	float fDelay = pDialog->getDelay();
	if (!D_FLOAT_EQUALS(fDelay, 0.f))
	{
		CCAction* pAction = CCSequence::create(CCDelayTime::create(fDelay),
			CCCallFuncO::create(this, callfuncO_selector(Step::delayPlay), ccs(pDialog->getResID())),
			NULL
			);
        pAction->setTag(SOUND_DELAY_TAG);
		runAction(pAction);
		return;
	}

	delayPlay(ccs(pDialog->getResID()));
}

void Step::delayPlay(CCObject *pObj)
{
	CCString *pName = dynamic_cast<CCString *>(pObj);
	if (pName)
	{
		m_nSoundID = SoundManager::sharedSoundManager()->playSFX(CCString::createWithFormat("tutorial/%s", pName->getCString()));
	}
}

void Step::onEnter()
{
	DLayerModal::onEnter();
}

void Step::onExit()
{
    stopSound();
	DLayerModal::onExit();
}


//MARK: - StepEnForce -

StepEnForce::StepEnForce()
:m_pAwContainer(NULL)
,m_pArrowArmature(NULL)
,m_nCurrentSubStep(0)
,m_bSimTouch(false)
,m_isSkipingKeyStep(false)
{
    
}

StepEnForce::~StepEnForce()
{
    CC_SAFE_RELEASE(m_pAwContainer);
	CC_SAFE_RELEASE(m_pArrowArmature);
    UNBINDING_ALL(m_pStepInfo, this);
}


bool StepEnForce::init(tutorial::StepInfo* pStepInfo)
{
    if(!Step::init(pStepInfo)) return false;
    BINDING_ONLY(m_pStepInfo, this, StepEnForce::onStepFinished, D_EVENT_TUTORIAL_STEP_COMPLETE);
    checkIsTrigger();
    return true;
}

void StepEnForce::trigger()
{
    Step::trigger();
	DM_GET_TUTORIAL_MODEL->setIsRunning(true);
    Game::sharedGame()->setTutorialRunning(true);
    nextSubStep();
}

void StepEnForce::nextStep()
{
	DM_GET_TUTORIAL_MODEL->setIsRunning(false);
    Game::sharedGame()->setTutorialRunning(false);
    Step::nextStep();
}

void StepEnForce::onStepFinished()
{
    nextSubStep();
}



void StepEnForce::nextSubStep()
{
    if(!m_pStepInfo->isTrigger()) return;
    simTouch();
    m_nCurrentSubStep++;
    if(m_nCurrentSubStep>m_pStepInfo->getSteps() || m_isSkipingKeyStep)
    {
        nextStep(); //下一步
        return;
    }

    removeAllChildren();

	if (checkWhetherNeedSkipKeyStep())
	{
		return;
	}

    loadUI();
    updateContentInfo();
}

void StepEnForce::loadUI()
{
    //CCB_READER_AND_ADDCHILD(CCString::createWithFormat("ui/ccb/tutorial/tutorial%d%d",m_pStepInfo->getID(),m_nCurrentSubStep)->getCString());
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_xinshou/pl_xinshou_yindao");
	//float offset =  (DESIGN_SIZE.height - CCDirector::sharedDirector()->getWinSize().height) * 0.5;
	//this->setPositionY(offset);
	updateContentId();
}


bool StepEnForce::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAwContainer", CCNode *, this->m_pAwContainer);
    return Step::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}

SEL_MenuHandler StepEnForce::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "buttonCallBack", StepEnForce::buttonCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "finishCallBack", StepEnForce::finishCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "areaCallBack", StepEnForce::areaCallBack);
    return NULL;
}

void StepEnForce::areaCallBack(cocos2d::CCObject* obj)
{
	nextSubStep();
}

void StepEnForce::buttonCallBack(cocos2d::CCObject* obj)
{
    CCMenuItem* pItem = (CCMenuItem*)obj;
    simTouch(pItem);

    scheduleOnce(schedule_selector(StepEnForce::delayToNextSubStep), 0.1f);
}

void StepEnForce::finishCallBack(cocos2d::CCObject* obj)
{
    CCMenuItem* pItem = (CCMenuItem*)obj;
    simTouch(pItem);
    simTouch();
    finishStep();
}

void StepEnForce::simTouch()
{
    if(!m_bSimTouch) return;
    CCTouch* pTouch = new CCTouch();
    pTouch->setTouchInfo(0,m_touchPoint.x,m_touchPoint.y);
    CCLOG("xx = %f yy = %f", pTouch->getLocationInView().x, pTouch->getLocationInView().y);
    Game::sharedGame()->dTouchCancelled(pTouch, NULL);
    Game::sharedGame()->dTouchBegan(pTouch, NULL);
    Game::sharedGame()->dTouchEnded(pTouch, NULL);
    m_bSimTouch=false;
}

void StepEnForce::simTouch(cocos2d::CCMenuItem* pItem)
{
    CCPoint p = ccp(pItem->getContentSize().width*0.5f,pItem->getContentSize().height*0.5f);
    CCPoint screenPos = pItem->convertToWorldSpace(p);
    screenPos = Game::sharedGame()->convertToNodeSpace(screenPos);
    m_touchPoint = CCDirector::sharedDirector()->convertToUI(screenPos);
    m_bSimTouch = true;
}

void StepEnForce::touchMenuCallBack( cocos2d::CCObject* obj )
{
	if (HTTP_CLIENT->getBusyLayer()->isShowing())
	{
		return;
	}

	tutorial::StepDialog *pDialog = DM_GET_TUTORIAL_MODEL->getStepDialog(m_sContentId.c_str());
	if (!pDialog)
	{
		finishStep();
	}

	switch(pDialog->getTouchType())
	{
	case tutorial::kTutorialTouchTypeNext:
		areaCallBack(obj);
		break;
	default:
		buttonCallBack(obj);
		break;
	}
}

void StepEnForce::updateContentInfo()
{
	Step::updateContentInfo();
    
	tutorial::StepDialog *pDialog = DM_GET_TUTORIAL_MODEL->getStepDialog(m_sContentId.c_str());
    if (pDialog == NULL) {
        return;
    }
    
    CCRect touchRect;
	if (m_pTouchMenu)
	{
		touchRect = pDialog->getTouchRect();
		if (touchRect.equals(CCRectMake(0,0,0,0)))
		{
			m_pTouchMenu->setEnabled(false);
		}
		else
		{
			m_pTouchMenu->setEnabled(true);
			m_pTouchMenu->setAnchorPoint(CCPointZero);
			m_pTouchMenu->setPosition(touchRect.origin);
			m_pTouchMenu->setContentSize(touchRect.size);
			m_pTouchMenu->setTarget(this, menu_selector(StepEnForce::touchMenuCallBack));
			PL_MOVE_WITH_RESOLUTION_DIFF(m_pTouchMenu, pDialog->getTouchAjustFactor());
            CCLog("TouchAjustFactor %f", pDialog->getTouchAjustFactor());
            CCLog("original Y:%f, current Y:%f", touchRect.origin.y, m_pTouchMenu->getPositionY());
#if TURORIAL_DEBUG
			m_pTouchMenu->getParent()->setZOrder(100);

			CCLayerColor* pDebugLayer = CCLayerColor::create(ccc4(255,0,0,128));
			m_pTouchMenu->getParent()->getParent()->addChild(pDebugLayer, 10);
			pDebugLayer->setContentSize(touchRect.size);
			pDebugLayer->setPosition(ccp(touchRect.origin.x, m_pTouchMenu->getPositionY()));
#endif 
		}
	}

    if(m_pAwContainer
		&& m_pTouchMenu 
		&& m_pTouchMenu->isEnabled() 
		&& m_pTouchMenu->getContentSize().equals(DESIGN_SIZE) == false)
	{
		if (m_pArrowArmature)
		{
			m_pArrowArmature->removeFromParentAndCleanup(true);
			CC_SAFE_RELEASE_NULL(m_pArrowArmature);
		}
		
		 m_pArrowArmature = ResourceManager::sharedResourceManager()->getUIAnimate("tutorial_finger");
		 if (m_pArrowArmature)
		 {
			 CC_SAFE_RETAIN(m_pArrowArmature);
			 m_pAwContainer->addChild(m_pArrowArmature);
			 CCPoint destination = ccp(m_pTouchMenu->getPositionX() + m_pTouchMenu->getContentSize().width * 0.5, m_pTouchMenu->getPositionY() + m_pTouchMenu->getContentSize().height * 0.5);
			 CCPoint positon = Tutorial::sharedTutorial()->getLastFingerPositon();
			 if (positon.equals(CCPointZero))
			 {
				 positon = m_pLeftLabelMark->getParent()->convertToWorldSpace(m_pLeftLabelMark->getPosition());
			 }
			 m_pAwContainer->setPosition(positon);
			 CCActionInterval* delayAction = CCDelayTime::create( (pDialog->getDelay() - 0.0f > FLT_EPSILON) ? pDialog->getDelay() : 0.0f);;
			 m_pAwContainer->runAction(CCSequence::create(delayAction, CCMoveTo::create(0.5f, destination), CCCallFunc::create(this, callfunc_selector(StepEnForce::runArrowTapAction)), NULL));
			 Tutorial::sharedTutorial()->setLastFingerPositon(destination);
		 }
	}
	else
	{
		Tutorial::sharedTutorial()->setLastFingerPositon(CCPointZero);
	}
    
  //  if (m_pMaskNode && m_pTouchMenu && pDialog->getTouchType() == tutorial::kTutorialTouchTypePassToScene) {
  //      CCClippingNode* pClip = CCClippingNode::create();
  //      pClip->setInverted(true);
  //      pClip->setAlphaThreshold(0.0f);//使用CCDrawNode时填1.0f
  //      CCSprite* bgSprite = CCSprite::create("PL_ui/mask/tutorial_mask_bg.png");
  //      bgSprite->setOpacity(150);
  //      bgSprite->setAnchorPoint(CCPointZero);
  //      bgSprite->setPosition(CCPointZero);
  //      pClip->addChild(bgSprite);
  //      
		///*CCDrawNode* pDrawNode = CCDrawNode::create();
		//CCPoint points[4] = {ccp(-touchRect.size.width * 0.5, touchRect.size.height * 0.5),
		//ccp(touchRect.size.width * 0.5, touchRect.size.height * 0.5),
		//ccp(touchRect.size.width * 0.5, -touchRect.size.height * 0.5),
		//ccp(-touchRect.size.width * 0.5, -touchRect.size.height * 0.5)
		//};
		//ccColor4F yellow = {1,1,0,1};
		//pDrawNode->drawPolygon(points, 4, yellow, 0, yellow);
		//pDrawNode->setPosition(ccp(touchRect.origin.x + touchRect.size.width * 0.5, touchRect.origin.y + touchRect.size.height * 0.5));*/
		//// pClip->setStencil(pDrawNode);
		//
		////CCNode* stencilNode = CCNode::create();
		//CCScale9Sprite* mask = CCScale9Sprite::create("PL_ui/mask/tutorial_mask.png");
		////CCSprite* mask = CCSprite::create("ui/mask/tutorial_mask.png");
		//mask->setContentSize(touchRect.size);
		//mask->setAnchorPoint(CCPointZero);
		//mask->setPosition(ccp(touchRect.origin.x, m_pTouchMenu->getPositionY()));
		////stencilNode->addChild(mask);
		//pClip->setStencil(mask);
  //      m_pMaskNode->addChild(pClip);
  //  }
    
    if (m_pTouchContinue) {
        m_pTouchContinue->setVisible(pDialog->getTouchType() == tutorial::kTutorialTouchTypeNext);
    }

    if (pDialog->getDelay() - 0.0f > FLT_EPSILON) {
        m_pRootNode->setVisible(false);
        m_pRootNode->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(pDialog->getDelay()), CCShow::create()));
    }

	//HTTP_CLIENT->getBusyLayer()->showBusy();
}

void StepEnForce::updateContentId()
{
	m_sContentId = CCString::createWithFormat("%d",m_pStepInfo->getID() * 10 + m_nCurrentSubStep)->getCString();
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_TUTORIAL_SUBSTEP, m_sContentId);
	CCUserDefault::sharedUserDefault()->purgeSharedUserDefault();
}

void StepEnForce::delayToNextSubStep( float t )
{
	nextSubStep();
	unschedule(schedule_selector(StepEnForce::update));
}

bool StepEnForce::checkWhetherNeedSkipKeyStep()
{
	if (m_pStepInfo->getKeyStepId() != 0)
	{
		int userStep = DM_GET_TUTORIAL_MODEL->getLastLocalUserStepId();
		CCLOG("key step:%d, step id: %d", m_pStepInfo->getKeyStepId(), m_pStepInfo->getID());
		if (userStep > m_pStepInfo->getKeyStepId())
		{
			nextStep();
			m_isSkipingKeyStep = true;
			return true;
		}
	}

	return false;
}

void StepEnForce::runArrowTapAction()
{
	if (m_pArrowArmature)
	{
		m_pArrowArmature->getAnimation()->play("ani2");
	}
}






//MARK: - StepPrompt -

StepPrompt::StepPrompt()
:gotoNav(false)
,m_pGoButton(NULL)
,m_pCloseButton(NULL)
,m_pIconContainer(NULL)
{
    
}

StepPrompt::~StepPrompt()
{
    UNBINDING_ALL(m_pStepInfo, this);
	CC_SAFE_RELEASE(m_pGoButton);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pIconContainer);
}

bool StepPrompt::init(tutorial::StepInfo* pStepInfo)
{
    if(!Step::init(pStepInfo)) return false;
    BINDING_ONLY(m_pStepInfo, this, StepPrompt::onStepFinished, D_EVENT_TUTORIAL_STEP_COMPLETE);
    checkIsTrigger();
    return true;
}

void StepPrompt::trigger()
{
    Step::trigger();
    loadUI();
    updateContentInfo();
}


void StepPrompt::loadUI()
{
    //CCB_READER_AND_ADDCHILD(CCString::createWithFormat("ui/ccb/tutorial/tutorial%d",m_pStepInfo->getID() * 10 + 1)->getCString());
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_xinshou/pl_xinshou_gongneng");
    m_sContentId = CCString::createWithFormat("%d",m_pStepInfo->getID() * 10 + 1)->getCString();

	m_pCloseButton->getButton()->setTarget(this, menu_selector(StepPrompt::closeCallBack));
	m_pGoButton->getButton()->setTarget(this, menu_selector(StepPrompt::goCallBack));
}


bool StepPrompt::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGoButton", DButton*,  this->m_pGoButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton*, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconContainer", CCNode*, this->m_pIconContainer);

	return Step::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}

SEL_MenuHandler StepPrompt::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    /*CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", StepPrompt::closeCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goCallBack", StepPrompt::goCallBack);*/
    return NULL;
}

void StepPrompt::closeCallBack(cocos2d::CCObject* obj)
{
    finishStep();
}

void StepPrompt::goCallBack(cocos2d::CCObject* obj)
{
    gotoNav = true;
    finishStep();
}

void StepPrompt::onStepFinished()
{
    if(gotoNav)
    {
       // navigation::NavigationNode* pNav = DM_GET_NAVIGATION_MODEL->getNavigationNodeById(m_pStepInfo->getNavId());
       Game::sharedGame()->NavigateTo((navigation::NavigationType)(m_pStepInfo->getNavId()));
    }
    nextStep();
}

void StepPrompt::updateContentInfo()
{
	Step::updateContentInfo();

	tutorial::StepDialog *pDialog = DM_GET_TUTORIAL_MODEL->getStepDialog(m_sContentId.c_str());
	if (pDialog == NULL) {
		return;
	}

	if (m_pIconContainer)
	{
		m_pIconContainer->removeAllChildren();
		CCSprite* pIcon = CCSprite::create(pDialog->getPromptIconPath().c_str());
		if (pIcon)
		{
			m_pIconContainer->addChild(pIcon);
		}
	}
}



