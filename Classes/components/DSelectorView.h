//
//  DSelectorView.h
//  QSMY
//
//  Created by wanghejun on 13-5-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DSelectorView__
#define __QSMY__DSelectorView__

#include "AppInclude.h"

class DSelectorView:public cocos2d::extension::CCTableView {
    
	
public:
	DSelectorView()
    :m_iStartIdx(0)
    {};
    
    virtual ~DSelectorView(){}
    
    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     */
    static DSelectorView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size);
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     */
    static DSelectorView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size, CCNode *container);
    
    //virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool initWithViewSize(cocos2d::CCSize size, CCNode* container = NULL);
    
    void reloadData();
    
    //update scrollview's contentoffset
    void updateContentOffset();
    
	void updateCellPositions();
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    
    virtual void setContentSize(const cocos2d::CCSize & size);
    
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
protected:
    
    virtual void setDirection(cocos2d::extension::CCScrollViewDirection eDirection) {cocos2d::extension::CCScrollView::setDirection(eDirection);}
    
    virtual void setVerticalFillOrder(cocos2d::extension::CCTableViewVerticalFillOrder order) { cocos2d::CCLog("not support"); }
    
    /**
     * Updates the content of the cell at a given index.
     *
     * @param idx index to find a cell
     */
    
    virtual void updateCellAtIndex(unsigned int idx);
    
    virtual void _setIndexForCell(unsigned int index, cocos2d::extension::CCTableViewCell *cell);
    
	virtual void _updateCellPositions();

protected:
    unsigned int m_iStartIdx;
};


#endif /* defined(__QSMY__DSelectorView__) */
