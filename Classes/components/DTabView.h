//
//  DTabView.h
//  Tab切换
//
//  Created by wanghejun on 13-3-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DTabView__
#define __QSMY__DTabView__

#include "AppInclude.h"
#include "DLayer.h"
#include "DTableView.h"

class DTabView;

class DTabViewDelegate
{
public:
	virtual void SelectTabAtIndex(DTabView *tabView, uint32_t idx) = 0;
};

class DTabView: public DLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate 
{
    
    CC_SYNTHESIZE(cocos2d::CCSize,m_tabSize,TabSize);  //Tab按钮尺寸
    
    CC_SYNTHESIZE_READONLY(DTableView*, m_pTabMenuScrollView, TabMenuScrollView);//Tab 滚动标签

	CC_SYNTHESIZE(DTabViewDelegate *, m_pDelegate, Delegate);
    
public:
    DTabView();
    virtual ~DTabView();
    
    
    virtual void onEnter(); 
    CREATE_FUNC(DTabView);
    
    static DTabView* create(cocos2d::CCRect rect);
    static DTabView* create(cocos2d::CCRect rect, float menuBoderMargin);
	static DTabView* create(cocos2d::CCRect rect, float menuBoderMargin, bool alignment);
    static DTabView* create(cocos2d::CCRect rect,
                            cocos2d::ccColor3B titleFontColor,
                            const char* titleFontName,
                            float titleFontSize,
                            float padding = 0.0,
                            float menuBoderMargin = 2.0,
                            int selectedTab = 0,
							bool alignment = false,				//tab在top or bottom
							bool clippingToBounds = true );		//裁切
    virtual bool init();

    void addTabForTest(CCNode * tabView); //for test;
    void addTabWithName(CCNode * tabView,const char *tabName);
	void addTabWithFontImg(CCNode * tabView,const char *fontImgName);
    void addTabWithImage(CCNode *tabView,const char *normalImage, const char *selectedImage = NULL, const char *disableImage = NULL);
    void addTab(CCNode *tabView,const char *tabName,const char *normalImage, const char *selectedImage = NULL, const char *disableImage = NULL, const char *tabFontImage = NULL);

	/*获得当前选中tab*/
	CCObject * getSelectedTab() const
	{
		return m_pTabViewArray->objectAtIndex(m_selectedTabIdx);
	}
	/*设置enable属性*/
	void setTabEnable(bool value, int idx);

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    void selectedTab(int index);
    
    //临时调整右对齐方法
    //FIXME: 调整构造方法，增加右对齐
    void setTopMenuisLeftAlignment(bool value);
    void setPadding(float f){
		m_padding = f;
	}
protected:
    
    virtual bool initWithRect(cocos2d::CCRect rect);
	virtual bool initWithRectAndMargin(cocos2d::CCRect rect, float menuBoderMargin);
    virtual bool initWithRectAndMarginAndAlign(cocos2d::CCRect rect,  float menuBoderMargin, bool alignment);
    virtual bool initWithAll(cocos2d::CCRect rect,
                                     cocos2d::ccColor3B titleFontColor,
                                     const char* titleFontName,
                                     float titleFontSize,
                                     float padding,
                                     float menuBoderMargin,
                                     int selectedTab,
									 bool alignment
									 );

    
    
    

	void reloadTabData();    
    void updateLeftAlignment();

private:

    DLayer* m_pTabView;
    
	cocos2d::CCArray* m_pTabNameArray;
    cocos2d::CCArray* m_pTabViewArray;
    
    cocos2d::ccColor3B m_titleFontColor;
    std::string m_titleFontName;
    float m_titleFontSize;
    float m_padding;
    float m_menuBoderMargin;
    int m_selectedTabIdx;
    int m_tabNum;
	bool m_alignment;
    bool m_bIsLeftAlignment;
    cocos2d::CCSize m_contentSize;
};



#endif /* defined(__QSMY__DTabView__) */
