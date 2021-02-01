//
//  AvatarModel.h
//  时装数据
//
//  Created by heven on 14-3-7.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__AvatarModel__
#define __QSMY__AvatarModel__

#include "Model.h"
#include "model/EquipModel.h"

#define D_EVENT_AVATAR_LIST_UPDATE "D_EVENT_AVATAR_LIST_UPDATE"

class AvatarModel;



namespace avatar {
    
	class AvatarPropertyInfo : public ValueObjcet
	{

	public:
		friend class ::AvatarModel;
		AvatarPropertyInfo();
		virtual ~AvatarPropertyInfo();
		CREATE_FUNC(AvatarPropertyInfo);
		D_INIT_DEFAULT(ValueObjcet);

		CC_SYNTHESIZE_READONLY(unsigned int, propertyType, PropertyType);	//类型
		CC_SYNTHESIZE_READONLY(unsigned int, propertyValue, PropertyValue); //值
	};

    class AvatarBaseInfo : public ValueObjcet
    {
    public:
        
        friend class ::AvatarModel;
        
        AvatarBaseInfo();
        virtual ~AvatarBaseInfo();
        CREATE_FUNC(AvatarBaseInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
        CC_SYNTHESIZE_READONLY(unsigned int, id, Id);                   //时装编号
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, name, Name);    //时装名称
        CC_SYNTHESIZE_READONLY(unsigned int, warrior_id, WarriorId);    //可装备弟子编号
		CC_SYNTHESIZE_READONLY(unsigned int, dressType, DressType);    //属性类型
		CC_SYNTHESIZE_READONLY(unsigned int, dressValue, DressValue);    //属性数值
		CC_SYNTHESIZE_READONLY(unsigned int, dressMark, DressMark);    //属性类型(节日等等)
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, desc, Desc);
        CC_SYNTHESIZE(bool, m_isOwn, IsOwn);

        //获取资源编号
        unsigned int getResId(){
            return D_FIX_DRESS_ID(id);
        }
		//获取时装加成类型
		equip::TreasurePropType getPropType(){
			return equip::TreasurePropType(dressType - 1);
		}
        
    };
    
    class AvatarInfo : public ValueObjcet
    {
        
    public:
        AvatarInfo();
        virtual ~AvatarInfo();
        CREATE_FUNC(AvatarInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
        static AvatarInfo* create(unsigned int id);
        
        void parseInfo(const Json::Value& val);
        
        CC_SYNTHESIZE_READONLY(unsigned int, m_nId, Id); //编号
        CC_SYNTHESIZE_READONLY(unsigned int, m_nLv, Level); //等级
        CC_SYNTHESIZE_READONLY(AvatarBaseInfo*, m_pBaseInfo, BaseInfo);
    };
}


class AvatarModel : public Model
{
    
public:
    AvatarModel();
    virtual ~AvatarModel();
    
    virtual bool init();
    
    void parseDresses(const Json::Value& val);
    
    avatar::AvatarInfo* parseDress(const Json::Value& val,avatar::AvatarInfo* pAvatar = NULL);
    
    avatar::AvatarInfo* getAvatarInfoById(unsigned int id);
    avatar::AvatarBaseInfo* getAvatarBaseInfoById(unsigned int id);
    
    bool hasAvatarOfWarrior(unsigned int wid);
    
    cocos2d::CCArray* findAvatarForWarrior(unsigned int wid);
    
    cocos2d::CCArray* findAvatarInfoForWarrior(unsigned int wid);

	cocos2d::CCArray* getPropertyList(unsigned int wid);

    /**
     *  @breif 添加时装
     *
     */
    avatar::AvatarInfo* findAvatarForId(unsigned int aid);

    void addAvatar(avatar::AvatarInfo* pAvatarInfo);
    
private:
    void loadItemBaseData();
    void parseItemBaseData(const CSVDecoder::CSV &val);
    
private:
    cocos2d::CCDictionary *m_pAvatarMap;
    cocos2d::CCDictionary *m_pAvatarBaseInfoMap;
};

#endif /* defined(__QSMY__AvatarModel__) */
