//
//  DataManager.cpp
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DataManager.h"


static DataManager *s_SharedDataManager = NULL;


DataManager* DataManager::sharedDataManager(void)
{
    if(!s_SharedDataManager)
    {
        s_SharedDataManager = new DataManager();
        s_SharedDataManager->init();
    }
    return s_SharedDataManager;
}


void DataManager::purgeSharedDataManager(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedDataManager);
}

#define CREATE_MODEL(_m_,cls) \
{\
    _m_ = new cls;m_pDatas->addObject(_m_);CC_SAFE_RELEASE(_m_);   \
}\

DataManager::DataManager(void)
:m_pDatas(NULL)
,m_pUserModel(NULL)
,m_pBattleModel(NULL)
,m_pSkillModel(NULL)
,m_pPveModel(NULL)
,m_pFormationModel(NULL)
,m_pWarriorModel(NULL)
,m_pItemModel(NULL)
,m_pEquipModel(NULL)
,m_pMatchModel(NULL)
,m_pPvpModel(NULL)
,m_pDropModel(NULL)
,m_pBuffModel(NULL)
,m_pDestinyModel(NULL)
,m_pMallModel(NULL)
,m_pOtherFormationModel(NULL)
,m_pNavigationModel(NULL)
,m_pLoginRewardModel(NULL)
,m_pSettingModel(NULL)
,m_pMailModel(NULL)
,m_pLotteryModel(NULL)
,m_pSettlementModel(NULL)
,m_pTutorialModel(NULL)
,m_pWishModel(NULL)
,m_pPrizeModel(NULL)
,m_pPlatformModel(NULL)
,m_pNoticeModel(NULL)
,m_pAnnounceModel(NULL)
,m_pGuildModel(NULL)
,m_pSoulHunterModel(NULL)
,m_pSoundModel(NULL)
,m_pConfigModel(NULL)
,m_pGuildBossModel(NULL)
,m_pAvatarModel(NULL)
,m_pActivityModel(NULL)
,m_pErrorTipModel(NULL)
,m_pFriendsModel(NULL)
,m_pChatModel(NULL)
,m_pFilterModel(NULL)
,m_pShareModel(NULL)
,m_pActivenessModel(NULL)
,m_pTowerModel(NULL)
,m_pSigninModel(NULL)
,m_pAchievementModel(NULL)
,m_pGundamEFModel(NULL)
,m_pWeeklyModel(NULL)
{
    
}

DataManager::~DataManager(void)
{
    CC_SAFE_RELEASE(m_pDatas);
    CCLOG("deallocing DataManager %p", this);
}


bool DataManager::init(void)
{
    m_pDatas                = cocos2d::CCArray::create();
    CC_SAFE_RETAIN(m_pDatas);
    
    CREATE_MODEL(m_pUserModel,UserModel);
    CREATE_MODEL(m_pSkillModel,SkillModel);
    CREATE_MODEL(m_pBattleModel,BattleModel);
    CREATE_MODEL(m_pPveModel,PveModel);
    CREATE_MODEL(m_pFormationModel,FormationModel);
	CREATE_MODEL(m_pWarriorModel  ,WarriorModel);
	CREATE_MODEL(m_pItemModel     ,ItemModel);
	CREATE_MODEL(m_pEquipModel    ,EquipModel);
	CREATE_MODEL(m_pMatchModel    ,MatchModel);
	CREATE_MODEL(m_pPvpModel      ,PvpModel);
	CREATE_MODEL(m_pDropModel     ,DropModel);
    CREATE_MODEL(m_pBuffModel     ,BuffModel);
    CREATE_MODEL(m_pDestinyModel  ,DestinyModel);
	CREATE_MODEL(m_pMallModel     ,MallModel);
    CREATE_MODEL(m_pActivenessModel,ActivenessModel);
    CREATE_MODEL(m_pOtherFormationModel,OtherFormationModel);
    CREATE_MODEL(m_pNavigationModel,NavigationModel);
	CREATE_MODEL(m_pTowerModel    ,TowerModel);
    CREATE_MODEL(m_pLoginRewardModel ,LoginRewardModel);
    CREATE_MODEL(m_pSettingModel  ,SettingModel);
	CREATE_MODEL(m_pMailModel     ,MailModel);
    CREATE_MODEL(m_pLotteryModel  ,LotteryModel);
    CREATE_MODEL(m_pSettlementModel,SettlementModel);
    CREATE_MODEL(m_pTutorialModel ,TutorialModel);
    CREATE_MODEL(m_pWishModel     ,WishModel);
    CREATE_MODEL(m_pPrizeModel    ,PrizeModel);
    CREATE_MODEL(m_pPlatformModel ,PlatformModel);
    CREATE_MODEL(m_pNoticeModel   ,NoticeModel);
    CREATE_MODEL(m_pAnnounceModel ,AnnounceModel);
    CREATE_MODEL(m_pGuildModel ,GuildModel);
    CREATE_MODEL(m_pSoulHunterModel ,SoulHunterModel);
	CREATE_MODEL(m_pSoundModel, SoundModel);
	CREATE_MODEL(m_pConfigModel, ConfigModel);
	CREATE_MODEL(m_pGuildBossModel, GuildBossModel);
    CREATE_MODEL(m_pAvatarModel, AvatarModel);
	CREATE_MODEL(m_pActivityModel, ActivityModel);
    CREATE_MODEL(m_pErrorTipModel, ErrorTipModel);
	CREATE_MODEL(m_pFriendsModel, FriendsModel);
	CREATE_MODEL(m_pFriendModel, FriendModel);
	CREATE_MODEL(m_pChatModel, ChatModel);
	CREATE_MODEL(m_pFilterModel, FilterModel);
    CREATE_MODEL(m_pShareModel, ShareModel);
	CREATE_MODEL(m_pSigninModel, SigninModel);
	CREATE_MODEL(m_pAchievementModel, AchievementModel);
	CREATE_MODEL(m_pBurnModel, BurnModel);
	CREATE_MODEL(m_pRankModel, RankModel);
    CREATE_MODEL(m_pKingTombModel, KingTombModel);
	CREATE_MODEL(m_pGundamEFModel, GundamEngineerFModel);
	CREATE_MODEL(m_pWeeklyModel, WeeklyModel);

	CCLOG("%s","DataManager::init");
    return true;
}

void DataManager::resetData()
{
    cocos2d::CCObject* obj = NULL;
    Model* pModel = NULL;
    CCARRAY_FOREACH(m_pDatas, obj)
    {
        pModel = static_cast<Model*>(obj);
        if(pModel->getInited()) pModel->reset();
    }
}

