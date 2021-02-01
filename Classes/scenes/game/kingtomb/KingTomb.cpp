//
//  KingTomb.cpp
//  LibQSMY
//
//  Created by jk on 14-8-1.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#include "KingTomb.h"
#include "KingTombMine.h"
#include "KingTombHistory.h"
#include "KingTombRank.h"
#include "utils/TimerUtil.h"

KingTomb::KingTomb()
:m_pAwardName(NULL)
,m_pKingTombBack(NULL)
,m_pKingTombFore(NULL)
,m_pTimeBmTf(NULL)
,m_BtnStateFriest(NULL)
,m_BtnState(NULL)
,m_pContainer(NULL)
,m_pQiangButton(NULL)
,m_pSearchButtonFirst(NULL)
,m_pSearchButton(NULL)
,m_pTombs(NULL)
,m_index(1)
,m_pNumBMTf1(NULL)
,m_pNumBMTf(NULL)
,m_pSpeedNumBmTf(NULL)
,m_pGetNumBmTf(NULL)
,m_pVipBmTf(NULL)
,m_pVip(NULL)
,m_punOpenTip(NULL)
,m_pDoneTip(NULL)
,m_pYuanBMTf(NULL)
,m_pSpeedUp(NULL)
,m_pFisish(NULL)
,m_pCompleteBtn(NULL)
,m_pSpeedUpBtn(NULL)
,m_pHistoryBtn(NULL)
,m_pRankBtn(NULL)
,m_pRuleBtn(NULL)
,m_pIsOkImg(NULL)
,m_pSnatch(NULL)
,p_pOutPut(NULL)
,m_pInfos(NULL)
,m_pBtnBG(NULL)
,m_pFindYuan2(NULL)
,m_pFindYuan1(NULL)
,m_pYuanBMFindTf2(NULL)
,m_pYuanBMFindTf1(NULL)
,m_pSpeedYuan(NULL)
,m_pFont_free1(NULL)
,m_pFont_free2(NULL)
,m_pItemIcon(NULL)
,m_pItemIconLeft(NULL)
,m_pItemNum(NULL)
,m_fightItemImg(NULL)
,m_pInfo(NULL)
,m_pLayer(NULL)
,m_pAnimate(NULL)
,m_pSearchTimes(0)
,m_pAnimatePlay(0)
,m_pSuccessAim(NULL)
,m_pDefeatAim(NULL)
,m_pIsUpData(false)
,m_pItemSpeed(NULL)
,m_pReward(NULL)
,m_pItemNone(NULL)
,m_pMenuHide(NULL)
,m_pDelegate(NULL)
,m_pBoxNodeLeft(NULL)
,m_pBoxNodeRight(NULL)
{
    D_RESET_C_ARRAY(m_pD, TOMBS_NUM_UNIT);
}



KingTomb::~KingTomb()
{
    CC_SAFE_RELEASE(m_pAwardName);
    CC_SAFE_RELEASE(m_pKingTombBack);
	CC_SAFE_RELEASE(m_pKingTombFore);
    CC_SAFE_RELEASE(m_pTimeBmTf);
    CC_SAFE_RELEASE(m_BtnStateFriest);
    CC_SAFE_RELEASE(m_BtnState);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pQiangButton);
    CC_SAFE_RELEASE(m_pSearchButtonFirst);
    CC_SAFE_RELEASE(m_pSearchButton);
    CC_SAFE_RELEASE(m_pTombs);
    CC_SAFE_RELEASE(m_pNumBMTf1);
    CC_SAFE_RELEASE(m_pNumBMTf);
    CC_SAFE_RELEASE(m_pSpeedNumBmTf);
    CC_SAFE_RELEASE(m_pGetNumBmTf);
    CC_SAFE_RELEASE(m_pVipBmTf);
    CC_SAFE_RELEASE(m_pVip);
    CC_SAFE_RELEASE(m_punOpenTip);
    CC_SAFE_RELEASE(m_pDoneTip);
    CC_SAFE_RELEASE(m_pYuanBMTf);
    CC_SAFE_RELEASE(m_pSpeedUp);
    CC_SAFE_RELEASE(m_pFisish);
    CC_SAFE_RELEASE(m_pCompleteBtn);
    CC_SAFE_RELEASE(m_pSpeedUpBtn);
	CC_SAFE_RELEASE(m_pRuleBtn);
	CC_SAFE_RELEASE(m_pHistoryBtn);
	CC_SAFE_RELEASE(m_pRankBtn);
    CC_SAFE_RELEASE(m_pIsOkImg);
    CC_SAFE_RELEASE(m_pSnatch);
    CC_SAFE_RELEASE(p_pOutPut);
    CC_SAFE_RELEASE(m_pInfos);
    CC_SAFE_RELEASE(m_pBtnBG);
    CC_SAFE_RELEASE(m_pFindYuan2);
    CC_SAFE_RELEASE(m_pFindYuan1);
    CC_SAFE_RELEASE(m_pYuanBMFindTf2);
    CC_SAFE_RELEASE(m_pYuanBMFindTf1);
    CC_SAFE_RELEASE(m_pFont_free2);
    CC_SAFE_RELEASE(m_pFont_free1);
    CC_SAFE_RELEASE(m_pSpeedYuan);
    CC_SAFE_RELEASE(m_pItemIcon);
	CC_SAFE_RELEASE(m_pItemIconLeft);
    CC_SAFE_RELEASE(m_pItemNum);
    CC_SAFE_RELEASE(m_fightItemImg);
    CC_SAFE_RELEASE(m_pLayer);
    CC_SAFE_RELEASE(m_pSuccessAim);
    CC_SAFE_RELEASE(m_pAnimate);
    CC_SAFE_RELEASE(m_pDefeatAim);
    CC_SAFE_RELEASE(m_pItemSpeed);
    CC_SAFE_RELEASE(m_pReward);
    CC_SAFE_RELEASE(m_pItemNone);
	D_SAFE_RELEASE_C_ARRAY(m_pD, TOMBS_NUM_UNIT);
	CC_SAFE_RELEASE(m_pBoxNodeLeft);
	CC_SAFE_RELEASE(m_pBoxNodeRight);
}


bool KingTomb::init()
{
	if (!DLayer::init())
	{
		return false;
	}
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("ItemBox", ItemBoxLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorHeadGradeBg", WarriorHeadGradeBgLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_banghui/pl_banghui_tanxian", pNodeLoaderLibrary);
    
    m_pSlideView = DSlideView::create(CCRectZero,CCRectMake(0, 0, m_pContainer->getContentSize().width, m_pContainer->getContentSize().height));
	m_pContainer->addChild(m_pSlideView);
    
    m_pTombs = CCArray::create();
    m_pTombs ->retain();
    
    for(int i = 0;i<TOMBS_NUM_UNIT;i++){
        KingTombMine *tombMine = KingTombMine::create();
		m_pSlideView->addSlideWithTab(tombMine,"PL_ui/banghui/banghui_dt_bg.png");
        m_pTombs->addObject(tombMine);
    }
    
	m_pSlideView->getTabView()->setDelegate(this);

	m_pSlideView->getContentView()->setDelegate(this);
    m_pQiangButton->getButton()->setTarget(this, menu_selector(KingTomb::fightBtnCallBack));
    m_pSearchButtonFirst->getButton()->setTarget(this, menu_selector(KingTomb::searchBtnCallBack));
    m_pSearchButton->getButton()->setTarget(this, menu_selector(KingTomb::searchBtnCallBack));
    m_pSpeedUpBtn->getButton()->setTarget(this, menu_selector(KingTomb::speedUpBtnCallBack));
    m_pCompleteBtn->getButton()->setTarget(this, menu_selector(KingTomb::getRewordBtnCallBack));

	m_pRuleBtn->getButton()->setTarget(this, menu_selector(KingTomb::ruleBtnCallBack));
	m_pHistoryBtn->getButton()->setTarget(this, menu_selector(KingTomb::historyBtnCallBack));
	m_pRankBtn->getButton()->setTarget(this, menu_selector(KingTomb::rankBtnCallBack));
    m_pSlideView->getContentView()->setBounceable(false);
    m_pmodel = DM_GET_KINGTOMB_MODEL;
    m_pmodel->getModeData();
    updateView();
    
	//动画隐藏事件
	CCMenu* pMenu = CCMenu::create();
	pMenu->setTouchEnabled(false);
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(CCPointZero);
	addChild(pMenu);
	m_pMenuHide = DMenuItemImageSound::create(this, menu_selector(KingTomb::hideCallBack));
	m_pMenuHide->setContentSize(DESIGN_SIZE);
	m_pMenuHide->setAnchorPoint(CCPointZero);
	pMenu->addChild(m_pMenuHide);
	m_pMenuHide->setVisible(false);

    m_pAnimate->setDelegate(this);
    m_pSuccessAim->stop();
    m_pSuccessAim->setVisible(false);
    m_pAnimate->stop();
    m_pDefeatAim->setVisible(false);
    m_pDefeatAim->stop();
	return true;
}




bool KingTomb::onAssignCCBMemberVariable(CCObject *pTarget,const char *pMemberVariableName, CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAwardName", CCRichLabelTTF *, m_pAwardName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pKingTombBack", CCNode *, m_pKingTombBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pKingTombFore", CCNode *, m_pKingTombFore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTimeBmTf", CCLabelBMFont *, m_pTimeBmTf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_BtnStateFriest", CCNode *, m_BtnStateFriest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_BtnState", CCNode *, m_BtnState);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pQiangButton", DButton *, m_pQiangButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSearchButtonFirst", DButton *, m_pSearchButtonFirst);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSearchButton", DButton *, m_pSearchButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNumBMTf1", CCLabelBMFont *, m_pNumBMTf1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNumBMTf", CCLabelBMFont *, m_pNumBMTf);

	D_CCB_ASSIGN_ARRAY("m_pD", CCMenuItemImage*, m_pD, TOMBS_NUM_UNIT);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedNumBmTf", CCLabelBMFont *, m_pSpeedNumBmTf);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGetNumBmTf", CCLabelBMFont *, m_pGetNumBmTf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipBmTf", CCLabelBMFont *, m_pVipBmTf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVip", CCNode *, m_pVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_punOpenTip", CCNode *, m_punOpenTip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDoneTip", CCNode *, m_pDoneTip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pYuanBMTf", CCLabelBMFont *, m_pYuanBMTf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedUp", CCNode *, m_pSpeedUp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFisish", CCNode *, m_pFisish);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCompleteBtn", DButton *, m_pCompleteBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedUpBtn", DButton *, m_pSpeedUpBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRuleBtn", DButton *, m_pRuleBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHistoryBtn", DButton *, m_pHistoryBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRankBtn", DButton *, m_pRankBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIsOkImg", CCNode *, m_pIsOkImg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSnatch", CCNode *, m_pSnatch);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "p_pOutPut", CCNode *, p_pOutPut);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInfos", CCNode *, m_pInfos);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnBG", CCNode *, m_pBtnBG);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFindYuan2", CCNode *, m_pFindYuan2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFindYuan1", CCNode *, m_pFindYuan1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pYuanBMFindTf2", CCLabelBMFont *, m_pYuanBMFindTf2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pYuanBMFindTf1", CCLabelBMFont *, m_pYuanBMFindTf1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFont_free1", CCNode *, m_pFont_free1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFont_free2", CCNode *, m_pFont_free2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedYuan", CCNode *, m_pSpeedYuan);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemIcon", ItemBox *, m_pItemIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemIconLeft", ItemBox *, m_pItemIconLeft);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNum", CCLabelBMFont *, m_pItemNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_fightItemImg", CCNode *, m_fightItemImg);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLayer", CCNode *, m_pLayer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimate", UIAnimate *, m_pAnimate);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSuccessAim", UIAnimate *, m_pSuccessAim);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefeatAim", UIAnimate *, m_pDefeatAim);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemSpeed", CCLabelBMFont *, m_pItemSpeed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReward", CCNode *, m_pReward);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNone", CCNode *, m_pItemNone);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxNodeRight", CCNode *, m_pBoxNodeRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxNodeLeft", CCNode *, m_pBoxNodeLeft);
    return false;
}

SEL_MenuHandler KingTomb::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName )
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE( this, "HistoryBtnCallBack",KingTomb::historyBtnCallBack ) ;
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE( this, "RuleBtnCallBack",KingTomb::ruleBtnCallBack ) ;
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE( this, "pageCallBack",KingTomb::pageCallBack );
    return NULL ;
}

void KingTomb::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
    if(view->getContainer()->getPosition().x>0){
        view->setPositionX(0);
    }
    float _x = view->getContainer()->getPosition().x * ((m_pKingTombFore->getContentSize().width-m_pContainer->getContentSize().width)/(view->getContentSize().width-m_pContainer->getContentSize().width));
    _x = _x>0?0:_x;
    _x = _x<m_pContainer->getContentSize().width-m_pKingTombFore->getContentSize().width?_x = m_pContainer->getContentSize().width-m_pKingTombFore->getContentSize().width:_x;
    
	//CCLOG("KingTomb::scrollViewDidScroll%f" , _x);

	m_pKingTombFore->setPositionX(_x);
	m_pKingTombBack->setPositionX(_x * 0.4);
    //m_pLayer->setPositionX(view->getContainer()->getPosition().x);
    if(m_pSlideView->getContentView()->isDragging()){
        if(view->getContainer()->getPosition().x<0 && view->getContainer()->getPosition().x>(m_pContainer->getContentSize().width-view->getContentSize().width)){
            upDataViewIsNone();
            m_pIsUpData = true;
        }
    }else{
        if(m_pIsUpData){
            m_index = m_pSlideView->getSelectedSlideIdx()+1;
            updateView();
            m_pIsUpData = false;
        }
    }
}

void KingTomb::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell){
}

void KingTomb::pageCallBack(cocos2d::CCObject *sender)
{
	CCNode* pMenu = dynamic_cast<CCNode*>(sender);
	int tag = pMenu->getTag();
	int delta = tag == 0 ? -1 : 1;
	int index = m_pSlideView->getSelectedSlideIdx() + delta;
	if(index >= 0 && index < TOMBS_NUM_UNIT)
	{
		//不在边界
		m_pSlideView->selectedTab(index , true);
		m_index = m_pSlideView->getSelectedSlideIdx()+1;
		updateView();
		m_pIsUpData = false;
	}
}

//排行榜
void KingTomb::rankBtnCallBack(cocos2d::CCObject *sender){
	KingTombRank *m_pKingTombRank = KingTombRank::create();
	m_pDelegate->openTopPopup(m_pKingTombRank);
}

//历史纪录按钮回调
void KingTomb::historyBtnCallBack(cocos2d::CCObject *sender){
    KingTombHistory *m_pKingTombHistory = KingTombHistory::create();
    m_pDelegate->openTopPopup(m_pKingTombHistory);
	//Game::sharedGame()->openPopup(m_pKingTombHistory);
}

//规则按钮回调
void KingTomb::ruleBtnCallBack(cocos2d::CCObject *sender){
    KingTombRule *m_pKingTombRule = KingTombRule::create();
    Game::sharedGame()->openPopup(m_pKingTombRule);
}

//战斗按钮回调
void KingTomb::fightBtnCallBack(cocos2d::CCObject *sender){
    m_pmodel->setDragonEnd(false);
    openFightPanel();
}

//搜索按钮回调
void KingTomb::searchBtnCallBack(cocos2d::CCObject *sender){
    if (m_pmodel->getFindYuan() > DM_GET_USER_MODEL->getUserInfo()->getPoints() && m_pmodel->getFreeFindNum()==0)
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
    }else{
        if(m_pInfo){
            m_pmodel->removemAssistUser(m_pInfo);
        }
        m_pAnimatePlay = true;
        m_pAnimate->setVisible(true);
        m_pSearchTimes = m_pmodel->getSearchTimes();
        m_pAnimate->getArmature()->getAnimation()->playByIndex(0,-1,-1,true);
        upDataViewIsNone();
        TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);

		KingTombMine *tombMine = static_cast< KingTombMine *>(m_pTombs->objectAtIndex(m_index-1));
		if(tombMine && tombMine->m_pNodeUnopen->isVisible() == false)
		{
			//隐藏当前的宝箱
			m_pSlideView->setVisible(false);
		}

        m_pmodel->search(m_index);
        Game::sharedGame()->setTouchEnable(false);
    }
}

void KingTomb::animationOver()
{
	//统一动画播放完成 隐藏
	m_pAnimate->stop();
	m_pAnimate->setVisible(false);
	m_pmodel->setStopAin(false);
	Game::sharedGame()->setTouchEnable(true);
	//恢复当前的宝箱显示
	m_pSlideView->setVisible(true);
}

void KingTomb::animationComplete(UIAnimate *pAnimate)
{
    if(!m_pAnimatePlay){
        //Game::sharedGame()->setTouchEnable(true);
		animationOver();
        updateView();
    }
    if(m_pSearchTimes != m_pmodel->getSearchTimes() &&m_pAnimatePlay){
        m_pAnimate->getArmature()->getAnimation()->playByIndex(0/*1*/,-1,-1,true);
		//已经请求到了数据 停止播放动画
        m_pAnimatePlay = false;
    }else {
        if(m_pAnimatePlay){
			//尚未收到响应，继续播放
            m_pAnimate->getArmature()->getAnimation()->playByIndex(0,-1,-1,true);
            m_pSearchTimes = m_pmodel->getSearchTimes();
            if(m_pmodel->getStopAin()){
				//如果收到错误码
				m_pAnimatePlay = false;
                /*m_pAnimate->stop();
                m_pAnimate->setVisible(false);
                m_pmodel->setStopAin(false);*/
            }
        }
    }
}



//打开战斗面板
void KingTomb::openFightPanel(){
    KingTombfight *m_pFightPanel = KingTombfight::create();
    m_pFightPanel->setDelegate(this);
    Game::sharedGame()->openPopup(m_pFightPanel);
    m_pFightPanel->setData(m_index);
	
	//PL_MOVE_NODE_ABOVE_CENTER_Y
	//addChild(m_pFightPanel);
}

//打开公会成员列表 挑选助战好友
void KingTomb::openGuildListPanel(uint32_t index){
    KingTombGuildList *m_pKingTombGuildList = KingTombGuildList::create();
    m_pKingTombGuildList->setDelegate(this);
    m_pKingTombGuildList->setIndex(index);
    m_pKingTombGuildList->setPos(m_index);
    Game::sharedGame()->openPopup(m_pKingTombGuildList);
}


//加速按钮回调
void KingTomb::speedUpBtnCallBack(cocos2d::CCObject *sender){
    if (m_pmodel->getFindYuan() > DM_GET_USER_MODEL->getUserInfo()->getPoints())
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
    }else{
        m_pmodel->speedUp(m_index);
    }
}

//领取按钮回调
void KingTomb::getRewordBtnCallBack(cocos2d::CCObject *sender){
    m_pmodel->getReword(m_index);
}

void KingTomb::setOutPutNumBmTf(uint32_t num){

	//统一内部处理万字
	drop::Drop* pDrop = m_pInfo->theKingTombIsNewType() ? drop::Drop::createPoint(1) : drop::Drop::createCoin(1);
	m_pItemIconLeft->updateTombItem(pDrop , num);
}

void KingTomb::upDataViewIsNone(){
    m_fightItemImg->setVisible(false);
    p_pOutPut->setVisible(false);
    m_pInfos->setVisible(false);
    m_pSnatch->setVisible(false);
    m_pIsOkImg->setVisible(false);
    m_pReward->setVisible(false);
}


void KingTomb::upDataAimView(){
    if(m_pmodel->getDragonEnd() ){
		m_pMenuHide->setVisible(true);
        if(m_pmodel->getDragonWin()){
            m_pSuccessAim->setVisible(true);
            m_pSuccessAim->getArmature()->getAnimation()->playByIndex(0,-1,-1,true);
            m_pDefeatAim->setVisible(false);
        }else{
            m_pDefeatAim->setVisible(true);
            m_pDefeatAim->getArmature()->getAnimation()->playByIndex(0,-1,-1,true);
            m_pSuccessAim->setVisible(false);
        }
    }
}


void KingTomb::upDataBtnView(){
    int findNum = m_pmodel->getFreeFindNum();  //剩余的探索数量
    int findYuan = m_pmodel->getFindYuan();   //探索需要花费的元宝
    
    m_pFindYuan1->setVisible(findNum==0);
    m_pFindYuan2->setVisible(findNum==0);
    
    m_pFont_free1->setVisible(findNum>0);
    m_pFont_free2->setVisible(findNum>0);
    m_pNumBMTf->setVisible(findNum>0);
    m_pNumBMTf1->setVisible(findNum>0);
    
    m_pYuanBMFindTf2->setString(D_CSTRING_FROM_UINT(findYuan));
    m_pYuanBMFindTf1->setString(D_CSTRING_FROM_UINT(findYuan));
    
    m_pNumBMTf->setString(D_CSTRING_FROM_UINT(findNum));
    m_pNumBMTf1->setString(D_CSTRING_FROM_UINT(findNum));
}




void KingTomb::updateView(){
    upDataAimView();
    
    //m_pBtnBG->setVisible(true);
    m_pInfo = m_pmodel->getKingTombInfoByPos(m_index);
    TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
    TimerUtil::sharedTimerUtil()->schedule(schedule_selector(KingTomb::updata), this, 1);
   
    upDataBtnView();

    // 当前在那个矿坑的位置
	for(int i = 0 ; i < TOMBS_NUM_UNIT ; i++)
	{
		//m_pD[i]->selected(m_index == i+1);
		m_index == i+1 ? m_pD[i]->selected() : m_pD[i]->unselected();
	}
    
    kingTomb::kingTombVipInfo *vipInfo = m_pmodel->getTombVipInfo(m_index);
    bool isOpen =  m_pmodel->isOpenTombByPos(m_index);
    
    
    m_pVip->setVisible(!isOpen);
    m_pSearchButtonFirst->getButton()->setEnabled(isOpen);
    m_pSearchButton->getButton()->setEnabled(isOpen);
    m_pVipBmTf->setString(D_CSTRING_FROM_UINT(vipInfo->getVip()));
    
    KingTombMine *tombMine = static_cast< KingTombMine *>(m_pTombs->objectAtIndex(m_index-1));
    
    if(m_pInfo){
        
        tombMine->setData(m_pInfo->getMyInfo()->getType());
        m_pInfos->setVisible(true);
        m_pReward->setVisible(true);
        m_pDoneTip->setVisible(m_pInfo->theKingTombIsWork());
        m_punOpenTip->setVisible(m_pInfo->theKingTombIsWork()==false);
        
        //倒计时
        uint32_t ts;
        uint32_t serverTime = GameManager::sharedGameManager()->getServerTime();
        if(m_pInfo->getMyInfo()->getOccupyTs()==0){
            ts = D_CONFIG_UINT_FOR_KEY(TOMBS_TIMELIMIT) + m_pInfo->getSearchTs() - serverTime;
            //如果超时了就移除掉
            if(m_pInfo->isTimeLimit()){
                m_pmodel->removeKingTombInfoByPos(m_pInfo->getId(),true);
                return;
            }
            m_pTimeBmTf->setString(TimeUtil::getSeconds2String(ts));
        }
        
        int state = m_pInfo->getState();
        upDataItemView();
        upDataOutPutView();
        upDataSpeedNumView();

        kingTomb::kingTombTypeInfo * tombInfo = m_pmodel->getTombTypeInfo(m_pInfo->getMyInfo()->getType());
        m_pAwardName->setString(tombInfo->getName().c_str());
        
        switch(state)
        {
            case kKingTombStateIsWork://工作中
                //產量顯示
                m_pFisish->setVisible(false);
                //加速按鈕顯示
                m_pSpeedUp->setVisible(true);
                m_BtnStateFriest->setVisible(false);  //按钮隐藏
                m_BtnState->setVisible(false);  // 按钮隐藏
                upDataSpeedBtnView();
                m_pTimeBmTf->getParent()->setVisible(true);  //时间ttf 显示
                m_pTimeBmTf->setString(TimeUtil::getSeconds2String(m_pInfo->getEndTs() - GameManager::sharedGameManager()->getServerTime()));
                break;
            case kKingTombStateIsComplete://工作完成可以領取獎勵了
                m_pFisish->setVisible(true);
                m_pSpeedUp->setVisible(false);
                m_pSnatch->setVisible(false);
                m_pIsOkImg->setVisible(true);
                m_BtnStateFriest->setVisible(false);
                m_BtnState->setVisible(false);
                m_pTimeBmTf->getParent()->setVisible(false);
                TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
                break;
            case kKingTombStateIsReady: //什麼礦都沒有
                m_pFisish->setVisible(false);
                m_pSpeedUp->setVisible(false);
                m_BtnStateFriest->setVisible(false);
                m_BtnState->setVisible(true);
                m_pTimeBmTf->getParent()->setVisible(true);
                break;
            case kKingTombStateIsRob: // 矿被抢夺了
                m_pFisish->setVisible(true);
                m_pSpeedUp->setVisible(false);
                m_BtnStateFriest->setVisible(false);
                m_BtnState->setVisible(false);
                m_pTimeBmTf->getParent()->setVisible(false);
                m_pSnatch ->setVisible(true);
                m_pIsOkImg->setVisible(false);  //矿完成了的图片显示
                TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
                // 如果矿被抢夺了  产量读取服务端  数据 不是自己计算的
                cocos2d::CCArray* bonusData = m_pInfo->getBonus();
                cocos2d::CCObject* obj = NULL;
                if (bonusData)
                {
                    CCARRAY_FOREACH(bonusData, obj)
                    {
                        drop::Drop* pDrop = (drop::Drop*)obj;
						if(m_pInfo->theKingTombIsNewType())
						{
							//新箱子
							if(pDrop->getType()==drop::kDropTypePoint){
								setOutPutNumBmTf(pDrop->getPoint());
							}else{
								m_pItemIcon->getCount()->setString("1");
							}
						}
						else
						{
							if(pDrop->getType()==drop::kDropTypeCoin){
								setOutPutNumBmTf(pDrop->getCoin());
							}else if(pDrop->getType()==drop::kDropTypePoint){
								m_pItemIcon->getCount()->setString(D_CSTRING_FROM_UINT(pDrop->getPoint()));
								//m_pItemNum->setString(D_CSTRING_FROM_UINT(pDrop->getPoint()));
							}else{
								m_pItemIcon->getCount()->setString(D_CSTRING_FROM_UINT(pDrop->getItem()->getNum()));
								//m_pItemNum->setString(D_CSTRING_FROM_UINT(pDrop->getItem()->getNum()));
							}
						}						
                    }
                }
                break;
        }
    }else{
        //没有矿坑的 显示设置
        m_fightItemImg->setVisible(false);
        p_pOutPut->setVisible(false);
        m_BtnState->setVisible(false);
        m_pAwardName->setString("");
        tombMine->setData(0);
        m_pSpeedUp->setVisible(false);
        m_pInfos->setVisible(false);
        m_pFisish->setVisible(false);
        m_pReward->setVisible(false);
        //m_pBtnBG->setVisible(isOpen);
        m_BtnStateFriest->setVisible(isOpen);
        TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
    }
}


void KingTomb::upDataSpeedNumView(){
    if(m_pInfo->getMyInfo()->getSpeedUpTs()==0){
        m_pSpeedNumBmTf->setString(D_CSTRING_FROM_UINT(m_pInfo->getMyInfo()->getSpeedNum()));
    }else{
        m_pSpeedNumBmTf->setString(D_CSTRING_FROM_UINT(m_pInfo->getMyInfo()->getSpeedNum()*2));
    }
}


//加速 显示不显示 能不能 点击
void KingTomb::upDataSpeedBtnView(){
    if(m_pInfo->getMyInfo()->getSpeedUpTs()==0){
        m_pSpeedUpBtn->getButton()->setEnabled(true);
        m_pYuanBMTf->setVisible(true);
        m_pSpeedYuan->setVisible(true);
        m_pYuanBMTf->setString(D_CSTRING_FROM_UINT(m_pInfo->getSpeedUpYuan()));
    }else{
        m_pSpeedUpBtn->getButton()->setEnabled(false);
        m_pYuanBMTf->setVisible(false);
        m_pSpeedYuan->setVisible(false);
    }
}


//产出数据updata
void KingTomb::upDataOutPutView(){
    int state = m_pInfo->getState();
    if(m_pInfo->theKingTombIsUser()){
        m_fightItemImg->setVisible(true);
        p_pOutPut->setVisible(false);
        setOutPutNumBmTf((int)floor(m_pInfo->getOtherInfo()->getOutputNum()*0.3));
    }else{
        m_fightItemImg->setVisible(false);
        p_pOutPut->setVisible(true);

        if(state == kKingTombStateIsReady){
            //m_pGetNumBmTf->setString("0");
			//m_pItemIconLeft->updateTombItem(drop::Drop::createCoin(1) , 0);
			setOutPutNumBmTf(0);
			m_pItemIconLeft->getAmountNode()->setVisible(false);
			//m_pItemIconLeft->updateTombItem(NULL);
        }else{
            setOutPutNumBmTf(m_pInfo->getMyInfo()->getOutputNum());
        }
    }
}


//额外物品icon updata
void KingTomb::upDataItemView()
{
    int state = m_pInfo->getState();
    if(m_pInfo->getMyInfo()->getExtraOutputType()==0)
	{
        //m_pItemNone->setVisible(true);
        m_pItemSpeed->setString("0");
        //m_pItemNum->setString("0");
        //m_pItemIcon->setVisible(false);
		m_pItemIcon->updateTombItem(NULL);

		m_pBoxNodeRight->setVisible(false);
		m_pBoxNodeLeft->setPositionX(110);
    }
	else
	{
		m_pBoxNodeRight->setVisible(true);
		m_pBoxNodeLeft->setPositionX(0);

        m_fightItemImg->setVisible(true);
        //m_pItemNone->setVisible(false);
        //m_pItemIcon->setVisible(true);
        kingTomb::kingTombItemInfo *itemInfo = m_pmodel->getTombItemInfo(m_pInfo->getMyInfo()->getExtraOutputType());
        CCInteger *num = (CCInteger*)itemInfo->getItemNumByLv()->objectForKey(m_pInfo->getMyInfo()->getType());
        if(m_pInfo->theKingTombIsNewType())
		{
			//新箱子
			m_pItemIcon->updateTombItem(drop::Drop::create(drop::kDropTypeEquip , num->getValue()));
		}
		else
		{
			if(itemInfo->getItemtype() == drop::kDropTypeItem){

				item::ItemInfo* pItemInfo = NULL;
				item::ItemBase* pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(itemInfo->getItemid());
				if(pItemBase) pItemInfo = item::ItemInfo::create(itemInfo->getItemid(),num->getValue());

				m_pItemIcon->updateTombItem(drop::Drop::create(pItemInfo));
			}else{
				m_pItemIcon->updateTombItem(drop::Drop::createPoint(num->getValue()));
			}
		}
		
        if(m_pInfo->getMyInfo()->getSpeedUpTs()==0){
            m_pItemSpeed->setString(D_CSTRING_FROM_UINT(num->getValue()));
        }else{
            m_pItemSpeed->setString(D_CSTRING_FROM_UINT(num->getValue()*2));
        }
        
        //如果是抢的别人的矿
        if(m_pInfo->theKingTombIsUser()){
            m_fightItemImg->setVisible(true);
			m_pItemIcon->getCount()->setString(D_CSTRING_FROM_UINT((int)floor(m_pInfo->getOtherInfo()->getOutputItemNum()*0.3)));
            //m_pItemNum->setString(D_CSTRING_FROM_UINT((int)floor(m_pInfo->getOtherInfo()->getOutputItemNum()*0.3)));
        }else{
            m_fightItemImg->setVisible(false);

            if(state == kKingTombStateIsReady){
				m_pItemIcon->getAmountNode()->setVisible(false);
                //m_pItemNum->setString("0");
            }else{
				m_pItemIcon->getCount()->setString(D_CSTRING_FROM_UINT(m_pInfo->getMyInfo()->getOutputItemNum()));
                //m_pItemNum->setString(D_CSTRING_FROM_UINT(m_pInfo->getMyInfo()->getOutputItemNum()));
            }
        }
        //新箱子 直接就是1
        if(m_pInfo->theKingTombIsNewType())m_pItemIcon->getCount()->setString("1");
    }
    
}


void KingTomb::updata(float dt){
    if(m_pInfo){
        uint32_t ts;
        uint32_t serverTime = GameManager::sharedGameManager()->getServerTime();
        if(m_pInfo->getMyInfo()->getOccupyTs()==0){
            ts = D_CONFIG_UINT_FOR_KEY(TOMBS_TIMELIMIT) + m_pInfo->getSearchTs() - serverTime;
            if(m_pInfo->isTimeLimit()){
                m_pmodel->removeKingTombInfoByPos(m_pInfo->getId(),true);
                return;
            }
        }else{
            int state = m_pInfo->getState();
            if(state ==kKingTombStateIsWork){
                ts =  m_pInfo->getEndTs() -  serverTime;
            }else if(state == kKingTombStateIsComplete){
                ts = 0;
                this->updateView();
            }else{
                ts = 0;
            }
        }
        m_pTimeBmTf->setString(TimeUtil::getSeconds2String(ts));
    }
}



void KingTomb::onEnter()
{
    TimerUtil::sharedTimerUtil()->schedule(schedule_selector(KingTomb::updata), this, 1);
    BINDING_ONLY(m_pmodel, this, KingTomb::updateView, D_EVENT_KINGTOMBLIST_GET);
    DLayer::onEnter();
}

void KingTomb::onExit()
{
    TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
    Game::sharedGame()->setTouchEnable(true);
    
    UNBINDING_ALL(m_pmodel, this);
	DLayer::onExit();
}

void KingTomb::hideCallBack(CCObject*)
{
	m_pMenuHide->setVisible(false);
	m_pSuccessAim->stop();
	m_pSuccessAim->setVisible(false);
	m_pDefeatAim->setVisible(false);
	m_pDefeatAim->stop();
	return;
}