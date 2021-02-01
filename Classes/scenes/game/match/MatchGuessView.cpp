//
//  MatchGuessView.cpp
//  淘汰赛比赛对阵-竞猜列表
//
//  Created by cll on 13-4-16
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MatchGuessView.h"
#include "Match.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace match;

#define  MATCH_TAB_RECT     CCRectMake(90, 136, 155, 160)
#define  MATCH_LIST_RECT	CCRectMake(0, 270, 640, 560)

/************************ match guess class begin*******************/

MatchGuessListView* MatchGuessListView::create(MatchDelegate* pDelegate)
{
	MatchGuessListView *pRet = new MatchGuessListView();
	if (pRet && pRet->init(pDelegate))
	{ 
		pRet->autorelease(); 
	} 
	else 
	{ 
		CC_SAFE_DELETE(pRet); 
	} 
	
	return pRet; 
}

MatchGuessListView::MatchGuessListView()
:m_pSlideView(NULL)
,m_pDelegate(NULL)
,m_pMatchModel(NULL)
,m_pTfTimes(NULL)
,m_pTfNum(NULL)
,m_nCurrentPage(-1)
,m_pTfTime8(NULL)
,m_pTfTime4(NULL)
,m_pTfTime2(NULL)
,isFirstEnter(true)
,m_pContainer(NULL)
,m_pNode8_4(NULL)
,m_pNode4_2(NULL)
,m_pNode2_1(NULL)
,m_pTfItemHas(NULL)
{

}

MatchGuessListView::~MatchGuessListView()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pTfTimes);
    CC_SAFE_RELEASE(m_pTfNum);
    CC_SAFE_RELEASE(m_pTfTime8);
    CC_SAFE_RELEASE(m_pTfTime4);
    CC_SAFE_RELEASE(m_pTfTime2);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pNode8_4);
	CC_SAFE_RELEASE(m_pNode4_2);
	CC_SAFE_RELEASE(m_pNode2_1);
	CC_SAFE_RELEASE(m_pTfItemHas);
}

bool MatchGuessListView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfTimes", CCLabelBMFont *, this->m_pTfTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfNum", CCLabelBMFont *, this->m_pTfNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfTime8", CCLabelBMFont *, this->m_pTfTime8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfTime4", CCLabelBMFont *, this->m_pTfTime4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfTime2", CCLabelBMFont *, this->m_pTfTime2);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode8-4", CCNode *, this->m_pNode8_4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode4-2", CCNode *, this->m_pNode4_2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode2-1", CCNode *, this->m_pNode2_1);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfItemHas",cocos2d::CCLabelBMFont*, this->m_pTfItemHas);
    return false;
}

SEL_MenuHandler MatchGuessListView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallBack", MatchGuessListView::menuCallBack);
	return NULL;
}

void MatchGuessListView::menuCallBack(CCObject * obj)
{
	m_pContainer->removeAllChildren();
	int tag = (dynamic_cast<CCNode*>(obj))->getTag();
	CCString* pString = CCString::create("");

	//处理时间高亮
	ccColor3B gray = ccc3(145,145,145);
	m_pTfTime8->setColor(tag == 1 ? ccWHITE : gray);m_pTfTime8->setScale(tag == 1 ? 1.0f : 0.8f);
	m_pTfTime4->setColor(tag == 2 ? ccWHITE : gray);m_pTfTime4->setScale(tag == 2 ? 1.0f : 0.8f);
	m_pTfTime2->setColor(tag == 3 ? ccWHITE : gray);m_pTfTime2->setScale(tag == 3 ? 1.0f : 0.8f);

	switch(tag){
	case 1:
		{
			MatchGuessView * quatFinalView = MatchGuessView::create(match::k8);
			quatFinalView->setDelegate(m_pDelegate);
			m_pContainer->addChild(quatFinalView);

			m_pNode8_4->setVisible(true);
			m_pNode4_2->setVisible(false);
			m_pNode2_1->setVisible(false);

			pString = CCString::createWithFormat("%d",m_pMatchModel->getMatchConfig()->m_nBetCost8);
			break;
		}
	case 2:
		{
			MatchGuessView * semiFinalView = MatchGuessView::create(match::k4);
			semiFinalView->setDelegate(m_pDelegate);
			m_pContainer->addChild(semiFinalView);

			m_pNode8_4->setVisible(false);
			m_pNode4_2->setVisible(true);
			m_pNode2_1->setVisible(false);

			pString = CCString::createWithFormat("%d",m_pMatchModel->getMatchConfig()->m_nBetCost4);
			break;
		}
	case 3:
		{
			MatchGuessView * finalView = MatchGuessView::create(match::k2);
			finalView->setDelegate(m_pDelegate);
			m_pContainer->addChild(finalView);

			m_pNode8_4->setVisible(false);
			m_pNode4_2->setVisible(false);
			m_pNode2_1->setVisible(true);

			pString = CCString::createWithFormat("%d",m_pMatchModel->getMatchConfig()->m_nBetCost2);
			break;
		}
	}
	m_pTfNum->setString(pString->getCString());
}

bool MatchGuessListView::init(MatchDelegate* pDelegate)
{
	if(!DLayer::init())
	{
		return false;
	}
	
	m_pDelegate = pDelegate;

	m_pMatchModel = DM_GET_MATCH_MODEL;
    
    
	/*m_pSlideView = DSlideView::create(MATCH_TAB_RECT,MATCH_LIST_RECT);
	addChild(m_pSlideView);
	m_pSlideView->getTabView()->setBounceable(false);
	m_pSlideView->getContentView()->setDelegate(this);


	MatchGuessView * quatFinalView = MatchGuessView::create(match::k8);
	quatFinalView->setDelegate(m_pDelegate);
	m_pSlideView->addSlideWithTab(quatFinalView,"ui/match/bg_nagtive_n.png","ui/match/bg_nagtive_h.png","ui/match/bg_nagtive_n.png");
	m_pSlideView->addSlideWithTab(quatFinalView,"ui/match/bg_nagtive_n.png","ui/match/bg_nagtive_h.png","ui/match/bg_nagtive_n.png");

	MatchGuessView * semiFinalView = MatchGuessView::create(match::k4);
	semiFinalView->setDelegate(m_pDelegate);
	m_pSlideView->addSlideWithTab(semiFinalView,"ui/match/bg_nagtive_n.png","ui/match/bg_nagtive_h.png","ui/match/bg_nagtive_n.png");
	m_pSlideView->addSlideWithTab(quatFinalView,"ui/match/bg_nagtive_n.png","ui/match/bg_nagtive_h.png","ui/match/bg_nagtive_n.png");

	MatchGuessView * finalView = MatchGuessView::create(match::k2);
	finalView->setDelegate(m_pDelegate);

	m_pSlideView->addSlideWithTab(finalView,"ui/match/bg_nagtive_n.png","ui/match/bg_nagtive_h.png","ui/match/bg_nagtive_n.png");
	m_pSlideView->addSlideWithTab(quatFinalView,"ui/match/bg_nagtive_n.png","ui/match/bg_nagtive_h.png","ui/match/bg_nagtive_n.png");*/
    
    //CCB_READER_AND_ADDCHILD("ui/ccb/match/matchGuessView");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beisai/PL_beisai_jingcai");    
    
    m_pTfTime8->setString(m_pMatchModel->getMatchConfig()->getMatchEndTime(match::k8).c_str());
    m_pTfTime4->setString(m_pMatchModel->getMatchConfig()->getMatchEndTime(match::k4).c_str());
    m_pTfTime2->setString(m_pMatchModel->getMatchConfig()->getMatchEndTime(match::k2).c_str());
    
    return true;
}


void MatchGuessListView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
    /*if(m_nCurrentPage==m_pSlideView->getSelectedSlideIdx()) return;
    m_nCurrentPage = m_pSlideView->getSelectedSlideIdx();
    CCString* pString = NULL;
    switch (m_nCurrentPage) {
        case 0:
            pString = CCString::createWithFormat("%d",m_pMatchModel->getMatchConfig()->m_nBetCost8);
            break;
        case 1:
            pString = CCString::createWithFormat("%d",m_pMatchModel->getMatchConfig()->m_nBetCost4);
            break;
        case 2:
            pString = CCString::createWithFormat("%d",m_pMatchModel->getMatchConfig()->m_nBetCost2);
            break;
        default:
            pString = CCString::create("");
            break;
    }
    m_pTfNum->setString(pString->getCString());*/
}

void MatchGuessListView::onEnter()
{
    DLayer::onEnter();
    BINDING_ONLY(m_pMatchModel, this, MatchGuessListView::updateView, D_EVENT_MATCH_UPDATE_BETLIST);
    updateView();
    if(isFirstEnter)
    {
        uint16_t index = 0;
		uint16_t tag = 1;
        switch (m_pMatchModel/*->getMatchConfig()*/->getMatching()) {
            case k16:
            case k8:
                index = 0;
				tag = 1;
                break;
            case k4:
                index = 1;
				tag = 2;
                break;
            case k2:
            case k1:
                index = 2;
				tag = 3;
                break;
            default:
                index = 0;
                break;
        }
        //m_pSlideView->selectedTab(index,false);
		CCNode* tagNode = CCNode::create();
		tagNode->setTag(tag);
		menuCallBack(tagNode);
        isFirstEnter = false;
    }
}

void MatchGuessListView::onExit()
{
    DLayer::onExit();
    UNBINDING_ALL(m_pMatchModel, this);
}


void MatchGuessListView::updateView()
{
    m_pTfTimes->setString(CCString::createWithFormat("%d",m_pMatchModel->getRemainBetTimes())->getCString());

	item::ItemInfo *m_pItem = DM_GET_ITEM_MODEL->getItemInfoByID(SKY_LIGHT_ID);
	uint32_t count = m_pItem?m_pItem->getNum():0;
	m_pTfItemHas->setString(CCString::createWithFormat("%d",count)->getCString());
}


////////////////////////////////////////////////////////////////////


#pragma mark MatchGuessView

MatchGuessView* MatchGuessView::create(MatchFinalCat finalNum)
{
	MatchGuessView *pRet = NULL;
    switch (finalNum) {
        case k8:
            pRet = new MatchGuessView8();
            break;
        case k4:
            pRet = new MatchGuessView4();
            break;
        case k2:
            pRet = new MatchGuessView2();
            break;
        default:
            break;
    }
    
	if (pRet && pRet->init(finalNum))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

MatchGuessView::MatchGuessView()
:m_finalNum(k8)
,m_pMatchModel(NULL)
,m_pDelegate(NULL)
,m_pVsList(NULL)
,m_pBetList(NULL)
,m_VSInfoVec(NULL)
{
    
}


MatchGuessView::~MatchGuessView()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pVsList);
    CC_SAFE_RELEASE(m_pBetList);
    CC_SAFE_RELEASE(m_VSInfoVec);
}

bool MatchGuessView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    return false;
}

bool MatchGuessView::init(MatchFinalCat finalNum)
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_VSInfoVec = MatchModel::VSInfoVec::create();
    CC_SAFE_RETAIN(m_VSInfoVec);
    
    m_pVsList = CCArray::create();
    CC_SAFE_RETAIN(m_pVsList);
    
    m_pBetList = CCArray::create();
    CC_SAFE_RETAIN(m_pBetList);
	
	m_finalNum = finalNum;

	m_pMatchModel = DM_GET_MATCH_MODEL;
    

	return true;

}

void MatchGuessView::onEnter()
{

	BINDING_EXEC(m_pMatchModel,this,MatchGuessView::updateVS,D_EVENT_MATCH_UPDATE_VSLIST);
	BINDING_EXEC(m_pMatchModel,this,MatchGuessView::updateBet,D_EVENT_MATCH_UPDATE_BETLIST);
    DLayer::onEnter();
}

void MatchGuessView::onExit()
{
	UNBINDING_ALL(m_pMatchModel,this);
	DLayer::onExit();
    reset();
}

void MatchGuessView::reset()
{
    MatchUserHead* pVs = NULL;
    uint8_t index = 0;
    uint8_t vs_count = (uint8_t)m_finalNum>>1;
    for (uint8_t i =0; i<vs_count; ++i)
    {
        if(index>=m_pVsList->count()) break;
        pVs = (MatchUserHead*)m_pVsList->objectAtIndex(index++);
        pVs->setVSInfo(NULL, match::kVSLeft);
        if(index>=m_pVsList->count()) break;
        pVs = (MatchUserHead*)m_pVsList->objectAtIndex(index++);
        pVs->setVSInfo(NULL, match::kVSRight);
        
    }
}

void MatchGuessView::updateBet()
{
    if(!m_pMatchModel->isSynced) return;
    updateBetInfo();
}


void MatchGuessView::betCallBack(cocos2d::CCObject* obj)
{
    CCMenuItem* pItem = (CCMenuItem*)obj;
    MatchBetMenu* pBet = (MatchBetMenu*)m_pBetList->objectAtIndex(pItem->getTag());
    if(m_pDelegate) m_pDelegate->showMatchBetView(pBet->getVSInfo()->finalNum,pBet->getVSInfo()->mid);
}

//void MatchGuessView::rewardCallBack(cocos2d::CCObject* obj)
//{
//    CCMenuItem* pItem = (CCMenuItem*)obj;
//    MatchBetMenu* pBet = (MatchBetMenu*)m_pBetList->objectAtIndex(pItem->getTag());
//    HTTP_CLIENT->getCupBetPrize(pBet->getBetInfo()->id);
//}

void MatchGuessView::updateVS()
{
    if(!m_pMatchModel->isSynced) return;
    updateMatchGuessList();
    updateVSInfo();
}

void MatchGuessView::updateVSInfo()
{
    match::VSInfo* pVSInfo = NULL;
    MatchUserHead* pVs = NULL;
    MatchBetMenu* pBet = NULL;
    
    uint8_t index = 0;
    uint8_t vs_count = (uint8_t)m_finalNum>>1;
    for (uint8_t i =0; i<vs_count; ++i)
    {
        //处理头像显示
        pVSInfo = i<m_VSInfoVec->count()?(match::VSInfo*)m_VSInfoVec->objectAtIndex(i):NULL;
        if(index>=m_pVsList->count()) break;
        pVs = (MatchUserHead*)m_pVsList->objectAtIndex(index++);
        pVs->setVSInfo(pVSInfo, match::kVSLeft);
        if(index>=m_pVsList->count()) break;
        pVs = (MatchUserHead*)m_pVsList->objectAtIndex(index++);
        pVs->setVSInfo(pVSInfo, match::kVSRight);
        
        
        //处理竞猜按钮
        if(i>=m_pBetList->count()) continue;
        pBet = (MatchBetMenu*)m_pBetList->objectAtIndex(i);
        pBet->getBetMenuItem()->setTag(i);
        pBet->getBgMenuItem()->setTag(i);
        pBet->getRewardMenuItem()->setTag(i);
        pBet->getViewMenuItem()->setTag(i);
        pBet->getViewMenuItem()->setTarget(this, menu_selector(MatchGuessView::betCallBack));
        pBet->getBetMenuItem()->setTarget(this, menu_selector(MatchGuessView::betCallBack));
        pBet->getBgMenuItem()->setTarget(this, menu_selector(MatchGuessView::betCallBack));
        pBet->getRewardMenuItem()->setTarget(this, menu_selector(MatchGuessView::betCallBack));
        pBet->setVSInfo(pVSInfo);
    }
}


void MatchGuessView::updateBetInfo()
{
    
    match::VSInfo* pVSInfo = NULL;
    MatchUserHead* pVs = NULL;
    MatchBetMenu* pBet = NULL;
    
    uint8_t index = 0;
    uint8_t vs_count = (uint8_t)m_finalNum>>1;
    for (uint8_t i =0; i<vs_count; ++i)
    {
        pVSInfo = i<m_VSInfoVec->count()?(match::VSInfo*)m_VSInfoVec->objectAtIndex(i):NULL;
        
        //处理头像显示
        if(pVSInfo)
        {
            BetInfo* pBetInfo = m_pMatchModel->getBetInfo(pVSInfo->finalNum, pVSInfo->mid);

			match::VS betUser = match::kVSNone;
			if (pBetInfo)
			{
				betUser = (match::VS)pBetInfo->betUser;
				if (pVSInfo->getIsSwitched())
				{
					if (betUser == match::kVSLeft)
					{
						betUser = match::kVSRight;
					}
					else if (betUser == match::kVSRight)
					{
						betUser = match::kVSLeft;
					}
				}
			}
			
            if(index>=m_pVsList->count()) continue;
            pVs = (MatchUserHead*)m_pVsList->objectAtIndex(index++);
            pVs->getImgBeted()->setVisible(pBetInfo!=NULL && betUser == match::kVSLeft);
            if(index>=m_pVsList->count()) continue;
            pVs = (MatchUserHead*)m_pVsList->objectAtIndex(index++);
            pVs->getImgBeted()->setVisible(pBetInfo!=NULL && betUser == match::kVSRight);
        }else
        {
            if(index>=m_pVsList->count()) continue;
            pVs = (MatchUserHead*)m_pVsList->objectAtIndex(index++);
            pVs->getImgBeted()->setVisible(false);
			//CCSprite* tempsprite = pVs->getImgBeted();
			//tempsprite->setVisible(false);
            if(index>=m_pVsList->count()) continue;
            pVs = (MatchUserHead*)m_pVsList->objectAtIndex(index++);
            pVs->getImgBeted()->setVisible(false);
        }
        
        
        
        //处理竞猜按钮
        if(i>=m_pBetList->count()) continue;
        pBet = (MatchBetMenu*)m_pBetList->objectAtIndex(i);
        pBet->getBetMenuItem()->setTag(i);
        pBet->getBgMenuItem()->setTag(i);
        pBet->getRewardMenuItem()->setTag(i);
        pBet->getViewMenuItem()->setTag(i);
        pBet->getViewMenuItem()->setTarget(this, menu_selector(MatchGuessView::betCallBack));
        pBet->getBetMenuItem()->setTarget(this, menu_selector(MatchGuessView::betCallBack));
        pBet->getBgMenuItem()->setTarget(this, menu_selector(MatchGuessView::betCallBack));
        pBet->getRewardMenuItem()->setTarget(this, menu_selector(MatchGuessView::betCallBack));
        pBet->setVSInfo(pVSInfo);
        
    }
}


void MatchGuessView::updateMatchGuessList()
{
	m_pMatchModel->getVSListByFinalNum(m_finalNum, m_VSInfoVec);
}

#pragma mark MatchGuessView8

MatchGuessView8::MatchGuessView8()
:m_pVs0(NULL)
,m_pVs1(NULL)
,m_pVs2(NULL)
,m_pVs3(NULL)
,m_pVs4(NULL)
,m_pVs5(NULL)
,m_pVs6(NULL)
,m_pVs7(NULL)
,m_pBet0(NULL)
,m_pBet1(NULL)
,m_pBet2(NULL)
,m_pBet3(NULL)
{
    
}

MatchGuessView8::~MatchGuessView8()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pVs0);
    CC_SAFE_RELEASE(m_pVs1);
    CC_SAFE_RELEASE(m_pVs2);
    CC_SAFE_RELEASE(m_pVs3);
    CC_SAFE_RELEASE(m_pVs4);
    CC_SAFE_RELEASE(m_pVs5);
    CC_SAFE_RELEASE(m_pVs6);
    CC_SAFE_RELEASE(m_pVs7);
    CC_SAFE_RELEASE(m_pBet0);
    CC_SAFE_RELEASE(m_pBet1);
    CC_SAFE_RELEASE(m_pBet2);
    CC_SAFE_RELEASE(m_pBet3);
}

bool MatchGuessView8::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs0", MatchUserHead *, this->m_pVs0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs1", MatchUserHead *, this->m_pVs1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs2", MatchUserHead *, this->m_pVs2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs3", MatchUserHead *, this->m_pVs3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs4", MatchUserHead *, this->m_pVs4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs5", MatchUserHead *, this->m_pVs5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs6", MatchUserHead *, this->m_pVs6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs7", MatchUserHead *, this->m_pVs7);
        
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBet0", MatchBetMenu *, this->m_pBet0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBet1", MatchBetMenu *, this->m_pBet1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBet2", MatchBetMenu *, this->m_pBet2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBet3", MatchBetMenu *, this->m_pBet3);
    
    return false;
}


bool MatchGuessView8::init(MatchFinalCat finalNum)
{
	if(!MatchGuessView::init(finalNum))
	{
		return false;
	}
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("MatchUserHead", MatchUserHeadLoader::loader());
    pNodeLoaderLibrary->registerCCNodeLoader("MatchBetMenu", MatchBetMenuLoader::loader());
    //CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/match/matchGuess8", pNodeLoaderLibrary);
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_beisai/PL_match_X8", pNodeLoaderLibrary);

    CC_SAFE_RELEASE(m_pVsList);
    m_pVsList = CCArray::create(m_pVs0,m_pVs1,m_pVs2,m_pVs3,m_pVs4,m_pVs5,m_pVs6,m_pVs7,NULL);
    CC_SAFE_RETAIN(m_pVsList);
    
    CC_SAFE_RELEASE(m_pBetList);
    m_pBetList = CCArray::create(m_pBet0,m_pBet1,m_pBet2,m_pBet3,NULL);
    CC_SAFE_RETAIN(m_pBetList);
    
    
	return true;
    
}


#pragma mark MatchGuessView4

MatchGuessView4::MatchGuessView4()
:m_pVs0(NULL)
,m_pVs1(NULL)
,m_pVs2(NULL)
,m_pVs3(NULL)
,m_pBet0(NULL)
,m_pBet1(NULL)
{
    
}
MatchGuessView4::~MatchGuessView4()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pVs0);
    CC_SAFE_RELEASE(m_pVs1);
    CC_SAFE_RELEASE(m_pVs2);
    CC_SAFE_RELEASE(m_pVs3);
    CC_SAFE_RELEASE(m_pBet0);
    CC_SAFE_RELEASE(m_pBet1);
}

bool MatchGuessView4::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs0", MatchUserHead *, this->m_pVs0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs1", MatchUserHead *, this->m_pVs1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs2", MatchUserHead *, this->m_pVs2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs3", MatchUserHead *, this->m_pVs3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBet0", MatchBetMenu *, this->m_pBet0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBet1", MatchBetMenu *, this->m_pBet1);
    return false;
}

bool MatchGuessView4::init(MatchFinalCat finalNum)
{
	if(!MatchGuessView::init(finalNum))
	{
		return false;
	}
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("MatchUserHead", MatchUserHeadLoader::loader());
    pNodeLoaderLibrary->registerCCNodeLoader("MatchBetMenu", MatchBetMenuLoader::loader());
    //CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/match/matchGuess4", pNodeLoaderLibrary);
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_beisai/PL_match_X4", pNodeLoaderLibrary);
	
    
    CC_SAFE_RELEASE(m_pVsList);
    m_pVsList = CCArray::create(m_pVs0,m_pVs1,m_pVs2,m_pVs3,NULL);
    CC_SAFE_RETAIN(m_pVsList);
    
    CC_SAFE_RELEASE(m_pBetList);
    m_pBetList = CCArray::create(m_pBet0,m_pBet1,NULL);
    CC_SAFE_RETAIN(m_pBetList);
    
	return true;
    
}

#pragma mark MatchGuessView2

MatchGuessView2::MatchGuessView2()
:m_pVs0(NULL)
,m_pVs1(NULL)
,m_pBet0(NULL)
{
    
}
MatchGuessView2::~MatchGuessView2()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pVs0);
    CC_SAFE_RELEASE(m_pVs1);
    CC_SAFE_RELEASE(m_pBet0);
}

bool MatchGuessView2::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs0", MatchUserHead *, this->m_pVs0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVs1", MatchUserHead *, this->m_pVs1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBet0", MatchBetMenu *, this->m_pBet0);
    return false;
}


bool MatchGuessView2::init(MatchFinalCat finalNum)
{
	if(!MatchGuessView::init(finalNum))
	{
		return false;
	}
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("MatchUserHead", MatchUserHeadLoader::loader());
    pNodeLoaderLibrary->registerCCNodeLoader("MatchBetMenu", MatchBetMenuLoader::loader());
    //CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/match/matchGuess2", pNodeLoaderLibrary);
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_beisai/PL_match_X2", pNodeLoaderLibrary);
    CC_SAFE_RELEASE(m_pVsList);
    m_pVsList = CCArray::create(m_pVs0,m_pVs1,NULL);
    CC_SAFE_RETAIN(m_pVsList);
    
    CC_SAFE_RELEASE(m_pBetList);
    m_pBetList = CCArray::create(m_pBet0,NULL);
    CC_SAFE_RETAIN(m_pBetList);    
    
	return true;
}


