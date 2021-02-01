g_skill_200013 =
{
	--佛劍分說&&劍子仙跡&&疏樓龍秀
	--对三行目标造成 180% 伤害，随机对1个攻击目标封技效果，持续1回合，并且3人防御 +20%，持续2回合    三峰齊出	
	-- 名称
		name = '三峰齊出',
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
			buff_id = 1014,		-- 技能buff
			visible = true,	-- 客户端是否可见			
			-- buff的过期类型
			-- 1: 持续一段时间移除 (如果时间为0为无限时间)
			expire_type = 1, 
			interval = 1,	-- 间隔回合数
			last = 2,	-- 持续时间 (回合数)			
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
				  defense = 20,
				}
			},
			{			
			buff_id = 1030,		-- 技能buff
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
				  -- trigger_rate = 50,				  
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
		attack_parameter = 1.80,
	--成长
		grow = 0.01,
	--buff触发几率
		buff_trigger_rate = 30,
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
		local trigger_rate = g_skill_200013.trigger_rate;
		local result =
		{
		    trigger = false,
		    coactors = {},
		    position = 7,
		    target = 2
		};

		local target_1,target_2,target_3,target_4,target_5,target_6,i;
		local is_target_1,is_target_2,is_target_3,is_target_4,is_target_5,is_target_6 = false,false,false,false,false,false;
		if unit.position <=4 then
			i=1 ;   
		elseif unit.position <= 6 and unit.position > 4 then 
			i=3 ;
		elseif unit.position <= 8 and unit.position > 6 then
			i=5 ;
		elseif unit.position <=10 and unit.position > 8 then
			i=7 ;
		else 
			i=9 ;
		end
		target_1 = unit.force.opponent:get_position_unit(i) ;
		target_2 = unit.force.opponent:get_position_unit(i + 1) ;
		target_3 = unit.force.opponent:get_position_unit(i + 2) ; 
		target_4 = unit.force.opponent:get_position_unit(i + 3) ;
		target_5 = unit.force.opponent:get_position_unit(i + 4) ;
		target_6 = unit.force.opponent:get_position_unit(i + 5) ; 
		if target_1 == nil then
			is_target_1 = true;
		elseif target_1.is_alive == false then
			is_target_1 = true;
		else
			is_target_1 = false;
		end
		if target_2 == nil then
			is_target_2 = true;
		elseif target_2.is_alive == false then
			is_target_2 = true;
		else
			is_target_2 = false;
		end
		if target_3 == nil then
			is_target_3 = true;
		elseif target_3.is_alive == false then
			is_target_3 = true;
		else
			is_target_3 = false;
		end
		if target_4 == nil then
			is_target_4 = true;
		elseif target_4.is_alive == false then
			is_target_4 = true;
		else
			is_target_4 = false;
		end
		if target_5 == nil then
			is_target_5 = true;
		elseif target_5.is_alive == false then
			is_target_5 = true;
		else
			is_target_5 = false;
		end
		if target_6 == nil then
			is_target_6 = true;
		elseif target_6.is_alive == false then
			is_target_6 = true;
		else
			is_target_6 = false;
		end
		if is_target_1 and is_target_2 and is_target_3 and is_target_4 and is_target_5 and is_target_6 then
			return result;
		end

	
		local s1 = unit.force:get_unit_by_sid_all(80);
		local s2 = unit.force:get_unit_by_sid_all(81);
		local s3 = unit.force:get_unit_by_sid_all(82);
    		if(s1 == nil or s2 == nil or s3 == nil or s2.is_alive ==false or s1.is_alive ==false or s3.is_alive ==false)then
    			return result;
    		end;
		
    		result.trigger = true;
		local partners = {s1.id,s2.id,s3.id};
		local s =1;
		local f =1
		local coactor1,coactor2;
		result.coactors = {coactor1,coactor2};
		while s <= 3 do
			if partners[s] ~= unit.id then
				result.coactors[f] = partners[s];
				f = f + 1;
			end
			s = s + 1;
		end

		if unit.position <=4 then
			result.position = 3;
		elseif unit.position <= 6 and unit.position > 4 then 
			result.position = 6;
		elseif unit.position <= 8 and unit.position > 6 then
			result.position = 7;
		elseif unit.position <=10 and unit.position > 8 then
			result.position = 10;
		else
			result.position = 11;
		end
		return result;
		
	end,
	
	--
	-- frames: 施法者回合数
	-- level: 技能等级
	-- unit: 拥有该技能的单位
	effect = function(frames, level, unit , attacker)
		local buff = g_skill_200013.buffs[1];
		local buff_trigger_rate = g_skill_200013.buff_trigger_rate;
		local buff_trigger = false ;

		local members = {unit.force:get_unit_by_sid_all(80),unit.force:get_unit_by_sid_all(81),unit.force:get_unit_by_sid_all(82)};
		local m_total = table.getn(members);
		local up = members[1].up;
		local h,v;
		for  h = 1,m_total,1 do
			if members[h].up < up then
				up = members[h].up;
			end
		end
		
		local skill_lv = up;

		local attack_parameter = g_skill_200013.attack_parameter;
		local grow =  g_skill_200013.grow;

		local targets = {};
		local opponent = unit.force.opponent;
		local target;
    		if unit.position <=4 then 
			local i = 1;
			while(i <=6) do 
				target = opponent:get_position_unit(i);
				if(target ~=nil) then
					targets[target.id] = target;
				end
				i = i + 1 ;
			end
		elseif unit.position <= 6 then 
			local i = 3;
			while(i <=8) do 
				target = opponent:get_position_unit(i);
				if(target ~=nil) then
					targets[target.id] = target;
				end
				i = i + 1;
			end;
		elseif unit.position <= 8 then 
			local i = 5;
			while(i <=10) do 
				target = opponent:get_position_unit(i);
				if(target ~=nil) then
					targets[target.id] = target;
				end
				i = i + 1;
			end;
		elseif unit.position <= 10 then 
			local i = 7;
			while(i <= 12) do 
				target = opponent:get_position_unit(i);
				if(target ~=nil) then
					targets[target.id] = target;
				end;
				i = i + 1;
			end;
		else 
			local i = 8;
			while(i <= 14) do 
				target = opponent:get_position_unit(i);
				if(target ~=nil) then
					targets[target.id] = target;
				end;
				i = i + 1;
			end;
		end

		for id,target in pairs(targets) do
			local damage= (unit.attack- target.defense) *( attack_parameter  +  grow * (skill_lv-1))*(1-target.skill_resist/100)  + unit.event_hurt - target.event_injury ;
			target:taking_damage(unit,damage ,false);	
			
			buff_trigger = script_check_prob(buff_trigger_rate);
			if buff_trigger == true then
			--恶鬼罗刹效果
				if target.sid > 10001000 and target.sid < 10010000 then

				else
					target:add_buff(g_skill_200013.buffs[2].buff_id, unit.id, 200013, buff);
				end
				
			end		
		end
				
		
		local unit1=unit.force:get_unit_by_sid(80);
		local unit2=unit.force:get_unit_by_sid(81);
		local unit3=unit.force:get_unit_by_sid(82);

		unit1:add_buff(buff.buff_id, unit1.id, 200013, buff);
		unit2:add_buff(buff.buff_id, unit2.id, 200013, buff);
		unit3:add_buff(buff.buff_id, unit3.id, 200013, buff);

	
    		unit.state.break_action = true;
	end
};