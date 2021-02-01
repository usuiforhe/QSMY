//
//  ItemModel.h
//  道具信息
//
//  Created by cll on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ItemModel__
#define __QSMY__ItemModel__

#include "Model.h"
#include "DropModel.h"

#define  D_EVENT_ITEMLIST_UPDATE "update itemList"
#define  D_EVENT_ITEMINFO_UPDATE "update itemInfo"
#define  D_EVENT_BUYITEM_SUCCESS "buyitem success"

#define  D_EVENT_ADD_POW "add pow"
#define  D_EVENT_ADD_ENDU "add endu"

#define  D_EVENT_AVAILABLE_EQUIP "available equip"

#define		TREASURE_ENHANCE_ITEM_ID		(D_CONFIG_UINT_FOR_KEY("LIANHUA_QIANGHUASHI"))		//强化石ID

//#define		ITEM_CHICKEN_ID			(D_CONFIG_UINT_FOR_KEY(ITEM_CHICKEN))		//鸡
#define		ITEM_NEILIDAN_ID		(D_CONFIG_UINT_FOR_KEY(ITEM_NEILIDAN))		//技能强化消耗内力丹ID

#define		CHUANGONG_DAN_ID1		(D_CONFIG_UINT_FOR_KEY(ITEM_CHUANGONGDAN1))		//普通传功丹ID
#define		CHUANGONG_DAN_ID2		(D_CONFIG_UINT_FOR_KEY(ITEM_CHUANGONGDAN2))		//高级传功丹ID

#define		PEIYANG_DAN_ID			(D_CONFIG_UINT_FOR_KEY(ITEM_PEIYANGDAN))		//培养丹ID
#define     ZHAO_MU_LING_ID		(D_CONFIG_UINT_FOR_KEY(ITEM_ZHAOMULING))			//招募令
#define     NORMAL_CARD_ITEM_ID		(D_CONFIG_UINT_FOR_KEY(ITEM_YIJIDANGQIANGCARD))	//招募令normal
#define     HARD_CARD_ITEM_ID		(D_CONFIG_UINT_FOR_KEY(ITEM_WANFUMOKAICARD))	//招募令hard
#define     SKY_LIGHT_ID            (D_CONFIG_UINT_FOR_KEY(ITEM_SKYLIGHT))        //杯赛道具 灯


#define     POWER_ID                (D_CONFIG_UINT_FOR_KEY(ITEM_POWER))        //体力
#define     COIN_ID                 (D_CONFIG_UINT_FOR_KEY(ITEM_COIN))        //金币
#define     POINT_ID                (D_CONFIG_UINT_FOR_KEY(ITEM_POINT))        //元宝


#define		ITEM_CHANGE_NAME_ID		(D_CONFIG_UINT_FOR_KEY(ITEM_CHANGE_NAME))
#define		 K_ITEM_GUILD_ID		(D_CONFIG_UINT_FOR_KEY(ITEM_GUILD))

#define		ITEM_CHAT_ID			(D_CONFIG_UINT_FOR_KEY(ITEM_CHAT))			//聊天道具

class ItemModel;

namespace item {
    
	enum ItemType
	{
		kItemTypeNormal=0,		//普通道具不可使用
		kItemTypeBox=1,			//宝箱
		kItemTypeVipPk=2,		//VIP礼包
		kItemTypeUsage=3,		//消耗型 传功丹、强化石之类
		kItemTypeReward=4,		//特殊奖励 新手礼包、封测礼包、弑神宝箱 tolua
		kItemTypePrivilege=5,	//限时道具 tolua
		kItemTypeSpecialPrivilege=6,	//特殊限时道具 月卡体验券 tolua
		kItemTypePow=8,
        kItemTypeFragment = 9,      //碎片
		kItemTypeEndu=11,
		kItemType200=12,	//200红包
	};
    
    //品阶定义
	enum ItemGrade
	{
		kItemGradeWhite,		//白色
		kItemGradeGreen,		//绿色
		kItemGradeBlue,         //蓝色
		kItemGradePurple,		//紫色
		kItemGradeRed,			//红色
	};

    
    class VipPkg:public ValueObjcet
    {
    public:
        struct VipPkgItem
        {
            drop::DropType type;
            uint32_t id;
            uint32_t count;
        };
        friend class ::ItemModel;
    private:
        uint32_t id;
        std::vector<VipPkgItem> items;
        
        CREATE_FUNC(VipPkg);
        D_INIT_DEFAULT(ValueObjcet);
        
    public:
        std::vector<VipPkgItem> &getItems()
        {
            return items;
        }
    };
    
    /*道具详细信息-本地数据表配置*/
    class ItemBase:public ValueObjcet
    {
        
    public:
        CREATE_FUNC(ItemBase);
        D_INIT_DEFAULT(ValueObjcet);
    
        friend class ::ItemModel;
        
        CC_SYNTHESIZE_READONLY(unsigned int, item_id, ItemId);              //道具ID
        CC_SYNTHESIZE_READONLY(ItemType, item_type, ItemType);              //道具类型
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, item_name, ItemName);//道具名
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, item_desc, ItemDesc);//道具描述
        CC_SYNTHESIZE_READONLY(bool, can_be_used, CanBeUsed);               //是否能用
        CC_SYNTHESIZE_READONLY(unsigned int, use_item_id, UseItemId);       //使用需消耗的物品id
        CC_SYNTHESIZE_READONLY(unsigned short, use_item_num, UseItemNum);   //使用消耗的物品数量
        CC_SYNTHESIZE_READONLY(unsigned int, order_priority, OrderPriority);//排序权重
		CC_SYNTHESIZE_READONLY(unsigned int, m_uResID, ResID);				//资源ID
        CC_SYNTHESIZE_READONLY(unsigned int, m_uSheet, Sheet);				//Sheet
        CC_SYNTHESIZE_READONLY(ItemGrade, item_grade, ItemGrade);           //道具品质
        CC_SYNTHESIZE_READONLY(bool, m_bShow, Show);           //是否显示
        bool isNeedShowGradeColor()
        {
            return item_grade != kItemGradeWhite;
        }
    };
    
    
    /*道具列表中基本信息*/
    class ItemInfo:public ValueObjcet
    {
    public:
        ItemInfo():itemId(0),num(0),baseInfo(NULL){};
    public:
        static ItemInfo* create(unsigned int id,unsigned int num);
        static ItemInfo* create(ItemBase*);
        CREATE_FUNC(ItemInfo);
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::ItemModel;
        
        CC_SYNTHESIZE_READONLY(unsigned int, itemId, ItemId);
        CC_SYNTHESIZE_READONLY(unsigned int, num, Num);
        CC_SYNTHESIZE_READONLY( ItemBase * , baseInfo, BaseInfo);

		bool isItemForRename();
    };
    
	enum UnenoughAlertType
	{
		UnenoughAlertTypeTips = 1,
		UnenoughAlertTypeMessage = 2,
	};

	class ItemUnenoughInfo : public ValueObjcet
	{
	public:

		CREATE_FUNC(ItemUnenoughInfo);
		D_INIT_DEFAULT(ValueObjcet);

	protected:
		friend class ::ItemModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nItemID, ItemID);
		CC_SYNTHESIZE_READONLY(UnenoughAlertType, m_eAlertType, AlertType);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sContent, Content);
		//CC_SYNTHESIZE_READONLY(uint32_t, m_nProductID, ProductID);
	};
    
    
    class ItemColorConfig:public ValueObjcet
    {
    public:
        CREATE_FUNC(ItemColorConfig);
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::ItemModel;
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(cocos2d::ccColor3B, color, Color);
    };
    
}

class ItemModel : public Model
{
    
public:
	typedef cocos2d::CCArray ItemInfoVec;
    
    
public:
    
	ItemModel();
    virtual ~ItemModel();
    virtual bool init();
	
    
	void useItem(item::ItemInfo * pItemInfo,uint32_t num=1);
	void useItem(unsigned int sid);
	/* 从 ItemInfoVec 获取道具基本信息*/
	item::ItemInfo * getItemInfoByID(unsigned int sid);
	item::ItemBase * getItemBaseByID(unsigned int sid);
	uint32_t getResIDByID(uint32_t sid);
    
	/* 解析网络下发道具信息列表*/
	void parseItems(const Json::Value &val);
	
	/* 获得道具信息*/
	void parseOneItem(const Json::Value &val);

	/* 體力類道具使用返回提示*/
	void addPow(item::ItemInfo* , bool bAllStarAward = false/*是否是从副本获得的体力*/);

	/* 耐力類道具使用返回提示*/
	void addEndu(item::ItemInfo*);
    
	/*减少道具数量*/
	void reduceItemNumByID(unsigned int sid, unsigned int num = 1);
    
	/*增加道具数量*/
	void addItemNumByID(unsigned int sid, unsigned int num = 1);
    
    /*道具数量变化*/
    void changeItemNumByID(unsigned int sid, int num);

	/*返回可合成的装备数（重复不计）*/
	unsigned int getAvailableEquipNum();
    
	unsigned int getItemNum(unsigned int sid)
	{
		item::ItemInfo * pItemInfo = getItemInfoByID(sid);
		if(pItemInfo)
			return pItemInfo->num;
		else
			return 0;
	};
    
	item::ItemInfo * parseItem(const Json::Value &val, item::ItemInfo * pItem = NULL);
    
    
    /**
     *	@brief	获取vip礼包内容
     *
     *	@param 	itemId 	礼包编号
     *
     *	@return	内容列表
     */
    cocos2d::CCArray* getVipPkgInfo(uint32_t itemId);

    void reset();
    
    
    bool isMyItem(item::ItemInfo*);
    
    
    /* 获取对应品阶的颜色值 */
	cocos2d::ccColor3B getItemColorByGrade(item::ItemGrade grade);
    
    
    /**
     *	@brief	根据sheet获取道具
     *
     *	@param 	sheet 	sheet编号
     *
     *	@param 	array 	容器
     *
     */
    void getItemsBySheet(uint32_t sheet,cocos2d::CCArray* array);
    
    
    ItemInfoVec *getItemInfoVec()
	{
		return m_itemInfoVec;
	};
    
    const std::set<uint32_t>& getSheets()
    {
        return m_Sheets;
    }
    
private:
    void addItemInorder(item::ItemInfo *pItemInfo);
	void loadItemBaseData();
	void parseItemBaseData(const CSVDecoder::CSV &val);
    void parseVipPkgData(const CSVDecoder::CSV &val);
	void loadUnenoughData(const CSVDecoder::CSV &val);
    void loadColorConfig(const CSVDecoder::CSV &val);
public:
	item::ItemUnenoughInfo *getUnenoughInfo(uint32_t itemID);
private:
    
    cocos2d::CCDictionary *m_itemBaseMap;			//道具基本信息
	ItemInfoVec *m_itemInfoVec;                 //拥有道具列表
    cocos2d::CCDictionary* m_pVipPkgBaseMap;    //vip礼包内容
	cocos2d::CCDictionary *m_pDictUnenough;
    cocos2d::CCDictionary *m_ItemColorMap;      //颜色值
    
    std::set<uint32_t>     m_Sheets;      //sheet类型
};





#endif /* defined(__QSMY__ItemModel__) */
