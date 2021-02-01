//
//  ActivenessModel.h
//  活跃度
//
//  created by cll on 13-6-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ActivenessModel__
#define __QSMY__ActivenessModel__

#include "Model.h"
#include "NavigationModel.h"

#define D_EVENT_ACTIVENESS_AWARD_DRAWN		"draw activenesses award"		//活跃度奖励领取
#define D_EVENT_ACTIVENESS_UPDATE			"update activeness"             //活跃度数据变化
#define D_EVENT_ACTIVENESS_USER_UPDATE		"update user activeness"        //活跃度数据变化

namespace drop
{
    class Drop;
}

namespace activeness {

#pragma mark -ActivenessInfo 活跃度配置信息 -
    
	//活跃度配置信息
	class ActivenessInfo : public ValueObjcet
	{

	public:
		ActivenessInfo():
        m_nId(0),
        m_eNavType(navigation::kNavNIL),
        m_sName(""),
        m_sDesc(""),
        m_nTarget(1),
        m_nPoint(0)
        ,m_eNavTag(0)
		{

		};
		D_INIT_DEFAULT(ValueObjcet);

		CREATE_FUNC(ActivenessInfo);
        
        /**
         *	@brief	从行加载
         *
         *	@param 	line 	数据行
         */
        void loadLine(const CSVDecoder::CSVLine & line);
        
        /**
         *	@brief	是否已解锁
         *
         *	@return	是否已解锁
         */
        bool isUnlocked();


	private:
		/**
		 *	@brief	活跃度编号
		 */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nId, Id);
		/**
		 *	@brief	导航类型
		 */
        CC_SYNTHESIZE_READONLY(navigation::NavigationType, m_eNavType, NavType);
        
        /**
		 *	@brief	导航信息
		 */
        CC_SYNTHESIZE_READONLY(uint8_t, m_eNavTag, NavTag);
        /**
		 *	@brief	名称
		 */
        CC_SYNTHESIZE_READONLY(std::string	, m_sName, Name);
        /**
		 *	@brief	描述
		 */
        CC_SYNTHESIZE_READONLY(std::string	, m_sDesc, Desc);
        /**
		 *	@brief	目标次数
		 */
        CC_SYNTHESIZE_READONLY(uint16_t, m_nTarget, Target);
        /**
		 *	@brief	可获得的点数
		 */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nPoint, Point);
	};
    
#pragma mark - UserActiveness 玩家活跃度达成情况 -
    class UserActiveness:public ValueObjcet
    {
    public:
		UserActiveness():
        m_nTimes(0),
        m_pInfo(NULL)
        {
            
        };
        
		~UserActiveness()
		{            
			CC_SAFE_RELEASE(m_pInfo);
		};
        
		CREATE_FUNC(UserActiveness);
		D_INIT_DEFAULT(ValueObjcet);
        
        /**
         *	@brief	从服务端数据加载
         *
         *	@param 	val 	服务端数据
         */
        void loadData(const Json::Value &val);
        
        /**
         *	@brief	目标是否已完成
         *
         *	@return	是否已完成
         */
        inline bool isTargetAchieved()

        {
            return m_nTimes >= m_pInfo->getTarget();
        }
        
    protected:
        /**
		 *	@brief	当前完成次数
		 */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nTimes, Times);
        /**
		 *	@brief	活跃度配置信息
		 */
        CC_SYNTHESIZE(ActivenessInfo*, m_pInfo, Info);
    };
    
#pragma mark - AwardItemInfo 活跃度奖励项配置 -
    class AwardInfo;
    
	class AwardItemInfo:public ValueObjcet
	{
	public:
		enum AwardItemType
		{
			kAwardTypeNone  = 0,
			kAwardTypeCoin  = 1,	//金币
			kAwardTypePoint = 2,	//点券
			kAwardTypeItem  = 3,    //道具
			kAwardTypeEquip = 4     //装备
		};

	public:
		AwardItemInfo():
        m_nType(kAwardTypeNone),
        m_nId(0),
        m_nNum(0){
        };
        
		~AwardItemInfo()
		{
            
		};

		CREATE_FUNC(AwardItemInfo);
		D_INIT_DEFAULT(ValueObjcet);
        
        drop::Drop * toDrop(AwardInfo * pAward);

	public:
        /**
		 *	@brief	奖励类型
		 */
		CC_SYNTHESIZE(AwardItemType, m_nType, Type);
        /**
		 *	@brief	奖励对应的物品编号
		 */
		CC_SYNTHESIZE(uint32_t, m_nId, Id);
        /**
		 *	@brief	奖励数量
		 */
		CC_SYNTHESIZE(uint32_t, m_nNum, Num);

	};
    
#pragma mark - AwardInfo 活跃度奖励配置 -
	class AwardInfo:public ValueObjcet
	{
	public:
		AwardInfo():m_nRequirePoint(0),m_pAwardItems(NULL){};

		CREATE_FUNC(AwardInfo);
		~AwardInfo()
		{
			CC_SAFE_RELEASE_NULL(m_pAwardItems);
		};

		bool init()
		{
            if(!ValueObjcet::init()) return false;
			m_pAwardItems = cocos2d::CCArray::create();
			CC_SAFE_RETAIN(m_pAwardItems);
			return true;
		};        
        
        /**
         *	@brief	从行加载
         *
         *	@param 	line 	数据行
         */
        void loadLine(const CSVDecoder::CSVLine & line, AwardInfo* pInfo);

	public:
        /**
		 *	@brief	需要活跃度分数
		 */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nRequirePoint, RequirePoint);
        /**
		 *	@brief	金币计算系数
		 */
        CC_SYNTHESIZE_READONLY(float, m_fCoinFactor, CoinFactor);
        /**
		 *	@brief	奖励项列表
		 */
        CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pAwardItems, AwardItems);
	};
}

#pragma mark - ActivenessModel 活跃度模块数据 -

class ActivenessModel : public Model
{
public:
    ActivenessModel();
    ~ActivenessModel();
    
    virtual bool init();
    
    CREATE_FUNC(ActivenessModel);

	/**
	 *	@brief	解析活跃度信息
	 *
	 *	@param 	val 	服务端数据
	 */
	void parseActiveness(const Json::Value &val);
    
	/**
	 *	@brief	领取奖励完成
	 *
	 *	@param 	point 	奖励对应的分数
	 */
    void onGetAwardComplete(uint32_t point);
    
    /**
     *	@brief	获取活跃度奖励列表
     *
     *	@return	活跃度奖励列表
     */
    cocos2d::CCArray * getAwardInfoList();
    
    /**
     *	@brief	获取活跃度信息列表
     *
     *	@return	活跃度信息列表
     */
    cocos2d::CCArray * getActivenessInfoList();
    
    /**
     *	@brief	根据活跃度分数获取活跃度奖励信息
     *
     *	@param 	point 	分数
     *
     *	@return 奖励信息
     */
    inline activeness::AwardInfo * getAwardInfo(uint32_t point)
    {
        return dynamic_cast<activeness::AwardInfo *>(m_pDicAward->objectForKey(point));
    }
    
    /**
     *	@brief	根据活跃度编号获取活跃度配置信息
     *
     *	@param 	activenessId 	活跃度编号
     *
     *	@return	活跃度配置信息
     */
    inline activeness::ActivenessInfo * getActivenessInfo(uint32_t activenessId)
    {
        return dynamic_cast<activeness::ActivenessInfo *>(m_pDicActivenessInfo->objectForKey(activenessId));
    }
    
    /**
     *	@brief	根据活跃度编号获取玩家的活跃度信息
     *
     *	@param 	activenessId 	活跃度编号
     *
     *	@return	玩家活跃度信息
     */
    inline activeness::UserActiveness * getActiveness(uint32_t activenessId)
    {
        return dynamic_cast<activeness::UserActiveness *>(m_pDicActiveness->objectForKey(activenessId));
    }
    
    /**
     *	@brief	判断奖励是否已领取
     *
     *	@param 	pAward 	奖励
     *
     *	@return	是否已领取
     */
    inline bool isAwardDrawn(activeness::AwardInfo * pAward)
    {
        cocos2d::CCObject * obj(NULL);
        CCARRAY_FOREACH(m_pArrDrawnAward, obj)
        {
            cocos2d::CCInteger * interger = dynamic_cast<cocos2d::CCInteger * >(obj);
            if(interger == NULL)
            {
                continue;
            }
            
            if(interger->getValue() == pAward->getRequirePoint())
            {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     *	@brief	判断奖励是否可以领取
     *
     *	@param 	pAward 	奖励
     *
     *	@return	是否可以领取
     */
    inline bool isAwardAvailable(activeness::AwardInfo * pAward)
    {
        return m_nPoint >= pAward->getRequirePoint();
    }
    
    /**
     *	@brief	获取是否有奖能领
     *
     *	@return	是否有奖能领
     */
    bool hasAwardDrawable();


private:
    void loadActivenessData();
	void loadAwardData();

private:
    /**
     *	@brief	活跃度点数
     */
    CC_SYNTHESIZE_READONLY(uint32_t, m_nPoint, Point);
    
    /**
     *	@brief	活跃度奖励配置
     */
    cocos2d::CCDictionary * m_pDicAward;
    /**
     *	@brief	活跃度配置字典
     */
    cocos2d::CCDictionary * m_pDicActivenessInfo;
    /**
     *	@brief	玩家活跃度字典
     */
    cocos2d::CCDictionary * m_pDicActiveness;
    /**
     *	@brief	玩家奖励领取信息
     */
    cocos2d::CCArray * m_pArrDrawnAward;
};

#endif /* defined(__QSMY__ActivenessModel__) */
