/* Copyright (c) 2010 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Defines EFI related structure. See more details in EFI 2.3 spec.
 *
 * To download EFI standard, please visit UEFI homepage:
 *    http://www.uefi.org/
 */
#ifndef VBOOT_REFERENCE_CGPTLIB_GPT_H_
#define VBOOT_REFERENCE_CGPTLIB_GPT_H_

#include <stdint.h>

#define GPT_HEADER_SIGNATURE "EFI PART"
#define GPT_HEADER_SIGNATURE_SIZE sizeof(GPT_HEADER_SIGNATURE)
#define GPT_HEADER_REVISION 0x00010000

/* The first 3 numbers should be stored in network-endian format
 * according to the GUID RFC.  The UEFI spec appendix A claims they
 * should be stored in little-endian format.  But they need to be
 * _displayed_ in network-endian format, which is also how they're
 * documented in the specs.
 *
 * Since what we have here are little-endian constants, they're
 * byte-swapped from the normal display order. */
#define GPT_ENT_TYPE_UNUSED \
  {{{0x00000000,0x0000,0x0000,0x00,0x00,{0x00,0x00,0x00,0x00,0x00,0x00}}}}
/* Hey, that one's right regardless of endianness... */

#define GPT_ENT_TYPE_EFI \
  {{{0x28732ac1,0x1ff8,0xd211,0xba,0x4b,{0x00,0xa0,0xc9,0x3e,0xc9,0x3b}}}}
/* c12a7328-f81f-11d2-ba4b-00a0c93ec93b */

#define GPT_ENT_TYPE_CHROMEOS_KERNEL \
  {{{0x5d2a3afe,0x324f,0xa741,0xb7,0x25,{0xac,0xcc,0x32,0x85,0xa3,0x09}}}}
/* FE3A2A5D-4F32-41A7-B725-ACCC3285A309 */

#define GPT_ENT_TYPE_CHROMEOS_ROOTFS \
  {{{0x02e2b83c,0x7e3b,0xdd47,0x8a,0x3c,{0x7f,0xf2,0xa1,0x3c,0xfc,0xec}}}}
/* 3CB8E202-3B7E-47DD-8A3C-7FF2A13CFCEC */

#define GPT_ENT_TYPE_CHROMEOS_RESERVED \
  {{{0x3d750a2e,0x489e,0xb043,0x83,0x37,{0xb1,0x51,0x92,0xcb,0x1b,0x5e}}}}
/* 2e0a753d-9e48-43b0-8337-b15192cb1b5e */

#define UUID_NODE_LEN 6
#define GUID_SIZE 16

/*  GUID definition.
 *  Defined in appendix A of EFI standard.
 */
typedef struct {
  union {
    struct {
      uint32_t time_low;
      uint16_t time_mid;
      uint16_t time_high_and_version;
      uint8_t clock_seq_high_and_reserved;
      uint8_t clock_seq_low;
      uint8_t node[UUID_NODE_LEN];
    } Uuid;
    uint8_t raw[GUID_SIZE];
  } u;
} __attribute__((packed)) Guid;

/* GPT header defines how many partitions exist on a drive and sectors managed.
 * For every drive device, there are 2 headers, primary and secondary.
 * Most of fields are duplicated except my_lba and entries_lba.
 *
 * You may find more details in chapter 5 of EFI standard.
 */
typedef struct {
  char signature[8];
  uint32_t revision;
  uint32_t size;
  uint32_t header_crc32;
  uint32_t reserved;
  uint64_t my_lba;
  uint64_t alternate_lba;
  uint64_t first_usable_lba;
  uint64_t last_usable_lba;
  Guid disk_uuid;

  uint64_t entries_lba;
  uint32_t number_of_entries;
  uint32_t size_of_entry;
  uint32_t entries_crc32;
  uint8_t padding[512 - 92]; /* Pad to end of sector */
} GptHeader;

/* GPT partition entry defines the starting and ending LBAs of a partition.
 * It also contains the unique GUID, type, and attribute bits.
 *
 * You may find more details in chapter 5 of EFI standard.
 */
typedef struct {
  Guid type;
  Guid unique;
  uint64_t starting_lba;
  uint64_t ending_lba;
  uint64_t attributes;
  uint16_t name[36];  /* UTF-16 encoded partition name */
} GptEntry;

#endif  /* VBOOT_REFERENCE_CGPTLIB_GPT_H_ */