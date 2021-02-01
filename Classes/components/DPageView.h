//
//  DPageView.h
//  横向单页滚动
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DPageView__
#define __QSMY__DPageView__

#include "AppInclude.h"


class DPageView;


enum ScrollDirection
{
    kScrollDirectionNone,
    kScrollDirectionLeft,
    kScrollDirectionRight
};

class DPageViewDataSource
{
public:
    
    virtual ~DPageViewDataSource() {}
    /**
     * cell height for a given table.
     *
     * @param table table to hold the instances of Class
     * @return cell size
     */
    virtual float cellSizeForTable(DPageView *view) = 0;
    /**
     * a cell instance at a given index
     *
     * @param idx index to search for a cell
     * @return cell found at idx
     */
    virtual cocos2d::CCNode* cellAtIndex(DPageView *view, unsigned int idx) = 0;
    /**
     * Returns number of cells in a given table view.
     *
     * @return number of cells
     */
    virtual uint32_t numberOfCellsInTableView(DPageView *view) = 0;
    
};


class DPageView :public cocos2d::extension::CCScrollView{
    
    CC_SYNTHESIZE_READONLY(int32_t, m_iCurrentPage, CurrentPage);
    CC_SYNTHESIZE_READONLY(bool, m_bAutoMove, AutoMove);
    CC_SYNTHESIZE_READONLY(bool, m_bFade, Fade);
    CC_SYNTHESIZE(float, m_fMoveInterval, MoveInterval);
    
public:
    DPageView();
    virtual ~DPageView();
    
    
    /**
     * Returns an autoreleased scroll view object.
     *
     * @param size view size
     * @param container parent object
     * @return autoreleased scroll view object
     */
    static DPageView* create(DPageViewDataSource* dataSource,cocos2d::CCSize size,CCNode* container = NULL);
    
    virtual void setCurrentPage(int32_t val,bool animated = false);
    
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void reloadData();
    
    virtual void setAutoMove(bool val);
    virtual void setFade(bool val);
    
protected:
    cocos2d::CCPoint m_touchPoint;
    DPageViewDataSource *m_pDataSource;
    ScrollDirection scrollDirection;
    ScrollDirection oldScrollDirection;
protected:
    /**
     * Init this object with a given size to clip its content.
     *
     * @param size view size
     * @return initialized scroll view object
     */
    virtual bool init(DPageViewDataSource* dataSource,cocos2d::CCSize size,cocos2d::CCNode* container = NULL);
    virtual void adjustScrollView(float offset);
    
    virtual void preparePages();
    virtual void autoMove();
    
    virtual void nextPage();
    virtual void fixCurrentPage();
    virtual void fixAlpha();
    virtual void fixAlpha(float dt);
};


#endif /* defined(__QSMY__DPageView__) */
