//
//  TreasureSelectView.h
//  宝物选择
//
//
#include "TreasureSelectView.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "../Game.h"

#define  TREASURE_CELL_SIZE	CCSizeMake(640, 130)    //单元大小

TreasureSelectView::TreasureSelectView()
	: DLayer()
	, m_pSelectTable(NULL)
	, m_pContainer(NULL)
	, m_pConfirmButton(NULL)
	, m_pTitleTreasure(NULL)
	, m_pTitleHero(NULL)
	, m_pBackButton(NULL)
	, m_pData(NULL)
	, m_pDelegate(NULL)
	, m_uCurrentID(0)
{

}

TreasureSelectView::~TreasureSelectView()
{
	CC_SAFE_RELEASE_NULL(m_pContainer);
	CC_SAFE_RELEASE_NULL(m_pConfirmButton);
	CC_SAFE_RELEASE_NULL(m_pBackButton);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pTitleTreasure);
	CC_SAFE_RELEASE(m_pTitleHero);
}
TreasureSelectView * TreasureSelectView::create()
{
	TreasureSelectView * pRet = new TreasureSelectView();
	if(pRet && pRet->init())
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

bool TreasureSelectView::init()
{
	if(!DLayer::init())
	{
		return false;
	}
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhuanhun/PL_zhuanhun_liebiao");
	m_pData = readCCData("PL_ui/ccb/pl_beibao/PL_beibao_wupin_cell");
	CC_SAFE_RETAIN(m_pData);

	m_pConfirmButton->getButton()->setTarget(this, menu_selector(TreasureSelectView::onConfirmClick));
	m_pBackButton->getButton()->setTarget(this, menu_selector(TreasureSelectView::onBackClick));

	m_pSelectTable = DTableView::create(this , PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pContainer->addChild(m_pSelectTable);

	m_pTitleTreasure->setVisible(true);
	m_pTitleHero->setVisible(false);
	return true;
}

void TreasureSelectView::onEnter()
{	
	DLayer::onEnter();
	Game::sharedGame()->setNavigatorVisable(false);
	//获取宝物列表
	DM_GET_EQUIP_MODEL->getEquipListByTypeIgnoreID(equip::kEquipTypeTreasure, 0, m_EquipInfoVec);
	//去除已经选中的
	makeList();
	//进来先把原先的清空
	m_EquipInfoVecSelected.removeAllObjects();
	m_pSelectTable->reloadData();
}

void TreasureSelectView::onExit()
{
	Game::sharedGame()->setNavigatorVisable(true);
	DLayer::onExit();
	m_EquipInfoVec.removeAllObjects();
	m_EquipInfoVecSelected.removeAllObjects();
}

void TreasureSelectView::makeList()
{
	CCArray*	pDelete = CCArray::create();
	CCDictionary*	pSelectDict = DM_GET_BURN_MODEL->getTreasureDict();
	CCObject* pObj = NULL;
	CCObject* pObjAll = NULL;

	CCARRAY_FOREACH(&m_EquipInfoVec, pObjAll)
	{		
		equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pObjAll);
		//CCLOG("%d %d" , pInfo->getId() , pInfo->getEquipId());

		//0.不能自己吞噬自己
		if(pInfo->getId() == m_uCurrentID)
		{
			pDelete->addObject(pObjAll);
			continue;
		}

		//1.判断该装备是否穿戴
		if(pInfo->isEquiped())
		{
			pDelete->addObject(pObjAll);
			continue;
		}
		//2.判断是否重了
		CCDictElement *pEle = NULL;
		CCDICT_FOREACH(pSelectDict, pEle)
		{
			equip::EquipCurInfo* pInfoSelected = dynamic_cast<equip::EquipCurInfo*>(pEle->getObject());
			if(pInfo->getId() == pInfoSelected->getId())
			{
				//去掉已经加的
				pDelete->addObject(pObjAll);
				break;
			}			
		}
	}

	//删除
	m_EquipInfoVec.removeObjectsInArray(pDelete);
}

bool TreasureSelectView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackButton", DButton *, m_pBackButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleTreasure", CCNode *, m_pTitleTreasure);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleHero", CCNode *, m_pTitleHero);
	return false;
}

void TreasureSelectView::onConfirmClick(cocos2d::CCObject *sender)
{
	//equip::EquipCurInfo * pInfo = (m_EquipInfoVecSelected.count() > 0 ? dynamic_cast<equip::EquipCurInfo *>(m_EquipInfoVecSelected.objectAtIndex(0)) : NULL);
	//m_pDelegate->setSelectedTreasure(pInfo);
	//removeFromParent();

	CCArray* pArray = NULL;
	if(m_EquipInfoVecSelected.count() > 0)
	{
		pArray = CCArray::create();
		pArray->addObjectsFromArray(&m_EquipInfoVecSelected);
	}
	//pArray = CCArray::createWithArray();
	m_pDelegate->setSelectedTreasureList(pArray);
	removeFromParent();
}

void TreasureSelectView::onBackClick(cocos2d::CCObject *sender)
{
	//m_pDelegate->setSelectedTreasure(NULL);
	//removeFromParent();

	m_pDelegate->setSelectedTreasureList(NULL);
	removeFromParent();
}


cocos2d::CCSize TreasureSelectView::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	return TREASURE_CELL_SIZE;
}

cocos2d::CCSize TreasureSelectView::cellSizeForTable(cocos2d::extension::CCTableView *table)
{
	return TREASURE_CELL_SIZE;
}

unsigned int TreasureSelectView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return m_EquipInfoVec.count();
}

cocos2d::extension::CCTableViewCell* TreasureSelectView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{

	EquipSelectViewCell * pCell = (EquipSelectViewCell *)table->dequeueCell();
	if(pCell == NULL)
	{
		pCell = EquipSelectViewCell::create(m_pData);
		pCell->setDelegate(this);
	}
	pCell->setTreasureInfo(dynamic_cast<equip::EquipCurInfo *>(m_EquipInfoVec.objectAtIndex(idx)));
	pCell->setSelected(m_EquipInfoVecSelected.containsObject(m_EquipInfoVec.objectAtIndex(idx)));
	return pCell;
}

void TreasureSelectView::setSelectedCell(EquipSelectViewCell * pCell)
{
	EquipSelectViewCell* equipCell = dynamic_cast<EquipSelectViewCell*>(pCell);
	if (!equipCell)
	{
		return;
	}

	if(m_EquipInfoVecSelected.containsObject(equipCell->getEquipInfo()))
	{
		//取消
		m_EquipInfoVecSelected.removeObject(equipCell->getEquipInfo());
	}
	else{
		//多选执行
		if(DM_GET_BURN_MODEL->getTreasureDict()->count() +m_EquipInfoVecSelected.count()>=TREASURE_BURN_MAX)
		{
			//已达到最大选择数
			Tips_Alert(D_LOCAL_STRING("TreasureTips2").c_str());
			return;
		}

		//单选
		//m_EquipInfoVecSelected.removeAllObjects();
		m_EquipInfoVecSelected.addObject(equipCell->getEquipInfo());
	}
	m_pSelectTable->reloadData();
}
