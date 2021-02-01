//
//  Battle.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Battle.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "managers/SoundManager.h"
#include "AppEventType.h"
#include "UnitView.h"
#include "DragonBone.h"
#include "BattleDefine.h"
#include "./settlements/SettlementView.h"
#include "utils/SystemUtil.h"

USING_NS_CC;
using namespace battle;
using namespace sp;



#define NEXT_ACTION_TAG 100
#define RESET_SKILL_ACTION_TAG 101
#define PAUSE_DIALOG_DELAY 999999.f

Battle::Battle()
:m_pBattleModel(NULL)
,m_pUnitModel(NULL)
,m_pView(NULL)
,m_pBuffModel(NULL)
,currentActionIndex(0)
,currentDialogIndex(0)
,m_nNumberOfLoadedSprites(0)
,m_nNumberOfSprites(0)
,m_pLoading(NULL)
,m_pDialogs(NULL)
,m_pSettle(NULL)
{
    
}

Battle::~Battle()
{
    releaseResource();
    CC_SAFE_RELEASE(m_pDialogs);
}

bool Battle::init()
{
    if(!DLayer::init())
    {
        return false;
    }
    m_kSkipType = battle::kSkipBattleTypeSkip;
	m_kPrevSkipType = battle::kSkipBattleTypeSkip;

    m_pBattleModel = DM_GET_BATTLE_MODEL;
    m_pUnitModel = DM_GET_WARRIOR_MODEL;
    m_pBuffModel    = DM_GET_BUFF_MODEL;
    
    showLoading();
    
    m_pDialogs = m_pBattleModel->getBattleDialog();
    CC_SAFE_RETAIN(m_pDialogs);

    ResourceManager::sharedResourceManager()->clearCache();
	this->runAction(CCSequence::create(CCDelayTime::create(0.0f),CCCallFunc::create(this, callfunc_selector(Battle::initBattleResource)),NULL));
	//initBattleResource();
	setKeypadEnabled(true);
    return true;
}

#pragma mark - battle -


void Battle::releaseResource()
{
    cocos2d::CCTextureCache::sharedTextureCache()->textureForKey("fonts/skill.png")->release();
    cocos2d::CCTextureCache::sharedTextureCache()->textureForKey("battle/skill/common/skillcoverbg.png")->release();
}

void Battle::initBattleResource()
{
    ResourceManager::sharedResourceManager()->addImage("fonts/skill.png", kCCTexture2DPixelFormat_RGBA4444);
    ResourceManager::sharedResourceManager()->addImage("battle/skill/common/skillcoverbg.png", kCCTexture2DPixelFormat_A8);
    cocos2d::CCTextureCache::sharedTextureCache()->textureForKey("fonts/skill.png")->retain();
    cocos2d::CCTextureCache::sharedTextureCache()->textureForKey("battle/skill/common/skillcoverbg.png")->retain();
    loadBattleResource();
}

void Battle::loadBattleResource()
{
    m_nNumberOfLoadedSprites = 0;
    /////////////////战斗单位资源/////////////////
    const char *_imagePath;
    
	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("battle/skill/common/num.png",NUM_SK);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
#define UI_COUNT 5//6 //增加异步加载 修改数量
	/*ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("battle/skill/common/skillcover_heroleft.png",SKILL_COVER_HEROLEFT_SK,this,callfunc_selector(Battle::loadingCallBack),false);
	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("battle/skill/common/skillcover_heroright.png",SKILL_COVER_SK,this,callfunc_selector(Battle::loadingCallBack),false);
	*/
	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("battle/skill/common/skillcover.png",SKILL_COVER_SK,this,callfunc_selector(Battle::loadingCallBack),false);
	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("battle/skill/common/skillnamelow.png",SKILL_COVER_UNIT_SK,this,callfunc_selector(Battle::loadingCallBack),false);
	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("battle/skill/common/effect.png",EFFECT_SK,this,callfunc_selector(Battle::loadingCallBack),false);
    if(m_pBattleModel->getBattleInfo()->isWin())
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("PL_ui/animation/win.png",ANI_SK,this,callfunc_selector(Battle::loadingCallBack),true);
    else
	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("PL_ui/animation/lose.png",ANI_SK,this,callfunc_selector(Battle::loadingCallBack),false);

	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("PL_ui/animation/battlestar.png",ANI_SK,this,callfunc_selector(Battle::loadingCallBack),false);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
    
    m_nNumberOfSprites = m_pBattleModel->getBattleInfo()->flipUnits.size() + m_pBattleModel->getBattleInfo()->units.size() + m_pBattleModel->getBattleInfo()->skills.size() + m_pBattleModel->getBattleInfo()->buffs.size() + UI_COUNT;
    
    std::set<uint32_t>::iterator itr;
    
    for (itr=m_pBattleModel->getBattleInfo()->skills.begin(); itr!=m_pBattleModel->getBattleInfo()->skills.end();++itr)
    {
        m_pBattleModel->parseSkill(*itr, CCString::createWithFormat("battle/skill/%d.json",*itr)->getCString());
    }
    
	CCLOG("开始加载右方势力不需要翻转的英雄单位 %d",m_pBattleModel->getBattleInfo()->flipUnits.size());
	for (itr=m_pBattleModel->getBattleInfo()->flipUnits.begin(); itr!=m_pBattleModel->getBattleInfo()->flipUnits.end();++itr)
	{
		//warrior::WarriorBaseInfo* pUnit = m_pUnitModel->getWarriorBaseByWID(*itr);
		_imagePath = CCString::createWithFormat("battle/unit/%d.png",*itr)->getCString();
		CCLOG("开始加载右方势力不需要翻转的英雄单位 %s",_imagePath);
		ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(_imagePath,UNIT_SK_NAME(*itr),
			this,callfunc_selector(Battle::loadingCallBack),SystemUtil::isNBDevice());
	}
    
    CCLOG("开始加载单位 %d",m_pBattleModel->getBattleInfo()->units.size());
    for (itr=m_pBattleModel->getBattleInfo()->units.begin(); itr!=m_pBattleModel->getBattleInfo()->units.end();++itr)
    {
        //warrior::WarriorBaseInfo* pUnit = m_pUnitModel->getWarriorBaseByWID(*itr);
        _imagePath = CCString::createWithFormat("battle/unit/%d.png",*itr)->getCString();
        CCLOG("开始加载单位 %s",_imagePath);
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(_imagePath,UNIT_SK_NAME(*itr),
                                                                                   this,callfunc_selector(Battle::loadingCallBack),SystemUtil::isNBDevice());
    }
    
    
    CCLOG("开始加载技能 %d",m_pBattleModel->getBattleInfo()->skills.size());
    for (itr=m_pBattleModel->getBattleInfo()->skills.begin(); itr!=m_pBattleModel->getBattleInfo()->skills.end();++itr)
    {
        battle::Skill* pSkill = m_pBattleModel->getSkill(*itr);
        if(!pSkill->visible)
        {
            --m_nNumberOfSprites;
            continue;
        }
        _imagePath = CCString::createWithFormat("battle/skill/%d.png",*itr)->getCString();
        CCLOG("开始加载技能 %s",_imagePath);
        //CCTextureCache::sharedTextureCache()->addImageAsync(_imagePath, this, callfuncO_selector(Battle::loadingCallBack));
        //m_pBattleModel->parseSkill(*itr, CCString::createWithFormat("battle/skill/%d.json",*itr)->getCString());
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(_imagePath,pSkill->getSkillInfo()->getSkeletonName(),
                                                                                   this,callfunc_selector(Battle::loadingCallBack),pSkill->getSkillInfo()->getHighQuality());
    }
    
    CCLOG("开始加载buff %d",m_pBattleModel->getBattleInfo()->buffs.size());
    for (itr=m_pBattleModel->getBattleInfo()->buffs.begin(); itr!=m_pBattleModel->getBattleInfo()->buffs.end();++itr) {

        //以下代码会导致重播加载卡住，眩晕buff为1025，资源文件夹内有该文件。
//        if(*itr == buff::KBuffTypeVertigo)
//        {
//            m_nNumberOfLoadedSprites++;
//            continue;//眩晕没有特效 回头看下是否需要配个不需要特效的buff类型
//        }
        
        _imagePath = CCString::createWithFormat("battle/buff/%d.png",*itr)->getCString();
        CCLOG("开始加载Buff %s",_imagePath);
        //CCTextureCache::sharedTextureCache()->addImageAsync(_imagePath, this, callfuncO_selector(Battle::loadingCallBack));
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(_imagePath,m_pBuffModel->getBuff(*itr)->getSkeletonName(),this,callfunc_selector(Battle::loadingCallBack),true);
        
    }
	if(DM_GET_BATTLE_MODEL->getShowFateAni() && SystemUtil::isNBDevice()){
		CCLOG("开始加载缘分加成");
		ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("battle/fate/fate_title.png",FATE_TITLE_SK,this,callfunc_selector(Battle::loadingCallBack),false);
		m_nNumberOfSprites++;
		for(int i=1; i<8; i++)
		{
			for(int j=0; j<3; j++)
			{
				_imagePath = CCString::createWithFormat("battle/fate/fate_%d_%d.png",i, j)->getCString();		
				CCLOG("开始加载英雄缘分动画 %s",_imagePath);
				m_nNumberOfSprites++;
				ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(_imagePath,CCString::createWithFormat("%s_%d_%d",FATE_SK, i, j)->getCString(),this,callfunc_selector(Battle::loadingCallBack),false);
			}
		}

		for(int i=1; i<4; i++)
		{
			_imagePath = CCString::createWithFormat("battle/fate/fate_buffer_%d.png",i)->getCString();		
			CCLOG("开始加载英雄缘分buff动画 %s",_imagePath);
			m_nNumberOfSprites++;
			ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(_imagePath,CCString::createWithFormat("%s_%d",FATE_BUFF_SK, i)->getCString(),this,callfunc_selector(Battle::loadingCallBack),false);
		}
	}

	if(m_pBattleModel->getBattleInfo()->triggerCoAction)
	{
		CCLOG("开始加载组合技 skillcover_2p.png");
		m_nNumberOfSprites++;
		ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("battle/skill/common/skillcover_2p.png",SKILL_COVER_2P_SK,this,callfunc_selector(Battle::loadingCallBack),false);
	}

    CCLOG("加载完毕");
}

void Battle::loadingCallBack()
{
    ++m_nNumberOfLoadedSprites;
    CCLOG("%d,%d,%d%%",m_nNumberOfLoadedSprites,m_nNumberOfSprites,(int)(((float)m_nNumberOfLoadedSprites / m_nNumberOfSprites) * 100));
    //m_pLoading->setString(CCString::createWithFormat("%d",(int)(((float)m_nNumberOfLoadedSprites / m_nNumberOfSprites) * 100))->getCString());
	m_pLoading->setLoadingPercent((float)m_nNumberOfLoadedSprites / m_nNumberOfSprites);
    if (m_nNumberOfLoadedSprites == m_nNumberOfSprites)
    {
		//this->runAction(CCSequence::create(CCDelayTime::create(0.0f),CCCallFunc::create(this, callfunc_selector(Battle::touchToBegin)),NULL));
//        this->runAction(CCSequence::create(CCDelayTime::create(0.0f),CCCallFunc::create(this, callfunc_selector(Battle::hideLoading)),NULL));
        this->runAction(CCSequence::create(CCDelayTime::create(0.0f),CCCallFunc::create(this, callfunc_selector(Battle::startBattle)),NULL));
        //startBattle();
    }
}

void Battle::showLoading()
{
    m_pLoading = LoadingLayer::sharedLoadingLayer();
	m_pLoading->setDelegate(this);
    addChild(m_pLoading);
}

void Battle::hideLoading()
{
	if (m_pLoading)
	{
		m_pLoading->removeFromParent();
		m_pLoading->setDelegate(NULL);
		m_pLoading = NULL;
	}
	
    startBattle();
}

void Battle::touchToBegin()
{
	m_pLoading->setMenuEnabled(true);
}

void Battle::loadingLayerTouched(LoadingLayer *pLayer)
{
	hideLoading();
}

void Battle::skipBattle()
{
//#if QSMY_DEBUG
//    endBattle();
//    return;
//#endif
    switch (m_kSkipType)
    {
        case battle::kSkipBattleTypePVE:
		case battle::kSkipBattleTypeTeam:
            Tips_Alert(D_LOCAL_STRING("pveSkipBattle").c_str());
            break;
        case battle::kSkipBattleTypeTower:
            Tips_Alert(D_LOCAL_STRING("towerSkipBattle").c_str());
            break;
        case battle::kSkipBattleTypePVP:
            //Tips_Alert(D_LOCAL_STRING("pvpSkipBattle").c_str());
			endBattle();
            break;
        case battle::kSkipBattleTypeOther:
            Tips_Alert(D_LOCAL_STRING("otherSkipBattle").c_str());
            break;
        case battle::kSkipBattleTypeWonder:
            //Tips_Alert(D_LOCAL_STRING("wonderSkipBattle").c_str());
            //break;
        case battle::kSkipBattleTypeSkip:
        case battle::kSkipBattleTypeMatch:
        default:
            endBattle();
            break;
    }
}

bool Battle::canSkipBattle()
{
    switch (m_kSkipType)
    {
		case battle::kSkipBattleTypePVE:
		case battle::kSkipBattleTypeTeam:
        case battle::kSkipBattleTypeTower:
        
        case battle::kSkipBattleTypeOther:
            return false;
		case battle::kSkipBattleTypePVP:
        case battle::kSkipBattleTypeWonder:
        case battle::kSkipBattleTypeSkip:
        case battle::kSkipBattleTypeMatch:
        default:
            return true;
    }
    return false;
}

void Battle::endBattle()
{
    m_kSkipType = battle::kSkipBattleTypeSkip;
    stopAllActions();
    m_pView->reset();
    if (!m_pBattleModel->getIsShowResult()) {
        this->endBattle(NULL);
        return;
    }
    if(m_pBattleModel->getBattleInfo()->isWin())
    {
		m_pView->removeStar(m_pBattleModel->getBattleInfo()->grade);
        Win* pWin = Win::create(m_pBattleModel->getBattleInfo()->grade);
		//popup自适应
		PL_MOVE_NODE_BELOW_CENTER_Y(pWin);
        pWin->setCallBack(this, callfuncO_selector(Battle::endBattle));
        m_pView->playEffect(pWin);
    }else
    {
		m_pView->removeStar();
        Lose* pLose = Lose::create();
		//popup自适应
		PL_MOVE_NODE_BELOW_CENTER_Y(pLose);
        pLose->setCallBack(this, callfuncO_selector(Battle::endBattle));
        m_pView->playEffect(pLose);
		DM_GET_SOUND_MODEL->playCommonEffectSound("lose", 0);
    }
}

void Battle::endBattle(CCObject*)
{
	//
    m_pScheduler->setTimeScale(1.0f);  //恢复播放速度
    SoundManager::sharedSoundManager()->playMainUISound();
    cleanup();
    //战后结算 战斗结算按钮 发送消息
	/************************************************************************/
	/* 將以下一句代碼註釋，打完留著戰鬥背景                                               */
	/************************************************************************/
    //removeAllChildren();
    

	//this->runAction(CCSequence::create(CCDelayTime::create(0.0f),CCCallFunc::create(this, callfunc_selector(Battle::showBusyLayer)),/*CCDelayTime::create(0.1f),CCCallFunc::create(this, callfunc_selector(Battle::showSettlement)),*/NULL));
	//this->runAction(CCSequence::create(CCDelayTime::create(0.5f),CCCallFunc::create(this, callfunc_selector(Battle::showSettlement)),NULL));
	//HTTP_CLIENT->getBusyLayer()->showBusy();
	showSettlement();
	//m_pSettle->setVisible(true);
}

void Battle::showBusyLayer()
{
	HTTP_CLIENT->getBusyLayer()->showBusy();
}

void Battle::replayBattle()
{
    removeAllChildren();
    showLoading();
	//loadBattleResource();
	this->runAction(CCSequence::create(CCDelayTime::create(0.0f),CCCallFunc::create(this, callfunc_selector(Battle::startBattle)),NULL));
}

void Battle::quitBattle()
{
	removeAllChildren();
	m_pSettle = NULL;
    ResourceManager::sharedResourceManager()->clearCache();
    m_pBattleModel->cleanup();
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_END_BATTLE);
}

void Battle::showOtherFormation(unsigned int tid)
{
	//退出结算界面，并显示对手阵容（机器人不显示）
	DM_GET_PVP_MODEL->setTargetID(tid);
	quitBattle();
}

void Battle::startBattle()
{
	if (m_pLoading)
	{
		m_pLoading->removeFromParent();
		m_pLoading->setDelegate(NULL);
		m_pLoading = NULL;
		LoadingLayer::purgeLoadingLayer();
	}
	
	

    SoundManager::sharedSoundManager()->playBattleSound();
    m_pScheduler->setTimeScale(1.0f); //FIXME 调整整体播放速度
    //scheduleUpdate();
    currentActionIndex=0;
    currentDialogIndex=0;
    
    m_pView = BattleView::create();
    m_pView->createSkipButton(canSkipBattle());
    m_pView->setDelegate(this);
    m_pView->setAnchorPoint(CCPointZero);
    m_pView->setPosition(CCPointZero);
    addChild(m_pView);
    //startBattle();
    
	if(DM_GET_BATTLE_MODEL->getShowFateAni() && SystemUtil::isNBDevice()){
		runAction(CCSequence::create(CCDelayTime::create(1.f), CCCallFunc::create(this, callfunc_selector(Battle::startFateEffect)), NULL
			));
	} else {
		beforeStartBattle();
	}
    //playNextAction();
}

void Battle::startFateEffect()
{
	m_pView->playFateEffect();
}

void Battle::beforeStartBattle()
{
	if (m_pBattleModel->getIsShowStart()) {
		BattleStart* pBattleStart = BattleStart::create();
		pBattleStart->setCallBack(this, callfuncO_selector(Battle::startBattle));
		m_pView->playEffect(pBattleStart);
	}else
	{
		startBattle(NULL);
	}    
}

void Battle::startBattle(CCObject*)
{
    CCLog("Battle Total:");
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfoShort();
//    CCActionInterval * delay = CCDelayTime::create(0.5f);
//    CCCallFunc * call = CCCallFunc::create(this, callfunc_selector(Battle::playNextDialog));
//    this->runAction(CCSequence::create(delay,call,NULL));
    playNextDialog();
}

void Battle::playNextDialog()
{
    if(!m_pDialogs || currentDialogIndex>=m_pDialogs->count())
    {
        m_pView->hideDialog();
        playNextAction();
    }else
    {
        playDialog((BattleDialogInfo*)m_pDialogs->objectAtIndex(currentDialogIndex++));
    }
}

void Battle::playDialog(battle::BattleDialogInfo* pInfo)
{
    //playNextDialog();
    m_pView->playDialog(pInfo);
}


void Battle::playNextAction(float delay)
{
    CCAction *pAction = CCSequence::create(CCDelayTime::create(delay),
                                           CCCallFunc::create(this, callfunc_selector(Battle::playNextAction)),
                                           NULL);
    pAction->setTag(NEXT_ACTION_TAG);
    this->runAction(pAction);
}


void Battle::playNextAction()
{
    this->stopActionByTag(NEXT_ACTION_TAG);
    CCLOG("播放第%d个动作",currentActionIndex);
    if(currentActionIndex>=m_pBattleModel->getBattleInfo()->m_pActions->count())
    {
        endBattle();
        
        return;
    }
    playAction((Action*)m_pBattleModel->getBattleInfo()->getActionByIndex(currentActionIndex++));
}

void Battle::playAction(battle::Action *pAction)
{
    switch (pAction->type)
    {
        case kActionAttack:
            playAttack((ActionAttack*)pAction);
            break;
        case kActionSkill:
            playSkill((ActionSkill*)pAction);
            break;
        case kActionBuff:
            playBuff((ActionBuff*)pAction);
            break;
        default:
            CCLOG("不明类型的动作！");
            playNextAction();
            break;
    }
}


void Battle::playAttack(ActionAttack * pAction)
{
    warrior::WarriorBaseInfo *pUnitInfo = m_pUnitModel->getWarriorBaseByWID(m_pBattleModel->getBattleInfo()->getUnit(pAction->actor)->sid);
    
    if(pUnitInfo==NULL)
    {
        CCLOG("非法的单位编号%d",pAction->actor);
        Battle::playNextAction();
        return;
    }
    
    CCLOG("%d进行一次普攻",pAction->actor);
    
    UnitView* pUnitView = m_pView->getUnit(pAction->actor);
    pUnitView -> playAction(UnitView::ATTACK,0.0f,true);
    
    //普攻只会造成一个结果 就是伤害
    float delay=pUnitInfo->getUnitAttackDmgDelay();
    float actionDelay = delay;
    
    //Action* pNextAction = m_pBattleModel->getBattleInfo()->getActionByIndex(currentActionIndex+1);
    //if(pNextAction && pResultDamage->actor!= pNextAction->actor) //如果被打者不是下回合释放者
    //{
    //    playDamage(pResultDamage,delay);
    //    actionDelay +=0.4f; //+x固定下回合开始间隔 加快战斗节奏
    //}else
    //{
    //    actionDelay = playDamage(pResultDamage,delay);
    //}
	uint8_t actor = 0;
	battle::Result* pResult = NULL;
	for (uint32_t i=0; i<pAction->getResults()->count(); i++)
	{
		pResult = (battle::Result*)pAction->getResults()->objectAtIndex(i); //数组是按单位编号排序的
		if(i==0) actor=pResult->actor;
		if(actor!=pResult->actor)   //如果人变了 累加unit_delay;
		{
			actor = pResult->actor;
			actionDelay +=0.4f;
		}
		actionDelay=playAttackResult(pResult,actionDelay);
	}
    
    playNextAction(actionDelay);
    
}



void Battle::playBuff(ActionBuff * pAction)
{
    CCLOG("%d的buff生效",pAction->actor);
    
    if(pAction->buff_id == buff::KBuffTypeVertigo)
    {
        playNextAction(0.0f);
        return;//眩晕不会造成伤害 也不会播放效果
    }
    UnitView* pUnitView;
    pUnitView = m_pView->getUnit(pAction->actor);
    pUnitView ->playBuffAction(pAction->buff_id,0.0f); //播放Buff效果
    
    float delay = 0.0f;
    if(pAction->getResults()->count()>0)
    {
        delay = 0.2f; //FIXME: 暂定0.2秒 或者上面动画返回时间??
		Result* pResult = (battle::Result*)pAction->getResults()->objectAtIndex(0);
		if(pResult->type == kResultHeal)
		{
			delay = playHeal((ResultHeal*)pResult, delay);
		} else 
		{
			delay = playDamage((ResultDamage*)pResult,delay);
		}
        
    }
    playNextAction(delay);
}



void Battle::playSkill(ActionSkill *pAction)
{
    battle::Skill *pSkill = m_pBattleModel->getSkill(pAction->skill_id);
    if(pSkill==NULL)
    {
        CCLOG("非法的技能编号%d",pAction->skill_id);
        Battle::playNextAction();
        return;
    }
    
    CCLOG("%d释放一个技能%d",pAction->actor,pSkill->getSid());
    
    float delay = 0.8f;
    float actionDelay = delay;
    float tempD = actionDelay;
    
    if(pSkill->screen_cover)//如果遮挡屏幕
    {
        
        UnitView* pUnitView = m_pView->getUnit(pAction->actor);
        pUnitView -> playAction(UnitView::CONJURE_BEGIN,0.0f,true);
        
        if(pSkill->getSkillInfo()->grade > skill::kSkillGradeBlue || pAction->m_pCoactors->size()>0) //品质大于蓝色 或者是合体技能
        {
            delay = m_pView->playSkillCover(pAction,delay);
        }else
        {
            /*去掉计算小技能字幕等待时间 delay = */pUnitView->playSkillCover(pAction,delay);
            delay+=0.8f; //FIXME: 固定小技能字幕时间0.8秒
        }
        
        float coactor_delay = 0.5f; //FIXME: 组合技其他队友施放间隔 临时写x秒 //coactor_delay*4 不应该大于4人cover的动画时间
        
        for (uint32_t i=0; i<pAction->m_pCoactors->size(); i++) {
            UnitView* pUnitView = m_pView->getUnit(pAction->m_pCoactors->at(i));
			if(!pUnitView) continue;
            pUnitView->playAction(UnitView::CONJURE_BEGIN,(float)coactor_delay*(i+1),true);
        }
    }
    
    
    if(pSkill->action) //如果有技能动作
    {
        UnitView* pUnitView = m_pView->getUnit(pAction->actor);
        tempD = pUnitView->playAction(UnitView::CONJURE_BEGIN,delay,true);
		pUnitView->playSkillCover(pAction,delay);
        if(tempD>actionDelay) actionDelay = tempD;
    }
    
    if(pSkill->skill_delay>=0)
    {
        delay += pSkill->skill_delay;
        tempD = m_pView->playSkill(pAction,delay);
        if(tempD>actionDelay) actionDelay = tempD;
    }
    
    if(pSkill->effect_delay>=0)
    {
        delay += pSkill->effect_delay;
        tempD = m_pView->playSkillEffect(pAction,delay);
        if(tempD>actionDelay) actionDelay = tempD;
    }
    
    battle::Result* pResult = NULL;
    
    if(pSkill->unit_effect_delay>=0)
    {
        //delay += pSkill->unit_effect_delay; //伤害delay不应该计算unit_effect_delay
        
        float unit_effect_delay = delay+pSkill->unit_effect_delay;
        uint8_t actor = 0;
        for (uint32_t i=0; i<pAction->getResults()->count(); i++)
        {
            pResult = (battle::Result*)pAction->getResults()->objectAtIndex(i);
            if(i==0) actor=pResult->actor;
            if(actor!=pResult->actor)   //如果人变了 累加unit_delay;
            {
                actor = pResult->actor;
                unit_effect_delay += pSkill->unit_delay;
            }
            if(pSkill->skill_type == skill::kSkillDmg && pResult->type != kResultDamage) continue;  //如果是伤害技能 但不是伤害结果 不播放unit effect
            if(pSkill->skill_type == skill::kSkillHeal && pResult->type != kResultHeal) continue;   //如果是治疗技能 但不是治疗结果 不播放unit effect
            UnitView* pUnitView = m_pView->getUnit(pResult->actor);               //取得作用单位
            tempD = pUnitView -> playUnitEffect(pSkill,unit_effect_delay); //播放单位效果
            if(tempD>actionDelay) actionDelay = tempD;
            
        }
    }
    
    uint8_t actor = 0;
    float result_delay = delay;
    bool autoPlayNextAction(true);

    for (uint32_t i=0; i<pAction->getResults()->count(); i++)
    {
        
        pResult = (battle::Result*)pAction->getResults()->objectAtIndex(i); //数组是按单位编号排序的
        if(i==0) actor=pResult->actor;
        if(actor!=pResult->actor)   //如果人变了 累加unit_delay;
        {
            actor = pResult->actor;
            result_delay+= pSkill->unit_delay;
        }
        tempD=playSkillResult(pSkill,pResult,result_delay);
        if(D_FLOAT_EQUALS(PAUSE_DIALOG_DELAY, tempD))
        {
            autoPlayNextAction = false;
        }else
        {
            if(tempD>actionDelay) actionDelay = tempD;
        }
    }

	if (pSkill->visible && !DM_GET_SOUND_MODEL->isSkillNoSound(pAction->skill_id))
	{

		uint32_t wid = m_pBattleModel->getBattleInfo()->getUnit(pAction->actor)->sid;
		if (pSkill->getSkillInfo()->isSpecial())
		{
			DM_GET_SOUND_MODEL->playSpecialSkillSound(wid);
		}
		else
		{
			DM_GET_SOUND_MODEL->playSkillKillSound(wid);
		}
	}
	
    resetSkillAction(pAction,actionDelay);
    if(autoPlayNextAction)playNextAction(actionDelay);
}


void Battle::resetSkillAction(ActionSkill *pAction,float delay)
{
    this->stopActionByTag(RESET_SKILL_ACTION_TAG);
    CCSequence* pCCAction = CCSequence::create(CCDelayTime::create(delay),
                       CCCallFuncO::create(this, callfuncO_selector(Battle::resetSkillAction),pAction),
                       NULL);
    pCCAction->setTag(RESET_SKILL_ACTION_TAG);
    this->runAction(pCCAction);
}

void Battle::resetSkillAction(CCObject* obj)
{
    ActionSkill *pAction = (ActionSkill *)obj;
    UnitView* pUnitView = m_pView->getUnit(pAction->actor);
    pUnitView -> playAction(UnitView::STAND);
    
    for (uint32_t i=0; i<pAction->m_pCoactors->size(); i++) {
        UnitView* pUnitView = m_pView->getUnit(pAction->m_pCoactors->at(i));
		if(!pUnitView) continue;
        pUnitView->playAction(UnitView::STAND);
    }
    
}

float Battle::playAttackResult(Result *pResult,float delay)
{
	switch (pResult->type) {
	case kResultDamage:
	{
		ResultDamage* pResultDamage = (ResultDamage*)pResult;
		
		UnitView* pUnitView = m_pView->getUnit(pResultDamage->actor);
		pUnitView -> playUnitEffect(pResultDamage,delay); //播放单位效果
		return playDamage(pResultDamage,delay);
	}
	case kResultHeal:
		return playHeal((ResultHeal*)pResult,delay);
	case kResultAddBuff:
		return playAddBuff((ResultAddBuff*)pResult,delay);
	default:
		return 0;
	}
}

float Battle::playSkillResult(battle::Skill *pSkill,Result *pResult,float delay)
{
    switch (pResult->type) {
        case kResultAddBuff:
            return playAddBuff(pSkill,(ResultAddBuff*)pResult,delay);
        case kResultRemoveBuff:
            return playRemoveBuff((ResultRemoveBuff*)pResult,delay);
        case kResultDamage:
            return playSkillDamage(pSkill,(ResultDamage*)pResult,delay);
        case kResultHeal:
            return playSkillHeal(pSkill,(ResultHeal*)pResult,delay);
        case kResultShow:
            return playSkillShowUnit(pSkill,(ResultShow*)pResult,delay);
        case kResultHide:
            return playSkillHideUnit(pSkill,(ResultHide*)pResult,delay);
        case kResultDialog:
            return playSkillDialog(pSkill,(ResultDialog*)pResult,delay);
        default:
            return 0;
    }
}

float Battle::playSkillDamage(battle::Skill *pSkill,battle::ResultDamage *pResult,float delay)
{
    
    float retDelay = 0.0f;
    float tempD = 0.0f;
    
    battle::DmgDelay* pDmgDelay = NULL;
    battle::ResultDamage *pResultClone = NULL;
    uint32_t count = pSkill->dmg_delay->count();
    uint32_t dmg = 0;
    for (uint32_t i=0;i<count; ++i)
    {
        pDmgDelay = (battle::DmgDelay*)pSkill->dmg_delay->objectAtIndex(i);
        delay += pDmgDelay->delay;
        pResultClone = new battle::ResultDamage(*pResult);
        if(i==count-1)
        {
            pResultClone->damange -= dmg;   //如果是最后一次伤害 修正伤害值
        }
        else
        {
            pResultClone->damange *= pDmgDelay->percent;
            dmg+= pResultClone->damange;
        }
        
        tempD = playDamage(pResultClone, delay);
        if(tempD>retDelay) retDelay = tempD;
        pResultClone->autorelease();
        
    }
    
    return retDelay;
}


float Battle::playDamage(ResultDamage *pResult,float delay)
{
    
    UnitView* pUnitView = m_pView->getUnit(pResult->actor);
    
    //扣血
    pUnitView ->setHp(pUnitView->getHp()-pResult->damange,delay);
    
    std::string animation;
    //播放伤害动画
    if(pResult->is_block) //是否播放格挡动画
    {
        animation = UnitView::PARRY;
    }else if(pResult->is_dodge)
    {
        animation = UnitView::DODGE;
    }else
    {
        animation = UnitView::HARD_STRAIGHT;
    }
    
    animation = pUnitView->getHp()>0 ? animation : UnitView::DEATH;
    
    //播放跳数字动画
    pUnitView->playNum(pResult,delay);
    
    //播放伤害动画
    float rtDelay = pUnitView -> playAction(animation.c_str(),delay,true);
    
    if(pUnitView->getHp()<=0) pUnitView->setDead();
    return rtDelay;
}


float Battle::playSkillHeal(battle::Skill *pSkill, battle::ResultHeal *pResult, float delay)
{
    battle::DmgDelay* pDmgDelay = (DmgDelay*)pSkill->dmg_delay->objectAtIndex(0); //加血技能只有一波效果
    playHeal(pResult,delay+pDmgDelay->delay);
    return delay;
}

float Battle::playSkillShowUnit(battle::Skill *pSkill, battle::ResultShow *pResult, float delay)
{
    UnitView* pUnitView = m_pView->getUnit(pResult->actor);
    return pUnitView->playShow(delay+pSkill->getTotalDmgDelay());
}


float Battle::playSkillHideUnit(battle::Skill *pSkill, battle::ResultHide *pResult, float delay)
{
    UnitView* pUnitView = m_pView->getUnit(pResult->actor);
    return pUnitView->playHide(delay+pSkill->getTotalDmgDelay());
}

float Battle::playSkillDialog(battle::Skill *pSkill, battle::ResultDialog *pResult, float delay)
{
    CC_SAFE_RELEASE(m_pDialogs);
    m_pDialogs = m_pBattleModel->getUnitDialog(pResult->dialog_id);
    CC_SAFE_RETAIN(m_pDialogs);
    currentDialogIndex = 0;
    if(m_pDialogs && m_pDialogs->count()>0)
    {
        this->runAction(CCSequence::create(CCDelayTime::create(delay+pSkill->getTotalDmgDelay()),CCCallFunc::create(this, callfunc_selector(Battle::playUnitDialog)) ,NULL));
        return PAUSE_DIALOG_DELAY;
    }
    return delay;
}

void Battle::playUnitDialog()
{
    playNextDialog();
}


float Battle::playHeal(ResultHeal *pResult,float delay)
{
    
    UnitView* pUnitView = m_pView->getUnit(pResult->actor);
    
    //加血
    pUnitView ->setHp(pUnitView->getHp() + pResult->heal,delay);
    
    //播放跳数字动画
    pUnitView->playNum(pResult,delay);
	CCString* pAction = CCString::create(EFFECT_HEAL_SOUND);
    runAction(CCSequence::create(CCDelayTime::create(delay), CCCallFuncO::create(pUnitView, callfuncO_selector(Battle::playSound), pAction), NULL));
    //播放伤害动画
    return delay;
    
}

void Battle::playSound(CCObject* obj)
{
	CCString* pAction = (CCString*)obj;
	std::string sid = EFFECT_SK;
	DM_GET_SOUND_MODEL->playEffectSound(sid, 0, pAction->getCString());
}

float Battle::playAddBuff(ResultAddBuff *pResult,float delay)
{
	UnitView* pUnitView = m_pView->getUnit(pResult->actor);
	if(pResult->buff_id==buff::KBuffTypeVertigo)
	{
		pUnitView->playAction(UnitView::VERTIGO,delay);
		return delay;
	}
	return pUnitView->playAddBuff(pResult->buff_id, delay);
}

float Battle::playAddBuff(battle::Skill *pSkill,ResultAddBuff *pResult,float delay)
{
    UnitView* pUnitView = m_pView->getUnit(pResult->actor);
    if(pResult->buff_id==buff::KBuffTypeVertigo)
    {
        pUnitView->playAction(UnitView::VERTIGO,delay+pSkill->buff_delay);
        return delay+pSkill->buff_delay;
    }
    return pUnitView->playAddBuff(pResult->buff_id, delay+pSkill->buff_delay);
}

float Battle::playRemoveBuff(ResultRemoveBuff *pResult,float delay)
{
    //TODO 目前做法不需要表现  直接把对应的buff移除 如果需要表现需要在addBuff中创建一个buff管理列表 退出战斗清空
    return 0;
}


#pragma mark - settlement -
void Battle::showSettlement()
{
    //ResourceManager::sharedResourceManager()->clearCache();
    m_pSettle = NULL;
    if(m_pBattleModel->getSettlementInfo() && (m_pSettle = SettlementView::create(this,m_pBattleModel->getSettlementInfo())))
    {
		DM_GET_SOUND_MODEL->playCommonEffectSound("Reward", 0);
        addChild(m_pSettle);
    }else
    {
        quitBattle(); //没有结算
    }
	//HTTP_CLIENT->getBusyLayer()->hideBusy();
	

	//if(m_pSettle){
	//	CC_SAFE_RELEASE(m_pSettle);//->release();
	//	addChild(m_pSettle);
	//}
	//else{
	//	quitBattle(); //没有结算
	//}
}

void Battle::keyBackClicked()
{
	if (DM_GET_TUTORIAL_MODEL->getIsRunning())
	{
		SystemUtil::quitGame();
		return;
	}
	
	if (m_pSettle)
	{
		m_pSettle->closeView();
	}
}