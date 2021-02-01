g_skill_500007 =
{
	-- 名称
	-- 小怪随机3治疗
	-- 对友方随机3个目标 100%治疗量	
	name = '小怪随机3治疗',
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
	trigger_rate = 30,
	-- 起始概率
	prd_n = 2,
	-- 绑定的事件
	bind_event =
	{
		4002
	},
	
	--
	-- 表示了技能包含的buff
	--
	buffs =
	{
		
	},
	--攻击系数
	attack_parameter = 1.00,
	--成长
	grow = 0.05,
	-- 验证该技能是否能够触发
	buff_trigger_rate = 100,
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
		local trigger_rate = g_skill_500007.trigger_rate;
		local units = unit.force:get_unit_list_table(true);
		local result =
		{
			trigger = false,
			coactors = {},
		    position = 7,
		    target = 1
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
	--	local buff = g_skill_500007.buffs[1];
		local buff_trigger_rate = g_skill_500007.buff_trigger_rate;
		local units = unit.force:get_unit_list_table(true);
		local total = table.getn(units);
		local b;
		local hurt = 0;
		for b = 1,total,1 do
			if (units[b].hp/units[b].max_hp < 1) then
			hurt = hurt + 1;
			end
		end
		local j,k;
		for  j = 1,total,1 do
			for  k = j,total,1 do
				if units[j].hp/units[j].max_hp > units[k].hp/units[k].max_hp then
				units[j],units[k] = units[k],units[j];
				end
			end
		end
		local attack_parameter = g_skill_500007.attack_parameter;
		local skill_lv = level;
		local grow =  g_skill_500007.grow;
		local heal= (unit.attack *( attack_parameter  +  grow * (skill_lv-1)))    ;
		local l = 1;
		local m = 1;

		local total = table.getn(units);
		if ( total <= 3 ) then
			
		else 
			total = 3;
		end

		while m <= total do
    			units[l]:heal(unit, heal, false);			
			m = m + 1;
			l = l + 1;
    		end


		unit.state.break_action = true;
	end
};