local skiplist = require "skiplist"


local test = skiplist.new()
print(test:add("aaa", 99))
test:add("bbb", 11)
test:add("ccc", 5)
test:add("ddd", 15)
print(test:rank("aaa"))
print(test:rank("bbb"))
print(test:rank("asdfff"))
print(test:count())
print(test:remove("bbb"))
print(test:count())
print(test:rank("bbb"))
print(test:score("aaa"))
print(test:score_by_name("bbb"))
print(test:score_by_rank(1))
print(test:delete(1))
local s = test:range_by_rank(1, 5)
local t = test:range_by_score(10, 100)
test:dump()

