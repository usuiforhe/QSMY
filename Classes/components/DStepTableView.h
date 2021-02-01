//
//  DStepTableView.h
//  每次滚动一条tableviw(pageview)
//
//  Created by wanghejun on 13-10-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DStepTableView__
#define __QSMY__DStepTableView__

#include "DTableView.h"


class DStepTableView
:public DTableView
{
   CC_SYNTHESIZE_READONLY(int32_t, m_pCurrentPage, CurrentPage); 
public:
    
    DStepTableView()
		:m_pCurrentPage(0){}
    virtual ~DStepTableView(){}
    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     */
    static DStepTableView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size);
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     */
    static DStepTableView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size, CCNode *container);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
    virtual void fixPosition(cocos2d::CCPoint &p);
    
private:
    
    cocos2d::CCPoint m_touchPoint;
};


#endif /* defined(__QSMY__DStepTableView__) */
