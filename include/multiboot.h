/* multiboot.h --> 定义一些Magic Number */
/* Copyright (c) 1998 著作权由Chapaev所有。著作权人保留一切权利。
 * 
 * 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
 * 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
 * 
 * * 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
 *   及下述的免责声明。
 * * 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
 *   于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
 *   的免责声明。
 * * 未获事前取得书面许可，不得使用本软件贡献者之名称，
 *   来为本软件之衍生物做任何表示支持、认可或推广、促销之行为。
 * 
 * 免责声明：本软件是由本软件之贡献者以现状（"as is"）提供，
 * 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
 * 的的适用性为默示性担保。加州大学董事会及本软件之贡献者，无论任何条件、
 * 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
 * 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
 * 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
 * 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
 * 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。*/

#ifndef __ATOMIC_MULTIBOOT_H__
#define __ATOMIC_MULTIBOOT_H__
     
/* How many bytes from the start of the file we search for the header. */
#define MULTIBOOT_SEARCH                        8192
     
/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC                  0x1BADB002
     
/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002
     
/* The bits in the required part of flags field we don't support. */
#define MULTIBOOT_UNSUPPORTED                   0x0000fffc
     
/* Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN                     0x00001000
     
/* Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN                    0x00000004
     
/* Flags set in the 'flags' member of the multiboot header. */
     
/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN                    0x00000001
     
/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO                   0x00000002
     
/* Must pass video information to OS. */
#define MULTIBOOT_VIDEO_MODE                    0x00000004
     
/* This flag indicates the use of the address fields in the header. */
#define MULTIBOOT_AOUT_KLUDGE                   0x00010000
     
/* Flags to be set in the 'flags' member of the multiboot info structure. */
     
/* is there basic lower/upper memory information? */
#define MULTIBOOT_INFO_MEMORY                   0x00000001
/* is there a boot device set? */
#define MULTIBOOT_INFO_BOOTDEV                  0x00000002
/* is the command-line defined? */
#define MULTIBOOT_INFO_CMDLINE                  0x00000004
/* are there modules to do something with? */
#define MULTIBOOT_INFO_MODS                     0x00000008
     
/* These next two are mutually exclusive */
     
/* is there a symbol table loaded? */
#define MULTIBOOT_INFO_AOUT_SYMS                0x00000010
/* is there an ELF section header table? */
#define MULTIBOOT_INFO_ELF_SHDR                 0X00000020
     
/* is there a full memory map? */
#define MULTIBOOT_INFO_MEM_MAP                  0x00000040
     
/* Is there drive info? */
#define MULTIBOOT_INFO_DRIVE_INFO               0x00000080
     
/* Is there a config table? */
#define MULTIBOOT_INFO_CONFIG_TABLE             0x00000100
     
/* Is there a boot loader name? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME         0x00000200
     
/* Is there a APM table? */
#define MULTIBOOT_INFO_APM_TABLE                0x00000400
     
/* Is there video information? */
#define MULTIBOOT_INFO_VIDEO_INFO               0x00000800
     
#ifndef ASM_FILE
     
typedef unsigned short          multiboot_uint16_t;
typedef unsigned int            multiboot_uint32_t;
typedef unsigned long long      multiboot_uint64_t;
     
struct multiboot_header
{
	/* Must be MULTIBOOT_MAGIC - see above. */
	multiboot_uint32_t magic;
     
	/* Feature flags. */
	multiboot_uint32_t flags;
     
	/* The above fields plus this one must equal 0 mod 2^32. */
	multiboot_uint32_t checksum;
     
	/* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
	multiboot_uint32_t header_addr;
	multiboot_uint32_t load_addr;
	multiboot_uint32_t load_end_addr;
	multiboot_uint32_t bss_end_addr;
	multiboot_uint32_t entry_addr;
     
	/* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
	multiboot_uint32_t mode_type;
	multiboot_uint32_t width;
	multiboot_uint32_t height;
	multiboot_uint32_t depth;
};
     
/* The symbol table for a.out. */
struct multiboot_aout_symbol_table
{
	multiboot_uint32_t tabsize;
	multiboot_uint32_t strsize;
	multiboot_uint32_t addr;
	multiboot_uint32_t reserved;
};
typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;
     
/* The section header table for ELF. */
struct multiboot_elf_section_header_table
{
	multiboot_uint32_t num;
	multiboot_uint32_t size;
	multiboot_uint32_t addr;
	multiboot_uint32_t shndx;
};
typedef struct multiboot_elf_section_header_table multiboot_elf_section_header_table_t;
     
struct multiboot_info
{
	/* Multiboot info version number */
	multiboot_uint32_t flags;
     
	/* Available memory from BIOS */
	multiboot_uint32_t mem_lower;
	multiboot_uint32_t mem_upper;
     
	/* "root" partition */
	multiboot_uint32_t boot_device;
     
	/* Kernel command line */
	multiboot_uint32_t cmdline;
     
	/* Boot-Module list */
	multiboot_uint32_t mods_count;
	multiboot_uint32_t mods_addr;
     
	union
	{
		multiboot_aout_symbol_table_t aout_sym;
		multiboot_elf_section_header_table_t elf_sec;
	} u;
     
	/* Memory Mapping buffer */
	multiboot_uint32_t mmap_length;
	multiboot_uint32_t mmap_addr;
     
	/* Drive Info buffer */
	multiboot_uint32_t drives_length;
	multiboot_uint32_t drives_addr;
     
	/* ROM configuration table */
	multiboot_uint32_t config_table;
     
	/* Boot Loader Name */
	multiboot_uint32_t boot_loader_name;
     
	/* APM table */
	multiboot_uint32_t apm_table;
     
	/* Video */
	multiboot_uint32_t vbe_control_info;
	multiboot_uint32_t vbe_mode_info;
	multiboot_uint16_t vbe_mode;
	multiboot_uint16_t vbe_interface_seg;
	multiboot_uint16_t vbe_interface_off;
	multiboot_uint16_t vbe_interface_len;
};
typedef struct multiboot_info multiboot_info_t;
     
struct multiboot_mmap_entry
{
	multiboot_uint32_t size;
	multiboot_uint64_t addr;
	multiboot_uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
	multiboot_uint32_t type;
} __attribute__((packed));
typedef struct multiboot_mmap_entry multiboot_memory_map_t;
     
struct multiboot_mod_list
{
	/* the memory used goes from bytes 'mod_start' to 'mod_end-1' inclusive */
	multiboot_uint32_t mod_start;
	multiboot_uint32_t mod_end;
     
	/* Module command line */
	multiboot_uint32_t cmdline;
     
	/* padding to take it to 16 bytes (must be zero) */
	multiboot_uint32_t pad;
};
typedef struct multiboot_mod_list multiboot_module_t;
     
#endif /* ! ASM_FILE */
     
#endif /* ! MULTIBOOT_HEADER */
