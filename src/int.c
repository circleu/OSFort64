#include "header/int.h"


IDT idt;
IDTR idtr;

void set_idt() {
    uint64_t isr;
    uint16_t* offs;

    isr = (uint64_t)timer;
    idt.timer.offs0 = (uint16_t)(isr & 0xffff);
    idt.timer.seg = 0x0008;
    idt.timer.ist = 0b000;
    idt.timer.zero0 = 0;
    idt.timer.type = 0b1110;
    idt.timer.zero1 = 0;
    idt.timer.dpl = 0b00;
    idt.timer.p = 1;
    idt.timer.offs1 = (uint16_t)((isr >> 16) & 0xffff);
    idt.timer.offs2 = (uint32_t)((isr >> 32) & 0xffffffff);

    isr = (uint64_t)keyboard;
    idt.keyboard.offs0 = (uint16_t)(isr & 0xffff);
    idt.keyboard.seg = 0x0008;
    idt.keyboard.ist = 0b000;
    idt.keyboard.zero0 = 0;
    idt.keyboard.type = 0b1110;
    idt.keyboard.zero1 = 0;
    idt.keyboard.dpl = 0b00;
    idt.keyboard.p = 1;
    idt.keyboard.offs1 = (uint16_t)((isr >> 16) & 0xffff);
    idt.keyboard.offs2 = (uint32_t)((isr >> 32) & 0xffffffff);

    isr = (uint64_t)spurious_irq;
    idt.spurious_irq.offs0 = (uint16_t)(isr & 0xffff);
    idt.spurious_irq.seg = 0x0008;
    idt.spurious_irq.ist = 0b000;
    idt.spurious_irq.zero0 = 0;
    idt.spurious_irq.type = 0b1110;
    idt.spurious_irq.zero1 = 0;
    idt.spurious_irq.dpl = 0b00;
    idt.spurious_irq.p = 1;
    idt.spurious_irq.offs1 = (uint16_t)((isr >> 16) & 0xffff);
    idt.spurious_irq.offs2 = (uint32_t)((isr >> 32) & 0xffffffff);

    isr = (uint64_t)pf;
    idt.pf.offs0 = (uint16_t)(isr & 0xffff);
    idt.pf.seg = 0x0008;
    idt.pf.ist = 0b000;
    idt.pf.zero0 = 0;
    idt.pf.type = 0b1111;
    idt.pf.zero1 = 0;
    idt.pf.dpl = 0b00;
    idt.pf.p = 1;
    idt.pf.offs1 = (uint16_t)((isr >> 16) & 0xffff);
    idt.pf.offs2 = (uint32_t)((isr >> 32) & 0xffffffff);

    isr = (uint64_t)df;
    idt.df.offs0 = (uint16_t)(isr & 0xffff);
    idt.df.seg = 0x0008;
    idt.df.ist = 0b000;
    idt.df.zero0 = 0;
    idt.df.type = 0b1111;
    idt.df.zero1 = 0;
    idt.df.dpl = 0b00;
    idt.df.p = 1;
    idt.df.offs1 = (uint16_t)((isr >> 16) & 0xffff);
    idt.df.offs2 = (uint32_t)((isr >> 32) & 0xffffffff);

    isr = (uint64_t)gp;
    idt.gp.offs0 = (uint16_t)(isr & 0xffff);
    idt.gp.seg = 0x0008;
    idt.gp.ist = 0b000;
    idt.gp.zero0 = 0;
    idt.gp.type = 0b1111;
    idt.gp.zero1 = 0;
    idt.gp.dpl = 0b00;
    idt.gp.p = 1;
    idt.gp.offs1 = (uint16_t)((isr >> 16) & 0xffff);
    idt.gp.offs2 = (uint32_t)((isr >> 32) & 0xffffffff);

    idtr.size = sizeof(IDT) - 1;
    idtr.offs = (uint64_t)req_page(&global_allocator);

    *(GATE_DESCRIPTOR*)(idtr.offs + 0x20 * sizeof(GATE_DESCRIPTOR)) = idt.timer;
    *(GATE_DESCRIPTOR*)(idtr.offs + 0x21 * sizeof(GATE_DESCRIPTOR)) = idt.keyboard;
    *(GATE_DESCRIPTOR*)(idtr.offs + 0x27 * sizeof(GATE_DESCRIPTOR)) = idt.spurious_irq;
    *(GATE_DESCRIPTOR*)(idtr.offs + 0x0e * sizeof(GATE_DESCRIPTOR)) = idt.pf;
    *(GATE_DESCRIPTOR*)(idtr.offs + 0x08 * sizeof(GATE_DESCRIPTOR)) = idt.df;
    *(GATE_DESCRIPTOR*)(idtr.offs + 0x0d * sizeof(GATE_DESCRIPTOR)) = idt.gp;

    asm volatile ("lidt (%0);"::"r"(&idtr));
}
void remap_pic() {
    asm volatile ("cli");

    outb(0x10 | 0x01, 0x20);
    io_wait();
    outb(0x10 | 0x01, 0xa0);
    io_wait();
    outb(0x20, 0x21);
    io_wait();
    outb(0x28, 0xa1);
    io_wait();
    outb(0x04, 0x21);
    io_wait();
    outb(0x02, 0xa1);
    io_wait();
    outb(0x01, 0x21);
    io_wait();
    outb(0x01, 0xa1);
    io_wait();

    outb(0x00, 0x21);
    io_wait();
    outb(0x00, 0x28);
    io_wait();

    asm volatile ("sti");
}
static __attribute__((interrupt)) void timer(struct interrupt_frame*) {
    outb(0x20, 0x20);
    return;
}
static __attribute__((interrupt)) void keyboard(struct interrupt_frame*) {
    uint8_t key = inb(0x60);
    print("pressed\r\n");
    
    outb(0x20, 0x20);
    return;
}
static __attribute__((interrupt)) void spurious_irq(struct interrupt_frame*) {
    outb(0x20, 0x20);
    return;
}
static __attribute__((interrupt)) void pf(struct interrupt_frame*) {
    panic((uint64_t)pf);

    return;
}
static __attribute__((interrupt)) void df(struct interrupt_frame*) {
    panic((uint64_t)df);

    return;
}
static __attribute__((interrupt)) void gp(struct interrupt_frame*) {
    panic((uint64_t)gp);

    return;
}
static void panic(uint64_t exception) {
    clear_screen(0x00444444);
    set_pos(0, 0);
    set_color(0x00ffffff);
    
    if (exception == (uint64_t)pf) print("Page Fault.\r\n\r\n");
    else if (exception == (uint64_t)df) print("Double Fault.\r\n\r\n");
    else if (exception == (uint64_t)gp) print("General Protection Fault.\r\n\r\n");
    else print("Unexpected Error.\r\n\r\n");

    while (1);
}