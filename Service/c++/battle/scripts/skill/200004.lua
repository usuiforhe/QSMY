g_skill_200004 =
{
	-- 箫中剑&&宵
	--对后排全体造成 180% 伤害	 天赦罪 	额外增加目标 10% 当前生命值的伤害

	-- 名称
		name = '天赦罪',
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
	
	--
	-- 表示了技能包含的buff
	--
	buffs =
		{		

		},
	--攻击系数
		attack_parameter = 1.8,
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
		local trigger_rate = g_skill_200004.trigger_rate;

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

		local targets2 = {};
		local units = unit.force.opponent:get_unit_list_table(true);
		local i = 2;  --后排
		while(i <=14) do 
			local target = unit.force.opponent:get_position_unit(i);
			if(target ~=nil) then
				targets2[target.id] = target;
			end
				i = i + 2;
		end		
		local flag= false;
		for id,v in pairs(targets2) do
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
		

		local s1 = unit.force:get_unit_by_sid_all(8);
		local s2 = unit.force:get_unit_by_sid_all(12);
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
		local buff = g_skill_200004.buffs[1];
		local target = unit.target;
		if target == nil then
		    return;
		end
		local target_id = target.id;
		local attack_parameter = g_skill_200004.attack_parameter;
		local grow =  g_skill_200004.grow;
		local buff_trigger_rate = g_skill_200004.buff_trigger_rate;
		local buff_trigger = false ;

		local members = {unit.force:get_unit_by_sid_all(8),unit.force:get_unit_by_sid_all(12)};
		local m_total = table.getn(members);
		local up = members[1].up;
		local h,v;
		for  h = 1,m_total,1 do
			if members[h].up < up then
				up = members[h].up;
			end
		end
		
		local skill_lv = up;
					
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
		for id,v in pairs(targets2) do
			if( v.is_alive == false ) then
				flag = false;
			else 
				flag = true;
				break;
			end
		end

		if ( flag == false ) then
			for id,v in pairs(targets1) do
				if target.sid > 10001000 and target.sid < 10010000 then
				
					local damage= (unit.attack-v.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)     + unit.event_hurt - v.event_injury      ;
					v:taking_damage(unit,damage ,false);
				else

					local damage= (unit.attack-v.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)  + v.hp * 0.1   + unit.event_hurt - v.event_injury      ;
					v:taking_damage(unit,damage ,false);

				end
				
			end
		else
			for id,v in pairs(targets2) do
				if target.sid > 10001000 and target.sid < 10010000 then
				
					local damage= (unit.attack-v.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)     + unit.event_hurt - v.event_injury      ;
					v:taking_damage(unit,damage ,false);
				else

					local damage= (unit.attack-v.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)  + v.hp * 0.1   + unit.event_hurt - v.event_injury      ;
					v:taking_damage(unit,damage ,false);

				end
			end
		end



    		unit.state.break_action = true;
	end
};