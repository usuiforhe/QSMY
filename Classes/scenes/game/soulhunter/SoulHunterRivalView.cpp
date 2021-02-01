//
//  SoulHunterRivalView.cpp
//  狩魂选择对手
//
//  Created by liuxiaogang on 13-10-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulHunterRivalView.h"
#include "SoulHunterWarriorView.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  SOULHUNTER_RIVAL_CELL_SIZE		CCSizeMake(640, 120)


#pragma mark - SoulHunterRivalView
#pragma mark - - init etc.
SoulHunterRivalView::SoulHunterRivalView()
:m_pData(NULL)
,m_pTargetWarrior(NULL)
,m_pNoRivalSprite(NULL)
,m_pRivalListContainer(NULL)
,m_pRefreshRivalMenuItem(NULL)
,m_pCloseRivalMenuItem(NULL)
,m_pListView(NULL)
,m_pModel(NULL)
,m_pcurWid(0)
,m_pDelegate(NULL)
{
}

SoulHunterRivalView::~SoulHunterRivalView()
{
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pTargetWarrior);
	CC_SAFE_RELEASE(m_pNoRivalSprite);
	CC_SAFE_RELEASE(m_pRivalListContainer);
	CC_SAFE_RELEASE(m_pRefreshRivalMenuItem);
	CC_SAFE_RELEASE(m_pCloseRivalMenuItem);
	CC_SAFE_RELEASE(m_pListView);	
}


SoulHunterRivalView*  SoulHunterRivalView::create()
{
	SoulHunterRivalView* pRet = new SoulHunterRivalView();
	if(pRet&& pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}


bool SoulHunterRivalView::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());

		cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
		pNodeLoaderLibrary->registerCCNodeLoader("SoulHunterWarriorView", SoulHunterWarriorViewLoader::loader());
		CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/soulHunter/soulHunterList",pNodeLoaderLibrary);

		//CCB_READER_AND_ADDCHILD("ui/ccb/soulHunter/soulHunterList");
		bRet = true;
	}while(0);

	m_pModel = DM_GET_SOUL_HUNTER_MODEL;

	m_pData = readCCData("ui/ccb/soulHunter/soulHunterListCell");
	CC_SAFE_RETAIN(m_pData);
	
	m_pListView = DTableView::create(this, m_pRivalListContainer->getContentSize());
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setAnchorPoint(CCPointZero);
	m_pListView->setPosition(m_pRivalListContainer->getPosition());
	CC_SAFE_RETAIN(m_pListView);
	addChild(m_pListView);
	return bRet;
}




bool SoulHunterRivalView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTargetWarrior",  SoulHunterWarriorView*, this->m_pTargetWarrior);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoRivalSprite",  cocos2d::CCSprite*, this->m_pNoRivalSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRivalListContainer",  cocos2d::CCNode*, this->m_pRivalListContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRefreshRivalMenuItem", CCMenuItem *, this->m_pRefreshRivalMenuItem); 
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseRivalMenuItem", CCMenuItem *, this->m_pCloseRivalMenuItem); 
    return false;
}


void SoulHunterRivalView::onExit()
{
	UNBINDING_ALL(m_pModel, this);
    DLayer::onExit();
}


void SoulHunterRivalView::onEnter()
{
	BINDING_EXEC(m_pModel, this, SoulHunterRivalView::updateRivals, D_EVENT_SOULHUNTER_RIVALS_UPDATED);
	BINDING_ONLY(m_pModel, this, SoulHunterRivalView::refreshRivals, D_EVENT_SOULHUNTER_NOT_EXIST);
	m_pRefreshRivalMenuItem->setTarget(this, menu_selector(SoulHunterRivalView::RefreshRivalsCallBack));
	m_pCloseRivalMenuItem->setTarget(this, menu_selector(SoulHunterRivalView::closeCallBack));
    DLayer::onEnter();
}

void SoulHunterRivalView::refreshRivals()
{
	if (m_pcurWid!=0)
	{
		HTTP_CLIENT->huntTargets(m_pcurWid);
	}
}


void SoulHunterRivalView::RefreshRivalsCallBack(cocos2d::CCObject* sender)
{
	refreshRivals();
}


void SoulHunterRivalView::setCurWid(uint32_t wid)
{
	m_pcurWid=wid;
}


void SoulHunterRivalView::updateRivals()
{
	m_pListView->reloadData();
	if (m_pModel->getRivals()->count() > 0)
	{
		m_pRefreshRivalMenuItem->setVisible(true);				//有对手
		m_pNoRivalSprite->setVisible(false);					//放在这里 根据更新数据设置精灵的可见性
	}
	else{
		m_pRefreshRivalMenuItem->setVisible(false);				//无对手
		m_pNoRivalSprite->setVisible(true);
	}
}


CCSize SoulHunterRivalView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return SOULHUNTER_RIVAL_CELL_SIZE;							//应该返回列表单行的大小
}


CCTableViewCell* SoulHunterRivalView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	SoulHunterRivalCell *cell = (SoulHunterRivalCell*)table->dequeueCell();

	if (!cell)
	{
		cell = SoulHunterRivalCell::create(m_pData);
	}
	cell->setData(static_cast<soulhunter::SoulHunterRivalInfo *>(m_pModel->getRivals()->objectAtIndex(idx)));
	cell->getButtonSnatch()->setTarget(this, menu_selector(SoulHunterRivalView::onSnatchClick));
	cell->getButtonSnatch()->setTag(idx);
	return cell;
}


void SoulHunterRivalView::onSnatchClick(cocos2d::CCObject *sender)
{
	CCMenuItem* pItem = static_cast<CCMenuItem*>(sender);
	int tag = pItem->getTag();
	soulhunter::SoulHunterRivalInfo * targetRival=static_cast<soulhunter::SoulHunterRivalInfo *>(m_pModel->getRivals()->objectAtIndex(tag));
	DM_GET_SOUL_HUNTER_MODEL->setLastBattle(targetRival->getUid(),m_pcurWid,targetRival->getIsNpc());	//保存当前攻击请求 用以再战
	DM_GET_SOUL_HUNTER_MODEL->doLastBattleRequest();                                                    //当前子弟的编号 自动保存掉落的东西
}


uint32_t SoulHunterRivalView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pModel->getRivals()->count();
}


void SoulHunterRivalView::closeCallBack(cocos2d::CCObject* sender)
{
	removeFromParent();
	if(m_pDelegate) getDelegate()->changeRinalViewClosed();
}

void SoulHunterRivalView::setData(warrior::WarriorCurInfo * pWarrior)
{
	m_pTargetWarrior->setData(pWarrior);
	setCurWid(pWarrior->getWid());
}

#pragma mark -SoulHunterRivalCell-
SoulHunterRivalCell::SoulHunterRivalCell()
:m_pRivalInfo(NULL)
,m_pContainer(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pSnatchButton(NULL)
{

}

SoulHunterRivalCell:: ~SoulHunterRivalCell()
{
	CC_SAFE_RELEASE(m_pRivalInfo);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pSnatchButton);
}

SoulHunterRivalCell * SoulHunterRivalCell::create(cocos2d::extension::CCData* pData)
{
	SoulHunterRivalCell* pRet = new SoulHunterRivalCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}else
	{
		CC_SAFE_DELETE(pRet);
	}
	return NULL;
}

bool SoulHunterRivalCell::init(cocos2d::extension::CCData* pData)
{
	CCB_READER_DATA_AND_ADDCHILD(pData);
	return CCTableViewCell::init();
}


bool SoulHunterRivalCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSnatchButton", cocos2d::CCMenuItem *, this->m_pSnatchButton);

	return false;
}

void SoulHunterRivalCell::setData(soulhunter::SoulHunterRivalInfo *pRival)
{
    CC_SAFE_RETAIN(pRival);
    CC_SAFE_RELEASE(m_pRivalInfo);
	m_pRivalInfo = pRival;
    m_pContainer->removeAllChildren();
    
	if(pRival == NULL)
	{
		setVisible(false);
		return;
	}
	else
	{
		m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pRivalInfo->getAvatarId()));
		m_pLevel->setString(CCString::createWithFormat("%d",m_pRivalInfo->getLv())->getCString());		
		m_pName->setString(m_pRivalInfo->getName().c_str());
		setVisible(true); 
	}  
}

