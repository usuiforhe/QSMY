
#ifndef __PILI__LoginView__
#define __PILI__LoginView__

#include "AppInclude.h"
#include "components/components.h"
#include "../LoginInclude.h"

#define EDIT_BOX_COUNT_2 2
#define EDIT_BOX_COUNT_3 3
#define EDIT_BOX_COUNT_4 4
#define EDIT_BOX_MAX_LEN 15
#define PLACE_HOLDER_COLOR ccc3(117, 117, 117)


class LoginView
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public cocos2d::extension::CCEditBoxDelegate
{
public:
	LoginView();
	virtual ~LoginView();
	CREATE_FUNC(LoginView);
	CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);

private:
	void loginCallBack(cocos2d::CCObject*);
	void registerCallBack(cocos2d::CCObject*);
	void closePopup(cocos2d::CCObject*);
	void onEnter();
	
	void menucallback(cocos2d::CCObject*);
	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	cocos2d::CCLabelTTF *m_pTips;
	DButton* m_pLogin;
	DButton* m_pRegister;
	DButton* m_pCloseBtn;
	cocos2d::CCNode *m_pEditBoxContainer[EDIT_BOX_COUNT_2];
	cocos2d::extension::CCEditBox *m_pEditBoxArr[EDIT_BOX_COUNT_2];

#pragma mark - - CCEditBoxDelegate
protected:
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox){};
};


#pragma mark - - LoginViewPW--
class LoginViewPW
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public cocos2d::extension::CCEditBoxDelegate
{
public:
	LoginViewPW();
	virtual ~LoginViewPW();
	CREATE_FUNC(LoginViewPW);
	CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);

private:
	void changePWCallback(cocos2d::CCObject*);
	void loginCallBack(cocos2d::CCObject*);
	void registerCallBack(cocos2d::CCObject*);
	void closePopup(cocos2d::CCObject*);
	void onEnter();
	
	void menucallback(cocos2d::CCObject*);
	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	cocos2d::CCLabelTTF *m_pTips;
	DButton* m_pLogin;
	DButton* m_pRegister;
	DButton* m_pCloseBtn;
	DButton* m_pChangePWBtn;
	cocos2d::CCNode *m_pEditBoxContainer[EDIT_BOX_COUNT_2];
	cocos2d::extension::CCEditBox *m_pEditBoxArr[EDIT_BOX_COUNT_2];

#pragma mark - - CCEditBoxDelegate
protected:
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox){};
	
};


#pragma mark - - ChangePWView--
class ChangePWView
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public cocos2d::extension::CCEditBoxDelegate
{
public:
	ChangePWView();
	virtual ~ChangePWView();
	CREATE_FUNC(ChangePWView);
	CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);

private:
	void changePWCallback(cocos2d::CCObject*);
	void closePopup(cocos2d::CCObject*);
	void onEnter();
	
	void menucallback(cocos2d::CCObject*);
	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	cocos2d::CCLabelTTF *m_pTips;
	DButton* m_pChangePWBtn;
	DButton* m_pCloseBtn;
	cocos2d::CCNode *m_pEditBoxContainer[EDIT_BOX_COUNT_4];
	cocos2d::extension::CCEditBox *m_pEditBoxArr[EDIT_BOX_COUNT_4];

#pragma mark - - CCEditBoxDelegate
protected:
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox){};
};


#pragma mark - - RegisterView--
class RegisterView
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public cocos2d::extension::CCEditBoxDelegate
{
public:
	RegisterView();
	virtual ~RegisterView();
	CREATE_FUNC(RegisterView);
	CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);
	CC_SYNTHESIZE(bool, m_bBinding, IsBinding);
private:
	void loginCallBack(cocos2d::CCObject*);
	void closePopup(cocos2d::CCObject*);
	void agreeClause(cocos2d::CCObject*);
	void onEnter();
	
	void menucallback(cocos2d::CCObject*);
	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	cocos2d::CCLabelTTF *m_pTips;
	DButton* m_pCloseBtn;
	DButton* m_pLogin;
	DButton* m_pClauseAgreeBtn;
	cocos2d::CCNode *m_pEditBoxContainer[EDIT_BOX_COUNT_4];
	cocos2d::extension::CCEditBox *m_pEditBoxArr[EDIT_BOX_COUNT_4];
	bool m_bClauseAgreed;
#pragma mark - - CCEditBoxDelegate
protected:
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox){};
};

#pragma mark - - BindingView--

class BindingView
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public cocos2d::extension::CCEditBoxDelegate
{
public:
	BindingView();
	virtual ~BindingView();
	CREATE_FUNC(BindingView);
	CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);

private:
	void loginCallBack(cocos2d::CCObject*);
	void registerCallBack(cocos2d::CCObject*);
	void closePopup(cocos2d::CCObject*);
	void onEnter();
	void agreeClause(cocos2d::CCObject*);
	void menucallback(cocos2d::CCObject*);
	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	cocos2d::CCLabelTTF *m_pTips;
	cocos2d::CCMenuItem* m_pLogin;
	DButton* m_pCloseBtn;
	DButton* m_pClauseAgreeBtn;
	bool m_bClauseAgreed;
	cocos2d::CCNode *m_pEditBoxContainer[EDIT_BOX_COUNT_3];
	cocos2d::extension::CCEditBox *m_pEditBoxArr[EDIT_BOX_COUNT_3];
	cocos2d::CCLabelTTF *m_pGuessUser;

#pragma mark - - CCEditBoxDelegate
protected:
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox){};
};

#endif