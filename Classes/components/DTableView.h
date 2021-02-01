//
//  DTableView.h
//  QSMY
//
//  Created by wanghejun on 13-4-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DTableView__
#define __QSMY__DTableView__

#include "AppInclude.h"


class DTableView:public cocos2d::extension::CCTableView {

	
public:
	DTableView(){};
    virtual ~DTableView(){};
    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     */
    static DTableView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size);
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     */
    static DTableView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size, CCNode *container);
        
    //virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void reloadData();
    
    void releaseCellsFreed();
    
    //update scrollview's contentoffset
    void updateContentOffset();

	void updateCellPositions(cocos2d::extension::CCTableViewCell* cell);
    
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    //virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void registerWithTouchDispatcher(){};
};


#endif /* defined(__QSMY__DTableView__) */
