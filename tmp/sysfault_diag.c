#include <stdint.h>
#include "stm32f10x_min_regs.h" // or just define SCB addresses yourself

typedef struct {
    uint32_t r0, r1, r2, r3, r12, lr, pc, psr;
} StackedRegs;

volatile uint32_t g_hfsr, g_cfsr, g_bfar, g_mmfar;
volatile StackedRegs g_stack;

__attribute__((naked)) void HardFault_Handler(void) {
    __asm volatile(
        "tst lr, #4                         \n"
        "ite eq                             \n"
        "mrseq r0, msp                      \n" // stacked frame on MSP?
        "mrsne r0, psp                      \n" // or PSP
        "ldr r1, [r0, #24]                  \n" // stacked PC
        "ldr r2, handler2_address_const     \n"
        "bx  r2                             \n"
        "handler2_address_const: .word prvHardFaultC \n"
    );
}

void prvHardFaultC(uint32_t *stack_address) {
    g_stack.r0  = stack_address[0];
    g_stack.r1  = stack_address[1];
    g_stack.r2  = stack_address[2];
    g_stack.r3  = stack_address[3];
    g_stack.r12 = stack_address[4];
    g_stack.lr  = stack_address[5];
    g_stack.pc  = stack_address[6];  // faulting PC
    g_stack.psr = stack_address[7];

    volatile uint32_t *SCB_HFSR  = (uint32_t *)0xE000ED2C;
    volatile uint32_t *SCB_CFSR  = (uint32_t *)0xE000ED28;
    volatile uint32_t *SCB_BFAR  = (uint32_t *)0xE000ED38;
    volatile uint32_t *SCB_MMFAR = (uint32_t *)0xE000ED34;

    g_hfsr  = *SCB_HFSR;
    g_cfsr  = *SCB_CFSR;
    g_bfar  = *SCB_BFAR;
    g_mmfar = *SCB_MMFAR;

    // Set a breakpoint here and inspect g_* variables
    for(;;);
}
