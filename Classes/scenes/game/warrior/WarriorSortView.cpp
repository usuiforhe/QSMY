#include "WarriorSortView.h"
#include "../common/common.h"

USING_NS_CC;
USING_NS_CC_EXT;

WarriorSortView::WarriorSortView()
	:m_pConfirmButton(NULL)
	,m_pBackButton(NULL)
	,m_pData(NULL)
	,m_pContainer(NULL)
	,m_pCurType(warrior::kSortTypeNone)
	,m_pSortCell(NULL)
{

}

WarriorSortView::~WarriorSortView()
{
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pBackButton);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE_NULL(m_pData);
	CC_SAFE_RELEASE_NULL(m_pSortCell);
}

bool WarriorSortView::init()
{
	if(!DLayer::init()) return false;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_hero/PL_hero_paixu");	

	m_pData = readCCData("PL_ui/ccb/pl_hero/PL_hero_paixu_btn");
	CC_SAFE_RETAIN(m_pData);

	m_pConfirmButton->getButton()->setTarget(this, menu_selector(WarriorSortView::onConfirmClick));
	m_pBackButton->getButton()->setTarget(this, menu_selector(WarriorSortView::onBackClick));

	DTableView* m_pTableView = DTableView::create(this, m_pContainer->getContentSize());
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setAnchorPoint(CCPointZero);
	m_pTableView->setPosition(CCPointZero);
	m_pTableView->setDelegate(this);
	m_pTableView->setFlyInStyle(kCCTableViewFlyInOnEnter);
	m_pContainer->addChild(m_pTableView);

	return true;
}

void WarriorSortView::onEnter()
{
	DLayer::onEnter();
}

void WarriorSortView::onExit()
{
	DLayer::onExit();
}

bool WarriorSortView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCloseButton",DButton*,m_pBackButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pConfirmButton",DButton*,m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pContainer", CCNode*, m_pContainer);
	return false;
}

cocos2d::CCSize WarriorSortView::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	return CCSizeMake(352, 76);
}

cocos2d::extension::CCTableViewCell* WarriorSortView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	warrior::WarriorSortType kType = static_cast<warrior::WarriorSortType>(idx+1);
	
	WarriorSortCell* pCell = static_cast<WarriorSortCell*>(table->dequeueCell());
	if(pCell == NULL)
	{
		pCell = WarriorSortCell::create(m_pData);
		pCell->setDelegate(this);
		pCell->setType(kType);
	}

	if(m_pCurType == kType)
	{
		pCell->setSelect(true);
	}
	else
	{
		pCell->setSelect(false);
	}
	return pCell;
}

unsigned int WarriorSortView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return warrior::kSortTypeCount-1;
}

void WarriorSortView::onBackClick(cocos2d::CCObject * sender)
{
	removeFromParent();
}

void WarriorSortView::onConfirmClick(cocos2d::CCObject * sender)
{
	m_pDelegate->setSortList(m_pCurType);
	removeFromParent();
}

void WarriorSortView::setSelectCell(WarriorSortCell* pCell)
{
	if(m_pSortCell == pCell)
	{
		return;
	}
	if(m_pSortCell!=NULL)
	{
		m_pSortCell->setSelect(false);
		m_pSortCell->release();
	}
	m_pSortCell = pCell;
	m_pCurType = warrior::kSortTypeNone;
	if(m_pSortCell!=NULL)
	{
		m_pSortCell->setSelect(true);
		m_pSortCell->retain();
		m_pCurType = m_pSortCell->getSortType();
	}
	
}

#pragma mark ----------------WarriorSortCell 排序单元基类------------------
WarriorSortCell::WarriorSortCell()
	:m_pCheckBox(NULL)
	,m_pDelegate(NULL)
	,m_pSortType(warrior::kSortTypeNone)
{
	D_RESET_C_ARRAY(m_pNodeBox,warrior::kSortTypeCount-1);
}

WarriorSortCell::~WarriorSortCell()
{
	D_SAFE_RELEASE_C_ARRAY(m_pNodeBox,warrior::kSortTypeCount-1);
	CC_SAFE_RELEASE(m_pCheckBox);
}

WarriorSortCell* WarriorSortCell::create(cocos2d::extension::CCData* pData)
{
	WarriorSortCell* pRet = new WarriorSortCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return NULL;
}

bool WarriorSortCell::init(cocos2d::extension::CCData* pData)
{
	 CCB_READER_DATA_AND_ADDCHILD(pData);
	 return true;
}

bool WarriorSortCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCheckBox",CCNode*,m_pCheckBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pGreenButton",CCNode*,m_pNodeBox[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pBlueButton",CCNode*,m_pNodeBox[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pPurpleButton",CCNode*,m_pNodeBox[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pOrangeButton",CCNode*,m_pNodeBox[3]);
	return false;
}

SEL_MenuHandler WarriorSortCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onClick", WarriorSortCell::onClick);
	return NULL;
}

void WarriorSortCell::setType(warrior::WarriorSortType pType)
{
	m_pSortType = pType;
	CCNode* pNode = NULL;
	for(int i=0;i<warrior::kSortTypeCount-1;i++)
	{
		pNode = m_pNodeBox[i];
		pNode->setVisible(false);
	}

	int index = static_cast<int>(pType);
	m_pNodeBox[index-1]->setVisible(true);
}

void WarriorSortCell::setSelect(bool boo)
{
	m_pCheckBox->setVisible(boo);
}

void WarriorSortCell::onClick(cocos2d::CCObject *sender)
{
	bool boo = !(m_pCheckBox->isVisible());
	if(m_pDelegate)
	{
		m_pDelegate->setSelectCell(boo?this:NULL);
	}
}