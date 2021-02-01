//
//  kingTombRule.cpp
//  LibQSMY
//
//  Created by jk on 14-8-13.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#include "KingTombRule.h"


KingTombRule::KingTombRule():m_pCloseBtn(NULL){

}

KingTombRule::~KingTombRule(){
    CC_SAFE_RELEASE(m_pCloseBtn);
}

bool KingTombRule::init()
{
	if (!DPopup::init())
	{
		return false;
	}
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_tanxian_guize") ;
    
    m_pCloseBtn->getButton()->setTarget(this, menu_selector(KingTombRule::closeCallBack));
	return true;
}

bool KingTombRule::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseBtn);
    
	return false;
}

//关闭按钮回调
void KingTombRule::closeCallBack(cocos2d::CCObject *sender){
    this->closePopup();
}
