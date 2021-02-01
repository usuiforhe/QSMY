//
//  MatchBetView.cpp
//  下注竞猜界面
//
//  Created by cll on 13-4-16
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MatchBetView.h"
#include "managers/ResourceManager.h"
#include "managers/TipsManager.h"


USING_NS_CC;

MatchBetView::MatchBetView()
:m_pVSInfo(NULL)
,m_pItemModel(NULL)
,m_pItem(NULL)
,m_pWarriorModel(NULL)

,m_pTfItemHas(NULL)
,m_pTfItemCost(NULL)
,m_pTfNameLeft(NULL)
,m_pTfNameRight(NULL)
,m_pTfLevelLeft(NULL)
,m_pTfLevelRight(NULL)
,m_pTfRateLeft(NULL)
,m_pTfRateRight(NULL)
,m_pBetBtnLeft(NULL)
,m_pBetBtnRight(NULL)
,m_pReplayMenu(NULL)
,m_pImgBetedLeft(NULL)
,m_pImgBetedRight(NULL)
,m_pImgLive(NULL)
,m_pImgReplay(NULL)
,m_pMatchModel(NULL)
,m_pButtonLeft(NULL)
,m_pButtonRight(NULL)
,m_pButtonMiddle(NULL)
,m_pCloseButton(NULL)
{
    D_RESET_C_ARRAY(m_pWarriorLeft, FORMATION_POS_MAX);
    D_RESET_C_ARRAY(m_pWarriorRight, FORMATION_POS_MAX);
	D_RESET_C_ARRAY(m_pLL, FORMATION_POS_MAX);
	D_RESET_C_ARRAY(m_pLR, FORMATION_POS_MAX);
}

MatchBetView::~MatchBetView()
{
    removeAllChildren();
    SAFE_RELEASE_UNBINDING_ALL(m_pItem, this);
    SAFE_RELEASE_UNBINDING_ALL(m_pVSInfo, this);
    CC_SAFE_RELEASE(m_pTfItemHas);
    CC_SAFE_RELEASE(m_pTfItemCost);
    CC_SAFE_RELEASE(m_pTfNameLeft);
    CC_SAFE_RELEASE(m_pTfNameRight);
    CC_SAFE_RELEASE(m_pTfLevelLeft);
    CC_SAFE_RELEASE(m_pTfLevelRight);
    CC_SAFE_RELEASE(m_pTfRateLeft);
    CC_SAFE_RELEASE(m_pTfRateRight);
    CC_SAFE_RELEASE(m_pBetBtnLeft);
    CC_SAFE_RELEASE(m_pBetBtnRight);
    CC_SAFE_RELEASE(m_pReplayMenu);
    D_SAFE_RELEASE_C_ARRAY(m_pWarriorLeft, FORMATION_POS_MAX);
    D_SAFE_RELEASE_C_ARRAY(m_pWarriorRight, FORMATION_POS_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pLL, FORMATION_POS_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pLR, FORMATION_POS_MAX);
    
    CC_SAFE_RELEASE(m_pImgBetedLeft);
    CC_SAFE_RELEASE(m_pImgBetedRight);
	CC_SAFE_RELEASE(m_pImgLive);
	CC_SAFE_RELEASE(m_pImgReplay);
	CC_SAFE_RELEASE(m_pButtonLeft);
	CC_SAFE_RELEASE(m_pButtonRight);
	CC_SAFE_RELEASE(m_pButtonMiddle);
	CC_SAFE_RELEASE(m_pCloseButton);
}

bool MatchBetView::init()
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pItemModel = DM_GET_ITEM_MODEL;
    m_pMatchModel = DM_GET_MATCH_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    //CCB_READER_AND_ADDCHILD("ui/ccb/match/matchBet");
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beisai/PL_beisai_xiazhu");

	m_pButtonLeft->getButton()->setTarget(this,menu_selector(MatchBetView::leftCallBack));
	m_pButtonRight->getButton()->setTarget(this,menu_selector(MatchBetView::rightCallBack));
	m_pButtonMiddle->getButton()->setTarget(this,menu_selector(MatchBetView::middleCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(MatchBetView::closeCallBack));

    return true;
}

void MatchBetView::onEnter()
{
    CCAssert(m_pVSInfo, "");
    DLayer::onEnter();
    BINDING_EXEC(m_pMatchModel, this, MatchBetView::updateBetView,D_EVENT_MATCH_UPDATE_BETLIST);
    BINDING_EXEC(m_pItemModel, this, MatchBetView::updateItemView, D_EVENT_ITEMLIST_UPDATE);
	BINDING_EXEC(m_pVSInfo, this, MatchBetView::updateVSInfo, D_EVENT_MATCH_UPDATE_VSINFO);
	BINDING_EXEC(m_pVSInfo, this, MatchBetView::updateReplay, D_EVENT_MATCH_REPLAY);
    
    updateView();
}


void MatchBetView::onExit()
{
    DLayer::onExit();
    UNBINDING_ALL(m_pItem, this);
    CC_SAFE_RELEASE_NULL(m_pItem);  //指针清空
    
    UNBINDING_ALL(m_pVSInfo, this);
    UNBINDING_ALL(m_pMatchModel, this);
    UNBINDING_ALL(m_pItemModel, this);
    updateWarriors(m_pWarriorLeft,CCArray::create() , true);
    updateWarriors(m_pWarriorRight,CCArray::create() , false);
}

void MatchBetView::updateVSInfo()
{
	match::VSPlayer *m_pLeft = m_pVSInfo->m_pLeft;
	match::VSPlayer *m_pRight = m_pVSInfo->m_pRight;
	if (m_pVSInfo->getIsSwitched())
	{
		m_pLeft = m_pVSInfo->m_pRight;
		m_pRight = m_pVSInfo->m_pLeft;
	}

    m_pTfRateLeft->setString(CCString::createWithFormat("%d%%",m_pLeft->rate)->getCString());
    m_pTfRateRight->setString(CCString::createWithFormat("%d%%",m_pRight->rate)->getCString());
    m_pReplayMenu->setVisible(m_pVSInfo->winUser!=match::kVSNone);
    updateBetView();
}

void MatchBetView::updateView()
{
    //下面的内容不会变
    
    
    m_pTfItemCost->setString(CCString::createWithFormat("%d",m_pMatchModel->getMatchConfig()->getBetCost((match::MatchFinalCat)m_pVSInfo->finalNum))->getCString());
    
	match::VSPlayer *m_pLeft = m_pVSInfo->m_pLeft;
	match::VSPlayer *m_pRight = m_pVSInfo->m_pRight;
	if (m_pVSInfo->getIsSwitched())
	{
		m_pLeft = m_pVSInfo->m_pRight;
		m_pRight = m_pVSInfo->m_pLeft;
	}
	

    m_pTfNameLeft->setString(m_pLeft->m_pDesp->name.c_str());
    m_pTfNameRight->setString(m_pRight->m_pDesp->name.c_str());
    
    m_pTfLevelLeft->setString(CCString::createWithFormat("%d",m_pLeft->m_pDesp->lv)->getCString());
    m_pTfLevelRight->setString(CCString::createWithFormat("%d",m_pRight->m_pDesp->lv)->getCString());
    
    updateWarriors(m_pWarriorLeft,m_pLeft->m_pDetail , true);
    updateWarriors(m_pWarriorRight,m_pRight->m_pDetail , false);
}

void MatchBetView::updateWarriors(WarriorHeadSmall** heads,cocos2d::CCArray *infos,bool isLeft)
{
    WarriorHeadSmall* pHead = NULL;
    match::FormationInfo* pFormationInfo = NULL;
    uint32_t index = 0;
    warrior::WarriorBaseInfo * pWBaseInfo = NULL;
    
    for (uint32_t i(0); i<FORMATION_POS_MAX; ++i) {
        
        pHead = heads[i];
        pFormationInfo = getFormationInfoByPid(infos, (uint8_t)pHead->getTag());
        
        if(pFormationInfo)
        {
            pWBaseInfo = m_pWarriorModel->getWarriorBaseByWID(pFormationInfo->wid);
            pHead->setWarriorInfo(pWBaseInfo);

            //pHead->getTfLevel()->setString(CCString::createWithFormat("%d",pFormationInfo->lv)->getCString());
			if(isLeft){
				m_pLL[i]->setString(CCString::createWithFormat("%d",pFormationInfo->lv)->getCString());
			}
			else{
				m_pLR[i]->setString(CCString::createWithFormat("%d",pFormationInfo->lv)->getCString());
			}
        }else
        {
            pHead->resetWarriorInfo();
			//若该位置没有单位，隐去点击上阵
			pHead->getTouchToChange()->setVisible(false);
        }
        ++index;
    }
}

match::FormationInfo* MatchBetView::getFormationInfoByPid(cocos2d::CCArray* infos, uint8_t pid)
{
	if (!infos)
	{
		return NULL;
	}
	
	cocos2d::CCObject* obj = NULL;
	match::FormationInfo* pFormationInfo = NULL;
	CCARRAY_FOREACH(infos, obj)
	{
		pFormationInfo = (match::FormationInfo*)obj;
		if (pFormationInfo && pFormationInfo->pid == pid)
		{
			return pFormationInfo;
		}
	}

	return NULL;
}

void MatchBetView::updateBetView()
{
    match::BetInfo* pBetInfo = m_pMatchModel->getBetInfo(m_pVSInfo->finalNum, m_pVSInfo->mid);
    m_pBetBtnLeft->setVisible(pBetInfo==NULL && m_pVSInfo->winUser==match::kVSNone);
	m_pBetBtnRight->setVisible(pBetInfo==NULL && m_pVSInfo->winUser==match::kVSNone);

	match::VS betUser = match::kVSNone;
	if (pBetInfo)
	{
		betUser = (match::VS)pBetInfo->betUser;
		if (m_pVSInfo->getIsSwitched())
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
    m_pImgBetedLeft->setVisible(pBetInfo!=NULL && betUser==match::kVSLeft);
    m_pImgBetedRight->setVisible(pBetInfo!=NULL && betUser==match::kVSRight);
}

void MatchBetView::updateItemView()
{
	UNBINDING_ALL(m_pItem, this);
    CC_SAFE_RELEASE_NULL(m_pItem);
    m_pItem = m_pItemModel->getItemInfoByID(SKY_LIGHT_ID);
    CC_SAFE_RETAIN(m_pItem);
    if(m_pItem)
    {
        BINDING_ONLY(m_pItem, this, MatchBetView::updateItemCount, D_EVENT_ITEMINFO_UPDATE);
    }
    updateItemCount();
}

void MatchBetView::updateItemCount()
{
    uint32_t count = m_pItem?m_pItem->getNum():0;
    m_pTfItemHas->setString(CCString::createWithFormat("%d",count)->getCString());
}


bool MatchBetView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfItemHas",cocos2d::CCLabelBMFont*, this->m_pTfItemHas);      //天灯剩余
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfItemCost",cocos2d::CCLabelBMFont*, this->m_pTfItemCost);     //下注额度
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfNameLeft",cocos2d::CCLabelTTF   *, this->m_pTfNameLeft);     //左边玩家的名字
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfNameRight",cocos2d::CCLabelTTF   *, this->m_pTfNameRight);    //右边玩家的名字
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfLevelLeft",cocos2d::CCLabelBMFont*, this->m_pTfLevelLeft);    //左边玩家等级
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfLevelRight",cocos2d::CCLabelBMFont*, this->m_pTfLevelRight);   //右边玩家等级
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfRateLeft",cocos2d::CCLabelBMFont*, this->m_pTfRateLeft);     //支持率
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfRateRight",cocos2d::CCLabelBMFont*, this->m_pTfRateRight);    //支持率
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBetBtnLeft",cocos2d::CCNode*, this->m_pBetBtnLeft);     //下注
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBetBtnRight",cocos2d::CCNode*, this->m_pBetBtnRight);    //下注
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReplayMenu",cocos2d::CCNode*, this->m_pReplayMenu);     //回放
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorLeft0",WarriorHeadSmall*, this->m_pWarriorLeft[0]);   //头像
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorLeft1",WarriorHeadSmall*, this->m_pWarriorLeft[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorLeft2",WarriorHeadSmall*, this->m_pWarriorLeft[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorLeft3",WarriorHeadSmall*, this->m_pWarriorLeft[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorLeft4",WarriorHeadSmall*, this->m_pWarriorLeft[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorLeft5",WarriorHeadSmall*, this->m_pWarriorLeft[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorLeft6",WarriorHeadSmall*, this->m_pWarriorLeft[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorRight0",WarriorHeadSmall*, this->m_pWarriorRight[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorRight1",WarriorHeadSmall*, this->m_pWarriorRight[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorRight2",WarriorHeadSmall*, this->m_pWarriorRight[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorRight3",WarriorHeadSmall*, this->m_pWarriorRight[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorRight4",WarriorHeadSmall*, this->m_pWarriorRight[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorRight5",WarriorHeadSmall*, this->m_pWarriorRight[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorRight6",WarriorHeadSmall*, this->m_pWarriorRight[6]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLL1",CCLabelBMFont*, this->m_pLL[0]);   //LEVEL
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLL2",CCLabelBMFont*, this->m_pLL[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLL3",CCLabelBMFont*, this->m_pLL[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLL4",CCLabelBMFont*, this->m_pLL[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLL5",CCLabelBMFont*, this->m_pLL[4]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLL6",CCLabelBMFont*, this->m_pLL[5]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLL7",CCLabelBMFont*, this->m_pLL[6]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLR1",CCLabelBMFont*, this->m_pLR[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLR2",CCLabelBMFont*, this->m_pLR[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLR3",CCLabelBMFont*, this->m_pLR[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLR4",CCLabelBMFont*, this->m_pLR[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLR5",CCLabelBMFont*, this->m_pLR[4]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLR6",CCLabelBMFont*, this->m_pLR[5]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLR7",CCLabelBMFont*, this->m_pLR[6]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImgBetedLeft",CCSprite*, this->m_pImgBetedLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImgBetedRight",CCSprite*, this->m_pImgBetedRight);
    

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "match_sprite_live",CCSprite*, this->m_pImgLive);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "match_sprite_replay",CCSprite*, this->m_pImgReplay);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonLeft",DButton*, this->m_pButtonLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonRight",DButton*, this->m_pButtonRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonMiddle",DButton*, this->m_pButtonMiddle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton",DButton*, this->m_pCloseButton);
		

    return false;
}


SEL_MenuHandler MatchBetView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", MatchBetView::closeCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallBack", MatchBetView::menuCallBack);
    return NULL;
}

void MatchBetView::closeCallBack(CCObject * obj)
{
	removeFromParent();
}

void MatchBetView::menuCallBack(cocos2d::CCObject * obj)
{
    
    CCMenuItemImage * pMenuItem = (CCMenuItemImage*) obj;
	int tag = pMenuItem->getTag();
    switch (tag) {
        case kBtnBetLeft:
			if (m_pVSInfo->getIsSwitched())
			{
				doCupBet(m_pVSInfo->finalNum, m_pVSInfo->mid, match::kVSRight);
			}
			else
			{
				doCupBet(m_pVSInfo->finalNum, m_pVSInfo->mid, match::kVSLeft);
			}
            break;
		case kBtnBetRight:
			if (m_pVSInfo->getIsSwitched())
			{
				doCupBet(m_pVSInfo->finalNum, m_pVSInfo->mid, match::kVSLeft);
			}
			else
			{
				doCupBet(m_pVSInfo->finalNum, m_pVSInfo->mid, match::kVSRight);
			}
            break;
        case kBtnReplay:
            HTTP_CLIENT->getCupMatchReplay(m_pVSInfo->finalNum, m_pVSInfo->mid);
            DM_GET_SETTLEMENT_MODEL->setSettlementInfoMatch(m_pVSInfo);
            break;
        default:
            break;
    }
}


void MatchBetView::leftCallBack(cocos2d::CCObject * obj)
{
		if (m_pVSInfo->getIsSwitched())
		{
			doCupBet(m_pVSInfo->finalNum, m_pVSInfo->mid, match::kVSRight);
		}
		else
		{
			doCupBet(m_pVSInfo->finalNum, m_pVSInfo->mid, match::kVSLeft);
		}
}

void MatchBetView::rightCallBack(cocos2d::CCObject * obj)
{
		if (m_pVSInfo->getIsSwitched())
		{
			doCupBet(m_pVSInfo->finalNum, m_pVSInfo->mid, match::kVSLeft);
		}
		else
		{
			doCupBet(m_pVSInfo->finalNum, m_pVSInfo->mid, match::kVSRight);
		}
}

void MatchBetView::middleCallBack(cocos2d::CCObject * obj)
{
	DM_GET_BATTLE_MODEL->setBattleType(battle::kBattleTypeMatch);

	HTTP_CLIENT->getCupMatchReplay(m_pVSInfo->finalNum, m_pVSInfo->mid);
	DM_GET_SETTLEMENT_MODEL->setSettlementInfoMatch(m_pVSInfo);
}

void MatchBetView::doCupBet(uint32_t finalNum, uint32_t mid, uint32_t betUser)
{
    if(m_pMatchModel->getRemainBetTimes()==0)
    {
        Tips_Alert(D_LOCAL_STRING("GuessingUseUp").c_str());
        return;
    }
    
    if(m_pItem==NULL || m_pItem->getNum()<m_pMatchModel->getMatchConfig()->getBetCost((match::MatchFinalCat)m_pVSInfo->finalNum))
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS,CCInteger::create(SKY_LIGHT_ID));
        return;
    }
    HTTP_CLIENT->doCupBet(finalNum, mid, betUser);
    closeCallBack(NULL);
}


void MatchBetView::updateReplay()
{
	if (m_pVSInfo->isWatched())
	{
	m_pImgReplay->setVisible(true);
	m_pImgLive->setVisible(false);
	}
	else
	{
	m_pImgReplay->setVisible(false);
	m_pImgLive->setVisible(true);
	}
	
}


/************************ match rule view end*******************/



