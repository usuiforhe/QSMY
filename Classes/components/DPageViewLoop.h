//
//  DPageViewLoop.h
//  横向单页循环滚动
//
//  Created by wanghejun on 13-5-17.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DPageViewLoop__
#define __QSMY__DPageViewLoop__

#include "AppInclude.h"
#include "DPageView.h"


class DPageViewLoop;


class DPageViewLoop :public DPageView
{
    
public:
    DPageViewLoop();
    virtual ~DPageViewLoop();
    
    
    /**
     * Returns an autoreleased scroll view object.
     *
     * @param size view size
     * @param container parent object
     * @return autoreleased scroll view object
     */
    static DPageViewLoop* create(DPageViewDataSource* dataSource,cocos2d::CCSize size,CCNode* container = NULL);
    
    void setCurrentPage(int32_t val,bool animated = false);

    
protected:
    
    virtual void preparePages();
    virtual void fixCurrentPage();
};


#endif /* defined(__QSMY__DPageViewLoop__) */
