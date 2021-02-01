//
//  DMessageBox.cpp
//  提示框
//
//  Created by ldr123 on 13-7-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DMessageBox.h"
#include "components.h"

USING_NS_CC;

DMessageBox::DMessageBox()
:m_pTarget(NULL)
,m_pCallBack(NULL)
, m_msg(NULL)
,m_pOK(NULL)
,m_pCancel(NULL)
{}
DMessageBox::~DMessageBox()
{
	CC_SAFE_RELEASE(m_msg);
    CC_SAFE_RELEASE(m_pTarget);
	CC_SAFE_RELEASE(m_pOK);
	CC_SAFE_RELEASE(m_pCancel);
}

static cocos2d::CCNode* m_pContainer = NULL;

void DMessageBox::setContainer(cocos2d::CCNode*pNode)
{
    CC_SAFE_RETAIN(pNode);
    CC_SAFE_RELEASE(m_pContainer);
    m_pContainer = pNode;
}

DMessageBox * DMessageBox::showMessageBox(const char *pMsg)
{
    DMessageBox* pRet =  DMessageBox::create(pMsg);
    if(pRet)
    {
        CC_ASSERT(m_pContainer);
        m_pContainer->addChild(pRet);
    }
    return pRet;
}

DMessageBox * DMessageBox::showMessageBox(const char *pMsg,cocos2d::CCObject* target,MessageBox_CallFunc callFn)
{
    DMessageBox* pRet =  DMessageBox::create(pMsg,target,callFn);
    if(pRet)
    {
        CC_ASSERT(m_pContainer);
        m_pContainer->addChild(pRet);
    }
    return pRet;
}


DMessageBox* DMessageBox::create(const char *pMsg)
{
	return DMessageBox::create(pMsg,NULL,NULL);
}

DMessageBox* DMessageBox::create(const char *pMsg,cocos2d::CCObject* target,MessageBox_CallFunc callFn)
{
	DMessageBox *pBox = new DMessageBox;
	if (pBox && pBox->init(pMsg,target,callFn))
	{
		pBox->autorelease();
		return pBox;
	}

	CC_SAFE_DELETE(pBox);
	return NULL;
}

bool DMessageBox::init(const char *pMsg,cocos2d::CCObject* target,MessageBox_CallFunc callFn)
{
	if (!DLayerModal::init())
	{
		return false;
	}

    CC_SAFE_RETAIN(target);
    CC_SAFE_RELEASE(m_pTarget);
    m_pTarget = target;
    m_pCallBack = callFn;

	//CCB_READER_AND_ADDCHILD("ui/ccb/common/messageBox");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_common/pl_common_tanchu");
    
	m_msg->setString(pMsg);
	m_pOK->getButton()->setTarget(this,menu_selector(DMessageBox::onTouchOk));
	m_pCancel->getButton()->setTarget(this,menu_selector(DMessageBox::onTouchCancel));

	return true;
}

bool DMessageBox::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_msg",			CCLabelTTF *,		m_msg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOK",			DButton *,		m_pOK);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCancel",			DButton *,		m_pCancel);

	return false;
}

SEL_MenuHandler DMessageBox::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onOk",		DMessageBox::onTouchOk);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCancel",	DMessageBox::onTouchCancel);

	return NULL;
}

void DMessageBox::onTouchOk(CCObject* pObj)
{
	if (m_pTarget)
	{
		if(m_pTarget) (m_pTarget->*m_pCallBack)(kMessageTypeOk,this);
	}
    removeFromParent();
}

void DMessageBox::onTouchCancel(CCObject* pObj)
{
	if (m_pTarget)
	{
		if(m_pTarget) (m_pTarget->*m_pCallBack)(kMessageTypeCancel,this);
	}
    removeFromParent();
}
