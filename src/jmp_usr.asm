.globl jmp_usr

jmp_usr:
    movq $0x00007fffffffe000, %rax
    pushq $0x23
    pushq %rax

    pushfq
    popq %rax
    or $0x200, %rax
    pushq %rax

    pushq $0x1b
    pushq %rdi

    iretq
