g_skill_100060 =
{
	-- 苍
	--对前排全体造成 186% 伤害 	天罡伏神	当敌方前排没有单位时，技能将对后排全体造成伤害
	-- 名称
		name = '苍',
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
		attack_parameter = 1.86,
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
	--

	is_trigger = function(frames, level, unit, event_id, event_args)
		local trigger_rate = g_skill_100060.trigger_rate;
		local result =
		{
		    trigger = false,
		    coactors = {},
		    position = 7,
		    target = 2
		};

		if( unit.up < 4 ) then
			local targets1 = {};
			local units = unit.force.opponent:get_unit_list_table(true);
			local i = 1;  --前排
			while(i <=14) do 
				local target = unit.force.opponent:get_position_unit(i);
				if(target ~=nil) then
					targets1[target.id] = target;
				end
					i = i + 2;
			end		
			local flag= false;
			for id,v in pairs(targets1) do
				if( v.is_alive == false ) then
					flag = false;
				else 
					flag = true;
					break;
				end
			end
			if (flag == false) then
				return result;
			end
		else

		end

				
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
		local buff = g_skill_100060.buffs[1];
		local target = unit.target;
		if target == nil then
		    return;
		end
		local target_id = target.id;
		local attack_parameter = g_skill_100060.attack_parameter;
		local skill_lv = level;
		local grow =  g_skill_100060.grow;
		local buff_trigger_rate = g_skill_100060.buff_trigger_rate;
		local buff_trigger = false ;
					
    		local targets1 = {};
		local targets2 = {};
		local opponent = unit.force.opponent;
    		local units = unit.force.opponent:get_unit_list_table(true);
		local i = 1;  --前排
		while(i <=14) do 
			local target = opponent:get_position_unit(i);
			if(target ~=nil) then
				targets1[target.id] = target;
			end
				i = i + 2;
		end
		local j = 2;  --后排
		while(j <=14) do 
			local target = opponent:get_position_unit(j);
			if(target ~=nil) then
				targets2[target.id] = target;
			end
				j = j + 2;
		end

		local flag= false;
		for id,v in pairs(targets1) do
			if( v.is_alive == false ) then
				flag = false;
			else 
				flag = true;
				break;
			end
		end

		if ( flag == false ) then
			if( unit.up < 4 ) then

			else
				for id,v in pairs(targets2) do
					local damage= (unit.attack- v.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)   + unit.event_hurt - v.event_injury  ;
					v:taking_damage(unit,damage ,false);	
				end
			end
		else
			for id,v in pairs(targets1) do
				local damage= (unit.attack- v.defense) *( attack_parameter  +  grow * (skill_lv-1)) + unit.event_hurt - v.event_injury   ;
				v:taking_damage(unit,damage ,false);
				
			end
		end
    		
    		unit.state.break_action = true;
	end
};