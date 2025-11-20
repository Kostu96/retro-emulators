FIM P7 0x80 / address of RAM output port connected to 4003

LOOP, SRC P7    / fill shift register with '1's
      LDM 16-6  / for 6 outputs
      XCH R0
      LDM 0b0110
      WMP
      LDM 0b0100
      WMP
      ISZ R0 *-4

      SRC P7
      LDM 0b0011 / put '0' into shift register
      WMP
      LDM 0b0001
      WMP
      LDM 16-6
      XCH R0     / counter for L1
      LDM 0b0111 / move '0' across shift register
      WMP
      LDM 0b0101
      WMP
      ISZ R0 *-4
      
      JUN LOOP

LEDTBL, 0xFC / 0
        0x60 / 1
        0xDA / 2
        0xF2 / 3
        0x66 / 4
        0xB6 / 5
        0xBE / 6
        0xE0 / 7
        0xFE / 8
        0xF6 / 9
LEDMIN, 0x02 / -
