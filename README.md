Windows game API Hooker
===============

Hook the game’s *SysCreateWindowExW* api. So we can chagne the game’s window size to *800x600 whitout frame*  without bug.

# usage
1. Compile the *API HOOKER.cpp* to dll
2. Change the game exe filename in *DLL inject.cpp*(line 12)
3. Compile the *DLL inject.cpp* to exe
4. Run *DLL inject.exe*, it will auto open the game and hook the *SysCreateWindowExW* api, change the game window resolution to *800x600*
