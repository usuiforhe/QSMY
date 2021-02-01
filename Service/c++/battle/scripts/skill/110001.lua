g_skill_110001 =
{
	-- 名称
	name = '诅咒',
	-- 优先级
	priority = 1 ,
	-- 冷却时间(回合数)
	cd = 1,
	
	-- 技能类型（0 主动释放, 1 被动)
	type = 0,
	--技能类型（1普通逻辑，2第一回合可能释放技能）
		skill_type = 1,
	--突破等级
		unit_up = 99,	
	-- 技能是否可见
	visible = true,
	
	-- 触发概率
	trigger_rate = 100,
	-- 起始概率
	prd_n = 1,
	-- 绑定的事件
	bind_event =
	{
		4009
	},
	
	--
	-- 表示了技能包含的buff
	--
	buffs =
	{
	},

	-- 验证该技能是否能够触发
	-- frames: 施法者回合数
	-- level: 技能等级
	-- unit: 拥有该技能的单位
	-- event_id: 触发技能的事件编号
	-- event_args: 事件参数，(参考: /doc/服务端设计/战斗事件说明书)
	-- 返回值：
	-- {
        --		trigger = true,     (是否触发技能)
        --		coactors = {1,2}    (合作施法者编号列表)
        --      position = 7,       (技能释放点位置)
        --      target = 2          (作用方 1:己方 2:敌方 4:全部)
        --  }
	--
	is_trigger = function(frames, level, unit, event_id, event_args)
        local result =
    	{
        	trigger = false,
	        coactors = {},
            position = unit.position,
            target = 1
		};
		local buff_layer = unit:get_buff_layer(1021, 0, 0);
		if buff_layer >= 1 then
		result.trigger = true;
		end
		return result;
	end,

	
	
	--
	-- frames: 施法者回合数
	-- level: 技能等级
	-- unit: 拥有该技能的单位
	effect = function(frames, level, unit , attacker)
		local buff_layer = unit:get_buff_layer(1021, 0, 0);
		local damage = unit.attack * (0.6 +buff_layer * 0.2);
    	unit:taking_damage(unit, damage, false);
    	unit.state.break_action = true;  	
	end
};