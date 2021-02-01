

#ifndef __PILI__SelectLoginType__
#define __PILI__SelectLoginType__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/GameManager.h"
#include "../LoginInclude.h"


class SelectLoginType
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
public:
	SelectLoginType();
	virtual ~SelectLoginType();
	CREATE_FUNC(SelectLoginType);
	CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);
private:
	void menuCallback(cocos2d::CCObject* obj);
	
	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}



};

#endif /* defined(__PILI__SelectLoginType__) */