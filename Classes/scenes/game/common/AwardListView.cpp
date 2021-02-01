//
//  AwardListView.cpp
//  奖品列表
//
//  Created by cll on 13-7-18
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "AwardListView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "model/DropModel.h"
#include "AwardItemView.h"
#include "gui/AwardItemSmall.h"
#include "utils/SceneUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;


#define  AWARD_CELL_SIZE    CCSizeMake(640, 140)


#define  CELL_ITEM_NUM		5			//一行道具个数


DPopup * AwardViewFactory::create(cocos2d::CCArray *pAwardList)
{
	
	if(pAwardList->count()>1){
		AwardListView* pRet= new AwardListView();
		
		if(pRet&& pRet->init(pAwardList))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			CC_SAFE_DELETE(pRet);
		}
	}	
	else
	{
		AwardItemView* pRet = new AwardItemView();
		if(pRet&& pRet->init((drop::Drop*)pAwardList->objectAtIndex(0)))
		{
			pRet->autorelease();
			return pRet;
		}else
		{
			CC_SAFE_DELETE(pRet);
		}
	}
	return NULL;
}

BaseAwardListView::BaseAwardListView()
:m_pListView(NULL)
,m_pAwardList(NULL)
,m_pCCData(NULL)
,m_pContainer(NULL)
{
};

BaseAwardListView::~BaseAwardListView()
{
    CC_SAFE_RELEASE(m_pCCData);
	CC_SAFE_RELEASE(m_pListView);
    CC_SAFE_RELEASE(m_pAwardList);
	CC_SAFE_RELEASE(m_pContainer);
}

/**
 *	@brief	初始化
 *
 *	@param 	pAwardList 	奖励列表，元素类型为:Drop *
 *
 *	@return	是否成功
 */
bool BaseAwardListView::init(cocos2d::CCArray *pAwardList)
{
	m_pAwardList = pAwardList;
    CC_SAFE_RETAIN(m_pAwardList);
	return init();
}

bool BaseAwardListView::init()
{
	if(!DPopup::init(true))
	{
		return false;
	}
    addChildrenBeforeList();
	//m_pCCData = readCCData("ui/ccb/common/awardCell");
	m_pCCData = readCCData("PL_ui/ccb/pl_beibao/PL_beibao_row");
	CC_SAFE_RETAIN(m_pCCData);
    
	CC_ASSERT(m_pContainer);
	m_pListView = DTableView::create(this, m_pContainer->getContentSize());
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setAnchorPoint(CCPointZero);
	CC_SAFE_RETAIN(m_pListView);
    m_pContainer->addChild(m_pListView);
    return true;
}

bool BaseAwardListView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", cocos2d::CCNode *, this->m_pContainer);
	return false;
}

void BaseAwardListView::onEnter()
{    
	updateAwardListView();
    DPopup::onEnter();
}

void BaseAwardListView::onExit()
{
	DPopup::onExit();
}


void BaseAwardListView::updateAwardListView()
{
	m_pListView->reloadData();
}

void BaseAwardListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}



CCSize BaseAwardListView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return AWARD_CELL_SIZE;
}

CCTableViewCell* BaseAwardListView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	if (!m_pAwardList) return NULL;
	
	AwardListViewCell *cell = (AwardListViewCell*)table->dequeueCell();
	CCObject *pObject0 = NULL;
	CCObject *pObject1 = NULL;
	CCObject *pObject2 = NULL;
	CCObject *pObject3 = NULL;
	CCObject *pObject4 = NULL;
    CCArray * pAwardList = NULL;
	uint32_t count = m_pAwardList->count();
	
    
	if (count>CELL_ITEM_NUM*idx)
	{
		pObject0 = m_pAwardList->objectAtIndex(CELL_ITEM_NUM*idx);
	}
    
	if (count>CELL_ITEM_NUM*idx+1)
	{
		pObject1 = m_pAwardList->objectAtIndex(CELL_ITEM_NUM*idx+1);
	}
    
	if (count>CELL_ITEM_NUM*idx+2)
	{
		pObject2 = m_pAwardList->objectAtIndex(CELL_ITEM_NUM*idx+2);
	}

	if (count>CELL_ITEM_NUM*idx+3)
	{
		pObject3 = m_pAwardList->objectAtIndex(CELL_ITEM_NUM*idx+3);
	}

	if (count>CELL_ITEM_NUM*idx+4)
	{
		pObject4 = m_pAwardList->objectAtIndex(CELL_ITEM_NUM*idx+4);
	}
	
	if (!cell)
	{
		cell = AwardListViewCell::create(m_pCCData);
	}
    
	pAwardList= CCArray::create(pObject0,pObject1,pObject2,pObject3,pObject4,NULL);
	cell->setAwardItems(pAwardList);
    
	return cell;
    
}

uint32_t BaseAwardListView::numberOfCellsInTableView(CCTableView *table)
{
	if(!m_pAwardList) return 0;
    
	uint32_t cellNum =  (m_pAwardList->count() + CELL_ITEM_NUM - 1)/CELL_ITEM_NUM;
    return cellNum;
}

#pragma mark - AwardListView -


AwardListView::AwardListView()
:BaseAwardListView()
,m_pEffectNode(NULL)
,m_pConfirmButton(NULL)
,m_pCloseButton(NULL)
,m_pTitleCommon(NULL)
,m_pTitleSoul(NULL)
{
	
};

AwardListView::~AwardListView()
{
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pConfirmButton);
    CC_SAFE_RELEASE(m_pEffectNode);
	CC_SAFE_RELEASE(m_pTitleCommon);
	CC_SAFE_RELEASE(m_pTitleSoul);
}

bool AwardListView::init(cocos2d::CCArray *pAwardList)
{
    if(pAwardList==NULL) return false;
    if(!BaseAwardListView::init(pAwardList)) return false;
    
    m_pEffectNode->removeAllChildren();
    m_pEffectNode->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("rewardmulti"));
    return true;
}

void AwardListView::closeCallBack(cocos2d::CCObject *obj)
{
	removeFromParent();
}

void AwardListView::shareCallBack(cocos2d::CCObject* obj)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHARE, CCStringMake(D_LOCAL_STRING("ShareMessage")));

	removeFromParent();
}

void AwardListView::addChildrenBeforeList()
{    
	//CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardMulti");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_shiliankai");

	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardListView::closeCallBack));
	m_pConfirmButton->getButton()->setTarget(this,menu_selector(AwardListView::closeCallBack));

	drop::Drop* firstDrop = (drop::Drop*)m_pAwardList->objectAtIndex(0);
	if(firstDrop->getType() == drop::kDropTypeSoul){
		m_pTitleCommon->setVisible(false);
		m_pTitleSoul->setVisible(true);
	}
	
	//m_pDBtnShare->getButton()->setTarget(this,menu_selector(AwardListView::shareCallBack));
}



bool AwardListView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleCommon", CCSprite *, this->m_pTitleCommon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleSoul", CCSprite *, this->m_pTitleSoul);

    return BaseAwardListView::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}



AwardListViewCell::AwardListViewCell()
:m_pAwardItems(NULL)
,m_pItem1Node(NULL)
,m_pItem2Node(NULL)
,m_pItem3Node(NULL)
,m_pItem4Node(NULL)
,m_pItem5Node(NULL)
{
    
}

AwardListViewCell::~AwardListViewCell()
{
	CC_SAFE_RELEASE(m_pAwardItems);
	CC_SAFE_RELEASE(m_pItem1Node);
	CC_SAFE_RELEASE(m_pItem2Node);
	CC_SAFE_RELEASE(m_pItem3Node);
	CC_SAFE_RELEASE(m_pItem4Node);
	CC_SAFE_RELEASE(m_pItem5Node);
}


AwardListViewCell* AwardListViewCell::create(cocos2d::extension::CCData* pData)
{
    AwardListViewCell* pRet = new AwardListViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
    }else
    {        
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool AwardListViewCell::init(cocos2d::extension::CCData* pData)
{
    //CCB_READER_DATA_AND_ADDCHILD(pData);

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("AwardItemSmall", AwardItemSmallLoader::loader());
    CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pNodeLoaderLibrary);
	//CCB_READER_DATA_AND_ADDCHILD(pData);
	return true;
}

void AwardListViewCell::setAwardItems(CCArray * pAwardItems)
{
	if(!pAwardItems || pAwardItems == m_pAwardItems) return;
	
	CC_SAFE_RELEASE(m_pAwardItems);
	CC_SAFE_RETAIN(pAwardItems);
	m_pAwardItems = pAwardItems;

	uint32_t count = m_pAwardItems->count();

	m_pItem1Node->updateView(count>0?(drop::Drop*)m_pAwardItems->objectAtIndex(0):NULL);
	m_pItem2Node->updateView(count>1?(drop::Drop*)m_pAwardItems->objectAtIndex(1):NULL);
	m_pItem3Node->updateView(count>2?(drop::Drop*)m_pAwardItems->objectAtIndex(2):NULL);

	m_pItem4Node->updateView(count>3?(drop::Drop*)m_pAwardItems->objectAtIndex(3):NULL);
	m_pItem5Node->updateView(count>4?(drop::Drop*)m_pAwardItems->objectAtIndex(4):NULL);
 
}

bool AwardListViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItem1Node", AwardItemSmall*, this->m_pItem1Node);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItem2Node", AwardItemSmall*, this->m_pItem2Node);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItem3Node", AwardItemSmall*, this->m_pItem3Node);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItem4Node", AwardItemSmall*, this->m_pItem4Node);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItem5Node", AwardItemSmall*, this->m_pItem5Node);
    
    return false;
}





