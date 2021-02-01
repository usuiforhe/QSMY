g_skill_200014 =
{
	-- 轩辕不败&&六祸苍龙
	--对后排全体造成 185% 伤害	 不敗之決       恢复最高伤害100%的血量
	-- 名称
		name = ' 不敗之決',
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
		attack_parameter = 1.85,
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
		local trigger_rate = g_skill_200014.trigger_rate;

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
		

		local s1 = unit.force:get_unit_by_sid_all(18);
		local s2 = unit.force:get_unit_by_sid_all(19);
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
		local buff = g_skill_200014.buffs[1];
		local target = unit.target;
		if target == nil then
		    return;
		end
		local target_id = target.id;
		local attack_parameter = g_skill_200014.attack_parameter;
		local grow =  g_skill_200014.grow;
		local buff_trigger_rate = g_skill_200014.buff_trigger_rate;
		local buff_trigger = false ;

		local members = {unit.force:get_unit_by_sid_all(18),unit.force:get_unit_by_sid_all(19)};
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

		local damageMax=0;

		if ( flag == false ) then
			for id,v in pairs(targets1) do
				local damage= (unit.attack-v.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)     + unit.event_hurt - v.event_injury      ;
				v:taking_damage(unit,damage ,false);
				if damage > damageMax then
					damageMax=damage;
				end
			end
		else
			for id,v in pairs(targets2) do
				local damage= (unit.attack- v.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)    + unit.event_hurt - v.event_injury      ;
				v:taking_damage(unit,damage ,false);
				if damage > damageMax then
					damageMax=damage;
				end
			end
		end

		unit:heal(unit,damageMax,false);

    		unit.state.break_action = true;
	end
};