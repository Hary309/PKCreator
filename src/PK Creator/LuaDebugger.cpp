#include "LuaDebugger.h"

#include <QString>

#include <lua.hpp>

LuaDebugger::LuaDebugger()
{
	m_pLuaVM = luaL_newstate();

	luaL_openlibs(m_pLuaVM);
}

LuaDebugger::~LuaDebugger()
{
	lua_close(m_pLuaVM);
}

const QString LuaDebugger::TestScript(const QString &script, const QString &name)
{
	int result = luaL_loadbuffer(m_pLuaVM, script.toStdString().c_str(), script.size(), name.toStdString().c_str());

	if (result)
	{
		return QString(lua_tostring(m_pLuaVM, -1));
	}

	ReloadLua();

	return QString("No errors");
}

void LuaDebugger::ReloadLua()
{
	lua_close(m_pLuaVM);
	m_pLuaVM = luaL_newstate();
	luaL_openlibs(m_pLuaVM);
}
