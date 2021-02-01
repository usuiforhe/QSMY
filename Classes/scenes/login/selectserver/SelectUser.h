



#ifndef __PILI__SelectUser__
#define __PILI__SelectUser__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/GameManager.h"
#include "../LoginInclude.h"
#include "model/PlatformModel.h"
#include "managers/PlatformManager.h"


class SelectUserDelegate
{
	//public:

};
class NameCell;

class SelectUser
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public SelectUserDelegate
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
{
enum ButtonTag{
	kLoginButton=1,
	kRegisterOrBindingButton,
	kRegisterButton,
	kChangePWButton,
	kFacebookLoginButton,
	kGoogelPlusLoginButton,
	kMenuUp,
	kMenuDown,
	kBindingForQuickLoginBtn
};

public:
	SelectUser();
	virtual ~SelectUser();
	static SelectUser* create(LoginType type=kQuickLogin);
	CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);
private:
	void menuCallback(cocos2d::CCObject* obj);

	virtual bool init(LoginType type);
	void reset();
	virtual void onEnter();
	virtual void onExit();
	void updateView();
	void loginOthersCallback(cocos2d::CCObject* obj);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);


	void closePopup(cocos2d::CCObject*);

private:

	PlatformModel       *m_pPlatformModel;
	cocos2d::CCLabelTTF     *m_pUserName;
	cocos2d::CCNode     *m_pTab1;
	cocos2d::CCNode     *m_pTab2;
	DButton* m_pCloseBtn;
	cocos2d::CCSprite   *m_pRegister;
	cocos2d::CCNode   *m_pQuickLoginNode;
	cocos2d::CCNode   *m_pLoginNode;
	cocos2d::CCNode     *m_pNameContainer;

	LoginType m_pLoginType;
	DTableView *m_pListView;
	cocos2d::extension::CCData  *m_pData;

	NameCell              *m_pNameCell;
	cocos2d::CCMenu        *m_pMenuUp;
	cocos2d::CCMenu        *m_pMenuDown;
	cocos2d::CCNode     *m_pDropBox;
	cocos2d::CCNode     *m_pDropContainer;
	cocos2d::CCMenuItem    *m_pRegisterBtn;
	cocos2d::CCMenuItem    *m_pLoginOthers;
};


//
//class NameCellListView
//	:public DLayer
//	,public SelectUserDelegate
//	,public cocos2d::extension::CCBMemberVariableAssigner
//	,public cocos2d::extension::CCBSelectorResolver
//	
//{
//public:
//	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
//	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
//	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
//
//private:
//	
//};


class NameCellView
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);
public:
	NameCellView();
	virtual ~NameCellView();

	static NameCellView* create(cocos2d::extension::CCData* pCell = NULL);

	virtual bool init(cocos2d::extension::CCData* pCell = NULL);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setUserLoginInfo(platform::UserLoginInfo*);
	
	platform::UserLoginInfo* getUserLoginInfo()
	{
		return m_pUserInfo;
	}
private:
	void menuCallback(cocos2d::CCObject* obj);

	NameCell        *m_pNameCell;
	cocos2d::CCMenuItem        *m_pClose;
	platform::UserLoginInfo* m_pUserInfo;
};

class NameCell
	:public cocos2d::CCNode
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	NameCell();
	virtual ~NameCell();

	CREATE_FUNC(NameCell);

	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setUserLoginInfo(platform::UserLoginInfo*);
private:
	cocos2d::CCSprite      *m_pGoogleIcon;
	cocos2d::CCSprite      *m_pFacebookIcon;
	cocos2d::CCSprite      *m_pGuessIcon;
	cocos2d::CCSprite      *m_pUserIcon;
	cocos2d::CCLabelTTF    *m_pName;
	
};


class NameCellLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(NameCellLoader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(NameCell);
};


#pragma mark -- RemoveUserTipsView 

class RemoveUserTipsView
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);
public:
	RemoveUserTipsView();
	virtual ~RemoveUserTipsView();
	CREATE_FUNC(RemoveUserTipsView);

private:
	
	void closePopup(cocos2d::CCObject*);
	void onEnter();
	void deleteCallback(cocos2d::CCObject*);
	void bindCallback(cocos2d::CCObject*);

	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	DButton* m_pDeleteBtn;
	DButton* m_pBindBtn;
	DButton* m_pCloseBtn;

};

#endif /* defined(__PILI__SelectUser__) */