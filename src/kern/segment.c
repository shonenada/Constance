#include <system.h>
#include <segment.h>

struct seg_desc gdt[GDT_SIZE];
struct gdt_ptr gp;

void seg_set(struct seg_desc *entry, uint base, uint limit, uint dpl, uint type) {
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

void ldt_set(struct seg_desc *entry, uint base) {
}

void tss_set(struct seg_desc *entry, uint base) {
}

void gdt_init() {
    seg_set(&gdt[0], 0, 0, 0, 0);    // Null GDT
    seg_set(&gdt[1], 0, 0xFFFFFFFF, RING0, SEG_CODE_EXRD);
    seg_set(&gdt[2], 0, 0xFFFFFFFF, RING0, SEG_DATA_RW);
    seg_set(&gdt[3], 0, 0xFFFFFFFF, RING3, SEG_CODE_EXRD);
    seg_set(&gdt[4], 0, 0xFFFFFFFF, RING3, SEG_DATA_RW);

    gp.base = (uint) &gdt;
    gp.limit = (SIZE_GDT_ENTRY * GDT_SIZE) - 1;

    gdt_flush();    // flush gdt, defined in entry.asm
}
