#include <system.h>
#include <segment.h>
#include <time.h>
#include <console.h>
#include <unistd.h>

extern uint hwint[256];
struct idt_entry idt[256];    // must be 256
struct idt_ptr idt_p;

int *irq_routines[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

char *exception_msg[] = {
    "Division By Zero",
    "Debug",
    "No Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Out of Bounds",
    "Invalid Operand Code",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TTS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

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
    idt_set(num, base, KERN_CS, STS_TYPE_TG32, RING3);
}

// Remap IRQ, or every time IRQ fires, we would get Double Fault Exception
// IRQ 0 to 15 are mapped to IDT[32] to IDT[47]
void irq_remap(void) {
    outportb(MASTER_PIC_PORT, 0x11);
    outportb(SLAVE_PIC_PORT, 0x11);
    outportb(MASTER_PIC_PORT + 1, 0x20);
    outportb(SLAVE_PIC_PORT + 1, 0x28);
    outportb(MASTER_PIC_PORT + 1, 0x04);
    outportb(SLAVE_PIC_PORT + 1, 0x02);
    outportb(MASTER_PIC_PORT + 1, 0x01);
    outportb(SLAVE_PIC_PORT + 1, 0x01);
    outportb(MASTER_PIC_PORT + 1, 0x00);
    outportb(SLAVE_PIC_PORT + 1, 0x00);
}

void isrs_init() {
    int i;
    for (i=0;i<32;i++)
        set_trap_gate(i, hwint[i]);
    set_task_gate(0x3, hwint[0x3]);
    set_task_gate(0x4, hwint[0x4]);
    set_task_gate(0x5, hwint[0x5]);
    set_task_gate(0x80, hwint[0x80]);
}

void irq_install(int no, int (*handler)(struct regs* r)) {
    irq_routines[no] = handler;
}

void irq_uninstall(int no) {
    irq_routines[no] = 0;
}

void irq_init() {
   // set_itr_gate(32, (uint) irq32);
    int i;
    for (i=32;i<48;i++)
        set_itr_gate(i, hwint[i]);
}

void idt_init() {
    irq_remap();
    idt_p.limit = (SIZE_IDT_ENTRY * 256) - 1;
    idt_p.base = (uint) &idt;
    memset(&idt, 0, SIZE_IDT_ENTRY * 256);
    isrs_init();
    irq_init();
    asm volatile ("lidt %0" :: "m"(idt_p));
}

// Trap/Interrupt Handler
// Just print some information
void int_handler(struct regs* rgs) {
    int (*handler) (struct regs *r);

    if (rgs->int_no < 32) {
        printk(exception_msg[rgs->int_no]);
        printk(" Exception with ERRNO %d ! System Halted! \n", rgs->err_code);
        for(;;);
    }
    else if (rgs->int_no >= 32 && rgs->int_no != 0x80) {
        handler = irq_routines[rgs->int_no - 32];
        if (handler) {
            handler(rgs);
        }
        if (rgs->int_no >= 40) {
            outportb(SLAVE_PIC_PORT, 0x20);     // send EOI to slave PIC
        }
        outportb(MASTER_PIC_PORT, 0x20);     // send EOI to master PIC
    }
    else if (rgs->int_no >= 120) {    // syscall
        handler = &do_syscall;
        handler(rgs);
    }
}
