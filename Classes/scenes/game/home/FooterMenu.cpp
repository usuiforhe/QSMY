#include "FooterMenu.h"
#include "../Game.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "../account/VipView.h"
#include "../inventory/ChangeName.h"

#include "../account/Account.h"

#define LOTTERY_CARD_COUNT 4

USING_NS_CC;
USING_NS_CC_EXT;
using namespace navigation;
using namespace sp;

FooterMenuView::FooterMenuView()
	: m_pPopMenuNode(NULL)
	, m_pSettingButton(NULL)
	,m_pFriendButton(NULL)
	,m_pRankButton(NULL)
    ,m_pOtherButton(NULL)
	,m_pAchievementLogo(NULL)
	,m_pAchievementCount(NULL)
	,m_pMallLogo(NULL)
	,m_pMallCount(NULL)
	,m_pAvailableBg(NULL)
	,m_pAvailableLabel(NULL)
{
    
}

void FooterMenuView::onEnter()
{
    DLayer::onEnter();
    
	//碎片角标
	BINDING_EXEC(DM_GET_ITEM_MODEL,this,FooterMenuView::updateAvailable,D_EVENT_AVAILABLE_EQUIP);

	//进入时更新
	BINDING_EXEC(DM_GET_LOTTERY_MODEL, this, FooterMenuView::updateMall, D_EVENT_LOTTERY_INITED);
	for (int i = 1; i < LOTTERY_CARD_COUNT; i++)
	{
		BINDING_ONLY(DM_GET_LOTTERY_MODEL->getCardInfoByType((card::CardType)i), this, FooterMenuView::updateMall, D_EVENT_LOTTERY_CARDINFO_CHANGED);
	}
	
	uint32_t achievementCount = DM_GET_ACHIEVEMENT_MODEL->getCurrentAchievement();
	updateAchievement(achievementCount);

    setTouchPriority(DTOUCH_FOOTER_MENU_VIEW);
    setTouchEnabled(true);
}


void FooterMenuView::onExit()
{
	UNBINDING_ALL(DM_GET_ITEM_MODEL,this);
    DLayer::onExit();
}

void FooterMenuView::updateAvailable()
{
	unsigned int num = DM_GET_ITEM_MODEL->getAvailableEquipNum();
	m_pAvailableBg->setVisible(num != 0);
	m_pAvailableLabel->setString(D_CSTRING_FROM_UINT(num));
}

FooterMenuView::~FooterMenuView()
{
    CC_SAFE_RELEASE(m_pPopMenuNode);
    CC_SAFE_RELEASE(m_pSettingButton);
    CC_SAFE_RELEASE(m_pFriendButton);
    CC_SAFE_RELEASE(m_pRankButton);
    CC_SAFE_RELEASE(m_pOtherButton);
	CC_SAFE_RELEASE(m_pAchievementLogo);
	CC_SAFE_RELEASE(m_pAchievementCount);
	CC_SAFE_RELEASE(m_pMallLogo);
	CC_SAFE_RELEASE(m_pMallCount);
	CC_SAFE_RELEASE(m_pAvailableBg);
	CC_SAFE_RELEASE(m_pAvailableLabel);
}

bool FooterMenuView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPopMenuNode", CCNode*, this->m_pPopMenuNode);
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSettingButton", DButton*, this->m_pSettingButton);
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFriendButton", DButton*, this->m_pFriendButton);
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRankButton", DButton*, this->m_pRankButton);
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOtherButton", CCMenuItemImage*, this->m_pOtherButton);

   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAchievementLogo", CCNode*, this->m_pAchievementLogo);
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAchievementCount", CCLabelBMFont*, this->m_pAchievementCount);
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMallLogo", CCNode*, this->m_pMallLogo);
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMallCount", CCLabelBMFont*, this->m_pMallCount);
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvailableBg", CCNode *, this->m_pAvailableBg);
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvailableLabel", CCLabelBMFont *, this->m_pAvailableLabel);

    return false;
}

SEL_MenuHandler FooterMenuView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackFooter", FooterMenuView::menuCallBack);
    return NULL;
}

void FooterMenuView::updateAchievement(uint32_t count)
{
	if(count == 0){
		//0 不显示
		m_pAchievementLogo->setVisible(false);
	}
	else{
		m_pAchievementLogo->setVisible(true);
		m_pAchievementCount->setString(D_CSTRING_FROM_UINT(count));
	}
}

void FooterMenuView::updateMall()
{
	unsigned int count = DM_GET_LOTTERY_MODEL->getFreeCount();
	if(count == 0){
		//0 不显示
		m_pMallLogo->setVisible(false);
	}
	else{
		//m_pMallLogo->setVisible(true);
		m_pMallCount->setString(D_CSTRING_FROM_UINT(count));
	}
}

void FooterMenuView::removeFromParentAndCleanup(bool cleanup)
{
    DLayer::removeFromParentAndCleanup(cleanup);
    setZOrder(0);//重置zorder
}

bool FooterMenuView::init()
{
    
    if ( !DLayer::init() )
    {
        return false;
    }
    
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_home/PL_Home_xia");

	this->setPositionY(0);

	if (m_pSettingButton)
	{
		m_pSettingButton->getButton()->setTag(kTagSetting);
		m_pSettingButton->getButton()->setTarget(this, menu_selector(FooterMenuView::menuCallBack));
	}

	if (m_pFriendButton)
	{
		m_pFriendButton->getButton()->setTag(kTagFriend);

		//m_pFriendButton->getButton()->setEnabled(false);
		m_pFriendButton->getButton()->setTarget(this, menu_selector(FooterMenuView::menuCallBack));
	}

	if (m_pRankButton)
	{
		m_pRankButton->getButton()->setTag(kTagRank);
		//临时屏蔽
		m_pRankButton->getButton()->setEnabled(true);
		m_pRankButton->getButton()->setTarget(this, menu_selector(FooterMenuView::menuCallBack));
	}
	
	//PL_MOVE_NODE_BELOW_CENTER_Y(this);

    return true;
}

void FooterMenuView::registerWithTouchDispatcher()
{
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    
    pDispatcher->addTargetedDelegate(this, getTouchPriority(), false);
}

bool FooterMenuView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (! m_bVisible )
    {
        return false;
    }
    
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }

    CCPoint worldTouchPoint_World = convertToWorldSpace(convertTouchToNodeSpace(pTouch));
    if (m_pPopMenuNode->boundingBox().containsPoint(worldTouchPoint_World)) {
        return false;
    }
    
    CCRect buttonRect = CCRectMake(0, 0, m_pOtherButton->getContentSize().width, m_pOtherButton->getContentSize().height);
    if (buttonRect.containsPoint(m_pOtherButton->convertTouchToNodeSpace(pTouch))) {
        return false;
    }
    
    if (m_pPopMenuNode && m_pPopMenuNode->isVisible()) {
        m_pPopMenuNode->setVisible(false);
    }
    
    return false;
}

void FooterMenuView::menuCallBack(CCObject *pObj)
{
	CCMenuItem *pItem = dynamic_cast<CCMenuItem *>(pObj);
	if (!pItem)
	{
		return;
	}
	switch(pItem->getTag())
	{
	case kTagHome:
		Game::sharedGame()->NavigateTo(navigation::kNavHome);
		break;
	case kTagItem:
		Game::sharedGame()->NavigateTo(navigation::kNavInventory);
		break;
	case kTagEquip:
		Game::sharedGame()->NavigateTo(navigation::kNavEquip);
		break;
	case kTagMission:
		Game::sharedGame()->NavigateTo(navigation::kNavAchievement);
		break;
	case kTagSetting:
		Game::sharedGame()->showSetting();
		if(m_pPopMenuNode) m_pPopMenuNode->setVisible(false);
		break;
	case kTagMall:
		Game::sharedGame()->NavigateTo(navigation::kNavMall);
		break;
	case kTagOther:
		if (m_pPopMenuNode)
		{
			m_pPopMenuNode->setVisible(!m_pPopMenuNode->isVisible());
		}
		break;
	case kTagFriend:
		Game::sharedGame()->NavigateTo(navigation::kNavFriends);
		if(m_pPopMenuNode) m_pPopMenuNode->setVisible(false);
		break;
	case kTagRank:
		Game::sharedGame()->NavigateTo(navigation::kNavRank);
		if(m_pPopMenuNode) m_pPopMenuNode->setVisible(false);
		break;
	default:
		;
	}
}

void FooterMenuView::setNavigateDisplay( navigation::NavigationType value )
{
	//TODO: 导航分页分类型显示
	setVisible(true);
	switch (value) {
	case kNavHome:              //首页
		setVisible(true);
		break;
	case kNavPve:               //pve

		break;
	case kNavFormation:         //阵容

		break;
	case kNavExperience:

		break;
	case kNavWonder:            //奇遇（活动）

		break;
	case kNavMall:              //市集（商城）

		break;
	case kNavLadder:            //论剑 pvp

		break;
	case kNavInventory:         //背包

		break;
		//case kNavBook:              //武林谱（图鉴）
		//
		//    break;
		//case kNavAnnounce:            //公告
		//    setVisible(false);
		break;
	case kNavSetting:           //设置
		setVisible(false);
		break;
	case kNavWarrior:           //弟子
		break;
	case kNavEquip:             //装备

		break;
	case kNavTreasure:          //心法

		break;
	case kNavDestiny:           //意志

		break;
	case kNavLottery:           //抽卡

		break;
	case kNavMatch:             //杯赛

		break;
	case kNavActiveness:        //活跃度
		setVisible(false);
		break;
	case kNavMail:              //邮件
		break;
	case kNavWish:              //许愿
		break;
	case kNavPrize:             //领奖
		setVisible(false);
		break;
	case kNavChargePrize:       //累充
		setVisible(false);
		break;
	case kNavStrategy:       //累充
		setVisible(false);
		break;
	case kNavAchievement:
		break;
	case kNavSignin:
		setVisible(false);
		break;
	case kNavActivity:
		setVisible(false);
		break;
	default:
		break;
	}
}
