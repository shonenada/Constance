#include <system.h>
#include <const.h>
#include <mm.h>
#include <page.h>

struct bucket_desc free_bk = {0,};
struct bk_dir bucket_dir[] = {
    {16, (struct bk_dir*) 0},
    {32, (struct bk_dir*) 0},
    {64, (struct bk_dir*) 0},
    {128, (struct bk_dir*) 0},
    {256, (struct bk_dir*) 0},
    {512, (struct bk_dir*) 0},
    {1024, (struct bk_dir*) 0},
    {2048, (struct bk_dir*) 0},
    {4096, (struct bk_dir*) 0}};

int bkslot(int size) {
    if (size <= 0) {
        return -1;
    }
    int i, n=16;
    for(i=0;(n<<i)<=4096;i++) {
        if (size <= (n<<i)) {
            return i;
        }
    }
    return -1;
}

int bkinit(struct bucket_desc *bucket, int size) {
    int i;
    uint page_addr;
    struct page *page;
    struct bucket_entry *be, *ben;

    page = palloc();
    page_addr = page->idx * PAGE_SIZE;
    bucket->size = size;
    bucket->page = page_addr;
    bucket->first = (struct bucket_entry *)page_addr;

    be = bucket->first;

    for (i=0;i<PAGE_SIZE/size;i++) {
        ben = (struct bucket_entry *) (page + i * size);
        be->next = ben;
        ben->next = NULL;
        be = ben;
    }

    return 0;
}

struct bucket_desc* bkalloc() {
    uint page_addr, page_end;
    struct page *page;
    struct bucket_desc* bucket, *list;

    if (free_bk.next == NULL) {
        list = &free_bk;
        page = palloc();        
        page_addr = page->idx * PAGE_SIZE;
        page_end = page_addr + PAGE_SIZE;
        cli();
        for (bucket=(struct bucket_desc*)page_addr;(uint)bucket < page_end; bk++) {
            list->next = bucket;
            bucket->next = NULL;
            list = bucket;
        }
        sti();
    }
    bucket = free_bk.next;
    free_bk.next = bucket.next;
    return bucket;
}

int bkfree(struct bucket_desc* bucket) {
    cli();
    bucket->next = free_bk.next;
    free_bk.next = bucket;
    sti();
    return 0;
}

void* kmalloc(int size) {
    uint idx;
    uint page_addr;
    struct page *page;
    struct bk_dir dir;
    struct bucket_desc* bucket;

    idx = bkslot(size);
    if (idx < 0) {
        panic("kmalloc(): wrong size");
        return NULL;
    }

    dir = bucket_dir[idx];
    size = dir->size;
    bucket = dir.next;

    if (size == PAGE_SIZE) {
        page = palloc();
        return (void*)(page->idx * PAGE_SIZE);
    }

    return _kalloc(bucket, size);
}

inline void* _kalloc(struct bucket_desc* bucket, int size) {
    struct bucket_desc* bk;
    struct bucket_entry *be;

    bk = bucket;

_loop:
    while (bk->next != NULL) {
        bk = bk->next;
        if (bk->first != NULL) {
            be = bk->first;
            bk->first = bk->first->next;
            return (void*)be;
        }
    }
    // not found
    bk = bkalloc();
    bkinit(bk, size);
    bk->next = bucket->next;
    bucket->bk_next = bk;
    goto _loop;
}

int kmfree(void *addr, uint size) {
    int idx;
    uint page_addr;
    struct page *page;
    struct bucket_dir dir;
    struct bucket_desc *bucket;
    struct bucket_entry *be;

    idx = bkslot(size);
    if (idx < 0) {
        panic("kfree(): wrong size");
        return -1;
    }
    page_addr = PPN(addr) * PAGE_SIZE;

    if (size == PAGE_SIZE) {
        page = pfind(PPN(addr));
        pfree(page);
        return 0;
    }

    dir = bucket_dir[idx];
    size = dir->size;
    bucket = dir.next;
    be = (struct bucket_entry*)addr;

    while (bucket->next != NULL) {
        bucket = bucket->next;
        if (bucket->page == page_addr) {
            be->next = bucket->first;
            bucket->first = be;
            return 0;
        }
    }
    return -1;
}
