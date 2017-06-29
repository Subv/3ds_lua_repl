## Nintendo 3DS Lua REPL

3DS Lua REPL is a Nintendo 3DS remote REPL to help debug the Nintendo 3DS kernel calls and OS functions.

## Building

You'll need to install lua as a portlibs into your devKitARM environment to be able to build this program.
A 3DS portlibs makefile that installs lua can be found [here](https://gist.github.com/Subv/4c45fd27bd656f8baaab072d7c23581d)

Run ```make zlib && make install-zlib && make lua && make install``` in the directory of that makefile to install lua.

After you have installed the lua portlib, you can build this program by running ```make``` in the root directory.

## Usage

The 'svc' table contains the exposed 3DS kernel calls, just call them as you would a normal lua function:

```
text = 'Hello!'
svc.OutputDebugString(text, strlen(text))
```

## TODO

Expose the kernel calls (svc) to lua, currently only a minimal subset is exposed under the 'svc' table.

