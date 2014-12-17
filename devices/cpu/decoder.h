void decode_formatI(uint16_t instruction);
void decode_formatII(uint16_t instruction);
void deocde_formatIII(uint16_t instruction);
void decode(uint16_t instruction);
uint16_t fetch();

#include "formatI.c"
#include "formatII.c"
#include "formatIII.c"
#include "decoder.c"
