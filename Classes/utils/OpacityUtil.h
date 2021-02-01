//
//  OpacityUtil.h
//  QSMY
//
//  Created by wanghejun on 13-5-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__OpacityUtil__
#define __QSMY__OpacityUtil__

#include "AppInclude.h"

USING_NS_CC;

static void setOpacity(CCNode* node,GLubyte alpha)
{
    CCRGBAProtocol *pRGBAProtocol = NULL;
    CCObject* pObj = NULL;
    CCNode *pNode = NULL;
    CCARRAY_FOREACH(node->getChildren(), pObj)
    {
        pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(pObj);
        if (pRGBAProtocol)
        {
            pRGBAProtocol->setOpacity(alpha);
        }
        pNode = dynamic_cast<CCNode*>(pObj);
        if(pNode) setOpacity(pNode, alpha);
    }
    
    
}

#endif /* defined(__QSMY__OpacityUtil__) */
