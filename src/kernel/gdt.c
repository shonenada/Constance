#include <system.h>

struct gdt_entry gdt[GDT_SIZE + 1];
struct gdt_ptr gp;

extern void gdt_flush();

void set_gdt(struct gdt_entry *entry, uint base, uint limit, uint dpl, uint type) {
    entry->base_low = base & 0xFFFF;
    entry->base_mid = (base >> 16) & 0xFF;
    entry->base_high = (base >> 24) & 0xFF;

    entry->limit_low = limit & 0xFFFF;
    entry->limit_high = (limit >> 16) & 0x0F;

    entry->attr_dpl = dpl;
    entry->attr_type = type;

    entry->attr_s = 1;
    entry->attr_present = 1;
    entry->attr_avl = 0;
    entry->attr_0 = 0;
    entry->attr_db = 1;    // Operand size 32-bit
    entry->attr_g= 1;     // 4KB
}

void gdt_init() {
    set_gdt(&gdt[0], 0, 0, 0, 0);    // Null GDT
    set_gdt(&gdt[1], 0, 0xFFFFFFFF, RING0, SEG_CODE_EXRDCA);
    set_gdt(&gdt[2], 0, 0xFFFFFFFF, RING0, SEG_DATA_RW);
    set_gdt(&gdt[3], 0, 0xFFFFFFFF, RING3, SEG_CODE_EXRDCA);
    set_gdt(&gdt[4], 0, 0xFFFFFFFF, RING3, SEG_DATA_RW);

    gdt_flush();    // flush gdt, defined in entry.asm

}
