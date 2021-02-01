//
//  DataManager.h
//  一个单例的数据管理类
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DataManager__
#define __QSMY__DataManager__

#include "AppInclude.h"
#include "model/BattleModel.h"
#include "model/SkillModel.h"
#include "model/PveModel.h"
#include "model/FormationModel.h"
#include "model/WarriorModel.h"
#include "model/ItemModel.h"
#include "model/EquipModel.h"
#include "model/UserModel.h"
#include "model/MatchModel.h"
#include "model/PvpModel.h"
#include "model/DropModel.h"
#include "model/BuffModel.h"
#include "model/DestinyModel.h"
#include "model/MallModel.h"
#include "model/ActivenessModel.h"
#include "model/OtherFormationModel.h"
#include "model/NavigationModel.h"
#include "model/TowerModel.h"
#include "model/LoginRewardModel.h"
#include "model/SettingModel.h"
#include "model/MailModel.h"
#include "model/LotteryModel.h"
#include "model/SettlementModel.h"
#include "model/TutorialModel.h"
#include "model/WishModel.h"
#include "model/PrizeModel.h"
#include "model/PlatformModel.h"
#include "model/NoticeModel.h"
#include "model/AnnounceModel.h"
#include "model/GuildModel.h"
#include "model/SoulHunterModel.h"
#include "model/SoundModel.h"
#include "model/ConfigModel.h"
#include "model/GuildBossModel.h"
#include "model/AvatarModel.h"
#include "model/ActivityModel.h"
#include "model/ErrorTipModel.h"
#include "model/FriendsModel.h"
#include "model/FriendModel.h"
#include "model/ChatModel.h"
#include "model/FilterModel.h"
#include "model/ShareModel.h"
#include "model/SigninModel.h"
#include "model/AchievementModel.h"
#include "model/BurnModel.h"
#include "model/RankModel.h"
#include "model/KingTombModel.h"
#include "model/GundamEngineerFModel.h"
#include "model/WeeklyModel.h"

#define CC_SYNTHESIZE_MODEL_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const {\
    if(!varName->getInited()) varName->init();  \
    return varName;\
}\


/************************************************************************/
/*                         Data model definition                        */
/************************************************************************/

#define DM_GET_USER_MODEL			(DataManager::sharedDataManager()->getUserModel())
#define DM_GET_WARRIOR_MODEL		(DataManager::sharedDataManager()->getWarriorModel())
#define DM_GET_FORMATION_MODEL		(DataManager::sharedDataManager()->getFormationModel())
#define DM_GET_DESTINY_MODEL		(DataManager::sharedDataManager()->getDestinyModel())
#define DM_GET_ITEM_MODEL			(DataManager::sharedDataManager()->getItemModel())
#define DM_GET_BATTLE_MODEL			(DataManager::sharedDataManager()->getBattleModel())
#define DM_GET_SKILL_MODEL			(DataManager::sharedDataManager()->getSkillModel())
#define DM_GET_EQUIP_MODEL			(DataManager::sharedDataManager()->getEquipModel())
#define DM_GET_PVE_MODEL			(DataManager::sharedDataManager()->getPveModel())
#define DM_GET_MATCH_MODEL			(DataManager::sharedDataManager()->getMatchModel())
#define DM_GET_PVP_MODEL			(DataManager::sharedDataManager()->getPvpModel())
#define DM_GET_DROP_MODEL			(DataManager::sharedDataManager()->getDropModel())
#define DM_GET_BUFF_MODEL			(DataManager::sharedDataManager()->getBuffModel())
#define DM_GET_MALL_MODEL			(DataManager::sharedDataManager()->getMallModel())
#define DM_GET_OTHERFORMATION_MODEL	(DataManager::sharedDataManager()->getOtherFormationModel())
#define DM_GET_ACTIVENESS_MODEL		(DataManager::sharedDataManager()->getActivenessModel())
#define DM_GET_NAVIGATION_MODEL		(DataManager::sharedDataManager()->getNavigationModel())
#define DM_GET_TOWER_MODEL			(DataManager::sharedDataManager()->getTowerModel())
#define DM_GET_LOGIN_REWARD_MODEL	(DataManager::sharedDataManager()->getLoginRewardModel())
#define DM_GET_SETTING_MODEL		(DataManager::sharedDataManager()->getSettingModel())
#define DM_GET_MAIL_MODEL			(DataManager::sharedDataManager()->getMailModel())
#define DM_GET_LOTTERY_MODEL		(DataManager::sharedDataManager()->getLotteryModel())
#define DM_GET_SETTLEMENT_MODEL		(DataManager::sharedDataManager()->getSettlementModel())
#define DM_GET_TUTORIAL_MODEL		(DataManager::sharedDataManager()->getTutorialModel())
#define DM_GET_WISH_MODEL           (DataManager::sharedDataManager()->getWishModel())
#define DM_GET_PRIZE_MODEL          (DataManager::sharedDataManager()->getPrizeModel())
#define DM_GET_PLATFORM_MODEL       (DataManager::sharedDataManager()->getPlatformModel())
#define DM_GET_NOTICE_MODEL         (DataManager::sharedDataManager()->getNoticeModel())
#define DM_GET_ANNOUNCE_MODEL       (DataManager::sharedDataManager()->getAnnounceModel())
#define DM_GET_GUILD_MODEL			(DataManager::sharedDataManager()->getGuildModel())
#define DM_GET_SOUL_HUNTER_MODEL	(DataManager::sharedDataManager()->getSoulHunterModel())
#define DM_GET_SOUND_MODEL			(DataManager::sharedDataManager()->getSoundModel())
#define DM_GET_CONFIG_MODEL			(DataManager::sharedDataManager()->getConfigModel())
#define DM_GET_GUILDBOSS_MODEL		(DataManager::sharedDataManager()->getGuildBossModel())
#define DM_GET_AVATAR_MODEL         (DataManager::sharedDataManager()->getAvatarModel())
#define DM_GET_ACTIVITY_MODEL		(DataManager::sharedDataManager()->getActivityModel())
#define DM_GET_ERRORTIP_MODEL		(DataManager::sharedDataManager()->getErrorTipModel())
#define DM_GET_FRIENDS_MODEL		(DataManager::sharedDataManager()->getFriendsModel())
#define DM_GET_CHAT_MODEL			(DataManager::sharedDataManager()->getChatModel())
#define DM_GET_FILTER_MODEL			(DataManager::sharedDataManager()->getFilterModel())
#define DM_GET_SHARE_MODEL			(DataManager::sharedDataManager()->getShareModel())
#define DM_GET_SIGNIN_MODEL			(DataManager::sharedDataManager()->getSigninModel())
#define DM_GET_ACHIEVEMENT_MODEL	(DataManager::sharedDataManager()->getAchievementModel())
#define DM_GET_BURN_MODEL	(DataManager::sharedDataManager()->getBurnModel())
#define DM_GET_FRIEND_MODEL		(DataManager::sharedDataManager()->getFriendModel())
#define DM_GET_RANK_MODEL		(DataManager::sharedDataManager()->getRankModel())
#define DM_GET_KINGTOMB_MODEL		(DataManager::sharedDataManager()->getKingTombModel())
#define DM_GET_GUNDAM_EF_MODEL		(DataManager::sharedDataManager()->getGundamEFModel())
#define DM_GET_WEEKLY_MODEL			(DataManager::sharedDataManager()->getWeeklyModel())

class DataManager : public cocos2d::CCObject 
{
	CC_SYNTHESIZE_MODEL_READONLY(UserModel*, m_pUserModel, UserModel);
    CC_SYNTHESIZE_MODEL_READONLY(BattleModel*, m_pBattleModel, BattleModel);
    CC_SYNTHESIZE_MODEL_READONLY(SkillModel*, m_pSkillModel, SkillModel);
    CC_SYNTHESIZE_MODEL_READONLY(PveModel*, m_pPveModel, PveModel);
    CC_SYNTHESIZE_MODEL_READONLY(FormationModel*, m_pFormationModel, FormationModel);
    CC_SYNTHESIZE_MODEL_READONLY(DestinyModel*, m_pDestinyModel, DestinyModel);
	CC_SYNTHESIZE_MODEL_READONLY(WarriorModel*, m_pWarriorModel, WarriorModel);
	CC_SYNTHESIZE_MODEL_READONLY(EquipModel*, m_pEquipModel, EquipModel);
	CC_SYNTHESIZE_MODEL_READONLY(ItemModel*, m_pItemModel, ItemModel);
	CC_SYNTHESIZE_MODEL_READONLY(MatchModel*, m_pMatchModel, MatchModel);
	CC_SYNTHESIZE_MODEL_READONLY(PvpModel*, m_pPvpModel, PvpModel);
	CC_SYNTHESIZE_MODEL_READONLY(DropModel*, m_pDropModel, DropModel);
    CC_SYNTHESIZE_MODEL_READONLY(BuffModel*, m_pBuffModel, BuffModel);
    CC_SYNTHESIZE_MODEL_READONLY(MallModel*, m_pMallModel, MallModel);
    CC_SYNTHESIZE_MODEL_READONLY(ActivenessModel*, m_pActivenessModel, ActivenessModel);
	CC_SYNTHESIZE_MODEL_READONLY(OtherFormationModel*, m_pOtherFormationModel, OtherFormationModel);
	CC_SYNTHESIZE_MODEL_READONLY(NavigationModel*, m_pNavigationModel, NavigationModel);
	CC_SYNTHESIZE_MODEL_READONLY(TowerModel*, m_pTowerModel, TowerModel);    
	CC_SYNTHESIZE_MODEL_READONLY(LoginRewardModel*, m_pLoginRewardModel, LoginRewardModel);
    CC_SYNTHESIZE_MODEL_READONLY(SettingModel*, m_pSettingModel, SettingModel);
    CC_SYNTHESIZE_MODEL_READONLY(MailModel*, m_pMailModel, MailModel);
    CC_SYNTHESIZE_MODEL_READONLY(LotteryModel*, m_pLotteryModel, LotteryModel);
    CC_SYNTHESIZE_MODEL_READONLY(SettlementModel*, m_pSettlementModel, SettlementModel);
    CC_SYNTHESIZE_MODEL_READONLY(TutorialModel*, m_pTutorialModel, TutorialModel);
    CC_SYNTHESIZE_MODEL_READONLY(WishModel*, m_pWishModel, WishModel);
    CC_SYNTHESIZE_MODEL_READONLY(PrizeModel*, m_pPrizeModel, PrizeModel);
    CC_SYNTHESIZE_MODEL_READONLY(PlatformModel*, m_pPlatformModel, PlatformModel);
    CC_SYNTHESIZE_MODEL_READONLY(NoticeModel*, m_pNoticeModel, NoticeModel);
	CC_SYNTHESIZE_MODEL_READONLY(AnnounceModel*, m_pAnnounceModel, AnnounceModel);
	CC_SYNTHESIZE_MODEL_READONLY(GuildModel*, m_pGuildModel, GuildModel);
	CC_SYNTHESIZE_MODEL_READONLY(SoulHunterModel*, m_pSoulHunterModel, SoulHunterModel);
	CC_SYNTHESIZE_MODEL_READONLY(SoundModel*, m_pSoundModel, SoundModel);
	CC_SYNTHESIZE_MODEL_READONLY(ConfigModel*, m_pConfigModel, ConfigModel);
	CC_SYNTHESIZE_MODEL_READONLY(GuildBossModel*, m_pGuildBossModel, GuildBossModel);
    CC_SYNTHESIZE_MODEL_READONLY(AvatarModel*, m_pAvatarModel, AvatarModel);
	CC_SYNTHESIZE_MODEL_READONLY(ActivityModel *, m_pActivityModel, ActivityModel);
    CC_SYNTHESIZE_MODEL_READONLY(ErrorTipModel *, m_pErrorTipModel, ErrorTipModel);
	CC_SYNTHESIZE_MODEL_READONLY(FriendsModel *, m_pFriendsModel, FriendsModel);
	CC_SYNTHESIZE_MODEL_READONLY(FriendModel *, m_pFriendModel, FriendModel);
	CC_SYNTHESIZE_MODEL_READONLY(ChatModel *, m_pChatModel, ChatModel);
	CC_SYNTHESIZE_MODEL_READONLY(FilterModel *, m_pFilterModel, FilterModel);
	CC_SYNTHESIZE_MODEL_READONLY(ShareModel *, m_pShareModel, ShareModel);
	CC_SYNTHESIZE_MODEL_READONLY(SigninModel *, m_pSigninModel, SigninModel);
	CC_SYNTHESIZE_MODEL_READONLY(AchievementModel *, m_pAchievementModel, AchievementModel);
	CC_SYNTHESIZE_MODEL_READONLY(BurnModel *, m_pBurnModel, BurnModel);
	CC_SYNTHESIZE_MODEL_READONLY(RankModel *, m_pRankModel, RankModel);
    CC_SYNTHESIZE_MODEL_READONLY(KingTombModel *, m_pKingTombModel, KingTombModel);
	CC_SYNTHESIZE_MODEL_READONLY(GundamEngineerFModel*, m_pGundamEFModel, GundamEFModel);
	CC_SYNTHESIZE_MODEL_READONLY(WeeklyModel *, m_pWeeklyModel, WeeklyModel);
public:    
    /** returns a shared instance of the dataManager */
    static DataManager* sharedDataManager(void);
    static void purgeSharedDataManager(void);
    
    void resetData();
private:
    DataManager(void);
    ~DataManager(void);
    bool init(void);
private:
    cocos2d::CCArray    *m_pDatas;
};


#endif /* defined(__QSMY__DataManager__) */
