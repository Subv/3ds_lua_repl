// The MIT License (MIT) 

// Copyright (c) 2017 Subv and contributors

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <3ds.h>
#include <sol.hpp>
#include <stdio.h>
#include "svc_wrapper.hpp"

/// Registers the 3DS svc functions with the lua runtime
void RegisterSVCs(sol::table& table) {
    #define REG_SVC_SIMPLE(name) table[#name] = svc##name
    #define REG_SVC(name, ...) table[#name] = Wrap<decltype(svc##name), __VA_ARGS__>(svc##name)
    
    REG_SVC(CreateEvent, 1, ResetType);
    REG_SVC_SIMPLE(SignalEvent);
    REG_SVC_SIMPLE(ClearEvent);
    REG_SVC_SIMPLE(CloseHandle);
    REG_SVC_SIMPLE(OutputDebugString);

    #undef REG_SVC
    #undef REG_SVC_SIMPLE
}

int main() {
    // Initialize graphics
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);

    consoleClear();
    printf("Press START to exit\n");

    sol::state lua;
    // open some common libraries
    lua.open_libraries(sol::lib::base);

    try {
        sol::table svc_table = lua.create_named_table("svc");
        RegisterSVCs(svc_table);

        lua.script(R"_(svc.OutputDebugString('Hello', 5) 
                       result, event = svc.CreateEvent(0)
                       print('Result: ' .. result .. ' Event: ' .. event))_", 
                    sol::default_on_error);
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