//
//  WishModel.h
//  QSMY
//
//  Created by 汪燕军 on 13-8-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WishModel__
#define __QSMY__WishModel__

#include "Model.h"
#include "NavigationModel.h"

#define D_EVENT_WISHES_GET      "wishes get"        //从服务端获得愿望列表完成
#define D_EVENT_WISH_UPDATE     "wish update"       //愿望发生变化
#define D_EVENT_WISH_RES_GET    "wish res get"      //获得了许愿结果

#define TOTAL_WISH_NUM      10      //最大愿望数量
#define UNLOCKABLE_WISH_NUM 5       //最大愿望数量
#define K_WISH_SKY_LIGHT_NUM_PER_TIME  (D_CONFIG_UINT_FOR_KEY(WISH_SKY_LIGHT_NUM_PER_TIME))   //每次许愿需要的天灯数量

namespace warrior {
    class WarriorCurInfo;
}

namespace wish
{
    /**
     *  @brief  许愿格子信息
     */
    class WishItem : public ValueObjcet
    {
    public:
        WishItem()
        : ValueObjcet()
        , m_nVipLevel(0)
        , m_nWarriorId(0)
        , m_nIndex(0)
        {
            
        }
        
        virtual ~WishItem()
        {
        }
        
        CREATE_FUNC(WishItem);
        
        D_INIT_DEFAULT(ValueObjcet);
        
        /**
         *	@brief	愿望格子是否已解锁
         *
         *	@return	是否已解锁
         */
        bool isUnlocked();
        
    private:
        /**
         *	@brief	VIP等级
         */
        CC_SYNTHESIZE(uint8_t, m_nVipLevel, VipLevel);

        /**
         *	@brief	弟子
         */
        CC_SYNTHESIZE(uint32_t, m_nWarriorId, WarriorId);
        
        /**
         *	@brief	索引
         */
        CC_SYNTHESIZE(uint32_t, m_nIndex, Index);
    };
}

class WishModel:public Model
{
public:
    WishModel()
    : Model()
    , m_pWishes(NULL)
    , m_pWishRes(NULL)
	, m_bWarriorsChanged(false)
    {
        
    };
    
    virtual ~WishModel()
    {
        CC_SAFE_RELEASE(m_pWishes);
        CC_SAFE_RELEASE(m_pWishRes);
    };
    
    CREATE_FUNC(WishModel);
    
    virtual bool init();
    
    /**
     *	@brief	从服务端同步数据
     */
    void syncFromServer();
    
    /**
     *	@brief	获取许愿项
     *
     *	@param 	index 	许愿格子索引
     *
     *	@return	许愿项信息
     */
    wish::WishItem * getWishItem(uint32_t index);
    
    /**
     *	@brief	弟子是否已许愿
     *
     *	@param 	warriorId 	弟子编号
     *
     *	@return	是否已许愿
     */
    bool isWarriorWished(uint32_t warriorId);
    
    /**
     *	@brief	许愿列表获取成功
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void wishesGet(const Json::Value & requestData, const Json::Value & responseData);
      
    /**
     *	@brief  获取解锁的愿望格子列表
     *
     *	@return	解锁的格子列表
     */
    cocos2d::CCArray * getUnlockList();
    
    /**
     *	@brief	设置愿望
     *
     *	@param 	index 	愿望索引
     *	@param 	pWarrior 	弟子信息
     */
    void setWish(uint32_t index, warrior::WarriorCurInfo * pWarrior);
    void setWarriorChanged();
    /**
     *	@brief	执行许愿
     *
     *	@param 	isTen 	是否十次
     */
    void doWish(bool isTen);
    
    /**
     *	@brief	许愿完成
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void doWishComplete(const Json::Value & requestData, const Json::Value & responseData);

	void saveWishWarriors();
    
private:
    /**
     *	@brief	许愿列表 单项为WishItem *
     */
    CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pWishes, Wishes);
    
    /**
     *	@brief	许愿结果 单项为CCInteger*
     */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *,  m_pWishRes, WishRes);

	bool m_bWarriorsChanged;
};

#endif /* defined(__QSMY__WishModel__) */
