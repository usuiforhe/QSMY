//
//  ActivenessAwardView.cpp
//  QSMY
//
//  Created by OnePiece on 13-6-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ActivenessAwardView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"
#include "../Game.h"
#include "Activeness.h"

using namespace activeness;
using namespace item;

USING_NS_CC;
USING_NS_CC_EXT;


ActivenessAwardView::ActivenessAwardView()
:BaseAwardListView()
,m_pPoint(NULL)
,m_pButtonContainer(NULL)
,m_pButton(NULL)
,m_pAwardInfo(NULL)
{
    
}

ActivenessAwardView::~ActivenessAwardView()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pPoint);
    CC_SAFE_RELEASE(m_pButtonContainer);
    CC_SAFE_RELEASE(m_pButton);
    CC_SAFE_RELEASE(m_pAwardInfo);
}

ActivenessAwardView * ActivenessAwardView::create(activeness::AwardInfo * pAwardInfo)
{
    ActivenessAwardView * pRet = new ActivenessAwardView();
    if(pRet && pRet->init(pAwardInfo))
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

void ActivenessAwardView::onEnter()
{
    BaseAwardListView::onEnter();
    BINDING_ONLY_O(DM_GET_ACTIVENESS_MODEL, this, ActivenessAwardView::onCloseClick, D_EVENT_ACTIVENESS_AWARD_DRAWN);
    BINDING_ONLY(DM_GET_ACTIVENESS_MODEL, this, ActivenessAwardView::updateAwardListView, D_EVENT_ACTIVENESS_UPDATE);
}

void ActivenessAwardView::onExit()
{
    UNBINDING_ALL(DM_GET_ACTIVENESS_MODEL, this);
    BaseAwardListView::onExit();
}

bool ActivenessAwardView::init(activeness::AwardInfo *pAwardInfo)
{
    if(pAwardInfo == NULL)
    {
        return false;
    }
    m_pAwardInfo = pAwardInfo;
    CC_SAFE_RETAIN(m_pAwardInfo);
    
    CCArray * pAwardList = CCArray::create();
    CCObject * obj(NULL);
    CCARRAY_FOREACH(pAwardInfo->getAwardItems(), obj)
    {
        AwardItemInfo * pAwardItem = dynamic_cast<AwardItemInfo*>(obj);
        if(pAwardItem != NULL)
        {
            drop::Drop * pDrop = pAwardItem->toDrop(pAwardInfo);
            if(pDrop != NULL)
            {
                pAwardList->addObject(pDrop);
            }
        }
    }


    return BaseAwardListView::init(pAwardList);
}



bool ActivenessAwardView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPoint", cocos2d::CCLabelBMFont *, this->m_pPoint);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonContainer", cocos2d::CCNode *, this->m_pButtonContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);
    
    return BaseAwardListView::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}

cocos2d::SEL_MenuHandler ActivenessAwardView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", ActivenessAwardView::onCloseClick);
    
    return BaseAwardListView::onResolveCCBCCMenuItemSelector(pTarget,pSelectorName);
}

void ActivenessAwardView::addChildrenBeforeList()
{
    CCB_READER_AND_ADDCHILD("ui/ccb/activeness/activenessReward");
}

void ActivenessAwardView::updateAwardListView()
{
    ActivenessModel * pModel = DM_GET_ACTIVENESS_MODEL;
    
    m_pPoint->setString(CCString::createWithFormat("%d", m_pAwardInfo->getRequirePoint())->getCString());
    
    bool drawn(pModel->isAwardDrawn(m_pAwardInfo));
    m_pButton->getButton()->setEnabled(pModel->isAwardAvailable(m_pAwardInfo));
    m_pButton->getButton()->setTarget(this, menu_selector(ActivenessAwardView::onDrawClick));
    m_pButtonContainer->setVisible(!drawn);
}

void ActivenessAwardView::onDrawClick(cocos2d::CCObject *sender)
{
    HTTP_CLIENT->getActivenessAward(m_pAwardInfo->getRequirePoint());
}

void ActivenessAwardView::onCloseClick(cocos2d::CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->closeAwardView();
	}
	
    //removeFromParent();
}
