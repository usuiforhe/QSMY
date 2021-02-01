//
//  DLayer.h
//  层
//
//  Created by wanghejun on 13-3-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DLayer__
#define __QSMY__DLayer__

#include "AppInclude.h"
#include "../AppMacros.h"
#include "DTouchDelegate.h"

#define FIX_MOVE_DIS 10.0f //修正轻移 防止快速点击误差造成移动


//DLayer 不可触摸
//DLayer 来转发所有触摸消息
class BaseLayer : public cocos2d::CCLayer, public DTouchDelegate
{
    //static
public:
    CREATE_FUNC( BaseLayer );
    
    virtual void onEnter()
    {
        setChildrenTouchDisable( this );
        cocos2d::CCLayer::onEnter();
    }
    
protected:
    
    virtual bool init()
    {
        if(!cocos2d::CCLayer::init()) return false;
		//width取值屏幕宽度，保证ipad上layer够宽，height取设计分辨率960，保证iphone4上可以点击960至1136范围内的按钮
		setContentSize(cocos2d::CCSizeMake(cocos2d::CCDirector::sharedDirector()->getWinSize().width , DESIGN_SIZE.height));
        m_pOwner = this;
        return true;
    }
    
    D_MESSAGE_BRIDGE();
    
   
    //取消所有CCLayer的默认触摸事件
    void setChildrenTouchDisable( cocos2d::CCNode* pParent )
    {
        cocos2d::CCArray* pChildren = pParent->getChildren();
        if( !pChildren )
        {
            return;
        }
        int iNumChildren = pChildren->count();
        for( int i = 0; i < iNumChildren; ++i )
        {
            cocos2d::CCLayer* pLayer = NULL;
            cocos2d::CCNode* pChild = ( cocos2d::CCNode* )pChildren->objectAtIndex( i );
            if (( pLayer = dynamic_cast< cocos2d::CCLayer* >( pChild )) )
            {
                pLayer->setTouchEnabled( false );
            }
            
            setChildrenTouchDisable( pChild );
        }
    }
};

//场景ui的根节点
class DRoot : public BaseLayer
{
protected:
    virtual bool init()
    {
        if(!BaseLayer::init())
        {
            return false;
        }
        
        setTouchEnabled(true);
        return true;
    }
    D_TOUCH_REGISTER_DEFAULT(0);
    
    //static
public:
    
    DRoot()
    :m_bTouched(false)
    ,m_pTouch(NULL)
    {
        
    }
    CREATE_FUNC( DRoot );
    //data
    
    virtual bool dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches = NULL)
    {
        if(m_bTouched) return true;
        m_pTouch = pTouch;
        m_pTouchPoint = pTouch->getLocation();
        m_bTouched= BaseLayer::dTouchBegan(pTouch,pEvent,bSwallowsTouches);
        return m_bTouched;
    }
    
    virtual void dTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
    {
        if(m_pTouch!=pTouch) return;
        if(ccpFuzzyEqual(m_pTouchPoint, cocos2d::CCPointZero,0.0f))
        {
            BaseLayer::dTouchMoved(pTouch,pEvent);
            return;
        }
        
        cocos2d::CCPoint p = pTouch->getLocation();
        if(ccpFuzzyEqual(m_pTouchPoint, p,FIX_MOVE_DIS))
        {
            return;
        }
        m_pTouchPoint = cocos2d::CCPointZero;
        BaseLayer::dTouchMoved(pTouch,pEvent);
    }
    
    virtual void dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
    {
        if(m_pTouch!=pTouch) return;
        m_pTouch = NULL;
        m_bTouched = false;
        BaseLayer::dTouchEnded(pTouch, pEvent);
    }
    
    virtual void dTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
    {
        if(m_pTouch!=pTouch) return;
        m_pTouch = NULL;
        m_bTouched = false;
        BaseLayer::dTouchCancelled(pTouch, pEvent);
    }
private:
    cocos2d::CCPoint m_pTouchPoint;
    bool    m_bTouched;
    cocos2d::CCTouch *m_pTouch;
};



//没有子节点的DLayer默认不接收Touch事件
class DLayer : public BaseLayer
{
protected:
    bool m_bDTouchEnabled;
    
public:
    CREATE_FUNC( DLayer );
    
    DLayer():m_bDTouchEnabled(false)
    {
        
    }
    
    void setDTouchEnabled(bool enabled)
    {
        m_bDTouchEnabled = enabled;
    }
    
    bool isDTouchEnabled()
    {
        return m_bDTouchEnabled;
    }
    
protected:
    D_INIT_DEFAULT( BaseLayer );
    //static

};


//模态层 阻住所有不是该层子节点的层的触摸事件（无视zorder容易产生冲突）  !!!普通模态层请使用cclayer放在最高层级模拟
class DLayerModal : public BaseLayer
{
protected:
    bool virtual init()
    {
        if(!BaseLayer::init())
        {
            return false;
        }
        
        BaseLayer::setTouchEnabled(true);
        return true;
    }
    
    virtual void setTouchEnabled(bool enabled)
    {
        
    }
    
    D_TOUCH_REGISTER_DEFAULT( cocos2d::kCCMenuHandlerPriority );
    //static
public:
    CREATE_FUNC( DLayerModal );
    
    virtual bool dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches = NULL)
    {
        m_pTouchPoint = pTouch->getLocation();
        BaseLayer::dTouchBegan(pTouch,pEvent,bSwallowsTouches);
        return isVisible();
    }
    
    virtual void dTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
    {
        if(ccpFuzzyEqual(m_pTouchPoint, cocos2d::CCPointZero,0.0f))
        {
            BaseLayer::dTouchMoved(pTouch,pEvent);
            return;
        }
        
        cocos2d::CCPoint p = pTouch->getLocation();
        if(ccpFuzzyEqual(m_pTouchPoint, p,FIX_MOVE_DIS))
        {
            return;
        }
        m_pTouchPoint = cocos2d::CCPointZero;
        BaseLayer::dTouchMoved(pTouch,pEvent);
    }
private:
    cocos2d::CCPoint m_pTouchPoint;
    //data
};




#endif /* defined(__QSMY__DLayer__) */
