#include "PictureFilter.h"
#include "../common/common.h"
USING_NS_CC;
USING_NS_CC_EXT;
PictureFilter::PictureFilter()
:m_pConfirmButton(NULL)
,m_pBackButton(NULL)
,m_pContainer(NULL)
,m_pData(NULL)
,m_pCurGrade(warrior::kWarFilterNone)
,m_pSortCell(NULL)
{

}
PictureFilter::~PictureFilter()
{
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pBackButton);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE_NULL(m_pData);
	CC_SAFE_RELEASE_NULL(m_pSortCell);
}
bool PictureFilter::init()
{
	if(!DPopup::init()) return false;
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_hero/PL_hero_tujian_fenlei");	
	m_pData = readCCData("PL_ui/ccb/pl_hero/PL_hero_tujian_fenlei_cell");
	CC_SAFE_RETAIN(m_pData);
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(PictureFilter::onConfirmClick));
	m_pBackButton->getButton()->setTarget(this, menu_selector(PictureFilter::onBackClick));
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
void PictureFilter::onEnter()
{
	DPopup::onEnter();
}

void PictureFilter::onExit()
{
	DPopup::onExit();
}
bool PictureFilter::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCloseButton",DButton*,m_pBackButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pConfirmButton",DButton*,m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pContainer", CCNode*, m_pContainer);
	return false;
}
cocos2d::CCSize PictureFilter::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	return CCSizeMake(352, 76);
}
cocos2d::extension::CCTableViewCell* PictureFilter::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	warrior::WarriorHandBookFilter kGrade = static_cast<warrior::WarriorHandBookFilter>(idx+1);
	PictureFilterCell* pCell = static_cast<PictureFilterCell*>(table->dequeueCell());
	if(pCell == NULL)
	{
		pCell = PictureFilterCell::create(m_pData);
		pCell->setDelegate(this);
		pCell->setType(kGrade);
	}
	if(m_pCurGrade == kGrade)
	{
		pCell->setSelect(true);
	}
	else
	{
		pCell->setSelect(false);
	}
	return pCell;
}
unsigned int PictureFilter::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return (warrior::kWarFilterCount)-1;
}
void PictureFilter::onBackClick(cocos2d::CCObject * sender)
{
	removeFromParent();
}
void PictureFilter::onConfirmClick(cocos2d::CCObject * sender)
{
	m_pDelegate_t->setWarriorListFilter(m_pCurGrade);
	removeFromParent();
}
void PictureFilter::setSelectCell(PictureFilterCell* pCell)
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
	m_pCurGrade = warrior::kWarFilterNone;
	if(m_pSortCell!=NULL)
	{
		m_pSortCell->setSelect(true);
		m_pSortCell->retain();
		m_pCurGrade = m_pSortCell->getFilterGrade();
	}
}
#pragma mark ----------------PictureFilterCell 排序单元基类------------------
PictureFilterCell::PictureFilterCell()
:m_pCheckBox(NULL)
,m_pDelegate(NULL)
,m_pFilterGrade(warrior::kWarFilterNone)
{
	D_RESET_C_ARRAY(m_pNodeBox,warrior::kWarFilterCount-1);
}

PictureFilterCell::~PictureFilterCell()
{
	D_SAFE_RELEASE_C_ARRAY(m_pNodeBox,warrior::kWarFilterCount-1);
	CC_SAFE_RELEASE(m_pCheckBox);
}
PictureFilterCell* PictureFilterCell::create(cocos2d::extension::CCData* pData)
{
	PictureFilterCell* pRet = new PictureFilterCell();
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
bool PictureFilterCell::init(cocos2d::extension::CCData* pData)
{
	CCB_READER_DATA_AND_ADDCHILD(pData);
	return true;
}
bool PictureFilterCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCheckBox",CCNode*,m_pCheckBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pGreenButton",CCNode*,m_pNodeBox[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pBlueButton",CCNode*,m_pNodeBox[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pPurpleButton",CCNode*,m_pNodeBox[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pOrangeButton",CCNode*,m_pNodeBox[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pRedButton",CCNode*,m_pNodeBox[4]);
	return false;
}
SEL_MenuHandler PictureFilterCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onClick", PictureFilterCell::onClick);
	return NULL;
}
void PictureFilterCell::setType(warrior::WarriorHandBookFilter pGrade)
{
	m_pFilterGrade = pGrade;
	CCNode* pNode = NULL;
	for(int i=0;i<warrior::kWarFilterCount-1;i++)
	{
		pNode = m_pNodeBox[i];
		pNode->setVisible(false);
	}
	int index = static_cast<int>(pGrade);
	m_pNodeBox[index-1]->setVisible(true);
}
void PictureFilterCell::setSelect(bool boo)
{
	m_pCheckBox->setVisible(boo);
}
void PictureFilterCell::onClick(cocos2d::CCObject *sender)
{
	bool boo = !(m_pCheckBox->isVisible());
	if(m_pDelegate)
	{
		m_pDelegate->setSelectCell(boo?this:NULL);
	}
}