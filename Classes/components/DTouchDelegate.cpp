//
//  DTouchDelegate.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-9.
//	Modified by mzp on 14-4-21
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DTouchDelegate.h"
#include "DLayer.h"
#include "components/DDraggingView.h"

//#include "DUtility.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma 标记开始点击
bool DTouchDelegate::dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches)
{
    CCAssert(m_pOwner, "");
    //把消息传递给自己的子节点
    return passEvent( m_pOwner, pTouch, pEvent ,bSwallowsTouches);
}

void DTouchDelegate::dTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //如果移动移动了，说明用户希望取消本次点击，执行滚动操作 
    CCScrollView* pScrollView;
    //传递点击消息给所有CCScrollView，
    for( uint32_t i = 0; i < m_pScrollViewClaimTouch->count(); ++i )
    {
        pScrollView = ( CCScrollView* )m_pScrollViewClaimTouch->objectAtIndex( i );
        if(pScrollView->isRunning()) pScrollView->ccTouchMoved( pTouch, pEvent );
        else
        {
            m_pScrollViewClaimTouch->removeObject(pScrollView);
            --i;
        }
    }

	//传递移动消息给所有DDraggingView
	DDraggingView* pDraggingView;
	for( uint32_t i = 0; i < m_pDraggingViewClaimTouch->count(); ++i )
	{
		pDraggingView = ( DDraggingView* )m_pDraggingViewClaimTouch->objectAtIndex( i );
		if(pDraggingView->isRunning()) pDraggingView->ccTouchMoved( pTouch, pEvent );
		else
		{
			m_pDraggingViewClaimTouch->removeObject(pDraggingView);
			--i;
		}
	}

    CCLayer* pLayer;
    //如果有滚动项被点击
    if( m_pScrollViewClaimTouch->count() > 0 ||
		m_pDraggingViewClaimTouch->count() > 0)
    {
        //取消所有非滚动的点击事件
        for(uint32_t i = 0; i < m_pItemsClaimTouch->count(); ++i )
        {
            pLayer = ( CCLayer* )m_pItemsClaimTouch->objectAtIndex( i );
            if(pLayer->isRunning()) pLayer->ccTouchCancelled( pTouch, pEvent );
        }
        m_pItemsClaimTouch->removeAllObjects();
    }else
    {
        //传递移动消息给其他不是CCScrollView的子节点
        for(uint32_t i = 0; i < m_pItemsClaimTouch->count(); ++i )
        {
            pLayer = ( CCLayer* )m_pItemsClaimTouch->objectAtIndex( i );
            if(pLayer->isRunning()) pLayer->ccTouchMoved( pTouch, pEvent );
            else
            {
               m_pItemsClaimTouch->removeObject(pLayer);
                --i;
            }
        }
    }
}

void DTouchDelegate::dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //传递点击结束消息给其他不是CCScrollView的子节点
    CCLayer* pLayer;
    for( uint32_t i = 0; i < m_pItemsClaimTouch->count(); ++i )
    {
        pLayer = ( CCLayer* )m_pItemsClaimTouch->objectAtIndex( i );
        if(pLayer->isRunning())
        {
            pLayer->ccTouchEnded( pTouch, pEvent );
        }
    }
    m_pItemsClaimTouch->removeAllObjects();
    
    //传递点击结束消息给所有CCScrollView
    CCScrollView* pScrollView;
    for(uint32_t i = 0; i < m_pScrollViewClaimTouch->count(); ++i )
    {
        pScrollView = ( CCScrollView* )m_pScrollViewClaimTouch->objectAtIndex( i );
        if(pScrollView->isRunning()) pScrollView->ccTouchEnded( pTouch, pEvent );
    }
    m_pScrollViewClaimTouch->removeAllObjects();
    
    //传递点击结束消息给所有DDraggingView
	DDraggingView* pDraggingView;
	for(uint32_t i = 0; i < m_pDraggingViewClaimTouch->count(); ++i )
	{
		pDraggingView = ( DDraggingView* )m_pDraggingViewClaimTouch->objectAtIndex( i );
		if(pDraggingView->isRunning()) pDraggingView->ccTouchEnded( pTouch, pEvent );
	}
	m_pDraggingViewClaimTouch->removeAllObjects();
}

void DTouchDelegate::dTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    //传递点击被打断的消息给其他不是CCScrollView的子节点
    CCLayer* pLayer;
    for( uint32_t i = 0; i < m_pItemsClaimTouch->count(); ++i )
    {
        pLayer = ( CCLayer* )m_pItemsClaimTouch->objectAtIndex( i );
        if(pLayer->isRunning()) pLayer->ccTouchCancelled( pTouch, pEvent );
    }
    m_pItemsClaimTouch->removeAllObjects();
    
    //传递点击被打断的消息给所有CCScrollView
    CCScrollView* pScrollView;
    for(uint32_t i = 0; i < m_pScrollViewClaimTouch->count(); ++i )
    {
        pScrollView = ( CCScrollView* )m_pScrollViewClaimTouch->objectAtIndex( i );
        if(pScrollView->isRunning()) pScrollView->ccTouchCancelled( pTouch, pEvent );
    }
    m_pScrollViewClaimTouch->removeAllObjects();

	//传递点击被打断消息给所有DDraggingView
	DDraggingView* pDraggingView;
	for(uint32_t i = 0; i < m_pDraggingViewClaimTouch->count(); ++i )
	{
		pDraggingView = ( DDraggingView* )m_pDraggingViewClaimTouch->objectAtIndex( i );
		if(pDraggingView->isRunning()) pDraggingView->ccTouchCancelled( pTouch, pEvent );
	}
	m_pDraggingViewClaimTouch->removeAllObjects();
}



bool DTouchDelegate::passEvent( cocos2d::CCNode* pParent, cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches )
{
    if( !pParent || !pParent->isVisible() )
    {
        return false;
    }
    
    bool containPoint = false;
    
    //if the item'size > 1, check whether use touches it. Such as TableView.
    //some items doesn't get size. they are medium for maintaining some children. Such as CCTableViewCell.
    if( pParent->getContentSize().width * pParent->getContentSize().height > 1.0f )
    {
        CCPoint pt = pTouch->getLocation();
        CCScrollView* pScrollView;
        CCRect rcBoundingBox;
        
        if( ( pScrollView = dynamic_cast< CCScrollView* >( pParent ) ) )
        {
            rcBoundingBox = CCRectMake( 0, 0, pScrollView->getViewSize().width, pScrollView->getViewSize().height );
        }else
        {
            rcBoundingBox = CCRectMake( 0, 0, pParent->getContentSize().width, pParent->getContentSize().height );
        }
        
        rcBoundingBox = CCRectApplyAffineTransform( rcBoundingBox, pParent->nodeToWorldTransform() );
        
        if( !rcBoundingBox.containsPoint( pt ) )
        {
            return false;
        }
        containPoint = true;
    }
    
    //获取所有子节点
    CCArray* pChildren = pParent->getChildren();
    
    //如果没有子节点，返回该节点是否认领事件
    if( !pChildren || pChildren->count()==0)
    {
        DLayer * pDLayer = NULL;
        //没有子节点的DLayer 通过 isDTouchEnabled 检查是否要认领事件
        if( ( pDLayer = dynamic_cast< DLayer* >( pParent ) ) )
        {
            return pDLayer->isDTouchEnabled();
        }
        //没有子节点的CCMenu 不认领事件
        CCMenu * pMenu = NULL;
        if( ( pMenu = dynamic_cast< CCMenu* >( pParent ) ) )
        {
            return false;
        }
        
        return containPoint;   //如果一个没有子节点的节点想认领事件它必须包含点击区
    }
    
	//按照深度排序子节点
    pParent->sortAllChildren();
    
	
    CCObject* pObject = NULL;
    
    //遍历所有子节点
    CCARRAY_FOREACH_REVERSE( pChildren, pObject )
    {
        //标记该节点是否认领事件
        bool bClaim = false;
        
        CCLayer* pLayer = NULL;
        CCNode* pNode = NULL;
        DTouchDelegate* pDTouchDelegate = NULL;
        
        pNode = ( CCNode*  )pObject;
        assert( pNode );
        if(!pNode->isVisible()) continue;
        
        bool __swallowsTouches = false;
        
        //如果该节点是CCLayer类型 调用它的ccTouchBegan()方法
        //Make sure that you have commented the CCAssertion statement in CCLayer::ccTouchBegan()
        
        if((pDTouchDelegate = dynamic_cast< DTouchDelegate* >( pNode )))
        {
            if(dynamic_cast< DLayerModal* >( pNode ) || dynamic_cast< DRoot* >( pNode ) )
            {
                continue;
            }
            bClaim = pDTouchDelegate->dTouchBegan( pTouch, pEvent ,&__swallowsTouches);
        }
        else
        {
            if( ( pLayer = dynamic_cast< CCLayer* >( pNode ) ) )
            {
                bClaim = pLayer->ccTouchBegan( pTouch, pEvent );
                if(bClaim) //增加对空层的排除
                {
                    if(pLayer->getChildrenCount()==0)
                    {
                        CCRect rcBoundingBox = CCRectMake( 0, 0, pLayer->getContentSize().width, pLayer->getContentSize().height );
                        rcBoundingBox = CCRectApplyAffineTransform( rcBoundingBox, pLayer->nodeToWorldTransform() );
                        bClaim = rcBoundingBox.containsPoint(pTouch->getLocation());
                    }
                }
            }
            passEvent( pNode, pTouch, pEvent ,&__swallowsTouches);
        }
        
        //如果该节点对此消息感兴趣，则把它加入到认领列表
        if( bClaim )
        {
            if ( dynamic_cast< CCScrollView* >( pNode ) )
            {
                m_pScrollViewClaimTouch->addObject( pNode );
            }
			else if ( dynamic_cast< DDraggingView* >( pNode ) )
			{
				m_pDraggingViewClaimTouch->addObject( pNode );
			}
            else if( dynamic_cast< CCLayer* >( pNode ) )
            {
                m_pItemsClaimTouch->addObject( pNode );
            }
			
                if(bSwallowsTouches) *bSwallowsTouches = true;
                return true;
            
        }
        if(__swallowsTouches)
        {
            //如果阻止事件传递 向上递归
            if(bSwallowsTouches) *bSwallowsTouches = true;
            break;
        }
    }
    //判断是否有子节点认领了触摸事件
    return m_pScrollViewClaimTouch->count()+m_pItemsClaimTouch->count()+m_pDraggingViewClaimTouch->count()>0;
}

