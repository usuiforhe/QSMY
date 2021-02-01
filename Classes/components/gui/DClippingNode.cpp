//
//  DClippingNode.cpp
//  QSMY
//
//  Created by heven on 14-2-27.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "DClippingNode.h"

USING_NS_CC;

DClippingNode::DClippingNode()
{
    
}

DClippingNode::~DClippingNode()
{
    
}

bool DClippingNode::init()
{
    if(!CCNode::init()) return false;
    setContentSize(DESIGN_SIZE);
    return true;
}

void DClippingNode::visit()
{
	// quick return if not visible
	if (!isVisible())
    {
		return;
    }
    
	kmGLPushMatrix();
	
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }
    
	this->transform();
    this->beforeDraw();
    
	if(m_pChildren)
    {
		ccArray *arrayData = m_pChildren->data;
		unsigned int i=0;
		
		// draw children zOrder < 0
		for( ; i < arrayData->num; i++ )
        {
			CCNode *child =  (CCNode*)arrayData->arr[i];
			if ( child->getZOrder() < 0 )
            {
				child->visit();
			}
            else
            {
				break;
            }
		}
		
		// this draw
		this->draw();
		
		// draw children zOrder >= 0
		for( ; i < arrayData->num; i++ )
        {
			CCNode* child = (CCNode*)arrayData->arr[i];
			child->visit();
		}
        
	}
    else
    {
		this->draw();
    }
    
    this->afterDraw();
	if ( m_pGrid && m_pGrid->isActive())
    {
		m_pGrid->afterDraw(this);
    }
    
	kmGLPopMatrix();
}


CCRect DClippingNode::getViewRect()
{
    CCPoint screenPos = this->convertToWorldSpace(CCPointZero);
    
    float scaleX = this->getScaleX();
    float scaleY = this->getScaleY();
    
    for (CCNode *p = m_pParent; p != NULL; p = p->getParent()) {
        scaleX *= p->getScaleX();
        scaleY *= p->getScaleY();
    }
    
    CCSize m_tViewSize = getContentSize();
    return CCRectMake(screenPos.x, screenPos.y, m_tViewSize.width*scaleX, m_tViewSize.height*scaleY);
    
}


void DClippingNode::beforeDraw()
{
    m_bScissorRestored = false;
    CCRect frame = getViewRect();
    if (CCEGLView::sharedOpenGLView()->isScissorEnabled()) {
        m_bScissorRestored = true;
        m_tParentScissorRect = CCEGLView::sharedOpenGLView()->getScissorRect();
        //set the intersection of m_tParentScissorRect and frame as the new scissor rect
        if (frame.intersectsRect(m_tParentScissorRect)) {
            float x = floorf(MAX(frame.origin.x, m_tParentScissorRect.origin.x));
            float y = floorf(MAX(frame.origin.y, m_tParentScissorRect.origin.y));
            float xx = ceilf(MIN(frame.origin.x+frame.size.width, m_tParentScissorRect.origin.x+m_tParentScissorRect.size.width));
            float yy = ceilf(MIN(frame.origin.y+frame.size.height, m_tParentScissorRect.origin.y+m_tParentScissorRect.size.height));
            CCEGLView::sharedOpenGLView()->setScissorInPoints(x, y, xx-x, yy-y);
        }
    }
    else {
        glEnable(GL_SCISSOR_TEST);
        CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    }
}

/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void DClippingNode::afterDraw()
{
    if (m_bScissorRestored) {//restore the parent's scissor rect
        CCEGLView::sharedOpenGLView()->setScissorInPoints(m_tParentScissorRect.origin.x, m_tParentScissorRect.origin.y, m_tParentScissorRect.size.width, m_tParentScissorRect.size.height);
    }
    else {
        glDisable(GL_SCISSOR_TEST);
    }
}

