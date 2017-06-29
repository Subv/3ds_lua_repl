#include <3ds.h>
#include <string.h>
#include <stdio.h>
#include <sol.hpp>

/// Registers the 3DS svc functions with the lua runtime
void RegisterSVCs(sol::table& table) {
    #define REG_SVC(name) table[#name] = svc##name
    REG_SVC(OutputDebugString);
    #undef REG_SVC
}

int main() {
    // Initialize graphics
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);

    consoleClear();
    printf("Press START to exit\n");

    sol::state lua;
    // open some common libraries
    lua.open_libraries(sol::lib::base, sol::lib::package);

    try {
        sol::table svc_table = lua.create_named_table("svc");
        RegisterSVCs(svc_table);

        lua.script("svc.OutputDebugString('Hello', 5)", sol::default_on_error);
    } catch (const sol::error& err) {
        printf("Exception was thrown: %s\n", err.what());
    }

    // Main loop
    while (aptMainLoop())
    {
        hidScanInput();

        // Respond to user input
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break; // break in order to return to hbmenu
    }

    gfxExit();
    return 0;
}