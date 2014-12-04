/*
 *  author: xjdrew
 *  date: 2014-06-03 20:38
 */

#include <stdio.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "skiplist.h"

static inline skiplist*
_to_skiplist(lua_State *L) {
    skiplist *sl = lua_touserdata(L, 1);
    if (sl==NULL) {
        luaL_error(L, "must be skiplist object");
    }
    return sl;
}

static int
_insert(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    luaL_checktype(L, 2, LUA_TSTRING);
    double score = luaL_checknumber(L, 3);
    size_t len;
    const char* ptr = lua_tolstring(L, 2, &len);
    slobj *obj = slCreateObj(ptr, len);
    lua_pushunsigned(L, slInsert(sl, score, obj));
    return 0;
}

static int
_remove(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    luaL_checktype(L, 2, LUA_TSTRING);
    size_t len;
    const char* ptr = lua_tolstring(L, 2, &len);
    lua_pushunsigned(L, slRemove(sl, ptr, len));
    return 1;
}

static int
_delete(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    unsigned long rank = luaL_checknumber(L, 2);
    lua_pushlstring(L, slDelete(sl, rank));
    return 1;
}

static int
_rank(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    luaL_checktype(L, 2, LUA_TSTRING);
    size_t len;
    const char* ptr = lua_tolstring(L, 2, &len);
    unsigned long rank = slGetRank(sl, ptr, len);
    if (rank == 0) {
        return 0;
    }
    lua_pushunsigned(L, rank);
    return 1;
}

static int
_count(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    lua_pushunsigned(L, sl->length);
    return 1;
}

static int
_score_by_rank(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    unsigned long rank = luaL_checknumber(L, 2);
    lua_pushnumber(L, slGetScore(sl, rank));
    return 1;
}

static int
_range_by_rank(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    unsigned long r1 = luaL_checkunsigned(L, 2);
    unsigned long r2 = luaL_checkunsigned(L, 3);
    if (r1 > r2) {
        return 0;
    }
    int rangelen = r2 - r1 + 1;
    skiplistNode* node = slGetNodeByRank(sl, r1);
    lua_createtable(L, rangelen, 0);
    int n = 0;
    while (node && (n < rangelen)) {
        n++;
        lua_pushlstring(L, node->obj->ptr, node->obj->length);
        lua_rawseti(L, -2, n);
        node = node->level[0].forward;
    }
    return 1;
}

/*
static int
_range_by_score(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    double s1 = luaL_checknumber(L, 2);
    double s2 = luaL_checknumber(L, 3);
    if (s1 > s2) {
        return 0;
    }
    skiplistNode *node = slFirstInRange(sl, s1, s2);
    lua_newtable(L);
    int n = 0;
    while (node) {
        if(reverse) {
            if(node->score < s2) break;
        } else {
            if(node->score > s2) break;
        }
        n++;
        lua_pushlstring(L, node->obj->ptr, node->obj->length);
        lua_rawseti(L, -2, n);
        node = reverse? node->backward:node->level[0].forward;
    }
    return 1;
}
*/

static int
_dump(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    slDump(sl);
    return 0;
}

static int
_new(lua_State *L) {
    skiplist *psl = slCreate();
    skiplist **sl = (skiplist**)lua_newuserdata(L, sizeof(skiplist*));
    *sl = psl;
    lua_pushvalue(L, lua_upvalueindex(1));
    lua_setmetatable(L, -2);
    return 1;
}

static int
_release(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    printf("collect sl:%p\n", sl);
    slFree(sl);
    return 0;
}

int luaopen_skiplist_c(lua_State *L) {
    luaL_checkversion(L);

    luaL_Reg l[] = {
        {"insert", _insert},
        {"remove", _remove},
        {"delete", _delete},
        {"count", _count},
        {"rank", _rank},
        {"score_by_rank", _score_by_rank},
        {"range_by_rank", _range_by_rank},
        {"range_by_score", _range_by_score},
        {"dump", _dump},
        {NULL, NULL}
    };

    lua_createtable(L, 0, 2);
    luaL_newlib(L, l);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, _release);
    lua_setfield(L, -2, "__gc");

    lua_pushcclosure(L, _new, 1);
    return 1;
}

