#include "pebble_app_info.h"
#include "src/resource_ids.auto.h"

const PebbleAppInfo __pbl_app_info __attribute__ ((section (".pbl_header"))) = {
  .header = "PBLAPP",
  .struct_version = { APP_INFO_CURRENT_STRUCT_VERSION_MAJOR, APP_INFO_CURRENT_STRUCT_VERSION_MINOR },
  .sdk_version = { APP_INFO_CURRENT_SDK_VERSION_MAJOR, APP_INFO_CURRENT_SDK_VERSION_MINOR },
  .app_version = { 2, 7 },
  .load_size = 0xb6b6,
  .offset = 0xb6b6b6b6,
  .crc = 0xb6b6b6b6,
  .name = "Calendar App",
  .company = "William Heaton",
  .icon_resource_id = RESOURCE_ID_IMAGE_MENU_ICON,
  .sym_table_addr = 0xA7A7A7A7,
  .flags = 0,
  .num_reloc_entries = 0xdeadcafe,
  .uuid = { 0xB4, 0x1E, 0x3D, 0xCF, 0x61, 0x62, 0x41, 0x47, 0x9C, 0x58, 0x64, 0x3E, 0x10, 0x91, 0xFB, 0x93 },
  .virtual_size = 0xb6b6
};
