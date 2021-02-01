//
//  ResourceManager.cpp
//  QSMY
//
//  Created by wanghejun on 13-4-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ResourceManager.h"
#include "DataManager.h"
#include "GameManager.h"
#include "../utils/CSVDecoder.h"
#include "../utils/StringUtil.h"
#include "../components/DMaskedSprite.h"
#include "../scenes/game/battle/BattleDefine.h"

using namespace sp;
using namespace mall;
using namespace drop;

#define DEFAULT "default"

USING_NS_CC;

#pragma mark - ResourceManager -
// singleton stuff
static ResourceManager *s_SharedResourceManager = NULL;

ResourceManager* ResourceManager::sharedResourceManager(void)
{
    if (!s_SharedResourceManager)
    {
        s_SharedResourceManager = new ResourceManager();
        s_SharedResourceManager->init();
    }
    
    return s_SharedResourceManager;
}


ResourceManager::ResourceManager()
:m_pResourceConfig(NULL)
{
    
}

ResourceManager::~ResourceManager()
{
    CC_SAFE_DELETE(m_pResourceConfig);
}


void ResourceManager::purgeResourceManager(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedResourceManager);
}

bool ResourceManager::init()
{
    CSVDecoder *csv = CSVDecoder::create(RESOURCE_CONFIG_PATH);
    
    CSVDecoder::CSVMap map;
    csv->getDataMapByKey("name", map);
  
    m_pResourceConfig = new ResourceConfig(
                                           map["warrior"][1].c_str(),
                                           map["equip"][1].c_str(),
										   map["item"][1].c_str(),
                                           map["skill"][1].c_str(),
                                           map["chapter"][1].c_str(),
                                           map["chapter_thumb"][1].c_str(),
                                           map["goods"][1].c_str(),
                                           map["destiny"][1].c_str(),
                                           map["mask"][1].c_str(),
                                           map["recharge"][1].c_str()
										   ,map["tutorial"][1].c_str()
										   ,map["warrior_head_icon"][1].c_str()
										   ,map["battleMap"][1].c_str()
                                           );
    
    return true;
}

void ResourceManager::removeAnimationCache()
{
    cocos2d::CCArray* pBattleWarrior = cocos2d::CCArray::create();
    DM_GET_FORMATION_MODEL->getBattleWarriors(pBattleWarrior);
    CCObject* obj = NULL;
    CCInteger* pInt = NULL;
    std::vector<std::string> excludeSkeletonNames;
    CCARRAY_FOREACH(pBattleWarrior, obj)
    {
        pInt = static_cast<CCInteger*>(obj);
        if(pInt)
        {
            warrior::WarriorCurInfo* pUnit = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pInt->getValue());
            if(pUnit)
            {
                excludeSkeletonNames.push_back(pUnit->getSkeletonName());
            }
        }
    }
    
    ArmatureDataManager::sharedArmatureDataManager()->removeAllTextureCache(excludeSkeletonNames);
    
}

void ResourceManager::clearCache()
{
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfoShort();
    removeAnimationCache();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCLog("after");
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfoShort();
}

cocos2d::CCTexture2D* ResourceManager::genearateWarriorMaskTexture(unsigned int maskId, cocos2d::CCPoint maskPosition)
{
    CCString* key = CCString::createWithFormat("mask_bg_%d_%f__%f__", maskId, maskPosition.x, maskPosition.y);
    
    CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->textureForKey(key->getCString());
    if (pTexture) {
        return pTexture;
    }
    
    do
    {
        CCSprite* bg = CCSprite::create("PL_ui/mask/mask_bg.png");
        if (bg == NULL) {
            break;
        }
        bg->setAnchorPoint(CCPointZero);
        
        CCSprite* mask = CCSprite::create(CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",maskId)->getCString());
        if (mask == NULL) {
            break;
        }
        mask->setAnchorPoint(CCPointZero);
        mask->setPosition(maskPosition);
        
        CCSize size = bg->getContentSize();
        CCRenderTexture *rt = CCRenderTexture::create(size.width, size.height, kCCTexture2DPixelFormat_RGBA8888);
        rt->beginWithClear(0, 0, 0, 0);
        mask->visit();
        bg->visit();
        rt->end();
        
        //CCLOG("%s", (CCFileUtils::sharedFileUtils()->getWritablePath() + "mask_5_bg.png").c_str());
        //rt->saveToFile((CCFileUtils::sharedFileUtils()->getWritablePath() + "mask_5_bg.png").c_str());
        
        CCImage *pImage = rt->newCCImage(true);
        CCTextureCache::sharedTextureCache()->addUIImage(pImage, key->getCString());
        pTexture = new CCTexture2D();
        if (pTexture && pTexture->initWithImage(pImage)) {
            pTexture->autorelease();
        }
        else{
            CC_SAFE_RELEASE_NULL(pTexture);
        }
        CC_SAFE_RELEASE(pImage);
        
    }while(0);
    
    return pTexture;
}

#pragma mark - 公用方法 -
CCSprite *ResourceManager::maskSprite(CCSprite *sourceSprite , CCSprite *maskSprite , std::string sourcePath , std::string maskPath,cocos2d::CCTexture2DPixelFormat textureFormat)
{
    std::string key = sourcePath+maskPath;
    
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->textureForKey(key.c_str());
    if(pTexture)
    {
        return CCSprite::createWithTexture(pTexture);
    }
    
    CCSize size = maskSprite->getContentSize();
    CCRenderTexture *rt = CCRenderTexture::create(size.width, size.height);
    
    ccBlendFunc targetBlendFunc,sourceBlendFunc;
    targetBlendFunc.src = GL_ONE;
    targetBlendFunc.dst = GL_ZERO;
    sourceBlendFunc.src = GL_DST_ALPHA;
    sourceBlendFunc.dst = GL_ZERO;
    maskSprite->setBlendFunc(targetBlendFunc);
    sourceSprite->setBlendFunc(sourceBlendFunc);
    
    rt->begin();
    maskSprite->visit();
    sourceSprite->visit();
    rt->end();
    
    CCImage *pImage = rt->newCCImage(true);
    CCTexture2D::setDefaultAlphaPixelFormat(textureFormat);
    CCTextureCache::sharedTextureCache()->addUIImage(pImage, key.c_str());
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA8888);
    CC_SAFE_RELEASE(pImage);
    return CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(key.c_str()));
}

cocos2d::CCString* ResourceManager::getResPath(const std::string& _path,unsigned int id)
{
    CCString *path = CCString::createWithFormat(_path.c_str(),id);
    if(!CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(path->getCString())))
    {
        CCLOG("找不到资源%s",path->getCString());
		std::string __path = _path;
        replace(__path,"%d", "%s");
        path = CCString::createWithFormat(__path.c_str(),DEFAULT);
    }
    return path;
}

void ResourceManager::addImage(const char* path,cocos2d::CCTexture2DPixelFormat format)
{
    cocos2d::CCTexture2DPixelFormat o = CCTexture2D::defaultAlphaPixelFormat();
    CCTexture2D::setDefaultAlphaPixelFormat(format);
    CCTextureCache::sharedTextureCache()->addImage(path);
    CCTexture2D::setDefaultAlphaPixelFormat(o);
}

#pragma mark - 技能图标 -
CCSprite *ResourceManager::getSkillIcon(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->skill.c_str(),id);
    addImage(path->getCString(),kCCTexture2DPixelFormat_RGB565);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"skill",1);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    return pIcon;
}

#pragma mark - 装备图标 -
CCSprite *ResourceManager::getEquipIcon(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->equip.c_str(),id);
    CCSprite *pIcon = CCSprite::create(path->getCString());
    return pIcon;
}

cocos2d::CCSprite *ResourceManager::getEquipIcon80P(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->equip.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",5);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.8f);
    return pIcon;
}

cocos2d::CCSprite *ResourceManager::getEquipIcon50P(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->equip.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",4);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.5f);
    return pIcon;
}

cocos2d::CCSprite *ResourceManager::getEquipIcon50PForList(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->equip.c_str(),id);
    //CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",17);
    //DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    //pIcon->setScale(0.35f);

    //return pIcon;

	CCSprite *s = CCSprite::create(path->getCString());
	s->setScale(0.35f);
	return s;

	//CCString *path = getResPath(m_pResourceConfig->equip.c_str(),id);

	//DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(), genearateWarriorMaskTexture(17, ccp(0,100)));
	////pIcon->setScale(1);
	//pIcon->setAnchorPoint(CCPointZero);
	////pIcon->setPosition(ccp(-origin.x * scale, -origin.y * scale));
	////pIcon->setScale(0.4f);
	//return pIcon;
}

cocos2d::CCSprite *ResourceManager::getEquipIcon40P(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->equip.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",1);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.4f);
    return pIcon;
}

cocos2d::CCSprite *ResourceManager::getEquipIcon30P(unsigned int id)
{
	CCString *path = getResPath(m_pResourceConfig->equip.c_str(),id);
	CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",1);
	DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
	pIcon->setScale(0.3f);
	return pIcon;
}

#pragma mark - 道具图标 -
CCSprite *ResourceManager::getItemIcon(unsigned int id)
{
	uint32_t resID = DM_GET_ITEM_MODEL->getResIDByID(id);
    CCString *path = getResPath(m_pResourceConfig->item.c_str(),resID);
    CCSprite *pIcon = CCSprite::create(path->getCString());
    return pIcon;
}


cocos2d::CCSprite *ResourceManager::getItemIcon80P(unsigned int id)
{
	uint32_t resID = DM_GET_ITEM_MODEL->getResIDByID(id);
    CCString *path = getResPath(m_pResourceConfig->item.c_str(),resID);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",5);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.8f);
    return pIcon;
}

cocos2d::CCSprite *ResourceManager::getItemIcon50P(unsigned int id)
{
	uint32_t resID = DM_GET_ITEM_MODEL->getResIDByID(id);
    CCString *path = getResPath(m_pResourceConfig->item.c_str(),resID);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",4);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.5f);
    return pIcon;
}

cocos2d::CCSprite *ResourceManager::getItemIcon50PForList(unsigned int id)
{
	/*uint32_t resID = DM_GET_ITEM_MODEL->getResIDByID(id);
    CCString *path = getResPath(m_pResourceConfig->item.c_str(),resID);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",2);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.5f);
    return pIcon;*/

	uint32_t resID = DM_GET_ITEM_MODEL->getResIDByID(id);
	CCString *path = getResPath(m_pResourceConfig->item.c_str(),resID);
	CCSprite *s = CCSprite::create(path->getCString());
	s->setScale(0.45f);
	return s;
}

cocos2d::CCSprite *ResourceManager::getItemIcon40P(unsigned int id)
{
	uint32_t resID = DM_GET_ITEM_MODEL->getResIDByID(id);
    CCString *path = getResPath(m_pResourceConfig->item.c_str(),resID);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",1);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.4f);
    return pIcon;
}

cocos2d::CCSprite *ResourceManager::getItemIcon30P(unsigned int id)
{
	uint32_t resID = DM_GET_ITEM_MODEL->getResIDByID(id);
	CCString *path = getResPath(m_pResourceConfig->item.c_str(),resID);
	CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",1);
	DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
	pIcon->setScale(0.3f);
	return pIcon;
}


#pragma mark - 商品图标 -

cocos2d::CCSprite * ResourceManager::getGoodsIcon80P(unsigned int id)
{
	CCString *path = getResPath(m_pResourceConfig->goods.c_str(),id);
	CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"item",5);
	DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
	pIcon->setScale(0.8f);
	return pIcon;
}

cocos2d::CCSprite * ResourceManager::getGoodsIcon(unsigned int id)
{
	CCString *path = getResPath(m_pResourceConfig->goods.c_str(),id);
    CCSprite *pIcon = CCSprite::create(path->getCString());
    return pIcon;
}

CCSprite *ResourceManager::getProductIconForShow(unsigned int id)
{

	mall::ProductBase * productBase = DM_GET_MALL_MODEL->getProductBaseByID(id);
    
	if (!productBase) return NULL;
    
    switch (productBase->getType())
    {
        case mall::kProductTypeItem:
        case mall::kProductTypeVipPk:
            return getItemIcon(productBase->getItemId());
        case mall::kProductTypeEquip:
            return getEquipIcon(productBase->getItemId());
        case mall::kProductTypeSoul:
        case mall::kProductTypeWarrior:
            return getWarriorIcon(productBase->getItemId());
        default:
            break;
    }
    
	return getGoodsIcon(productBase->getResId());

}

CCSprite *ResourceManager::getProductIconForList(unsigned int id, unsigned short type)
{
	
	mall::ProductBase * productBase = DM_GET_MALL_MODEL->getProductBaseByID(id);
    
	if (!productBase) return NULL;
    
    switch (productBase->getType())
    {
        case mall::kProductTypeItem:
        case mall::kProductTypeVipPk:
            //return getItemIcon80P(productBase->getItemId());
			return getItemIcon50P(productBase->getItemId());
        case mall::kProductTypeEquip:
            return getEquipIcon80P(productBase->getItemId());
        case mall::kProductTypeSoul:
        case mall::kProductTypeWarrior:
            return getWarriorIcon50PForMall(productBase->getItemId());
        default:
            break;
    }
	return getGoodsIcon80P(productBase->getResId());
}

#pragma mark - 充值图标 -  
cocos2d::CCSprite *ResourceManager::getRechargeIcon(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->recharge.c_str(),id);
    CCSprite *pIcon = CCSprite::create(path->getCString());
	pIcon->setScale(0.3f);
    return pIcon;
}


#pragma mark - 战场资源 -
CCSprite * ResourceManager::getUnitImage(unsigned int sid, bool needFlip/*=false*/, cocos2d::CCPoint point/*=CCPointZero*/)
{
    CCSprite *pIcon = getWarriorIcon(sid, 0.f, needFlip);
	pIcon->setAnchorPoint(ccp(0.5, 0.5));
	pIcon->setPosition(point);
	pIcon->setScale(0.8f);
    return pIcon;
}


Armature* ResourceManager::getUnitAnimate(unsigned int sid)
{
    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCString::createWithFormat("battle/unit/%d.png",sid)->getCString(),UNIT_SK_NAME(sid));
    Armature *armature = Armature::create(UNIT_SK_NAME(sid),UNIT_ARMATURE);
    armature->getAnimation()->playByIndex(0);
    return armature;
}


Armature* ResourceManager::getUnitAnimate2X(unsigned int sid)
{
    Armature *armature = getUnitAnimate(sid);
    armature->setScale(2.0f);
    return armature;
}

ArmatureLoader* ResourceManager::getUnitAnimateAsync(unsigned int sid, ArmatureLoaderDelegate *pDelegate /* = NULL */)
{
    ArmatureLoader *pRet = ArmatureLoader::create();
	pRet->setDelegate(pDelegate);
    pRet->load(CCString::createWithFormat("battle/unit/%d.png",sid)->getCString(), UNIT_SK_NAME(sid),UNIT_ARMATURE,SystemUtil::isNBDevice());
    return pRet;
}

ArmatureLoader* ResourceManager::getUnitAnimate2XAsync(unsigned int sid, ArmatureLoaderDelegate *pDelegate /* = NULL */)
{
    ArmatureLoader *pArmatureLoader = getUnitAnimateAsync(sid, pDelegate);
    pArmatureLoader->setScale(2.0f);
    return pArmatureLoader;
}



#pragma mark - 弟子图标 -
cocos2d::CCSprite * ResourceManager::getWarriorIcon(unsigned int id, float specifiedScale/* = 0.0f*/, bool needFlip/*=false*/)
{
	warrior::WarriorBaseInfo* baseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(id);
	//如果有翻转后的资源图片，则在战斗使用翻转后的武将资源图片
    unsigned int resId = 0;
    if (baseInfo)
    {
        if (needFlip && baseInfo->getFlipRes() > 0)
        {
            resId = baseInfo->getFlipRes();
        }
        else
        {
            resId = baseInfo->getResId();
        }
    }
    else
    {
        resId = id;
    }

    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(), resId);
    CCSprite *pIcon = CCSprite::create(path->getCString());
	/************************************************************************/
	/* pIcon->setAnchorPoint(ccp(0.67f,0.5f));
	pIcon->setScale(2.f);                                                                     */
	/************************************************************************/

	const warrior::IconsInfo* iconInfo = DM_GET_WARRIOR_MODEL->getWarriorIconInfoByType(id, warrior::kIconTypeFull);
	CCPoint offset;
	float scale;
	if (iconInfo) {
		offset = iconInfo->get_Origin();
		scale = iconInfo->get_Scale();
	}
	else{
		offset = ccp(0, 0);
		scale = 1.0f;
	}

	if ( (specifiedScale >= - FLT_EPSILON && specifiedScale <= FLT_EPSILON) == false)
	{
		scale = specifiedScale;
	}
	
	//配置表里offset已为负值
	pIcon->setPosition(ccp(offset.x * scale, offset.y * scale));
	 pIcon->setScale(scale);

    pIcon->setAnchorPoint(ccp(0.5f,0.0f));
	
   
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorHeadIcon( unsigned int id, cocos2d::CCSize size)
{
    warrior::WarriorBaseInfo* baseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(id);
	//如果有翻转后的资源图片，则在战斗使用翻转后的武将资源图片
    unsigned int resId = 0;
    if (baseInfo)
    {
        resId = baseInfo->getResId();
    }
    else
    {
        resId = id;
    }

	CCString* path = getResPath(m_pResourceConfig->warrior_head_icon.c_str(), resId);
	CCSprite* pIcon = CCSprite::create(path->getCString());
	if (pIcon == NULL)
	{
		pIcon = getWarriorIcon40PForFormation(id);
	}
	else
	{
		cocos2d::CCSize originalSize = pIcon->getContentSize();
		pIcon->setScaleX(size.width / originalSize.width);
		pIcon->setScaleY(size.height / originalSize.height);
	}

	return pIcon;
}


cocos2d::CCSprite * ResourceManager::getWarriorIcon50PForLottery(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCSprite *pIcon = CCSprite::create(path->getCString());
    pIcon->setAnchorPoint(ccp(0.5f, 0.0f));
    pIcon->setScale(1.f);
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon50PForActivityList(unsigned int id)
{
    CCSprite* pIcon = getWarriorIcon50PForList(id);
    pIcon->setAnchorPoint(ccp(0.52f,0.477f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon50PForList(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    const warrior::IconsInfo* iconInfo = DM_GET_WARRIOR_MODEL->getWarriorIconInfoByType(id, warrior::kIconTypeWarriorList);
    CCPoint origin;
	float scale;
    if (iconInfo) {
        origin = iconInfo->get_Origin();
		scale = iconInfo->get_Scale();
    }
    else{
        origin = ccp(157, 249);
		scale = 1.0f;
    }

    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(), genearateWarriorMaskTexture(5, origin));
    pIcon->setScale(scale);
    pIcon->setAnchorPoint(CCPointZero);
    pIcon->setPosition(ccp(-origin.x * scale, -origin.y * scale));
    return pIcon;
}


cocos2d::CCSprite * ResourceManager::getWarriorIconForChangeFormation( unsigned int id )
{
	CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
	const warrior::IconsInfo* iconInfo = DM_GET_WARRIOR_MODEL->getWarriorIconInfoByType(id, warrior::kIconTypeChangeFormation);
	CCPoint origin;
	float scale;
	if (iconInfo) {
		origin = iconInfo->get_Origin();
		scale = iconInfo->get_Scale();
	}
	else{
		origin = ccp(157, 249);
		scale = 1.0f;
	}

	DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(), genearateWarriorMaskTexture(15, origin));
	pIcon->setScale(scale);
	pIcon->setAnchorPoint(CCPointZero);
	pIcon->setPosition(ccp(-origin.x * scale, -origin.y * scale));
	return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon50PForFormation(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",6);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(1.f);
    pIcon->setAnchorPoint(ccp(0.66f,0.46f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon50PForDetail(unsigned int id)
{
	warrior::WarriorBaseInfo* baseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(id);
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(), baseInfo ? baseInfo->getResId() : id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",7);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(1.f);
    pIcon->setAnchorPoint(ccp(0.51f,0.57f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon50PForMall(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",9);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(1.f);
    pIcon->setAnchorPoint(ccp(0.61f,0.51f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon50P(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",8);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.76f);
    pIcon->setAnchorPoint(ccp(0.64f,0.52f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon40P(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",8);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.8f);
    pIcon->setAnchorPoint(ccp(0.64f,0.52f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon40PForDestiny(unsigned int id)
{
	CCString *path = getResPath(m_pResourceConfig->warrior_head_icon.c_str(),id);
	CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",16);
	bool isNewIcon = true;
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(path->getCString());
	if(CCFileUtils::sharedFileUtils()->isFileExist(fullPath) == false)
	{
		path = getResPath(m_pResourceConfig->warrior.c_str(),id);
		mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",8);
		isNewIcon = false;
	}
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.8f);
	if (!isNewIcon)
	{
		pIcon->setAnchorPoint(ccp(0.62f,0.53f));
	}
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon60PForFriendFound(unsigned int id)
{
	CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
	CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",13);
	DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
	pIcon->setScale(1.2f);
	pIcon->setAnchorPoint(ccp(0.5,0.5));
	return pIcon;
}


cocos2d::CCSprite * ResourceManager::getWarriorIcon40PForFormation(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",2);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.8f);
    pIcon->setAnchorPoint(ccp(0.61f,0.54f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon40PForMatch(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",3);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.8f);
    pIcon->setAnchorPoint(ccp(0.59f,0.50f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon40PForChangeFormation(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",4);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.8f);
    pIcon->setAnchorPoint(ccp(0.6f,0.54f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon40PForGuildBoss(unsigned int id)
{
	CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
	CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",5);
	DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
	pIcon->setScale(.8f);
	pIcon->setAnchorPoint(ccp(0.7f,0.57f));
	return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon176PForPve(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",10);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(3.52f);
    pIcon->setAnchorPoint(ccp(0.51f,0.51f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIcon50ForSoulHunter(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",13);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.88f);
    pIcon->setAnchorPoint(ccp(0.64f,0.52f));
    return pIcon;
}

cocos2d::CCSprite * ResourceManager::getWarriorIconForTutorial(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",12);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(1.06f);
    pIcon->setAnchorPoint(ccp(0.59f,0.48f));
    return pIcon;
}
cocos2d::CCSprite * ResourceManager::getWarriorIconForAward(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->warrior.c_str(),id);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"warrior",14);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    pIcon->setScale(0.8f);
    pIcon->setAnchorPoint(ccp(0.62f,0.53f));
    return pIcon;
}

cocos2d::CCSprite *ResourceManager::getWarriorIcon25PForPvp(unsigned int id)
{
	CCSprite *pIcon = getWarriorIcon(id);
	float deltax = pIcon->getPositionX();
	pIcon->setPositionX(deltax * (-1.0));
	pIcon->setScale(0.5f);
	pIcon->setFlipX(true);
	return pIcon;
}

#pragma mark - 场景图标 -
CCSprite * ResourceManager::getSceneIcon(unsigned int id)
{
	battle::SceneInfo* pSceneInfo = DM_GET_BATTLE_MODEL->getSceneInfo(id);
    CCString *path = getResPath(m_pResourceConfig->battle_map.c_str(),pSceneInfo->res);
    addImage(path->getCString(), kCCTexture2DPixelFormat_RGB565);
    CCSprite* pImage = CCSprite::create(path->getCString());
    return pImage;
}


CCSprite * ResourceManager::getSceneThumbIcon(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->chapter_thumb.c_str(),id);
    addImage(path->getCString(), kCCTexture2DPixelFormat_RGB565);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"chapterThumb",1);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    ccBlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
    pIcon->setBlendFunc(blendFunc);
    return pIcon;
}


CCSprite * ResourceManager::getSceneThumbIconForTeam(unsigned int id)
{
	CCString *path = getResPath(m_pResourceConfig->chapter_thumb.c_str(),id);
	addImage(path->getCString(), kCCTexture2DPixelFormat_RGB565);
	CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"chapterThumb",2);
	DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
	ccBlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
	pIcon->setBlendFunc(blendFunc);
	return pIcon;
}

cocos2d::CCSprite * ResourceManager::getSettlementTitle(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->chapter.c_str(),id);
    addImage(path->getCString(), kCCTexture2DPixelFormat_RGB565);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"chapter",1);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    ccBlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
    pIcon->setBlendFunc(blendFunc);
    return pIcon;
}

sp::Armature* ResourceManager::getEffect(unsigned int id)
{
    CCString* pSkeletonName = CCString::createWithFormat("SCENE_INFO_%02d",id);
    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCString::createWithFormat("effect/effect%02d.png",id)->getCString(),pSkeletonName->getCString());
    Armature *armature = Armature::create(pSkeletonName->getCString(),CHAPTER_ARMATURE);
    armature->getAnimation()->playByIndex(0);
    return armature;
}

CCSprite * ResourceManager::getChapterIcon(unsigned int id)
{
	CCString *path = getResPath(m_pResourceConfig->chapter.c_str(),id);
	addImage(path->getCString(), kCCTexture2DPixelFormat_RGB565);
	CCSprite* pImage = CCSprite::create(path->getCString());
	return pImage;
}

#pragma mark - 天命 -

cocos2d::CCSprite * ResourceManager::getDestinyIcon(unsigned int id)
{
    CCString *path = getResPath(m_pResourceConfig->destiny.c_str(),id);
    addImage(path->getCString(),kCCTexture2DPixelFormat_RGB565);
    CCString *mask = CCString::createWithFormat(m_pResourceConfig->mask.c_str(),"destiny",1);
    DMaskedSprite* pIcon = DMaskedSprite::create(path->getCString(),mask->getCString());
    return pIcon;
}

#pragma mark - UI 动画 -
Armature* ResourceManager::getUIAnimate(const char* name)
{
    return getUIAnimate(name,sp::BATCHNODE_ZORDER);
}

Armature* ResourceManager::getUIAnimate(const char* name,RENDER_TYPE renderType)
{
//     ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCString::createWithFormat("ui/animation/%s.png",name)->getCString(),name);
//     Armature *armature = Armature::create(name,ANI_SK,renderType);
//     armature->getAnimation()->playByIndex(0);
//     return armature;
	Armature *pArmature = getUIAnimateOnly(name, renderType);
	if (!pArmature)
	{
		return NULL;
	}
	
	pArmature->getAnimation()->playByIndex(0);
	return pArmature;
}

Armature *ResourceManager::getUIAnimateOnly(const char *name,sp::RENDER_TYPE renderType /* = sp::BATCHNODE_ZORDER */)
{
	CCLOG("load ani armature resource: %s", CCString::createWithFormat("PL_ui/animation/%s.png",name)->getCString());
	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCString::createWithFormat("PL_ui/animation/%s.png",name)->getCString(),name);
	Armature *armature = Armature::create(name,ANI_SK,renderType);
	return armature;
}

Armature *ResourceManager::getUIAnimateOnly(std::string &name,sp::RENDER_TYPE renderType /* = sp::BATCHNODE_ZORDER */)
{
	return getUIAnimateOnly(name.c_str(), renderType);
}

#pragma mark - 突破等级 -
cocos2d::CCSprite * ResourceManager::getUpLevel(int8_t level)
{
    if(level<0) return CCSprite::create();
    return CCSprite::create(CCString::createWithFormat("PL_ui/common/%djie.png", level)->getCString());
}





#pragma mark - 技能图标 -
CCSprite *ResourceManager::getTutorialEmotion(unsigned int id)
{
	CCString *path = getResPath(m_pResourceConfig->tutorial.c_str(),id);
	CCSprite *pIcon = CCSprite::create(path->getCString());
	return pIcon;
}
