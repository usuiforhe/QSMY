//
//  DLoadingTableView.h
//  QSMY
//
//  Created by Eci on 13-11-04.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DLoadingTableView__
#define __QSMY__DLoadingTableView__

#include "AppInclude.h"
#include "components/components.h"

class DLoadingTableViewDelegate
{
public:
	virtual void startLoading() = 0;
};


class DLoadingTableView : public DTableView {

	
public:
	DLoadingTableView();
    virtual ~DLoadingTableView();
    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     */
    static DLoadingTableView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size);
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     */
    static DLoadingTableView* create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size, CCNode *container);

	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	void endLoading();

	void setReachTheEnd(bool isReachTheEnd);
	void setLoadingEnabled(bool isLoadingEnabled);
	void callBackRefresh(CCObject *sender);  

protected:

	CC_SYNTHESIZE(DLoadingTableViewDelegate *, m_pLoadingDelegate, LoadingDelegate);
	CC_SYNTHESIZE_READONLY(bool, m_isReachTheEnd, ReachTheEnd);
	CC_SYNTHESIZE_READONLY(bool, m_isLoadingEnabled, LoadingEnabled);

	CC_SYNTHESIZE(uint32_t, m_uPageSize, PageSize);

private:
	DLayer    *m_pEffectLayer;            //播放动画效果层

	uint32_t m_uLastCount;

	CCPoint m_ptLastPoint;

	bool m_bIsLoading;
};


#endif /* defined(__QSMY__DLoadingTableView__) */
