#include "menuDef.h"

menuDef::menuDef(const char* name, void (*action)(), menuDef* subs)
	: name(name), action(action), subs(subs) {}