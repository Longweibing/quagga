/*
  PIM for Quagga
  Copyright (C) 2008  Everton da Silva Marques

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; see the file COPYING; if not, write to the
  Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
  MA 02110-1301 USA
  
  $QuaggaId: $Format:%an, %ai, %h$ $
*/

#include <zebra.h>

#include "pimd.h"
#include "pim_pim.h"
#include "pim_msg.h"
#include "pim_util.h"

void pim_msg_build_header(char *pim_msg, int pim_msg_size,
			  uint8_t pim_msg_type)
{
  uint16_t checksum;

  zassert(pim_msg_size >= PIM_PIM_MIN_LEN);

  /*
   * Write header
   */

  *(uint8_t *) PIM_MSG_HDR_OFFSET_VERSION(pim_msg) = (PIM_PROTO_VERSION << 4) | pim_msg_type;
  *(uint8_t *) PIM_MSG_HDR_OFFSET_RESERVED(pim_msg) = 0;

  /*
   * Compute checksum
   */

  *(uint16_t *) PIM_MSG_HDR_OFFSET_CHECKSUM(pim_msg) = 0;
  checksum = pim_inet_checksum(pim_msg, pim_msg_size);
  *(uint16_t *) PIM_MSG_HDR_OFFSET_CHECKSUM(pim_msg) = checksum;
}

char *pim_msg_addr_encode_ipv4_ucast(char *buf,
				     int buf_size,
				     struct in_addr addr)
{
  const int ENCODED_IPV4_UCAST_SIZE = 6;

  if (buf_size < ENCODED_IPV4_UCAST_SIZE) {
    return 0;
  }

  buf[0] = PIM_MSG_ADDRESS_FAMILY_IPV4; /* addr family */
  buf[1] = '\0';    /* native encoding */
  *(struct in_addr *)(buf + 2) = addr;

  return buf + ENCODED_IPV4_UCAST_SIZE;
}

char *pim_msg_addr_encode_ipv4_group(char *buf,
				     int buf_size,
				     struct in_addr addr)
{
  const int ENCODED_IPV4_GROUP_SIZE = 8;

  if (buf_size < ENCODED_IPV4_GROUP_SIZE) {
    return 0;
  }

  buf[0] = PIM_MSG_ADDRESS_FAMILY_IPV4; /* addr family */
  buf[1] = '\0';    /* native encoding */
  buf[2] = '\0';    /* reserved */
  buf[3] = 32;      /* mask len */
  *(struct in_addr *)(buf + 4) = addr;

  return buf + ENCODED_IPV4_GROUP_SIZE;
}

char *pim_msg_addr_encode_ipv4_source(char *buf,
				      int buf_size,
				      struct in_addr addr)
{
  const int ENCODED_IPV4_SOURCE_SIZE = 8;

  if (buf_size < ENCODED_IPV4_SOURCE_SIZE) {
    return 0;
  }

  buf[0] = PIM_MSG_ADDRESS_FAMILY_IPV4; /* addr family */
  buf[1] = '\0';    /* native encoding */
  buf[2] = '\0';    /* reserved */
  buf[3] = 32;      /* mask len */
  *(struct in_addr *)(buf + 4) = addr;

  return buf + ENCODED_IPV4_SOURCE_SIZE;
}