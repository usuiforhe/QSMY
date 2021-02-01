//
//  MallModel.h
//  商城
//
//  Created by cll on 13-6-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MallModel__
#define __QSMY__MallModel__

#include "Model.h"


#define  D_EVENT_MALLLIST_UPDATE "update malllist"


#define BUY_CONDITION_VIP "vip"
#define BUY_CONDITION_NUM_LESS "num_less"

#define PRODUCT_POW_ID				(D_CONFIG_UINT_FOR_KEY(PRODUCT_POW))
//#define PRODUCT_PVE_EXEC_TIMES_ID	(D_CONFIG_UINT_FOR_KEY(PRODUCT_PVE_EXEC_TIMES))

class MallModel;

namespace mall {
    
	//类型定义
	enum ProductType
	{
		kProductTypeALL=0,
        
		kProductTypeItem=1,			//道具
		kProductTypeVipPk=2,			//礼包
		kProductTypeEquip=3,			//装备
		kProductTypeSoul=4,			//魂魄
		kProductTypeWarrior=5,		//弟子
        kProductTypeAvatar=15,     //时装
        
		kProductTypeCount=7			//类型总数 !!
	};
    
    enum ProductPayType
    {
        kProductPayTypePoint = 1,
        kProductPayTypeCoin = 2,
    };

	/*商品基本信息*/
	class ProductBase:public ValueObjcet
	{
	public:
		CREATE_FUNC(ProductBase);
		D_INIT_DEFAULT(ValueObjcet);
        
        friend class ::MallModel;

		CC_SYNTHESIZE_READONLY(unsigned int, pId, PId);  //商品ID
        CC_SYNTHESIZE_READONLY(ProductType, type, Type);  //类型
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, req_desc, ReqDesc); //购买条件描述
        CC_SYNTHESIZE_READONLY(unsigned int, req_num, ReqNum);  //需求数
        CC_SYNTHESIZE_READONLY(unsigned int, old_price, OldPrice);
        CC_SYNTHESIZE_READONLY(bool, if_look, IfLook);
        CC_SYNTHESIZE_READONLY(bool, if_time, IfTime);      //是否限时
        CC_SYNTHESIZE_READONLY(unsigned int, num, Num); //一次购买个数
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, name, ProductName); //商品名
        CC_SYNTHESIZE_READONLY(unsigned int, itemId, ItemId); //获得商品对应的物品ID
        CC_SYNTHESIZE_READONLY(unsigned int, resId, ResId); //获得商品对应的图标ID
	};

	/*商城列表商品信息*/
	class ProductInfo:public ValueObjcet
	{
	public:
		ProductInfo():pId(0),price(0),pay_type(kProductPayTypePoint),baseInfo(NULL),m_sDesc(""){};
	public:
		CREATE_FUNC(ProductInfo);
		D_INIT_DEFAULT(ValueObjcet);
        friend class ::MallModel;

        //是否满足购买条件
        bool isCanBuy();
        CC_SYNTHESIZE_READONLY(ProductBase *, baseInfo, BaseInfo);
        CC_SYNTHESIZE_READONLY(unsigned int, pId, PId);          //商品ID
        CC_SYNTHESIZE_READONLY(unsigned int, price, Price);      //当前单价
        CC_SYNTHESIZE_READONLY(ProductPayType, pay_type, PayType);//支付类型:1点券 2 金币
        
        unsigned int getNum();   //拥有个数
        
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sDesc, Desc);//描述
        
        
	};
}

class MallModel : public Model
{

public:
	typedef cocos2d::CCArray ProductInfoVec;
    
private:
	typedef cocos2d::CCDictionary ProductBaseMap;
	typedef cocos2d::CCDictionary RedPacketInfoMap;

public:
    
	MallModel();
    virtual ~MallModel();
    virtual bool init();
	
	mall::ProductBase * getProductBaseByID(int pid);	

	/* 获取商品列表 */
	void getProductsByType(mall::ProductType pType, ProductInfoVec &vec);
    /* 获取商城显示的商品列表 */
    void getProductsInMallByType(mall::ProductType pType, ProductInfoVec &vec);

	void parseProducts(const Json::Value &val);

	void parseOneProduct(const Json::Value &val);
    
    /**
     *	@brief	获取商品信息
     *
     *	@param 	id 	商品编号
     *
     *  @return	商品信息
     */
    mall::ProductInfo* getProductById(unsigned int id);
	mall::ProductInfo* getProductByItemId(unsigned int ItemId);
    /**
     *	@brief	获取商品价格
     *
     *	@param 	id  商品编号
     *
     *	@return	商品价格
     */
    uint32_t getProductPriceById(unsigned int id);

	cocos2d::CCArray* getRedPacketListById(unsigned int id);
    
    
    /**
     *	@brief	检查道具是否在商城中出售
     *
     *	@param 	id 	道具编号
     *
     *	@return	是否在
     */
    bool  isItemInMall(unsigned int itemId);


    virtual void reset();
    
private:

	void load200RedPacket();
	void loadMallData();
	void parseProductsData(const CSVDecoder::CSV &val);

private:
    
	RedPacketInfoMap *m_pRedPacketInfoMap; //红包信息表
	ProductBaseMap *m_productBaseMap;	//商品基本信息
	ProductInfoVec *m_productInfoVec;	//商品列表
	CC_SYNTHESIZE(uint32_t, m_nUnenoughItemID, UnenoughItemID);
};





#endif /* defined(__QSMY__MallModel__) */
