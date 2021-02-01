//
//  DSlideView.h
//  Slide切换
//
//  Created by cll on 13-4-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DSlideView__
#define __QSMY__DSlideView__

#include "AppInclude.h"
#include "DTableView.h"
#include "DLayer.h"

class DSlideViewCell:public cocos2d::extension::CCTableViewCell
{
    CC_SYNTHESIZE_READONLY(unsigned int, m_id, Id);
public:
    DSlideViewCell(unsigned int id):cocos2d::extension::CCTableViewCell(), m_id(id){
        
    }
    
    virtual ~DSlideViewCell(){
        removeAllChildren();
    }
};


class DSlideView: public DLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
    CC_SYNTHESIZE_READONLY(DTableView *, m_pTabView, TabView);
    CC_SYNTHESIZE_READONLY(DTableView *, m_pContentView, ContentView);
	CC_SYNTHESIZE(unsigned short,m_selectedSlideIdx, SelectedSlideIdx);
public:
    DSlideView();
    virtual ~DSlideView();
    
	CREATE_FUNC(DSlideView);
    
    virtual void onEnter();
    
    static DSlideView* create(cocos2d::CCRect contentRect);
    static DSlideView* create(cocos2d::CCRect tabRect, cocos2d::CCRect contentRect);
    static DSlideView* create(cocos2d::CCRect tabRect, cocos2d::CCRect contentRect, unsigned short selectedSlide);
    
    virtual bool init();
    
    /**
     *	@brief	删除内容
     *
     *	@param 	slideIdx  cell 编号
     */
    void removeSlideByIdx(unsigned short cellIdx);


    /**
     *	@brief	增加内容
     *
     *	@param 	contentView 	<#contentView description#>
     *	@param 	normalImage 	<#normalImage description#>
     *	@param 	NULL 	<#NULL description#>
     *	@param 	NULL 	<#NULL description#>
     *
     *	@return	cell 编号
     */
    unsigned int addSlideWithTab(CCNode *contentView,const char *normalImage, const char *selectedImage = NULL, const char *disableImage = NULL);

    /**
     *	@brief	获取内容总数
     *
     *	@return	数量
     */
    unsigned int getSlideCount();


	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
	virtual bool dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches);
	//virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void dTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    

    void selectedTab(unsigned short index,bool animate=false);
    
    
	void selectedTabById(unsigned short Id,bool animate=false);
	unsigned int addSlideWithTabView(CCNode *contentView,cocos2d::CCMenu *tabView);
    //设置是否屏蔽滑动
	void setTouchMovedEnabled(bool val);
protected:
    
    
	virtual bool initWithRect(cocos2d::CCRect tabRect);
	virtual bool initWithRects(cocos2d::CCRect tabRect, cocos2d::CCRect contentRect);
	virtual bool initWithRectsAndIdx(cocos2d::CCRect tabRect, cocos2d::CCRect contentRect, unsigned short selectedSlideIdx);
    
	void adjustSlide(float offset);
    
    void reloadSlideData(float adjustTabOffset = 0.f,bool animate=false);

private:
  
	cocos2d::CCArray* m_pTabViewArray;
    cocos2d::CCArray* m_pContentViewArray;
    //int m_selectedSlideIdx;
	uint32_t m_slideCount;
	cocos2d::CCRect m_contentRect;
    cocos2d::CCRect m_tabRect;
	cocos2d::CCPoint m_touchPoint;
    bool m_bSlide;//是否拖动
	bool m_bTouchMovedEnabled;//是否屏蔽滑动
};

#endif /* defined(__QSMY__DSlideView__) */
