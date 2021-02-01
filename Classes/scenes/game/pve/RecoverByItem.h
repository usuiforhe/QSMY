#ifndef __RECOVER_BY_ITEM__
#define __RECOVER_BY_ITEM__

#include "AppInclude.h"
#include "../common/common.h"


class PveDelegate;
class ItemInfo;
class RecoverByItem
	:public cocos2d::CCLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(PveDelegate*,m_Delegate,Delegate);
public:
	RecoverByItem();
	virtual ~RecoverByItem();
	CREATE_FUNC(RecoverByItem);

	virtual bool init();
	virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);

	virtual void onEnter();
	virtual void onExit();
private:
	uint32_t         m_nCount;
private:
	cocos2d::CCLabelTTF       *m_pTodayTotal0;
	cocos2d::CCLabelBMFont    *m_pCount;

	DButton                     *m_pCloseButton;
	DButton                     *m_pCannelButton;
	DButton                     *m_pSubmitButton;
	DButton                     *m_pSubButton;
	DButton                     *m_pAddButton;

	cocos2d::CCMenuItem         *m_pSubMinButton;
	DButton                     *m_pAddMaxButton;

private:
	void closeCallBack(cocos2d::CCObject* spend = NULL);

	void addCallBack(cocos2d::CCObject*);
	void subCallBack(cocos2d::CCObject*);

	void addMaxCallBack(cocos2d::CCObject*);
	void subMinCallBack(cocos2d::CCObject*);

	void submitCallBack(cocos2d::CCObject*);

	void updateView();
	void updateInfo();
	void buyHandler();

private:
	item::ItemInfo             *m_pPower;
};
#endif //__RECOVER_BY_ITEM__