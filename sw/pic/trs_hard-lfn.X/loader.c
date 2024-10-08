#include "integer.h"

const BYTE loader_code[256] = {
#if 0
    // new loader
    // try loading frehdx.rom, then frehd.rom
    0xFE, 0xFE, 0xFF, 0x18, 0x02, 0x18, 0x2D, 0x3A,
    0x40, 0x38, 0xE6, 0x80, 0xC0, 0x11, 0x29, 0x50,
    0x06, 0x0A, 0xCD, 0x33, 0x50, 0x28, 0x09, 0x11,
    0x20, 0x50, 0x06, 0x09, 0xCD, 0x33, 0x50, 0xC0,
    0xE9, 0x46, 0x52, 0x45, 0x48, 0x44, 0x2E, 0x52,
    0x4F, 0x4D, 0x46, 0x52, 0x45, 0x48, 0x44, 0x58,
    0x2E, 0x52, 0x4F, 0x4D, 0x21, 0x00, 0x00, 0x39,
    0xE5, 0xDD, 0xE1, 0xEB, 0x3E, 0x03, 0xD3, 0xC4,
    0xCD, 0xB8, 0x50, 0x78, 0x3C, 0x3C, 0xD3, 0xC3,
    0x0E, 0xC2, 0x3E, 0x01, 0xED, 0x79, 0xED, 0xB3,
    0xAF, 0xED, 0x79, 0xCD, 0xB8, 0x50, 0xC0, 0x0E,
    0xFF, 0xCD, 0x93, 0x50, 0x3D, 0x20, 0x0C, 0xCD,
    0x84, 0x50, 0xCD, 0x93, 0x50, 0x77, 0x23, 0x10,
    0xF9, 0x18, 0xEE, 0x3D, 0x28, 0x0B, 0xCD, 0x93,
    0x50, 0x47, 0xCD, 0x93, 0x50, 0x10, 0xFB, 0x18,
    0xE0, 0xCD, 0x84, 0x50, 0x3E, 0x06, 0xD3, 0xC4,
    0xCD, 0xB8, 0x50, 0xAF, 0xC9, 0xCD, 0x93, 0x50,
    0x47, 0xCD, 0x93, 0x50, 0x6F, 0x05, 0xCD, 0x93,
    0x50, 0x67, 0x05, 0xC9, 0x0C, 0x20, 0x03, 0xCD,
    0x9C, 0x50, 0xDB, 0xC2, 0xC9, 0xAF, 0xD3, 0xC3,
    0x3E, 0x04, 0xD3, 0xC4, 0xCD, 0xB8, 0x50, 0x20,
    0x05, 0xDB, 0xCF, 0xE6, 0x08, 0xC0, 0x3E, 0x06,
    0xD3, 0xC4, 0xCD, 0xB8, 0x50, 0xDD, 0xF9, 0x3C,
    0xC9, 0xDB, 0xCF, 0x07, 0x38, 0xFB, 0xDB, 0xCF,
    0xE6, 0x01, 0xC9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
#else
    // original loader
    // load frehd.rom
    0xfe, 0xfe, 0xff, 0x18, 0x02, 0x18, 0x19, 0x3a,
    0x40, 0x38, 0xe6, 0x80, 0xc0, 0x11, 0x16, 0x50,
    0x06, 0x09, 0xcd, 0x1f, 0x50, 0xc0, 0xe9, 0x46,
    0x52, 0x45, 0x48, 0x44, 0x2e, 0x52, 0x4f, 0x4d,
    0x21, 0x00, 0x00, 0x39, 0xe5, 0xdd, 0xe1, 0xeb,
    0x3e, 0x03, 0xd3, 0xc4, 0xcd, 0xa4, 0x50, 0x78,
    0x3c, 0x3c, 0xd3, 0xc3, 0x0e, 0xc2, 0x3e, 0x01,
    0xed, 0x79, 0xed, 0xb3, 0xaf, 0xed, 0x79, 0xcd,
    0xa4, 0x50, 0xc0, 0x0e, 0xff, 0xcd, 0x7f, 0x50,
    0x3d, 0x20, 0x0c, 0xcd, 0x70, 0x50, 0xcd, 0x7f,
    0x50, 0x77, 0x23, 0x10, 0xf9, 0x18, 0xee, 0x3d,
    0x28, 0x0b, 0xcd, 0x7f, 0x50, 0x47, 0xcd, 0x7f,
    0x50, 0x10, 0xfb, 0x18, 0xe0, 0xcd, 0x70, 0x50,
    0x3e, 0x06, 0xd3, 0xc4, 0xcd, 0xa4, 0x50, 0xaf,
    0xc9, 0xcd, 0x7f, 0x50, 0x47, 0xcd, 0x7f, 0x50,
    0x6f, 0x05, 0xcd, 0x7f, 0x50, 0x67, 0x05, 0xc9,
    0x0c, 0x20, 0x03, 0xcd, 0x88, 0x50, 0xdb, 0xc2,
    0xc9, 0xaf, 0xd3, 0xc3, 0x3e, 0x04, 0xd3, 0xc4,
    0xcd, 0xa4, 0x50, 0x20, 0x05, 0xdb, 0xcf, 0xe6,
    0x08, 0xc0, 0x3e, 0x06, 0xd3, 0xc4, 0xcd, 0xa4,
    0x50, 0xdd, 0xf9, 0x3c, 0xc9, 0xdb, 0xcf, 0x07,
    0x38, 0xfb, 0xdb, 0xcf, 0xe6, 0x01, 0xc9, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
#endif
};
