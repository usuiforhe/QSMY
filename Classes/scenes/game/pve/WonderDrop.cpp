//
//  WonderDrop.cpp
//  QSMY
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#define WONDER_ANI_COUNT_DOWN	3.f

#include "WonderDrop.h"
#include "managers/GameManager.h"
#include "../common/gui/WarriorGradeBg.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace sp;

WonderDrop::WonderDrop()
	: m_pBtnLeft(NULL)
	, m_pBtnRight(NULL)
	, m_pBMFTime(NULL)
//	, m_pNodeIcon(NULL)
//	, m_pTTFName(NULL)
	, m_pNodeItem(NULL)
	, m_pNodeHead(NULL)
	, m_pNode3Btns(NULL)
	, m_pNode1Btn(NULL)
	, m_pRichDescription(NULL)
	, m_pBMFExp(NULL)
	, m_pData(NULL)
	, m_pDelegate(NULL)
	, m_uIdx(0)
	, m_fDisposeTime(0.f)
	, m_pRoot(NULL)
	, m_pArmature(NULL)
	, m_bAnimating(false)
	, m_pGradeWarrior(NULL)
    , m_pWarriorName(NULL)
	,m_pPoint(NULL)
    ,m_pUserPointsNode(NULL)
	,m_bIsDone(false)
	,m_pBtnThird(NULL)
	,m_ichestGrade(0)
    ,m_pBossGrade(NULL)
    ,m_pBossContainer(NULL)
    ,m_pBossName(NULL)
    ,m_pBossLevel(NULL)
	,m_pLeftLabel7(NULL)
	,m_pRightLabel7(NULL)
    ,m_pNode3LaterLabel(NULL)
    ,m_pNode2Btns(NULL)
    ,m_pNode2LeftBtn(NULL)
    ,m_pNode2RightBtn(NULL)
{
    D_RESET_C_ARRAY(m_pBoxColor, CHEST_BOX_COLOR_NUM);
	D_RESET_C_ARRAY(m_pBoxState, CHEST_BOX_COLOR_NUM*2);
	D_RESET_C_ARRAY(m_pNodeType, settlement::kWonderTypeMax - 1);
}

WonderDrop::~WonderDrop()
{
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->removeAllObservers(this);
	}
	D_SAFE_RELEASE_C_ARRAY(m_pNodeType, settlement::kWonderTypeMax - 1);
	D_SAFE_RELEASE_C_ARRAY(m_pBoxColor, CHEST_BOX_COLOR_NUM);
	D_SAFE_RELEASE_C_ARRAY(m_pBoxState, CHEST_BOX_COLOR_NUM*2);
	CC_SAFE_RELEASE(m_pBtnLeft);
	CC_SAFE_RELEASE(m_pBtnRight);
	CC_SAFE_RELEASE(m_pBMFTime);
//	CC_SAFE_RELEASE(m_pNodeIcon);
//	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pNodeItem);
	CC_SAFE_RELEASE(m_pNodeHead);
	CC_SAFE_RELEASE(m_pNode3Btns);
	CC_SAFE_RELEASE(m_pNode1Btn);
	CC_SAFE_RELEASE(m_pRichDescription);
	CC_SAFE_RELEASE(m_pBMFExp);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pRoot);
	CC_SAFE_RELEASE(m_pGradeWarrior);
    CC_SAFE_RELEASE(m_pWarriorName);
	CC_SAFE_RELEASE(m_pPoint);
    CC_SAFE_RELEASE(m_pUserPointsNode);
	CC_SAFE_RELEASE(m_pBtnThird);
    CC_SAFE_RELEASE(m_pBossGrade);
    CC_SAFE_RELEASE(m_pBossContainer);
    CC_SAFE_RELEASE(m_pBossName);
    CC_SAFE_RELEASE(m_pBossLevel);
	CC_SAFE_RELEASE(m_pLeftLabel7);
	CC_SAFE_RELEASE(m_pRightLabel7);
    CC_SAFE_RELEASE(m_pNode3LaterLabel);
    CC_SAFE_RELEASE(m_pNode2Btns);
    CC_SAFE_RELEASE(m_pNode2LeftBtn);
    CC_SAFE_RELEASE(m_pNode2RightBtn);
}

bool WonderDrop::init()
{
    if(!DLayer::init()) return false;

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorHeadGradeBg", WarriorHeadGradeBgLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorGradeBg", WarriorGradeBgLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("AwardItemSmall", AwardItemSmallLoader::loader());
	CCB_READER_BY_CUSTOM_LOADER("PL_ui/ccb/pl_qiyu/qiyu_tansuo", pNodeLoaderLibrary, m_pRoot);
	CC_SAFE_RETAIN(m_pRoot);
	m_pRoot->setAnchorPoint(ccp(0.5, 0.5));
	m_pRoot->setPosition(ccpAdd(D_DESIGN_POINT_CENTER, ccp(0, 20)));
	PL_MOVE_WITH_RESOLUTION_DIFF(m_pRoot , -1);
	if (m_pRoot)
	{
		addChild(m_pRoot);
		m_pRoot->setScale(0.0f);
		m_pRoot->runAction(CCScaleTo::create(0.2f, 1.0f));
	}
	else
	{
		return false;
	}

	m_pBtnLeft->setTarget(this, menu_selector(WonderDrop::leftCallBack));
	m_pBtnRight->setTarget(this, menu_selector(WonderDrop::rightCallBack));

    return true;
}

void WonderDrop::onEnter()
{
	updateView();
	m_fDisposeTime = WONDER_ANI_COUNT_DOWN;
	scheduleUpdate();
	DLayer::onEnter();
}

void WonderDrop::onExit()
{
	unscheduleUpdate();
	DLayer::onExit();
}

void WonderDrop::updateView()
{
}

bool WonderDrop::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	//CCLOG("%s", pMemberVariableName);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pNodeType", CCNode *, m_pNodeType, settlement::kWonderTypeMax - 1);
	D_CCB_ASSIGN_ARRAY("m_pBoxColor", CCNode *, m_pBoxColor, CHEST_BOX_COLOR_NUM);
	D_CCB_ASSIGN_ARRAY("m_pBoxState", CCNode *, m_pBoxState, CHEST_BOX_COLOR_NUM*2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnLeft", CCMenuItem *, m_pBtnLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRight", CCMenuItem *, m_pBtnRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnThird", CCMenuItem *, m_pBtnThird);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFTime", CCLabelBMFont *, m_pBMFTime);
//	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeIcon", CCNode *, m_pNodeIcon);
//	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode1Btn", CCNode *, m_pNode1Btn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode3Btns", CCNode *, m_pNode3Btns);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRichDescription", CCRichLabelTTF *, m_pRichDescription);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeItem", AwardItemSmall *, m_pNodeItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHead", CCNode *, m_pNodeHead);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFExp", CCLabelBMFont *, m_pBMFExp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeWarrior", WarriorGradeBg *, m_pGradeWarrior);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorName", CCLabelTTF*, m_pWarriorName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPoint" , CCLabelBMFont*, m_pPoint);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUserPointsNode", CCNode*, m_pUserPointsNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBossGrade",  WarriorGradeBg*, m_pBossGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBossContainer", CCNode *, m_pBossContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBossName", CCLabelTTF*, m_pBossName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBossLevel", CCLabelBMFont *, m_pBossLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLeftLabel7", CCNode *, m_pLeftLabel7);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRightLabel7", CCNode *, m_pRightLabel7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode3LaterLabel", CCNode *, m_pNode3LaterLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode2Btns", CCNode *, m_pNode2Btns);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode2LeftBtn", DButton *, m_pNode2LeftBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode2RightBtn", DButton *, m_pNode2RightBtn);
    return false;
}

SEL_MenuHandler WonderDrop::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackClose", WonderDrop::closeCallBack);
	return NULL;
}

void WonderDrop::closeSelf()
{
	if (m_pDelegate)
	{
		m_pDelegate->displayWonders(m_uIdx + 1);
	}
	removeFromParentAndCleanup(true);
}

void WonderDrop::closeCallBack(CCObject* obj)
{
	animateBeforeClose();
}

void WonderDrop::setData(settlement::WonderInfo *pData)
{
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
//    pData->bossId = 1;
//    pData->bossLevel = 2;
//    pData->start = 100000;
//    pData->wonderType = settlement::kWonderTypeBoss;
	for (int i = 0; i < settlement::kWonderTypeMax - 1; i++)
	{
		m_pNodeType[i]->setVisible(i + 1 == pData->wonderType);
	}

	m_pNode1Btn->setVisible(pData->start > 0);
	m_pUserPointsNode->setVisible(true);
    
    if(pData->wonderType == settlement::kWonderTypeChest
       || pData->wonderType == settlement::kWonderTypeBoss)
    {
        m_pNode3Btns->setVisible(false);
        m_pNode2Btns->setVisible(true);
    } else
    {
        m_pNode3Btns->setVisible(!m_pNode1Btn->isVisible());
        m_pNode2Btns->setVisible(false);
        
    }
	if (pData->start > 0)
	{
		m_pBMFTime->setString(TimeUtil::getSeconds2String(pData->start - GameManager::sharedGameManager()->getServerTime()));
	}
	else
	{
		m_pBMFTime->setString(TimeUtil::getSeconds2String(pData->end - GameManager::sharedGameManager()->getServerTime()));
	}

	m_pNodeItem->setVisible(pData->wonderType == settlement::kWonderTypeFight || pData->wonderType==settlement::kWonderTypeTrader);
//	m_pNodeIcon->removeAllChildrenWithCleanup(true);
	if (pData->wonderType == settlement::kWonderTypeFight)
	{
//		m_pNodeIcon->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(pData->itemID));
		m_pNodeItem->updateView(drop::Drop::create(drop::kDropTypeItem, pData->itemID, pData->itemNum));
		item::ItemBase *pBase = DM_GET_ITEM_MODEL->getItemBaseByID(pData->itemID);
//		m_pTTFName->setString(pBase->getItemName().c_str());
		m_pRichDescription->setString(D_LOCAL_STRING("$lujianbuping").c_str());
	}
	else if(pData->wonderType==settlement::kWonderTypeWarrior)
	{
 		uint32_t wid = DM_GET_MALL_MODEL->getProductBaseByID(pData->productId)->getItemId();
		CCNode* pNode = ResourceManager::sharedResourceManager()->getUnitAnimate2XAsync(wid);
		pNode->setScaleX(-2.0);
		pNode->setAnchorPoint(ccp(0.5, 0.5));
		m_pNodeHead->removeAllChildren();
		m_pNodeHead->addChild(pNode);
		warrior::WarriorBaseInfo *pBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
//		m_pTTFName->setString(pBase->getWarriorName().c_str());
		m_pRichDescription->setString(D_LOCAL_STRING("$buqieryu", pData->price).c_str());//fixme
		m_pGradeWarrior->setWarriorGrade(pBase->getColorGrade());
        m_pWarriorName->setString(pBase->getWarriorName().c_str());
	}
	else if(pData->wonderType == settlement::kWonderTypeBoss)
	{
        m_pNode2LeftBtn->getButton()->setTarget(this, menu_selector(WonderDrop::leftCallBack));
        m_pNode2RightBtn->getButton()->setTarget(this, menu_selector(WonderDrop::rightCallBack));
		CCNode* pNode = ResourceManager::sharedResourceManager()->getWarriorIcon(pData->bossId, 0.8);
        m_pBossContainer->addChild(pNode);
        warrior::WarriorBaseInfo* info = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pData->bossId);
        m_pBossName->setString(info->getWarriorName().c_str());
        m_pBossLevel->setString(CCString::createWithFormat("%d", pData->bossLevel)->getCString());
        m_pBossGrade->setWarriorGrade(info->getColorGrade());
        m_pUserPointsNode->setVisible(false);
		m_pRichDescription->setString(D_LOCAL_STRING("yingxiongtiaozhan").c_str());
	}
	else if (pData->wonderType == settlement::kWonderTypeTrader)
	{
		mall::ProductBase *pBase = DM_GET_MALL_MODEL->getProductBaseByID(pData->productId);
		m_pNodeItem->updateView(drop::Drop::create(drop::DropType(pBase->getType()), pBase->getItemId(), pBase->getNum()));
// 		CCSprite* pNode = ResourceManager::sharedResourceManager()->getEquipIcon50P(eid);
// 		m_pNodeIcon->addChild(pNode);
//		equip::EquipBaseInfo *pBase = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(eid);
//		m_pTTFName->setString(pBase->getEquipName().c_str());


		m_pRichDescription->setString(D_LOCAL_STRING("$yunyoushangren", pBase->getProductName().c_str(), pData->price).c_str());//fixme
	}
	else if (pData->wonderType == settlement::kWonderTypeSuperior)
	{
		m_pRichDescription->setString(D_LOCAL_STRING("$gaorenzhidian").c_str());//fixme
		m_pBMFExp->setString(D_CSTRING_FROM_UINT(pData->exp));
	}
	else if(pData->wonderType == settlement::kWonderTypeChest)
	{
        m_pNode3LaterLabel->setVisible(false);
		m_pNode2RightBtn->getButton()->setTarget(this, menu_selector(WonderDrop::rightCallBack));
        m_pNode2LeftBtn->getButton()->setTarget(this, menu_selector(WonderDrop::leftCallBack));
		mall::ProductBase *pBase = DM_GET_MALL_MODEL->getProductBaseByID(pData->productId);
		//m_pRichDescription->setString(D_LOCAL_STRING("$yunyoushangren", pBase->getProductName().c_str(), pData->price).c_str())
		m_ichestGrade = (uint8_t)DM_GET_ITEM_MODEL->getItemBaseByID(pBase->getItemId())->getItemGrade() - 1;
		showChestState(m_ichestGrade, kCloseState);
		uint32_t now = GameManager::sharedGameManager()->getServerTime();
		m_fDisposeTime = m_pData->end - now;
	}
	else
	{
		m_pRichDescription->setString(D_LOCAL_STRING("$shenbijuanzhou1").c_str());//fixme
	}
	if(m_pPoint) m_pPoint->setString(CCString::createWithFormat("%d",DM_GET_USER_MODEL->getUserInfo()->getPoints())->getCString());
}

void WonderDrop::showChestState(uint8_t color, ChestState state)
{
	for(int i=0; i<CHEST_BOX_COLOR_NUM; i++)
	{
		if(i == color)
		{
			m_pBoxColor[i]->setVisible(true);
			m_pBoxState[i*2]->setVisible(state != kOpenState);
			m_pBoxState[i*2+1]->setVisible(state == kOpenState);
			
		} else {
			m_pBoxColor[i]->setVisible(false);
		}
		
	}
}

void WonderDrop::leftCallBack(CCObject *obj)
{
	if (m_pData == NULL)
	{
		closeSelf();
		return;
	}
	uint32_t now = GameManager::sharedGameManager()->getServerTime();
	if (m_pData->start > 0 && m_pData->start > now)
	{
//		closePopup();
		return;
	}
	if (m_pData->end <= now)
	{
		closeSelf();
		return;
	}
	switch (m_pData->wonderType)
	{
	case settlement::kWonderTypeTrader:
		unscheduleUpdate();
		HTTP_CLIENT->wonderTrader(m_pData->wonderID, 0, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		
		break;
	case settlement::kWonderTypeSuperior:
		HTTP_CLIENT->wonderSuperior(m_pData->wonderID, 0, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
	case settlement::kWonderTypeTreasure:
		HTTP_CLIENT->wonderTreasure(m_pData->wonderID, 0, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
	case settlement::kWonderTypeWarrior:
		unscheduleUpdate();
		HTTP_CLIENT->wonderWarrior(m_pData->wonderID, 0, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
	case settlement::kWonderTypeFight:
		unscheduleUpdate();
		HTTP_CLIENT->wonderFight(m_pData->wonderID, 1, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
    case settlement::kWonderTypeBoss:
        unscheduleUpdate();
        HTTP_CLIENT->wonderFightBoss(m_pData->wonderID, this, callfuncJson_selector(WonderDrop::wonderCallBack));
        break;
	case settlement::kWonderTypeChest:
		unscheduleUpdate();
		showChestState(m_ichestGrade, kOpenState);
		HTTP_CLIENT->wonderBox(m_pData->wonderID, 0, this, callfuncJson_selector(WonderDrop::wonderCallBack));

		break;
	default:;
	}
		
}

void WonderDrop::rightCallBack(CCObject *obj)
{
	if (m_pData == NULL)
	{
		closeSelf();
		return;
	}
	uint32_t now = GameManager::sharedGameManager()->getServerTime();
	if (m_pData->start > 0 && m_pData->start > now)
	{
//		closePopup();
		return;
	}
	if (m_pData->end <= now)
	{
		closeSelf();
		return;
	}

	DMessageBox* pPBox = NULL;
	CCString* pNotice = NULL;

	switch (m_pData->wonderType)
	{
	case settlement::kWonderTypeTrader:
		unscheduleUpdate();

		pNotice = CCString::create(D_LOCAL_STRING("$qiyu_yunyoushangren" , m_pData->price));
		pPBox = DMessageBox::showMessageBox(pNotice->getCString(), this, messageBox_callfunc_selector(WonderDrop::onMessageBoxComfirm));
		pPBox->setUserObject(CCInteger::create(m_pData->wonderID*10+(int)m_pData->wonderType));

		//HTTP_CLIENT->wonderTrader(m_pData->wonderID, 1, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
	case settlement::kWonderTypeSuperior:
		HTTP_CLIENT->wonderSuperior(m_pData->wonderID, 1, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
	case settlement::kWonderTypeTreasure:
		HTTP_CLIENT->wonderTreasure(m_pData->wonderID, 1, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
	case settlement::kWonderTypeWarrior:
		unscheduleUpdate();

		pNotice = CCString::create(D_LOCAL_STRING("$qiyu_buqieryu" , m_pData->price));
		pPBox = DMessageBox::showMessageBox(pNotice->getCString(), this, messageBox_callfunc_selector(WonderDrop::onMessageBoxComfirm));
		pPBox->setUserObject(CCInteger::create(m_pData->wonderID*10+(int)m_pData->wonderType));

		//HTTP_CLIENT->wonderWarrior(m_pData->wonderID, 1, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
	case settlement::kWonderTypeFight:
		unscheduleUpdate();
		HTTP_CLIENT->wonderFight(m_pData->wonderID, 0, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
	case settlement::kWonderTypeChest:
		unscheduleUpdate();
		showChestState(m_ichestGrade, kOpenState);
		HTTP_CLIENT->wonderBox(m_pData->wonderID, 1, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		break;
    case settlement::kWonderTypeBoss:
        closeSelf();
        break;
	default:;
	}
}

void WonderDrop::onMessageBoxComfirm(MessageType type,DMessageBox*target)
{
	if(type == kMessageTypeOk)
	{
		CCInteger* pInt = dynamic_cast<CCInteger*>(target->getUserObject());
		int wonderID = pInt->getValue()/10;
		int wonderType = pInt->getValue()%10;
		//奇遇中，3不期而遇4云游商人 目前只处理这两种情况
		
		//const char* m = wonderType == 3 ? "warrior" : "trader";
		//cocos2d::CCString* pStr = CCString::createWithFormat("{\"id\":%d,\"buy\":%d}" , wonderID , 1);
		//HTTP_CLIENT->requestFromLua(NULL , "Wonder" , m , pStr->getCString());

		if(wonderType == 3)
		{
			HTTP_CLIENT->wonderWarrior(wonderID, 1, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		}
		else
		{
			HTTP_CLIENT->wonderTrader(wonderID, 1, this, callfuncJson_selector(WonderDrop::wonderCallBack));
		}
	}
}

void WonderDrop::update(float dt)
{
	if (m_pData == NULL || m_bIsDone)
	{
		unscheduleUpdate();
		//if(!isRunning()) return;
		closeSelf();
		return;
	}
	if (m_fDisposeTime <= 0.f)
	{
		settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();
		if(pSettlementInfo->isHasWonder() && m_uIdx < (pSettlementInfo->m_pWonders->count()))
		{
			if(m_pData->wonderType == settlement::kWonderTypeChest)
			{
				HTTP_CLIENT->wonderBox(m_pData->wonderID, 0, this, callfuncJson_selector(WonderDrop::wonderCallBack));
			} else 
			{
				animateBeforeClose();
			}
			
			unscheduleUpdate();
			return;
		}
	}
	
	uint32_t now = GameManager::sharedGameManager()->getServerTime();
	
	if (m_pData->start > 0 && m_pData->start > now)
	{
		m_pBMFTime->setString(TimeUtil::getSeconds2String(m_pData->start - now));
	}
	else if (m_pData->end > now)
	{
		m_pBMFTime->setString(TimeUtil::getSeconds2String(m_pData->end - now));
	}
	m_fDisposeTime -= dt;
}


void WonderDrop::wonderCallBack(const Json::Value &,const Json::Value &responseData)
{

	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	switch (m_pData->wonderType)
	{
	case settlement::kWonderTypeTrader:
		m_bIsDone = true;
		break;
	case settlement::kWonderTypeSuperior:

		break;
	case settlement::kWonderTypeTreasure:

		break;
	case settlement::kWonderTypeWarrior:
		m_bIsDone = true;
		break;
	case settlement::kWonderTypeFight:
		m_bIsDone = true;
		break;
	case settlement::kWonderTypeChest:
		m_bIsDone = true;
		break;
    case settlement::kWonderTypeBoss:
        closeSelf();
        return;
        break;
	default:;
	}
	if(!isRunning()) return;
	
	closeSelf();
	
}

void WonderDrop::animateBeforeClose()
{
	if (m_bAnimating || m_bIsDone || m_pData->wonderType == settlement::kWonderTypeChest)
	{
		return;
	}
	m_bAnimating = true;
	sp::Armature *pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("wonder_drop_ani", sp::ZORDER);
	m_pRoot->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.3f, 0.16f), CCCallFunc::create(this, callfunc_selector(WonderDrop::nodeScaled))));
	static const char *pIcons[] = {NULL, "qiyu_icon_gaoren", "qiyu_icon_juanzong", "qiyu_icon_ouyu", "qiyu_icon_shangren", "qiyu_icon_dajie", "qiyu_icon_tiaozhan"};
	CCSprite *pIcon = CCSprite::create(CCString::createWithFormat("PL_ui/qiyu/%s.png", pIcons[m_pData->wonderType])->getCString());
	pArmature->getBone("icon")->replaceDisplayAt(pIcon, 0);
	pArmature->setPosition(D_DESIGN_POINT_CENTER);
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(WonderDrop::animateEnd), COMPLETE);
	PL_MOVE_WITH_RESOLUTION_DIFF(pArmature , -1);
	addChild(pArmature);
	m_pArmature = pArmature;

	sp::Armature *pGotAni = ResourceManager::sharedResourceManager()->getUIAnimate("wonderGot");
	pGotAni->setPosition(D_DESIGN_POINT_CENTER);
	//PL_MOVE_NODE_ABOVE_CENTER_Y(pGotAni);
	PL_MOVE_WITH_RESOLUTION_DIFF(pGotAni , -1);
	addChild(pGotAni);
}

void WonderDrop::nodeScaled()
{
	m_pRoot->setVisible(false);
}

void WonderDrop::animateEnd(sp::Armature * target, const char* event,const char* data)
{
	closeSelf();
}