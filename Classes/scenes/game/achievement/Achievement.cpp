//
//  Achievement.cpp
//  QSMY
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "Achievement.h"
#include "../inventory/ItemInfoView.h"

USING_NS_CC;
USING_NS_CC_EXT;

Achievement::Achievement()
	:	m_pCloseButton(NULL)
	,	m_pCell(NULL)
	,	m_pTableContainer(NULL)
	,	m_pNodeContainer(NULL)
	,	m_pBMFPercent(NULL)
	,	m_pPBPercent(NULL)
	,	m_pModel(NULL)
	,	m_currentTag(0)
	,	m_pItemBox(NULL)
	,	m_pBMFScore(NULL)
	,	m_pBtnGet(NULL)
{
    D_RESET_C_ARRAY(m_pTableView, ACHIEVE_TAB_COUNT);
	D_RESET_C_ARRAY(m_pArr, ACHIEVE_TAB_COUNT);
	D_RESET_C_ARRAY(m_pBtn, ACHIEVE_TAB_COUNT);
	D_RESET_C_ARRAY(m_pTabLabel, ACHIEVE_TAB_COUNT);
}

Achievement::~Achievement()
{
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pTableContainer);
	CC_SAFE_RELEASE(m_pNodeContainer);
	CC_SAFE_RELEASE(m_pBMFPercent);
	CC_SAFE_RELEASE(m_pPBPercent);
	CC_SAFE_RELEASE(m_pBMFScore);
	CC_SAFE_RELEASE(m_pBtnGet);
	CC_SAFE_RELEASE(m_pItemBox);
	D_SAFE_RELEASE_C_ARRAY(m_pArr, ACHIEVE_TAB_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_pBtn, ACHIEVE_TAB_COUNT);
	//D_SAFE_RELEASE_C_ARRAY(m_pTableView, ACHIEVE_TAB_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_pTabLabel, ACHIEVE_TAB_COUNT);
}

bool Achievement::init()
{
    if(!DLayer::init()) return false;
    
	m_pModel = DM_GET_ACHIEVEMENT_MODEL;

	cocos2d::CCNode *pNode = NULL;
	CCB_READER("PL_ui/ccb/pl_chengjiu/PL_chengjiu",pNode);
	if(pNode) addChild(pNode);

	CCB_READER("PL_ui/ccb/pl_chengjiu/PL_chengjiu_tiaojian_cell", m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	//初始先把脚标隐藏
	for(int i = 0 ; i< ACHIEVE_TAB_COUNT ; i++){
		m_pTabLabel[i]->getParent()->setVisible(false);
	}
    
	m_pCloseButton->getButton()->setTarget(this, menu_selector(Achievement::closeCallBack));
	m_pBtnGet->getButton()->setTarget(this, menu_selector(Achievement::getScoreAward));

	DTabView *pTabView = DTabView::create(CCRectMake(m_pTableContainer->getPositionX(), 
		m_pTableContainer->getPositionY(),
		m_pTableContainer->getContentSize().width, 
		PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pTableContainer->getContentSize().height, -PL_MAX_HEIGHT_DIFF)),
		0);

	const char *fontFile[ACHIEVE_TAB_COUNT] = 
	{
		"PL_ui/chengjiu/chengjiu_wenzi_zhandou.png",
		"PL_ui/chengjiu/chengjiu_wenzi_dengji.png",
		"PL_ui/chengjiu/chengjiu_wenzi_shouji.png",
		"PL_ui/chengjiu/chengjiu_wenzi_rongyu.png"
	};

	for (int i = 0; i < ACHIEVE_TAB_COUNT; i++)
	{
		m_pTableView[i] = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pTableContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
		m_pTableView[i]->setDelegate(this);
		m_pTableView[i]->setTag(i);
		m_pTableView[i]->setVerticalFillOrder(kCCTableViewFillTopDown);
		m_pTableView[i]->setFlyInStyle(kCCTableViewFlyInReload);
		pTabView->addTab(m_pTableView[i], "","PL_ui/beibao/pl_bb_btn_xiaofenye.png","PL_ui/beibao/pl_bb_btn_xiaofenye_up.png",NULL,"PL_ui/beibao/pl_bb_btn_xiaofenye_up.png");

		m_pBtn[i]->setTag(i);
		m_pBtn[i]->setTarget(this, menu_selector(Achievement::tabCallBack));
	}
	pTabView->setTabSize(CCSizeMake(1,1));

	m_pBtn[0]->setEnabled(false);
	m_pBtn[0]->selected();
	pTabView->setDelegate(this);
	m_pNodeContainer->addChild(pTabView);
	m_pTabView = pTabView;

	PL_MOVE_NODE_BELOW_CENTER_Y(pNode);
    return true;
}

void Achievement::onEnter()
{
	//绑定
	BINDING_ONLY(m_pModel, this, Achievement::updateView, D_EVENT_ACHIEVEMENT_UPDATE);
	BINDING_ONLY(m_pModel, this, Achievement::updateAward, D_EVENT_ACHIEVEMENT_SCOREAWARD);
	//刷新数据
	m_pModel->getListFromServer();
	DLayer::onEnter();
}

void Achievement::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

void Achievement::updateAward()
{
	unsigned int myscore = m_pModel->getCurrentScore();
	unsigned int myindex = m_pModel->getScoreRewardId();
	achievement::AwardInfo* pAward = NULL;
	pAward = dynamic_cast<achievement::AwardInfo*>(m_pModel->getDictAward()->objectForKey(myindex));
	if(pAward)
	{
		//更新盒子
		m_pItemBox->updateDropItem(drop::Drop::create(pAward->getType() , pAward->getID() , pAward->getAmount()));
		//更新level
		m_pBMFScore->setString(D_CSTRING_FROM_UINT(pAward->getScore()));
		m_pBtnGet->getButton()->setEnabled(myscore>=pAward->getScore());
		//更新进度条
		m_pBMFPercent->setString(CCString::createWithFormat("%u/%u", m_pModel->getCurrentScore(), pAward->getScore())->getCString());
		float pencent = (float)m_pModel->getCurrentScore() / pAward->getScore();
		m_pPBPercent->setPercent(MIN(pencent , 1.0f));
	}
	else{
		//防止出错
		m_pBtnGet->getButton()->setEnabled(false);
		//进度条
		m_pBMFPercent->setString(CCString::createWithFormat("%u/%u", m_pModel->getCurrentScore(), m_pModel->getTotalScore())->getCString());
		float pencent = (float)m_pModel->getCurrentScore() / m_pModel->getTotalScore();
		m_pPBPercent->setPercent(MIN(pencent , 1.0f));
	}
}

void Achievement::getScoreAward(CCObject*)
{
	unsigned int myindex = m_pModel->getScoreRewardId();
	HTTP_CLIENT->getTaskScoreAward(myindex);
}

void Achievement::onView(unsigned int id , drop::DropType type)
{
	if(type == drop::kDropTypeItem)
	{
		ItemInfoView *m_pItemInfo = ItemInfoView::create();
		m_pItemInfo->setItemInfo(item::ItemInfo::create(id, 0));
		addChild(m_pItemInfo);
	}
}

void Achievement::SelectTabAtIndex(DTabView *tabView, uint32_t idx)
{
	if (m_pArr[idx] == NULL)
	{
		m_pArr[idx] = m_pModel->getAchievementByCategory((achievement::TaskCategory)(idx + 1));
		CC_SAFE_RETAIN(m_pArr[idx]);
	}
	m_pTableView[idx]->reloadData();
}

void Achievement::tabCallBack(CCObject *pObj)
{
	CCMenuItem *pItem = static_cast<CCMenuItem *>(pObj);
	int tag = pItem->getTag();
	m_pTabView->selectedTab(tag);
	m_currentTag = tag;
	for (int i = 0; i < ACHIEVE_TAB_COUNT; i++)
	{
		m_pBtn[i]->setEnabled(i != tag);
		if (i == tag)
		{
			m_pBtn[i]->selected();
		}
		else
		{
			m_pBtn[i]->unselected();
		}
	}
	
}

bool Achievement::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableRect", CCNode *, m_pTableContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeContainer", CCNode *, m_pNodeContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPercent", CCLabelBMFont *, m_pBMFPercent);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPBPercent", DProgressBar *, m_pPBPercent);
	D_CCB_ASSIGN_ARRAY("m_pBtn", CCMenuItem *, m_pBtn, ACHIEVE_TAB_COUNT);
	D_CCB_ASSIGN_ARRAY("m_pTabLabel", CCLabelBMFont *, m_pTabLabel, ACHIEVE_TAB_COUNT);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBox", ItemBox *, m_pItemBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnGet", DButton *, m_pBtnGet);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFScore", CCLabelBMFont *, m_pBMFScore);
    return false;
}

CCSize Achievement::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

CCTableViewCell *Achievement::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	AchievementCell *pCell = (AchievementCell *)(table->dequeueCell());
	if (!pCell)
	{
		pCell = AchievementCell::create();
	}
	pCell->setData((achievement::TaskInfo *)m_pArr[table->getTag()]->objectAtIndex(idx));
	pCell->setDelegate(this);
	return pCell;
}

unsigned int Achievement::numberOfCellsInTableView(CCTableView *table)
{
	int tag = table->getTag();
	if (m_pArr[tag] == NULL)
	{
		return 0;
	}
	
	return m_pArr[tag]->count();
}

void Achievement::closeCallBack(CCObject* obj)
{
    Game::sharedGame()->NavigateTo(navigation::kNavHome);
}

void Achievement::updateView()
{
	for(int i = 0 ; i< ACHIEVE_TAB_COUNT ; i++){
		uint32_t num = m_pModel->getAchievedNumByCate((achievement::TaskCategory)(i+1));
		if(num > 0)
		{
			m_pTabLabel[i]->getParent()->setVisible(true);
			m_pTabLabel[i]->setString(CCString::createWithFormat("%d",num)->getCString());
		}
		else
		{
			m_pTabLabel[i]->getParent()->setVisible(false);
		}		
	}

	//更新当前列表
	CC_SAFE_RELEASE(m_pArr[m_currentTag]);
	m_pArr[m_currentTag] = m_pModel->getAchievementByCategory((achievement::TaskCategory)(m_currentTag + 1));
	CC_SAFE_RETAIN(m_pArr[m_currentTag]);
	m_pTableView[m_currentTag]->reloadData();
}




using namespace sp;

AchievementEffect::AchievementEffect()
	:m_pFlash(NULL)
{

}

AchievementEffect::~AchievementEffect()
{
	CC_SAFE_RELEASE(m_pFlash);
}

AchievementEffect* AchievementEffect::create()
{
	AchievementEffect* pRet = new AchievementEffect();
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}else
	{
		CC_SAFE_DELETE(pRet);
	}
	return NULL;
}

bool AchievementEffect::init()
{
	if(!DEffectNode::init()) return false;
	m_pFlash =  ResourceManager::sharedResourceManager()->getUIAnimate("achievement",sp::ZORDER);
	m_pFlash->getAnimation()->addObserver(this, SPNotification_callfunc_selector(AchievementEffect::animateEnd), COMPLETE);
	m_pFlash->setPosition(D_DESIGN_POINT_CENTER);
	CC_SAFE_RETAIN(m_pFlash);
	showFlash();
	return true;
}

void AchievementEffect::showFlash()
{
	m_pFlash->getAnimation()->playByIndex(0);
	addChild(m_pFlash);
}

void AchievementEffect::animateEnd(sp::Armature * target, const char* event,const char* data)
{
	callback();
	removeFromParent();
}