//
//  DEffectNode.h
//  特效层基类
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DEffectNode__
#define __QSMY__DEffectNode__

#include "DLayer.h"

class DEffectNode:public DLayerModal
{
public:
    DEffectNode();
    virtual ~DEffectNode();
    CREATE_FUNC(DEffectNode);
    D_INIT_DEFAULT(DLayerModal);
    D_TOUCH_REGISTER_DEFAULT( DTOUCH_EFFECT_PRIORITY );
    void setCallBack(cocos2d::CCObject *pTarget,cocos2d::SEL_CallFuncO pCallBack);
    void removeFromParentAndCleanup(bool cleanup);
protected:
    void callback();
protected:
    cocos2d::CCObject   *m_pTarget;
    cocos2d::SEL_CallFuncO     m_pCallBack;
};

class DEffectLayer:public DLayer
{
    
public:
    CREATE_FUNC(DEffectLayer);
    virtual bool init();
    DEffectLayer();
    virtual ~DEffectLayer();
    /**
     *	@brief	增加动画效果
     *
     *	@param 	pNode 	动画节点
     *	@param 	showImm 	是否立即显示 默认为false添加到队列
     */
    virtual void addNode(DEffectNode* pNode,bool showImm = false);

    virtual void update(float dt);
	bool isPlaying() {return getChildrenCount() || deque->count();}
	virtual void removeAllNode();
private:
    virtual void addChild(CCNode *child, int zOrder, int tag);
    cocos2d::CCArray* deque;
};

#endif /* defined(__QSMY__DEffectNode__) */
