#include <system.h>
#include <sched.h>

struct task* current;
struct task* tasks[NR_TASKS];

