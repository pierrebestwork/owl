;//---------------------------------------------------------------------------
;// ObjectWindows
;//
;//  By Yura Bidus -> 128 bits support
;//
;// unsigned 128 bit integer
;// void _u128add(uint8* l, uint8* r);
;//  void _u128sub(uint8* l, uint8* r)
;//  int _u128cmp(uint8* l, uint8* r)  return <0, ==0, >0
;// signed 128 bit integer
;//  int _i128cmp(uint8* l, uint8* r)  return <0, ==0, >0
;// void _128and(uint8* l, uint8* r, uint8* d)
;// void _u128mul(uint8* l, uint8* r, uint8* d)
;// void _128div(uint8* l, uint8* r, uint8* rem)
;// void _u128neg(uint8* n)
;//
;// void _128not(uint8* l)
;// void _128xor(uint8* l, uint8* r) 
;// void _128xor(uint8* l, uint8* r)
;// void _128or(uint8* l, uint8* r)
;// void _128and(uint8* l, uint8* r) 
;// void _128shr(uint8* n, uint32 count)
;// void _128shl(uint8* n, uint32 count)
;//---------------------------------------------------------------------------
        .386p
        .model   flat
        title   _128supp.asm
_TEXT   segment dword public use32 'CODE'

;//---------------------------------------------------------------------------
; add 128 + 128
;void _u128add(uint8* l, uint8* r)
;
public __u128add@8
__u128add@8 proc near

u128l   EQU [ebp+8]   ; first
u128r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

  mov  esi, u128l
  mov  edi, u128r

    mov     eax, dword ptr [esi]    ;//No need for dword ptr operator since
    add     eax, dword ptr [edi]    ;//these are dword variables.
    mov     dword ptr [esi], eax    ;// store L.O. result
    mov     eax, dword ptr [esi+4]  ;//Add in the values from the L.O.
    adc     eax, dword ptr [edi+4]  ;//entity to the H.O. entity using
    mov     dword ptr [esi+4], eax  ;//the ADC instruction.
    mov     eax, dword ptr [esi++8]
    adc     eax, dword ptr [edi+8]
    mov     dword ptr [esi+8], eax
    mov     eax, dword ptr [esi+12]
    adc     eax, dword ptr [edi+12]
    mov     dword ptr [esi+12], eax

    pop edi
    pop esi
    pop ebp

    ret 2*4

__u128add@8 endp

;//---------------------------------------------------------------------------
; add 128 + 128
;void _u128sub(uint8* l, uint8* r)
;
public __u128sub@8
__u128sub@8 proc near

u128l   EQU [ebp+8]   ; first
u128r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

  mov  esi, u128l
  mov  edi, u128r

    mov     eax, dword ptr [esi]    ;//No need for dword ptr operator since
    sub     eax, dword ptr [edi]    ;//these are dword variables.
    mov     dword ptr [esi], eax    ;// store L.O. result
    mov     eax, dword ptr [esi+4]  ;//Subtract the values from the L.O.
    sbb     eax, dword ptr [edi+4]  ;// entity to the H.O. entity using
    mov     dword ptr [esi+4], eax  ;// the SUB and SBB instructions.
    mov     eax, dword ptr [esi+8]
    sbb     eax, dword ptr [edi+8]
    mov     dword ptr [esi+8], eax
    mov     eax, dword ptr [esi+12]
    sbb     eax, dword ptr [edi+12]
    mov     dword ptr [esi+12], eax

    pop edi
    pop esi
    pop ebp
    ret 2*4

__u128sub@8 endp

;//---------------------------------------------------------------------------
; add 128 + 128
;int _u128cmp(uint8* l, uint8* r)
; return >0; == 0; < 0;
;
public __u128cmp@8
__u128cmp@8 proc near

u128l   EQU [ebp+8]   ; first
u128r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

    mov  esi, u128l
    mov  edi, u128r
    mov     eax, dword ptr [esi+4]   ;//Get H.O. dword
    cmp     eax, dword ptr [edi+4]
    mov      eax, 1
    ja      _u128End                ;// if greater put 1 into eax and exit
    jb      _u128Less
    mov     eax, dword ptr [esi]
    cmp     eax, dword ptr [edi]
    ja      _u128End                ;// if greater put 1 into eax and exit
    jb      _u128Less
    xor      eax,  eax               ;// equal return 0
    jmp     _u128End

  _u128Less:
    neg      eax                     ;// put -1 into eax and exit
    jmp     _u128End
  _u128End:
    pop edi
    pop esi
    pop ebp
    ret 2*4

__u128cmp@8 endp

;//---------------------------------------------------------------------------
;// add 128 + 128
;int _i128cmp(uint8* l, uint8* r)
; return >0; == 0; < 0;
;
public __i128cmp@8
__i128cmp@8 proc near

i128l   EQU [ebp+8]   ; first
i128r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

    mov esi, i128l
    mov edi, i128r
    mov eax, dword ptr [esi+4]   ;//Get H.O. dword
    cmp eax, dword ptr [edi+4]
    mov      eax, 1
    jg      _i128End                ;// if greater put 1 into eax and exit
    jl      _i128Less
    mov     eax, dword ptr [esi]
    cmp     eax, dword ptr [edi]
    jg      _i128End                ;// if greater put 1 into eax and exit
    jl      _i128Less
    xor      eax,  eax               ;// equal return 0
    jmp     _i128End

  _i128Less:
    neg      eax                     ;// put -1 into eax and exit
    jmp     _i128End
  _i128End:
    pop edi
    pop esi
    pop ebp
    ret 2*4

__i128cmp@8 endp

;//---------------------------------------------------------------------------
;// add 128 + 128
;void _u128neg(uint8* n)
;
;
public __u128neg@4
__u128neg@4 proc near

u128l   EQU [ebp+8]   ; first

    push ebp
    mov  ebp,esp
    push esi

    mov     esi, u128l

    mov     eax, 0
    sub     eax, dword ptr [esi]
    sbb     eax, 0
    mov     dword ptr [esi], eax
    mov     eax, 0
    sub     eax, dword ptr [esi+4]
    sbb     eax, 0
    mov     dword ptr [esi+4], eax
    mov     eax, 0
    sub     eax, dword ptr [esi+8]
    sbb     eax, 0
    mov     dword ptr [esi+8], eax
    mov     eax, 0
    sub     eax, dword ptr [esi+12]
    sbb     eax, 0
    mov     dword ptr [esi+12], eax

    pop esi
    pop ebp
    ret 1*4
__u128neg@4 endp


;//---------------------------------------------------------------------------
; mul 128 + 128
;void _u128mul(uint8* l, uint8* r, uint8* r)
;
public __u128mul@12
__u128mul@12 proc near

u128l   EQU [ebp+8]   ; first
u128r   EQU [ebp+12]  ; second
u128p    EQU [ebp+16]  ; product

    push ebp
    mov  ebp,esp
    push esi
    push edi
    push ebx
    push ecx
    push edx

  mov esi,  u128l
  mov edi,  u128r

  ;  4  3  2  1 
   ;|32|32|32|32| - Multiplier 
   ;|32|32|32|32| - Multiplicand
  ;

  ;// Multiply the 1. word of Multiplier times Multiplicand:
  ;// first step
  ;// 1 * 1
  mov eax,  dword ptr [edi]
  mov ebx,  eax           ;//Save Multiplier val
    mul dword ptr [esi]      ;//Multiply 1. dwords
    mov  ecx,  u128p
    mov  dword ptr [ecx],eax ;//Save partial product
    mov  ecx,  edx           ;//Save 2. dword

  ;// 1 * 2
    mov eax, ebx             ;//Get Multiplier in EBX
    mul dword ptr [esi+4]   ;//Multiply 1 * 2
  add eax, ecx             ;//Add partial product
  adc edx, 0               ;//Don’t forget carry!
    mov  ecx,  u128p
    mov  dword ptr [ecx+4],eax ;//Save partial product
    mov ecx, edx             ;//for now.

  ;// 1 * 3
    mov eax, ebx             ;//Get Multiplier in EBX
    mul dword ptr [esi+8]   ;//Multiply 1 * 2
  add eax, ecx             ;//Add partial product
  adc edx, 0               ;//Don’t forget carry!
    mov  ecx,  u128p
    mov  dword ptr [ecx+8],eax ;//Save partial product
    mov ecx, edx             ;//for now.

  ;// 1 * 4
    mov eax, ebx             ;//Get Multiplier in EBX
    mul dword ptr [esi+12]   ;//Multiply 1 * 2
  add eax, ecx             ;//Add partial product
  adc edx, 0               ;//Don’t forget carry!
    mov  ecx,  u128p
    mov  dword ptr [ecx+12],eax ;//Save partial product
    ;mov ecx, edx             ;//for now not save becouse result not 256 bits.


  ;// second step
  ;// 2 * 1
  mov eax,  dword ptr [edi+4]
  mov ebx,  eax           ;//Save Multiplier val
    mul dword ptr [esi]      ;//Multiply 1. dwords
    mov  ecx,  u128p
    add  dword ptr [ecx+4],eax ;//Save partial product
    mov  ecx,  edx           ;//Save 2. dword
    
  ;// 2 * 2
    mov eax, ebx             ;//Get Multiplier in EBX
    mul dword ptr [esi+4]   ;//Multiply 1 * 2
  add eax, ecx             ;//Add partial product
  adc edx, 0               ;//Don’t forget carry!
    mov  ecx,  u128p
    mov  dword ptr [ecx+8],eax ;//Save partial product
    mov ecx, edx             ;//for now.

  ;// 2 * 3
    mov eax, ebx             ;//Get Multiplier in EBX
    mul dword ptr [esi+8]   ;//Multiply 1 * 2
  add eax, ecx             ;//Add partial product
  adc edx, 0               ;//Don’t forget carry!
    mov  ecx,  u128p
    mov  dword ptr [ecx+12],eax ;//Save partial product
    ;mov ecx, edx             ;//for now not save becouse result only 128 bits.

  ;// 2 * 4   -> not dealing with 256 bits nombers
    ;mov eax, ebx             ;//Get Multiplier in EBX
    ;mul dword ptr [esi+12]   ;//Multiply 1 * 2
  ;add eax, ecx             ;//Add partial product
  ;adc edx, 0               ;//Don’t forget carry!
    ;mov  ecx,  u128p
    ;mov  dword ptr [ecx+16],eax ;//Save partial product
    ;mov ecx, edx             ;//for now.


  ;// third step
  ;// 3 * 1
  mov eax,  dword ptr [edi+8]
  mov ebx,  eax             ;//Save Multiplier val
    mul dword ptr [esi]        ;//Multiply 1. dwords
    mov  ecx,  u128p
    add  dword ptr [ecx+8],eax ;//Save partial product
    mov  ecx,  edx           ;//Save 2. dword
    
  ;// 3 * 2
    mov eax, ebx             ;//Get Multiplier in EBX
    mul dword ptr [esi+4]   ;//Multiply 1 * 2
  add eax, ecx             ;//Add partial product
  adc edx, 0               ;//Don’t forget carry!
    mov  ecx,  u128p
    mov  dword ptr [ecx+12],eax ;//Save partial product
    ;mov ecx, edx             ;//for now not save becouse result only 128 bits.

  ;// 3 * 3 -> not dealing with 256 bits nombers
    ;mov eax, ebx             ;//Get Multiplier in EBX
    ;mul dword ptr [esi+8]   ;//Multiply 1 * 2
  ;add eax, ecx             ;//Add partial product
  ;adc edx, 0               ;//Don’t forget carry!
    ;mov  ecx,  u128p
    ;mov  dword ptr [ecx+12],eax ;//Save partial product
    ;mov ecx, edx             ;//for now not save becouse result 128 bits.

  ;// 3 * 4   -> not dealing with 256 bits nombers
    ;mov eax, ebx             ;//Get Multiplier in EBX
    ;mul dword ptr [esi+12]   ;//Multiply 1 * 2
  ;add eax, ecx             ;//Add partial product
  ;adc edx, 0               ;//Don’t forget carry!
    ;mov  ecx,  u128p
    ;mov  dword ptr [ecx+16],eax ;//Save partial product
    ;mov ecx, edx             ;//for now.

  ;// last step
  ;// 4 * 1
  mov eax,  dword ptr [edi+12]
  mov ebx,  eax             ;//Save Multiplier val
    mul dword ptr [esi]        ;//Multiply 1. dwords
    mov  ecx,  u128p
    add  dword ptr [ecx+12],eax ;//Save partial product
    ;mov  ecx,  edx           ;//not save becouse result only 128 bits.
    
  ;// 4 * 2
    ;mov eax, ebx             ;//Get Multiplier in EBX
    ;mul dword ptr [esi+4]   ;//Multiply 1 * 2
  ;add eax, ecx             ;//Add partial product
  ;adc edx, 0               ;//Don’t forget carry!
    ;mov  ecx,  u128p
    ;mov  dword ptr [ecx+16],eax ;//Save partial product
    ;mov ecx, edx             ;//for now not save becouse result only 128 bits.

  ;// 4 * 3 -> not dealing with 256 bits nombers
    ;mov eax, ebx             ;//Get Multiplier in EBX
    ;mul dword ptr [esi+8]   ;//Multiply 1 * 2
  ;add eax, ecx             ;//Add partial product
  ;adc edx, 0               ;//Don’t forget carry!
    ;mov  ecx,  u128p
    ;mov  dword ptr [ecx+20],eax ;//Save partial product
    ;mov ecx, edx             ;//for now not save becouse result only 128 its.

  ;// 4 * 4   -> not dealing with 256 bits nombers
    ;mov eax, ebx             ;//Get Multiplier in EBX
    ;mul dword ptr [esi+12]   ;//Multiply 1 * 2
  ;add eax, ecx             ;//Add partial product
  ;adc edx, 0               ;//Don’t forget carry!
    ;mov  ecx,  u128p
    ;mov  dword ptr [ecx+24],eax ;//Save partial product
    ;mov ecx, edx             ;//for now not save becouse result only 128 its.

    pop edx
    pop ecx
    pop ebx
    pop edi
    pop esi
    pop ebp

    ret 3*4

__u128mul@12 endp

;//---------------------------------------------------------------------------
; 
;void _128div(uint8* l, uint8* r, uint8* rem)
;
public __128div@12
__128div@12 proc near


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
    mov ecx,    128/32      ;//Count off 128/32 bits in ecx
  xor  eax,  eax
  _128divCheckZerro:
  cmp dword ptr [edi], 0
  je  _128divCheckZerroNext
  mov  eax,  1

  _128divCheckZerroNext:
    add edi,  4
  dec  ecx
  jnz  _128divCheckZerro
  
  ;// if zerro will be exeption
  mov  ecx,  eax   
  xor edx,  edx    
  div  ecx     ;// edx:eax/ecx


    mov esi,  ndivd
    mov edi,  ndiv
    mov edx,  nrem
    mov ecx,    128      ;//Count off 64 bits in ecx


    ;// Assume Dividend (and Quotient) is [esi], Divisor is in [edi],
    ;// and Remainder is in [edx].
    
  xor  eax,  eax
  mov dword ptr [edx],   eax  ;//Set remainder to zero
    mov dword ptr [edx+4],   eax  ;//Set remainder to zero
  mov dword ptr [edx+8],   eax  ;//Set remainder to zero
    mov dword ptr [edx+12],  eax  ;//Set remainder to zero

  _128divBitLoop:                   ;//256 bit SHL operation  
    shl dword ptr [esi],  1   ;//
    rcl dword ptr [esi+4],  1   ;//
    rcl dword ptr [esi+8],  1   ;//
    rcl dword ptr [esi+12],  1   ;//
    rcl dword ptr [edx],  1   ;//
    rcl dword ptr [edx+4],   1   ;//
    rcl dword ptr [edx+8],   1   ;//
    rcl dword ptr [edx+12],  1   ;//
    
  mov  eax, dword ptr [edx+12]  ;//Compare H.O. words of Rem,
    cmp eax, dword ptr [edi+12]  ;//Divisor.
    ja _128divGoesInto           ;//
    jb _128divTryNext
  mov  eax, dword ptr [edx+8]  ;//Compare H.O. words of Rem,
    cmp eax, dword ptr [edi+8]  ;//Divisor.
    ja _128divGoesInto           ;//
    jb _128divTryNext
  mov  eax, dword ptr [edx+4]  ;//Compare H.O. words of Rem,
    cmp eax, dword ptr [edi+4]  ;//Divisor.
    ja _128divGoesInto           ;//
    jb _128divTryNext
    mov  eax, dword ptr [edx]
    cmp eax, dword ptr [edi]  ;//Compare L.O. words.
    jb _128divTryNext
  _128divGoesInto:
    mov  eax,  dword ptr [edi]   ;//
    sub dword ptr [edx], eax   ;//Remainder := Remainder - Divisor
    mov  eax,  dword ptr [edi+4] ;// 
    sbb  dword ptr [edx+4], eax   ;//
    mov  eax,  dword ptr [edi+8] ;// 
    sbb  dword ptr [edx+8], eax   ;//
    mov  eax,  dword ptr [edi+12];// 
    sbb  dword ptr [edx+12], eax ;//
    inc dword ptr [esi]     ;//Set L.O. bit of AX
  _128divTryNext:
    dec ecx ;//Repeat 128 times.
    jne _128divBitLoop

    pop edx
    pop ecx
    pop edi
    pop esi
    pop ebp


    ret 3*4

__128div@12 endp

;//---------------------------------------------------------------------------
;// 
;void _128shl(uint8* n, uint32 count)
;
;
public __128shl@8
__128shl@8 proc near

u128   EQU [ebp+8]   ; first
count  EQU [ebp+12]   ; count

    push ebp
    mov  ebp,esp
    push esi
    push ecx

    mov     esi, u128
    mov     ecx, count

  _128shlLoop:
    shl dword ptr [esi],  1   ;// 128 bit shl
    rcl dword ptr [esi+4],  1   ;//
    rcl dword ptr [esi+8],  1   ;//
    rcl dword ptr [esi+12],  1   ;//
  dec ecx
  jnz _128shlLoop  

    pop ecx
    pop esi
    pop ebp
    ret 2*4
__128shl@8 endp


;//---------------------------------------------------------------------------
;// 
;void _128shr(uint8* n, uint32 count)
;
;
public __128shr@8
__128shr@8 proc near

u128   EQU [ebp+8]   ; first
count  EQU [ebp+12]   ; count

    push ebp
    mov  ebp,esp
    push esi
    push ecx

    mov     esi, u128
    mov     ecx, count

  _128shrLoop:
    shr dword ptr [esi+12],  1   ;// 128 bit shr
    rcr dword ptr [esi+8],  1   ;//
    rcr dword ptr [esi+4],  1   ;//
    rcr dword ptr [esi],  1   ;//
  dec ecx
  jnz _128shrLoop  

    pop ecx
    pop esi
    pop ebp
    ret 2*4
__128shr@8 endp


;//---------------------------------------------------------------------------
;// and 128,128 -> dest
;void _128and(uint8* l, uint8* r)
; return >0; == 0; < 0;
;
public __128and@8
__128and@8 proc near

_128l   EQU [ebp+8]   ; first
_128r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

  mov  esi, _128l
  mov  edi, _128r

    mov     eax, dword ptr [esi]
    and     eax, dword ptr [edi]
    mov     dword ptr [esi], eax

    mov     eax, dword ptr [esi+4]
    and     eax, dword ptr [edi+4]
    mov     dword ptr [esi+4], eax

    mov     eax, dword ptr [esi+8]
    and     eax, dword ptr [edi+8]
    mov     dword ptr [esi+8], eax

    mov     eax, dword ptr [esi+12]
    and     eax, dword ptr [edi+12]
    mov     dword ptr [esi+12], eax

    pop edi
    pop esi
    pop ebp
    ret 2*4
__128and@8 endp

;//---------------------------------------------------------------------------
; add 128 + 128
;void _128or(uint8* l, uint8* r)
;
public __128or@8
__128or@8 proc near

_128l   EQU [ebp+8]   ; first
_128r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

  mov  esi, _128l
  mov  edi, _128r

    mov     eax, dword ptr [esi]
    or    eax, dword ptr [edi]
    mov     dword ptr [esi], eax

    mov     eax, dword ptr [esi+4]
    or    eax, dword ptr [edi+4]
    mov     dword ptr [esi+4], eax

    mov     eax, dword ptr [esi+8]
    or    eax, dword ptr [edi+8]
    mov     dword ptr [esi+8], eax

    mov     eax, dword ptr [esi+12]
    or    eax, dword ptr [edi+12]
    mov     dword ptr [esi+12], eax

    pop edi
    pop esi
    pop ebp
    ret 2*4

__128or@8 endp


;//---------------------------------------------------------------------------
; add 128 + 128
;void _128xor(uint8* l, uint8* r)
;
public __128xor@8
__128xor@8 proc near

_128l   EQU [ebp+8]   ; first
_128r   EQU [ebp+12]  ; second

    push ebp
    mov  ebp,esp
    push esi
    push edi

  mov  esi, _128l
  mov  edi, _128r

    mov     eax, dword ptr [esi]
    xor    eax, dword ptr [edi]
    mov     dword ptr [esi], eax

    mov     eax, dword ptr [esi+4]
    xor    eax, dword ptr [edi+4]
    mov     dword ptr [esi+4], eax

    mov     eax, dword ptr [esi+8]
    xor    eax, dword ptr [edi+8]
    mov     dword ptr [esi+8], eax

    mov     eax, dword ptr [esi+12]
    xor    eax, dword ptr [edi+12]
    mov     dword ptr [esi+12], eax

    pop edi
    pop esi
    pop ebp
    ret 2*4

__128xor@8 endp


;//---------------------------------------------------------------------------
; not 128 
;void _128not(uint8* l)
;
public __128not@4
__128not@4 proc near

_128   EQU [ebp+8]   ; first

    push ebp
    mov  ebp,esp
    push esi

  mov  esi, _128

    mov  eax, dword ptr [esi]
    not  eax
    mov  dword ptr [esi], eax
    
  mov  eax, dword ptr [esi+4]
    not  eax
    mov  dword ptr [esi+4], eax

    mov  eax, dword ptr [esi+8]
    not  eax
    mov  dword ptr [esi+8], eax
    
  mov  eax, dword ptr [esi+12]
    not  eax
    mov  dword ptr [esi+12], eax
    
  pop esi
    pop ebp
    ret 1*4

__128not@4 endp



_TEXT   ends
        END
