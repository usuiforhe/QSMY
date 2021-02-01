//
//  BoxPreviewView.cpp
//	宝箱预览
//
//  Created by Eci on 13-11-15.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "BoxPreviewView.h"
#include "model/Model.h"
#include "managers/DataManager.h"


class RewardItem : public ValueObjcet
{
public:
	CREATE_FUNC(RewardItem);
	D_INIT_DEFAULT(ValueObjcet);
	drop::DropType	price_type;
	uint32_t		price_id;
	uint32_t		price_number;
};



BoxPreviewView::BoxPreviewView()
	:m_pButton(NULL)
	,m_pButtonClose(NULL)
	,m_pLabelName(NULL)
	,m_pButtonToHide(NULL)
	,m_pNodeToHide(NULL)
	,m_pLabelToHide(NULL)
	,m_pCheck(NULL)
	,m_pTitleCommon(NULL)
{	
};

BoxPreviewView::~BoxPreviewView()
{
	CC_SAFE_RELEASE(m_pButton);
	CC_SAFE_RELEASE(m_pButtonClose);
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pButtonToHide);
	CC_SAFE_RELEASE(m_pNodeToHide);
	CC_SAFE_RELEASE(m_pLabelToHide);
	CC_SAFE_RELEASE(m_pCheck);
	CC_SAFE_RELEASE(m_pTitleCommon);
}

BoxPreviewView *BoxPreviewView::create(cocos2d::CCArray *pAwardList)
{
	BoxPreviewView *pRet = new BoxPreviewView();
	if (pRet && pRet->init(pAwardList))
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

bool BoxPreviewView::init(cocos2d::CCArray *pAwardList)
{
	if(pAwardList==NULL) return false;
	
	return BaseAwardListView::init(pAwardList);
}

void BoxPreviewView::closeCallBack(cocos2d::CCObject *obj)
{
	removeFromParent();
}

void BoxPreviewView::addChildrenBeforeList()
{    
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_shiliankai");
	if(m_pButton)m_pButton->getButton()->setTarget(this,menu_selector(BoxPreviewView::closeCallBack));
	if(m_pButtonClose)m_pButtonClose->getButton()->setTarget(this,menu_selector(BoxPreviewView::closeCallBack));
	if(m_pLabelName)m_pLabelName->setString("");
	if(m_pButtonToHide)m_pButtonToHide->setVisible(false);
	if(m_pNodeToHide)m_pNodeToHide->setVisible(false);
	if(m_pLabelToHide)m_pLabelToHide->setVisible(false);
}



bool BoxPreviewView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pButtonClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonUse", DButton *, this->m_pButtonToHide);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUse", CCNode *, this->m_pNodeToHide);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmount", CCNode *, this->m_pLabelToHide);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCheck",CCNode *,  this->m_pCheck);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleCommon",CCNode *,  this->m_pTitleCommon);
	return BaseAwardListView::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}


void BoxPreviewView::setAwardName(const char *pName)
{
	if (m_pLabelName) {
        m_pLabelName->setString(pName);
    }
}

void BoxPreviewView::showLabel(BoxViewLabelType labelType)
{
	m_pTitleCommon->setVisible(false);
	m_pCheck->setVisible(false);
	switch(labelType)
	{
	case kCheck:
		m_pCheck->setVisible(true); break;
	case kCongretulation:
		m_pTitleCommon->setVisible(true); break;
	}
}