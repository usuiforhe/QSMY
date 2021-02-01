g_skill_200001 =
{
	--师九如&&嗜杀者
	--对后排目标单体造成 370%伤害		七魄绝杀	眩晕，持续1回合
	-- 名称
		name = '七魄绝杀',
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
			{			
			buff_id = 1025,		-- 技能buff
			visible = true,	-- 客户端是否可见			
			-- buff的过期类型
			-- 1: 持续一段时间移除 (如果时间为0为无限时间)
			expire_type = 1, 
			interval = 1,	-- 间隔回合数
			last = 1,	-- 持续时间 (回合数)			
			-- 参数列表
			arg =
				{
				  -- 冰冻
				  -- trigger_rate = 100,				  
				  --诅咒
				  --  level = 1,
				  --中毒,灼烧,伤害
				  --  factor = 0.1,
				  --回避
				  -- dodge_rate = 100 ,
				  --攻击
				  -- attack = 50,
				  --眩晕
				  --治疗
				  --heals = 0.1,
				  --减速
				  --speed = 10,
				  --暴击
				  --critical_rate=10,
				  --格挡
				  --block_rate = 10,
				  --防御
				  --defense = 20,
				}
			}			
		},
	--攻击系数
		attack_parameter = 3.70,
	--成长
		grow = 0.03,
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
		local trigger_rate = g_skill_200001.trigger_rate;
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

		local s1 = unit.force:get_unit_by_sid_all(25);
		local s2 = unit.force:get_unit_by_sid_all(32);
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
	
		local buff = g_skill_200001.buffs[1];
		local attack_parameter = g_skill_200001.attack_parameter;
		local buff_trigger_rate = g_skill_200001.buff_trigger_rate;
		local grow =  g_skill_200001.grow;

		local members = {unit.force:get_unit_by_sid_all(25),unit.force:get_unit_by_sid_all(32)};
		local m_total = table.getn(members);
		local up = members[1].up;
		local h,v;
		for  h = 1,m_total,1 do
			if members[h].up < up then
				up = members[h].up;
			end
		end
		
		local skill_lv = up;
		
		
		local buff_trigger = false ;

		local opponent = unit.force.opponent;
		local target,target1;
		
		if unit.position <=4 then 
			local a = {2,6,10,14,3,7,11};  --  11,7,3,14,10,6,2
			local i = 1;
			while i<8 do
				local j = a[i];
				target = opponent:get_position_unit(j);
				if( (target ~= nil) and (target.is_alive == true) ) then
					target1 = target;
					break;
				end
				i = i + 1 ;
			end
		elseif unit.position <=6 and unit.position>4 then
			local a = {6,2,10,14,3,7,11};
			local i = 1;
			while i<8 do
				local j = a[i]
				target = opponent:get_position_unit(j);
				if( (target ~= nil) and (target.is_alive == true) ) then
					target1 = target;
					break;
				end
				i = i + 1 ;
			end
		elseif unit.position <=7 and unit.position>6 then
			local a = {6,10,2,14,3,7,11};
			local i = 1;
			while i<8 do
				local j = a[i]
				target = opponent:get_position_unit(j);
				if( (target ~= nil) and (target.is_alive == true) ) then
					target1 = target;
					break;
				end
				i = i + 1 ;
			end
		elseif unit.position <=10 and unit.position>7 then
			local a = {10,6,14,2,3,7,11};
			local i = 1;
			while i<8 do
				local j = a[i]
				target = opponent:get_position_unit(j);
				if( (target ~= nil) and (target.is_alive == true) ) then
					target1 = target;
					break;
				end
				i = i + 1 ;
			end
		elseif unit.position <=11 and unit.position>10 then
			local a = {10,14,6,2,3,7,11};
			local i = 1;
			while i<8 do
				local j = a[i]
				target = opponent:get_position_unit(j);
				if( (target ~= nil) and (target.is_alive == true) ) then
					target1 = target;
					break;
				end
				i = i + 1 ;
			end
		else --unit.position <=14 and unit.position>11 then
			local a = {14,10,6,2,3,7,11};
			local i = 1;
			while i<8 do
				local j = a[i]
				target = opponent:get_position_unit(j);
				if( (target ~= nil) and (target.is_alive == true) ) then
					target1 = target;
					break;
				end
				i = i + 1 ;
			end
		end

	
		local damage= (unit.attack- target1.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)   + unit.event_hurt - target1.event_injury     ;
		target1:taking_damage(unit, damage, false); 

		buff_trigger = script_check_prob(buff_trigger_rate);
		if buff_trigger == true then
		--恶鬼罗刹效果
			if target.sid > 10001000 and target.sid < 10010000 then

			else
			target:add_buff(buff.buff_id, unit.id, 200001, buff);
			end
		end    
 		
    		unit.state.break_action = true;
	end
};