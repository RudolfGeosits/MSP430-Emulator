/* Cheat Sheet:
 *  IN:  0 = digital 0, 1 = digital 1
 *  OUT: 0 = INPUT, 1 = OUTPUT
 *  DIR: 0 = INPUT, 1 = OUTPUT
 *  REN: 0 = Pull Down, 1 = Pull Up
 *  IFG: 0 = No int pend, 1 = Interrupt Pending
 *   IE: 0 = int not enabled, 1 = int enabled
 */

void handle_port1 ()
{
  /* Handler P1.0 */
  if (*P1DIR & 0x01) {      /* Check Direction */
    P1DIR_0 = true;         /* Set P1DIR.0 flag (for gui) ON */
    if (*P1OUT & 0x01) {    /* Check OUTPUT */
      P1OUT_0 = true;       /* Set P1OUT.0 flag (for gui) ON */
    }
    else {
      P1OUT_0 = false;
    }
  }
  else {                    /* Check INPUT */
    P1DIR_0 = false;
  }

  if (*P1IE & 0x01) {       /* Check if Interrupt Enabled for pin */
    P1IE_0 = true;

    if (*P1IFG & 0x01) {    /* Check For Interrupt Pending */            
      P1IFG_0 = true;       /* Set P1IFG.0 flag indicating INT */
    }
    else {
      P1IFG_0 = false;
    }
  }
  else {
    P1IE_0 = false;
  }

  /* Handler P1.1 */
  if (*P1DIR & 0x02) {
    P1DIR_1 = true;
    if (*P1OUT & 0x02) {
      P1OUT_1 = true;
    }
    else {
      P1OUT_1 = false;
    }
  }
  else {
    P1DIR_1 = false;
  }

  if (*P1IE & 0x02) {
    P1IE_1 = true;

    if (*P1IFG & 0x02) {
      P1IFG_1 = true;
    }
    else {
      P1IFG_1 = false;
    }
  }
  else {
    P1IE_1 = false;
  }
  
  /* Handler P1.2 */
  if (*P1DIR & 0x04) {
    P1DIR_2 = true;
    if (*P1OUT & 0x04) {
      P1OUT_2 = true;
    }
    else {
      P1OUT_2 = false;
    }
  }
  else {
    P1DIR_2 = false;
  }

  if (*P1IE & 0x04) {
    P1IE_2 = true;

    if (*P1IFG & 0x04) {
      P1IFG_2 = true;
    }
    else {
      P1IFG_2 = false;
    }
  }
  else {
    P1IE_2 = false;
  }

  /* Handler P1.3 */
  if (*P1DIR & 0x08) {
    P1DIR_3 = true;
    if (*P1OUT & 0x08) {
      P1OUT_3 = true;
    }
    else {
      P1OUT_3 = false;
    }
  }
  else {
    P1DIR_3 = false;
  }

  if (*P1IE & 0x08) {
    P1IE_3 = true;

    if (*P1IFG & 0x08) {
      P1IFG_3 = true;
    }
    else {
      P1IFG_3 = false;
    }
  }
  else {
    P1IE_3 = false;
  }

  /* Handler P1.4 */
  if (*P1DIR & 0x10) {
    P1DIR_4 = true;
    if (*P1OUT & 0x10) {
      P1OUT_4 = true;
    }
    else {
      P1OUT_4 = false;
    }
  }
  else {
    P1DIR_4 = false;
  }

  if (*P1IE & 0x10) {
    P1IE_4 = true;

    if (*P1IFG & 0x10) {
      P1IFG_4 = true;
    }
    else {
      P1IFG_4 = false;
    }
  }
  else {
    P1IE_4 = false;
  }


  /* Handler P1.5 */
  if (*P1DIR & 0x20) {
    P1DIR_5 = true;
    if (*P1OUT & 0x20) {
      P1OUT_5 = true;
    }
    else {
      P1OUT_5 = false;
    }
  }
  else {
    P1DIR_5 = false;
  }

  if (*P1IE & 0x20) {
    P1IE_5 = true;

    if (*P1IFG & 0x20) {
      P1IFG_5 = true;
    }
    else {
      P1IFG_5 = false;
    }
  }
  else {
    P1IE_5 = false;
  }

  /* Handler P1.6 */
  if (*P1DIR & 0x40) {
    P1DIR_6 = true;
    if (*P1OUT & 0x40) {
      P1OUT_6 = true;
    }
    else {
      P1OUT_6 = false;
    }
  }
  else {
    P1DIR_6 = false;
  }

  if (*P1IE & 0x40) {
    P1IE_6 = true;

    if (*P1IFG & 0x40) {
      P1IFG_6 = true;
    }
    else {
      P1IFG_6 = false;
    }
  }
  else {
    P1IE_6 = false;
  }

  /* Handler P1.7 */
  if (*P1DIR & 0x80) {
    P1DIR_7 = true;
    if (*P1OUT & 0x80) {
      P1OUT_7 = true;
    }
    else {
      P1OUT_7 = false;
    }
  }
  else {
    P1DIR_7 = false;
  }

  if (*P1IE & 0x80) {
    P1IE_7 = true;

    if (*P1IFG & 0x80) {
      P1IFG_7 = true;
    }
    else {
      P1IFG_7 = false;
    }
  }
  else {
    P1IE_7 = false;
  }

}
