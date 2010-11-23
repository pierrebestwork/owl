#//---------------------------------------------------------------------------
#// ObjectWindows
#// Copyright (c) 1995, 1996 by Borland International, All rights Reserved
#//
#//$Revision: 1.25 $
#//
#//   DibSpriteBlt8 : 32 Bit Non-GDI SpriteBlt for 8bpp DIBs
#//   DibCopyBlt8 :   32 Bit Non-GDI CopyBlt for 8bpp DIBs
#//
#//---------------------------------------------------------------------------
.title "dibblt32.s"
.text
.align 4
#//---------------------------------------------------------------------------
#
#void WINAPI
#DibCopyBlt8(uint8 HUGE* dstBits, uint32 width, uint32 height, uint32 dstPitch,
#           uint8 HUGE* srcBits, uint32 srcPitch)#

.global _DibCopyBlt8@24
.def _DibCopyBlt8@24; .scl 2; .type 32; .endef
_DibCopyBlt8@24:

#.equ dstBits   ,8(%ebp)   # Destination bits surface
#.equ wdth      ,12(%ebp)  # Bits Width
#.equ height    ,16(%ebp)  # Bits Height
#.equ dstPitch  ,20(%ebp)  # Destination scan pitch

#.equ srcBits   ,24(%ebp)  # Source bits surface
#.equ srcPitch  ,28(%ebp)  # Source scan pitch

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi

    movl 12(%ebp), %ecx           # ECX is Width
    orl  %ecx,%ecx
    jz  DCBComplete         # No Source Width

    movl 16(%ebp), %edx         # EDX is line counter
    orl %edx,%edx
    jz DCBComplete          # No Source Height

    xorl %esi, %esi            # Clear out ESI
    movl 24(%ebp), %esi        # ESI point to source

    xorl %edi, %edi
    movl 8(%ebp), %edi        # EDI point to dest

    subl %ecx, 28(%ebp)       # Adjust for width of object.
    subl %ecx, 20(%ebp)       # Adjust for width of object.

    movl %ecx, %ebx
    shrl $2, %ebx

    movl %ecx, %eax
    andl $3,%eax

DCBLoopY:

    movl %ebx,%ecx
    rep movsl (%esi), (%edi)
    movl %eax,%ecx
    rep movsb (%esi), (%edi)
    addl 28(%ebp),%esi
    addl 20(%ebp),%edi
    dec %edx
    jnz DCBLoopY

DCBComplete:

    pop %edi
    pop %esi
    pop %ebp

#    ret 6*4
		ret $24

#DibCopyBlt8 endp

#//---------------------------------------------------------------------------
#
#void WINAPI
#DibSpriteBlt8(uint8 HUGE* dstBits, uint32 width, uint32 height, uint32 dstPitch,
#             uint8 HUGE* srcBits, uint32 srcPitch, uint8 transparent)#

.global _DibSpriteBlt8@28
.def _DibSpriteBlt8@28; .scl 2; .type 32; .endef
_DibSpriteBlt8@28:

#.equ dstBits   ,8(%ebp)   # Destination bits surface
#.equ wdth      ,12(%ebp)  # Bits Width
#.equ height    ,16(%ebp)  # Bits Height
#.equ dstPitch  ,20(%ebp)  # Destination scan pitch

#.equ srcBits   ,24(%ebp)  # Source bits surface
#.equ srcPitch  ,28(%ebp)  # Source scan pitch

#.equ trans     ,32(%ebp)  # Transparent color

    pushl %ebp
    movl  %esp,%ebp

    pushl %esi
    pushl %edi
    pushl %ebx

    movl 32(%ebp), %eax
    movl 16(%ebp), %edx
    movl 28(%ebp), %ecx

    movl 12(%ebp), %ebx           # ECX is Width
    orl  %ebx,%ebx
    jz  DSBComplete         # No Source Width

    xorl %esi, %esi
    movl 24(%ebp), %esi        # ESI point to source

    xorl %edi, %edi
    movl 8(%ebp), %edi        # EDI point to dest

    subl %ebx, %ecx             # Adjust for width of object.
    subl %ebx,20(%ebp)         # Adjust for width of object.

DSBLoopY:

    andl %edx, %edx            # Flag more height.
    jz  DSBComplete         # Complete if no more height.

    movl 12(%ebp), %ebx           # ECX is pixel counter

DSBLoopX:

    movb (%esi), %ah           # Load pixel.
    cmpb %ah, %al              # Is pixel masked?
    jz  DSBLoopXMasked      # Yes, jump past move.

    movb %ah, (%edi)

DSBLoopXMasked:

    inc %esi                 # Increment Source.
    inc %edi                 # Increment Destination.

    dec %ebx                 # Decrement scan line.
    jnz DSBLoopX            # Jump to next pixel of scan line.

# move on to the start of the next line

    addl %ecx,%esi            # Adjust to next scan line.
    addl 20(%ebp), %edi       # Adjust to next scan line.

    dec %edx                 # line counter
    jmp DSBLoopY            # Jump back for more height.

DSBComplete:

    popl %ebx
    popl %edi
    popl %esi
    popl %ebp

#    ret 7*4
	ret $28

#DibSpriteBlt8 endp

#_TEXT   ends
#        END
