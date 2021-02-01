//
//  DPopup.h
//  弹出框
//
//  Created by wanghejun on 13-3-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DPopup__
#define __QSMY__DPopup__

#include "AppInclude.h"
#include "DLayer.h"


#define D_POPUP_PRIORITY( iPriority )          \
virtual int getPriority()                       \
{                                                \
    return iPriority;                             \
}

class DPopupDelegate;

class DPopup : public cocos2d::CCLayer
{
    CC_SYNTHESIZE(DPopupDelegate*, m_PopuppDelegate, PopupDelegate);
	//是否允许backkey 默认允许
    CC_SYNTHESIZE(bool, m_bBackKeyEnabled, BackKeyEnabled);
public:
    CREATE_FUNC(DPopup);
    virtual bool init(bool animate);
    virtual bool init();
    //virtual bool initWithCloseButtonAndBg(cocos2d::extension::CCControlButton * btn,
    //                                      cocos2d::CCLayerColor * bg);
    DPopup();
    virtual ~DPopup();

    bool isOpening();
    virtual void closePopup();
    
    //弹出优先级 不能影响已经弹出的窗口
    virtual int getPriority()
    {
        return 0;
    }
    
    virtual void removeFromParentAndCleanup(bool cleanup);
    
    virtual void onEnter();
    
    virtual void addChild(cocos2d::CCNode *child, int zOrder, int tag);
    virtual void addChild(cocos2d::CCNode *child);
    
    virtual void removeAllChildrenWithCleanup(bool cleanup);
    
    void setOpacity(GLubyte opacity);

	virtual void onBackKey(); //for android back key
    
protected:
    
    virtual void showAnimate();
    virtual void createCloseMenu();
    cocos2d::CCMenuItem * m_pCloseButton;
    void doClosePopup(cocos2d::CCObject* obj);

	cocos2d::CCLayerColor* m_pBg;
private:
    cocos2d::CCNode *m_pContainer;
    bool m_bInited;
    bool m_bAnimate;
};


class DPopupLayer:public DLayer
{
    
public:
    
    CREATE_FUNC(DPopupLayer);
    virtual bool init();
    DPopupLayer();
    virtual ~DPopupLayer();
    
    /**
     *	@brief	打开对话框
     *
     *	@param 	pPopup 	弹窗
     */
    virtual void openPopup(DPopup* pPopup);

    virtual void update(float dt);
    
    virtual void setVisible(bool val);
    
    virtual void onEnter();

	DPopup *getPopingUp();
private:
    
    virtual void addChild(CCNode *child, int zOrder, int tag);
    cocos2d::CCArray* deque;
};



class DPopupDelegate
{
public:
	virtual void onPopupClose(DPopup*) = 0;
	virtual void onPopupOpen(DPopup*){};
	virtual void onPopupBack(DPopup* pPopup)  //for android back key
	{
		pPopup->removeFromParent();
		this->onPopupClose(pPopup);
	}
};

#endif /* defined(__QSMY__DPopup__) */
