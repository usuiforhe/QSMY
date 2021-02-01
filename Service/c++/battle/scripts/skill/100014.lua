g_skill_100014 =
{
	--东宫神玺
	--对目标单体造成 328%伤害    神逸风流	25%暴击，持续2回合
	-- 名称
		name = '东宫神玺',
	-- 优先级
		priority = 1 ,
	-- 冷却时间(回合数)
		cd = 1,	
	-- 技能类型（0 主动释放, 1 被动)
		type = 0,	
	--技能类型（1普通逻辑，2第一回合可能释放技能,  3技能释放概率 + 10%）
		skill_type = 3,
	--突破等级
		unit_up = 5,

	-- 技能是否可见
		visible = true,	
	-- 触发概率
		trigger_rate = 35,
	-- 起始概率
		prd_n = 2,
	-- 绑定的事件
		bind_event =
		{
			4002
		},
	
	-- 表示了技能包含的buff
		buffs =
		{		
			{			
			buff_id = 1012,		-- 技能buff
			visible = true,	-- 客户端是否可见			
			-- buff的过期类型
			-- 1: 持续一段时间移除 (如果时间为0为无限时间)
			expire_type = 1, 
			interval = 1,	-- 间隔回合数
			last = 1,	-- 持续时间 (回合数)			
			-- 参数列表
			arg =
				{
				  critical_rate = 25,
				}
			}
		},
	--攻击系数
		attack_parameter = 3.28,
	--成长
		grow = 0.04,
	--buff触发几率
		buff_trigger_rate = 100,
	-- 验证该技能是否能够触发
	-- frames: 施法者回合数
	-- level: 技能等级
	-- unit: 拥有该技能的单位
	-- event_id: 触发技能的事件编号
	-- event_args: 事件参数，(参考: /doc/服务端设计/战斗事件说明书)
	-- 返回值：
	-- {
        --		trigger = true,		-- 是否触发技能
        --		coactors = {1,2} (合作施法者编号列表)
        --      position = 7,       (技能释放点位置)
        --      target = 1          (作用方 1:己方 2:敌方 4:全部)
        --  }

	is_trigger = function(frames, level, unit, event_id, event_args)
		local trigger_rate = g_skill_100014.trigger_rate;
		local result =
		{
		    trigger = false,
		    coactors = {},
		    position = 7,
		    target = 2
		};
		
		if(unit.target == nil) then
			return result;
		end;
		
		result.trigger = true;		
		return result;
	end,
	
	
	--
	-- frames: 施法者回合数
	-- level: 技能等级
	-- unit: 拥有该技能的单位
	effect = function(frames, level, unit , attacker)	
		local buff = g_skill_100014.buffs[1];
		local target = unit.target;	
		if target == nil then
		    return;
		end

		local target_id = target.id;
		local attack_parameter = g_skill_100014.attack_parameter;
		local skill_lv = level;
		local grow =  g_skill_100014.grow;
		local buff_trigger_rate = g_skill_100014.buff_trigger_rate;
		local buff_trigger = false ;

	
		local damage= (unit.attack- target.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)  + unit.event_hurt - target.event_injury      ;
		target:taking_damage(unit, damage, false); 
	
	if( unit.up < 4 ) then
		
	else
		buff_trigger = script_check_prob(buff_trigger_rate);
		if buff_trigger == true then
			unit:add_buff(buff.buff_id, unit.id, 100014, buff);
		end   
	end

	
		 
		
    		
    	unit.state.break_action = true;
	end
};