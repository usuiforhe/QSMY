#ifndef __QSMY__BFWebView_h
#define __QSMY__BFWebView_h

#include "cocos2d.h"

class CCWebView : public cocos2d::CCObject
{
public:
    CCWebView();
    ~CCWebView();
    
    CREATE_FUNC(CCWebView);
	bool init();
    
    /**
     *	@brief	显示WebView
     *
     *	@param 	url 	地址
     *	@param 	x 	x位置
     *	@param 	y 	y位置(左上的位置，坐标系为左上0)
     *	@param 	width 	宽度
     *	@param 	height 	高度
     */
    void showWebView(const char* url, float x, float y, float width, float height);

    void showWebView(const char* url, cocos2d::CCNode* container);
    
    void updateURL(const char* url);
        
    void removeWebView();
    
private:

};

#endif