gcc -o hello2.exe hello2.cpp -lkernel32 -luser32 -Wl,--subsystem,windows -fvtable-thunks