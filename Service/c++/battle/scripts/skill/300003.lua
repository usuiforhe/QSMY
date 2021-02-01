g_skill_300003 =
{
	--恶鬼罗刹 四非凡人
	--第1回合伤害 = 对方队伍血量最高的单位 * （X + random( -0.10 , 0.10 ) )     其他回合伤害 = 对方队伍血量最高的单位 * （Y + random( -0.10 , 0.10 ) ) 
	-- 名称
		name = '四非凡人',
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
		trigger_rate = 100,
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
		attack_parameter = 1.01,
	--成长
		grow = 0.01,
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
		local trigger_rate = g_skill_300003.trigger_rate;
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

		local target = unit.target;
		if target == nil then
		    return;
		end
		local target_id = target.id;
		local attack_parameter = g_skill_300003.attack_parameter;
		local skill_lv = level;
		local grow =  g_skill_300003.grow;
		local buff_trigger_rate = g_skill_300003.buff_trigger_rate;
		local buff_trigger = false ;
		
		--恶鬼
		local targetMax=0;
		
		local units = unit.force.opponent:get_unit_list_table(true);
		local total = table.getn(units);

		if total<=1 then
			targetMax=units[1];
		else 
			--从大到小排序
			local i ,j,temp;
			for i=1,total-1,1 do
				for j=i+1,total,1 do
					if units[i].max_hp<units[j].max_hp then 
						temp=units[i];
						units[i]=units[j];
						units[j]=temp;
					end
				end	
			end
			targetMax=units[1];
		end
		
		local damage=0;
		if unit.is_first_cast == false then
			local i = 1;
    			while i <= total do
				target = units[i];
				damage=units[1].max_hp * ( 0.2 + math.random(200) * 0.001 );
				target:taking_damage(unit, damage, false); 
    				i = i + 1;
    			end
			unit.is_first_cast = true;
		else
			local i = 1;
    			while i <= total do
				target = units[i];
				damage=units[1].max_hp * ( 0.6 + math.random(200) * 0.001 );
				target:taking_damage(unit, damage, false);
    				i = i + 1;
    			end
		end		
		
    		unit.state.break_action = true;
	end
};