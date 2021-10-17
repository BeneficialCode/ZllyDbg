#pragma once

typedef struct _TEB                                          // 66 elements, 0xFB8 bytes (sizeof) 
{
    /*0x000*/     struct _NT_TIB NtTib;                                    // 8 elements, 0x1C bytes (sizeof)
}TEB, * PTEB;