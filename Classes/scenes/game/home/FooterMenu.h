#ifndef __QSMY__FOOTERMENU__
#define __QSMY__FOOTERMENU__

#include "AppInclude.h"
#include "components/components.h"
#include "model/NavigationModel.h"

enum FooterTag
{
	kTagHome	= 0,
	kTagItem	= 1,
	kTagEquip	= 2,
	kTagMission	= 3,
	kTagSetting	= 4,
	kTagMall	= 5,
	kTagOther = 6,
	kTagFriend = 7,
	kTagRank = 8,
};


class FooterMenuView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
	
public:
	FooterMenuView();
	virtual ~FooterMenuView();

	CREATE_FUNC(FooterMenuView);

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
	void menuCallBack(cocos2d::CCObject* obj);

    virtual void removeFromParentAndCleanup(bool cleanup);
	void setNavigateDisplay(navigation::NavigationType value);
    
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	//更新成就角标
	void updateAchievement(uint32_t count);
	void updateMall();
	//更新碎片角标
	void updateAvailable();

private:
	cocos2d::CCNode*                m_pPopMenuNode;
    cocos2d::CCMenuItemImage*       m_pOtherButton;
	DButton*						m_pSettingButton;
	DButton*						m_pFriendButton;
	DButton*						m_pRankButton;

	cocos2d::CCNode*                m_pAchievementLogo;
	cocos2d::CCLabelBMFont*    m_pAchievementCount;
	cocos2d::CCNode *m_pMallLogo;
	cocos2d::CCLabelBMFont *m_pMallCount;

	cocos2d::CCNode				*m_pAvailableBg;
	cocos2d::CCLabelBMFont	*m_pAvailableLabel;
};


#endif /* defined(__QSMY__FOOTERMENU__) */