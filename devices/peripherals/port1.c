/* Cheat Sheet:
 *  IN:
 *  OUT: 0 = INPUT, 1 = OUTPUT
 *  DIR: 0 = INPUT, 1 = OUTPUT
 *  
 */

void handle_port1 ()
{
  /* Handler P1.0 */
  if (*P1DIR & 0x01) {      /* Check Direction */
    if (*P1OUT & 0x01) {    /* Check OUTPUT */
      printf("LED ON\n");      
    }
    else {
      printf("LED OFF\n");
    }
  }
  else {                    /* Check INPUT */
    
  }

  /* Handler P1.1 */
  
}
