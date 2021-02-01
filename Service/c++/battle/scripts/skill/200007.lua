g_skill_200007 =
{
	--冲田鹰司&&神鹤佐木
	--对全体造成149%的伤害    八部天龙法	
	-- 名称
		name = '八部天龙法',
	-- 优先级
		priority = 1 ,
	-- 冷却时间(回合数)
		cd = 1,	
	-- 技能类型（0 主动释放, 1 被动)
		type = 0,	
	--技能类型（1普通逻辑，2第一回合可能释放技能,  3技能释放概率 + 10%）
		skill_type = 1,
	--突破等级
		unit_up = 5,

	-- 技能是否可见
		visible = true,	
	-- 触发概率
		trigger_rate = 10,
	-- 起始概率
		prd_n = 8,
	-- 绑定的事件
		bind_event =
		{
			4002
		},
	
	-- 表示了技能包含的buff
		buffs =
		{		
		
		},
	--攻击系数
		attack_parameter = 1.49,
	--成长
		grow = 0.02,
	--buff触发几率
		buff_trigger_rate = 0,
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
		local trigger_rate = g_skill_200007.trigger_rate;

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
		
		local s1 = unit.force:get_unit_by_sid_all(21);
		local s2 = unit.force:get_unit_by_sid_all(30);
    		if(s1 == nil or s2 == nil or s2.is_alive ==false or s1.is_alive ==false )then
    			return result;
    		end;
		
    		result.trigger = true;
		local partners = {s1.id,s2.id};
		local s =1;
		local f =1
		local coactor1;
		result.coactors = {coactor1};
		while s <= 2 do
			if partners[s] ~= unit.id 
			then
				result.coactors[f] = partners[s];
				f = f + 1;
			end
			s = s + 1;
		end
		return result;
	end,
	
	--
	-- frames: 施法者回合数
	-- level: 技能等级
	-- unit: 拥有该技能的单位
	effect = function(frames, level, unit , attacker)	
		local target = unit.target;	
		if target == nil then
		    return;
		end

		local members = {unit.force:get_unit_by_sid_all(21),unit.force:get_unit_by_sid_all(30)};
		local m_total = table.getn(members);
		local up = members[1].up;
		local h,v;
		for  h = 1,m_total,1 do
			if members[h].up < up then
				up = members[h].up;
			end
		end
		
		local skill_lv = up;

		local attack_parameter = g_skill_200007.attack_parameter;
		local grow =  g_skill_200007.grow;

		local units = unit.force.opponent:get_unit_list_table(true);
    		local i = 1;
    		local total = table.getn(units);
    		while i <= total do
			target = units[i];
				local damage= (unit.attack- target.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)  +  unit.event_hurt - target.event_injury    ;
				target:taking_damage(unit, damage  , false);
    			i = i + 1;
    		end
			
    		unit.state.break_action = true;
	end
};