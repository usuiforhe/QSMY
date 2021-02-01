//
//  PLWheelView.h
//  QSMY
//
//  Created by wanghejun on 13-5-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PLWheelView__
#define __QSMY__PLWheelView__

#include "AppInclude.h"

#include "DSelectorView.h"

class PLWheelView;

class PLWheelViewDelegate
{
public:
	virtual void PLWheelViewTouched(PLWheelView *pView) {};
	virtual void PLWheelViewCenterIdxChanged(PLWheelView *pView, unsigned int centerIdx) = 0;
	virtual void PLWheelViewStopWheeling(PLWheelView *pView, unsigned int centerIdx) = 0;
	virtual void PLWheelViewStartWheeling(PLWheelView *pView, unsigned int centerIdx) = 0;
	virtual void PLWheelViewBouncePercent(PLWheelView *pView, float pct) = 0;
};


class PLWheelView:public DSelectorView {



	CC_SYNTHESIZE_READONLY(unsigned int, m_uCenterIdx, CenterIdx);
	CC_SYNTHESIZE(PLWheelViewDelegate *, m_pWheelDelegate, WheelDelegate);
	
public:
	PLWheelView()
//    :m_iStartIdx(0)
	:m_uCenterIdx(0)
	,m_fCenterRy(0.f)
	,m_pWheelDelegate(NULL)
	,m_nSoundID(0)
    {};
    
    virtual ~PLWheelView();
    
    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     */
    static PLWheelView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size);
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     */
    static PLWheelView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size, CCNode *container);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool initWithViewSize(cocos2d::CCSize size, CCNode* container = NULL);
    
/*    void reloadData();
    
    //update scrollview's contentoffset
    void updateContentOffset();
    
	void updateCellPositions();

	virtual void setContentSize(const cocos2d::CCSize & size);*/
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
protected:
    
 /*   virtual void setDirection(cocos2d::extension::CCScrollViewDirection eDirection) {cocos2d::extension::CCScrollView::setDirection(eDirection);}
    
    virtual void setVerticalFillOrder(cocos2d::extension::CCTableViewVerticalFillOrder order) { cocos2d::CCLog("not support"); }
    
    void updateCellAtIndex(unsigned int idx);*/
    
	void _setIndexForCell(unsigned int index, cocos2d::extension::CCTableViewCell *cell);

	void _updateCellPositions();
    
protected:
//    unsigned int m_iStartIdx;

//	unsigned int m_uCenterIdx;
	
	float m_fCenterRy;
private:
	void moveToCenter(float dt);
	unsigned int m_nSoundID;
};


#endif /* defined(__QSMY__PLWheelView__) */
