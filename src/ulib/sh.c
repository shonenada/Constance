#include <const.h>
#include <system.h>
#include <fs.h>
#include <sh.h>
#include <console.h>
#include <sched.h>

#define LINE_SIZE 1024

// TODO: refactor;
int exec_cmd(char* buf) {
    int idx = 0;
    char *tmp;
    char arg[100];
    tmp = buf;
    if (strncmp(tmp, "cd", 2) == 0) {
        tmp += 3;
        while(*tmp != '\0' && *tmp != ' ')
            arg[idx++] = *tmp++;
        arg[idx] = '\0';
        do_chdir(arg);
    }
    else if (strncmp(tmp, "ls", 2) == 0) {
        tmp += 3;
        while(*tmp != '\0' && *tmp != ' ')
            arg[idx++] = *tmp++;
        arg[idx] = '\0';
        do_ls(arg);
    }
    else if (strncmp(tmp, "cat", 3) == 0) {
        tmp += 4;
        while(*tmp != '\0' && *tmp != ' ')
            arg[idx++] = *tmp++;
        arg[idx] = '\0';
        do_cat(arg);
    } else if (strncmp(tmp, "print", 5) == 0) {
        dump_task(current);
        dump_inode(current->wdir);
    } else {
        printk("command not found: %s\n", tmp);
    }
}

int repl() {
    int cnt;
    int cmd;
    char line_buf[LINE_SIZE];
    while(1) {
        puts("$ ");
        memset(line_buf, 0, LINE_SIZE);
        cnt = do_read(0, line_buf, LINE_SIZE);
        line_buf[cnt] = 0;
        exec_cmd(line_buf);
    }
    return 0;
}
