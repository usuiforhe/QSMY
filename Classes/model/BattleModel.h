//
//  BattleModel.h
//  战斗信息
//
//  Created by wanghejun on 13-3-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BattleModel__
#define __QSMY__BattleModel__

#include "Model.h"
#include "../AppMacros.h"
#include "SettlementModel.h"
#include "SkillModel.h"


class BattleModel;

namespace battle
{
    
    //跳过战斗类型
    enum SkipBattleType
    {
        kSkipBattleTypeSkip     = 0,        //可以跳过
        kSkipBattleTypePVE      = 1,        //未获胜的关卡无法跳过战斗
        kSkipBattleTypeTower    = 2,        //困难难度3星通过可跳过战斗。
        kSkipBattleTypePVP      = 3,        //战况不明，无法跳过。
        kSkipBattleTypeWonder   = 4,        //战况不明，无法跳过。
        kSkipBattleTypeMatch    = 5,        //可以跳过
		kSkipBattleTypeTeam		= 6,		//未获胜的关卡无法跳过战斗
        kSkipBattleTypeOther    = 7,        //备用
    };

	enum BattleType
	{
		kBattleTypeNormal		= 0,		//普通战斗类型
		kBattleTypeMatch			= 1,		//杯赛战斗
		kBattleTypePvp				= 2,		//经济场战斗
		kBattleTypeFriend			= 3,		//好友切磋
	};
    
    class BattleForce;
    class Action;
    
    //战斗势力 1：左边 2：右边
    enum ForceType
    {
        kForceLeft = 1,
        kForceRight
    };
    
    //1：攻击 2：施放技能 3：BUFF生效
    enum ActionType
    {
        kActionAttack=1,
        kActionSkill,
        kActionBuff
    };
    
    //1：受到伤害 2：BUFF被添加 3：BUFF移除 4:治疗 5:单位显示 6:单位隐藏 7:对话
    enum ResultType
    {
        kResultDamage =1,
        kResultAddBuff,
        kResultRemoveBuff,
        kResultHeal,
        kResultShow,
        kResultHide,
        kResultDialog,
    };
    
    enum SkillTargetType
    {
        kSkillTargetTypeSelf = 1
    };
    
    
    enum EffectType
    {
        kEffectTypeNone=0,
        kEffectTypeH=1,        //1只看target_position的横坐标，2只看target_position的纵坐标 0不看坐标和skill一样
        kEffectTypeV
    };
    
    
    enum SkillLayerType    //技能显示图层，0人物上面，1人物下面
    {
        kSkillLayerTypeUp = 0,
        kSkillLayerTypeBottom
    };
    
    enum DialogDirection
    {
		kDialogDirectionNoHead = 0,
        kDialogDirectionLeft = 1,
        kDialogDirectionRight = 2
    };
    
    class BattleDialogInfo:public ValueObjcet
    {
    public:
        BattleDialogInfo()
        :m_Direction(kDialogDirectionLeft)
        ,m_uWid(0)
        ,m_Content("")
        {
            
        }
        CREATE_FUNC(BattleDialogInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
        CC_SYNTHESIZE_READONLY(DialogDirection, m_Direction, Direction);
        CC_SYNTHESIZE_READONLY(std::string, m_Content, Content);
        CC_SYNTHESIZE_READONLY(uint32_t, m_uWid, WarriorId);
        
        friend class ::BattleModel;
    };
    
    class SceneInfo:public ValueObjcet
    {
    public:
        SceneInfo()
        :id(0)
        ,effect(0)
		,res(0)
        {
        }
        CREATE_FUNC(SceneInfo);
        D_INIT_DEFAULT(ValueObjcet);
        uint32_t id;
        uint32_t effect;
		uint32_t res;
    };
    
    class DmgDelay:public ValueObjcet
    {
    public:
        DmgDelay()
        :percent(1.0f)
        ,delay(0.0f)
        {
            
        }
        
        CREATE_FUNC(DmgDelay);
        D_INIT_DEFAULT(ValueObjcet);
        
        float percent;   //伤害分配
        float delay;     //本次伤害延迟(s)
    };
    
    
    class Skill:public ValueObjcet
    {
        
        //CC_SYNTHESIZE_READONLY(uint32_t, sid, Sid);
    public:
        uint32_t sid;
        const uint32_t getSid()
        {
            return sid;
        }
        
        friend class ::BattleModel;
        
    public:
        Skill()
        :dmg_delay(NULL)
        ,effect_type(kEffectTypeNone)
        ,skill_type(skill::kSkillDmg)
        ,skill_layer(kSkillLayerTypeUp)
        ,effect_layer(kSkillLayerTypeUp)
        ,unit_effect_layer(kSkillLayerTypeUp)
        ,unit_effect_num(0)
        ,unit_effect_itr(0)
        ,screen_cover(false)
        ,action(false)
        ,skill_delay(0.f)
        ,effect_delay(0.f)
        ,unit_effect_delay(0.f)
        ,unit_delay(0.f)
        ,buff_delay(0.f)
        ,m_pSkillInfo(NULL)
        ,visible(true)
        {
            
        }
        
        
        CREATE_FUNC(Skill);
        
        bool init()
        {
            if(!ValueObjcet::init()) return false;
            dmg_delay = cocos2d::CCArray::create();
            CC_SAFE_RETAIN(dmg_delay);
            return true;
        }
        
        virtual ~Skill()
        {
            CC_SAFE_RELEASE(dmg_delay);
            CC_SAFE_RELEASE(m_pSkillInfo);
        }
        
        uint8_t getCurrentUnitEffect()
        {
            if(unit_effect_num==0) return 0;
            return unit_effect_itr++%unit_effect_num; //每次返回一个不同的并且合法的效果编号
        }
        
        float getTotalDmgDelay()
        {
            if(dmg_delay==NULL) return 0;
            float rt = 0;
            for (uint32_t i =0; i<dmg_delay->count(); ++i) {
                rt+= ((DmgDelay*)dmg_delay->objectAtIndex(i))->delay;
            }
            return rt;
        }
        
    public:
        bool screen_cover;                      //是否包含遮罩（如果包含遮罩，则遮罩前开始播放单位吟唱动画），0没有，1有
        bool action;                            //是否包含action，0没有，1有
        bool visible;                            //是否需要画面表现
        EffectType effect_type;                 //effect类型：1只看target_position的横坐标，2只看target_position的纵坐标
        skill::SkillType skill_type;                   //skill作用对象，0伤害，1治疗
        
        cocos2d::CCArray *dmg_delay;            //伤害分配以及每个伤害数字的DELAY（该DELAY为从effect开始计算的伤害延迟，每个延迟之间为相对延迟）
        float skill_delay;                      //从action开始计算的技能延迟，-1/1000没有，其他代表DELAY时间（秒）（注：skill是指不需要控制注册点坐标的技能特效）
        float effect_delay;                     //从skill开始计算的技能延迟，-1/1000没有，其他值代表DELAY时间（秒）（注：effect是指需要通过target_position控制注册点坐标的技能特效）
        float unit_effect_delay;                //从effect开始计算的技能延迟，-1/1000没有，其他值代表DELAY时间（秒）（注：unit_effect是指跟单位相关的effect类型）
        float unit_delay;                       //如果每个单位间有链式延迟，每个单位间的DELAY（注：该DELAY将影响到 unit_effect_delay以及dmg_delay)
        float buff_delay;                       //从nit_effect开始计算的buff延迟，-1/1000没有，其他值代表DELAY时间（秒）
        
        SkillLayerType skill_layer;             //skill的图层，0人物上面，1人物下面
        SkillLayerType effect_layer;            //effect的图层，0人物上面，1人物下面
        SkillLayerType unit_effect_layer;       //unit_effect的图层，0人物上面，1人物下面
        
        uint8_t unit_effect_num;                //unit_effect动画效果个数
        
        CC_SYNTHESIZE_READONLY(skill::Skill *, m_pSkillInfo, SkillInfo);
        
    private:
        void setSkillInfo(skill::Skill* pSkillInfo)
        {
            CC_SAFE_RELEASE(m_pSkillInfo);
            CC_SAFE_RETAIN(pSkillInfo);
            m_pSkillInfo = pSkillInfo;
        }
        
    private:
        uint32_t unit_effect_itr;
    };
    
    class Result:public ValueObjcet
    {
    public:
        Result()
        :m_pAction(NULL)
        {
            
        }
        virtual ~Result(){}
        ResultType type;
        uint8_t actor;
        Action *m_pAction; //weak
        
        CREATE_FUNC(Result);
        D_INIT_DEFAULT(ValueObjcet);
    };
    
    class ResultDamage:public Result
    {
    public:
        //伤害量
        uint32_t damange;
        //是否暴击
        bool is_critical;
        //是否躲闪
        bool is_dodge;
        //是否格挡
        bool is_block;
        CREATE_FUNC(ResultDamage);
        D_INIT_DEFAULT(Result);
    };
    
    class ResultHeal:public Result
    {
    public:
        //治疗量
        uint32_t heal;
        //是否暴击
        bool is_critical;
        CREATE_FUNC(ResultHeal);
        D_INIT_DEFAULT(Result);
    };
    
    class ResultBuff:public Result
    {
    public:
        //BUFF编号
        uint32_t buff_id;
        CREATE_FUNC(ResultBuff);
        D_INIT_DEFAULT(Result);
    };
    
    
    class ResultShow:public Result
    {
    public:
        CREATE_FUNC(ResultShow);
        D_INIT_DEFAULT(Result);
    };
    
    class ResultHide:public Result
    {
    public:
        CREATE_FUNC(ResultHide);
        D_INIT_DEFAULT(Result);
    };
    
    class ResultDialog:public Result
    {
    public:
        ResultDialog():dialog_id(0){}
        uint32_t dialog_id;
        CREATE_FUNC(ResultDialog);
        D_INIT_DEFAULT(Result);
    };
    
    class ResultAddBuff:public ResultBuff
    {
    public:
        CREATE_FUNC(ResultAddBuff);
        D_INIT_DEFAULT(ResultBuff);
    };
    
    class ResultRemoveBuff:public ResultBuff
    {
    public:
        CREATE_FUNC(ResultRemoveBuff);
        D_INIT_DEFAULT(ResultBuff);
    };
    
    
    class Action:public ValueObjcet
    {
    public:
        Action():m_pResults(NULL)
        {
            
        }
        virtual ~Action()
        {
            CC_SAFE_RELEASE(m_pResults);
        }
        
        CREATE_FUNC(Action);
        bool init()
        {
            if(!ValueObjcet::init()) return false;
            m_pResults =cocos2d::CCArray::create();
            CC_SAFE_RETAIN(m_pResults);
            return true;
        }
        
        ActionType type;
        uint8_t actor;
        
        cocos2d::CCArray* getResults()
        {
            return m_pResults;
        }
        
        void addResult(Result* pResult)
        {
            Result* _pResult = NULL;
            for (uint32_t i=0; i<m_pResults->count(); ++i) {
                _pResult = (Result*)m_pResults->objectAtIndex(i);
                if(_pResult->actor==pResult->actor)
                {
                    m_pResults->insertObject(pResult, i);
                    return;
                }
            }
            m_pResults->addObject(pResult);
        }
    private:
        cocos2d::CCArray *m_pResults;
    };
    
    
    class ActionAttack :public Action
    {
    public:
        CREATE_FUNC(ActionAttack);
        D_INIT_DEFAULT(Action);
    };
    
    class ActionSkill :public Action
    {
    public:
        ActionSkill():m_pCoactors(new std::vector<uint32_t>())
        {
            
        }
        
        virtual ~ActionSkill()
        {
            CC_SAFE_DELETE(m_pCoactors);
        }
        
        CREATE_FUNC(ActionSkill);
        D_INIT_DEFAULT(Action);
        
        //辅助施法者
        std::vector<uint32_t> *m_pCoactors;
        //技能编号
        uint32_t skill_id;
        uint8_t position;
        SkillTargetType target;
    };
    
    class ActionBuff :public Action
    {
    public:
        //BUFF编号
        uint32_t buff_id;
        CREATE_FUNC(ActionBuff);
        D_INIT_DEFAULT(Action);
    };
    
    
    
    class BattleUnit:public ValueObjcet
    {
    public:
        BattleUnit()
        :m_pForce(NULL)
        ,is_boss(false)
        ,dress_id(0)
        ,is_show(true)
        {
            
        }
        
        CREATE_FUNC(BattleUnit);
        D_INIT_DEFAULT(ValueObjcet);
        
        uint32_t getAvatarId() const;
        
        uint8_t id;
        uint32_t sid;
        uint8_t position;
        uint8_t size;
        uint16_t level;
        int32_t max_hp;
        int32_t hp;
        float fight_factor;
        bool is_boss;
        uint32_t dress_id;
        bool is_show;
        BattleForce   *m_pForce; //weak
        
    };
    
    class BattleForce:public ValueObjcet
    {
        //CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, name, Name)
        
    public:
        
        CREATE_FUNC(BattleForce);
        D_INIT_DEFAULT(ValueObjcet);
        uint8_t id;
        uint32_t speed;
        uint32_t user_id;
        bool is_mob;
        uint16_t level;
        std::string name;
        friend class BattleModel;
    };
    
    
    class BattleInfo:public ValueObjcet
    {
    public:
        BattleInfo()
        :m_pForces(NULL)
        ,m_pActions(NULL)
        ,m_pUnits(NULL)
		,triggerCoAction(false)
        {
            
        }
        
        CREATE_FUNC(BattleInfo);
        bool init()
        {
            if(!ValueObjcet::init()) return false;
            m_pForces = cocos2d::CCArray::create();
            m_pActions = cocos2d::CCArray::create();
            m_pUnits = cocos2d::CCArray::create();
            
            CC_SAFE_RETAIN(m_pForces);
            CC_SAFE_RETAIN(m_pActions);
            CC_SAFE_RETAIN(m_pUnits);
            
            return true;
        }
        
        virtual ~BattleInfo()
        {
            CC_SAFE_RELEASE(m_pForces);
            CC_SAFE_RELEASE(m_pActions);
            CC_SAFE_RELEASE(m_pUnits);
        }
        
        uint32_t scene;
        uint32_t  map;
        uint8_t grade;
        ForceType winner;
        cocos2d::CCArray *m_pForces;   //map
        cocos2d::CCArray *m_pActions;
        cocos2d::CCArray *m_pUnits;     //map
		bool triggerCoAction;  //是否触发组合技

        
        Action* getActionByIndex(uint32_t index)
        {
            if(index>=m_pActions->count()) return NULL;
            return (Action*)m_pActions->objectAtIndex(index);
        }
        
        bool isWin()
        {
            return winner==kForceLeft;
        }
        
		std::set<uint32_t> flipUnits;   //战场中所有出现的flip units
        std::set<uint32_t> units;   //战场中所有出现的units
        std::set<uint32_t> skills;  //战场中所有出现的skills
        std::set<uint32_t> buffs;   //战场中所有出现的buffs
    public:
        /*
         * 通过势力类型获取势力
         */
        const BattleForce* getForce(ForceType type)
        {
            CCObject* obj = NULL;
            BattleForce * pForce = NULL;
            CCARRAY_FOREACH(m_pForces, obj)
            {
                pForce = (BattleForce*)obj;
                if(pForce->id == type) return pForce;
            }
            return NULL;
        }
        
        /**
         * 通过单位战斗编号获取战斗单位
         */
        const BattleUnit* getUnit(uint8_t id)
        {
            CCObject* obj = NULL;
            BattleUnit * pUnit = NULL;
            CCARRAY_FOREACH(m_pUnits, obj)
            {
                pUnit = (BattleUnit*)obj;
                if(pUnit->id == id) return pUnit;
            }
            return NULL;
        }
        
        void validBattleData();
        
    };
    
}





class BattleModel : public Model
{
    
    CC_SYNTHESIZE_READONLY(battle::BattleInfo *, m_pBattleInfo, BattleInfo)
    CC_SYNTHESIZE_RETAIN(settlement::SettlementInfo *, m_pSettlementInfo, SettlementInfo)
    
    /**
     *	@brief	是否显示胜利失败动画
     *
     */
    CC_SYNTHESIZE(bool, m_bShowResult, IsShowResult);

    /**
     *	@brief	是否显示战斗开始动画
     *
     */
    CC_SYNTHESIZE(bool, m_bShowStart, IsShowStart);

	/**
     *	@brief	设置战斗类型(杯赛，论剑还是普通战斗)
     *
     */
	CC_SYNTHESIZE(battle::BattleType, m_battleType, BattleType);
	
	/**
     *	@brief	是否缘分加成动画
     *
     */
	CC_SYNTHESIZE(bool, m_bShowFateAni, ShowFateAni);
    
    typedef cocos2d::CCDictionary SkillMap; //skillinfo cache
public:
    
    
    BattleModel();
    virtual ~BattleModel();
    virtual bool init();
    
    void cleanup();
    void parseInfo(const Json::Value &val);
    void parseInfo(std::vector<char> & vec);
    
    void parseSkill(uint32_t sid, const char* filePath);
    battle::Skill* getSkill(uint32_t sid);
    
    battle::SceneInfo* getSceneInfo(uint32_t id);
    
    void setSpeedUp(bool val);
    bool getSpeedUp();
    float getBattleSpeed();
    
    
    /**
     *	@brief	获取关卡战场开场对话信息
     *
     *	@return	战场开场对话信息
     */
    cocos2d::CCArray* getBattleDialog();

    
    /**
     *	@brief	获取战斗单位对话信息
     *
     *	@param 	dialog_id 	对话信息编号
     *
     *	@return	战斗单位对话信息
     */
    cocos2d::CCArray* getUnitDialog(uint32_t dialog_id);


    
private:
    
    void loadData();
    
    void parseSceneInfo(const CSVDecoder::CSV &val);
    void parseBattleDialog(const CSVDecoder::CSV &val);
    void parseUnitDialog(const CSVDecoder::CSV &val);
    
    
    cocos2d::CCArray* createDialog(cocos2d::CCDictionary* m_pDialogMap,uint32_t id);
    
    void parseForce(cocos2d::CCArray * const pForces,const Json::Value &val);
    void parseForce(cocos2d::CCArray * const pForces,std::vector<char> & vec, std::vector<char>::iterator & iter);
    
    void parseUnit(cocos2d::CCArray * const pUnits,battle::BattleForce * const pForce,const Json::Value &val);
    void parseUnit(cocos2d::CCArray * const pUnits,battle::BattleForce * const pForce,std::vector<char> & vec, std::vector<char>::iterator & iter);
    
    void parseAction(cocos2d::CCArray * const pActions,const Json::Value &val);
    void parseAction(cocos2d::CCArray * const pActions,std::vector<char> & vec, std::vector<char>::iterator & iter);
    void parseResult(battle::Action * const pAction,const Json::Value &val);
    void parseResult(battle::Action * const pAction,std::vector<char> & vec, std::vector<char>::iterator & iter);
    void parseCoactors(std::vector<uint32_t> * const pCoactors,const Json::Value &val);
    void parseCoactors(std::vector<uint32_t> * const pCoactors,std::vector<char> & vec, std::vector<char>::iterator & iter);
    
    
    void parseDmgDelay(cocos2d::CCArray* const pDmgDelays,const Json::Value &val);
    void parseSkill(const Json::Value &val);
    
    bool canSpeedUp();
private:
    
    SkillMap *m_Skills;
    cocos2d::CCDictionary* m_pBattleDialogs;
    cocos2d::CCDictionary* m_pUnitDialogs;
    cocos2d::CCDictionary* m_pSceneInfos;
    
    bool m_bSpeedUp;
    
#define DAMAGE_RESULT_CRITICAL_FLAG 0x01
#define DAMAGE_RESULT_DODGE_FLAG 0x02
#define DAMAGE_RESULT_BLOCK_FLAG 0x04
};


#endif /* defined(__QSMY__BattleModel__) */
