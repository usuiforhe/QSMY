//
//  DScrollLabel.h
//  文字滚动
//
//  Created by wanghejun on 13-3-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DScrollLabel__
#define __QSMY__DScrollLabel__

#include "AppInclude.h"
#include "DLayer.h"


//the anchor point of DScrollLabel is the left-bottom corner.
class DScrollLabel : public DLayer, public cocos2d::extension::CCScrollViewDelegate
{
public://static functions
	static DScrollLabel* create( const char* pszText, const char *fontName, float fontSize, cocos2d::CCTextAlignment hAlignment, cocos2d::CCSize ScrollViewSize );
	
public:
	DScrollLabel();
	~DScrollLabel();
	bool init( const char* pszText, const char *fontName, float fontSize, cocos2d::CCTextAlignment hAlignment, cocos2d::CCSize& ScrollViewSize );
	
	void setTextColor(const cocos2d::ccColor3B& color3)
	{ if(m_pLabel) m_pLabel->setColor( color3 ); }
    
    void setText(const char*pszText);
	
public://scroll view delegate
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){}
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){}
	
private:
	cocos2d::extension::CCScrollView* m_pScrollView;
	cocos2d::CCLabelTTF* m_pLabel;
};


#endif /* defined(__QSMY__DScrollLabel__) */
