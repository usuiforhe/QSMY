g_skill_100025 =
{
	-- 名称
	-- 师九如
	-- 对友方随机3个目标 104%治疗量	百里光华	友方所有增加5%攻击和5%防御，持续2回合
	name = '师九如',
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
				  defense = 5,
				}
			},
			{			
			buff_id = 1013,		-- 技能buff
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
				  -- trigger_rate = 50,				  
				  --诅咒
				  --  level = 1,
				  --中毒,灼烧,伤害
				  --  factor = 0.1,
				  --回避
				  -- dodge_rate = 100 ,
				  --攻击
				   attack = 5,
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
				  --defense = 5,
				}
			}
	},
	--攻击系数
	attack_parameter = 1.04,
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
		local trigger_rate = g_skill_100025.trigger_rate;
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
	--	local buff = g_skill_100025.buffs[1];
		local buff_trigger_rate = g_skill_100025.buff_trigger_rate;
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
		local attack_parameter = g_skill_100025.attack_parameter;
		local skill_lv = level;
		local grow =  g_skill_100025.grow;
--		local heal= (unit.attack *( attack_parameter  +  grow * (skill_lv-1)))    ;
		local l = 1;
		local m = 1;

		local total = table.getn(units);
		if ( total <= 3 ) then
			
		else 
			total = 3;
		end

		while m <= total do			
			local heal= (unit.attack *( attack_parameter  +  grow * (skill_lv-1) +  units[l].base_heal_rate/100 ))    ;
    			units[l]:heal(unit, heal, false);			
			m = m + 1;
			l = l + 1;
    		end


		-- 全体加BUFF的特殊效果
		if( unit.up < 4 ) then
			
		else
			buff_trigger = script_check_prob(buff_trigger_rate);
			if buff_trigger == true then
				local unitsforce = unit.force:get_unit_list_table(true);
				local kk = 1;
				local total2 = table.getn(unitsforce);
				while kk <= total2 do
					local targetforce = unitsforce[kk];
					targetforce:add_buff(g_skill_100025.buffs[1].buff_id, unit.id, 100025, g_skill_100025.buffs[1]);	
					targetforce:add_buff(g_skill_100025.buffs[2].buff_id, unit.id, 100025, g_skill_100025.buffs[2]);	
					kk = kk + 1;
				end --while
			end --if
		end

		unit.state.break_action = true;
	end
};