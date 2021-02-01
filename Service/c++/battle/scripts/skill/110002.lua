g_skill_110002 =
{
	-- 名称
	name = '反击眩晕',
	-- 优先级
	priority = 1 ,
	-- 冷却时间(回合数)
	cd = 0,
	
	-- 技能类型（0 主动释放, 1 被动)
	type = 1,
	--技能类型（1普通逻辑，2第一回合可能释放技能）
		skill_type = 1,
	--突破等级
		unit_up = 99,	
	-- 技能是否可见
	visible = true,
	
	-- 触发概率
	trigger_rate = 37,
	-- 起始概率
	prd_n = 1,
	-- 绑定的事件
	bind_event =
	{
		4011
	},
	
	--
	-- 表示了技能包含的buff
	--
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
		attack_parameter = 0.88,
	--成长
		grow = 0.04,
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
		local buff = g_skill_110002.buffs[1];
		local buff_trigger_rate = g_skill_110002.buff_trigger_rate;
		local buff_trigger = false ;
		

				if(  attacker == nil or attacker.sid == unit.sid) then
					
				else
					buff_trigger = script_check_prob(buff_trigger_rate);
					if buff_trigger == true then
					--恶鬼罗刹效果
						if attacker.sid > 10001000 and attacker.sid < 10010000 then

						else
							attacker:add_buff(buff.buff_id, unit.id, 110002, buff);
							
						end
					end    
				end


	end
};