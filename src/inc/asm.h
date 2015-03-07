#ifndef __ASM_H
#define __ASM_H

inline void cli();
inline void sti();
inline void ltr(uint n);
inline void lldt(uint n);

#endif
