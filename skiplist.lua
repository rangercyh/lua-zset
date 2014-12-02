local skiplist = require "skiplist.c"

local mt = {}
mt.__index = mt

--添加
function mt:add(name, score)
	local old = self.tbl[name]
	if old then
		if old == score then
			return
		end
		self.sl:remove(name)
	end
	self.sl:insert(name, score)
	self.tbl[name] = score
	return rank
end

--根据name删除
function mt:remove(name)
	local score = self.tbl[name]
	if score then
		self.sl:remove(name)
		self.tbl[name] = nil
	end
	return rank, score
end

--根据rank删除
function mt:delete(rank)
	local name, score = self.sl:delete(rank)
	return name, score
end

--根据name获得rank
function mt:rank(name)
	return self.sl:rank(name)
end

--返回总数量
function mt:count()
	return self.sl:count()
end

--返回name对应的score
function mt:score_by_name(name)
	return self.tbl[name]
end

--返回rank对应的score
function mt:score_by_rank(rank)
	return self.sl:score_by_rank(rank)
end

--返回rank在begin到end之间的所有数据
--[[
{
	{ name, score }, ...
}
]]
function mt:range_by_rank(begin, end)
	return self.sl:range_by_rank(begin, end)
end

--返回score在begin到end之间的所有数据
function mt:range_by_score(begin, end)
	return self.sl:range_by_score(begin, end)
end

--内存整理
function mt:dump()
	self.sl:dump()
end

local M = {}
function M.new()
	local obj = {}
	obj.sl = skiplist()
	obj.tbl = {}
	return setmetatable(obj, mt)
end
return M
