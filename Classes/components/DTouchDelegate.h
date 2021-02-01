//
//  DTouchDelegate.h
//  事件委托
//
//  Created by wanghejun on 13-3-9.
//	Modified by mzp on 14-4-21
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DTouchDelegate__
#define __QSMY__DTouchDelegate__

#include "AppInclude.h"

class DTouchDelegate
{
public:
    DTouchDelegate()
    :m_pOwner( NULL )
    ,m_bDraging( false )
    {
        m_pItemsClaimTouch = cocos2d::CCArray::createWithCapacity( 255 );
        assert( m_pItemsClaimTouch );
        m_pItemsClaimTouch->retain();
        
        m_pScrollViewClaimTouch = cocos2d::CCArray::createWithCapacity( 255 );
        assert( m_pScrollViewClaimTouch );
        m_pScrollViewClaimTouch->retain();

		m_pDraggingViewClaimTouch = cocos2d::CCArray::createWithCapacity( 255 );
		assert( m_pDraggingViewClaimTouch );
		m_pDraggingViewClaimTouch->retain();
    }
    virtual ~DTouchDelegate()
    {
        CC_SAFE_RELEASE_NULL( m_pItemsClaimTouch );
        CC_SAFE_RELEASE_NULL( m_pScrollViewClaimTouch );
		CC_SAFE_RELEASE_NULL( m_pDraggingViewClaimTouch );
    }
public:
    // default implements are used to call script callback if exist
    virtual bool dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches = NULL);
    virtual void dTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void dTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
protected:    
    //return value:
    //true: pParent is touched by user
    //false: pParent isn't touched by user.
    bool passEvent( cocos2d::CCNode* pParent, cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches);
private:
   
    bool m_bDraging;
    //items claim touch message
    cocos2d::CCArray* m_pItemsClaimTouch;
    cocos2d::CCArray* m_pScrollViewClaimTouch;
	cocos2d::CCArray* m_pDraggingViewClaimTouch;
protected:
     cocos2d::CCNode* m_pOwner;
};


#endif /* defined(__QSMY__DTouchDelegate__) */
