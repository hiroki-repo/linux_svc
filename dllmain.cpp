// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"
#include "Windows.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

UINT32 svcfuncnf(UINT32 prm_0, UINT32 prm_1, UINT32 prm_2, UINT32 prm_3, UINT32 prm_4, UINT32 prm_5, UINT32 prm_6) {
    return -1;
}

#ifdef _M_IX86
/*
push esi
mov esi,esp
add esi,4
push ebx
push edi
push ebp
mov eax, dword ptr [esi + 4]
mov ebx, dword ptr [esi + 8]
mov ecx, dword ptr [esi + 12]
mov edx, dword ptr [esi + 16]
mov edi, dword ptr [esi + 24]
mov ebp, dword ptr [esi + 28]
mov esi, dword ptr [esi + 20]
int 0x80
pop ebp
pop edi
pop ebx
pop esi
ret
*/
char svcfunc[] = { 0x56 ,0x89 ,0xE6 ,0x83 ,0xC6 ,0x04 ,0x53 ,0x57 ,0x55 ,0x8B ,0x46 ,0x04 ,0x8B ,0x5E ,0x08 ,0x8B ,0x4E ,0x0C ,0x8B ,0x56 ,0x10 ,0x8B ,0x7E ,0x18 ,0x8B ,0x6E ,0x1C ,0x8B ,0x76 ,0x14 ,0xCD ,0x80 ,0x5D ,0x5F ,0x5B ,0x5E ,0xC3 };
#else
#ifdef _M_ARM
/*
mov r7,r0
mov r0,r1
mov r1,r2
mov r2,r3
str.w r4,svctmp+0
str.w r5,svctmp+4
pop {r3-r5}
svc #0
push {r3-r5}
ldr.w r4,svctmp+0
ldr.w r5,svctmp+4
bx lr
svctmp:
*/
char svcfunc[] = { 0x00 ,0x70 ,0xA0 ,0xE1 ,0x01 ,0x00 ,0xA0 ,0xE1 ,0x02 ,0x10 ,0xA0 ,0xE1 ,0x03 ,0x20 ,0xA0 ,0xE1 ,0x18 ,0x40 ,0x8F ,0xE5 ,0x18 ,0x50 ,0x8F ,0xE5 ,0x38 ,0x00 ,0xBD ,0xE8 ,0x00 ,0x00 ,0x00 ,0xEF ,0x38 ,0x00 ,0x2D ,0xE9 ,0x04 ,0x40 ,0x9F ,0xE5 ,0x04 ,0x50 ,0x9F ,0xE5 ,0x1E ,0xFF ,0x2F ,0xE1 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
#else
char svcfunc[256];
memcpy(svcfunc, svcfuncnf, sizeof(svcfuncnf));
#endif
#endif

typedef UINT32 typeofsvc(UINT32, UINT32, UINT32, UINT32, UINT32, UINT32, UINT32);

extern "C" __declspec(dllexport) UINT32 svc(UINT32 prm_0, UINT32 prm_1, UINT32 prm_2, UINT32 prm_3, UINT32 prm_4, UINT32 prm_5, UINT32 prm_6) {
    DWORD tmp=0;
    VirtualProtect(&svcfunc,sizeof(svcfunc), 0x40,&tmp);
    UINT32 svctmp[6];
#ifdef _M_IX86
    __asm {
        push esi
        push ebx
        push edi
        push ebp
        mov eax, [prm_0]
        mov ebx, [prm_1]
        mov ecx, [prm_2]
        mov edx, [prm_3]
        mov esi, [prm_4]
        mov edi, [prm_5]
        mov ebp, [prm_6]
        int 128
        pop ebp
        pop edi
        pop ebx
        pop esi
        //ret
    }
#else
    return ((typeofsvc*)(&svcfunc))(prm_0, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6);
#endif
}
