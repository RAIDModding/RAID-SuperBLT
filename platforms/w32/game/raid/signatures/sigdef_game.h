CREATE_NORMAL_CALLABLE_SIGNATURE(lua_call, void, "\x48\x63\xC2\x4C\x8B\xD1", "xxxxxx", 0, lua_State*, int, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pcall, int, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x59\x10\x41\x8B\xF0", "xxxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, int, int, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_gettop, int, "\x48\x8B\x41\x28\x48\x2B\x41\x20", "xxxxxxxx", 0, lua_State*)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_settop, void, "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x85\xD2\x78\x7A", "xxxxxxxxxxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_toboolean, int, "\x48\x83\xEC\x28\xE8\xCC\xCC\xCC\xCC\x48\x8B\x08\x33\xC0", "xxxxx????xxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_tointeger, ptrdiff_t, "\x48\x83\xEC\x28\xE8\xCC\xCC\xCC\xCC\x48\x8B\x10\x48\x8B\xCA\x48\xC1\xF9\x2F\x83\xF9\xF2\x73\x0E", "xxxxx????xxxxxxxxxxxxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_tonumber, lua_Number, "\x48\x83\xEC\x28\xE8\xCC\xCC\xCC\xCC\x48\x8B\x10\x48\x8B\xCA\x48\xC1\xF9\x2F\x83\xF9\xF2\x77\x09", "xxxxx????xxxxxxxxxxxxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_tolstring, const char*, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x49\x8B\xF8\x8B\xDA\x48\x8B\xF1", "xxxxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, int, size_t*)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_objlen, size_t, "\x40\x53\x48\x83\xEC\x20\x4C\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x48\x8B\xD8", "xxxxxxxxxx????xxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_touserdata, void*, "\x48\x83\xEC\x28\xE8\xCC\xCC\xCC\xCC\x48\x8B\x00", "xxxxx????xxx", 0, lua_State*, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_loadfilex, int, "\x48\x89\x5C\x24\x20\x55\x56\x57\x48\x81\xEC\x50\x02\x00\x00", "xxxxxxxxxxxxxxx", 0, lua_State*, const char*, const char*)
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_loadstring, int, "\x48\x83\xEC\x48\x48\x89\x54\x24\x30", "xxxxxxxxx", 0, lua_State*, const char*)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_loadx, int, "\x4C\x8B\xDC\x53\x56\x57\x48\x81\xEC\xE0\x00\x00\x00", "xxxxxxxxxxxxx", 0, lua_State*, lua_Reader, void*, const char*, const char*)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_getfield, void, "\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x20\x4D\x8B\xD8", "xxxxxxxxxxxxx", 0, lua_State*, int, const char*)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_setfield, void, "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x4D\x8B\xD8\x48\x8B\xD9", "xxxxxxxxxxxxxxxx", 0, lua_State*, int, const char*)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_createtable, void, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x4C\x8B\x49\x10\x41\x8B\xF8", "xxxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, int, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_newuserdata, void*, "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x4C\x8B\x41\x10\x48\x8B\xFA", "xxxxxxxxxxxxxxxxx", 0, lua_State*, size_t)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_insert, void, "\x4C\x8B\xC9\x85\xD2\x7E\x24", "xxxxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_replace, void, "\x40\x53\x48\x83\xEC\x20\x44\x8B\xC2", "xxxxxxxxx", 0, lua_State*, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_remove, void, "\x4C\x8B\xC1\x85\xD2\x7E\x23\x8D\x42\xFF\x48\x63\xD0\x48\x8B\x41\x20\x48\x8B\x49\x28", "xxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_newstate, lua_State*, "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\xF2\x48\x8B\xE9\x48\x8B\xCE", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0, lua_Alloc, void*)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_close, void, "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\x79\x10\x48\x8B\xB7\xC8\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0, lua_State*)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_gettable, void, "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x4C\x8B\x43\x28\x48\x8B\xD0\x49\x83\xE8\x08", "xxxxxxxxxx????xxxxxxxxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_settable, void, "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x4C\x8B\x43\x28\x48\x8B\xD0\x49\x83\xE8\x10", "xxxxxxxxxx????xxxxxxxxxxx", 0, lua_State*, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_setmetatable, int, "\x48\x83\xEC\x28\x4C\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x49\x8B\x53\x28\x4C\x8B\xC8", "xxxxxxxx????xxxxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_getmetatable, int, "\x48\x83\xEC\x28\x4C\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x48\x8B\x08\x48\x8B\xC1\x48\xC1\xF8\x2F\x83\xF8\xF4", "xxxxxxxx????xxxxxxxxxxxxx", 0, lua_State*, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushnumber, void, "\x48\x8B\x41\x28\xF2\x0F\x11\x08", "xxxxxxxx", 0, lua_State*, lua_Number)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushinteger, void, "\x48\x8B\x41\x28\x0F\x57\xC0", "xxxxxxx", 0, lua_State*, size_t)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushboolean, void, "\x48\x83\xEC\x28\x48\xB8\x00\x00\x00\x00\x00\x80\x00\x00", "xxxxxxxxxxxxxx", 0, lua_State*, bool)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushcclosure, void, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x4C\x8B\x49\x10\x48\x8B\xF2", "xxxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, lua_CFunction, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushlstring, void, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x4C\x8B\x49\x10\x49\x8B\xF8", "xxxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, const char*, size_t)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushstring, void, "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xFA\x48\x8B\xD9\x48\x85\xD2\x75\x0D", "xxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, const char*)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushfstring, const char*, "\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\x48\x83\xEC\x20\x4C\x8B\x41\x10", "xxxxxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, const char*, ...)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_checkstack, int, "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x81\xFA\x40\x1F\x00\x00", "xxxxxxxxxxxxxxx", 0, lua_State*, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushvalue, void, "\x48\x83\xEC\x28\x4C\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x49\x8B\x53\x28\x48\x8B\x00", "xxxxxxxx????xxxxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushnil, void, "\x48\x8B\x41\x28\x48\xC7\x00\xFF\xFF\xFF\xFF\x48\x83\x41\x28\x08\x48\x8B\x41\x28", "xxxxxxxxxxxxxxxxxxxx", 0, lua_State*)

CREATE_NORMAL_CALLABLE_SIGNATURE(luaI_openlib, void, "\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x20\x48\x8B\x41\x28", "xxxxxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, const char*, const luaL_Reg*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_ref, int, "\x48\x89\x5C\x24\x20\x57\x48\x83\xEC\x20\x8D\x82\x0F\x27\x00\x00", "xxxxxxxxxxxxxxxx", 0, lua_State*, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_rawget, void, "\x40\x53\x48\x83\xEC\x20\x4C\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x49\x8B\x5B\x28", "xxxxxxxxxx????xxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_rawset, void, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x48\x8B\x73\x28", "xxxxxxxxxxxxxxxxxxx????xxxx", 0, lua_State*, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_rawgeti, void, "\x40\x53\x48\x83\xEC\x20\x4D\x63\xD8", "xxxxxxxxx", 0, lua_State*, int, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_rawseti, void, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x4D\x63\xD8", "xxxxxxxxxxxxxxxxxx", 0, lua_State*, int, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_type, int, "\x48\x83\xEC\x28\x4C\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x48\x8B\xD0", "xxxxxxxx????xxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_typename, const char*, "\x8D\x42\x01\x48\x98\x48\x8D\x0D", "xxxxxxxx", 0, lua_State*, int)
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_unref, void, "\x45\x85\xC0\x0F\x88\x8E\x00\x00\x00", "xxxxxxxxx", 0, lua_State*, int, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_equal, int, "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x45\x8B\xD8\x48\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x41\x8B\xD3\x48\x8B\xCB\x48\x8B\xF8\xE8\xCC\xCC\xCC\xCC\x4C\x8B\x17", "xxxxxxxxxxxxxxxxx????xxxxxxxxxx????xxx", 0, lua_State*, int, int)

CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_newmetatable, int, "\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x41\x56\x48\x83\xEC\x20\x48\x8B\x41\x10", "xxxxxxxxxxxxxxxxxxxx", 0, lua_State*, const char*)
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_checkudata, int, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x4D\x8B\xD8", "xxxxxxxxxxxxxxxxxx", 0, lua_State*, int, const char*)
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_error, int, "\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\x48\x83\xEC\x20\x4C\x8D\x44\x24\x40", "xxxxxxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, const char*, ...)
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_error, int, "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x48\x85\xC0", "xxxxxxxxxxxxxx????xxx", 0, lua_State*)

CREATE_CALLABLE_CLASS_SIGNATURE(application_update, void*, "\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x56\x41\x57\x48\x83\xEC\x50\x0F\x29\x74\x24\x40\x0F\x29\x7C\x24\x30\x48\x8B\xF9", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0, int*, int*)
CREATE_CALLABLE_CLASS_SIGNATURE(luaL_newstate, int, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x41\x0F\xB6\xF8\x0F\xB6\xF2", "xxxxxxxxxxxxxxxxxxxxxx", 0, char, char, int)

CREATE_CALLABLE_CLASS_SIGNATURE(node_from_xml, void, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x50", "xxxxxxxxxxxxxxx", 0, void*, char*, int*)

CREATE_CALLABLE_CLASS_SIGNATURE(try_open_property_match_resolver, int, "\x48\x89\x54\x24\x10\x55\x53\x56\x57\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\xE9", "xxxxxxxxxxxxxxxxxxxx", 0)

CREATE_NORMAL_CALLABLE_SIGNATURE(Archive_ctor_datastore_thing, int64_t, "\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x30\x48\x8D\x2D", "xxxxxxxxxxxxxxxxxxxxxxx", 0)

CREATE_NORMAL_CALLABLE_SIGNATURE(index2adr, void*, "\x4C\x8B\xC1\x85\xD2\x7E\x23\x8D\x42\xFF\x48\x63\xD0\x48\x8B\x41\x20\x48\x8D\x04\xD0", "xxxxxxxxxxxxxxxxxxxxx", 0, lua_State*, int)