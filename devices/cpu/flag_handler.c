/*
  MSP430 Emulator
  Copyright (C) 2014, 2015 Rudolf Geosits (rgeosits@live.esu.edu)  
                                                                      
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
                                                                   
  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.          
                                                       
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses
*/

#include "decoder.h"
#include "flag_handler.h"

/**
 * @brief Test if the result of the asm instruction is zero
 * @param result_addr The address of the operation's result
 * @param bw_flag Byte or Word flag
 * @return true if zero, false otherwise
 */
uint8_t is_zero (uint16_t *result_addr, uint8_t bw_flag)
{
  if (bw_flag == WORD) {
    if (*result_addr == 0 ) {
      return 1;
    }

    return 0;
  }
  else if (bw_flag == BYTE) {
    if (*(int8_t *) result_addr == 0) {
      return 1;
    }

    return 0;
  }

  return false;
}

/**
 * @brief Test if the result of the asm instruction is negative
 * @param result_addr The address of the operation's result
 * @param bw_flag Byte or Word flag
 * @return true if zero, false otherwise
 */
uint8_t is_negative(int16_t *result_addr, uint8_t bw_flag)
{
  if (bw_flag == WORD) {
    if (*result_addr < 0) {
      return 1;
    }

    return 0;
  }
  else if (bw_flag == BYTE) {
    if (*((int8_t *) result_addr) < 0) {
      return 1;
    }

    return 0;
  }

  return false;
}

/**
 * @brief Test if the result of the asm instruction WILL carry
 * @param original_dst_value The original value at the destination
 * @param source_value The value at the source location
 * @param bw_flag Byte or Word flag
 * @return true if zero, false otherwise
 */
uint8_t is_carried(uint32_t original_dst_value, uint32_t source_value,
                   uint8_t bw_flag)
{
  if (bw_flag == WORD) {
    if ((65535 - (uint16_t)source_value) < (uint16_t)original_dst_value ||
        ((original_dst_value + source_value) >> 16) != 0) {
      return 1;
    }

    return 0;
  }
  else if (bw_flag == BYTE) {
    if ((255 - (uint8_t)source_value) < (uint8_t)original_dst_value ||
        ((original_dst_value + source_value) >> 8) != 0) {
      return 1;
    }

    return 0;
  }

  return false;
}

/**
 * @brief Test if the result of the asm instruction is overflowed
 * @param source_value The value at the source operand
 * @param destination_value The value at the destination operand
 * @param result A pointer to the result of the operation
 * @param bw_flag Byte or Word flag
 * @return true if zero, false otherwise
 */
uint8_t is_overflowed(uint16_t source_value, uint16_t destination_value,
                      uint16_t *result, uint8_t bw_flag)
{
  if (bw_flag == WORD) {
    if ( (source_value >> 15) == (destination_value >> 15) &&
         (*result >> 15) != (destination_value >> 15) ) {
      return 1;
    }

    return 0;
  }
  else if (bw_flag == BYTE) {
    uint8_t dst_prev_value = (uint8_t) destination_value;
    uint8_t src_value = (uint8_t) source_value;

    if ( (src_value >> 7) == (dst_prev_value >> 7) &&
         (*(uint8_t *)result >> 7) != (dst_prev_value >> 7)) {
      return 1;
    }

    return 0;
  }
  else {
    printf("Error, overflowed function: invalid bw_flag\n");
  }

  return false;
}
