//
//  LotteryEffect.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "LotteryEffect.h"
#include "../../../managers/ResourceManager.h"
#include "../warrior/WarriorInfo.h"
#include "utils/SceneUtil.h"

#define  ChangeBoneDisplayType(_ani_, _boneName_, _type_) \
	{\
sp::DecorativeDisplay* display = (sp::DecorativeDisplay* )_ani_->getBone(_boneName_)->getDisplayManager()->getDecorativeDisplayList()->objectAtIndex(0);\
display->getDisplayData()->setDisplayType(_type_);\
}

const char * NameGradeFiles[] = {NULL, "PL_ui/all/all_name_lv_bg.png", "PL_ui/all/all_name_lan_bg.png", "PL_ui/all/all_name_zi_bg.png", "PL_ui/all/all_name_jin_bg.png"};

USING_NS_CC;
using namespace sp;

LotteryEffect::LotteryEffect()
:m_pNode(NULL)
,m_pWarriorBaseInfo(NULL)
,m_pCloseButton(NULL)
// ,m_pName(NULL)
// ,m_pNum(NULL)
// ,m_pSpeed(NULL)
// ,m_pHp(NULL)
// ,m_pDefence(NULL)
// ,m_pAttack(NULL)
// ,m_pSoulMark(NULL)
,m_pCallBack(NULL)
,m_pTarget(NULL)
//,m_pShareButton(NULL)
,m_pCheckButton(NULL)
,m_pPopupNode(NULL)
,m_eType(card::kCardTypeEasy)
,m_pCard(NULL)
,m_pDBtnContinue(NULL)
,m_pGradeWarrior(NULL)
,m_pDelegate(NULL)
//,m_pLottery(NULL)
,m_pWarriorShow(NULL)
,m_pBtnNode(NULL)
,m_pMenuItem(NULL)
,m_bReady(false)
,m_bAnimationReady(false)
,m_pLoader(NULL)
,m_uSoulNum(0)
,m_pNodeOne(NULL)
,m_pNodeTen(NULL)
,m_pNodeBack(NULL)
,m_pNodeGoon(NULL)
,m_pCheckButtonTen(NULL)
,m_pWarriorInfoView(NULL)
,m_pNodeRest(NULL)
,m_pNodeNext(NULL)
,m_pBMFRest(NULL)
,m_pNodeTips(NULL)
,m_pGrade(NULL)
,m_pGradeName(NULL)
,m_pTTFName(NULL)
,m_pHeroOwned(NULL)
,m_pZizhiNum(NULL)
{
//    D_RESET_C_ARRAY(m_pNodeGrade, WARRIOR_GRADE_NUM);
}

LotteryEffect::~LotteryEffect()
{
    CC_SAFE_RELEASE(m_pNode);
    
    CC_SAFE_RELEASE(m_pCloseButton);
//     CC_SAFE_RELEASE(m_pName);
//     CC_SAFE_RELEASE(m_pNum);
//     
//     CC_SAFE_RELEASE(m_pSpeed);
//     CC_SAFE_RELEASE(m_pHp);
//     CC_SAFE_RELEASE(m_pDefence);
//     CC_SAFE_RELEASE(m_pAttack);
    CC_SAFE_RELEASE(m_pWarriorShow);
//	CC_SAFE_RELEASE(m_pLottery);
//    CC_SAFE_RELEASE(m_pSoulMark);
//	CC_SAFE_RELEASE(m_pShareButton);
	CC_SAFE_RELEASE(m_pCheckButton);
    
    CC_SAFE_RELEASE(m_pTarget);

	CC_SAFE_RELEASE(m_pCard);
	CC_SAFE_RELEASE(m_pDBtnContinue);
	CC_SAFE_RELEASE(m_pGradeWarrior);
	CC_SAFE_RELEASE(m_pBtnNode);
	CC_SAFE_RELEASE(m_pLoader);
//	D_SAFE_RELEASE_C_ARRAY(m_pNodeGrade, WARRIOR_GRADE_NUM);
	CC_SAFE_RELEASE(m_pNodeOne);
	CC_SAFE_RELEASE(m_pNodeTen);
	CC_SAFE_RELEASE(m_pNodeBack);
	CC_SAFE_RELEASE(m_pNodeGoon);
	CC_SAFE_RELEASE(m_pCheckButtonTen);
	CC_SAFE_RELEASE(m_pNodeRest);
	CC_SAFE_RELEASE(m_pNodeNext);
	CC_SAFE_RELEASE(m_pBMFRest);
	CC_SAFE_RELEASE(m_pNodeTips);
	CC_SAFE_RELEASE(m_pGrade);
	CC_SAFE_RELEASE(m_pGradeName);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pHeroOwned);
	CC_SAFE_RELEASE(m_pZizhiNum);
}

LotteryEffect* LotteryEffect::create(warrior::WarriorCurInfo* pInfo, card::CardType eType)
{
    LotteryEffect* pRet = new LotteryEffect();
    if(pRet&& pRet->init(pInfo, eType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

LotteryEffect* LotteryEffect::create(warrior::SoulInfo* pInfo, card::CardType eType)
{
    LotteryEffect* pRet = new LotteryEffect();
    if(pRet&& pRet->init(pInfo, eType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

LotteryEffect* LotteryEffect::create()
{
	LotteryEffect* pRet = new LotteryEffect();
	if(pRet&& pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return NULL;
}

bool LotteryEffect::init()
{
	if(!DPopup::init()) return false;

	m_pPopupNode = CCNode::create();
	addChild(m_pPopupNode, 2, 0);

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorGradeBg", WarriorGradeBgLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("CardButton", CardButtonLoader::loader());
	CCB_READER_BY_CUSTOM_LOADER("PL_ui/ccb/pl_shop/PL_beibao_chouka_3ji", pNodeLoaderLibrary, m_pNode);
	//    CCB_READER("PL_ui/ccb/pl_shop/PL_beibao_chouka_3ji", m_pNode);
	CC_SAFE_RETAIN(m_pNode);
	addChild(m_pNode);
	m_pBtnNode->setVisible(false);

	m_pWarriorShow =  ResourceManager::sharedResourceManager()->getUIAnimateOnly("lottery0",sp::ZORDER);
	CC_SAFE_RETAIN(m_pWarriorShow);

	m_pCloseButton->setTarget(this, menu_selector(LotteryEffect::closeCallBack));
	//	m_pShareButton->getButton()->setTarget(this, menu_selector(LotteryEffect::shareCallBack));
	m_pCheckButton->getButton()->setTarget(this, menu_selector(LotteryEffect::checkCallBack));

	return true;
}

bool LotteryEffect::init(warrior::SoulInfo* pInfo, card::CardType eType)
{
	m_uSoulNum = pInfo->getNum();
    if(!init(pInfo->getBaseInfo(), eType)) return false;
//     m_pNum->setString(CCString::createWithFormat("%d",pInfo->getNum())->getCString());
//     m_pNum->setVisible(true);
//     m_pSoulMark->setVisible(true);
    return true;
}

bool LotteryEffect::init(warrior::WarriorCurInfo* pInfo, card::CardType eType)
{
	m_uSoulNum = 0;
    if(!init(pInfo->getBaseInfo(), eType)) return false;
//     m_pNum->setString("");
//     m_pNum->setVisible(false);
//     m_pSoulMark->setVisible(false);
    return true;
}


bool LotteryEffect::init(warrior::WarriorBaseInfo* pInfo, card::CardType eType)
{
    if(!DPopup::init()) return false;

	m_eType = eType;

	m_pPopupNode = CCNode::create();
	addChild(m_pPopupNode, 2, 0);
	m_pWarriorBaseInfo = pInfo;
    

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorGradeBg", WarriorGradeBgLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("CardButton", CardButtonLoader::loader());
	CCB_READER_BY_CUSTOM_LOADER("PL_ui/ccb/pl_shop/PL_beibao_chouka_3ji", pNodeLoaderLibrary, m_pNode);
//    CCB_READER("PL_ui/ccb/pl_shop/PL_beibao_chouka_3ji", m_pNode);
    CC_SAFE_RETAIN(m_pNode);
	addChild(m_pNode);
	m_pBtnNode->setVisible(false);
    
    m_pCloseButton->setTarget(this, menu_selector(LotteryEffect::closeCallBack));
//	m_pShareButton->getButton()->setTarget(this, menu_selector(LotteryEffect::shareCallBack));
	m_pCheckButton->getButton()->setTarget(this, menu_selector(LotteryEffect::checkCallBack));

//     m_pName->setString(pInfo->getWarriorName().c_str());
//     m_pAttack->setString(CCString::createWithFormat("%d",pInfo->getBasicAttack())->getCString());
//     m_pDefence->setString(CCString::createWithFormat("%d",pInfo->getBasicDefence())->getCString());
//     m_pHp->setString(CCString::createWithFormat("%d",pInfo->getBasicHp())->getCString());
//     m_pSpeed->setString(CCString::createWithFormat("%d",pInfo->getBasicSpeed())->getCString());
        
//     m_pLottery =  ResourceManager::sharedResourceManager()->getUIAnimate("lottery1",sp::BATCHNODE_ZORDER);
// 	m_pLottery->setPosition(D_DESIGN_POINT_CENTER);
// 	m_pLottery->getAnimation()->addObserver(this, SPNotification_callfunc_selector(LotteryEffect::animateEnd), COMPLETE);
//     addChild(m_pLottery);
// 	CC_SAFE_RETAIN(m_pLottery);

//     if(pInfo->getColorGrade()>warrior::kWarGradeBlue)
//     {
//         pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("lottery_lightning",sp::BATCHNODE_ZORDER);
//         pArmature->setPosition(D_DESIGN_POINT_CENTER);
//         addChild(pArmature);
//     }

    m_pWarriorShow =  ResourceManager::sharedResourceManager()->getUIAnimateOnly("lottery0",sp::ZORDER);
	m_pWarriorShow->getAnimation()->addObserver(this, SPNotification_callfunc_selector(LotteryEffect::animateEnd), COMPLETE);
// 	for (int i = 0; i < m_pWarriorShow->getBoneList()->count(); i++)
// 	{
// 		sp::Bone *pBone = (sp::Bone *)m_pWarriorShow->getBoneList()->objectAtIndex(i);
// 		CCLOG("%s", pBone->getName().c_str());
// 	}
	

//	m_pWarriorShow->getBone("grade")->replaceDisplayAt(CCSprite::create(CCString::createWithFormat("PL_ui/all/all_bg_grade%d.png", pInfo->getColorGrade())->getCString()), 0);
	m_pWarriorShow->getBone("grade2")->replaceDisplayAt(CCSprite::create(CCString::createWithFormat("PL_ui/all/heroGradeFont%d.png", pInfo->getColorGrade())->getCString()), 0);
//	ChangeBoneDisplayType(m_pWarriorShow, "grade", SP_DISPLAY_SPRITE);
//	CCSprite *pNameGrade = CCSprite::create(NameGradeFiles[pInfo->getColorGrade()]);
//	m_pWarriorShow->getBone("name")->replaceDisplayAt(pNameGrade, 0);
//	ChangeBoneDisplayType(m_pWarriorShow, "grade2", SP_DISPLAY_SPRITE);

// 	m_pLoader = ResourceManager::sharedResourceManager()->getUnitAnimateAsync(m_pWarriorBaseInfo->getWarriorId(), this);
// 	m_pLoader->setDelegate(this);
// 	CC_SAFE_RETAIN(m_pLoader);//release in delegate

	CCNode *pIcon = ResourceManager::sharedResourceManager()->getWarriorIcon(m_pWarriorBaseInfo->getWarriorId());
	pIcon->setAnchorPoint(ccp(0.5f,0.5f));
	pIcon->setPositionY(25);
	m_pWarriorShow->getBone("hero")->replaceDisplayAt(pIcon, 0);

// 	CCLabelTTF *pTTFName = CCLabelTTF::create();
// 	pTTFName->setString(pInfo->getWarriorName().c_str());
// 	pTTFName->setFontSize(D_FONT_SIZE_DEFAULT);
// 	pTTFName->setPosition(ccpMult(pNameGrade->getContentSize(), 0.5));
// 	pNameGrade->addChild(pTTFName);
//	m_pWarriorShow->getBone("name")->replaceDisplayAt(pTTFName, 0);
//	ChangeBoneDisplayType(m_pWarriorShow, "name", SP_DISPLAY_SPRITE);
// 	m_pWarriorShow->getBone("soul")->setOpacity(m_uSoulNum > 0 ? 255 : 0);
// 	
// 
// 	if (m_uSoulNum)
// 	{
// 		CCLabelBMFont *pBMFNum = CCLabelBMFont::create(CCString::createWithFormat("x%u", m_uSoulNum)->getCString(), "fonts/number_charge.fnt");
// 		SPContainer *pContainer = SPContainer::create(sp::ZORDER);
// 		pBMFNum->setAnchorPoint(ccp(0,0.5));
// 		pContainer->addChild(pBMFNum);
// 		m_pWarriorShow->getBone("aum")->replaceDisplayAt(pContainer, 0);
// 	}
	
	m_pWarriorShow->setPosition(D_DESIGN_POINT_CENTER);
	PL_MOVE_WITH_RESOLUTION_DIFF(m_pWarriorShow, -0.75);
    addChild(m_pWarriorShow, 1, -1);
    CC_SAFE_RETAIN(m_pWarriorShow);
    
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setTouchEnabled(false);
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu);
    
    m_pMenuItem = DMenuItemImageSound::create(this, menu_selector(LotteryEffect::menuCallBack));
    m_pMenuItem->setContentSize(DESIGN_SIZE);
    m_pMenuItem->setAnchorPoint(CCPointZero);
    pMenu->addChild(m_pMenuItem);


	m_pCard->setCard(DM_GET_LOTTERY_MODEL->getCardInfoByType(m_eType));
// 	for (int i = 0; i < WARRIOR_GRADE_NUM; i++)
// 	{
// 		m_pNodeGrade[i]->setVisible(false);
// 	}
// 	m_pNodeGrade[WARRIOR_GRADE_NUM - pInfo->getColorGrade()]->setVisible(true);

	m_pDBtnContinue->getButton()->setTarget(this, menu_selector(LotteryEffect::onceMoreCallBack));

//	m_pGradeWarrior->setWarriorGrade(m_pWarriorBaseInfo->getColorGrade());

    setOpacity(0);

	m_pNodeOne->setVisible(m_eType != card::kCardTypeHard10 && m_eType != card::kCardTypeSpecial10);
	m_pNodeTen->setVisible(m_eType == card::kCardTypeHard10 || m_eType == card::kCardTypeSpecial10);
	m_pNodeBack->setVisible(m_pNodeOne->isVisible());
	m_pNodeGoon->setVisible(m_pNodeTen->isVisible());
	m_pCheckButtonTen->getButton()->setTarget(this, menu_selector(LotteryEffect::checkCallBack));

	updateRest();

	m_pGrade->setWarriorGrade(m_pWarriorBaseInfo->getColorGrade());
	m_pGradeName->setWarriorGrade(m_pWarriorBaseInfo->getColorGrade());
	m_pTTFName->setString(m_pWarriorBaseInfo->getWarriorName().c_str());
	if(m_pZizhiNum)
	{
		m_pZizhiNum->setString(CCString::createWithFormat("%d",m_pWarriorBaseInfo->getQuality())->getCString());
	}

	m_bReady = true;
	m_pMenuItem->setVisible(false);
	m_bAnimationReady = true;
	m_pWarriorShow->getAnimation()->playByIndex(0);
	m_pBtnNode->setVisible(true);

	m_pGrade->hideGradeLabel();
	resetSchedule();
    return true;
}

void LotteryEffect::setData(warrior::WarriorCurInfo*pInfo, card::CardType eType)
{
	m_uSoulNum = 0;
	setData(pInfo->getBaseInfo() , eType);
}

void LotteryEffect::setData(warrior::SoulInfo*pInfo, card::CardType eType)
{
	m_uSoulNum = pInfo->getNum();
	setData(pInfo->getBaseInfo() , eType);
}

void LotteryEffect::setData(warrior::WarriorBaseInfo*pInfo, card::CardType eType)
{
	m_eType = eType;
	m_pWarriorBaseInfo = pInfo;

	m_pWarriorShow->getAnimation()->addObserver(this, SPNotification_callfunc_selector(LotteryEffect::animateEnd), COMPLETE);
	m_pWarriorShow->getBone("grade2")->replaceDisplayAt(CCSprite::create(CCString::createWithFormat("PL_ui/all/heroGradeFont%d.png", pInfo->getColorGrade())->getCString()), 0);
	
	CCNode *pIcon = ResourceManager::sharedResourceManager()->getWarriorIcon(m_pWarriorBaseInfo->getWarriorId());
	pIcon->setAnchorPoint(ccp(0.5f,0.5f));
	pIcon->setPositionY(25);
	m_pWarriorShow->getBone("hero")->replaceDisplayAt(pIcon, 0);

	m_pWarriorShow->setPosition(D_DESIGN_POINT_CENTER);
	PL_MOVE_WITH_RESOLUTION_DIFF(m_pWarriorShow, -0.75);
	//if(m_pWarriorShow->getParent()) m_pWarriorShow->removeFromParent();
	addChild(m_pWarriorShow, 1, -1);
	//CC_SAFE_RETAIN(m_pWarriorShow);


	CCMenu* pMenu = CCMenu::create();
	pMenu->setTouchEnabled(false);
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(CCPointZero);
	addChild(pMenu);

	m_pMenuItem = DMenuItemImageSound::create(this, menu_selector(LotteryEffect::menuCallBack));
	m_pMenuItem->setContentSize(DESIGN_SIZE);
	m_pMenuItem->setAnchorPoint(CCPointZero);
	pMenu->addChild(m_pMenuItem);


	m_pCard->setCard(DM_GET_LOTTERY_MODEL->getCardInfoByType(m_eType));
	m_pDBtnContinue->getButton()->setTarget(this, menu_selector(LotteryEffect::onceMoreCallBack));

	setOpacity(0);

	m_pNodeOne->setVisible(m_eType != card::kCardTypeHard10 && m_eType != card::kCardTypeSpecial10);
	m_pNodeTen->setVisible(m_eType == card::kCardTypeHard10 || m_eType == card::kCardTypeSpecial10);
	m_pNodeBack->setVisible(m_pNodeOne->isVisible());
	m_pNodeGoon->setVisible(m_pNodeTen->isVisible());
	m_pCheckButtonTen->getButton()->setTarget(this, menu_selector(LotteryEffect::checkCallBack));

	updateRest();

	m_pGrade->setWarriorGrade(m_pWarriorBaseInfo->getColorGrade());
	m_pGradeName->setWarriorGrade(m_pWarriorBaseInfo->getColorGrade());
	m_pTTFName->setString(m_pWarriorBaseInfo->getWarriorName().c_str());
	if(m_pZizhiNum)
	{
		m_pZizhiNum->setString(CCString::createWithFormat("%d",m_pWarriorBaseInfo->getQuality())->getCString());
	}

	m_bReady = true;
	m_pMenuItem->setVisible(false);
	m_bAnimationReady = true;
	m_pWarriorShow->getAnimation()->playByIndex(0);
	m_pBtnNode->setVisible(true);

	m_pGrade->hideGradeLabel();
	resetSchedule();
}

void LotteryEffect::setWarriorExistsState(bool isExist)
{
	m_pHeroOwned->setVisible(isExist);
}



void LotteryEffect::onExit()
{
	DPopup::onExit();
	//m_pDelegate->playHeroSound(0);
}

void LotteryEffect::onBackKey()
{
	/************************************************************************/
	/* 同关闭按钮@fengkerong.billy                                                            */
	/************************************************************************/
	//关闭当前页
	closeCallBack(NULL);

	//if (m_pPopupNode->getChildrenCount() > 0)
	//{
	//	m_pPopupNode->removeAllChildren();
	//	return;
	//}
	//DPopup::onBackKey();	
}

void LotteryEffect::setCallBack(cocos2d::CCObject *pTarget,cocos2d::SEL_CallFuncO pCallBack)
{
    CC_SAFE_RETAIN(pTarget);
    CC_SAFE_RELEASE(m_pTarget);
    m_pTarget = pTarget;
    m_pCallBack = pCallBack;
}

void LotteryEffect::callback()
{
    if(m_pTarget) (m_pTarget->*m_pCallBack)(this);
}


bool LotteryEffect::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", CCMenuItem *, this->m_pCloseButton);
    
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNum", CCLabelBMFont *, this->m_pNum);
//     
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelBMFont *, this->m_pSpeed);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelBMFont *, this->m_pHp);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelBMFont *, this->m_pDefence);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelBMFont *, this->m_pAttack);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulMark", CCSprite *, this->m_pSoulMark);
//	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pShareButton", DButton *, this->m_pShareButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCheckButton", DButton *, this->m_pCheckButton);
    
//	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pNodeGrade", CCNode *, m_pNodeGrade, WARRIOR_GRADE_NUM);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCard", CardButton *, this->m_pCard);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnContinue", DButton *, this->m_pDBtnContinue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeWarrior", WarriorGradeBg *, this->m_pGradeWarrior);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnNode", CCNode *, m_pBtnNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeOne", CCNode *, m_pNodeOne);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeTen", CCNode *, m_pNodeTen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBack", CCNode *, m_pNodeBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeGoon", CCNode *, m_pNodeGoon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCheckButtonTen", DButton *, this->m_pCheckButtonTen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRest", CCNode *, this->m_pNodeRest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNext", CCNode *, this->m_pNodeNext);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRest", CCLabelBMFont *, this->m_pBMFRest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeTips", CCNode *, this->m_pNodeTips);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", WarriorGradeBg *, this->m_pGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeName", WarriorGradeBg *, this->m_pGradeName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFWarriorName", CCLabelTTF *, this->m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHeroOwned", CCSprite *, this->m_pHeroOwned);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum", CCLabelBMFont* , m_pZizhiNum);
    return false;
}

void LotteryEffect::closeCallBack(cocos2d::CCObject*)
{
	if (!m_bAnimationReady)
	{
		return;
	}
    callback();

	//新增关闭接口(尽在限时英雄单抽时调用，用于释放SpecialLottery界面)
	if(m_pDelegate && m_eType == card::kCardTypeSpecial)
		m_pDelegate->onClose();

    removeFromParent();
	
}

void LotteryEffect::shareCallBack(cocos2d::CCObject*)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHARE, CCStringMake(D_LOCAL_STRING("ShareMessage")));

	callback();
	removeFromParent();
}

void LotteryEffect::checkCallBack(cocos2d::CCObject*)
{
	if (!m_pWarriorBaseInfo)
	{
		return;
	}
	
	m_pWarriorInfoView = WarriorInfoView::create(WarriorInfoView::kTypeCloseOnly);
	m_pWarriorInfoView->setDelegate(this);
	m_pWarriorInfoView->setWarrior(m_pWarriorBaseInfo);
	
	m_pPopupNode->addChild(m_pWarriorInfoView);
	releaseSchedule();
}

void LotteryEffect::menuCallBack(cocos2d::CCObject*)
{
	m_bReady = true;
// 	if (!m_bAnimationReady)
// 	{
// 		return;
// 	}

	m_pMenuItem->setVisible(false);
// 	m_pLottery->getAnimation()->removeAllObservers(this);
// 	m_pLottery->setVisible(false);
	//	m_pLottery->getAnimation()->playByIndex(1);
	m_bAnimationReady = true;
	m_pWarriorShow->getAnimation()->playByIndex(0);
//     m_pNode->removeFromParent();
//     addChild(m_pNode);
	m_pBtnNode->setVisible(true);
	resetSchedule();
}

void LotteryEffect::loadingFinished(ArmatureLoader *pLoader)
{
	CCNode *pNode = pLoader->getArmature();
	pNode->setAnchorPoint(ccp(0.5, 0.5));
	pNode->setPosition(ccp(0, -100));
	pNode->setScale(2.0f);
//	m_pWarriorShow->getBone("hero")->replaceDisplayAt(pNode, 0);
	CC_SAFE_RELEASE_NULL(m_pLoader);
	m_bAnimationReady = true;
	if (m_bReady)
	{
		menuCallBack(NULL);
	}
}

void LotteryEffect::closeInfoView(WarriorViewType type)
{
	WarriorDelegate::closeInfoView(type);
	m_pWarriorInfoView->removeFromParent();
	m_pWarriorInfoView = NULL;
	resetSchedule();
}

void LotteryEffect::animateEnd(sp::Armature * target, const char* event,const char* data)
{
// 	if (target == m_pLottery)
// 	{
// 		menuCallBack(NULL);
// 	}
// 	else if (target == m_pWarriorShow)
// 	{
// 		m_bAnimationReady = true;
// 	}
}

void LotteryEffect::onceMoreCallBack(CCObject *pObj)
{
	if (m_pDelegate)
	{
		m_pDelegate->onceMore(this, m_eType);
	}
}

void LotteryEffect::releaseSchedule()
{
	if (m_eType == card::kCardTypeHard10 || m_eType == card::kCardTypeSpecial10)
	{
		unschedule(schedule_selector(LotteryEffect::callBackAutoClose));
	}
}

void LotteryEffect::resetSchedule()
{
	if (m_eType == card::kCardTypeHard10 || m_eType == card::kCardTypeSpecial10)
	{
		scheduleOnce(schedule_selector(LotteryEffect::callBackAutoClose), 5.f);
	}
}

void LotteryEffect::callBackAutoClose(float dt)
{
	releaseSchedule();
	closeCallBack(NULL);
}

void LotteryEffect::updateRest()
{
	if (m_eType != card::kCardTypeHard && m_eType != card::kCardTypeSpecial)
	{
		m_pNodeTips->setVisible(false);
		return;
	}
	m_pNodeTips->setVisible(true);
	unsigned int rest = DM_GET_LOTTERY_MODEL->getRestForOrange();
	//处理限时英雄
	if(m_eType == card::kCardTypeSpecial) rest = DM_GET_LOTTERY_MODEL->getRestForSpecial();

	m_pNodeNext->setVisible(rest == 0);
	m_pNodeRest->setVisible(rest != 0);
	m_pBMFRest->setString(D_CSTRING_FROM_UINT(rest));
}
///////////////////////十连抽页面

LotteryEffectTen::LotteryEffectTen()
	: m_pDBtnOnceMore(NULL)
	, m_pDBtnTenthMore(NULL)
	, m_pCloseButton(NULL)
	, m_pWarriorShow(NULL)
	, m_pArr(NULL)
	, m_uIdx(0)
	, m_pNodeDrop(NULL)
	,m_pCallBack(NULL)
	,m_pTarget(NULL)
	,m_pNodeRest(NULL)
	,m_pNodeNext(NULL)
	,m_pBMFRest(NULL)
	,m_pPointCost(NULL)
	,m_pItemCost(NULL)
	,m_bSpecial(false)
{
	D_RESET_C_ARRAY(m_pWarrior, LOTTERY_TEN_MAX);
}

LotteryEffectTen::~LotteryEffectTen()
{
	CC_SAFE_RELEASE(m_pDBtnOnceMore);
	CC_SAFE_RELEASE(m_pDBtnTenthMore);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pWarriorShow);
	CC_SAFE_RELEASE(m_pArr);
	CC_SAFE_RELEASE(m_pNodeDrop);
	D_SAFE_RELEASE_C_ARRAY(m_pWarrior, LOTTERY_TEN_MAX);
	CC_SAFE_RELEASE(m_pTarget);
	CC_SAFE_RELEASE(m_pNodeRest);
	CC_SAFE_RELEASE(m_pNodeNext);
	CC_SAFE_RELEASE(m_pBMFRest);
	CC_SAFE_RELEASE(m_pPointCost);
	CC_SAFE_RELEASE(m_pItemCost);
}

LotteryEffectTen* LotteryEffectTen::create(bool special)
{
	LotteryEffectTen* pRet = new LotteryEffectTen();
	pRet->m_bSpecial = special;
	if(pRet&& pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return NULL;
}

bool LotteryEffectTen::init()
{
	if(!DPopup::init()) return false;

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorGradeBg", WarriorGradeBgLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorHead", WarriorHeadLoader::loader());
	CCB_READER_BY_CUSTOM_LOADER("PL_ui/ccb/pl_shop/PL_beibao_chouka_3ji_shilianchou", pNodeLoaderLibrary, m_pNodeDrop);
	CC_SAFE_RETAIN(m_pNodeDrop);
	addChild(m_pNodeDrop);
	m_pNodeDrop->setVisible(false);

	m_pDBtnOnceMore->getButton()->setTarget(this, menu_selector(LotteryEffectTen::callBackOnce));
	m_pDBtnTenthMore->getButton()->setTarget(this, menu_selector(LotteryEffectTen::callBackTenth));
	m_pCloseButton->setTarget(this, menu_selector(LotteryEffectTen::closeCallBack));
	
	return true;
}

void LotteryEffectTen::setData(cocos2d::CCArray *pArr)
{
	CC_SAFE_RETAIN(pArr);
	CC_SAFE_RELEASE(m_pArr);
	m_pArr = pArr;

	CCObject *pObj = NULL;
	int idx = 0;
	CCARRAY_FOREACH(pArr, pObj)
	{
		if (idx >= LOTTERY_TEN_MAX)
		{
			break;
		}
		drop::Drop *pDrop = dynamic_cast<drop::Drop *>(pObj);
		if (pDrop == NULL || (pDrop->getType() != drop::kDropTypeWarrior && pDrop->getType() != drop::kDropTypeSoul))
		{
			continue;
		}
		warrior::WarriorBaseInfo *pInfo = NULL;
		if (pDrop->getType() == drop::kDropTypeWarrior)
		{
			pInfo = pDrop->getWarrior()->getBaseInfo();
			m_pWarrior[idx]->setNum(0);
		}
		else
		{
			pInfo = pDrop->getSoul()->getBaseInfo();
			m_pWarrior[idx]->setNum(pDrop->getSoul()->getNum());
		}
		if (!pInfo)
		{
			continue;
		}
		m_pWarrior[idx]->setWarriorBaseInfo(pInfo);
		m_pWarrior[idx]->setIsSoul(pDrop->getType() == drop::kDropTypeSoul);
		m_pWarrior[idx]->setVisible(true);
		idx++;
	}

	for (int i = idx; i < LOTTERY_TEN_MAX; i++)
	{
		m_pWarrior[i]->setVisible(false);
	}

	updateRest();
}

LotteryEffectTen* LotteryEffectTen::create(cocos2d::CCArray *pArr , bool special)
{
	LotteryEffectTen* pRet = new LotteryEffectTen();
	pRet->m_bSpecial = special;
	if(pRet&& pRet->init(pArr))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return NULL;
}
bool LotteryEffectTen::init(cocos2d::CCArray *pArr)
{
	if(!DPopup::init()) return false;

	CC_SAFE_RETAIN(pArr);
	CC_SAFE_RELEASE(m_pArr);
	m_pArr = pArr;

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorGradeBg", WarriorGradeBgLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorHead", WarriorHeadLoader::loader());
	CCB_READER_BY_CUSTOM_LOADER("PL_ui/ccb/pl_shop/PL_beibao_chouka_3ji_shilianchou", pNodeLoaderLibrary, m_pNodeDrop);
	CC_SAFE_RETAIN(m_pNodeDrop);
	addChild(m_pNodeDrop);
	m_pNodeDrop->setVisible(false);

	CCObject *pObj = NULL;
	int idx = 0;
	CCARRAY_FOREACH(pArr, pObj)
	{
		if (idx >= LOTTERY_TEN_MAX)
		{
			break;
		}
		drop::Drop *pDrop = dynamic_cast<drop::Drop *>(pObj);
		if (pDrop == NULL || (pDrop->getType() != drop::kDropTypeWarrior && pDrop->getType() != drop::kDropTypeSoul))
		{
			continue;
		}
		warrior::WarriorBaseInfo *pInfo = NULL;
		if (pDrop->getType() == drop::kDropTypeWarrior)
		{
			pInfo = pDrop->getWarrior()->getBaseInfo();
			m_pWarrior[idx]->setNum(0);
		}
		else
		{
			pInfo = pDrop->getSoul()->getBaseInfo();
			m_pWarrior[idx]->setNum(pDrop->getSoul()->getNum());
		}
		if (!pInfo)
		{
			continue;
		}
		m_pWarrior[idx]->setWarriorBaseInfo(pInfo);
		m_pWarrior[idx]->setIsSoul(pDrop->getType() == drop::kDropTypeSoul);
		m_pWarrior[idx]->setVisible(true);
		idx++;
	}
	
	for (int i = idx; i < LOTTERY_TEN_MAX; i++)
	{
		m_pWarrior[i]->setVisible(false);
	}
	
	m_pDBtnOnceMore->getButton()->setTarget(this, menu_selector(LotteryEffectTen::callBackOnce));
	m_pDBtnTenthMore->getButton()->setTarget(this, menu_selector(LotteryEffectTen::callBackTenth));
	m_pCloseButton->setTarget(this, menu_selector(LotteryEffectTen::closeCallBack));

	//showWarrior();

	updateRest();

	return true;
}

bool LotteryEffectTen::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	D_CCB_ASSIGN_ARRAY("m_pWarrior", WarriorHead *, m_pWarrior, LOTTERY_TEN_MAX);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnOnceMore", DButton *, m_pDBtnOnceMore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnTenthMore", DButton *, m_pDBtnTenthMore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", CCMenuItem *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRest", CCNode *, this->m_pNodeRest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNext", CCNode *, this->m_pNodeNext);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRest", CCLabelBMFont *, this->m_pBMFRest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPointCost", CCNode *, this->m_pPointCost);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCost", CCNode *, this->m_pItemCost);
	return false;
}

void LotteryEffectTen::callBackOnce(CCObject *pObj)
{
	if (m_pDelegate)
	{
		if(m_bSpecial){
			m_pDelegate->onceMore(this, card::kCardTypeSpecial);
		}
		else
		{
			m_pDelegate->onceMore(this, card::kCardTypeHard);
		}	
	}
}

void LotteryEffectTen::callBackTenth(CCObject *pObj)
{
	if (m_pDelegate)
	{
		if(m_bSpecial){
			m_pDelegate->onceMore(this, card::kCardTypeSpecial10);
		}
		else
		{
			m_pDelegate->onceMore(this, card::kCardTypeHard10);
		}
	}

}

void LotteryEffectTen::closeCallBack(cocos2d::CCObject*)
{
	if(m_pTarget) (m_pTarget->*m_pCallBack)(this);
	
	//新增关闭接口
	if(m_pDelegate)
		m_pDelegate->onClose();

	removeFromParent();
}

void LotteryEffectTen::onBackKey()
{
	//关闭当前页
	closeCallBack(NULL);
	//DPopup::onBackKey();
}

void LotteryEffectTen::showWarrior()
{
	warrior::WarriorCurInfo* pInfo = NULL;
	for (unsigned int i = m_uIdx; i < m_pArr->count(); i++)
	{
		drop::Drop *pDrop = dynamic_cast<drop::Drop *>(m_pArr->objectAtIndex(i));
		if (pDrop == NULL || pDrop->getType() == drop::kDropTypeWarrior)
		{
			pInfo = pDrop->getWarrior();//->getBaseInfo();
            if(m_uIdx > 0){
                
				m_pDelegate->playHeroSound(pInfo->getWid());
			} else
			{
				m_pDelegate->setCurWarriorId(pInfo->getWid());
			}
			m_uIdx = i + 1;
			
			break;
		}
	}
	
	if (pInfo)
	{
// 		m_pNodeDrop->setVisible(false);
// 		CC_SAFE_RELEASE(m_pWarriorShow);
// // 		m_pWarriorShow =  ResourceManager::sharedResourceManager()->getUIAnimate("lottery_warriorshow",sp::ZORDER);
// // 		m_pWarriorShow->getAnimation()->addObserver(this, SPNotification_callfunc_selector(LotteryEffectTen::animateEnd), COMPLETE);
// // 		CCSprite* pNode = ResourceManager::sharedResourceManager()->getWarriorIcon50PForLottery(pInfo->getWarriorId());
// // 		m_pWarriorShow->getBone("character")->replaceDisplayAt(pNode, 0);
// // 		m_pWarriorShow->getBone("grade")->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pInfo->getWarriorId()));
// 		m_pWarriorShow =  ResourceManager::sharedResourceManager()->getUIAnimate("lottery0",sp::ZORDER);
// 		m_pWarriorShow->getBone("grade1")->replaceDisplayAt(CCSprite::create(CCString::createWithFormat("PL_ui/all/all_bg_grade%d.png", pInfo->getColorGrade())->getCString()), 0);
// 		m_pWarriorShow->getBone("grade")->replaceDisplayAt(CCSprite::create(NameGradeFiles[pInfo->getColorGrade()]), 0);
// 		CCLabelTTF *pTTFName = CCLabelTTF::create();
// 		pTTFName->setString(pInfo->getWarriorName().c_str());
// 		pTTFName->setFontSize(D_FONT_SIZE_DEFAULT);
// 		m_pWarriorShow->getBone("name")->replaceDisplayAt(pTTFName, 0);
// 		m_pWarriorShow->getAnimation()->addObserver(this, SPNotification_callfunc_selector(LotteryEffectTen::animateEnd), COMPLETE);
// 		CCNode* pNode = ResourceManager::sharedResourceManager()->getUnitAnimate2X(pInfo->getWarriorId());
// 		pNode->setAnchorPoint(ccp(0.5, 0.5));
// 		pNode->setPosition(ccp(0, -100));
// 		m_pWarriorShow->getBone("hero")->replaceDisplayAt(pNode, 0);
// 
// 		m_pWarriorShow->setPosition(D_DESIGN_POINT_CENTER);
// 		addChild(m_pWarriorShow, 1, -1);
		// 		CC_SAFE_RETAIN(m_pWarriorShow);
		LotteryEffect *pEff = LotteryEffect::create(pInfo, card::kCardTypeHard10);
		pEff->setCallBack(this, callfuncO_selector(LotteryEffectTen::effectCallBack));
		pEff->setWarriorExistsState(false);
        pEff->setDelegate(m_pDelegate);
		addChild(pEff);
		PL_MOVE_NODE_ABOVE_CENTER_Y(pEff);
	}
	else
	{
		m_pNodeDrop->setVisible(true);
	}
}

void LotteryEffectTen::animateEnd(sp::Armature * target, const char* event,const char* data)
{
	m_pWarriorShow->removeFromParent();
	showWarrior();
}

void LotteryEffectTen::effectCallBack(CCObject *pObj)
{
	showWarrior();
}
void LotteryEffectTen::setCallBack(cocos2d::CCObject *pTarget,cocos2d::SEL_CallFuncO pCallBack)
{
	CC_SAFE_RETAIN(pTarget);
	CC_SAFE_RELEASE(m_pTarget);
	m_pTarget = pTarget;
	m_pCallBack = pCallBack;
}

void LotteryEffectTen::updateRest()
{

	unsigned int rest = DM_GET_LOTTERY_MODEL->getRestForOrange();
	//处理限时英雄的情况
	if(m_bSpecial) rest = DM_GET_LOTTERY_MODEL->getRestForSpecial();

	m_pNodeNext->setVisible(rest == 0);
	m_pNodeRest->setVisible(rest != 0);
	m_pBMFRest->setString(D_CSTRING_FROM_UINT(rest));

	//处理超级招募令
	unsigned int itemnum = DM_GET_ITEM_MODEL->getItemNum(HARD_CARD_ITEM_ID);
	m_pItemCost->setVisible(itemnum > 0);
	m_pPointCost->setVisible(!m_pItemCost->isVisible());
}