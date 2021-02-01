#include "RankView.h"
#include "managers/DataManager.h"
#include "Rank.h"

using namespace rank;

RankView::RankView()
	:m_pCCBData(NULL)
	,m_pRankTable(NULL)
	,m_pRankList(NULL)
	
{
	
}

RankView::~RankView()
{
	CC_SAFE_RELEASE(m_pCCBData);
}

RankView* RankView::create(RankType rankType, const cocos2d::CCSize& size)
{
	RankView* p = new RankView();
	if(p && p->init(rankType, size))
	{
		p->autorelease();
		return p;
	} else 
	{
		CC_SAFE_DELETE(p);
		p = NULL;
	}
	return NULL;
}

bool RankView::init(RankType rankType, const cocos2d::CCSize& size)
{
	if(!DLayer::init())
	{
		return false;
	}

	m_eRankType = rankType;

	m_pRankModel = DM_GET_RANK_MODEL;
	m_pRankList = m_pRankModel->getRankListByType(m_eRankType);

	m_pCCBData = readCCData("PL_ui/ccb/pl_paihang/PL_paihang_cell");
	CC_SAFE_RETAIN(m_pCCBData);

	m_pRankTable = DTableView::create(this, size);
	m_pRankTable->setDirection(kCCScrollViewDirectionVertical);
	m_pRankTable->setDelegate(this);
	m_pRankTable->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pRankTable->setFlyInStyle(kCCTableViewFlyInOnEnter);
	
	this->addChild(m_pRankTable);

	return true;
}

void RankView::onEnter()
{
	BINDING_ONLY(m_pRankModel, this, RankView::updateRankList, RANK_LIST_UPDATE);
	updateRankList();
	DLayer::onEnter();
}

void RankView::onExit()
{
	UNBINDING_ALL(m_pRankModel, this);
	DLayer::onExit();
}


void RankView::updateRankList()
{
	m_pRankList = m_pRankModel->getRankListByType(m_eRankType);
	m_pRankTable->reloadData();
}

cocos2d::extension::CCTableViewCell* RankView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	RankCell* pCell = (RankCell*)table->dequeueCell();
	if(!pCell)
	{
		pCell = RankCell::create(m_pCCBData);
	}
	pCell->setRankInfo((UserRankInfo*)m_pRankList->objectAtIndex(idx));
	return pCell;
}
unsigned int RankView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return m_pRankList->count();
}
void RankView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{

}

cocos2d::CCSize RankView::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	return CCSizeMake(633, 80);
}





#pragma mark ---------------RankCell--------------------

RankCell::RankCell()
	:m_pNormal(NULL),
	m_pName(NULL),
	m_pLevelVal(NULL),
	m_pRankVal(NULL),
	m_pVipLevel(NULL),
	m_pRankInfo(NULL)
{
	D_RESET_C_ARRAY(m_pLevel, RANK_LEVEL_HIGHT_LIGHT_NUM);
	D_RESET_C_ARRAY(m_pTypeLabel, RANK_TYPE_COUNT);
}
RankCell::~RankCell()
{
	D_SAFE_RELEASE_C_ARRAY(m_pLevel, RANK_LEVEL_HIGHT_LIGHT_NUM);
	CC_SAFE_RELEASE(m_pNormal);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pLevelVal);
	CC_SAFE_RELEASE(m_pRankVal);
	CC_SAFE_RELEASE(m_pVipLevel);
	D_SAFE_RELEASE_C_ARRAY(m_pTypeLabel, RANK_TYPE_COUNT);
	CC_SAFE_RELEASE(m_pRankInfo);
}

RankCell* RankCell::create(cocos2d::extension::CCData* ccbData)
{
	RankCell* p = new RankCell();
	if(p && p->init(ccbData))
	{
		p->autorelease();
		return p;
	} else 
	{
		CC_SAFE_DELETE(p);
		p = NULL;
	}
	return NULL;
}

bool RankCell::init(cocos2d::extension::CCData* ccbData)
{
	CCB_READER_DATA_AND_ADDCHILD(ccbData);

	return true;
}

//ccb method
bool RankCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNormal", CCNode*, m_pNormal);
	D_CCB_ASSIGN_ARRAY("m_pLevel", CCNode*, m_pLevel, RANK_LEVEL_HIGHT_LIGHT_NUM);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF*, m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelVal", CCLabelBMFont*, m_pLevelVal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRankVal", CCLabelBMFont*, m_pRankVal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipLevel", CCLabelBMFont*, m_pVipLevel);
	D_CCB_ASSIGN_ARRAY("m_pTypeLabel", CCNode*, m_pTypeLabel, RANK_TYPE_COUNT);
	return false;
}

SEL_MenuHandler RankCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallback", RankCell::menuCallback);
	return NULL;
}

void RankCell::menuCallback(cocos2d::CCObject *obj)
{
	Rank::shareRank()->showOtherFormation(m_pRankInfo->getUserID());
	
}

void RankCell::setRankInfo(rank::UserRankInfo* pRankInfo)
{
	CC_SAFE_RELEASE(m_pRankInfo);
	CC_SAFE_RETAIN(pRankInfo);
	m_pRankInfo = pRankInfo;
	for(uint8_t i=0; i<RANK_LEVEL_HIGHT_LIGHT_NUM; i++)
	{
		m_pLevel[i]->setVisible(pRankInfo->getRank() - 1 == i);
	}
	m_pNormal->setVisible(pRankInfo->getRank() > 3);
	
	m_pName->setString(pRankInfo->getName().c_str());
	m_pLevelVal->setString(D_CSTRING_FROM_UINT(pRankInfo->getRank()));
	m_pRankVal->setString(D_CSTRING_FROM_UINT(pRankInfo->getNum()));
	m_pVipLevel->setString(D_CSTRING_FROM_UINT(pRankInfo->getVip()));

	for(int i(0); i<RANK_TYPE_COUNT; i++)
	{
		m_pTypeLabel[i]->setVisible(false);
	}
	m_pTypeLabel[pRankInfo->getType() - 1]->setVisible(true);
}