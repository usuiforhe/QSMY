#include "BurnSelectView.h"
#include "../warrior/WarriorListView.h"
#include "managers/GameManager.h"
USING_NS_CC;
USING_NS_CC_EXT;

BurnSelectView::BurnSelectView()
	: DLayer()
	, m_pSelectTable(NULL)
	, m_pContainer(NULL)
	, m_pConfirmButton(NULL)
	, m_pBackButton(NULL)
	,isMoreSelet(false)
	,m_pSoulTable(NULL)
	,m_pDTabView(NULL)
	,m_pTabContainer(NULL)
	,m_pBurnTips(NULL)
{

}

BurnSelectView::~BurnSelectView()
{
	CC_SAFE_RELEASE_NULL(m_pContainer);
	CC_SAFE_RELEASE_NULL(m_pConfirmButton);
	CC_SAFE_RELEASE_NULL(m_pBackButton);
	CC_SAFE_RELEASE_NULL(m_pTabContainer);
	CC_SAFE_RELEASE_NULL(m_pSelectTable);
	CC_SAFE_RELEASE_NULL(m_pSoulTable);
	CC_SAFE_RELEASE_NULL(m_pDTabView);
	CC_SAFE_RELEASE_NULL(m_pBurnTips);
}
BurnSelectView * BurnSelectView::create(WarriorFilterDelegate * pFilter)
{
	BurnSelectView * pRet = new BurnSelectView;
	if(pRet && pRet->init(pFilter))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
	}

	return pRet;
}

bool BurnSelectView::init(WarriorFilterDelegate *pFilter)
{
	if(!DLayer::init())
	{
		return false;
	}

	//CCB_READER_AND_ADDCHILD("ui/ccb/warrior/warriorListSelect");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhuanhun/PL_zhuanhun_liebiao");

	m_pBurnTips->setVisible(true);
	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height + 21, -PL_MAX_HEIGHT_DIFF);
	m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, posY, m_pContainer->getContentSize().width, height),12);
	m_pDTabView->retain();

	m_pSelectTable = BurnListSelectTable::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pSelectTable->setFilter(pFilter);
	CC_SAFE_RETAIN(m_pSelectTable);
	m_pDTabView->addTab(m_pSelectTable, "", "PL_ui/all/all_btn_fenye_lan_up.png", "PL_ui/all/all_btn_fenye_lan.png", NULL, "PL_ui/liebiao/hero_lb_wenzi_yingxiong.png");
	m_pSoulTable = SoulSelectTable::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	CC_SAFE_RETAIN(m_pSoulTable);
	m_pDTabView->addTab(m_pSoulTable, "", "PL_ui/all/all_btn_fenye_lv_up.png", "PL_ui/all/all_btn_fenye_lv.png", NULL, "PL_ui/liebiao/hero_lb_wenzi_hunpo.png");
	//m_pContainer->addChild(m_pSoulTable);
	m_pTabContainer->addChild(m_pDTabView);

	return true;
}

void BurnSelectView::onEnter()
{
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(BurnSelectView::onConfirmClick));
	m_pBackButton->getButton()->setTarget(this, menu_selector(BurnSelectView::onBackClick));
	DLayer::onEnter();
	Game::sharedGame()->setNavigatorVisable(false);
}

void BurnSelectView::onExit()
{
	Game::sharedGame()->setNavigatorVisable(true);
	DLayer::onExit();
}

bool BurnSelectView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackButton", DButton *, m_pBackButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTabContainer",CCNode* , m_pTabContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBurnTips",CCNode* , m_pBurnTips);
	return false;
}

void BurnSelectView::setFilter(WarriorFilterDelegate *pFilter)
{
	m_pSelectTable->setFilter(pFilter);
}

void BurnSelectView::setSelectedWarrior(warrior::WarriorCurInfo *pSelectedWarrior)
{
	m_pSelectTable->setSelectedWarrior(pSelectedWarrior);
}

void BurnSelectView::onConfirmClick(cocos2d::CCObject *sender)
{
	if(!isMoreSelet)
	{
		m_pDelegate->setSelectedWarrior(m_pSelectTable->getSelectedWarrior());
	}
	else
	{
		//多选执行
		if(m_pSelectTable->getSelecteMore()->count()>0)
		{
			m_pDelegate->setSelectedWarriorMore(m_pSelectTable->getSelecteMore(), 1);
		}
		else if(m_pSoulTable->getSelecteMore()->count()>0)
		{
			m_pDelegate->setSelectedWarriorMore(m_pSoulTable->getSelecteMore(),2);
		}
	}
	removeFromParent();
}

void BurnSelectView::onBackClick(cocos2d::CCObject *sender)
{
	m_pDelegate->setSelectedWarrior(NULL);
	removeFromParent();
}

#pragma mark ----------------BurnListSelectTable 弟子选择列表------------------

BurnListSelectTable * BurnListSelectTable::create(cocos2d::CCSize size)
{
	BurnListSelectTable * pRet = new BurnListSelectTable;
	if(pRet && pRet->init(size))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
	}
	return pRet;
}

BurnListSelectTable::BurnListSelectTable():
FiltableWarriorList(),
	m_pSelectedWarrior(NULL)
	,m_cSelecteMore(NULL)
	,m_bMoreSelete(false)
{

}

BurnListSelectTable::~BurnListSelectTable()
{
	CC_SAFE_RELEASE_NULL(m_pSelectedWarrior);
	CC_SAFE_RELEASE_NULL(m_pData);
	if(m_cSelecteMore)
	{
		m_cSelecteMore->removeAllObjects();
	}
	CC_SAFE_RELEASE(m_cSelecteMore);
}

bool BurnListSelectTable::init(cocos2d::CCSize size)
{
	if(!FiltableWarriorList::init(size))
	{
		return false;
	}

	//m_pData = readCCData("ui/ccb/warrior/warriorListSelectCell");
	//m_pData = readCCData("PL_ui/ccb/pl_hero/PL_hero_xy_cell");
	m_pData = readCCData("PL_ui/ccb/pl_zhuanhun/PL_zhuanhun_liebiao_cell");
	m_cSelecteMore = CCArray::create();
	CC_SAFE_RETAIN(m_cSelecteMore);
	CC_SAFE_RETAIN(m_pData);
	return true;
}

void BurnListSelectTable::setSelectedCell(WarriorCell * pCell,WarriorCell * curCell,bool boo)
{
	if(m_bMoreSelete && m_cSelecteMore!=NULL && boo)
	{
		//多选执行
		if(DM_GET_BURN_MODEL->getBurnNum() +m_cSelecteMore->count()>=TOTAL_BURN_NUM)
		{
			//已达到最大选择数
			Tips_Alert(D_LOCAL_STRING("$LianhuaHeroEnough").c_str());
			return;
		}
	}
	FiltableWarriorList::setSelectedCell(pCell,curCell,m_bMoreSelete);//是否可以多选
	CC_SAFE_RELEASE_NULL(m_pSelectedWarrior);
	if(pCell != NULL)
	{
		m_pSelectedWarrior = pCell->getWarrior();
		CC_SAFE_RETAIN(m_pSelectedWarrior);
	}
	if(NULL != curCell && m_bMoreSelete) 
	{
		changeSelectList(curCell,boo);
	}
}

//多选执行
void BurnListSelectTable::changeSelectList(WarriorCell * pCell,bool boo)
{
	if(pCell)
	{
		if(!boo)
		{
			if(m_cSelecteMore->containsObject(pCell->getWarrior()))
			{
				m_cSelecteMore->removeObject(pCell->getWarrior());
			}
		}
		else
		{
			m_cSelecteMore->addObject(pCell->getWarrior());
		}

	}

	CCLOG(CCString::createWithFormat("m_cSelecteMore:%d",m_cSelecteMore->count())->getCString());
}

cocos2d::extension::CCTableViewCell* BurnListSelectTable::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	warrior::WarriorCurInfo * pWarrior = (warrior::WarriorCurInfo *) m_pWarriorList->objectAtIndex(idx);

	WarriorListCellSelect * pCell = (WarriorListCellSelect *)table->dequeueCell();
	if(pCell == NULL)
	{
		pCell = WarriorListCellSelect::create(m_pData);
	}
	pCell->setDelegate(this);
	pCell->setWarrior(pWarrior);

	if(m_bMoreSelete)
	{
		if(m_cSelecteMore)
		{
			if(m_cSelecteMore->containsObject(pCell->getWarrior()))
			{
				pCell->setSelected(true);
			}
			else
			{
				pCell->setSelected(false);
			}
		}
	}
	else
	{
		if(pWarrior == m_pSelectedWarrior)
		{
			setSelectedCell(pCell);
			pCell->setSelected(true);
		}
		else
		{
			pCell->setSelected(false);
		}
	}
	return pCell;
}

void BurnListSelectTable::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{

}