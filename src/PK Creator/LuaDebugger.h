#pragma once

struct lua_State;

class QString;

class LuaDebugger
{
private:
	lua_State *m_pLuaVM;

public:
	LuaDebugger();
	~LuaDebugger();

	QString TestScript(const QString &script, const QString &name);

	void ReloadLua();
};

