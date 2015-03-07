#ifndef __ASM_H
#define __ASM_H

inline volatile void cli();
inline volatile void sti();
inline volatile void ltr(uint n);
inline volatile void lldt(uint n);

#endif
