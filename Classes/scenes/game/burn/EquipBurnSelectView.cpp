//
//  EquipBurnSelectView.h
//  宝物选择
//
//
#include "EquipBurnSelectView.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "../Game.h"
#include "../equip/EquipMultiSelectView.h"

#define  TREASURE_CELL_SIZE	CCSizeMake(640, 130)    //单元大小

EquipBurnSelectView::EquipBurnSelectView()
	: DLayer()
	//, m_pSelectTable(NULL)
	, m_pContainer(NULL)
	, m_pTabContainer(NULL)
	, m_pConfirmButton(NULL)
	, m_pTitleTreasure(NULL)
	, m_pTitleEquip(NULL)
	, m_pTitleHero(NULL)
	, m_pBackButton(NULL)
	, m_pData(NULL)
	, m_pDelegate(NULL)
	, m_uCurrentID(0)
	, m_pDTabView(NULL)
{

}

EquipBurnSelectView::~EquipBurnSelectView()
{
	CC_SAFE_RELEASE_NULL(m_pContainer);
	CC_SAFE_RELEASE_NULL(m_pTabContainer);
	CC_SAFE_RELEASE_NULL(m_pConfirmButton);
	CC_SAFE_RELEASE_NULL(m_pBackButton);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pTitleTreasure);
	CC_SAFE_RELEASE(m_pTitleEquip);
	CC_SAFE_RELEASE(m_pTitleHero);
	CC_SAFE_RELEASE_NULL(m_pDTabView);
}
EquipBurnSelectView * EquipBurnSelectView::create()
{
	EquipBurnSelectView * pRet = new EquipBurnSelectView();
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

bool EquipBurnSelectView::init()
{
	if(!DLayer::init())
	{
		return false;
	}
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhuanhun/PL_zhuanhun_liebiao");
	m_pData = readCCData("PL_ui/ccb/pl_beibao/PL_beibao_wupin_cell");
	CC_SAFE_RETAIN(m_pData);

	m_pConfirmButton->getButton()->setTarget(this, menu_selector(EquipBurnSelectView::onConfirmClick));
	m_pBackButton->getButton()->setTarget(this, menu_selector(EquipBurnSelectView::onBackClick));

	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height + 18, -PL_MAX_HEIGHT_DIFF);
	m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, posY, m_pContainer->getContentSize().width, height),30);
	m_pDTabView->retain();
	m_pDTabView->setPadding(12.0f);
	//m_pDTabView->addTab(m_pSelectTable, "", "PL_ui/all/all_btn_fenye_lan_up.png", "PL_ui/all/all_btn_fenye_lan.png", NULL, "PL_ui/liebiao/hero_lb_wenzi_yingxiong.png");
	
	std::string tabName[3] = {"pl_bb_btn_wenzi_wuqi","pl_bb_btn_wenzi_kuijia","pl_bb_btn_wenzi_shoushi"};
	for (uint32_t i = 0; i < 3; i++)
	{
		EquipMultiSelectView* equipView = EquipMultiSelectView::create(equip::EquipType(i + 1), PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));//TODO
		m_pDTabView->addTab(equipView,"","PL_ui/beibao/pl_bb_btn_xiaofenye.png","PL_ui/beibao/pl_bb_btn_xiaofenye_up.png",NULL,CCString::createWithFormat("PL_ui/beibao/%s.png",tabName[i].c_str())->getCString());
	}


	m_pTabContainer->addChild(m_pDTabView);

	m_pTitleEquip->setVisible(true);
	m_pTitleTreasure->setVisible(false);
	m_pTitleHero->setVisible(false);
	return true;
}

void EquipBurnSelectView::onEnter()
{	
	//DM_GET_BURN_MODEL->reset();
	DLayer::onEnter();
	Game::sharedGame()->setNavigatorVisable(false);
}

void EquipBurnSelectView::onExit()
{
	Game::sharedGame()->setNavigatorVisable(true);
	DLayer::onExit();
}

bool EquipBurnSelectView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTabContainer", CCNode *, m_pTabContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackButton", DButton *, m_pBackButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleTreasure", CCNode *, m_pTitleTreasure);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleEquip", CCNode *, m_pTitleEquip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleHero", CCNode *, m_pTitleHero);
	return false;
}

void EquipBurnSelectView::onConfirmClick(cocos2d::CCObject *sender)
{
	m_pDelegate->onSelectConfirm();
	removeFromParent();
}

void EquipBurnSelectView::onBackClick(cocos2d::CCObject *sender)
{
	m_pDelegate->onSelectConfirm();
	removeFromParent();
}