//
//  UserInfo.h
//  用户基本信息title

//  Created by cll on 13-4-1.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__USERINFO__
#define __QSMY__USERINFO__

#include "AppInclude.h"
#include "components/components.h"
#include "model/UserModel.h"
#include "UserDetail.h"

namespace user {
    
    
    /**
     *	@brief	延迟显示属性对象
     */
    class UserInfoDiff

    :public cocos2d::CCObject
    {
    public:
        UserInfoDiff()
        :iUserExp(0)
        ,iPoints(0)
        ,iCoins(0)
        ,iPower(0)
        {
            
        }
        ~UserInfoDiff(){}
        CREATE_FUNC(UserInfoDiff);
        D_INIT_FUNC();

        uint32_t iUserExp;			//玩家的累计经验
		uint32_t iPoints;			//点券 + 礼券
		uint32_t iCoins;			//游戏币
        uint32_t iPower;			//当前体力
        
    };

	enum UserInfoDisplayType
	{
		kUserInfoFull = 0,
		kUserInfoSimple,
		kUserInfoMax,
	};

}


class UserInfoView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public UserDetailDelegate
,public DPopupDelegate
{
	
public:
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    static UserInfoView* sharedUserInfoView(void);
    static void purgeUserInfoView(void);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
	void menuCallBack(cocos2d::CCObject* obj);
	void userDetailsCallBack(cocos2d::CCObject* obj);
    
    virtual void removeFromParentAndCleanup(bool cleanup);
    
    /**
     *	@brief	增加差值显示  处理 金币 经验 和 点券 体力 的延迟显示
     *
     *	@param 	pInfo 差值玩家信息
     */
    void addDiff(user::UserInfoDiff* pInfo);

    void removeDiff(user::UserInfoDiff* pInfo);

	void setDisplayType(user::UserInfoDisplayType type);
    
private:
    
    UserInfoView();
    
    virtual ~UserInfoView();
    
    void onUserInfoUpdated();
    
    void updateUserInfo();
	void updateFullInfo();
	void updateSimpleInfo();
	void addPow();
	void addEndu();
    
    void diffValue(user::UserInfoDiff* pInfo);

	virtual void willShowVipView(UserDetail *pDetailView);
	virtual void onPopupClose(DPopup*);
	virtual void onPopupBack(DPopup*);
private:
    user::UserInfoDisplayType  m_eDisplayType;
    user::UserInfo *userInfo;

	cocos2d::CCNode*	m_pFullInfoCCBNode;//完整版用户信息
	cocos2d::CCNode* m_pSimpleCCBNode;//简化版用户信息

	cocos2d::CCNode*	m_pPowerNode;
	cocos2d::CCNode*	m_pEnduranceNode;

	cocos2d::CCLabelTTF *username;		/*弟子名*/

	//简化版用户信息内容
	cocos2d::CCLabelTTF *username_simple;
	cocos2d::CCLabelBMFont* coins_simple;
	cocos2d::CCLabelBMFont* points_simple;

    cocos2d::CCLabelBMFont *power;		/*体力*/	
    cocos2d::CCLabelBMFont *points;		/*元宝/点券数*/
    cocos2d::CCLabelBMFont *coins;		/*游戏币*/
    cocos2d::CCLabelBMFont *exp;		/*经验*/
    cocos2d::CCLabelBMFont *level;		/*等级*/
    cocos2d::CCLabelBMFont *vipLevel;	/*VIP等级*/
    DProgressBar* expline;				/*经验条*/

	DProgressBar *m_pPBPower;
	DProgressBar *m_pPBEndu;
	cocos2d::CCLabelBMFont *m_pBMFEndu;		/*体力*/	
	cocos2d::CCLabelBMFont *m_pBMFTime;
    
    cocos2d::CCArray        *m_pDiffs;    //显示差值

	cocos2d::CCNode	*m_pEffectNode;
	cocos2d::CCNode *m_pPrivileges;

	void updatePrivileges();
};

#endif /* defined(__QSMY__USERINFO__) */
