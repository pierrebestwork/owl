#//---------------------------------------------------------------------------
#// ObjectWindows
#//
#//	By Yura Bidus -> 128 bits support
#//
#// unsigned 128 bit integer
#// void _u128add(uint8* l, uint8* r);
#//	void _u128sub(uint8* l, uint8* r)
#//	int _u128cmp(uint8* l, uint8* r)  return <0, ==0, >0
#// signed 128 bit integer
#//	int _i128cmp(uint8* l, uint8* r)  return <0, ==0, >0
#// void _128and(uint8* l, uint8* r, uint8* d)
#// void _u128mul(uint8* l, uint8* r, uint8* d)
#// void _128div(uint8* l, uint8* r, uint8* rem)
#// void _u128neg(uint8* n)
#//---------------------------------------------------------------------------
.title "_128bs.s"

.macro FUNCT name
.globl \name
.def \name; .scl 2; .type 32; .endef
\name:
.endm

.text
.align 4
#//---------------------------------------------------------------------------
# add 128 + 128
#void _u128add(uint8* l, uint8* r)
#
FUNCT _u128add

#u128l   EQU [ebp+8]   ; first
#u128r   EQU [ebp+12]  ; second

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi

	movl	8(%ebp),%esi
	movl	12(%ebp),%edi

  movl     (%esi),%eax    #No need for dword ptr operator since
  addl     (%edi),%eax    #these are dword variables.
  movl     %eax,(%esi)    # store L.O. result
  movl     4(%esi),%eax   #Add in the values from the L.O.
  adcl     4(%edi),%eax   #entity to the H.O. entity using
  movl     %eax,4(%esi)   #the ADC instruction.
  movl     8(%esi),%eax
  adcl     8(%edi),%eax
  movl     %eax,8(%esi)
  movl     12(%esi),%eax
  adcl     12(%edi),%eax
  movl     %eax,12(%esi)

  popl %edi
  popl %esi
  popl %ebp

  ret $8

#---------------------------------------------------------------------------
# add 128 + 128
#void _u128sub(uint8* l, uint8* r)
#
FUNCT _u128sub

#u128l   EQU [ebp+8]   ; first
#u128r   EQU [ebp+12]  ; second

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi

	movl	8(%ebp),%esi
	movl	12(%ebp),%edi

    movl  (%esi),%eax    #No need for  operator since
    subl  (%edi),%eax    #these are dword variables.
    movl  %eax,(%esi)    # store L.O. result
    movl  4(%esi),%eax   #Subtract the values from the L.O.
    sbbl  4(%edi),%eax   # entity to the H.O. entity using
    movl  %eax,4(%esi)   # the SUB and SBB instructions.
    movl  12(%esi),%eax
    sbbl  12(%edi),%eax
    movl  %eax,12(%esi)
    movl  12(%esi),%eax
    sbbl  12(%edi),%eax
    movl  %eax,12(%esi)

    popl %edi
    popl %esi
    popl %ebp
    ret $8


#---------------------------------------------------------------------------
# add 128 + 128
#int _u128cmp(uint8* l, uint8* r)
# return >0; == 0; < 0;
#
FUNCT _u128cmp

#u128l   EQU [%ebp+8]   ; first
#u128r   EQU [%ebp+12]  ; second

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi

    movl	8(%ebp),%esi
    movl	12(%ebp),%edi
    movl  4(%esi),%eax 	#Get H.O. dword
    cmpl  4(%edi),%eax
    movl	$1,%eax
    ja      _u128End								# if greater put 1 into %eax and exit
    jb      _u128Less
    movl     (%esi),%eax
    cmpl     (%edi),%eax
    ja      _u128End								# if greater put 1 into %eax and exit
    jb      _u128Less
    xorl			%eax,	%eax   						# equal return 0
    jmp     _u128End

  _u128Less:
    negl			%eax                     # put -1 into %eax and exit
    jmp     _u128End
  _u128End:
    popl %edi
    popl %esi
    popl %ebp
    ret $8


#---------------------------------------------------------------------------
# add 128 + 128
#int _i128cmp(uint8* l, uint8* r)
# return >0; == 0; < 0;
#
FUNCT _i128cmp

#i128l   EQU [%ebp+8]   ; first
#i128r   EQU [%ebp+12]  ; second

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi

    movl 8(%ebp),%esi
    movl 12(%ebp),%edi
    movl 4(%esi),%eax 	#Get H.O. dword
    cmpl 4(%edi),%eax
    movl	$1,%eax
    jg      _i128End								# if greater put 1 into %eax and exit
    jl      _i128Less
    movl     (%esi),%eax
    cmpl     (%edi),%eax
    jg      _i128End								# if greater put 1 into %eax and exit
    jl      _i128Less
    xorl			%eax,	%eax   						# equal return 0
    jmp     _i128End

  _i128Less:
    negl	    %eax                     # put -1 into %eax and exit
    jmp     _i128End
  _i128End:
    popl %edi
    popl %esi
    popl %ebp
    ret $8


#---------------------------------------------------------------------------
# add 128 + 128
#void _u128neg(uint8* n)
#
FUNCT _u128neg

#u128l   EQU [%ebp+8]   ; first

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi

    movl  8(%ebp),%esi

#    movl     $0,%eax
		xorl		%eax,%eax
    subl     (%esi),%eax
    sbbl     $0,%eax
    movl     %eax,(%esi)
#    movl     $0,%eax
		xorl		%eax,%eax
    subl     4(%esi),%eax
    sbbl     $0,%eax
    movl     %eax,4(%esi)
#    movl     $0,%eax
		xorl		%eax,%eax
    subl     8(%esi),%eax
    sbbl     $0,%eax
    movl     %eax,8(%esi)
#    movl     $0,%eax
		xorl		%eax,%eax
    subl     12(%esi),%eax
    sbbl     $0,%eax
    movl     %eax,12(%esi)

    popl %esi
    popl %ebp
    ret $4


#---------------------------------------------------------------------------
# mul 128 + 128
#void _u128mul(uint8* l, uint8* r, uint8* r)
#
FUNCT _u128mul

#u128l   EQU [%ebp+8]   ; first
#u128r   EQU [%ebp+12]  ; second
#u128p  	EQU [%ebp+16]	; product

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi
    pushl %ebx
    pushl %ecx
    pushl %edx

	movl 8(%ebp),%esi
	movl 12(%ebp),%edi

	#  4  3  2  1 
 	#|32|32|32|32| - Multiplier 
 	#|32|32|32|32| - Multiplicand
	#

	# Multiply the 1. word of Multiplier times Multiplicand:
	# first step
	# 1 * 1
	movl (%edi),%eax
	movl %eax, %ebx 					#Save Multiplier val
  mull (%esi)								#Multiply 1. dwords
  movl 16(%ebp),%ecx
  movl %eax, (%ecx)				 	#Save partial product
  movl %edx,	%ecx 					#Save 2. dword

	# 1 * 2
  movl %ebx, %eax 					#Get Multiplier in %ebx
  mull  4(%esi) 						#Multiply 1 * 2
	addl %ecx, %eax 					#Add partial product
	adcl $0, %edx 						#Don’t forget carry!
  movl 16(%ebp),%ecx
  movl %eax,4(%ecx)					#Save partial product
  movl %edx, %ecx						#for now.

	# 1 * 3
  movl %ebx, %eax 					#Get Multiplier in %ebx
  mull 8(%esi)						 	#Multiply 1 * 2
	addl %ecx, %eax 					#Add partial product
	adcl $0,%edx 							#Don’t forget carry!
  movl 16(%ebp),%ecx
  movl %eax,8(%ecx)					#Save partial product
  movl %edx, %ecx 						#for now.

	# 1 * 4
  movl %ebx, %eax 						#Get Multiplier in %ebx
  mull 12(%esi)							 	#Multiply 1 * 2
	addl %ecx, %eax 						#Add partial product
	adcl $0,%edx 								#Don’t forget carry!
  movl 16(%ebp),%ecx
  movl	 %eax,12(%ecx) #Save partial product
  #movl %edx, %ecx 						#for now not save becouse result not 256 bits.


	# second step
	# 2 * 1
	movl 4(%edi),%eax
	movl %eax,	%ebx 					#Save Multiplier val
  mull (%esi)								#Multiply 1. dwords
  movl 16(%ebp),%ecx
  addl %eax,4(%ecx)				#Save partial product
  movl	%edx,	%ecx 					#Save 2. dword
		
	# 2 * 2
  movl %ebx, %eax 						#Get Multiplier in %ebx
  mull 4(%esi) 								#Multiply 1 * 2
	addl %ecx, %eax 						#Add partial product
	adcl $0,%edx 								#Don’t forget carry!
  movl 16(%ebp),%ecx
  movl %eax,8(%ecx) 					#Save partial product
  movl %edx, %ecx 						#for now.

	# 2 * 3
  movl %ebx, %eax 						#Get Multiplier in %ebx
  mull 8(%esi) 								#Multiply 1 * 2
	addl %ecx, %eax 						#Add partial product
	adcl $0,%edx 								#Don’t forget carry!
  movl 16(%ebp),%ecx
  movl %eax,12(%ecx) 					#Save partial product
  #movl %edx, %ecx 						#for now not save becouse result only 128 bits.

	# 2 * 4   -> not dealing with 256 bits nombers
  #movl %ebx, %eax 						#Get Multiplier in %ebx
  #mull 12(%esi) 								#Multiply 1 * 2
	#addl %ecx, %eax 						#Add partial product
	#adcl $0,%edx 								#Don’t forget carry!
  #movl 16(%ebp),%ecx
  #movl %eax,16(%ecx) 					#Save partial product
  #movl %edx, %ecx 						#for now.


	# third step
	# 3 * 1
	movl 8(%edi),%eax
	movl %eax,	%ebx 					#Save Multiplier val
  mull (%esi)								#Multiply 1. dwords
  movl 16(%ebp),%ecx
  addl %eax,8(%ecx)				#Save partial product
  movl	%edx,	%ecx 					#Save 2. dword

	# 3 * 2
  movl %ebx, %eax 						#Get Multiplier in %ebx
  mull 4(%esi) 								#Multiply 1 * 2
	addl %ecx, %eax 						#Add partial product
	adcl $0,%edx 								#Don’t forget carry!
  movl 16(%ebp),%ecx
  movl %eax,8(%ecx) 					#Save partial product
  #movl %edx, %ecx 						#not 256 bits.

	# 3 * 3 -> not dealing with 256 bits nombers
  #movl %ebx, %eax 						#Get Multiplier in %ebx
  #mull 8(%esi) 								#Multiply 1 * 2
	#addl %ecx, %eax 						#Add partial product
	#adcl $0,%edx 								#Don’t forget carry!
  #movl 16(%ebp),%ecx
  #movl %eax,12(%ecx) 					#Save partial product
  #movl %edx, %ecx 						#for now.

	# 3 * 4   -> not dealing with 256 bits nombers
  #movl %ebx, %eax 						#Get Multiplier in %ebx
  #mull 12(%esi) 								#Multiply 1 * 2
	#addl %ecx, %eax 						#Add partial product
	#adcl $0,%edx 								#Don’t forget carry!
  #movl 16(%ebp),%ecx
  #movl %eax,16(%ecx) 					#Save partial product
  #movl %edx, %ecx 						#for now.

	# last step
	# 4 * 1
	movl 8(%edi),%eax
	movl %eax,	%ebx 					#Save Multiplier val
  mull (%esi)								#Multiply 1. dwords
  movl 16(%ebp),%ecx
  addl %eax,8(%ecx)				#Save partial product
  #movl	%edx,	%ecx 					#Save 2. dword

	# 4 * 2
  #movl %ebx, %eax 						#Get Multiplier in %ebx
  #mull 4(%esi) 								#Multiply 1 * 2
	#addl %ecx, %eax 						#Add partial product
	#adcl $0,%edx 								#Don’t forget carry!
  #movl 16(%ebp),%ecx
  #movl %eax,8(%ecx) 					#Save partial product
  #movl %edx, %ecx 						#not 256 bits.

	# 4 * 3 -> not dealing with 256 bits nombers
  #movl %ebx, %eax 						#Get Multiplier in %ebx
  #mull 8(%esi) 								#Multiply 1 * 2
	#addl %ecx, %eax 						#Add partial product
	#adcl $0,%edx 								#Don’t forget carry!
  #movl 16(%ebp),%ecx
  #movl %eax,12(%ecx) 					#Save partial product
  #movl %edx, %ecx 						#for now.

	# 4 * 4   -> not dealing with 256 bits nombers
  #movl %ebx, %eax 						#Get Multiplier in %ebx
  #mull 12(%esi) 								#Multiply 1 * 2
	#addl %ecx, %eax 						#Add partial product
	#adcl $0,%edx 								#Don’t forget carry!
  #movl 16(%ebp),%ecx
  #movl %eax,16(%ecx) 					#Save partial product
  #movl %edx, %ecx 						#for now.

    popl %edx
    popl %ecx
    popl %ebx
    popl %edi
    popl %esi
    popl %ebp

    ret $12


#---------------------------------------------------------------------------
#
#void _128div(uint8* l, uint8* r, uint8* rem)
#
FUNCT _128div

#ndivd  	EQU [%ebp+8]  ; dividend
#ndiv  	EQU [%ebp+12] ; divisor
#nrem  	EQU [%ebp+16] ; remainder

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi
    pushl %ecx
    pushl %edx

	# check divisor for zerro
  movl 12(%ebp),%edi
  movl $4,%ecx			#Count off 128/32 bits in %ecx
	xorl	%eax,	%eax
  _128divCheckZerro:
	cmpl $0, (%edi)
	je	_128divCheckZerroNext
	movl	$1,%eax

  _128divCheckZerroNext:
  addl $4,%edi
	decl	%ecx
	jnz	_128divCheckZerro
	
	# if zerro will be exeption
	movl	%eax,	%ecx
	xorl %edx,	%edx		
	divl	%ecx     # %edx:%eax/%ecx


  movl 8(%ebp),%esi
  movl 12(%ebp),%edi
  movl 16(%ebp),%edx
  movl $128,%ecx   			#Count off 64 bits in %ecx


    # Assume Dividend (and Quotient) is [%esi], Divisor is in [%edi],
    # and Remainder is in [%edx].
    
	xorl	%eax,	%eax
	movl  %eax,(%edx)		#Set remainder to zero
  movl  %eax,4(%edx)	#Set remainder to zero
	movl  %eax,8(%edx)	#Set remainder to zero
  movl  %eax,12(%edx) #Set remainder to zero

  _128divBitLoop:                   #256 bit SHL operation  
    shll  $1,(%esi)
    rcll  $1,4(%esi)
    rcll  $1,8(%esi)
    rcll  $1,12(%esi)
    rcll  $1,(%edx)
    rcll  $1,4(%edx)
    rcll  $1,8(%edx)
    rcll  $1,12(%edx)
    
  	movl	12(%edx),%eax  #Compare H.O. words of Rem,
	  cmpl  12(%edi),%eax  #Divisor.
	  ja _128divGoesInto 			    #
	  jb _128divTryNext
		movl	8(%edx),%eax  #Compare H.O. words of Rem,
	  cmpl  8(%edi),%eax	#Divisor.
	  ja _128divGoesInto 			    #
	  jb _128divTryNext
		movl	4(%edx),%eax  #Compare L.O. words of Rem,
	  cmpl  4(%edi),%eax	#Divisor.
	  ja _128divGoesInto 			    #
	  jb _128divTryNext
		movl	(%edx),%eax  #Compare L.O. words of Rem,
	  cmpl  (%edi),%eax	 #Divisor.
  	jb _128divTryNext
  _128divGoesInto:
    movl	(%edi),%eax   	#
    subl  %eax,(%edx) 		#Remainder := Remainder - Divisor
    movl	4(%edi),%eax   	#
    subl  %eax,4(%edx) 		#Remainder := Remainder - Divisor
    movl	8(%edi),%eax   	#
    subl  %eax,8(%edx) 		#Remainder := Remainder - Divisor
    movl	12(%edi),%eax   #
    subl  %eax,12(%edx) 	#Remainder := Remainder - Divisor
    incl  (%esi) 					#Set L.O. bit of AX
  _128divTryNext:
    decl %ecx 						#Repeat 128 times.
    jne _128divBitLoop

    popl %edx
    popl %ecx
    popl %edi
    popl %esi
    popl %ebp


    ret $12


#---------------------------------------------------------------------------
#
#void _128shl(uint8* n, uint32 count)
#
#
FUNCT _128shl

#u128   EQU [%ebp+8]   ; first
#count  EQU [%ebp+12]   ; count

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %ecx

    movl  8(%ebp),   %esi
    mov   12(%ebp),  %ecx

  _128shlLoop:
    shll  $1,(%esi) 	# 128 bit shl
    rcll  $1,4(%esi)
    rcll  $1,8(%esi)
    rcll  $1,12(%esi)
		decl  %ecx
	jnz _128shlLoop

    popl %ecx
    popl %esi
    popl %ebp
    ret $8


#---------------------------------------------------------------------------
#
#void _128shr(uint8* n, uint32 count)
#
FUNCT _128shr

#u128   EQU [%ebp+8]   ; first
#count  EQU [%ebp+12]   ; count

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %ecx

    movl  8(%ebp),   %esi
    movl  12(%ebp),  %ecx

  _128shrLoop:
    shrl  $1,12(%esi) 	# 128 bit shr
    rcrl  $1,8(%esi)	#
    rcrl  $1,4(%esi) 	#
    rcrl  $1,(%esi) 	#
		decl %ecx
		jnz _128shrLoop

    popl %ecx
    popl %esi
    popl %ebp
    ret $8


#---------------------------------------------------------------------------
# and 128,128 -> dest
#void _128and(uint8* l, uint8* r)
# return >0; == 0; < 0;
#
FUNCT _128and

#_128l   EQU [%ebp+8]   ; first
#_128r   EQU [%ebp+12]  ; second

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi

	movl	8(%ebp),%esi
	movl	12(%ebp),%edi

    movl  (%esi),   %eax
    andl  (%edi),   %eax
    movl  %eax,    (%esi)

    movl  4(%esi),   %eax
    andl  4(%edi),   %eax
    movl  %eax,    4(%esi)

    movl  8(%esi),   %eax
    andl  8(%edi),   %eax
    movl  %eax,    8(%esi)

    movl  12(%esi),   %eax
    andl  12(%edi),   %eax
    movl  %eax,    12(%esi)

    popl %edi
    popl %esi
    popl %ebp
    ret $8

#---------------------------------------------------------------------------
# add 128 + 128
#void _128or(uint8* l, uint8* r)
#
FUNCT _128or

#_128l   EQU [%ebp+8]   ; first
#_128r   EQU [%ebp+12]  ; second

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi

	movl	8(%ebp),%esi
	movl	12(%ebp),%edi

    movl  (%esi),   %eax
    orl   (%edi),   %eax
    movl  %eax,    (%esi)

    movl  4(%esi),   %eax
    orl   4(%edi),   %eax
    movl  %eax,    4(%esi)

    movl  8(%esi),   %eax
    orl   8(%edi),   %eax
    movl  %eax,    8(%esi)

    movl  12(%esi),   %eax
    orl   12(%edi),   %eax
    movl  %eax,    12(%esi)

    popl %edi
    popl %esi
    popl %ebp
    ret $8


#---------------------------------------------------------------------------
# add 128 + 128
#void _128xor(uint8* l, uint8* r)
#
FUNCT _128xor

#_128l   EQU [%ebp+8]   ; first
#_128r   EQU [%ebp+12]  ; second

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi
    pushl %edi

	movl	8(%ebp),%esi
	movl	12(%ebp),%edi

    movl  (%esi),   %eax
    xorl  (%edi),   %eax
    movl  %eax,    (%esi)

    movl  4(%esi),   %eax
    xorl  4(%edi),   %eax
    movl  %eax,    4(%esi)

    movl  8(%esi),   %eax
    xorl  8(%edi),   %eax
    movl  %eax,    8(%esi)

    movl  12(%esi),   %eax
    xorl  12(%edi),   %eax
    movl  %eax,    12(%esi)

    popl %edi
    popl %esi
    popl %ebp
    ret $8



#---------------------------------------------------------------------------
# not 128
#void _128not(uint8* l)
#
FUNCT _128not

#_128   EQU [%ebp+8]   ; first

    pushl %ebp
    movl  %esp,%ebp
    pushl %esi

	movl	8(%ebx),%esi

    movl  (%esi),   %eax
    notl  %eax
    movl  %eax,    (%esi)

    movl  4(%esi),   %eax
    notl  %eax
    movl  %eax,    4(%esi)

    movl  8(%esi),   %eax
    notl  %eax
    movl  %eax,    8(%esi)

    movl  12(%esi),   %eax
    notl  %eax
    movl  %eax,    12(%esi)

		popl %esi
    popl %ebp
    ret $4

##################################################################