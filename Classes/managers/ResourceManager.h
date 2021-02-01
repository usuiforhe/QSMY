//
//  ResourceManager.h
//  QSMY
//
//  Created by wanghejun on 13-4-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ResourceManager__
#define __QSMY__ResourceManager__

#include "AppInclude.h"
#include "DragonBone.h"
#include "../loaders/ArmatureLoader.h"

#pragma mark - ResourceConfig -


#define DEFAULT_UNIT 0
#define NO_PVE_MAP 0
#define DEFAULT_SKILL   0   //默认找不到的资源编号

#define UNIT_SK_NAME(id) cocos2d::CCString::createWithFormat("UNIT_%d",id)->getCString()

#define ANI_SK "ani"

//FIXME: 改成配置路径和贴图格式
class ResourceConfig
{
public:
    ResourceConfig(const char* _warrior
                   ,const char* _equip
				   ,const char* _item
                   ,const char* _skill
                   ,const char* _chapter
                   ,const char* _chapter_thumb
                   ,const char* _goods
                   ,const char* _destiny
                   ,const char* _mask
                   ,const char* _recharge
				   ,const char* _tutorial
				   ,const char* _warrior_head_icon
				   ,const char* _battle_map
                   )
    :warrior(_warrior)
    ,equip(_equip)
	,item(_item)
    ,skill(_skill)
    ,chapter(_chapter)
    ,goods(_goods)
    ,destiny(_destiny)
    ,mask(_mask)
    ,chapter_thumb(_chapter_thumb)
    ,recharge(_recharge)
	,tutorial(_tutorial)
	,warrior_head_icon(_warrior_head_icon)
	,battle_map(_battle_map)
    {
        
    };
    
    const std::string warrior;          //弟子
    const std::string equip;            //装备
	const std::string item;             //道具
    const std::string skill;            //技能
    const std::string chapter;          //章节
    const std::string goods;            //商品
    const std::string destiny;          //天命
    const std::string mask;             //遮罩
    const std::string chapter_thumb;    //章节缩略图
    const std::string recharge;         //充值
	const std::string tutorial;
	const std::string warrior_head_icon; //弟子小头像icon
	const std::string battle_map;
};

#pragma mark - ResourceManager -
class ResourceManager :public cocos2d::CCObject
{
public:
    ResourceManager();
    ~ResourceManager();
    /** returns a shared instance of the gameManager */
    static ResourceManager* sharedResourceManager();
    static void purgeResourceManager();
    bool init();
    
    void removeAnimationCache();
    
	//注意：加载战斗资源时clearCache会清除已加载的资源，会导致战斗中找不到需要的资源
    void clearCache();
    
    cocos2d::CCTexture2D* genearateWarriorMaskTexture(unsigned int maskId, cocos2d::CCPoint maskPosition);
#pragma mark - variable -
private:
    ResourceConfig * m_pResourceConfig;
    cocos2d::CCTexture2D* getWarriorTexture(unsigned int id);
    

private:
    cocos2d::CCSprite *maskSprite(cocos2d::CCSprite *sourceSprite , cocos2d::CCSprite *maskSprite , std::string sourcePath=std::string("") , std::string maskPath=std::string("") ,cocos2d::CCTexture2DPixelFormat textureFormat = cocos2d::kCCTexture2DPixelFormat_RGBA8888);
    
    /**
     *	@brief	获取文件路径
     *
     *	@param 	std::string 	路径格式
     *	@param 	id 	编号
     *
     *	@return	文件路径
     */
    cocos2d::CCString* getResPath(const std::string& path,unsigned int id);
#pragma mark - 公用方法 -
public:
    
    void addImage(const char* path,cocos2d::CCTexture2DPixelFormat format);

#pragma mark - 技能图标 -
public:
    cocos2d::CCSprite *getSkillIcon(unsigned int id);
    
#pragma mark - 装备图标 -
    cocos2d::CCSprite *getEquipIcon(unsigned int id);           //装备原图标
    cocos2d::CCSprite *getEquipIcon80P(unsigned int id);        //装备原图标 0.8
    cocos2d::CCSprite *getEquipIcon50PForList(unsigned int id); //装备原图标 0.5
    cocos2d::CCSprite *getEquipIcon50P(unsigned int id);        //装备原图标 0.5
    cocos2d::CCSprite *getEquipIcon40P(unsigned int id);        //装备原图标 0.4
	cocos2d::CCSprite *getEquipIcon30P(unsigned int id);        //装备原图标 0.3
    
#pragma mark - 道具图标 -
public:
    cocos2d::CCSprite *getItemIcon(unsigned int id);            //装备原图标
    cocos2d::CCSprite *getItemIcon80P(unsigned int id);         //装备原图标 0.8
    cocos2d::CCSprite *getItemIcon50PForList(unsigned int id);  //装备原图标 0.5
    cocos2d::CCSprite *getItemIcon50P(unsigned int id);         //装备原图标 0.5
    cocos2d::CCSprite *getItemIcon40P(unsigned int id);         //装备原图标 0.4
	cocos2d::CCSprite *getItemIcon30P(unsigned int id);         //装备原图标 0.3
    
#pragma mark - 商品图标 -
public:
	cocos2d::CCSprite *getProductIconForShow(unsigned int id);
	cocos2d::CCSprite *getProductIconForList(unsigned int id, unsigned short type);

	cocos2d::CCSprite *getGoodsIcon80P(unsigned int id);
    cocos2d::CCSprite *getGoodsIcon(unsigned int id);
#pragma mark - 充值图标 -    
public:
	cocos2d::CCSprite *getRechargeIcon(unsigned int id);

	
#pragma mark - 战场资源 -
    sp::Armature* getUnitAnimate(unsigned int sid);
    sp::Armature* getUnitAnimate2X(unsigned int sid);
    
    ArmatureLoader * getUnitAnimateAsync(unsigned int sid, ArmatureLoaderDelegate *pDelegate = NULL);
    ArmatureLoader * getUnitAnimate2XAsync(unsigned int sid, ArmatureLoaderDelegate *pDelegate = NULL);
    //战斗技能展示
    cocos2d::CCSprite *getUnitImage(unsigned int sid, bool needFlip=false, cocos2d::CCPoint point=cocos2d::CCPointZero);

#pragma mark - 弟子图标 -
public:
	//(已用PL新遮罩，配置)
	cocos2d::CCSprite * getWarriorIcon50PForList(unsigned int id);              //列表50
	cocos2d::CCSprite * getWarriorIconForChangeFormation(unsigned int id);


    cocos2d::CCSprite * getWarriorIcon(unsigned int id, float specifiedScale = 0.0f, bool needFlip=false);                        //获取弟子图像
    cocos2d::CCSprite * getWarriorHeadIcon(unsigned int id, cocos2d::CCSize size = cocos2d::CCSizeMake(70, 70));	//获取PL新添加的弟子头像icon
    cocos2d::CCSprite * getWarriorIcon50PForLottery(unsigned int id);           //获取弟子抽卡
    cocos2d::CCSprite * getWarriorIcon50PForActivityList(unsigned int id);              //活动列表50
    
    cocos2d::CCSprite * getWarriorIcon50PForFormation(unsigned int id);         //上阵50
    cocos2d::CCSprite * getWarriorIcon50PForDetail(unsigned int id);            //弟子详50
    cocos2d::CCSprite * getWarriorIcon50PForMall(unsigned int id);              //商城50
    cocos2d::CCSprite * getWarriorIcon50P(unsigned int id);                     //小框50
    cocos2d::CCSprite * getWarriorIcon40P(unsigned int id);                     //小框40
    cocos2d::CCSprite * getWarriorIcon40PForDestiny(unsigned int id);           //天命40
    cocos2d::CCSprite * getWarriorIconForAward(unsigned int id);                //十连抽40
    
    cocos2d::CCSprite * getWarriorIcon40PForFormation(unsigned int id);         //阵容40
    cocos2d::CCSprite * getWarriorIcon40PForMatch(unsigned int id);             //杯赛40
    cocos2d::CCSprite * getWarriorIcon40PForChangeFormation(unsigned int id);   //布阵40

	cocos2d::CCSprite * getWarriorIcon40PForGuildBoss(unsigned int id);			//苍龙现40
    
    cocos2d::CCSprite * getWarriorIcon50ForSoulHunter(unsigned int id);           //狩魂
    
    

	cocos2d::CCSprite * getWarriorIcon60PForFriendFound(unsigned int id);           //好友
    cocos2d::CCSprite * getWarriorIcon176PForPve(unsigned int id);              //pve探索176
    
    cocos2d::CCSprite * getWarriorIconForTutorial(unsigned int id);              //新手选英雄

    cocos2d::CCSprite * getWarriorIcon25PForPvp(unsigned int id);
#pragma mark - 场景图标 -
public:
    cocos2d::CCSprite * getSceneIcon(unsigned int id);
	cocos2d::CCSprite * getSceneThumbIcon(unsigned int id);
	cocos2d::CCSprite * getSceneThumbIconForTeam(unsigned int id);
    cocos2d::CCSprite * getSettlementTitle(unsigned int id);
    sp::Armature*       getEffect(unsigned int id);

	cocos2d::CCSprite * getChapterIcon(unsigned int id);
#pragma mark - 天命 -
public:
    cocos2d::CCSprite * getDestinyIcon(unsigned int id);

#pragma mark - UI 动画 -
public:
    sp::Armature* getUIAnimate(const char* name);
    sp::Armature* getUIAnimate(const char* name,sp::RENDER_TYPE renderType);
	sp::Armature* getUIAnimateOnly(const char *name,sp::RENDER_TYPE renderType = sp::BATCHNODE_ZORDER); //dont play
	sp::Armature* getUIAnimateOnly(std::string &name,sp::RENDER_TYPE renderType = sp::BATCHNODE_ZORDER); //dont play
    
#pragma mark - 突破等级 -
public:
    cocos2d::CCSprite * getUpLevel(int8_t level);

	cocos2d::CCSprite * getTutorialEmotion(unsigned int id);
};

#endif /* defined(__QSMY__ResourceManager__) */
