#include "DSwitchButton.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const float ACTION_DURATION = 0.2f;

DSwitchButton::DSwitchButton()
	: m_pContainer(NULL)
	,m_pMaskSprite(NULL)
	,m_pOnThumbSprite(NULL)
	,m_pOffThumbSprite(NULL)
	,m_pOnLabel(NULL)
	,m_pOffLabel(NULL)
	,m_bMoved(false)
	, m_bOn(false)
	,m_onPosition(ccp(0, 0))
	,m_offPosition(ccp(0, 0))
	,m_frameBorderWidth(5.0f)
{

}

DSwitchButton::~DSwitchButton()
{

}

bool DSwitchButton::initWithMaskSprite( CCSprite *maskSprite, CCSprite * onThumbSprite, CCSprite * offThumbSprite )
{
	return initWithMaskSprite(maskSprite, onThumbSprite, offThumbSprite, NULL, NULL);
}

bool DSwitchButton::initWithMaskSprite( CCSprite *maskSprite, CCSprite * onThumbSprite, CCSprite * offThumbSprite, CCLabelTTF* onLabel, CCLabelTTF* offLabel )
{
	if (maskSprite == NULL || onThumbSprite == NULL)
	{
		return false;
	}

	CCControl::init();

	m_pMaskSprite = maskSprite;
	m_pOnThumbSprite = onThumbSprite;
	m_pOffThumbSprite = offThumbSprite;
	m_pOnLabel = onLabel;
	m_pOffLabel = offLabel;

	m_bOn = true;
	setTouchEnabled(true);

	CCNode* m_pContainer = CCNodeRGBA::create();
	m_pContainer->setContentSize(maskSprite->getContentSize());
	m_pContainer->addChild(m_pMaskSprite);

	m_onPosition = ccp(m_pMaskSprite->getPositionX() - 0.25 * m_pMaskSprite->getContentSize().width + m_frameBorderWidth,
		m_pMaskSprite->getPositionY() + m_frameBorderWidth);
	m_offPosition = ccp(m_pMaskSprite->getPositionX() + 0.25 * m_pMaskSprite->getContentSize().width - m_frameBorderWidth,
		m_pMaskSprite->getPositionY() + m_frameBorderWidth);

	if (m_pOnLabel)
	{
		m_pContainer->addChild(m_pOnLabel);
	}

	if (m_pOffLabel)
	{
		m_pContainer->addChild(m_pOffLabel);
	}

	m_pContainer->addChild(m_pOnThumbSprite);
	m_pOnThumbSprite->setPosition(m_onPosition);

	if (m_pOffThumbSprite)
	{
		m_pContainer->addChild(m_pOffThumbSprite);
		m_pOffThumbSprite->setPosition(m_onPosition);
		m_pOffThumbSprite->setOpacity(0);
	}

	m_pContainer->setPosition(maskSprite->getContentSize().width * 0.5f, maskSprite->getContentSize().height * 0.5f);
	this->addChild(m_pContainer);
	
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(ccp (0.5f, 0.5f));
	this->setContentSize(m_pMaskSprite->getContentSize());
	return true;
}

DSwitchButton* DSwitchButton::create( CCSprite *maskSprite, CCSprite * onThumbSprite, CCSprite * offThumbSprite )
{
	return DSwitchButton::create(maskSprite, onThumbSprite, offThumbSprite, NULL, NULL);
}

DSwitchButton* DSwitchButton::create( CCSprite *maskSprite, CCSprite * onThumbSprite, CCSprite * offThumbSprite, CCLabelTTF* onLabel, CCLabelTTF* offLabel )
{
	DSwitchButton* pRet = new DSwitchButton();
	if (pRet && pRet->initWithMaskSprite(maskSprite, onThumbSprite, offThumbSprite, onLabel, offLabel))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

void DSwitchButton::setOn( bool isOn )
{
	setOn(isOn, false);
}

void DSwitchButton::setOn( bool isOn, bool animated )
{
	m_bOn = isOn;

	m_pOnThumbSprite->stopAllActions();
	if (m_pOffThumbSprite)
	{
		m_pOffThumbSprite->stopAllActions();
	}

	CCPoint destination = m_bOn ? m_offPosition : m_onPosition;
	if (animated)
	{
		m_pOnThumbSprite->runAction(CCMoveTo::create(ACTION_DURATION, destination));
		if (m_pOffThumbSprite)
		{
			m_pOffThumbSprite->runAction(CCMoveTo::create(ACTION_DURATION, destination));
			CCActionInterval* fadeOut = CCFadeTo::create(ACTION_DURATION, 0);
			CCActionInterval* fadeIn = CCFadeTo::create(ACTION_DURATION, 255);

			m_pOnThumbSprite->runAction(m_bOn ? fadeIn : fadeOut);
			m_pOffThumbSprite->runAction(m_bOn ? fadeOut : fadeIn);
		}
	}
	else
	{
		m_pOnThumbSprite->setPosition(destination);
		if (m_pOffThumbSprite)
		{
			if (m_bOn == false)
			{
				m_pOnThumbSprite->setOpacity(0);
				m_pOffThumbSprite->setOpacity(255);
			}
			m_pOffThumbSprite->setPosition(destination);
		}
	}

	 sendActionsForControlEvents(CCControlEventValueChanged);
}

void DSwitchButton::setEnabled( bool enabled )
{
	m_bEnabled = enabled;
	if (m_pContainer)
	{
		m_pContainer->setOpacity((enabled) ? 255 : 128);
	}
}

cocos2d::CCPoint DSwitchButton::locationFromTouch( CCTouch* touch )
{
	CCPoint touchLocation   = touch->getLocation();                      // Get the touch position
	touchLocation           = this->convertToNodeSpace(touchLocation);                  // Convert to the node space of this class

	return touchLocation;
}

bool DSwitchButton::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if (!isTouchInside(pTouch) || !isEnabled() || !isVisible())
	{
		return false;
	}

	m_bMoved = false;
	
	if (m_pOffThumbSprite)
	{
		m_pOffThumbSprite->setColor(ccGRAY);
	}
	m_pOnThumbSprite->setColor(ccGRAY);

	return true;
}

void DSwitchButton::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	 CCPoint location    = this->locationFromTouch(pTouch);
	 m_bMoved = true;
	 //todo move 
}

void DSwitchButton::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	 CCPoint location   = this->locationFromTouch(pTouch);
	 m_pOnThumbSprite->setColor(ccWHITE);
	 if (m_pOffThumbSprite)
	 {
		 m_pOffThumbSprite->setColor(ccWHITE);
	 }

	 if (m_bMoved)
	 {
		 setOn(!(location.x < m_pMaskSprite->getContentSize().width / 2), true);
	 } 
	 else
	 {
		 setOn(!m_bOn, true);
	 }
}

void DSwitchButton::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	CCPoint location   = this->locationFromTouch(pTouch);
	m_pOnThumbSprite->setColor(ccWHITE);
	if (m_pOffThumbSprite)
	{
		m_pOffThumbSprite->setColor(ccWHITE);
	}

	if (m_bMoved)
	{
		setOn(!(location.x < m_pMaskSprite->getContentSize().width / 2), true);
	} 
	else
	{
		setOn(!m_bOn, true);
	}
}
