//
//  kingTombMine.h
//  LibQSMY
//  矿坑 
//  Created by jk on 14-8-4.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#ifndef __LibQSMY__kingTombMine__
#define __LibQSMY__kingTombMine__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"

using namespace cocos2d;

class KingTombMine
: public DLayer
{
public:
    KingTombMine();
    ~KingTombMine();    
    virtual void onEnter();
	virtual void onExit() ;   
    CREATE_FUNC( KingTombMine ) ;
	virtual bool init() ;
public:
    void setData(int type);
	//方便外层可以获得该元素是否可见
	CCNode*				m_pNodeUnopen;
private:    
	sp::Armature*		m_pBoxOpen;
	CCSprite*			m_pBoxLost;
	CCSprite*			m_pBoxOld;
	CCNode*				m_pBoxContainer;
};
#endif /* defined(__LibQSMY__kingTombMine__) */
