//
//  GuildTreeView.h
//  公会天工树
//
//  Created by Eci on 13-10-29.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "GuildTreeView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#define UNSELECT -1
#include "GuildHead.h"
#include "GuildTreeCell.h"

using namespace sp;

GuildTreeView::GuildTreeView()
	:
	m_pLabelName(NULL),
	m_pLabelPosition(NULL),
	m_pLabelLevel(NULL),
	m_pLabelExp(NULL),
	m_pExpBar(NULL),
	m_pLabelNextExp(NULL),
	m_pRichLabelNotice0(NULL),
	m_pRichLabelNotice1(NULL),
	m_pRichLabelNotice2(NULL),
	m_pDelegate(NULL)
	,m_pNodeChivalry(NULL)
	,m_pNodeOverlord(NULL)
	,m_pButtonChivalry(NULL)
	,m_pButtonOverlord(NULL)
	,m_pButtonCancel(NULL)
	//,m_pGuildHead(NULL)
	,m_pModel(NULL)
	,m_pContainer(NULL)
	,m_pLabelRest(NULL)
	,m_pLabelChivalry(NULL)
	,m_pLabelOverlord(NULL)
	,m_pLabelCoinsCount(NULL)
	,m_pLabelItemsRest(NULL)
	,m_pLabelTimesRest(NULL)
	,m_pButtonItem1(NULL)
	,m_pButtonItem10(NULL)
	,m_pButtonCoins(NULL)
	,m_pListView(NULL)
	,m_iType(0)
	,m_isExpShown(false)
{
}

GuildTreeView::~GuildTreeView()
{
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelPosition);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelExp);
	CC_SAFE_RELEASE(m_pExpBar);
	CC_SAFE_RELEASE(m_pLabelNextExp);
	CC_SAFE_RELEASE(m_pRichLabelNotice0);
	CC_SAFE_RELEASE(m_pRichLabelNotice1);
	CC_SAFE_RELEASE(m_pRichLabelNotice2);
	CC_SAFE_RELEASE(m_pNodeChivalry);
	CC_SAFE_RELEASE(m_pNodeOverlord);
	CC_SAFE_RELEASE(m_pButtonChivalry);
	CC_SAFE_RELEASE(m_pButtonOverlord);
	CC_SAFE_RELEASE(m_pButtonCancel);
	//CC_SAFE_RELEASE(m_pGuildHead);
	CC_SAFE_RELEASE(m_pLabelRest);
	CC_SAFE_RELEASE(m_pLabelChivalry);
	CC_SAFE_RELEASE(m_pLabelOverlord);
	CC_SAFE_RELEASE(m_pLabelCoinsCount);
	CC_SAFE_RELEASE(m_pLabelItemsRest);
	CC_SAFE_RELEASE(m_pLabelTimesRest);
	CC_SAFE_RELEASE(m_pButtonItem1);
	CC_SAFE_RELEASE(m_pButtonItem10);
	CC_SAFE_RELEASE(m_pButtonCoins);
	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pContainer);
}

GuildTreeView* GuildTreeView::create()
{
	GuildTreeView *pRet = new GuildTreeView();
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

bool GuildTreeView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILD_MODEL;

	//cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	//CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	//pNodeLoaderLibrary->registerCCNodeLoader("GuildHead", GuildHeadLoader::loader());
	////CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/guild/guildTree", pNodeLoaderLibrary);
	//CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_banghui/pl_banghui_jianshe", pNodeLoaderLibrary);

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_jianshe");
	
	//m_pButtonCancel->setTarget(this, menu_selector(GuildTreeView::callBackCancel));


	m_pButtonChivalry->setTag(0);
	m_pButtonOverlord->setTag(1);
	m_pButtonChivalry->setTarget(this, menu_selector(GuildTreeView::callBackType));
	m_pButtonOverlord->setTarget(this, menu_selector(GuildTreeView::callBackType));

	m_pButtonItem1->getButton()->setTag(0);
	m_pButtonItem10->getButton()->setTag(1);
	m_pButtonCoins->getButton()->setTag(2);
	m_pButtonItem1->getButton()->setTarget(this, menu_selector(GuildTreeView::callBackUpgrade));
	m_pButtonItem10->getButton()->setTarget(this, menu_selector(GuildTreeView::callBackUpgrade));
	m_pButtonCoins->getButton()->setTarget(this, menu_selector(GuildTreeView::callBackUpgrade));
	
	m_pListView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillBottomUp);
	m_pListView->retain();
	m_pContainer->addChild(m_pListView);

	setType(0);

	return true;
}

void GuildTreeView::onEnter()
{
	DLayer::onEnter();

	
	BINDING_EXEC(m_pModel, this, GuildTreeView::updateView, D_EVENT_GUILD_GET);
	BINDING_ONLY(m_pModel, this, GuildTreeView::updateTree, D_EVENT_GUILD_TREE);
	BINDING_ONLY(m_pModel, this, GuildTreeView::updateNotice, D_EVENT_GUILD_GET_NOTICE);
//	m_pModel->getGuildNoticeByType(guild::GuildNoticeTypeOverlord);
//	m_pModel->getGuildNoticeByType(guild::GuildNoticeTypeChivalry);
}

void GuildTreeView::updateView()
{
	guild::GuildInfo *guildInfo = m_pModel->getGuildInfo();
	guild::GuildUser *selfInfo = m_pModel->getSelfInfo();
	if(guildInfo && selfInfo) GUILD_HEAD_UPDATE(guildInfo , selfInfo);

	/*m_pGuildHead->setName(guildInfo->getName().c_str());
	m_pGuildHead->setLevel(guildInfo->getLevel());
	if (m_isExpShown)
	{
		m_pGuildHead->setGuildExp(guildInfo->getContributionLevel(), guildInfo->getContributionNext(), .2f);
	}
	else
	{
		m_pGuildHead->setGuildExp(guildInfo->getContributionLevel(), guildInfo->getContributionNext(), .0f);
		m_isExpShown = true;
	}
	m_pGuildHead->setUserExp(selfInfo->getNextExp());
	m_pGuildHead->setKnighthood(selfInfo->getKnighthoodName().c_str());*/
	updateTree();
}

void GuildTreeView::updateNotice()
{
	//if (1 == m_iType)
	//{
	//	m_pGuildHead->setNotice(m_pModel->getGuildNoticeByType(guild::GuildNoticeTypeOverlord));
	//}
	//else
	//{
	//	m_pGuildHead->setNotice(m_pModel->getGuildNoticeByType(guild::GuildNoticeTypeChivalry));
	//}
	m_pRichLabelNotice0->setString("");
	m_pRichLabelNotice1->setString("");
	m_pRichLabelNotice2->setString("");
	CCArray *tmpArray = m_pModel->getGuildNoticeByType(guild::GuildNoticeTypeNone);
	if (!tmpArray)
	{
		return;
	}
	CCArray *noticeArray = CCArray::create();
	for (unsigned int i = 0; i < NOTICE_MAX && i < tmpArray->count(); i++)
	{
		noticeArray->addObject(tmpArray->objectAtIndex(i));
	}
	if (noticeArray && noticeArray->count() > 0)
	{
		noticeArray->reverseObjects();
		guild::GuildNotice *notice0 = (guild::GuildNotice *)noticeArray->objectAtIndex(0);
		m_pRichLabelNotice0->setString(notice0->getMarquee().c_str());
		if (noticeArray->count() > 1)
		{
			guild::GuildNotice *notice1 = (guild::GuildNotice *)noticeArray->objectAtIndex(1);
			m_pRichLabelNotice1->setString(notice1->getMarquee().c_str());
			if (noticeArray->count() > 2)
			{
				guild::GuildNotice *notice2 = (guild::GuildNotice *)noticeArray->objectAtIndex(2);
				m_pRichLabelNotice2->setString(notice2->getMarquee().c_str());
			}
		}
	}
}

void GuildTreeView::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
	m_isExpShown = false;
}

bool GuildTreeView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_position", CCLabelTTF *, this->m_pLabelPosition);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp", CCLabelBMFont *, this->m_pLabelExp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp_bar", DProgressBar *, this->m_pExpBar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_next_exp", CCLabelBMFont *, this->m_pLabelNextExp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_0", CCRichLabelTTF *, this->m_pRichLabelNotice0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_1", CCRichLabelTTF *, this->m_pRichLabelNotice1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_2", CCRichLabelTTF *, this->m_pRichLabelNotice2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_cancel", CCMenuItem *, this->m_pButtonCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_items", CCNode *, this->m_pNodeChivalry);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_coins", CCNode *, this->m_pNodeOverlord);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_chivalry", CCMenuItem *, this->m_pButtonChivalry);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_overlord", CCMenuItem *, this->m_pButtonOverlord);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_rest", CCLabelBMFont *, this->m_pLabelRest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_chivalry_level", CCLabelBMFont *, this->m_pLabelChivalry);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_overlord_level", CCLabelBMFont *, this->m_pLabelOverlord);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_coins_count", CCLabelBMFont *, this->m_pLabelCoinsCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_items_rest", CCLabelBMFont *, this->m_pLabelItemsRest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_times_rest", CCLabelBMFont *, this->m_pLabelTimesRest);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_head", GuildHead *, this->m_pGuildHead);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_item1", DButton *, this->m_pButtonItem1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_item10", DButton *, this->m_pButtonItem10);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_enter", DButton *, this->m_pButtonCoins);
	return false;
}


void GuildTreeView::callBackCancel(CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->willCloseTreeView();
	}
}

void GuildTreeView::setType(int iType)
{
	m_iType = iType;
	if (1 == m_iType)
	{
		m_pNodeChivalry->setVisible(false);
		m_pNodeOverlord->setVisible(true);
		m_pButtonChivalry->unselected();
		m_pButtonOverlord->selected();
	}
	else
	{
		m_pNodeChivalry->setVisible(true);
		m_pNodeOverlord->setVisible(false);
		m_pButtonChivalry->selected();
		m_pButtonOverlord->unselected();
	}
	this->updateTree();
	this->updateNotice();
}

void GuildTreeView::callBackType(CCObject *sender)
{
	CCMenuItem *pItem = dynamic_cast<CCMenuItem *>(sender);
	int tag = pItem->getTag();
	setType(tag);
}

void GuildTreeView::updateTree()
{
	uint64_t count = 0;
	uint32_t level = 0;
	uint32_t maxLevel = 0;
	uint32_t rest = 0;
	if (1 == m_iType)
	{
		count = m_pModel->getTreeOverlord()->getItemCountRest();
		level = m_pModel->getTreeOverlord()->getLevel();
		maxLevel = m_pModel->getMaxLevelOverlord();
		rest = D_CONFIG_UINT_FOR_KEY(GUILD_MAX_OVERLORD_COUNT) - m_pModel->getSelfInfo()->getOverlord();
	}
	else
	{
		count = m_pModel->getTreeChivalry()->getItemCountRest();
		level = m_pModel->getTreeChivalry()->getLevel();
		maxLevel = m_pModel->getMaxLevelChivalry();
		rest = D_CONFIG_UINT_FOR_KEY(GUILD_MAX_CHIVALRY_COUNT) - m_pModel->getSelfInfo()->getChivalry();
		uint32_t itemNum = DM_GET_ITEM_MODEL->getItemNum(K_ITEM_GUILD_ID);
		m_pLabelItemsRest->setString(CCString::createWithFormat("%u", itemNum)->getCString());
	}
	if (level >= maxLevel)
	{
		m_pLabelRest->setString(CCString::createWithFormat("--", count)->getCString());
	}
	else
	{
		if (1 == m_iType)
		{
			//m_pLabelRest->setString(CCString::createWithFormat("%llu", count / 10000)->getCString());
			m_pLabelRest->setString(D_CSTRING_WITH10K_FROM_UINT(count));
		}
		else
		{
			m_pLabelRest->setString(CCString::createWithFormat("%llu", count)->getCString());
		}
	}
	m_pLabelTimesRest->setString(CCString::createWithFormat("%u", rest)->getCString());
	m_pLabelCoinsCount->setString(D_CSTRING_WITH10K_FROM_UINT(m_pModel->getSelfInfo()->getCoinsNeeded()));
	//m_pLabelChivalry->setString(CCString::createWithFormat("%u", m_pModel->getTreeChivalry()->getLevel())->getCString());
	//m_pLabelOverlord->setString(CCString::createWithFormat("%u", m_pModel->getTreeOverlord()->getLevel())->getCString());
	if (level > maxLevel - 2)
	{
		int height = TREE_CELL_SIZE.height * maxLevel;
		height -= m_pContainer->getContentSize().height;
		m_pListView->setContentOffset(ccp(0, - height));
	}
	else if (level <= 2)
	{
		m_pListView->setContentOffset(ccp(0, 0));
	}
	else
	{
		m_pListView->setContentOffset(ccp(0, - TREE_CELL_SIZE.height * (level - 2)));
	}
	m_pListView->reloadData();
}

void GuildTreeView::callBackUpgrade(CCObject *sender)
{
	CCMenuItem *pItem = dynamic_cast<CCMenuItem *>(sender);
	int type = 0;
	uint32_t num = 0;
	switch (pItem->getTag())
	{
	case 0:
		type = 2;
		num = 1;
		break;
	case 1:
		type = 2;
		num = 10;
		break;
	case 2:
		type = 1;
		num = m_pModel->getSelfInfo()->getCoinsNeeded();
		break;
	default:
		break;
	};
	if (0 == num)
	{
		Tips_Alert(D_LOCAL_STRING("GuildTimesError").c_str());
		return;
	}
	if (1 == type)
	{
		if (DM_GET_USER_MODEL->getUserInfo()->getCoins() < num)
		{
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_COIN);
			return;
		}
	}
	else
	{
		if (m_pModel->getSelfInfo()->getChivalry() >= D_CONFIG_UINT_FOR_KEY(GUILD_MAX_CHIVALRY_COUNT))
		{
			Tips_Alert(D_LOCAL_STRING("GuildTimesError").c_str());
			return;
		}
		else if (m_pModel->getSelfInfo()->getChivalry() + num > D_CONFIG_UINT_FOR_KEY(GUILD_MAX_CHIVALRY_COUNT))
		{
			num = D_CONFIG_UINT_FOR_KEY(GUILD_MAX_CHIVALRY_COUNT) - m_pModel->getSelfInfo()->getChivalry();
		}
		if (DM_GET_ITEM_MODEL->getItemNum(K_ITEM_GUILD_ID) == 0)
		{
			//Tips_Alert(D_LOCAL_STRING("GuildItemsError").c_str());
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS,CCInteger::create(K_ITEM_GUILD_ID));
			return;
		}
		else if (DM_GET_ITEM_MODEL->getItemNum(K_ITEM_GUILD_ID) < num)
		{
			num = DM_GET_ITEM_MODEL->getItemNum(K_ITEM_GUILD_ID);
		}
	}
	HTTP_CLIENT->guildUpgradeTree(type, num, this, callfuncJson_selector(GuildTreeView::callBackUpgrade));
}


void GuildTreeView::callBackUpgrade(const Json::Value &requestData,const Json::Value &responseData)
{

	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;


	Armature* pArmature =  GuildExpEffect::getArmature(m_pModel->getGuildInfo()->getContributionAdded(), m_pModel->getSelfInfo()->getContributionAdded());
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(GuildTreeView::callBackShowExp), COMPLETE);
    addChild(pArmature);


	//GuildExpEffect* m_pGuildEffect = GuildExpEffect::create();
	//addChild(m_pGuildEffect);
	//m_pGuildEffect->playEffect(m_pModel->getGuildInfo()->getContributionAdded(), m_pModel->getSelfInfo()->getContributionAdded());
}
void GuildTreeView::callBackShowExp(Armature * target, const char* event,const char* data)
{
	target->removeFromParent();
	this->updateView();
}

CCTableViewCell* GuildTreeView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{

	GuildTreeCell *cell = (GuildTreeCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildTreeCell::create();
	}
	uint32_t level = 0;
	guild::GuildTreeSkill *pData = NULL;
	if (1 == m_iType)
	{
		level = m_pModel->getTreeOverlord()->getLevel();
	}
	else
	{
		level = m_pModel->getTreeChivalry()->getLevel();
	}
	pData = m_pModel->getTreeSkillByLevelAndType(idx + 1, m_iType);

	cell->setData(pData);
	cell->setEnabled(level > idx);
	cell->setType(m_iType);
	return cell;
}

uint32_t GuildTreeView::numberOfCellsInTableView(CCTableView *table)
{
	if (1 == m_iType)
	{
		return m_pModel->getMaxLevelOverlord();
	}
	else
	{
		return m_pModel->getMaxLevelChivalry();
	}
}


CCSize GuildTreeView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return TREE_CELL_SIZE;
}

void GuildTreeView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
/*	m_iSelectIndex = m_iSelectIndex == cell->getIdx() ?UNSELECT :cell->getIdx();
	DTableView * DTable = dynamic_cast<DTableView *>(table);
	DTable->reloadData();
*/}
