#include "Rank.h"
#include "managers/ResourceManager.h"
#include "managers/DataManager.h"
#include "../friend/FriendFormation.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace rank;

static Rank* pRank = NULL;


Rank* Rank::shareRank()
{
	if(!pRank)
	{
		pRank = new Rank();
		pRank->init();
	}
	return pRank;
}

Rank::Rank()
	:m_pMyLevel(NULL)
	,m_pMyRankVal(NULL)
	,m_pCloseButton(NULL)
	,m_pTableNode(NULL)
	,m_pContainer(NULL)
	,m_uCurrIdx(0)
	,m_pDTabView(NULL)
{
	D_RESET_C_ARRAY(m_pLabel, RANK_TYPE_COUNT);
}

Rank::~Rank()
{
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pMyRankVal);
	CC_SAFE_RELEASE(m_pMyLevel);
	D_SAFE_RELEASE_C_ARRAY(m_pLabel, RANK_TYPE_COUNT);
	CC_SAFE_RELEASE(m_pTableNode);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pDTabView);
}


bool Rank::init()
{
	if(!DLayer::init())
	{
		return false;
	}
	
	DM_GET_RANK_MODEL->getRankInfo();

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_paihang/PL_paihang");

	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height+5, -PL_GET_HEIGHT_DIFF);
	m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, posY, m_pContainer->getContentSize().width, height),5);
	m_pDTabView->retain();


	std::string tabName[RANK_TYPE_COUNT] = {"PL_phb_btn_wenzi_zdl","PL_phb_btn_wenzi_dj"};
	RankType tabType[] = {kForceRank, kLevelRank};

	for(uint32_t i = 0; i < RANK_TYPE_COUNT; i++)
	{
		RankView * pItemList = RankView::create(tabType[i], CCSizeMake(m_pContainer->getContentSize().width, height-50));
		if(pItemList)
		{
			m_pDTabView->addTab(pItemList,"","PL_ui/all/all_btn_fenye_lan.png","PL_ui/all/all_btn_fenye_lv.png",NULL,CCString::createWithFormat("PL_ui/paihangbang/%s.png",tabName[i].c_str())->getCString());
		}
	}
	m_pDTabView->setDelegate(this);
	addChild(m_pDTabView);

	m_uCurrIdx = 0;
	m_pRankModel = DM_GET_RANK_MODEL;
	return true;
}


bool Rank::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton*, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableNode", CCNode *, this->m_pTableNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMyRankVal", CCLabelBMFont *, this->m_pMyRankVal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMyLevel", CCLabelBMFont *, this->m_pMyLevel);
	D_CCB_ASSIGN_ARRAY("m_pLabel", CCNode*, m_pLabel, RANK_TYPE_COUNT);
	
	return true;
}

SEL_MenuHandler Rank::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallback", Rank::menuCallback);
	return NULL;
}

void Rank::menuCallback(cocos2d::CCObject *obj)
{

}

void Rank::SelectTabAtIndex(DTabView *tabView, uint32_t idx)
{
	m_uCurrIdx = idx;
	updateView();
}

void Rank::updateView()
{
	for(int i(0); i<RANK_TYPE_COUNT; i++)
	{
		m_pLabel[i]->setVisible(false);
	}
	m_pLabel[m_uCurrIdx]->setVisible(true);
	UserRankInfo* pRankInfo = NULL;
	switch(m_uCurrIdx + 1)
	{
	case kForceRank:
		{ 
			pRankInfo = DM_GET_RANK_MODEL->getMyFightInfo();
			break;
		}
	case kLevelRank:
		{ 
			pRankInfo = DM_GET_RANK_MODEL->getMyLevelInfo();
			break;
		}
	default: break;
	}
	if(!pRankInfo) return ;
	m_pMyRankVal->setString(D_CSTRING_FROM_UINT(pRankInfo->getNum()));
	m_pMyLevel->setString(D_CSTRING_FROM_UINT(pRankInfo->getRank()));
}

void Rank::onEnter()
{
	BINDING_ONLY(m_pRankModel, this, Rank::updateView, RANK_LIST_UPDATE);
	m_pCloseButton->getButton()->setTarget(this, menu_selector(Rank::closeView));
	DLayer::onEnter();
}

void Rank::onExit()
{
	UNBINDING_ALL(m_pRankModel, this);
	DLayer::onExit();
}


void Rank::closeView(cocos2d::CCObject*)
{
	removeFromParent();
	Game::sharedGame()->NavigateTo(navigation::kNavHome);
}


void Rank::showOtherFormation(uint32_t uUserId)
{
	FriendFormation* pFormation = FriendFormation::create();
	pFormation->setInfo(uUserId);
	addChild(pFormation);
}