--
-- 防御提升
--

-- 特别说明：argument中的times原理为，每次调用on_effect函数前，将times + 1。
-- 举例：第一次调用on_effect时, times = 1, 第二次调用on_effect时，times = 2
--
g_buff_1014 =
{    
	--
	-- 当buff被添加一层时调用（第一次添加时不会调用，只会调用on_add）
	-- owner: {unit} buff拥有者
	-- source: {unit} 上buff的人
	-- argument:
	-- {
        --		arg: {lua table},	// skill脚本中的一个buff的table
        --		layer: 3,			// buff 的当前层数
        --		times: 1,			// 当前是第几次调用
        --      source_skill:11  // 来源技能编号
        -- }
	-- 返回值：无
	--
	on_add_layer = function(owner, source, argument)
		owner.defense = owner.defense + math.floor(owner.base_defense * argument.arg.defense/100);
	end,
    
	--
	-- 当buff被移除一层时调用
	-- owner: {unit} buff拥有者
	-- source: {unit} 上buff的人
	-- argument:
	-- {
        --		arg: {lua table},	// skill脚本中的一个buff的table
        --		layer: 3,			// buff 的当前层数
        --		times: 1,			// 当前是第几次调用
        --      source_skill:11  // 来源技能编号
        -- }
	-- 返回值：无
	--
	on_remove_layer = function(owner, source, argument)
	end,
	
	--
	-- 当buff第一次被添加时调用
	-- owner: {unit} buff拥有者
	-- source: {unit} 上buff的人
	-- argument:
	-- {
        --		arg: {lua table},	// skill脚本中的一个buff的table
        --		layer: 3,			// buff 的当前层数
        --		times: 1,			// 当前是第几次调用
        --      source_skill:11  // 来源技能编号
        -- }
	--
	on_add = function(owner, source, argument)
		owner.defense = owner.defense + math.floor(owner.base_defense * argument.arg.defense/100);
	end,
	
	--
	-- 是否需要触发效果
	-- owner: {unit} buff拥有者
	-- source: {unit} 上buff的人
	-- argument:
	-- {
        --		arg: {lua table},	// skill脚本中的一个buff的table
        --		layer: 3,			// buff 的当前层数
        --		times: 1,			// 当前是第几次调用
        --      source_skill:11  // 来源技能编号
        -- }
	-- 返回值：是否触发效果
	--
	is_trigger = function(owner, source, argument)
		return false;
	end,
	
	--
	-- 当buff生效时调用
	-- owner: {unit} buff拥有者
	-- source: {unit} 上buff的人
	-- argument:
	-- {
        --		arg: {lua table},	// skill脚本中的一个buff的table
        --		layer: 3,			// buff 的当前层数
        --		times: 1,			// 当前是第几次调用
        --      source_skill:11  // 来源技能编号
        -- }
	-- 返回值：无
	--
	on_effect = function(owner, source, argument)
	
	end,
	
	--
	-- 当buff退出时调用
	-- owner: {unit} buff拥有者
	-- source: {unit} 上buff的人
	-- argument:
	-- {
        --		arg: {lua table},	// skill脚本中的一个buff的table
        --		layer: 3,			// buff 的当前层数
        --		times: 1,			// 当前是第几次调用
        --      source_skill:11  // 来源技能编号
        -- }
	-- 返回值：无
	--
	on_exit = function(owner, source, argument)
		owner.defense = owner.defense - math.floor(owner.base_defense * argument.arg.defense/100 )* argument.layer;
	end
};

