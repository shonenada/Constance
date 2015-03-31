#ifndef __ASM_H
#define __ASM_H

inline void cli();
inline void sti();
inline void ltr(uint n);
inline void lldt(uint n);
inline void insl(int port, void* addr, int cnt);
inline void outsl(int port, const void* addr, int cnt);
inline void lpgd(struct pde *pgdir);

#endif
