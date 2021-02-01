//
//  Pve.h
//  pve
//
//  Created by wanghejun on 13-3-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Pve__
#define __QSMY__Pve__

#define PVE_ACTION_MOVE_DIS	640
#define PVE_ACTION_MOVE_DUR_ENTER	0.4f
#define PVE_ACTION_MOVE_DUR_EXIT	0.2f

#define PVE_BATTLE_BACK_Y_POS	-200

#define MAX_ROTATION 25

#include "AppInclude.h"
#include "components/components.h"

#include "managers/GameManager.h"
#include "model/PveModel.h"
#include "model/UserModel.h"
#include "../Game.h"

class ChapterView;
class MapView;
class MapInfoView;
class RecoverPower;
//class RecoverByItem;
class RushSettle;

class PveDelegate
{
public:
    virtual void showChapterView()					= 0;
    virtual void showChapterView(uint32_t chapterId, bool bFromChapter = false)= 0;
    virtual void showMapView(uint32_t chapterId, bool bFromChapter = false,bool bFromMapInfo = false)    = 0;
	virtual void showTianqiChapterView()					= 0;
	virtual void showTianqiChapterView(uint32_t chapterId, bool bFromChapter = false)= 0;
    virtual void showMapInfoView(uint32_t id)		= 0;
    virtual void hideMapInfoView()					= 0;
	virtual void playOnceMore(uint32_t mid)			= 0;
	virtual void showTouchItem(uint32_t chapterId)	= 0;
    virtual void startExploreRequest(uint32_t mid)  = 0;
    virtual void rushMap(uint32_t mid)              = 0;
    virtual void showPowerNotEnough()               = 0;    //显示购买体力
    virtual void clearRushCD()                      = 0;    //清除扫荡cd
    virtual void closePopup()                       = 0;    
    virtual void getGradeAward(uint32_t cid)        = 0;    //获取三星奖励
	
	virtual void playNextDialog()					= 0;
	virtual bool isLastDialog()						= 0;
	virtual void replayDialog(pve::ChapterInfo *pInfo) = 0;
	virtual void endDialog()						= 0;

	virtual void showDropItemInfo(cocos2d::CCNode* obj,bool setY = true)=0;
	virtual void showEliteView() = 0;
};

class PveRewardDelegate
{
public:
	virtual void onTouchPveRewardOk(uint32_t chapterId,uint8_t vip)		= 0;
	virtual void onTouchPveRewardCancel(uint32_t chapterId)	= 0;
};

class Pve
:public DLayer
,public PveDelegate
,public PveRewardDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public DPopupDelegate
,public BackKeyInterface
{
public:
    Pve();
    ~Pve();
    virtual bool init(pve::PveFightType type);
    //CREATE_FUNC(Pve);
	static Pve* create(pve::PveFightType type = pve::PveFightNormal);
	virtual bool										onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler					onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler	onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName) {return NULL;}
    
    virtual void onEnter();
	virtual void onExit();

	void onceMore(const Json::Value &requestData, const Json::Value &responseData);
    
    virtual void showChapterView();
    virtual void showChapterView(uint32_t id, bool bFromMapList = false);
    virtual void showMapView(uint32_t id, bool bFromChapter = false,bool bFromMapInfo = false);
	virtual void showTianqiChapterView();        //副本活动
	virtual void showTianqiChapterView(uint32_t id, bool bFromMapList = false);
	//virtual void showTianqiMapView(uint32_t id, bool bFromChapter = false,bool bFromMapInfo = false);
    virtual void showMapInfoView(uint32_t id);
    virtual void hideMapInfoView();
    virtual void startExploreRequest(uint32_t mid);
    virtual void rushMap(uint32_t mid);
    virtual void showPowerNotEnough();
    virtual void clearRushCD();
    virtual void getGradeAward(uint32_t cid);
	void onGradeAward(const Json::Value &requestData,const Json::Value &responseData);
    
    virtual void confirmDoRush(MessageType type,DMessageBox* target);
    
	virtual void playOnceMore(uint32_t mid);
	virtual void showTouchItem(uint32_t chapterId);
	virtual void onTouchPveRewardOk(uint32_t chapterId,uint8_t vip);
	virtual void onTouchPveRewardCancel(uint32_t chapterId);
	virtual void closePopup();
    
    virtual void onPopupClose(DPopup*);
    virtual void onPopupOpen(DPopup*);
	void RemoveChild();
    
	void displayWonders(uint32_t idx = 0);
	virtual void showDropItemInfo(cocos2d::CCNode* obj,bool setY = true);
	virtual void showEliteView();

private:
    void displayRushResult();
    void displayExploreResult();
    void displayExploreBattle(pve::MapInfo* info);
    
    virtual void showRushResultEnd(float dt);
    virtual void showResultEnd(cocos2d::CCObject *exploreNode);
    virtual void exploreBattleEnd(cocos2d::CCObject *exploreNode);
    
private:
	void startExploreHandler(const Json::Value &,const Json::Value &);
    void rushMapEnd(const Json::Value &,const Json::Value &);
    void updateView();
    void updateActivityView(); //副本活动
private:
    
    cocos2d::CCNode *m_pContainer;
    
    PveModel        *m_pPveModel;
    UserModel       *m_pUserModel;
	cocos2d::CCLayerColor*	m_pPopup;
    
private:
    RecoverPower    *m_pRecoverPower;
	//RecoverByItem   *m_pRecoverByItem;
    RushSettle      *m_pRushSettle;

private:
	void showDialogs(int mapID);
	cocos2d::CCArray    *m_pDialogs;
	uint32_t            currentDialogIndex;
	virtual void playNextDialog();
	virtual bool isLastDialog();
	MapInfoView* m_pMapInfoWithDialog;
	MapInfoView *m_pMapInfo;
	int m_nLastShowMapID;
	int m_nLastShowChapterID;

	bool m_bAfterBattle;
	bool m_bReplayDialog;
	bool m_bFirstPlay;
	bool m_bRushResult;
public:
	void replayDialog(pve::ChapterInfo *pInfo);
	void endDialog();

	virtual void onBackKey();

	void confirmDoBuy(MessageType type,DMessageBox* targe);

private:
	unsigned int m_uBuyMid;
};

#endif /* defined(__QSMY__Pve__) */
