/* multiboot.h - Multiboot header file. */
/* Copyright (C) 1999,2003,2007,2008,2009,2010  Free Software Foundation, Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
 *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef YATO_DEVICE_BOOT_MULTIBOOT_MULTIBOOT_H
#define YATO_DEVICE_BOOT_MULTIBOOT_MULTIBOOT_H

#define MULTIBOOT_SEARCH 8192
#define MULTIBOOT_HEADER_ALIGNMENT 4

#define MULTIBOOT_HEADER_MAGIC 0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

#define MULTIBOOT_MOD_ALIGNMENT 0x00001000
#define MULTIBOOT_INFO_ALIGNMENT 0x00000004

#define MULTIBOOT_PAGE_ALIGNMENT 0x00000001
#define MULTIBOOT_MEMORY_INFO 0x00000002
#define MULTIBOOT_VIDEO_MODE 0x00000004
#define MULTIBOOT_AOUT_KLUDGE 0x00010000

#define MULTIBOOT_INFO_MEMORY 0x00000001
#define MULTIBOOT_INFO_BOOTDEV 0x00000002
#define MULTIBOOT_INFO_CMDLINE 0x00000004
#define MULTIBOOT_INFO_MODS 0x00000008

/* These next two are mutually exclusive */
#define MULTIBOOT_INFO_AOUT_SYMS 0x00000010
#define MULTIBOOT_INFO_ELF_SHDR 0X00000020
#define MULTIBOOT_INFO_MEM_MAP 0x00000040
#define MULTIBOOT_INFO_DRIVE_INFO 0x00000080
#define MULTIBOOT_INFO_CONFIG_TABLE 0x00000100
#define MULTIBOOT_INFO_BOOT_LOADER_NAME 0x00000200
#define MULTIBOOT_INFO_APM_TABLE 0x00000400
#define MULTIBOOT_INFO_VBE_INFO 0x00000800
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO 0x00001000

#ifndef ASM_FILE

typedef unsigned char multiboot_uint8_t;
typedef unsigned short multiboot_uint16_t;
typedef unsigned int multiboot_uint32_t;
typedef unsigned long long multiboot_uint64_t;

struct MultibootHeader {
	// Must be MULTIBOOT_MAGIC - see above.
	multiboot_uint32_t magic;
	// Feature flags
	multiboot_uint32_t flags;
	// The above fields plus this one must equal 0 mod 2^32.
	multiboot_uint32_t checksum;

	// These are only valid if MULTIBOOT_AOUT_KLUDGE is set.
	multiboot_uint32_t header_addr;
	multiboot_uint32_t load_addr;
	multiboot_uint32_t load_end_addr;
	multiboot_uint32_t bss_end_addr;
	multiboot_uint32_t entry_addr;

	// These are only valid if MULTIBOOT_VIDEO_MODE is set.
	multiboot_uint32_t mode_type;
	multiboot_uint32_t width;
	multiboot_uint32_t height;
	multiboot_uint32_t depth;
};
typedef struct MultibootHeader MultibootHeader;

struct MultibootAOutSymbolTable {
	multiboot_uint32_t tabsize;
	multiboot_uint32_t strsize;
	multiboot_uint32_t addr;
	multiboot_uint32_t reserved;
};
typedef struct MultibootAOutSymbolTable MultibootAOutSymbolTable;

struct MultibootElfSectionHeaderTable {
	multiboot_uint32_t num;
	multiboot_uint32_t size;
	multiboot_uint32_t addr;
	multiboot_uint32_t shndex;
};
typedef struct MultibootElfSectionHeaderTable MultibootElfSectionHeaderTable;

struct MultibootInfo {
	multiboot_uint32_t flags;
	multiboot_uint32_t mem_lower;
	multiboot_uint32_t mem_upper;
	multiboot_uint32_t cmdline;
	multiboot_uint32_t mods_count;
	multiboot_uint32_t mods_addr;
	union {
		MultibootAOutSymbolTable aout_sym;
		MultibootElfSectionHeaderTable elf_section;
	} u;

	multiboot_uint32_t mmap_length;
	multiboot_uint32_t mmap_addr;

	multiboot_uint32_t drives_length;
	multiboot_uint32_t drives_addr;

	multiboot_uint32_t rom_config_table;

	multiboot_uint32_t boot_loader_name;

	multiboot_uint32_t apm_table;

	multiboot_uint32_t vbe_control_info;
	multiboot_uint32_t vbe_mode_info;
	multiboot_uint16_t vbe_mode;
	multiboot_uint16_t vbe_interface_seg;
	multiboot_uint16_t vbe_interface_off;
	multiboot_uint16_t vbe_interface_len;

	multiboot_uint64_t framebuffer_addr;
	multiboot_uint32_t framebuffer_pitch;
	multiboot_uint32_t framebuffer_width;
	multiboot_uint32_t framebuffer_height;
	multiboot_uint8_t framebuffer_bpp;

#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB 1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2
	multiboot_uint8_t framebuffer_type;
	union {
		struct {
			multiboot_uint32_t framebuffer_palette_addr;
			multiboot_uint16_t framebuffer_palette_num_colors;
		};
		struct {
			multiboot_uint8_t framebuffer_red_field_position;
			multiboot_uint8_t framebuffer_red_mask_size;
			multiboot_uint8_t framebuffer_green_field_position;
			multiboot_uint8_t framebuffer_green_mask_size;
			multiboot_uint8_t framebuffer_blue_field_position;
			multiboot_uint8_t framebuffer_blue_mask_size;
		};
	};
};
typedef struct MultibootInfo MultibootInfo;

struct MultibootColor {
	multiboot_uint8_t red;
	multiboot_uint8_t green;
	multiboot_uint8_t blue;
};
typedef struct MultibootColor MultibootColor;

struct MultibootMmapEntry {
	multiboot_uint32_t size;
	multiboot_uint64_t addr;
	multiboot_uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_MEMORY_RESERVED 2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS 4
#define MULTIBOOT_MEMORY_BADRAM 5
	multiboot_uint32_t type;
} __attribute__((packed));
typedef struct MultibootMmapEntry MultibootMmapEntry;

struct MultibootModList {
	multiboot_uint32_t mod_start;
	multiboot_uint32_t mod_end;

	multiboot_uint32_t cmdline;

	multiboot_uint32_t pad;
};
typedef struct MultibootModList MultibootModList;

struct MultibootApmInfo {
	multiboot_uint16_t version;
	multiboot_uint16_t cseg;
	multiboot_uint32_t offset;
	multiboot_uint16_t cseg_16;
	multiboot_uint16_t dseg;
	multiboot_uint16_t flags;
	multiboot_uint16_t cseg_len;
	multiboot_uint16_t cseg_16_len;
	multiboot_uint16_t dseg_len;
};
typedef struct MultibootApmInfo MultibootApmInfo;

#endif // ASM_FILE

#endif // YATO_DEVICE_BOOT_MULTIBOOT_MULTIBOOT_H
