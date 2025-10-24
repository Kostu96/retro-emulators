# 40xx Assembler
### syntax grammar
```
program = line* EOF

line = '=' expression '\n'
     | LABEL '=' expression '\n'
     | LABEL',' instruction '\n'
     | LABEL',' expression
     | instruction '\n'

instruction = NO_ARG_INST
            | ONE_ARG_INST expression
            | TWO_ARG_INST expression expression

expression = operand (('+' | '-') operand)*

operand = LABEL
        | NUMBER
        | REGISTER
        | REGISTER_PAIR
        | '*'
```
### lexical grammar
```
LETTER = ['A'-'Z']

DIGIT = ['0'-'9']

HEX_DIGIT = ['0'-'9','A'-'F']

LABEL = LETTER(LETTER | DIGIT)*

NUMBER = DIGIT+
       | HEX_DIGIT+'H'

REGISTER = 'R'HEX_DIGIT
         | 'R'(DIGIT | "10" | "11" | "12" | "13" | "14" | "15")

REGISTER_PAIR = 'P'['0'-'7']

NO_ARG_INST = "NOP" | "HLT" | "BBS" | "LCR" | "OR4" | "OR5" | "AN6" | "AN7"
            | "DB0" | "DB1" | "SB0" | "SB1" | "EIN" | "DIN" | "RPM" | "WRM"
            | "WMP" | "WRR" | "WPM" | "WR0" | "WR1" | "WR2" | "WR3" | "SBM"
            | "RDM" | "RDR" | "ADM" | "RD0" | "RD1" | "RD2" | "RD3" | "CLB"
            | "CLC" | "IAC" | "CMC" | "CMA" | "RAL" | "RAR" | "TCC" | "DAC"
            | "TCS" | "STC" | "DAA" | "KBP" | "DCL"

ONE_ARG_INST = "SRC" | "FIN" | "JIN" | "INC" | "ADD"
             | "SUB" | "LDM" | "XCH" | "BBL" | "LD"

TWO_ARG_INST = "JCN" | "FIM" | "JUN" | "JMS" | "ISZ"
```
