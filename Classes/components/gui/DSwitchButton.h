
#ifndef __PL_DSwitchButton_H__
#define __PL_DSwitchButton_H__

#include "AppInclude.h"

class DSwitchButton : public cocos2d::extension::CCControl
{
public:
    DSwitchButton();
    virtual ~DSwitchButton();
   
    bool initWithMaskSprite(cocos2d::CCSprite *maskSprite, cocos2d::CCSprite * onThumbSprite, cocos2d::CCSprite * offThumbSprite);

    static DSwitchButton* create(cocos2d::CCSprite *maskSprite, cocos2d::CCSprite * onThumbSprite, cocos2d::CCSprite * offThumbSprite);

    bool initWithMaskSprite(cocos2d::CCSprite *maskSprite, cocos2d::CCSprite * onThumbSprite, cocos2d::CCSprite * offThumbSprite, cocos2d::CCLabelTTF* onLabel, cocos2d::CCLabelTTF* offLabel);

    static DSwitchButton* create(cocos2d::CCSprite *maskSprite, cocos2d::CCSprite * onThumbSprite, cocos2d::CCSprite * offThumbSprite, cocos2d::CCLabelTTF* onLabel, cocos2d::CCLabelTTF* offLabel);

    void setOn(bool isOn, bool animated);
    void setOn(bool isOn);
    bool isOn(void) { return m_bOn; }
    bool hasMoved() { return m_bMoved; }
    virtual void setEnabled(bool enabled);

    cocos2d::CCPoint locationFromTouch(cocos2d::CCTouch* touch);
    //events
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	CC_SYNTHESIZE(float, m_frameBorderWidth, FrameBorder);
protected:
	cocos2d::CCNodeRGBA*	m_pContainer;
	cocos2d::CCPoint		m_onPosition;
	cocos2d::CCPoint		m_offPosition;
	cocos2d::CCSprite*	m_pMaskSprite;
	cocos2d::CCSprite*	m_pOnSprite;
	cocos2d::CCSprite*	m_pOffSprite;
	cocos2d::CCSprite*	m_pOnThumbSprite;
	cocos2d::CCSprite*	m_pOffThumbSprite;
	cocos2d::CCLabelTTF*		m_pOnLabel;
	cocos2d::CCLabelTTF*		m_pOffLabel;
   
    bool m_bMoved;
    bool m_bOn;
};

#endif
