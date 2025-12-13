#pragma once

class menuDef {
public:
    menuDef() = default;
    menuDef(const char* name, void (*action)(), menuDef* subs);

    const char* name = nullptr;
    void (*action)() = nullptr;
    menuDef* subs = nullptr;
};
