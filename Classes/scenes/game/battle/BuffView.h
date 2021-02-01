//
//  BuffEffect.h
//  QSMY
//
//  Created by wanghejun on 13-3-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BuffEffect__
#define __QSMY__BuffEffect__

#include "AppInclude.h"
#include "DragonBone.h"


/**
 *	@brief	buff效果
 */
class BuffEffect:public cocos2d::CCNode
{
    
public:
    
    BuffEffect()
    
    :m_pArmature(NULL)
    ,m_fDelay(0.0f)
    {
        
    }
    virtual ~BuffEffect()
    {
        CC_SAFE_RELEASE(m_pArmature);
		m_pArmature->getAnimation()->removeObserver(this, COMPLETE);
    }
    
    static BuffEffect* create(uint32_t buff_id,const char* action_name,float delay);
    
    virtual float getDuration();
    
    /**
     * Sets the Z order which stands for the drawing order, and reorder this node in its parent's children array.
     *
     * The Z order of node is relative to its "brothers": children of the same parent.
     * It's nothing to do with OpenGL's z vertex. This one only affects the draw order of nodes in cocos2d.
     * The larger number it is, the later this node will be drawn in each message loop.
     * Please refer to setVertexZ(float) for the difference.
     *
     * @param nZOrder   Z order of this node.
     */
    virtual void setZOrder(int zOrder);
    /**
     * Gets the Z order of this node.
     *
     * @see setZOrder(int)
     *
     * @return The Z order.
     */
    virtual int getZOrder();

	void stopAnimation();
    
protected:
    void startAnimation();
    virtual bool init(uint32_t buff_id,const char* action_name,float delay);
    void onMovementEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
    
protected:
    sp::Armature * m_pArmature;
    float m_fDelay;
	uint32_t m_nBuffID;
};


#endif /* defined(__QSMY__BuffEffect__) */
