.globl load_gdt

load_gdt:
    lgdt (%rdi)
    pushq $0x08
    leaq .reload_seg(%rip), %rax
    pushq %rax
    retfq
.reload_seg:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    retq
