//
//  PvpBestEver.h
//  Ê×³äµ¯¿ò
//
//  Created by fengkerong on 14-7-9.
//  Copyright (c) 2013Äê thedream. All rights reserved.
//

#ifndef __QSMY__PVP_BESTEVER__
#define __QSMY__PVP_BESTEVER__

#include "AppInclude.h"
#include "../common/common.h"

class PvpBestEver
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	PvpBestEver();
	virtual ~PvpBestEver();

	CREATE_FUNC(PvpBestEver);

	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	
protected:

	void closeCallBack(cocos2d::CCObject *obj);

private:

	DButton		*m_pCloseButton;
	DButton		*m_pConfirmButton;
	cocos2d::CCLabelBMFont		*m_pHigh;
	cocos2d::CCLabelBMFont		*m_pNow;
	cocos2d::CCLabelBMFont		*m_pGrow;
	cocos2d::CCLabelBMFont		*m_pAward;
	cocos2d::CCNode					*m_pNode123;
	cocos2d::CCNode					*m_pFirst;
	cocos2d::CCNode					*m_pSecond;
	cocos2d::CCNode					*m_pThird;
};

#endif /* defined(__QSMY__PVP_BESTEVER__) */
