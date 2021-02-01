//
//  PLAdaptiveNode.cpp
//  QSMY
//
//  Created by Eci on 14-6-18.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "PLAdaptiveNode.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PROPERTY_MOVE "moveWithResolutionDiff"
#define PROPERTY_FLEXIBLE   "flexibleHeight"
#define PROPERTY_DESIGN_SIZE	"designSize"



PLAdaptiveNode::PLAdaptiveNode()
	: m_fMoveWithResolutionDiff(0.f)
    , m_fFlexibleHeight(0.f)
{
    
}

PLAdaptiveNode::~PLAdaptiveNode()
{
}

void PLAdaptiveNode::setMoveWithResolutionDiff(float var)
{
	m_fMoveWithResolutionDiff = var;
//	PL_MOVE_WITH_RESOLUTION_DIFF(this, var);
}

void PLAdaptiveNode::setFlexibleHeight(float var)
{
    m_fFlexibleHeight = var;
//    PL_MOVE_WITH_FLEXIBLE_HEIGHT(this, var);
}

void PLAdaptiveNode::onNodeLoaded(cocos2d::CCNode *pNode, cocos2d::extension::CCNodeLoader *pNodeLoader)
{
     PL_MOVE_WITH_RESOLUTION_DIFF(this, m_fMoveWithResolutionDiff);
 	PL_MOVE_WITH_FLEXIBLE_HEIGHT(this, m_fFlexibleHeight);
}

void PLAdaptiveNodeLoader::onHandlePropTypeFloat(CCNode * pNode, CCNode * pParent, const char* pPropertyName, float pFloat, CCBReader * pCCBReader)
{
    if(strcmp(pPropertyName, PROPERTY_MOVE) == 0) {
		((PLAdaptiveNode *)pNode)->setMoveWithResolutionDiff(pFloat);
    }
	else if (strcmp(pPropertyName, PROPERTY_FLEXIBLE) == 0)
    {
		((PLAdaptiveNode *)pNode)->setFlexibleHeight(pFloat);
    }
    else
	{
        cocos2d::extension::CCNodeLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName,pFloat,pCCBReader);
    }
}

void PLAdaptiveNodeLoader::onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char* pPropertyName, CCSize pSize, CCBReader * pCCBReader)
{
	if(strcmp(pPropertyName, PROPERTY_DESIGN_SIZE) == 0) {
	}
	else
	{
		cocos2d::extension::CCNodeLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName,pSize,pCCBReader);
	}
}