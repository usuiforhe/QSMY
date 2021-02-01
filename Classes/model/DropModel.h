//
//  DropModel.h
//
//  掉落信息
//  Created by wanghejun on 13-7-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DropModel__
#define __QSMY__DropModel__

#include "Model.h"


#define  D_EVENT_DROPS_SHOW		"show drops"
#define  D_EVENT_DROPS_REMOVE	"remove drops"

namespace equip {
    class EquipCurInfo;
}

namespace item {
    class ItemInfo;
}

namespace warrior {
    class WarriorCurInfo;
}

namespace warrior {
    class SoulInfo;
}

namespace avatar {
    class AvatarInfo;
}


namespace drop
{
    
    enum DropType
    {
        kDropTypeNone=0,
        kDropTypeItem = 1,			//道具
        kDropTypePkg = 2,			//礼包
        kDropTypeEquip = 3,			//装备
        kDropTypeSoul = 4,			//魂魄
        kDropTypeWarrior = 5,		//弟子
		kDropTypeCoin = 6,			//金币
		kDropTypePoint = 7,			//点券
		kDropTypeVip = 9,			//Vip
		kDropTypePower = 12,		//体力
        kDropTypeAvatar = 15,       //时装
		kDropTypeJJCScore = 17,  //JJC积分
    };
    
    //掉落   临时数据 用于界面显示
    class Drop:public ValueObjcet
    {
	public:
        Drop()
        :m_eType(kDropTypeNone)
        ,m_pEquip(NULL)
        ,m_pItem(NULL)
        ,m_pWarrior(NULL)
		,m_pSoul(NULL)
        ,m_pAvatarInfo(NULL)
		,m_nCoin(0)
		,m_nPoint(0)
        ,m_nVip(0)
        ,m_nPower(0)
        {
            
        };
        
        virtual ~Drop();
        
        
        D_INIT_DEFAULT(ValueObjcet);
        
        static Drop * create(avatar::AvatarInfo * pAvatarInfo);
        static Drop * create(equip::EquipCurInfo * pEquip);
        static Drop * create(item::ItemInfo * pItem);
        static Drop * create(warrior::WarriorCurInfo * pWarrior);
        static Drop * create(warrior::SoulInfo * pSoul);
        static Drop * createCoin(uint32_t coin);
        static Drop * createPoint(uint32_t Point);
        static Drop * createVip(uint32_t vip);
        static Drop * createPower(uint32_t power);
		static Drop * createJJCScore(uint32_t score);

		static Drop * create(DropType eType, unsigned int dropID, unsigned int dropNum = 1);
    protected:
        bool init(avatar::AvatarInfo * pAvatarInfo);
		bool init(equip::EquipCurInfo * pEquip);
		bool init(item::ItemInfo * pItem);
		bool init(warrior::WarriorCurInfo * pWarrior);
		bool init(warrior::SoulInfo * pSoul);
        bool initCoin(uint32_t coin);
        bool initPoint(uint32_t point);
        bool initPower(uint32_t power);
		bool initJJCScore(uint32_t score);
        
        bool initVip(uint32_t vip);
        
    protected:
        CC_SYNTHESIZE_READONLY(DropType, m_eType, Type);
        CC_SYNTHESIZE_READONLY(equip::EquipCurInfo *, m_pEquip, Equip);
        CC_SYNTHESIZE_READONLY(avatar::AvatarInfo*, m_pAvatarInfo, AvatarInfo);
        CC_SYNTHESIZE_READONLY(item::ItemInfo *, m_pItem, Item);
        CC_SYNTHESIZE_READONLY(warrior::WarriorCurInfo *, m_pWarrior, Warrior);
        CC_SYNTHESIZE_READONLY(warrior::SoulInfo *, m_pSoul, Soul);
        CC_SYNTHESIZE_READONLY(uint32_t, m_nCoin, Coin);
        CC_SYNTHESIZE_READONLY(uint32_t, m_nPoint, Point);
        CC_SYNTHESIZE_READONLY(uint32_t, m_nVip, Vip);
        CC_SYNTHESIZE_READONLY(uint32_t, m_nPower, Power);
		 CC_SYNTHESIZE_READONLY(uint32_t, m_nScore, Score);
    };    
}

class DropModel : public Model
{
public:
    
    DropModel();
    virtual ~DropModel();

    virtual bool init();

	cocos2d::CCArray* getDropArray(uint32_t id);
	void removeDrop(unsigned int id);
    
public:
    
	static bool dropSort(CCObject* , CCObject*);
	static int getGrade(drop::Drop*);
	static unsigned int getDropCount(drop::Drop* pDropItem);
	static cocos2d::CCSprite* getDropIconSprite(drop::Drop* pDropItem);
	void addDrops(const Json::Value &val,uint32_t id , bool noSort = false);
    void addDrop(const Json::Value &val,uint32_t id);
	drop::Drop * parseDrop(const Json::Value &val);
	// 文字化掉落信息
	std::string dropContent(drop::Drop* pDrop);
    
private:
    void inDrops(drop::Drop *pDrop,cocos2d::CCArray *pArray , bool noSort);
    cocos2d::CCArray* resetDrop(uint32_t id);
private:
	cocos2d::CCDictionary  *m_pDropsMap;		//临时掉落数据
};

#endif /* defined(__QSMY__DropModel__) */
