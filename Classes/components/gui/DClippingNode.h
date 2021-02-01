//
//  DClippingNode.h
//  裁切
//
//  Created by heven on 14-2-27.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__DClippingNode__
#define __QSMY__DClippingNode__

#include "AppInclude.h"

class DClippingNode:public cocos2d::CCNode
{
    
public:
    DClippingNode();
    virtual ~DClippingNode();
    
    CREATE_FUNC(DClippingNode);
    
private:
    
    virtual bool init();
    
    cocos2d::CCRect getViewRect();
    /**
     * clip this view so that outside of the visible bounds can be hidden.
     */
    void beforeDraw();
    /**
     * retract what's done in beforeDraw so that there's no side effect to
     * other nodes.
     */
    void afterDraw();
    
    virtual void visit();

private:
    /**
     * scissor rect for parent, just for restoring GL_SCISSOR_BOX
     */
    cocos2d::CCRect m_tParentScissorRect;
    bool m_bScissorRestored;
};

#endif /* defined(__QSMY__DClippingNode__) */
