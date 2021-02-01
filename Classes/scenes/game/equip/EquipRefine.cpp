//
//  EquipRefine.cpp
//  装备精炼
//
//  Created by mzp on 14-04-09.
//	Modified 
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "EquipRefine.h"
#include "EquipRefineCell.h"
#include "../common/common.h"
#include "EquipDelegate.h"
#include "../formation/Formation.h"
#include "../../../managers/DataManager.h"
#include "../inventory/Inventory.h"
#include "../../../managers/GameManager.h"
#include "EquipEnhance.h"
#include "../formation/Formation.h"
USING_NS_CC;



EquipRefine::EquipRefine()
:m_pCCBData(NULL)
,m_pTableView(NULL)
,m_pRefineItemArray(NULL)
,m_pItemInfoArray(NULL)
,m_pEquipDelegate(NULL)
,m_pFormationDelegate(NULL)
,m_pEquipInfo(NULL)
,m_pModel(NULL)
,bProgressUpdate(false)
,m_pTableNode(NULL)
,m_pContainer(NULL)
,m_pGrade(NULL)
,m_pName(NULL)
,m_pLevel(NULL)
,m_pUpLevelNode(NULL)
,m_pUpLevel(NULL)
,m_pFight(NULL)
,m_pProperty(NULL)
,m_pPropertyResult(NULL)
,m_pCloseButton(NULL)
,m_pResist(NULL)
,m_pResistResult(NULL)
,m_pTypeLeft(NULL)
,m_pTypeRight(NULL)
,m_pTypeLeftDown(NULL)
,m_pTypeRightDown(NULL)
{
	D_RESET_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

EquipRefine::~EquipRefine()
{
    CC_SAFE_RELEASE(m_pEquipInfo);   
    CC_SAFE_RELEASE(m_pCCBData);

    CC_SAFE_RELEASE(m_pTableNode);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pGrade);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pUpLevelNode);
	CC_SAFE_RELEASE(m_pUpLevel);
	CC_SAFE_RELEASE(m_pFight);
	CC_SAFE_RELEASE(m_pProperty);
	CC_SAFE_RELEASE(m_pPropertyResult);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pResist);
	CC_SAFE_RELEASE(m_pResistResult);
	CC_SAFE_RELEASE(m_pTypeLeft);
	CC_SAFE_RELEASE(m_pTypeRight);
	CC_SAFE_RELEASE(m_pTypeLeftDown);
	CC_SAFE_RELEASE(m_pTypeRightDown);
	D_SAFE_RELEASE_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

bool EquipRefine::init()
{
	if ((!DLayer::init()))
	{
		return false;
	}

    m_pModel = DM_GET_EQUIP_MODEL;


	CCNodeLoaderLibrary * pNodeLoaderLibarary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibarary);
	pNodeLoaderLibarary->registerCCNodeLoader("EquipTypeMark", EquipTypeMarkLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_zhuangbeijinglian/PL_zhuangbeijinglian", pNodeLoaderLibarary);
    m_pCCBData = readCCData("PL_ui/ccb/pl_common/pl_qianghuabox");

    CC_SAFE_RETAIN(m_pCCBData);

    m_pRefineItemArray = m_pModel->getEquipRefineItemInfoVec();

	m_pItemInfoArray = CCArray::create();
	CC_SAFE_RETAIN(m_pItemInfoArray);
    
    m_pCloseButton->getButton()->setTarget(this, menu_selector(EquipRefine::closeCallBack));
    //m_pReturnButton->setTarget(this, menu_selector(EquipRefine::closeCallBack));
    //DM_GET_WARRIOR_MODEL->getWarriorUpgradeItemsInfo(m_pUpgradeInfoArray);

    if (m_pTableNode) {
        m_pTableView = DTableView::create(this, m_pTableNode->getContentSize());
        m_pTableView->setDirection(kCCScrollViewDirectionHorizontal);
        m_pTableView->setDelegate(this);
        //m_pTableView->reloadData();
        m_pTableView->setTouchEnabled(false);
		m_pTableView->setScrollEnabled(false);
        m_pTableNode->addChild(m_pTableView);

		updateItems();
    }
    
	return true;
}

void EquipRefine::onEnter()
{
	DLayer::onEnter();

	/************************************************************************/
	/* 开始绑定iteminfo，物品使用计数                                                        */
	/************************************************************************/
	//CCObject* obj = NULL;
	//warrior::UpgradeItemInfo* upgradeInfo = NULL;
	//CCARRAY_FOREACH(m_pUpgradeInfoArray, obj)
	//{
	//	upgradeInfo = (warrior::UpgradeItemInfo*)obj;
	//	uint32_t itemid = upgradeInfo->getItemId();
	//	item::ItemInfo * iteminfo = DM_GET_ITEM_MODEL->getItemInfoByID(itemid);

	//	if(iteminfo)
	//	{
	//		//bind
	//		m_pItemInfoArray->addObject(iteminfo);
	//		BINDING_ONLY(iteminfo,this,EquipRefine::updateItems,D_EVENT_ITEMINFO_UPDATE);
	//	}
	//}

	//UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	//UserInfoView::sharedUserInfoView()->setDisplayType(user::kUserInfoSimple);

	//自适应
	/*CCNode* head = CCNode::create();
	PL_MOVE_NODE_ABOVE_CENTER_Y(head);
	head->addChild(UserInfoView::sharedUserInfoView());
	addChild(head , 100);*/
}

void EquipRefine::onExit()
{
	//unbind
	/*CCObject* obj = NULL;
	item::ItemInfo * iteminfo = NULL;
	CCARRAY_FOREACH(m_pItemInfoArray, obj)
	{
	iteminfo = (item::ItemInfo *)obj;
	UNBINDING_ALL(iteminfo, this);
	}

	UNBINDING_ALL(m_pEquipInfo, this);*/

	DLayer::onExit();
}

void EquipRefine::setEquip( equip::EquipCurInfo * pInfo )
{
	//进来的时候不需要 进度条特效
	bProgressUpdate = false;

    if (pInfo == m_pEquipInfo)
    {
		//UNBINDING_ALL(m_pEquipInfo, this);
		//BINDING_ONLY(m_pEquipInfo,this,EquipRefine::updateWarriorInfo,D_EVENT_WARRIORINFO_UPDATE);
        return;
    }
    
    CC_SAFE_RELEASE(m_pEquipInfo);
    CC_SAFE_RETAIN(pInfo);
    m_pEquipInfo = pInfo;
    
    updateInfo();
    //binding();
}

bool EquipRefine::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableNode", CCNode*, this->m_pTableNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", WarriorGradeBg *, this->m_pGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevelNode", CCNode*, this->m_pUpLevelNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCLabelBMFont *, this->m_pUpLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont *, this->m_pFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyResult", CCLabelBMFont *, this->m_pPropertyResult);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pResist", CCLabelBMFont *, this->m_pResist);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pResistResult", CCLabelBMFont *, this->m_pResistResult);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTypeLeft", EquipTypeMark *, this->m_pTypeLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTypeRight", EquipTypeMark *, this->m_pTypeRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTypeLeftDown", EquipTypeMark *, this->m_pTypeLeftDown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTypeRightDown", EquipTypeMark *, this->m_pTypeRightDown);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar1", DProgressBar *, this->m_pProgressBar[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar2", DProgressBar *, this->m_pProgressBar[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar *, this->m_pProgressBar[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar4", DProgressBar *, this->m_pProgressBar[3]);
	return false;
}

cocos2d::SEL_MenuHandler EquipRefine::onResolveCCBCCMenuItemSelector( cocos2d::CCObject * pTarget, const char* pSelectorName )
{
	return NULL;
}

void EquipRefine::binding()
{
    BINDING_EXEC(m_pEquipInfo,this,EquipRefine::updateInfo,D_EVENT_WARRIORINFO_UPDATE);
}

void EquipRefine::updateInfo()
{
    if (!m_pEquipInfo || m_pEquipInfo->getBaseInfo() == NULL) {
        Tips_Error(D_LOCAL_STRING("WarriorInfoError"));
        return;
    }

	m_pContainer->removeAllChildren();
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon(m_pEquipInfo->getEquipId()));

	m_pName->setString(m_pEquipInfo->getBaseInfo()->getEquipName().c_str());
	m_pLevel->setString(CCString::createWithFormat("%i",m_pEquipInfo->getLevel())->getCString());
	int iGrade = (int)m_pEquipInfo->getBaseInfo()->getEquipGrade();
	warrior::WarriorGrade wGrade = (warrior::WarriorGrade)iGrade;
	m_pGrade->setWarriorGrade(wGrade);

	int iFight = m_pModel->computeEquipFight(m_pEquipInfo);
	m_pFight->setString(CCString::createWithFormat("%d",iFight)->getCString());
	unsigned int refineLevel = m_pEquipInfo->getRefineLevel();
	m_pUpLevel->setString(D_CSTRING_FROM_UINT(refineLevel));

	//EquipMark
	m_pTypeLeft->setEquipType(m_pEquipInfo->getBaseInfo()->getRefineType1());
	m_pTypeRight->setEquipType(m_pEquipInfo->getBaseInfo()->getRefineType1());
	m_pTypeLeftDown->setEquipType(m_pEquipInfo->getBaseInfo()->getRefineType2());
	m_pTypeRightDown->setEquipType(m_pEquipInfo->getBaseInfo()->getRefineType2());

	//装备属性
	std::string str;
	equip::EquipPropType type = DM_GET_EQUIP_MODEL->getEquipPropType(m_pEquipInfo, str);
	uint32_t prop_value = m_pModel->computeEquipInfo(m_pEquipInfo,type);
	uint32_t result_value = prop_value + m_pEquipInfo->getBaseInfo()->getRefineRate1();
	m_pProperty->setString(CCString::createWithFormat("+%d",prop_value)->getCString());
	m_pPropertyResult->setString(CCString::createWithFormat("+%d",result_value)->getCString());

	//精炼额外属性 保留两位小数
	std::string pecent = "%";
	float prop_refine = refineLevel * m_pEquipInfo->getBaseInfo()->getRefineRate2();
	float result_refine = prop_refine + m_pEquipInfo->getBaseInfo()->getRefineRate2();	
	m_pResist->setString(CCString::createWithFormat("+%.2f%s",prop_refine,pecent.c_str())->getCString());
	m_pResistResult->setString(CCString::createWithFormat("+%.2f%s",result_refine,pecent.c_str())->getCString());

	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = (int)m_pEquipInfo->getBaseInfo()->getEquipGrade();
		m_pProgressBar[i]->setVisible( color == i+1);
		if(color == i+1)
		{
			m_pProgressBar[i]->setPercent(0.0f);
			float percent = m_pModel->getEquipRefineExpPercent(m_pEquipInfo);
			m_pProgressBar[i]->setPercent(percent , 0.2f);
		}
	}

}

void EquipRefine::updateItems()
{
	//物品列表更新
	CCObject* pObj = NULL;
	equip::EquipRefineItemInfo* pInfo = NULL;
	item::ItemInfo*	pItemInfo = NULL;
	CCARRAY_FOREACH(m_pRefineItemArray , pObj)
	{
		pInfo = (equip::EquipRefineItemInfo*)pObj;
		pItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(pInfo->getItemId());
		pInfo->setItemNum(pItemInfo == NULL ? 0 : pItemInfo->getNum());
	}

	m_pTableView->reloadData();
}


void EquipRefine::closeCallBack( cocos2d::CCObject* sender )
{
	if (m_pFormationDelegate)
	{
		m_pFormationDelegate->showFormationView();
	}
	else if(m_pEquipDelegate)
	{
		m_pEquipDelegate->closeInfoView(kEquipRefine);
	}
	else
	{
		removeFromParent();
	}

	//removeFromParent();
}

void EquipRefine::itemUseCallBack(unsigned int itemId)
{
   itemKeepUseEndCallBack(itemId, 1);
}

void EquipRefine::itemKeepUseCallBack(unsigned int itemId, unsigned int amountPerUse /* amountPerUse = 1 */)
{
	//CCLog("-----------------------itemKeepUseCallBack");
	//bProgressUpdate = true;
	//if (m_pEquipInfo)
	//{
	//	warrior::UpgradeItemInfo* itemInfo = DM_GET_WARRIOR_MODEL->getWarriorUpgradeItemInfoById(itemId);
	//	if (itemInfo)
	//	{
	//		m_pEquipInfo->addExp(itemInfo->getExperience() * amountPerUse);
	//		DM_GET_USER_MODEL->getUserInfo()->changeCoins(-(int)(itemInfo->getCoinConsum() * amountPerUse));
	//	}
	//	else
	//	{
	//		//todo
	//		Tips_Error("item info error");
	//	}
	//}
}

void EquipRefine::itemKeepUseEndCallBack( unsigned int itemId, unsigned int count )
{
	CCLog("-----------------------itemKeepUseCallBack at the end!");
	HTTP_CLIENT->refineEquip(m_pEquipInfo->getId(), itemId, count, this, callfuncJson_selector(EquipRefine::upgradeResultCallBack));
	////吃了经验丹 进度条可能需要 更新
	bProgressUpdate = true;
}

CCSize EquipRefine::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    return CCSizeMake(130, 120);
}

CCTableViewCell* EquipRefine::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    EquipRefineItemCell* pCell = (EquipRefineItemCell*)table->dequeueCell();
    if (pCell == NULL) {
        pCell = EquipRefineItemCell::create(m_pCCBData);
        pCell->setScale(0.74f);
    }
    
    equip::EquipRefineItemInfo* itemInfo = (equip::EquipRefineItemInfo*)(m_pRefineItemArray->objectAtIndex(idx));
    //pCell->setItemId(itemInfo->getItemId());
	pCell->setInfo(itemInfo);
    pCell->setDelegate(this);
    return pCell;
}

unsigned int EquipRefine::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return m_pRefineItemArray->count();
}

void EquipRefine::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)
{
    
}

void EquipRefine::upgradeResultCallBack( const Json::Value &requestData, const Json::Value &responseData )
{
	updateInfo();
	updateItems();

	/*int criticalType =  responseData["data"]["criType"].asInt();
    if (criticalType < warrior::kUpgradeCriticalType_1 || criticalType > warrior::kUpgradeCriticalType_3) {
        return;
    }
	
    if (m_pAnimationNode) {
        m_pAnimationNode->removeAllChildrenWithCleanup(true);
        sp::Armature* pAnimation = ResourceManager::sharedResourceManager()->getUIAnimate(CCString::createWithFormat("warriorUpCritical%d", criticalType)->getCString());
        pAnimation->getAnimation()->addObserver(this, SPNotification_callfunc_selector(EquipRefine::animationPlayEnd), COMPLETE);
        m_pAnimationNode->addChild(pAnimation);
    }
	DM_GET_SOUND_MODEL->playCommonEffectSound(WARRIOR_ENHANCE_ACTION, 0);*/

	//todo
	//播放动画
}

void EquipRefine::animationPlayEnd(sp::Armature *target, const char *event, const char *data)
{
    //if(m_pAnimationNode)
    //{
    //    m_pAnimationNode->removeAllChildren();
    //}
}

//根据物品id计算出使用后的经验，判断是否超出限制
bool EquipRefine::isLvlLimit(unsigned int itemId)
{	
	//return m_pEquipInfo->getWarriorLv()>USER_WARRIOR_LEVEL_RATIO*DM_GET_USER_MODEL->getUserInfo()->getUserLv();
	return false;
}

//
//EquipRefineItemCell
//
EquipRefineItemCell::EquipRefineItemCell()
:m_pIconContainer(NULL)
,m_pCountNode(NULL)
,m_pCountLabel(NULL)
,m_pDelegate(NULL)
,m_pInfo(NULL)
,m_pTouchMenu(NULL)
,m_pMark(NULL)
,m_itemId(0)
,m_itemUseCount(0)
,m_nSoundID(0)
{
    
}

EquipRefineItemCell::~EquipRefineItemCell()
{
    CC_SAFE_RELEASE(m_pIconContainer);
    CC_SAFE_RELEASE(m_pCountLabel);
    CC_SAFE_RELEASE(m_pCountNode);
    CC_SAFE_RELEASE(m_pTouchMenu);
	CC_SAFE_RELEASE(m_pMark);
}

EquipRefineItemCell* EquipRefineItemCell::create(cocos2d::extension::CCData *ccbData)
{
    EquipRefineItemCell* pRet = new EquipRefineItemCell();
    if (pRet && pRet->init(ccbData)) {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        pRet = NULL;
    }
    
    return pRet;
}

bool EquipRefineItemCell::init(cocos2d::extension::CCData *pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);
    
    m_pTouchMenu->addTargetWithActionForControlEvents(this, cccontrol_selector(EquipRefineItemCell::upgradeButtonCallBack), CCControlEventTouchUpInside);
    m_pTouchMenu->addTargetWithActionForControlEvents(this, cccontrol_selector(EquipRefineItemCell::upgradeButtonLongPressCallBack), CCControlEventLongPress);
    m_pTouchMenu->addTargetWithActionForControlEvents(this, cccontrol_selector(EquipRefineItemCell::upgradeButtonLongPressEndCallBack), CCControlEventLongPressEnd);
	m_pTouchMenu->setLongPressRepeatFaster(true);
    return true;
}

void EquipRefineItemCell::upgradeButtonCallBack(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent controlEvent)
{
    if (m_pDelegate) {
        m_pDelegate->itemUseCallBack(m_itemId);
    }
	
	m_itemUseCount = 0;
}

void EquipRefineItemCell::upgradeButtonLongPressCallBack(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent controlEvent)
{
	CCLOG("upgradeButtonLongPressCallBack");
	//item::ItemInfo * itemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(m_itemId);
	//if (itemInfo == NULL || itemInfo->getNum() == 0)
	//{
	//	return;
	//}
	//if(m_pDelegate && m_pDelegate->isLvlLimit(m_itemId))
	//{
	//	upgradeButtonLongPressEndCallBack(m_pTouchMenu, CCControlEventLongPressEnd);
	//	return;
	//}
	////判断铜币和经验丹是否还够
	//warrior::UpgradeItemInfo* itemUpgradeInfo = DM_GET_WARRIOR_MODEL->getWarriorUpgradeItemInfoById(m_itemId);
	//
	//bool coinless = (itemUpgradeInfo == NULL) ? true : DM_GET_USER_MODEL->getUserInfo()->getCoins() < itemUpgradeInfo->getCoinConsum();
	//bool itemless = itemInfo->getNum() - m_itemUseCount <= 0;
	//if (coinless || itemless)
	//{
	//	upgradeButtonLongPressEndCallBack(m_pTouchMenu, CCControlEventLongPressEnd);
	//	return;
	//}
	//m_nSoundID = SoundManager::sharedSoundManager()->playSFX(CCString::createWithFormat("skill/hero2"));
	//unsigned int amountPerUse;
	//if(m_itemUseCount > 700)
	//{
	//	amountPerUse =  10;
	//}
	//else if(m_itemUseCount > 400)
	//{
	//	amountPerUse =  5;
	//}
	//else if (m_itemUseCount > 200)
	//{
	//	amountPerUse = 3;
	//}
	//else
	//{
	//	amountPerUse = 1;
	//}

	////铜币也可能不够了!
	//if(DM_GET_USER_MODEL->getUserInfo()->getCoins() < amountPerUse * itemUpgradeInfo->getCoinConsum())
	//{
	//	amountPerUse = 1;
	//}

 //   m_itemUseCount += amountPerUse;
 //   //经验丹不够了
	//if (m_itemUseCount > (int)itemInfo->getNum())
	//{
	//	m_itemUseCount =  (int)itemInfo->getNum();
	//}
	//m_pCountLabel->setString(CCString::createWithFormat("%d", itemInfo->getNum() - m_itemUseCount)->getCString());
 //   
 //   if (m_pDelegate) {
 //       m_pDelegate->itemKeepUseCallBack(m_itemId, amountPerUse);
 //   }

	//if(m_pDelegate && m_pDelegate->isLvlLimit(m_itemId))
	//{
	//	upgradeButtonLongPressEndCallBack(m_pTouchMenu, CCControlEventLongPressEnd);
	//	return;
	//}
}

void EquipRefineItemCell::upgradeButtonLongPressEndCallBack( cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent )
{
	/*if (m_nSoundID) {
		SoundManager::sharedSoundManager()->stopSFX(m_nSoundID);
		m_nSoundID = 0;
	}
	if (m_pDelegate && m_itemUseCount > 0)
	{
		m_pDelegate->itemKeepUseEndCallBack(m_itemId, m_itemUseCount);
	}

	m_itemUseCount = 0;*/
}

void EquipRefineItemCell::onEnter()
{
    CCTableViewCell::onEnter();
}

void EquipRefineItemCell::onExit()
{
    CCTableViewCell::onExit();
}

bool EquipRefineItemCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconContainer", CCNode*, this->m_pIconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCountLabel", CCLabelTTF*, this->m_pCountLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCountNode", CCNode*, m_pCountNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchMenu", CCControlButton*, m_pTouchMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMark", CCNode*, m_pMark);
    return false;
}

void EquipRefineItemCell::setInfo(equip::EquipRefineItemInfo* pInfo)
{
	m_pInfo = pInfo;
	m_itemId = pInfo->getItemId();
    updateView();
}

void EquipRefineItemCell::updateView()
{
    if (m_pInfo == NULL) return;

    m_pIconContainer->removeAllChildren();
    CCSprite* pIcon = ResourceManager::sharedResourceManager()->getItemIcon50P(m_itemId);
    if (pIcon) m_pIconContainer->addChild(pIcon);
    
	m_pCountLabel->setString(CCString::createWithFormat("%d", m_pInfo->getItemNum())->getCString());

  //  item::ItemInfo * itemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(m_itemId);
  //  if (m_pCountLabel) {
		//if(itemInfo){
		//	m_pCountLabel->setString(CCString::createWithFormat("%d", itemInfo->getNum())->getCString());
		//}
		//else{
		//	//没这个东西，so，0
		//	m_pCountLabel->setString("0");
		//}
  //  }
}