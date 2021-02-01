//
//  Lottery.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Lottery.h"
#include "components/components.h"
#include "../user/UserInfo.h"
#include "../../../managers/GameManager.h"
#include "../../../managers/ResourceManager.h"
#include "../Game.h"
#include "../common/AwardListView.h"
#include <algorithm>
#include "../../../managers/SoundManager.h"

#include "../activity/ActivitySoulExchange.h"

#include "LotteryAnimation.h"


static int sortDropFromCard10(const CCObject* p1, const CCObject* p2)
{
	drop::Drop *pDrop1 = ((drop::Drop *)p1);
	drop::Drop *pDrop2 = ((drop::Drop *)p2);
	if (pDrop1->getType() != pDrop2->getType())
	{
		return pDrop1->getType() > pDrop2->getType();
	}
	else if (pDrop1->getType() == drop::kDropTypeWarrior)
	{
		return pDrop1->getWarrior()->getBaseInfo()->getColorGrade() > pDrop2->getWarrior()->getBaseInfo()->getColorGrade();
	}
	else if (pDrop1->getType() == drop::kDropTypeSoul)
	{
		return pDrop1->getSoul()->getBaseInfo()->getColorGrade() > pDrop2->getSoul()->getBaseInfo()->getColorGrade();
	}
	return 0;
};

USING_NS_CC;
USING_NS_CC_EXT;

Lottery::Lottery()
:m_pLotteryModel(NULL)
,m_pDelegate(NULL)
,m_pEffectNode(NULL)
,m_pNodeMore(NULL)
,m_pDBtnClose(NULL)
,m_pEffect(NULL)
,m_pNodeFree(NULL)
,m_pNodeGold(NULL)
,m_pItemCost(NULL)
,m_pAnimate(NULL)
,m_pNodeRest(NULL)
,m_pNodeNext(NULL)
,m_pBMFRest(NULL)
,m_pBMFPriceOne(NULL)
,m_pBMFPriceTen(NULL)
,m_pBMFSoulRight(NULL)
,m_pBMFSoulLeft(NULL)
,m_pSoulNode(NULL)
,m_pMorePop(NULL)
,m_nSoundID(0)
,m_bCurrentCardResponsed(false)
,m_bLotteryEffectTenShowed(false)
{
    D_RESET_C_ARRAY(m_pCardButtons, CARD_COUNT);
	D_RESET_C_ARRAY(m_pBtn, CARD_COUNT);
}

Lottery::~Lottery()
{
    D_SAFE_RELEASE_C_ARRAY(m_pCardButtons, CARD_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_pBtn, CARD_COUNT);
    
    CC_SAFE_RELEASE(m_pLotteryModel);
	CC_SAFE_RELEASE(m_pEffectNode);
	CC_SAFE_RELEASE(m_pNodeMore);
	CC_SAFE_RELEASE(m_pDBtnClose);
	CC_SAFE_RELEASE(m_pNodeFree);
	CC_SAFE_RELEASE(m_pNodeGold);
	CC_SAFE_RELEASE(m_pItemCost);
	CC_SAFE_RELEASE(m_pNodeRest);
	CC_SAFE_RELEASE(m_pNodeNext);
	CC_SAFE_RELEASE(m_pBMFRest);
	CC_SAFE_RELEASE(m_pBMFPriceOne);
	CC_SAFE_RELEASE(m_pBMFPriceTen);
	CC_SAFE_RELEASE(m_pBMFSoulRight);
	CC_SAFE_RELEASE(m_pBMFSoulLeft);
	CC_SAFE_RELEASE(m_pSoulNode);
}

Lottery* Lottery::create(SoulExchangeDelegate* pDelegate)
{
	Lottery *pRet = new Lottery();
	pRet->setDelegate(pDelegate);
	if (pRet && pRet->init())
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		CC_SAFE_DELETE(pRet); 
		return NULL; 
	} 
}

bool Lottery::init()
{
    if(!DLayer::init()) return false;
    m_pLotteryModel = DM_GET_LOTTERY_MODEL;
    CC_SAFE_RETAIN(m_pLotteryModel);
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("CardButton", CardButtonLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_shop/PL_beibao_chouka", pNodeLoaderLibrary);
       
    
    UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView());
    
//     sp::Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("lottery_bg",sp::ZORDER);
//     pArmature->setPosition(D_DESIGN_POINT_CENTER);
//     pArmature->getAnimation()->playByIndex(0);
//     addChild(pArmature);
    
    
    
    m_pLotteryModel->getCardInfo();

// 	m_pEffectNode->removeAllChildren();
// 	pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("lottery_star");
// 	m_pEffectNode->addChild(pArmature);
    
	ActivitySoulExchange *pExchangeView = ActivitySoulExchange::create();
	if (pExchangeView && m_pDelegate)
	{
		m_pDelegate->addHead(pExchangeView);
	}

	m_pNodeMore->setVisible(false);
	m_pDBtnClose->getButton()->setTarget(this, menu_selector(Lottery::closeMorePop));

	const char *pBtnForIp4[] = {"pl_ck_btn_yiyidibai_ip4", "pl_ck_btn_yiqidangqian_ip4", "pl_ck_btn_wanfumokai_ip4"};
	const char *pBtnForIp4Up[] = {"pl_ck_btn_yiyidibai_ip4_up", "pl_ck_btn_yiqidangqian_ip4_up", "pl_ck_btn_wanfumokai_ip4_up"};

	if (PL_GET_HEIGHT_DIFF > 1.f)
	{
		for (int i = 0; i < CARD_COUNT; i++)
		{
			m_pBtn[i]->setNormalImage(CCSprite::create(CCString::createWithFormat("PL_ui/chouka/%s.png", pBtnForIp4[i])->getCString()));
			m_pBtn[i]->setSelectedImage(CCSprite::create(CCString::createWithFormat("PL_ui/chouka/%s.png", pBtnForIp4Up[i])->getCString()));
			PL_MOVE_NODE_ABOVE_CENTER_Y(m_pBtn[i]);
		}
		
	}
	

	m_pNodeMore->removeFromParent();
	m_pNodeMore->setPosition(ccpSub(D_DESIGN_POINT_CENTER, ccpMult(m_pNodeMore->getContentSize(), 0.5)));
	PL_MOVE_NODE_ABOVE_CENTER_Y(m_pNodeMore);
	m_iCurrWid = 0;
    return true;
}

bool Lottery::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCard1", CardButton *, this->m_pCardButtons[0]);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCard2", CardButton *, this->m_pCardButtons[1]);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCard3", CardButton *, this->m_pCardButtons[2]);
    D_CCB_ASSIGN_ARRAY("m_pCard", CardButton *, this->m_pCardButtons, CARD_COUNT);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeMore", CCNode *, this->m_pNodeMore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnClose", DButton *, this->m_pDBtnClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFree", CCNode *, this->m_pNodeFree);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeGold", CCNode *, this->m_pNodeGold);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCost", CCNode *, this->m_pItemCost);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRest", CCNode *, this->m_pNodeRest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNext", CCNode *, this->m_pNodeNext);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRest", CCLabelBMFont *, this->m_pBMFRest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPriceOne", CCLabelBMFont *, this->m_pBMFPriceOne);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPriceTen", CCLabelBMFont *, this->m_pBMFPriceTen);
	D_CCB_ASSIGN_ARRAY("m_pBtn", CCMenuItemImage *, this->m_pBtn, CARD_COUNT);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFSoulRight", CCLabelBMFont *, this->m_pBMFSoulRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFSoulLeft", CCLabelBMFont *, this->m_pBMFSoulLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulNode", CCNode *, this->m_pSoulNode);
    return false;
}

SEL_MenuHandler Lottery::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallBack", Lottery::menuCallBack);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "moreCallBack", Lottery::moreCallBack);
    return NULL;
}

void Lottery::onEnter()
{
	DLayer::onEnter();
	BINDING_EXEC(m_pLotteryModel, this, Lottery::updateView , D_EVENT_LOTTERY_INITED);
	BINDING_ONLY(m_pLotteryModel, this, Lottery::onCardDraw , D_EVENT_LOTTERY_DRAW);
}

void Lottery::onExit()
{
	UNBINDING_ALL(m_pLotteryModel, this);
	DLayer::onExit();
}

void Lottery::resetView()
{
    for (uint8_t i =0; i<CARD_COUNT; ++i)
    {
        m_pCardButtons[i]->setCard(NULL);
    }
}

void Lottery::updateView()
{
    if(!m_pLotteryModel->isSynced)
    {
        resetView();
        return;
    }
    
    for (uint8_t i =0; i<CARD_COUNT; ++i)
    {
        m_pCardButtons[i]->setCard(m_pLotteryModel->getCardInfoByType((card::CardType)(i+1)));
    }

	m_pBMFPriceOne->setString(D_CSTRING_FROM_UINT(m_pLotteryModel->getCardInfoByType(card::kCardTypeHard)->getPrice()));
	m_pBMFPriceTen->setString(D_CSTRING_FROM_UINT(m_pLotteryModel->getCardInfoByType(card::kCardTypeHard10)->getPrice()));

	updateRest();
}

void Lottery::updateRest()
{

	unsigned int rest = m_pLotteryModel->getRestForOrange();
	m_pNodeNext->setVisible(rest == 0);
	m_pNodeRest->setVisible(rest != 0);
	m_pBMFRest->setString(D_CSTRING_FROM_UINT(rest));
}

void Lottery::menuCallBack(cocos2d::CCObject* sender)
{
	//点击了抽卡之后 隐藏掉魂魄兑换的标志
	m_pSoulNode->setVisible(false);

    CCMenuItem* pItem = (CCMenuItem*)sender;
    int tag = pItem->getTag();
	drawCard(card::CardType(tag));
}

void Lottery::updateState()
{
	bool bFree = !(m_pLotteryModel->getCardInfoByType(card::kCardTypeHard)->isCost());
	unsigned int itemnum = DM_GET_ITEM_MODEL->getItemNum(HARD_CARD_ITEM_ID);
	m_pNodeFree->setVisible(bFree);
	m_pNodeGold->setVisible(itemnum == 0 && !bFree);
	m_pItemCost->setVisible(itemnum != 0 && !bFree);
}

void Lottery::onCardDraw()
{
	//m_bCurrentCardResponsed = true;
	Json::Value requestData = m_pLotteryModel->getRequestData();
	Json::Value responseData = m_pLotteryModel->getResponseData();

	//if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;

	updateRest();
	updateState();


	card::CardType eType = (card::CardType)responseData["card"]["type"].asUInt();

	if (requestData["type"].asUInt() == card::kCardTypeHard10)
	{
		//十连抽显示内容
		//显示掉落UI
		DropModel *pModel = DM_GET_DROP_MODEL;
		cocos2d::CCArray *pAwardList = pModel->getDropArray(requestData[UUID].asInt());
		if(!pAwardList || pAwardList->count() == 0) return;
		CCArray *pList = CCArray::create();
		for (unsigned int i = 0; i < pAwardList->count(); i++)
		{
			drop::Drop *pDrop = (drop::Drop *)pAwardList->objectAtIndex(i);
			if (pDrop->getType() == drop::kDropTypeSoul)
			{
				warrior::SoulInfo *pSoul = pDrop->getSoul();
				warrior::WarriorGrowInfo *pGrow = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(pSoul->getWid());
				unsigned int num = pSoul->getNum();
				while (num > 0)
				{
					num -= pGrow->getRctSoul();
					pList->addObject(drop::Drop::create(warrior::SoulInfo::create(pSoul->getWid(), pGrow->getRctSoul())));
				}
				continue;
			}
			pList->addObject(pDrop);
		}
		std::sort(pList->data->arr, pList->data->arr + pList->data->num, sortDropFromCard10);

		LotteryEffectTen *pEffect = dynamic_cast<LotteryEffectTen*>(m_pEffect);
		if(pEffect)
		{
			pEffect->setData(pList);
			//若动画播完了 仍未显示结果 则在这里补显示
			pEffect->showWarrior();
			//if(!m_bLotteryEffectTenShowed)
			//{
			//	//若动画播完了 仍未显示结果 则在这里补显示
			//	pEffect->showWarrior();
			//}
		}
	}
	else
	{
		//单抽显示内容
		LotteryEffect* pEffect = dynamic_cast<LotteryEffect*>(m_pEffect);

		if(pEffect)
		{
			if(responseData["drop"].isMember("warrior"))
			{
				warrior::WarriorCurInfo* pInfo = DM_GET_WARRIOR_MODEL->parseWarrior(responseData["drop"]["warrior"]);
				pEffect->setData(pInfo , eType);
				pEffect->setWarriorExistsState(false);
				m_iCurrWid = pInfo->getWid();
			}
			else if(responseData["drop"].isMember("soul"))
			{
				warrior::SoulInfo *pInfo = DM_GET_WARRIOR_MODEL->parseSoul(responseData["drop"]["soul"]);
				pEffect->setData(pInfo , eType);
				pEffect->setWarriorExistsState(true);
				m_iCurrWid = pInfo->getWid();
			}
			pEffect->setUserObject(cocos2d::CCInteger::create(requestData[UUID].asUInt()));
		}
	}	
}

void Lottery::onLottery(const Json::Value & requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;

	updateRest();
	updateState();

	closeMorePop(NULL);
	if (m_pAnimate)
	{
		m_pAnimate->closePopup();
		m_pAnimate = NULL;
	}
	card::CardType eType = (card::CardType)responseData["data"]["card"]["type"].asUInt();
	m_pAnimate = LotteryAnimation::create(eType);
	m_pAnimate->setDelegate(this);
	Game::sharedGame()->openPopup(m_pAnimate);

	if (m_pEffect)
	{
		m_pEffect->closePopup();
		m_pEffect = NULL;
	}

	if (requestData["type"].asUInt() == card::kCardTypeHard10)
	{
		//显示掉落UI
		DropModel *pModel = DM_GET_DROP_MODEL;
		cocos2d::CCArray *pAwardList = pModel->getDropArray(requestData[UUID].asInt());

		if(!pAwardList || pAwardList->count() == 0) return;

		CCArray *pList = CCArray::create();

		for (unsigned int i = 0; i < pAwardList->count(); i++)
		{
			drop::Drop *pDrop = (drop::Drop *)pAwardList->objectAtIndex(i);
			if (pDrop->getType() == drop::kDropTypeSoul)
			{
				warrior::SoulInfo *pSoul = pDrop->getSoul();
				warrior::WarriorGrowInfo *pGrow = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(pSoul->getWid());
				unsigned int num = pSoul->getNum();
				while (num > 0)
				{
					num -= pGrow->getRctSoul();
					pList->addObject(drop::Drop::create(warrior::SoulInfo::create(pSoul->getWid(), pGrow->getRctSoul())));
				}
				continue;
			}
			pList->addObject(pDrop);
		}

		std::sort(pList->data->arr, pList->data->arr + pList->data->num, sortDropFromCard10);

		LotteryEffectTen *pEffect = LotteryEffectTen::create(pList);
		pEffect->setCallBack(this, callfuncO_selector(Lottery::onShowed));
		m_pEffect = pEffect;
		pEffect->setDelegate(this);
        pEffect->showWarrior();
		Game::sharedGame()->openPopup(pEffect);
		//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));
		return;
	}
    LotteryEffect* pEffect = NULL;
    if(responseData["data"]["drop"].isMember("warrior"))
    {
        warrior::WarriorCurInfo* pInfo = DM_GET_WARRIOR_MODEL->parseWarrior(responseData["data"]["drop"]["warrior"]);
        pEffect = LotteryEffect::create(pInfo, eType);
		pEffect->setWarriorExistsState(false);
		m_iCurrWid = pInfo->getWid();
    }
    else if(responseData["data"]["drop"].isMember("soul"))
    {
        warrior::SoulInfo *pInfo = DM_GET_WARRIOR_MODEL->parseSoul(responseData["data"]["drop"]["soul"]);
        pEffect = LotteryEffect::create(pInfo, eType);
        pEffect->setWarriorExistsState(true);
		m_iCurrWid = pInfo->getWid();
    }

	m_pEffect = pEffect;

    if(pEffect)
    {
		pEffect->setDelegate(this);
        pEffect->setCallBack(this, callfuncO_selector(Lottery::onShowed));
        pEffect->setUserObject(cocos2d::CCInteger::create(requestData[UUID].asUInt()));
        Game::sharedGame()->openPopup(pEffect);
	}
}

void Lottery::onShowed(cocos2d::CCObject *exploreNode)
{
	m_pEffect = NULL;
//     CCNode* pEffect = dynamic_cast<CCNode*>(exploreNode);
//     NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_REMOVE,pEffect->getUserObject());
}

void Lottery::moreCallBack(cocos2d::CCObject *pObj)
{
	if (m_pMorePop == NULL)
	{
		//显示魂魄兑换
		m_pSoulNode->setVisible(DM_GET_ACTIVITY_MODEL->getSoulPointInfo()->isOpen());
		if(m_pSoulNode->isVisible())
		{
			//m_pBMFSoulLeft->setString(D_CSTRING_FROM_UINT(DM_GET_ACTIVITY_MODEL->getSoulPointInfo()->getSoulPointFromCardAtIndex(2)));
			//m_pBMFSoulRight->setString(D_CSTRING_FROM_UINT(DM_GET_ACTIVITY_MODEL->getSoulPointInfo()->getSoulPointFromCardAtIndex(3)));
			m_pBMFSoulLeft->setString(CCString::createWithFormat("+%d" , DM_GET_ACTIVITY_MODEL->getSoulPointInfo()->getSoulPointFromCardAtIndex(2))->getCString());
			m_pBMFSoulRight->setString(CCString::createWithFormat("+%d" , DM_GET_ACTIVITY_MODEL->getSoulPointInfo()->getSoulPointFromCardAtIndex(3))->getCString());
		}

		m_pMorePop = DPopup::create();
		m_pMorePop->addChild(m_pNodeMore);
		Game::sharedGame()->openPopup(m_pMorePop);
		updateState();
		m_pNodeMore->setVisible(true);
	}
}

void Lottery::closeMorePop(cocos2d::CCObject *pObj)
{
	if (m_pMorePop)
	{
		//隐藏
		m_pSoulNode->setVisible(false);

		m_pMorePop->closePopup();
		m_pMorePop = NULL;
		m_pNodeMore->removeFromParent();
	}
}

void Lottery::onceMore(DPopup *pView, card::CardType eType)
{
//	pView->closePopup();
	drawCard(eType);
}

void Lottery::showAnimation(card::CardType eType)
{
	closeMorePop(NULL);
	if (m_pAnimate)
	{
		m_pAnimate->closePopup();
		m_pAnimate = NULL;
	}

	m_pAnimate = LotteryAnimation::create(eType);
	m_pAnimate->setDelegate(this);
	Game::sharedGame()->openPopup(m_pAnimate);

	if (m_pEffect)
	{
		m_pEffect->closePopup();
		m_pEffect = NULL;
	}

	if (eType == card::kCardTypeHard10)
	{	
		LotteryEffectTen *pEffect = LotteryEffectTen::create();
		pEffect->setCallBack(this, callfuncO_selector(Lottery::onShowed));
		m_pEffect = pEffect;
		pEffect->setDelegate(this);
		//pEffect->showWarrior();
		Game::sharedGame()->openPopup(pEffect);
	}
	else
	{
		//预先加载单抽（有界面无数据）
		LotteryEffect* pEffect = LotteryEffect::create();
		m_pEffect = pEffect;
		pEffect->setDelegate(this);
		pEffect->setCallBack(this, callfuncO_selector(Lottery::onShowed));
		Game::sharedGame()->openPopup(pEffect);	
	}
	
}

void Lottery::drawCard(card::CardType eType)
{
	if(eType<card::kCardTypeEasy || eType>card::kCardTypeHard10)
	{
		CCLOG("error type");
		return;
	}
	card::CardInfo* pCard = m_pLotteryModel->getCardInfoByType(eType);
	if (!pCard)
	{
		return;
	}

	//提示招募令 限时英雄不需要（招募令）
	if(eType == card::kCardTypeEasy)
	{
		item::ItemInfo *pItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(ZHAO_MU_LING_ID);
		if(pItemInfo == NULL)
		{
			Tips_Alert(D_LOCAL_STRING("zhaomulingnotenough"));
			return;
		}
	}

	if(pCard->isCost() && pCard->getType() > card::kCardTypeEasy  && pCard->getPrice()>DM_GET_USER_MODEL->getUserInfo()->getPoints())
	{
		if (m_pEffect)
		{
			m_pEffect->closePopup();
			m_pEffect = NULL;
		}
		closeMorePop(NULL);
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
		return;
	}
	showAnimation(eType);
	//HTTP_CLIENT->drawCard(pCard->getType(),pCard->isCost(),this,callfuncJson_selector(Lottery::onLottery));
}

void Lottery::doConfirm(card::CardType eType)
{

	if(eType<card::kCardTypeEasy || eType>card::kCardTypeHard10)
	{
		CCLOG("error type");
		return;
	}
	card::CardInfo* pCard = m_pLotteryModel->getCardInfoByType(eType);
	if (!pCard)
	{
		return;
	}


	if(pCard->isCost() && pCard->getType() > card::kCardTypeEasy  && pCard->getPrice()>DM_GET_USER_MODEL->getUserInfo()->getPoints())
	{
		if (m_pEffect)
		{
			m_pEffect->closePopup();
			m_pEffect = NULL;
		}
		closeMorePop(NULL);
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
		return;
	}
	//调接口抽卡
	//HTTP_CLIENT->drawCard(pCard->getType(),pCard->isCost());
	HTTP_CLIENT->drawCard(pCard->getType(),pCard->isCost(),this,callfuncJson_selector(Lottery::onError));
	//置位
	//m_bCurrentCardResponsed = false;
	//m_bLotteryEffectTenShowed = false;
}

void Lottery::onError(const Json::Value & requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0)
	{
		doCancel(card::kCardTypeEasy);
	}
}

void Lottery::doCancel(card::CardType eType)
{
	if (m_pAnimate)
	{
		m_pAnimate->closePopup();
		m_pAnimate = NULL;
	}
	if (m_pEffect)
	{
		m_pEffect->closePopup();
		m_pEffect = NULL;
	}
	//确保返回时，界面可以点击
	Game::sharedGame()->setTouchEnable(true);
}

void Lottery::animationEnded(card::CardType eType)
{
//	card::CardInfo* pCard = m_pLotteryModel->getCardInfoByType(eType);
	playHeroSound(m_iCurrWid);
	if (m_pAnimate)
	{
		m_pAnimate->closePopup();
		m_pAnimate = NULL;
	}
	if(eType == card::kCardTypeHard10 || eType == card::kCardTypeSpecial10)
	{
		/*if(m_bCurrentCardResponsed)
		{
			m_bLotteryEffectTenShowed = true;
			LotteryEffectTen *pEffect = dynamic_cast<LotteryEffectTen*>(m_pEffect);
			if(pEffect)
			{
				pEffect->showWarrior();
			}
		}*/		
	}
}

void Lottery::playHeroSound(unsigned int wid)
{
	//stopHeroSound(0.f);
	if(wid == 0) return;
	sound::Sound* pSound = DM_GET_SOUND_MODEL->getUnitSound(wid, 0, "voice");
	if(NULL == pSound) return;
	m_nSoundID = SoundManager::sharedSoundManager()->playSFX(CCString::createWithFormat(pSound->getName().c_str()));
	scheduleOnce(schedule_selector(Lottery::stopHeroSound), 5.f);
}

void Lottery::stopHeroSound(float delay)
{
	m_iCurrWid = 0;
	if (m_nSoundID) {
		SoundManager::sharedSoundManager()->stopSFX(m_nSoundID);
		m_nSoundID = 0;
	}
}
