//
//  SettlementViewMatch.cpp
//  QSMY
//
//  Created by appleos on 14-1-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "SettlementViewMatch.h"


USING_NS_CC;


SettlementViewMatch::SettlementViewMatch()
:m_pButton(NULL)
,m_pContainer(NULL)
,m_pLabelName(NULL)
,m_pLabelLevel(NULL)
,m_pLabelRank(NULL)
,m_pInfo(NULL)
,m_pDelegate(NULL)
,m_pCloseMenu(NULL)
{
    D_RESET_C_ARRAY(m_pNode, NODE_MAX);
}

SettlementViewMatch::~SettlementViewMatch()
{
    CC_SAFE_RELEASE(m_pButton);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pLabelName);
    CC_SAFE_RELEASE(m_pLabelLevel);
    CC_SAFE_RELEASE(m_pLabelRank);
    CC_SAFE_RELEASE(m_pInfo);
	CC_SAFE_RELEASE(m_pCloseMenu);
    D_SAFE_RELEASE_C_ARRAY(m_pNode, NODE_MAX);
}


bool SettlementViewMatch::init(SettlementDelegate *delegate,settlement::SettlementInfo *pInfo)
{
    CCAssert(delegate!=NULL && dynamic_cast<settlement::SettlementInfoMatch*>(pInfo)!=NULL, "");
    if(!DLayer::init()) return false;
    CC_SAFE_RELEASE(m_pInfo);
    CC_SAFE_RETAIN(pInfo);
    m_pInfo = dynamic_cast<settlement::SettlementInfoMatch*>(pInfo);
    m_pDelegate = delegate;
    
    //CCB_READER_AND_ADDCHILD("ui/ccb/match/matchSettle");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beisai/PL_beisai_jiesuan");
    
    //m_pButton->getButton()->setTarget(this, menu_selector(SettlementViewMatch::callBackClose));
	m_pCloseMenu->setTarget(this, menu_selector(SettlementViewMatch::callBackClose));
    
    m_pLabelName->setString(m_pInfo->getUserName().c_str());
    m_pLabelLevel->setString(CCString::createWithFormat("%u", m_pInfo->getUserLevel())->getCString());
    
    for (int i = 0; i < NODE_MAX; i++) {
        m_pNode[i]->setVisible(false);
    }
    
    if (m_pInfo->getRank() == 1) {
        m_pNode[2]->setVisible(true);
    }
    else if (m_pInfo->getRank() == 2)
    {
        m_pNode[1]->setVisible(true);
    }
    else if (m_pInfo->getRank() == 3 || m_pInfo->getRank() == 4)
    {
        m_pNode[0]->setVisible(true);
        //m_pLabelRank->setString(CCString::createWithFormat("%d", (int)(log((float)m_pInfo->getRank()) / log(2.f)))->getCString());
    }
	else
	{
		m_pNode[3]->setVisible(true);
	}
    
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(m_pInfo->getAvatarID()));
    
    return true;
}

bool SettlementViewMatch::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, m_pButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelName", CCLabelTTF *, m_pLabelName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelLevel", CCLabelBMFont *, m_pLabelLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelRank", CCLabelBMFont *, m_pLabelRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseMenu", CCMenuItem *, m_pCloseMenu);
	
    D_CCB_ASSIGN_ARRAY("m_pNode", CCNode *, m_pNode, NODE_MAX);
    return false;
}

void SettlementViewMatch::closeView()
{
	if(m_pDelegate) m_pDelegate->quitBattle();
}

void SettlementViewMatch::callBackClose(CCObject *sender)
{
    closeView();
}