#include "WarriorUpgrade.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"
#include "../common/gui/WarriorGradeBg.h"
#include "Warrior.h"
#include "../user/UserInfo.h"
#include "../../../managers/SoundManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;


WarriorUpgradeView::WarriorUpgradeView()
:m_pCCBData(NULL)
,m_pContainer(NULL)
,m_pTableNode(NULL)
,m_pAnimationNode(NULL)
,m_pTableView(NULL)
,m_pUpgradeInfoArray(NULL)
,m_pItemInfoArray(NULL)
,m_pDelegate(NULL)
,m_pWarriorInfo(NULL)
,m_pWarriorModel(NULL)
//,m_pProgressBar(NULL)
,m_pDesc(NULL)
,m_pSkillBox1(NULL)
,m_pSkillBox2(NULL)
,m_pFight(NULL)
,m_pLevel(NULL)
,m_pUpLevel(NULL)
,m_pName(NULL)
,m_pReturnButton(NULL)
,m_pWarriorGradeBg(NULL)
,m_pWarriorTypeMark(NULL)
,m_pWarriorSprite(NULL)
,m_pAttackLabel(NULL)
,m_pAddAttackLabel(NULL)
,m_pDefenceLabel(NULL)
,m_pAddDefenceLabel(NULL)
,m_pHpLabel(NULL)
,m_pAddHpLabel(NULL)
,m_pSpeedLabel(NULL)
,m_pAddSpeedLabel(NULL)
,bProgressUpdate(false)
{
    D_RESET_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

WarriorUpgradeView::~WarriorUpgradeView()
{
    CC_SAFE_RELEASE(m_pWarriorInfo);
    
    CC_SAFE_RELEASE(m_pCCBData);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pTableNode);
    CC_SAFE_RELEASE(m_pAnimationNode);
    CC_SAFE_RELEASE(m_pUpgradeInfoArray);
	CC_SAFE_RELEASE(m_pItemInfoArray);
    CC_SAFE_RELEASE(m_pWarriorSprite);
    CC_SAFE_RELEASE(m_pDesc);
    CC_SAFE_RELEASE(m_pSkillBox2);
    CC_SAFE_RELEASE(m_pSkillBox1);
    CC_SAFE_RELEASE(m_pFight);
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pUpLevel);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pWarriorGradeBg);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pWarriorSprite);
    CC_SAFE_RELEASE(m_pReturnButton);
    
    CC_SAFE_RELEASE(m_pAttackLabel);
    CC_SAFE_RELEASE(m_pAddAttackLabel);
    CC_SAFE_RELEASE(m_pDefenceLabel);
    CC_SAFE_RELEASE(m_pAddDefenceLabel);
    CC_SAFE_RELEASE(m_pHpLabel);
    CC_SAFE_RELEASE(m_pAddHpLabel);
    CC_SAFE_RELEASE(m_pSpeedLabel);
    CC_SAFE_RELEASE(m_pAddSpeedLabel);

	D_SAFE_RELEASE_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

bool WarriorUpgradeView::init()
{
	if ((!DLayer::init()))
	{
		return false;
	}

    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_qianghua/pl_qianghua");
    m_pCCBData = readCCData("PL_ui/ccb/pl_common/ItemBox99p");
    CC_SAFE_RETAIN(m_pCCBData);
    m_pUpgradeInfoArray = CCArray::create();
    CC_SAFE_RETAIN(m_pUpgradeInfoArray);
	m_pItemInfoArray = CCArray::create();
	CC_SAFE_RETAIN(m_pItemInfoArray);
    
    
    m_pReturnButton->setTarget(this, menu_selector(WarriorUpgradeView::closeCallBack));
    DM_GET_WARRIOR_MODEL->getWarriorUpgradeItemsInfo(m_pUpgradeInfoArray);

    if (m_pTableNode) {
        m_pTableView = DTableView::create(this, m_pTableNode->getContentSize());
        m_pTableView->setDirection(kCCScrollViewDirectionHorizontal);
        m_pTableView->setDelegate(this);
        m_pTableView->reloadData();
        m_pTableView->setTouchEnabled(false);
		m_pTableView->setScrollEnabled(false);
        m_pTableNode->addChild(m_pTableView);
    }
    
    //m_pDesc->setVerticalTextStyle(kCCVerticalTextStyleRightToLeft);

	return true;
}

void WarriorUpgradeView::onEnter()
{
	DLayer::onEnter();

	/************************************************************************/
	/* 开始绑定iteminfo，物品使用计数                                                        */
	/************************************************************************/
	CCObject* obj = NULL;
	warrior::UpgradeItemInfo* upgradeInfo = NULL;
	CCARRAY_FOREACH(m_pUpgradeInfoArray, obj)
	{
		upgradeInfo = (warrior::UpgradeItemInfo*)obj;
		uint32_t itemid = upgradeInfo->getItemId();
		item::ItemInfo * iteminfo = DM_GET_ITEM_MODEL->getItemInfoByID(itemid);

		if(iteminfo)
		{
			//bind
			m_pItemInfoArray->addObject(iteminfo);
			BINDING_ONLY(iteminfo,this,WarriorUpgradeView::updateItems,D_EVENT_ITEMINFO_UPDATE);
		}
	}

	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	UserInfoView::sharedUserInfoView()->setDisplayType(user::kUserInfoSimple);
	//addChild(UserInfoView::sharedUserInfoView(), 100);	//添加用户基本信息
	//自适应
	CCNode* head = CCNode::create();
	PL_MOVE_NODE_ABOVE_CENTER_Y(head);
	head->addChild(UserInfoView::sharedUserInfoView());
	addChild(head , 100);
}

void WarriorUpgradeView::onExit()
{
	//unbind
	CCObject* obj = NULL;
	item::ItemInfo * iteminfo = NULL;
	CCARRAY_FOREACH(m_pItemInfoArray, obj)
	{
		iteminfo = (item::ItemInfo *)obj;
		UNBINDING_ALL(iteminfo, this);
	}

	UNBINDING_ALL(m_pWarriorInfo, this);

	DLayer::onExit();
}

void WarriorUpgradeView::setWarrior( warrior::WarriorCurInfo * pWarriorInfo )
{
	//进来的时候不需要 进度条特效
	bProgressUpdate = false;

    if (pWarriorInfo == m_pWarriorInfo)
    {
		UNBINDING_ALL(m_pWarriorInfo, this);
		BINDING_ONLY(m_pWarriorInfo,this,WarriorUpgradeView::updateWarriorInfo,D_EVENT_WARRIORINFO_UPDATE);
        return;
    }
    
    CC_SAFE_RELEASE(m_pWarriorInfo);
    CC_SAFE_RETAIN(pWarriorInfo);
    m_pWarriorInfo = pWarriorInfo;
    
    updateWarriorInfo();
    binding();
}

void WarriorUpgradeView::setWarriorDelegate(WarriorDelegate *delegate)
{
    m_pDelegate = delegate;
}

bool WarriorUpgradeView::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttackLabel", CCLabelBMFont*, this->m_pAttackLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddAttackLabel", CCLabelBMFont*, this->m_pAddAttackLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefenceLabel", CCLabelBMFont*, this->m_pDefenceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddDefenceLabel", CCLabelBMFont*, this->m_pAddDefenceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHpLabel", CCLabelBMFont*, this->m_pHpLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddHpLabel", CCLabelBMFont*, this->m_pAddHpLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedLabel", CCLabelBMFont*, this->m_pSpeedLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddSpeedLabel", CCLabelBMFont*, this->m_pAddSpeedLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReturnButton", CCMenuItemImage*, this->m_pReturnButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableNode", CCNode*, this->m_pTableNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimationNode", CCNode*, this->m_pAnimationNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode*, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorSprite", CCNode*, this->m_pWarriorSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF*, this->m_pDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBox1", SkillBox*, this->m_pSkillBox1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBox2", SkillBox*, this->m_pSkillBox2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont*, this->m_pFight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont*, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCLabelBMFont*, this->m_pUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF*, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg*, this->m_pWarriorGradeBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark*, this->m_pWarriorTypeMark);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar*, this->m_pProgressBar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar1", DProgressBar *, this->m_pProgressBar[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar2", DProgressBar *, this->m_pProgressBar[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar *, this->m_pProgressBar[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar4", DProgressBar *, this->m_pProgressBar[3]);
    
	return false;
}

cocos2d::SEL_MenuHandler WarriorUpgradeView::onResolveCCBCCMenuItemSelector( cocos2d::CCObject * pTarget, const char* pSelectorName )
{
	return NULL;
}

void WarriorUpgradeView::binding()
{
    BINDING_EXEC(m_pWarriorInfo,this,WarriorUpgradeView::updateWarriorInfo,D_EVENT_WARRIORINFO_UPDATE);
}

void WarriorUpgradeView::updateWarriorInfo()
{
    if (!m_pWarriorInfo || m_pWarriorInfo->getBaseInfo() == NULL) {
        Tips_Error(D_LOCAL_STRING("WarriorInfoError"));
        return;
    }
    
    if (m_pWarriorSprite) {
        m_pWarriorSprite->removeAllChildren();
        m_pWarriorSprite->addChild(ResourceManager::sharedResourceManager()->getUnitAnimate2X(m_pWarriorInfo->getAvatarId()));
    }
    
    m_pContainer->removeAllChildren();
    CCNode* pWarriorNode = ResourceManager::sharedResourceManager()->getWarriorIcon(m_pWarriorInfo->getAvatarId(), 0.8f);
    if (pWarriorNode) {
        m_pContainer->addChild(pWarriorNode);
    }
    
    m_pDesc->setString(m_pWarriorInfo->getBaseInfo()->getWarriorDesc().c_str());
    
    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
    
    skill::Skill* pSkill1 = DM_GET_SKILL_MODEL->getSkill(m_pWarriorInfo->getBaseInfo()->getSkillId());
    m_pSkillBox1->updateView(pSkill1, m_pWarriorInfo->getSlv());
    m_pSkillBox1->getTouchMenu()->setTag(m_pWarriorInfo->getBaseInfo()->getSkillId());
    
    
    skill::Skill* pSkill2 = DM_GET_SKILL_MODEL->getSkill(m_pWarriorInfo->getBaseInfo()->getSpSkillId());
    int spLevel = DM_GET_SKILL_MODEL->getSpecialSkillLv(pSkill2);
    m_pSkillBox2->updateView(pSkill2, spLevel);
    m_pSkillBox2->getTouchMenu()->setTag(m_pWarriorInfo->getBaseInfo()->getSpSkillId());
    
    
    
	int deltalevel = (m_pWarriorInfo->getWarriorLv() - atoi(m_pLevel->getString()));
    m_pLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorLv())->getCString());
    
    m_pUpLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorUpLv())->getCString());
    
    m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
    
	DProgressBar*progressbar = NULL;
	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = (int)m_pWarriorInfo->getBaseInfo()->getColorGrade();
		m_pProgressBar[i]->setVisible( color == i+1);
		if(color == i+1)progressbar = m_pProgressBar[i];
	}


    if (progressbar) {
        int nCurGetExp = m_pWarriorInfo->getWarriorExp();
        int nCurExp = DM_GET_WARRIOR_MODEL->getWarriorCurExpByGetExp(nCurGetExp, m_pWarriorInfo->getWarriorLv(), m_pWarriorInfo->getBaseInfo()->getRealGrade());
        int nUpExp = DM_GET_WARRIOR_MODEL->getWarriorUpExp(m_pWarriorInfo->getNextLv(), m_pWarriorInfo->getBaseInfo()->getRealGrade());
        
		//CCLog(CCString::createWithFormat("----------------------exp_%d_%d_%d_%d_%d" , m_pWarriorInfo->getWarriorLv() , m_pWarriorInfo->getNextLv() , nCurGetExp , nCurExp , nUpExp)->getCString());

		float percent = (float)nCurExp/nUpExp;
        percent = percent > 1.0 ? 1.0 : percent;

		if(!bProgressUpdate){
			//进度条普通更新
			progressbar->setPercent(percent,0.2f);
			//bProgressUpdate = true;
		}
		else{
			//进度条花式更新(带动画)
			progressbar->setPercentWithFlash(percent,0.5f , deltalevel , true);
		}   
    }
    
    m_pWarriorGradeBg->setWarriorGrade(m_pWarriorInfo->getBaseInfo()->getColorGrade());
    
    m_pWarriorInfo->computeWarriorProps();
    uint32_t currentAttack = m_pWarriorInfo->getAttack();
	uint32_t currentDefence = m_pWarriorInfo->getDefence();
	uint32_t currentHp = m_pWarriorInfo->getHp();
	uint32_t currentSpeed = m_pWarriorInfo->getBaseInfo()->getBasicSpeed();
	uint32_t attackAdd = m_pWarriorInfo->getBaseInfo()->getBasicAttackAdd();
	uint32_t defenceAdd = m_pWarriorInfo->getBaseInfo()->getBasicDefenceAdd();
	uint32_t hpAdd = m_pWarriorInfo->getBaseInfo()->getBasicHpAdd();
	uint32_t speedAdd = 0;
    
	m_pFight->setString(CCString::createWithFormat("%d", m_pWarriorInfo->computeWarriorForce())->getCString());
	m_pAttackLabel->setString(CCString::createWithFormat("%d" ,currentAttack)->getCString());
	m_pDefenceLabel->setString(CCString::createWithFormat("%d", currentDefence)->getCString());
	m_pHpLabel->setString(CCString::createWithFormat("%d", currentHp)->getCString());
	m_pSpeedLabel->setString(CCString::createWithFormat("%d", currentSpeed)->getCString());
    
	m_pAddAttackLabel->setString(CCString::createWithFormat("%d", attackAdd)->getCString());
	m_pAddDefenceLabel->setString(CCString::createWithFormat("%d", defenceAdd)->getCString());
	m_pAddHpLabel->setString(CCString::createWithFormat("%d", hpAdd)->getCString());
	m_pAddSpeedLabel->setString(CCString::createWithFormat("%d", speedAdd)->getCString());
}

void WarriorUpgradeView::updateItems()
{
	//物品列表更新
	m_pTableView->reloadData();
}


void WarriorUpgradeView::closeCallBack( cocos2d::CCObject* sender )
{
    if(m_pDelegate)
    {
       m_pDelegate->closeInfoView(kUpgradeView);
    }
    else
    {
        removeFromParent();
    }
}

void WarriorUpgradeView::itemUseCallBack(unsigned int itemId)
{
   itemKeepUseEndCallBack(itemId, 1);
}

void WarriorUpgradeView::itemKeepUseCallBack(unsigned int itemId, unsigned int amountPerUse /* amountPerUse = 1 */)
{
	//CCLog("-----------------------itemKeepUseCallBack");
	bProgressUpdate = true;
	if (m_pWarriorInfo)
	{
		warrior::UpgradeItemInfo* itemInfo = DM_GET_WARRIOR_MODEL->getWarriorUpgradeItemInfoById(itemId);
		if (itemInfo)
		{
			m_pWarriorInfo->addExp(itemInfo->getExperience() * amountPerUse);
			DM_GET_USER_MODEL->getUserInfo()->changeCoins(-(int)(itemInfo->getCoinConsum() * amountPerUse));
		}
		else
		{
			//todo
			Tips_Error("item info error");
		}
	}
}

void WarriorUpgradeView::itemKeepUseEndCallBack( unsigned int itemId, unsigned int count )
{
	//CCLog("-----------------------itemKeepUseCallBack at the end!");
	HTTP_CLIENT->upgradeWarrior(m_pWarriorInfo->getWid(), itemId, count, this, callfuncJson_selector(WarriorUpgradeView::upgradeResultCallBack));
	//吃了经验丹 进度条可能需要 更新
	bProgressUpdate = true;
}

CCSize WarriorUpgradeView::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    return CCSizeMake(115, 75);
}

CCTableViewCell* WarriorUpgradeView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    WarriorUpgradeItemCell* pCell = (WarriorUpgradeItemCell*)table->dequeueCell();
    if (pCell == NULL) {
        pCell = WarriorUpgradeItemCell::create(m_pCCBData);
        pCell->setScale(0.74f);
    }
    
    warrior::UpgradeItemInfo* itemInfo = (warrior::UpgradeItemInfo*)(m_pUpgradeInfoArray->objectAtIndex(idx));
    pCell->setItemId(itemInfo->getItemId());
    pCell->setDelegate(this);
    return pCell;
}

unsigned int WarriorUpgradeView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return m_pUpgradeInfoArray->count();
}

void WarriorUpgradeView::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)
{
    
}

void WarriorUpgradeView::upgradeResultCallBack( const Json::Value &requestData, const Json::Value &responseData )
{
	int criticalType =  responseData["data"]["criType"].asInt();
    if (criticalType < warrior::kUpgradeCriticalType_1 || criticalType > warrior::kUpgradeCriticalType_3) {
        return;
    }
	
    if (m_pAnimationNode) {
        m_pAnimationNode->removeAllChildrenWithCleanup(true);
        sp::Armature* pAnimation = ResourceManager::sharedResourceManager()->getUIAnimate(CCString::createWithFormat("warriorUpCritical%d", criticalType)->getCString());
        pAnimation->getAnimation()->addObserver(this, SPNotification_callfunc_selector(WarriorUpgradeView::animationPlayEnd), COMPLETE);
        m_pAnimationNode->addChild(pAnimation);
    }
	DM_GET_SOUND_MODEL->playCommonEffectSound(WARRIOR_ENHANCE_ACTION, 0);

	//todo
	//播放动画
}

void WarriorUpgradeView::animationPlayEnd(sp::Armature *target, const char *event, const char *data)
{
    if(m_pAnimationNode)
    {
        m_pAnimationNode->removeAllChildren();
    }
}


//
//WarriorUpgradeItemCell
//
WarriorUpgradeItemCell::WarriorUpgradeItemCell()
:m_pIconContainer(NULL)
,m_pCountNode(NULL)
,m_pCountLabel(NULL)
,m_pDelegate(NULL)
,m_pTouchMenu(NULL)
,m_itemId(0)
,m_itemUseCount(0)
,m_nSoundID(0)
{
    
}

WarriorUpgradeItemCell::~WarriorUpgradeItemCell()
{
    CC_SAFE_RELEASE(m_pIconContainer);
    CC_SAFE_RELEASE(m_pCountLabel);
    CC_SAFE_RELEASE(m_pCountNode);
    CC_SAFE_RELEASE(m_pTouchMenu);
}

WarriorUpgradeItemCell* WarriorUpgradeItemCell::create(cocos2d::extension::CCData *ccbData)
{
    WarriorUpgradeItemCell* pRet = new WarriorUpgradeItemCell();
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

bool WarriorUpgradeItemCell::init(cocos2d::extension::CCData *pData)
{
    CCNodeLoaderLibrary * pCCNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pCCNodeLoaderLibrary);
    pCCNodeLoaderLibrary->registerCCNodeLoader("WarriorTypeMark", WarriorTypeMarkLoader::loader());
    
    CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pCCNodeLoaderLibrary);
    
    if (m_pCountNode) {
        m_pCountNode->setVisible(true);
    }
    
    if (m_pCountLabel) {
        m_pCountLabel->setString("0");
    }

//    CCScale9Sprite * button9Sprite = CCScale9Sprite::create("icons/item/1.png");
//    button9Sprite->setPreferredSize(CCSizeMake(100, 101));
//    CCControlButton* button = CCControlButton::create(button9Sprite);
//    button->setContentSize(CCSizeMake(100, 101));
//    button->setAdjustBackgroundImage(false);
//    button->addTargetWithActionForControlEvents(this, cccontrol_selector(WarriorUpgradeItemCell::upgradeButtonCallBack), CCControlEventTouchUpInside);
//    button->addTargetWithActionForControlEvents(this, cccontrol_selector(WarriorUpgradeItemCell::upgradeButtonLongPressCallBack), CCControlEventLongPress);
//    m_pIconContainer->addChild(button, 1);
    
    m_pTouchMenu->addTargetWithActionForControlEvents(this, cccontrol_selector(WarriorUpgradeItemCell::upgradeButtonCallBack), CCControlEventTouchUpInside);
    m_pTouchMenu->addTargetWithActionForControlEvents(this, cccontrol_selector(WarriorUpgradeItemCell::upgradeButtonLongPressCallBack), CCControlEventLongPress);
    m_pTouchMenu->addTargetWithActionForControlEvents(this, cccontrol_selector(WarriorUpgradeItemCell::upgradeButtonLongPressEndCallBack), CCControlEventLongPressEnd);
	m_pTouchMenu->setLongPressRepeatFaster(true);
    return true;
}

void WarriorUpgradeItemCell::upgradeButtonCallBack(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent controlEvent)
{
    if (m_pDelegate) {
        m_pDelegate->itemUseCallBack(m_itemId);
    }
	
	m_itemUseCount = 0;
}

void WarriorUpgradeItemCell::upgradeButtonLongPressCallBack(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent controlEvent)
{
	//CCLOG("upgradeButtonLongPressCallBack");
	item::ItemInfo * itemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(m_itemId);
	if (itemInfo == NULL || itemInfo->getNum() == 0)
	{
		return;
	}
	if(m_pDelegate && m_pDelegate->isLvlLimit(m_itemId))
	{
		upgradeButtonLongPressEndCallBack(m_pTouchMenu, CCControlEventLongPressEnd);
		return;
	}
	//判断铜币和经验丹是否还够
	warrior::UpgradeItemInfo* itemUpgradeInfo = DM_GET_WARRIOR_MODEL->getWarriorUpgradeItemInfoById(m_itemId);
	
	//int coinCount = DM_GET_USER_MODEL->getUserInfo()->getCoins();
	//int coinNeed = itemUpgradeInfo->getCoinConsum();
	//CCLOG("----------%d-%d" , coinCount , coinNeed);

	bool coinless = (itemUpgradeInfo == NULL) ? true : DM_GET_USER_MODEL->getUserInfo()->getCoins() < itemUpgradeInfo->getCoinConsum();
	bool itemless = itemInfo->getNum() - m_itemUseCount <= 0;
	if (coinless || itemless)
	{
		upgradeButtonLongPressEndCallBack(m_pTouchMenu, CCControlEventLongPressEnd);
		return;
	}
	m_nSoundID = SoundManager::sharedSoundManager()->playSFX(CCString::createWithFormat("skill/hero2"));
	unsigned int amountPerUse;
	if(m_itemUseCount > 700)
	{
		amountPerUse =  10;
	}
	else if(m_itemUseCount > 400)
	{
		amountPerUse =  5;
	}
	else if (m_itemUseCount > 200)
	{
		amountPerUse = 3;
	}
	else
	{
		amountPerUse = 1;
	}

	//铜币也可能不够了!
	if(DM_GET_USER_MODEL->getUserInfo()->getCoins() < amountPerUse * itemUpgradeInfo->getCoinConsum())
	{
		amountPerUse = 1;
	}

    m_itemUseCount += amountPerUse;
    //经验丹不够了
	if (m_itemUseCount > (int)itemInfo->getNum())
	{
		m_itemUseCount =  (int)itemInfo->getNum();
	}
	m_pCountLabel->setString(CCString::createWithFormat("%d", itemInfo->getNum() - m_itemUseCount)->getCString());
    
    if (m_pDelegate) {
        m_pDelegate->itemKeepUseCallBack(m_itemId, amountPerUse);
    }

	if(m_pDelegate && m_pDelegate->isLvlLimit(m_itemId))
	{
		upgradeButtonLongPressEndCallBack(m_pTouchMenu, CCControlEventLongPressEnd);
		return;
	}
}

//根据物品id计算出使用后的经验，判断是否超出限制
bool WarriorUpgradeView::isLvlLimit(unsigned int itemId)
{	
	/*warrior::UpgradeItemInfo* itemInfo = DM_GET_WARRIOR_MODEL->getWarriorUpgradeItemInfoById(itemId);
	int exp = itemInfo->getExperience();
	uint16_t currLevel = m_pWarriorInfo->getWarriorLv();
	int warriorExp = m_pWarriorInfo->getWarriorExp();
	warriorExp += exp;
	int warriorLv = DM_GET_WARRIOR_MODEL->getWarriorLevelByExp(warriorExp, m_pWarriorInfo->getBaseInfo()->getRealGrade(), currLevel);

	CCLog(CCString::createWithFormat("%d,%d",m_pWarriorInfo->getWarriorLv(),warriorLv)->getCString());
	if(warriorLv > USER_WARRIOR_LEVEL_RATIO*DM_GET_USER_MODEL->getUserInfo()->getUserLv())
	{
		return true;
	}

	return false;*/
	//return m_pWarriorInfo->checkLevelLimit();
	return m_pWarriorInfo->getWarriorLv()>USER_WARRIOR_LEVEL_RATIO*DM_GET_USER_MODEL->getUserInfo()->getUserLv();
}

void WarriorUpgradeItemCell::upgradeButtonLongPressEndCallBack( cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent )
{
	if (m_nSoundID) {
		SoundManager::sharedSoundManager()->stopSFX(m_nSoundID);
		m_nSoundID = 0;
	}
	if (m_pDelegate && m_itemUseCount > 0)
	{
		m_pDelegate->itemKeepUseEndCallBack(m_itemId, m_itemUseCount);
	}

	m_itemUseCount = 0;
}

void WarriorUpgradeItemCell::onEnter()
{
    CCTableViewCell::onEnter();
}

void WarriorUpgradeItemCell::onExit()
{
    CCTableViewCell::onExit();
}

bool WarriorUpgradeItemCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconContainer", CCNode*, this->m_pIconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCountLabel", CCLabelTTF*, this->m_pCountLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCountNode", CCNode*, m_pCountNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchMenu", CCControlButton*, m_pTouchMenu);
    return false;
}

void WarriorUpgradeItemCell::setItemId(unsigned int itemId)
{
    /*if (m_itemId == itemId) {
        return;
    }*/
    m_itemId = itemId;
    updateView();
}

void WarriorUpgradeItemCell::updateView()
{
    if (m_itemId == 0) {
        return;
    }
    
    m_pIconContainer->removeAllChildren();
    CCSprite* pIcon = ResourceManager::sharedResourceManager()->getItemIcon50P(m_itemId);
    if (pIcon) {
        m_pIconContainer->addChild(pIcon);
    }
    
    item::ItemInfo * itemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(m_itemId);
    if (m_pCountLabel) {
		if(itemInfo){
			m_pCountLabel->setString(CCString::createWithFormat("%d", itemInfo->getNum())->getCString());
		}
		else{
			//没这个东西，so，0
			m_pCountLabel->setString("0");
		}
    }
}

//DLongPressButton* DLongPressButton::create(cocos2d::extension::CCScale9Sprite *sprite)
//{
//    DLongPressButton* pRet = new DLongPressButton();
//    if (pRet && pRet->initWithBackgroundSprite(sprite)) {
//        pRet->autorelease();
//        pRet->setTouchEnabled(false);
//        pRet->setTouchEnabled(true);
//        return pRet;
//    }
//    
//    CC_SAFE_DELETE(pRet);
//    return NULL;
//}
//
//bool DLongPressButton::initWithBackgroundSprite(cocos2d::extension::CCScale9Sprite *sprite)
//{
//    return CCControlButton::initWithBackgroundSprite(sprite);
//}
//
//bool DLongPressButton::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//    bool touched = CCControlButton::ccTouchBegan(pTouch, pEvent);
//    
//    if (touched) {
//        schedule(schedule_selector(DLongPressButton::longPressCallBack), 1.0f);
//    }
//    
//    return touched;
//}
//
//void DLongPressButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
//{
//    CCControlButton::ccTouchMoved(pTouch, pEvent);
//}
//
//void DLongPressButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//    unscheduleAllSelectors();
//    
//    CCControlButton::ccTouchEnded(pTouch, pEvent);
//}
//
//void DLongPressButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
//{
//    unscheduleAllSelectors();
//    CCControlButton::ccTouchCancelled(pTouch, pEvent);
//}
//
//void DLongPressButton::waitForLongPress(float t)
//{
//    unschedule(schedule_selector(DLongPressButton::waitForLongPress));
//    
//    if (!isEnabled() || !isVisible() || !hasVisibleParents() )
//    {
//        return;
//    }
//    
//    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
//    {
//        if (c->isVisible() == false)
//        {
//            return ;
//        }
//    }
//
//    schedule(schedule_selector(DLongPressButton::longPressCallBack), 0.3f);
//}
//
//void DLongPressButton::longPressCallBack(float t)
//{
//    if (!isEnabled() || !isVisible() || !hasVisibleParents() )
//    {
//        unscheduleAllSelectors();
//        return;
//    }
//    
//    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
//    {
//        if (c->isVisible() == false)
//        {
//            unscheduleAllSelectors();
//            return ;
//        }
//    }
//
//    sendActionsForControlEvents(CCControlEventLongPress);
//}

