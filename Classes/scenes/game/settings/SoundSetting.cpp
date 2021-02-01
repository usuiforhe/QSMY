//
//  SoundSetting.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoundSetting.h"
#include "managers/DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

SoundSetting::SoundSetting()
: m_pSettingModel(NULL)
,m_pMusicButtonContainer(NULL)
,m_pSfxButtonContainer(NULL)
{
    
}

SoundSetting::~SoundSetting()
{
	CC_SAFE_RELEASE(m_pMusicButtonContainer);
	CC_SAFE_RELEASE(m_pSfxButtonContainer);
}

bool SoundSetting::init()
{
    if(!DLayer::init()) return false;
    m_pSettingModel = DM_GET_SETTING_MODEL;
    
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_shezhi/pl_shezhi_yinyue");

	if (m_pMusicButtonContainer)
	{
		DSwitchButton* musicButton = DSwitchButton::create
			(CCSprite::create("PL_ui/shezhi/shezhi_kaiguan_bg.png"),
			CCSprite::create("PL_ui/shezhi/shezhi_kai_btn.png"),
			CCSprite::create("PL_ui/shezhi/shezhi_guan_btn.png")
			);

		m_pMusicButtonContainer->addChild(musicButton);
		musicButton->addTargetWithActionForControlEvents(this, cccontrol_selector(SoundSetting::updateMusicStatus), CCControlEventValueChanged);
		musicButton->setOn(m_pSettingModel->getMusicStatus());
	}

	if (m_pSfxButtonContainer)
	{
		DSwitchButton* sfxButton = DSwitchButton::create
			(CCSprite::create("PL_ui/shezhi/shezhi_kaiguan_bg.png"),
			CCSprite::create("PL_ui/shezhi/shezhi_kai_btn.png"),
			CCSprite::create("PL_ui/shezhi/shezhi_guan_btn.png")
			);

		m_pSfxButtonContainer->addChild(sfxButton);
		sfxButton->addTargetWithActionForControlEvents(this, cccontrol_selector(SoundSetting::updateSfxStatus), CCControlEventValueChanged);
		sfxButton->setOn(m_pSettingModel->getSfxStatus());
	}
	
    return true;
}


bool SoundSetting::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMusicButtonContainer", cocos2d::CCNode*, this->m_pMusicButtonContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSfxButtonContainer", cocos2d::CCNode*, this->m_pSfxButtonContainer);

    return false;
}

SEL_MenuHandler SoundSetting::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

void SoundSetting::onEnter()
{
    DLayer::onEnter();
}

void SoundSetting::onExit()
{
    UNBINDING_ALL(m_pSettingModel, this);
    DLayer::onExit();
}

void SoundSetting::updateMusicStatus(CCObject* sender, CCControlEvent controlEvent)
{
	DSwitchButton* pSwitch = dynamic_cast<DSwitchButton*>(sender);
	if (pSwitch == NULL)
	{
		return;
	}
	
	if (m_pSettingModel->getMusicStatus() == pSwitch->isOn())
	{
		return;
	}
	
	m_pSettingModel->setMusicStatus(pSwitch->isOn());
}

void SoundSetting::updateSfxStatus(CCObject* sender, CCControlEvent controlEvent)
{
	DSwitchButton* pSwitch = dynamic_cast<DSwitchButton*>(sender);
	if (pSwitch == NULL)
	{
		return;
	}

	if (m_pSettingModel->getSfxStatus() == pSwitch->isOn())
	{
		return;
	}
	

	m_pSettingModel->setSfxStatus(pSwitch->isOn());
}

