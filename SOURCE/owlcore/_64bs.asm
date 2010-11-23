;//---------------------------------------------------------------------------
;// ObjectWindows
;//
;// 64 bit support for OWL
;//
;// void __stdcall _u64add(uint8* l, uint8* r);
;// void __stdcall _u64sub(uint8* l, uint8* r);
;// int  _stdcall _u64cmp(uint8* l, uint8* r);
;// int  _stdcall _i64cmp(uint8* l, uint8* r);
;// void _stdcall _u64mul(uint8* l, uint8* r, uint8* p);
;// void _stdcall _64div(uint8* l, uint8* r, uint8* rem);
;// void _stdcall _64and(uint8* l, uint8* r);
;//
;// Created by Yura Bidus  May 23 1998
;//
;//---------------------------------------------------------------------------
        .386p
        .model   flat
        title   _64supp.asm
_TEXT   segment dword public use32 'CODE'

;//---------------------------------------------------------------------------
;
;void __stdcall _u64add(uint8* l, uint8* r);
;

public _u64add
_u64add proc near

u64l    EQU [ebp+8]  ; left
u64r    EQU [ebp+12] ; right

    push ebp
    mov  ebp,esp
    push esi
    push edi

    mov esi,     u64l
    mov edi,     u64r

    mov eax,  dword ptr [edi]
    add dword ptr [esi],  eax
    mov eax,   dword ptr [edi+4]
    adc dword ptr [esi+4], eax

    pop edi
    pop esi
    pop ebp

    ret 4*2
_u64add endp


;//---------------------------------------------------------------------------
;
;void __stdcall _u64sub(uint8* l, uint8* r);
;

public _u64sub
_u64sub proc near

u64l    EQU [ebp+8]  ; left
u64r    EQU [ebp+12] ; right

    push ebp
    mov  ebp,esp
    push esi
    push edi

    mov esi,  u64l
    mov edi,  u64r

    mov eax,  dword ptr [edi]
    sub [esi],  eax
    mov eax,  dword ptr [edi+4]
    sbb [esi+4],eax

    pop edi
    pop esi
    pop ebp

  ret 4*2
_u64sub endp


;//---------------------------------------------------------------------------
;
;int  _stdcall _u64cmp(uint8* l, uint8* r);
;

public _u64cmp
_u64cmp proc near

u64l    EQU [ebp+8]  ; left
u64r    EQU [ebp+12] ; right

    push ebp
    mov  ebp,esp
    push esi
    push edi

    mov esi,     u64l
    mov edi,     u64r

    mov eax,  dword ptr [edi]
    cmp [esi],  eax
    mov  eax,  1   ;// set flag greater
    ja  _u64cmpEnd
    jb  _u64cmpLess
    mov eax,  dword ptr [edi+4]
    cmp [esi+4],eax
    mov  eax,  1   ;// set flag greater
    ja  _u64cmpEnd
    jb  _u64cmpLess
    mov  eax,  0
    jmp  _u64cmpEnd

_u64cmpLess:
    neg  eax

_u64cmpEnd:
    pop edi
    pop esi
    pop ebp

  ret 4*2
_u64cmp endp

;//---------------------------------------------------------------------------
;
;int  _stdcall _i64cmp(uint8* l, uint8* r);
;

public _i64cmp
_i64cmp proc near

i64l    EQU [ebp+8]  ; left
i64r    EQU [ebp+12] ; right

    push ebp
    mov  ebp,esp
    push esi
    push edi

    mov esi,     i64l
    mov edi,     i64r

    mov eax,    dword ptr [edi]
    cmp [esi],  eax
    mov  eax,  1   ;// set flag greater
    jg  _i64cmpEnd
    jl  _i64cmpLess
    mov eax,  dword ptr [edi+4]
    cmp [esi+4],eax
    mov  eax,  1   ;// set flag greater
    jg  _i64cmpEnd
    jl  _i64cmpLess
    mov  eax,  0
    jmp  _i64cmpEnd

_i64cmpLess:
    neg  eax

_i64cmpEnd:
    pop edi
    pop esi
    pop ebp

  ret 4*2
_i64cmp endp

;//-----------------------------------------------------------------------------
;
;void _stdcall _u64mul(uint8* l, uint8* r, uint8* p);
;
public _u64mul
_u64mul proc near

u64l    EQU [ebp+8]  ; multiplicand
u64r    EQU [ebp+12] ; multiplyer
u128p    EQU [ebp+16] ; product

    push ebp
    mov  ebp,esp
    push esi
    push edi
    push ebx
    push ecx
    push edx

  mov esi,  u64l
  mov edi,  u64r

  ;// Multiply the L.O. word of Multiplier times Multiplicand:
  mov eax,  dword ptr [edi]
  mov ebx,  eax       ;//Save Multiplier val
    mul dword ptr [esi]      ;//Multiply L.O. dwords
    mov  ecx,  u128p
    mov  dword ptr [ecx],eax   ;//Save partial product
    mov  ecx,  edx       ;//Save H.O. dword

    mov eax, ebx         ;//Get Multiplier in EBX
    mul dword ptr [esi+4]     ;//Multiply L.O. * H.O.
  add eax, ecx         ;//Add partial product
  adc edx, 0           ;//Don’t forget carry!
  mov ebx, eax         ;//Save partial product
    mov ecx, edx         ;//for now.

    ;//Multiply the H.O. word of Multiplier times Multiplicand:
    mov eax, dword ptr [edi+4]  ;//Get H.O. Multiplier
  mul dword ptr [esi]      ;//Times L.O. dword
  add eax, ebx         ;//Add partial product
    mov  ebx, u128p
  mov dword ptr [ebx+2], eax  ;//Save partial product
    adc ecx, edx         ;//Add in carry/H.O.!
    mov eax, dword ptr [edi+4]  ;//Multiply the H.O.
    mul dword ptr [esi+4]     ;//words together.
  add eax, ecx         ;//Add partial product
  adc edx, 0           ;//Don’t forget carry!
  mov dword ptr [ebx+8], eax  ;//Save partial product
  mov dword ptr [ebx+12],edx

    pop edx
    pop ecx
    pop ebx
    pop edi
    pop esi
    pop ebp

    ret 4*3
_u64mul endp


;//---------------------------------------------------------------------------
;
;void _64div(uint8* l, uint8* r, uint8* rem)
;
public _64div
_64div proc near


ndivd    EQU [ebp+8]  ; dividend
ndiv    EQU [ebp+12] ; divisor
nrem    EQU [ebp+16] ; remainder

    push ebp
    mov  ebp,esp
    push esi
    push edi
    push ecx
    push edx

  ;// check divisor for zerro
    mov edi,  ndiv
    mov ecx,    64/32    ;//Count off 64/32 bits in ecx
  xor  eax,  eax
  
  _64divCheckZerro:
  cmp dword ptr [edi], 0  
  je  _64divCheckZerroNext
  mov  eax,  1

  _64divCheckZerroNext:
    add edi,  4
  dec  ecx
  jnz  _64divCheckZerro
  
  ;// if zerro will be exeption
  mov  ecx,  eax   
  xor edx,  edx
  div  ecx     ;// edx:eax/ecx
 
    mov esi,  ndivd
    mov edi,  ndiv
    mov edx,  nrem
    mov ecx,    64      ;//Count off 64 bits in ecx

    ;// Assume Dividend (and Quotient) is [esi], Divisor is in [edi],
    ;// and Remainder is in [edx].
    mov dword ptr [edx],   0  ;//Set remainder to zero
    mov dword ptr [edx+4],   0  ;//Set remainder to zero

  _64divBitLoop:                ;//128 bit SHL operation
    shl dword ptr [esi],  1   ;// 
    rcl dword ptr [esi+4],  1   ;//
    rcl dword ptr [edx],  1   ;// 
    rcl dword ptr [edx+4],   1   ;//
    
  mov  eax, dword ptr [edx+4]  ;//Compare H.O. words of Rem,
    cmp eax, dword ptr [edi+4]  ;//Divisor.
    ja _64divGoesInto           ;//
    jb _64divTryNext
    mov  eax, dword ptr [edx]
    cmp eax, dword ptr [edi]  ;//Compare L.O. words.
    jb _64divTryNext
  _64divGoesInto:
    mov  eax,  dword ptr [edi]   ;//
    sub dword ptr [edx], eax   ;//Remainder := Remainder - Divisor
    mov  eax,  dword ptr [edi+4] ;// 
    sbb  dword ptr [edx+4], eax   ;//
    inc dword ptr [esi]     ;//Set L.O. bit of AX
  _64divTryNext:
    dec ecx ;//Repeat 64 times.
    jne _64divBitLoop

    pop edx
    pop ecx
    pop edi
    pop esi
    pop ebp


    ret 3*4

_64div endp


;//-----------------------------------------------------------------------------
;
;void _stdcall _u64neg(uint8* u64);
;
public _u64neg
_u64neg proc near
u64 EQU [ebp+8]  ;

    push ebp
    mov  ebp,esp
    push esi

    mov esi,  u64

    mov eax,  0
    sub  eax,  dword ptr [esi]
    sbb eax, 0
    mov  dword ptr [esi], eax
  mov eax,  0
    sub  eax,  dword ptr [esi+4]
    sbb eax, 0
    mov  dword ptr [esi+4], eax

    pop esi
    pop ebp
    ret 1*4
_u64neg endp
;

;//-----------------------------------------------------------------------------
;
;void _stdcall _64shl(uint8* u64, uint32 count);
;
public _64shl
_64shl proc near
u64    EQU [ebp+8];
count EQU [ebp+12];
    push ebp
    mov  ebp,esp
    push esi
    push ecx

  mov  esi,  u64
  mov  ecx,  count

  _64shlLoop:
    shl dword ptr [esi],  1   ;// 64 bit shl
    rcl dword ptr [esi+4],  1   ;//
  dec ecx
  jnz _64shlLoop  

    pop ecx
    pop esi
    pop ebp
  ret 2*4
_64shl endp
;


;//-----------------------------------------------------------------------------
;
;void _stdcall _64shr(uint8* u64, uint32 count);
;
public _64shr
_64shr proc near
u64    EQU [ebp+8]  ;
count    EQU [ebp+12]  ;
    push ebp
    mov  ebp,esp
    push esi
    push ecx

  mov  esi,  u64
  mov  ecx,  count

  _64shrLoop:
    shr dword ptr [esi+4],  1   ;// 64 bit shl
    rcr dword ptr [esi],  1   ;//
  dec ecx
  jnz _64shrLoop

    pop ecx
    pop esi
    pop ebp
  ret 2*4
_64shr endp


;//---------------------------------------------------------------------------
;// and
;void _64and(uint8* l, uint8* r)
;
public _64and
_64and proc near

_64l   EQU [ebp+8]   ; first
_64r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

  mov  esi, _64l
  mov  edi, _64r

    mov     eax, dword ptr [esi]
    and     eax, dword ptr [edi]
    mov     dword ptr [esi], eax

    mov     eax, dword ptr [esi+4]
    and     eax, dword ptr [edi+4]
    mov     dword ptr [esi+4], eax

    pop edi
    pop esi
    pop ebp
    ret 2*4
_64and endp

;//---------------------------------------------------------------------------
; or 64 + 64
;void _64or(uint8* l, uint8* r)
;
public _64or
_64or proc near

_64l   EQU [ebp+8]   ; first
_64r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

  mov  esi, _64l
  mov  edi, _64r

    mov     eax, dword ptr [esi]
    or    eax, dword ptr [edi]
    mov     dword ptr [esi], eax

    mov     eax, dword ptr [esi+4]
    or    eax, dword ptr [edi+4]
    mov     dword ptr [esi+4], eax

    pop edi
    pop esi
    pop ebp
    ret 2*4

_64or endp


;//---------------------------------------------------------------------------
; or 64 + 64
;void _64xor(uint8* l, uint8* r)
;
public _64xor
_64xor proc near

_64l   EQU [ebp+8]   ; first
_64r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

  mov  esi, _64l
  mov  edi, _64r

    mov     eax, dword ptr [esi]
    xor    eax, dword ptr [edi]
    mov     dword ptr [esi], eax

    mov     eax, dword ptr [esi+4]
    xor    eax, dword ptr [edi+4]
    mov     dword ptr [esi+4], eax


    pop edi
    pop esi
    pop ebp
    ret 2*4

_64xor endp


;//---------------------------------------------------------------------------
; not 64
;void _64not(uint8* l)
;
public _64not
_64not proc near

_64   EQU [ebp+8]   ; first

    push ebp
    mov  ebp,esp
    push esi

  mov  esi, _64

    mov  eax, dword ptr [esi]
    not  eax
    mov  dword ptr [esi], eax

  mov  eax, dword ptr [esi+4]
    not  eax
    mov  dword ptr [esi+4], eax

  pop esi
    pop ebp
    ret 1*4

_64not endp


_TEXT   ends

publicdll  _u64add
publicdll  _u64sub
publicdll  _u64cmp
publicdll  _i64cmp
publicdll  _u64neg
publicdll  _u64mul
publicdll  _64div
publicdll  _64shl
publicdll  _64shr
publicdll  _64and
publicdll  _64or
publicdll  _64xor
publicdll  _64not


 END
