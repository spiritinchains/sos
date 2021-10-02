
#pragma once

struct ps2ctrl_status {
    unsigned isOutputBufferFull : 1;     // must be set before reading data
    unsigned isInputBufferFull : 1;      // must be clear before writing data
    unsigned sysFlag : 1;
    unsigned isCommand : 1;              // is written data a command or device data
    unsigned unk4 : 1;                   // Chipset specific
    unsigned unk5 : 1;                   // Chipset specific
    unsigned timeoutError : 1;
    unsigned parityError : 1;
}__attribute((packed));

struct ps2ctrl_config {
    unsigned firstPortIntt : 1;
    unsigned secondPortIntt : 1;
    unsigned sysFlag : 1;
    unsigned bit3 : 1;                   // Should be zero
    unsigned firstPortClk : 1;
    unsigned secondPortClk : 1;
    unsigned firstPortTransl : 1;
    unsigned bit7 : 1;                   // Must be zero
};

// TODO: keycode stuff will go here
