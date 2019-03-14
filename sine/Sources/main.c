/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#define __ASM __asm /*!< asm keyword for GNU Compiler */ 
#define __INLINE inline /*!< inline keyword for GNU Compiler */ 
#define __STATIC_INLINE static inline 

#include "derivative.h" /* include peripheral declarations */

int32_t sine(int32_t);

int main(void){
	int32_t tmp=sine(0b00011001001000011111101101010100);
	while(1);
	return 0;
}

int32_t sine(int32_t x){
	__asm volatile ("MOV r0, %0\n"::"r" (x)); // r0= x
	__asm volatile ("LDR r3, =#0x80000000");
	__asm volatile ("AND r3, r0, r3"); //signo en r3
	__asm volatile ("LSL r0, r0, #1");	//r0= magnitude of x
	__asm volatile ("UMULL r2, r1, r0, r0"); //x^2 en r1
	
	__asm volatile ("LDR r2, =#0x00618618\n");//1/42 se guarda en r2
	__asm volatile ("UMULL r4, r2, r2, r1"); //x^2/42 se guarda en r2
	__asm volatile ("LDR r4, =#0x10000000");
	__asm volatile ("SUB r2, r4, r2");//1-x^2/42
	
	__asm volatile ("LDR r4,=#0x00CCCCCC");//1/20
	__asm volatile ("UMULL r5, r4, r4, r1\n");//x^2/20 en r4
	__asm volatile ("UMULL r4, r2, r2, r4\n");//x^2/20(1-x^2/42))
	__asm volatile ("LDR r4, =#0x10000000");
	__asm volatile ("ADD r2, r4, r2");//1+x^2/20(1-x^2/42))

	__asm volatile ("LDR r4, =#0x2AAAAAAA");
	__asm volatile ("UMULL r5, r4, r4, r1");//x^2/6
	__asm volatile ("UMULL r4, r2, r2, r4");//x^2/6(1+x^2/20(1-x^2/42)))
	
	__asm volatile ("LDR r4, =#0x10000000");
	__asm volatile ("SUB r2, r4, r2");
	__asm volatile ("UMULL r4, r2, r2, r0");
	
	__asm volatile ("mov %0, r2\n":"=r" (x)); // r0= x
	return  x;
}
