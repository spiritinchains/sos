
#include <kernel.h>
#include <8042ctrl.h>

struct ps2ctrl_status ps2ctrl_getstat()
{
    char statb;
    inb(PS2CTRL_STAT_CMD, statb);
    // unholy pointer hack because C was being pedantic about struct casting
    return *(struct ps2ctrl_status*)&statb;
}

void ps2ctrl_sendcmd(char cmdbyte)
{
    outb(PS2CTRL_STAT_CMD, cmdbyte);
}

void ps2ctrl_readb(char* data)
{
    char d;
    // check is output buffer bit is set to 1
    struct ps2ctrl_status x = ps2ctrl_getstat();
    if (x.isOutputBufferFull) {
        inb(PS2CTRL_DATA, d)
    }
    *data = d;
}

void ps2ctrl_iramrb(char* data, char idx) {
    //
}

void ps2ctrl_iramwb(char data, char idx) {
    //
}
