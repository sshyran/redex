/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Util.h"

/*
 * The the dex spec can be found here:
 * https://source.android.com/devices/tech/dalvik/dex-format.html
 *
 * The values here can be found within the spec.  Naming is
 * kept close enough so that you should be able to search
 * the spec for the variable name.
 *
 */

#define DEX_HEADER_DEXMAGIC_V35 "dex\n035"
#define DEX_HEADER_DEXMAGIC_V37 "dex\n037"
#define DEX_HEADER_DEXMAGIC_V38 "dex\n038"
#define DEX_HEADER_DEXMAGIC_V39 "dex\n039"

#define ENDIAN_CONSTANT (0x12345678)
#define REVERSE_ENDIAN_CONSTANT (0x78563412)

/* clang-format off */
#define TYPE_HEADER_ITEM               (0x0000)
#define TYPE_STRING_ID_ITEM            (0x0001)
#define TYPE_TYPE_ID_ITEM              (0x0002)
#define TYPE_PROTO_ID_ITEM             (0x0003)
#define TYPE_FIELD_ID_ITEM             (0x0004)
#define TYPE_METHOD_ID_ITEM            (0x0005)
#define TYPE_CLASS_DEF_ITEM            (0x0006)
#define TYPE_CALL_SITE_ID_ITEM         (0x0007)
#define TYPE_METHOD_HANDLE_ITEM        (0x0008)
#define TYPE_MAP_LIST                  (0x1000)
#define TYPE_TYPE_LIST                 (0x1001)
#define TYPE_ANNOTATION_SET_REF_LIST   (0x1002)
#define TYPE_ANNOTATION_SET_ITEM       (0x1003)
#define TYPE_CLASS_DATA_ITEM           (0x2000)
#define TYPE_CODE_ITEM                 (0x2001)
#define TYPE_STRING_DATA_ITEM          (0x2002)
#define TYPE_DEBUG_INFO_ITEM           (0x2003)
#define TYPE_ANNOTATION_ITEM           (0x2004)
#define TYPE_ENCODED_ARRAY_ITEM        (0x2005)
#define TYPE_ANNOTATIONS_DIR_ITEM      (0x2006)
#define TYPE_HIDDENAPI_CLASS_DATA_ITEM (0xF000)

enum MethodHandleType {
  // Method handle is a static field setter (accessor)
  METHOD_HANDLE_TYPE_STATIC_PUT = 0x00,
  // Method handle is a static field getter (accessor)
  METHOD_HANDLE_TYPE_STATIC_GET = 0x01,
  // Method handle is an instance field setter (accessor)
  METHOD_HANDLE_TYPE_INSTANCE_PUT = 0x02,
  // Method handle is an instance field getter (accessor)
  METHOD_HANDLE_TYPE_INSTANCE_GET = 0x03,
  // Method handle is a static method invoker
  METHOD_HANDLE_TYPE_INVOKE_STATIC = 0x04,
  // Method handle is an instance method invoker
  METHOD_HANDLE_TYPE_INVOKE_INSTANCE = 0x05,
  // Method handle is a constructor method invoker
  METHOD_HANDLE_TYPE_INVOKE_CONSTRUCTOR = 0x06,
  // Method handle is a direct method invoker
  METHOD_HANDLE_TYPE_INVOKE_DIRECT = 0x07,
  // Method handle is an interface method invoker
  METHOD_HANDLE_TYPE_INVOKE_INTERFACE = 0x08 
};

/* clang-format on */

#define type_id_item uint32_t
#define string_id_item uint32_t

/*
 * This header exists at the beginning of a non-optimized dex.  The checking
 * we do on this has to do with making sure we're working on a non-opt
 * dex.  See link to Dalvik Executable Format above.
 */

PACKED(struct dex_header {
  char magic[8];
  uint32_t checksum;
  uint8_t signature[20];
  uint32_t file_size;
  uint32_t header_size;
  uint32_t endian_tag;
  uint32_t link_size;
  uint32_t link_off;
  uint32_t map_off;
  uint32_t string_ids_size;
  uint32_t string_ids_off;
  uint32_t type_ids_size;
  uint32_t type_ids_off;
  uint32_t proto_ids_size;
  uint32_t proto_ids_off;
  uint32_t field_ids_size;
  uint32_t field_ids_off;
  uint32_t method_ids_size;
  uint32_t method_ids_off;
  uint32_t class_defs_size;
  uint32_t class_defs_off;
  uint32_t data_size;
  uint32_t data_off;
});

PACKED(struct dex_string_id { uint32_t offset; });

PACKED(struct dex_type_id { uint32_t string_idx; });

PACKED(struct dex_map_item {
  uint16_t type;
  uint16_t na /* Not used */;
  uint32_t size /* Item count, not byte size */;
  uint32_t offset /* From start of file */;
});

PACKED(struct dex_map_list {
  uint32_t size; /* Number of items below */
  dex_map_item items[];
});

#define DEX_NO_INDEX (0xffffffff)
PACKED(struct dex_class_def {
  uint32_t typeidx;
  uint32_t access_flags;
  uint32_t super_idx;
  uint32_t interfaces_off;
  uint32_t source_file_idx;
  uint32_t annotations_off;
  uint32_t class_data_offset;
  uint32_t static_values_off;
});

PACKED(struct dex_method_id {
  uint16_t classidx;
  uint16_t protoidx;
  uint32_t nameidx;
});

PACKED(struct dex_field_id {
  uint16_t classidx;
  uint16_t typeidx;
  uint32_t nameidx;
});

PACKED(struct dex_proto_id {
  uint32_t shortyidx;
  uint32_t rtypeidx;
  uint32_t param_off;
});

PACKED(struct dex_callsite_id { uint32_t callsite_off; });

PACKED(struct dex_methodhandle_id {
  uint16_t method_handle_type;
  uint16_t unused1;
  uint16_t field_or_method_id;
  uint16_t unused2;
});

PACKED(struct dex_field_annotation {
  uint32_t field_idx;
  uint32_t annotations_off;
});

PACKED(struct dex_method_annotation {
  uint32_t method_idx;
  uint32_t annotations_off;
});

PACKED(struct dex_parameter_annotation {
  uint32_t method_idx;
  uint32_t annotations_off;
});

PACKED(struct dex_type_item { uint16_t type_idx; });

PACKED(struct dex_annotation_set_ref_item { uint32_t annotations_off; });

PACKED(struct dex_annotation_off_item { uint32_t annotation_off; });

PACKED(struct dex_code_item {
  uint16_t registers_size;
  uint16_t ins_size;
  uint16_t outs_size;
  uint16_t tries_size;
  uint32_t debug_info_off;
  uint32_t insns_size;
});

PACKED(struct dex_tries_item {
  uint32_t start_addr;
  uint16_t insn_count;
  uint16_t handler_off;
});

PACKED(struct dex_annotations_directory_item {
  uint32_t class_annotations_off;
  uint32_t fields_size;
  uint32_t methods_size;
  uint32_t parameters_size;
});

/* clang-format off */
using DexDebugItemOpcode = uint8_t;
enum DexDebugItemOpcodeValues : uint8_t {
  DBG_END_SEQUENCE         = 0x00,
  DBG_ADVANCE_PC           = 0x01,
  DBG_ADVANCE_LINE         = 0x02,
  DBG_START_LOCAL          = 0x03,
  DBG_START_LOCAL_EXTENDED = 0x04,
  DBG_END_LOCAL            = 0x05,
  DBG_RESTART_LOCAL        = 0x06,
  DBG_SET_PROLOGUE_END     = 0x07,
  DBG_SET_EPILOGUE_BEGIN   = 0x08,
  DBG_SET_FILE             = 0x09
};
/* clang-format on */

constexpr int32_t DBG_FIRST_SPECIAL = 0x0a;
constexpr int32_t DBG_LAST_SPECIAL = 0xff;
constexpr int32_t DBG_LINE_BASE = -4;
constexpr int32_t DBG_LINE_RANGE = 15;
