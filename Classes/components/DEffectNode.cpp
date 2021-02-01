//
//  DEffectNode.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DEffectNode.h"

USING_NS_CC;

DEffectNode::DEffectNode()
:m_pCallBack(NULL)
,m_pTarget(NULL)
{
    
}

DEffectNode::~DEffectNode()
{
    CC_SAFE_RELEASE(m_pTarget);
}

void DEffectNode::setCallBack(cocos2d::CCObject *pTarget,cocos2d::SEL_CallFuncO pCallBack)
{
    CC_SAFE_RETAIN(pTarget);
    CC_SAFE_RELEASE(m_pTarget);
    m_pTarget = pTarget;
    m_pCallBack = pCallBack;
}

void DEffectNode::callback()
{
    if(m_pTarget) (m_pTarget->*m_pCallBack)(this);
}

void DEffectNode::removeFromParentAndCleanup(bool cleanup)
{
    DEffectLayer* pParent = dynamic_cast<DEffectLayer*>(m_pParent);
    DLayerModal::removeFromParentAndCleanup(cleanup);
    if (pParent != NULL)
    {
        pParent->update(0.f);
    }
}

#pragma mark DeffectLayer

DEffectLayer::DEffectLayer()
:deque(NULL)
{
    
}

DEffectLayer::~DEffectLayer()
{
    CC_SAFE_RELEASE(deque);
}


bool DEffectLayer::init()
{
    if(!DLayer::init()) return false;
    deque = CCArray::create();
    CC_SAFE_RETAIN(deque);
    return true;
}

void DEffectLayer::addChild(CCNode *child, int zOrder, int tag)
{
    CCAssert(0, "plz use addNode");
}

void DEffectLayer::addNode(DEffectNode* pNode,bool showImm)
{
    if(showImm || getChildrenCount()==0)
    {
        CCNode::addChild(pNode, 0, -1);
    }else
    {
        if(deque->count()==0)
        {
            scheduleUpdate();
        }
        deque->addObject(pNode);
    }
}

void DEffectLayer::update(float dt)
{
    if(getChildrenCount()==0)
    {
        if(deque->count()>0)
        {
            DEffectNode* pNode = (DEffectNode*)deque->objectAtIndex(0);
            CCNode::addChild(pNode, 0, -1);
            deque->removeObjectAtIndex(0);
        }
        if(deque->count()==0)
        {
            unscheduleUpdate();
        }
    }
}

void DEffectLayer::removeAllNode()
{
	if(deque->count()>0)
	{
		unscheduleUpdate();
		CCNode::removeAllChildrenWithCleanup(true);
		while(deque->count()>0)
		{
			deque->removeObjectAtIndex(0);
		}
	}
}



