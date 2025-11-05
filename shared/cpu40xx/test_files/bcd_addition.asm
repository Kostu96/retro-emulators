; 16-digit BCD addition subroutine
  JUN START
  NOP
  BBS
START
  FIM P0, 0   ; RAM register of A
  FIM P1, $20 ; RAM register of B
  JMS BCDADD
DONE
  JUN DONE
  
BCDADD
  LDM 0
  XCH R6
  CLC
ADD1
  SRC P2
  RDM
  SRC P0
  ADM
  DAA
  WRM
  INC R1
  INC R5
  ISZ R6, ADD1
  BBL
