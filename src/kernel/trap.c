#include <system.h>
#include <isrs.h>

struct idt_entry idt[256];    // must be 256
struct idt_ptr idt_p;

void idt_set(int num, uint base, ushort selector, uchar type, uchar dpl) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;

    idt[num].selector = selector;
    idt[num].attr_type = type;
    idt[num].attr_dpl = dpl;

    idt[num].zero = 0;
    idt[num].attr_s = 0;
    idt[num].attr_present = 1;
}

void set_itr_gate(int num, uint base) {
    idt_set(num, base, KERN_CS, STS_TYPE_IG32, RING0);
}

void set_trap_gate(int num, uint base) {
    idt_set(num, base, KERN_CS, STS_TYPE_TG32, RING0);
}

void set_task_gate(int num, uint base) {
    idt_set(num, base, KERN_CS, STS_TYPE_IG32, RING3);
}

void isrs_init() {
    set_trap_gate(0, (uint) isr0);
    set_trap_gate(1, (uint) isr1);
    set_trap_gate(2, (uint) isr2);
    set_itr_gate(3, (uint) isr3);
    set_itr_gate(4, (uint) isr4);
    set_itr_gate(5, (uint) isr5);
    set_trap_gate(6, (uint) isr6);
    set_trap_gate(7, (uint) isr7);
    set_trap_gate(8, (uint) isr8);
    set_trap_gate(9, (uint) isr9);
    set_trap_gate(10, (uint) isr10);
    set_trap_gate(11, (uint) isr11);
    set_trap_gate(12, (uint) isr12);
    set_trap_gate(13, (uint) isr13);
    set_trap_gate(14, (uint) isr14);
    set_trap_gate(15, (uint) isr15);
    set_trap_gate(16, (uint) isr16);
    set_trap_gate(17, (uint) isr17);
    set_trap_gate(18, (uint) isr18);
    set_trap_gate(19, (uint) isr19);
    set_trap_gate(20, (uint) isr20);
    set_trap_gate(21, (uint) isr21);
    set_trap_gate(22, (uint) isr22);
    set_trap_gate(23, (uint) isr23);
    set_trap_gate(24, (uint) isr24);
    set_trap_gate(25, (uint) isr25);
    set_trap_gate(26, (uint) isr26);
    set_trap_gate(27, (uint) isr27);
    set_trap_gate(28, (uint) isr28);
    set_trap_gate(29, (uint) isr29);
    set_trap_gate(30, (uint) isr30);
    set_trap_gate(31, (uint) isr31);
}

void idt_init() {
    idt_p.limit = (SIZE_IDT_ENTRY * 256) - 1;
    idt_p.base = (uint) &idt;
    memset(&idt, 0, SIZE_IDT_ENTRY);

    isrs_init();
    
    __asm__ __volatile__ ("lidt %0" :: "m"(idt_p));
}

// Trap/Interrupt Handler
// Just print some information
void fault_handler(struct regs * rgs) {
    if (rgs->int_no < 32) {
        puts(exception_msg[rgs->int_no]);
        puts("Exception! System Halted! \n");
        for(;;);
    }
}
