//
//  kingTombRule.h
//   规则
//
//
//  Created by jk on 14-8-13.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#ifndef __LibQSMY__KingTombRule__
#define __LibQSMY__KingTombRule__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "model/KingTombModel.h"
#include "managers/GameManager.h"
#include "KingTombfight.h"
#include "../common/gui/AwardItemSmall.h"


class KingTombRule;

using namespace cocos2d;

class KingTombRule
: public DPopup
, public cocos2d::extension::CCBMemberVariableAssigner
//, public cocos2d::extension::CCBSelectorResolver
{
public:
	KingTombRule();
	~KingTombRule();
	CREATE_FUNC( KingTombRule ) ;
	virtual bool init() ;
public:
    
    virtual bool onAssignCCBMemberVariable( cocos2d::CCObject *pTarget,
                                           const char *pMemberVariableName, cocos2d::CCNode *pNode ) ;
	
    

    
    //-----------CCBSelectorResolver------------------
//    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
//    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {return NULL;};
    
private:
    void closeCallBack(cocos2d::CCObject *sender);

    DButton *m_pCloseBtn;
    
};


#endif /* defined(__LibQSMY__KingTombRule__) */
