//
//  WarriorFateView.cpp
//  缘
//
//  Created by cll on 13-7-24
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "WarriorFate.h"
#include "managers/GameManager.h"
#include "utils/StringUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  FATE_LIST_SIZE		CCSizeMake(640, 255)

#define  FATE_CELL_SIZE		CCSizeMake(640, 73)

WarriorFateView::WarriorFateView()
:m_pListView(NULL)
,m_pFateArray(NULL)
,m_pCCData(NULL)
,m_pWarrior(NULL)
{
};

WarriorFateView::~WarriorFateView()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pFateArray);
    CC_SAFE_RELEASE(m_pWarrior);
	CC_SAFE_RELEASE(m_pCCData);
}


bool WarriorFateView::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pCCData = readCCData("ui/ccb/warrior/warriorFateCell");
	CC_SAFE_RETAIN(m_pCCData);
    
	m_pListView = DTableView::create(this, FATE_LIST_SIZE);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setAnchorPoint(CCPointZero);
	m_pListView->setPosition(CCPointZero);
	addChild(m_pListView);
    
	return true;
    
}

void WarriorFateView::onEnter()
{
	
    DLayer::onEnter();
	
}

void WarriorFateView::onExit()
{
	DLayer::onExit();
}

void WarriorFateView::setFateArray(cocos2d::CCArray *pFateArray,warrior::WarriorBaseInfo* pWarrior)
{
    setFateArray(pFateArray, warrior::WarriorCurInfo::create(pWarrior));
}


void WarriorFateView::setFateArray(CCArray *pFateArray,warrior::WarriorCurInfo* pWarrior)
{
    CC_SAFE_RETAIN(pWarrior);
    CC_SAFE_RELEASE(m_pWarrior);
    m_pWarrior = pWarrior;
	
	CC_SAFE_RETAIN(pFateArray);
    CC_SAFE_RELEASE(m_pFateArray);
	m_pFateArray = pFateArray;
    
	updateView();
}

void WarriorFateView::updateView()
{
	m_pListView->reloadData();
	m_pListView->setContentOffset(m_pListView->minContainerOffset());
}


void WarriorFateView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	uint32_t idx =  cell->getIdx();
    
	CCLOG("rank cell touched at index: %i", idx);
    
}



CCSize WarriorFateView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return FATE_CELL_SIZE;
}

CCTableViewCell* WarriorFateView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	
	WarriorFateCell *cell = (WarriorFateCell*)table->dequeueCell();
    
	if (!cell)
	{
		cell = WarriorFateCell::create(m_pCCData);
		
	}
    
	cell->setFateInfo((warrior::FateInfo *)m_pFateArray->objectAtIndex(idx),m_pWarrior);
    
	return cell;
}

uint32_t WarriorFateView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pFateArray?m_pFateArray->count():0;
}



WarriorFateCell::WarriorFateCell()
:m_pFateInfo(NULL)
,m_pFateName(NULL)
,m_pFateDesc(NULL)
,m_pPropAdd(NULL)
,m_pPropWord(NULL)
,m_pStateAct(NULL)
,m_pStateNotAct(NULL)
,m_pWarrior(NULL)
{
    
}



WarriorFateCell::~WarriorFateCell()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pWarrior);
	CC_SAFE_RELEASE(m_pFateInfo);
	CC_SAFE_RELEASE(m_pFateName);
	CC_SAFE_RELEASE(m_pFateDesc);
	CC_SAFE_RELEASE(m_pPropAdd);
	CC_SAFE_RELEASE(m_pPropWord);
	CC_SAFE_RELEASE(m_pStateAct);
	CC_SAFE_RELEASE(m_pStateNotAct);
}


WarriorFateCell* WarriorFateCell::create(cocos2d::extension::CCData* pData)
{
	WarriorFateCell* pRet = new WarriorFateCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
	}else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool WarriorFateCell::init(cocos2d::extension::CCData* pData)
{
	CCB_READER_DATA_AND_ADDCHILD(pData);
	return true;
}


void WarriorFateCell::setFateInfo(warrior::FateInfo *pFateInfo,warrior::WarriorCurInfo* pWarrior)
{
    
	CC_SAFE_RETAIN(pWarrior);
    CC_SAFE_RELEASE(m_pWarrior);
    m_pWarrior = pWarrior;
    
	CC_SAFE_RETAIN(pFateInfo);
	CC_SAFE_RELEASE(m_pFateInfo);
	m_pFateInfo = pFateInfo;
    
	updateCellView();
}


void WarriorFateCell::updateCellView()
{
	if(DM_GET_WARRIOR_MODEL->isMyWarrior(m_pWarrior) && DM_GET_WARRIOR_MODEL->checkFateActivated(m_pFateInfo))
	{
		m_pStateAct->setVisible(true);
		m_pStateNotAct->setVisible(false);
        
	}
	else
	{
		m_pStateAct->setVisible(false);
		m_pStateNotAct->setVisible(true);
	}
    
	m_pFateName->setString(m_pFateInfo->name.c_str());
	m_pPropWord->removeAllChildren();
    
	std::string  desc = "";
	uint32_t addNum = 0;
	CCSprite * pPropWord = NULL;
    
	if (m_pFateInfo->type == warrior::kFateTypeWarrior)
	{
		CCInteger *member_id = NULL;
		CCObject *obj = NULL;
		const char *warrior_name = NULL;
		
        
		std::vector<std::string> warriorNames;
        
		CCARRAY_FOREACH(m_pFateInfo->member_ids, obj)
		{
			member_id = (CCInteger*)obj;
			warrior_name = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(member_id->getValue())->getWarriorName().c_str();
			warriorNames.push_back(CCString::createWithFormat("%s",warrior_name)->getCString());
		}
        
		if(m_pFateInfo->add_attack>0)
		{
			pPropWord = CCSprite::create("ui/common/font_pro_gong.png");
			addNum = m_pFateInfo->add_attack;
		}
		else if(m_pFateInfo->add_defence>0)
		{
			pPropWord = CCSprite::create("ui/common/font_pro_fang.png");
			addNum = m_pFateInfo->add_defence;
		}
		else
		{
			pPropWord = CCSprite::create("ui/common/font_pro_xue.png");
			addNum = m_pFateInfo->add_hp;
		}
        
		m_pPropWord->addChild(pPropWord);
		m_pPropAdd->setString(CCString::createWithFormat("+%d%%",addNum)->getCString());
        
		desc = CCString::create(D_LOCAL_STRING("FateDesc1%s",join(warriorNames,", ").c_str()).c_str())->getCString();
        
	}
	else if(m_pFateInfo->equip_id>0)
	{
		const char *equip_name = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(m_pFateInfo->equip_id)->getEquipName().c_str();
        
		if(m_pFateInfo->add_attack>0)
		{
			pPropWord = CCSprite::create("ui/common/font_pro_gong.png");
			addNum = m_pFateInfo->add_attack;
		}
		else if(m_pFateInfo->add_defence>0)
		{
			pPropWord = CCSprite::create("ui/common/font_pro_fang.png");
			addNum = m_pFateInfo->add_defence;
		}
		else
		{
			pPropWord = CCSprite::create("ui/common/font_pro_xue.png");
			addNum = m_pFateInfo->add_hp;
		}
        
		m_pPropWord->addChild(pPropWord);
		m_pPropAdd->setString(CCString::createWithFormat("+%d%%",addNum)->getCString());
        
		desc = CCString::create(D_LOCAL_STRING("FateDesc2%s",equip_name))->getCString();
        
	}
	
	m_pFateDesc->setString(desc.c_str());
    
}


bool WarriorFateCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStateAct", CCScale9Sprite*, this->m_pStateAct);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStateNotAct", CCScale9Sprite*, this->m_pStateNotAct);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateName", CCLabelTTF*, this->m_pFateName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateInfo", CCLabelTTF*, this->m_pFateDesc);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropAdd", CCLabelBMFont*, this->m_pPropAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropWord", CCNode*, this->m_pPropWord);
	return false;
}

