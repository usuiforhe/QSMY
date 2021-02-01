#ifndef __PILI_RankModel__
#define __PILI_RankModel__
#include "Model.h"
#include "json/json.h"

#define RANK_TYPE_COUNT 2
#define RANK_LIST_UPDATE "rank list update"

class RankModel;
namespace rank{
	enum RankType{
		kNone,
		kForceRank,
		kLevelRank
	};


	class UserRankInfo : public ValueObjcet
	{
	public:
		UserRankInfo():m_iID(0), m_iUserID(0), m_iRank(0), m_iNum(0), m_iVip(0), m_iName(""), m_pType(kNone){};
		CREATE_FUNC(UserRankInfo);
		D_INIT_DEFAULT(ValueObjcet);
	protected:
		CC_SYNTHESIZE(uint32_t, m_iID, ID);
		CC_SYNTHESIZE(uint32_t, m_iUserID, UserID);
		CC_SYNTHESIZE(uint32_t, m_iRank, Rank);
		CC_SYNTHESIZE(uint32_t, m_iNum, Num);
		CC_SYNTHESIZE(uint32_t, m_iVip, Vip);
		CC_SYNTHESIZE(std::string, m_iName, Name);
		CC_SYNTHESIZE(RankType, m_pType, Type);

		friend class ::RankModel;
	};


}

#pragma mark -----------------RankModel-----

class RankModel : public Model
{
	CC_SYNTHESIZE_READONLY(rank::UserRankInfo*, m_pMyFightInfo, MyFightInfo);
	CC_SYNTHESIZE_READONLY(rank::UserRankInfo*, m_pMyLevelInfo, MyLevelInfo);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pFightInfoList, FightInfoList);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pLevelInfoList, LevelInfoList);

public:
	RankModel();
	virtual ~RankModel();
	virtual bool init();

	void getRankInfo(float dt=0.f);

	void parseRankInfo(const Json::Value &requestData, const Json::Value &responseData);

	rank::UserRankInfo* parseRank(const Json::Value & val, rank::UserRankInfo*);

	cocos2d::CCArray* getRankListByType(rank::RankType type);
	
};

#endif  //__PILI_RankModel__