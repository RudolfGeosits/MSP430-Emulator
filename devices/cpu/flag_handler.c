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
}

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
}
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
}
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
}
