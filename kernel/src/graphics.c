
#include <kernel.h>
#include <multiboot2.h>

#include "vmm.h"

struct vbe_table
{
    uint16_t set_window;
    uint16_t set_disp_start;
    uint16_t set_primary_palette;
    uint16_t ports_memory;
}__attribute__((packed)) *vtab;

struct graphics_info
{
    uint32_t width;
    uint32_t height;
    uint32_t* fb;
} gfxinfo;

struct tga_header
{
    uint8_t id_len;
    uint8_t colormap;               // color map exists
    uint8_t img_type;               // image type
    uint16_t colormap_origin;       // color map origin
    uint16_t colormap_len;          // length of color map
    uint8_t colormap_entsize;       // color map entry size in bits
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t depth;
    uint8_t img_desc;
}__attribute__((packed));

enum tga_img_type
{
    TGA_NO_DATA = 0,
    TGA_COLORMAPPED = 1,
    TGA_RGB = 2,
    TGA_BW = 3,
    TGA_RLE_COLORMAPPED = 9,
    TGA_RLE_RGB = 10,
    TGA_COMPRESSED_BW = 11
};

void graphics_init()
{
    extern struct multiboot_tag_vbe* tag_mvbe;
    extern struct multiboot_tag_framebuffer* tag_mfbi;

    printk("\n---- Graphics ----\n");

    printk("Framebuffer: %x\n", (uint32_t)tag_mfbi->common.framebuffer_addr);

    uintptr_t vtab_addr;
    vtab_addr = (tag_mvbe->vbe_interface_seg << 4) + tag_mvbe->vbe_interface_off;
    vtab = vtab_addr & 0xFFFFF;

    printk("VBE address: %x\n", (uint32_t)vtab);
    printk("VBE length: %d\n", (uint32_t)tag_mvbe->vbe_interface_len);

    printk("%x\n", (uint32_t)(vtab->set_window));
    printk("%x\n", (uint32_t)(vtab->set_disp_start));
    printk("%x\n", (uint32_t)(vtab->set_primary_palette));
    printk("%x\n", (uint32_t)(vtab->ports_memory));

    gfxinfo.height = tag_mfbi->common.framebuffer_height;
    gfxinfo.width = tag_mfbi->common.framebuffer_width;
    gfxinfo.fb = 0xe0000000;

    uintptr_t phys_fb = tag_mfbi->common.framebuffer_addr;
    uint32_t fb_size = 4 * gfxinfo.height * gfxinfo.width;
    for (int i = 0; i < fb_size; i += 4096)
    {
        page_bind(0xe0000000 + i, phys_fb + i);
        // printk("%x %x\n", 0xe0000000 + i, (char*)gfxinfo.fb + i);
    }
}

void print_tga_info(struct tga_header* tga)
{
    printk("Resolution: %d x %d\n", tga->width, tga->height);
    printk("Depth: %d\n", tga->depth);
}

void image_draw(void* img, uint32_t x, uint32_t y)
{
    struct tga_header* tga = img;
    // print_tga_info(tga);

    uint32_t* colormap = NULL;

    uint8_t* src = (uint8_t*)img + sizeof(struct tga_header);

    if (tga->colormap == 1)
    {
        colormap = kmalloc(tga->colormap_len * sizeof(uint32_t));
        for (int i = 0; i < tga->colormap_len; i++)
        {
            if (tga->colormap_entsize == 32)
            {
                colormap[i] = ((uint32_t*)src)[i];
            }
            else if (tga->colormap_entsize == 24)
            {
                uint8_t* tmp = src;
                colormap[i] = *(uint32_t*)(src + i * 3);
                colormap[i] &= 0xFFFFFF;
            }
        }
        src += tga->colormap_len * tga->colormap_entsize / 8;
    }

    uint32_t base = x + y * gfxinfo.width;

    // colormap[250] = 0;
    // colormap[251] = 0;
    // colormap[252] = 0;
    // colormap[253] = 0;

    // uint32_t ca[4] = {
    //     0xFF00FF, 0x00FFFF, 0xFFFF00, 0x00FF00
    // };

    // extern uint64_t timer_ticks;
    // uint32_t t = timer_ticks % 64;
    // uint32_t c = t / 16;
    // uint32_t b = (t % 16) * 16;
    // uint32_t a = (255 - b);
    // colormap[250 + c] = (a | (a << 8) | (a << 16)) & ca[c];
    // colormap[250 + ((c+1) % 4)] = (b | (b << 8) | (b << 16)) & ca[(c+1)%4];

    for (int i = 0; i < tga->height; i++)
    {
        for (int j = 0; j < tga->width; j++)
        {
            if (x + j >= gfxinfo.width || y + i >= gfxinfo.height)
            {
                continue;
            }

            int u = tga->height - i - 1;
            // int v = tga->width - j - 1;
            int v = j;
            
            int k = i * tga->width + j;
            int w = u * tga->width + v;

            size_t index = base + k;

            if (tga->colormap == 1)
            {
                if (tga->depth == 8)
                {
                    gfxinfo.fb[index] = colormap[src[w]];
                }
            }
            else
            {
                gfxinfo.fb[index] = ((uint32_t*)src)[w];
            }
        }
    }

    if (colormap) kfree(colormap);
}
