	.thumb
	.cpu cortex-m3

	.section isr_vector
	.word 0
	.word _start + 1
	.word _handler + 1
	.word _handler + 1
	.word _handler + 1
	.word _handler + 1
	.word _handler + 1

	.text
	.global _start
	
	.equ GPIOC_BASE, 0x40020C00
	.equ GPIOC_CRL, GPIOC_BASE
	.equ GPIOC_CRH, GPIOC_BASE + 0x04
	.equ GPIOC_IDR, GPIOC_BASE + 0x08
    .equ GPIOC_ODR, GPIOC_BASE + 0x0c
    .equ GPIOC_BSRR, GPIOC_BASE + 0x10
    .equ GPIOC_BRR, GPIOC_BASE + 0x14
    .equ GPIOC_LCKR, GPIOC_BASE + 0x18

    .equ RCC_BASE, 0x40023800
	.equ RCC_AHB1ENR, RCC_BASE+0x30

_start:
	ldr r0,=RCC_AHB1ENR
	ldr r1,=0x00100008	
	str r1,[r0]

	ldr r0,=GPIOC_BASE
	ldr r1,=0x55000000	
	str r1,[r0]

	ldr r0,=GPIOC_ODR
	ldr r1,=0xffffffff
	str r1,[r0]

stop:
	b stop


_handler:
	b _handler
