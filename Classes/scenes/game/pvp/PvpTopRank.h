//
//  PvpTopRankView.h
//  论剑top10列表
//
//  Created by cll on 13-6-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PvpTopRankView__
#define __QSMY__PvpTopRankView__

#include "AppInclude.h"
#include "components/components.h"
#include "model/PvpModel.h"

struct PvpDelegate;
class PvpTopRankCell;


class PvpTopRankView :public DLayer,  public cocos2d::extension::CCTableViewDataSource,public cocos2d::extension::CCTableViewDelegate
{

    
public:
	PvpTopRankView();

	~PvpTopRankView();
    
	CREATE_FUNC(PvpTopRankView);

    virtual bool init();

	virtual void onEnter();
	virtual void onExit();
    
	void setDelegate(PvpDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
private:

	void updateView();

private:
    
    DTableView *m_pListView;
	PvpDelegate *m_pDelegate;
    
    cocos2d::extension::CCData * m_pData;
    cocos2d::extension::CCData * m_pData0;
    cocos2d::CCDictionary      * m_pRanks;
    
};


#endif /* defined(__QSMY__PvpTopRankView__) */
