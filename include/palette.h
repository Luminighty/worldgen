#ifndef PALETTE_H
#define PALETTE_H

#define RGB(r,g,b) (r << 16 | g << 8 | b)

#define PAL_GRAY(v) RGB(v, v, v)

#define PAL_BLACK		PAL_GRAY(0)
#define PAL_WHITE		PAL_GRAY(0xFF)

#define PAL_BROWN		RGB(0xA0, 0x52, 0x2D)
#define PAL_DARK_BROWN		RGB(0x8B, 0x45, 0x13)
#define PAL_RED			RGB(0xBD, 0x00, 0x13)
#define PAL_GREEN		RGB(0x4A, 0xB1, 0x18)
#define PAL_ORANGE		RGB(0xE7, 0x74, 0x1E)
#define PAL_YELLOW		RGB(0xF5, 0xDD, 0x42)
#define PAL_BLUE		RGB(0x0F, 0x4A, 0xC6)
#define PAL_PURPLE		RGB(0x83, 0x08, 0xff)


#define color_into_raylib(color) (Color){.r = ((color) >> 16) & 0xFF, .g = ((color) >> 8) & 0xFF, .b = (color) & 0xFF, .a=255 }


#endif // PALETTE_H
