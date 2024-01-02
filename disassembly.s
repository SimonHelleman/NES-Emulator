c004    SEI 
c005    CLD 
c006    LDX #255
c008    TXS 
c009    LDA $2002
c00c    BPL $fb
c00e    LDA $2002
c011    BPL $fb
c013    LDA #0
c015    STA $2000
c018    STA $2001
c01b    STA $2005
c01e    STA $2005
c021    LDA $2002
c024    LDX #32
c026    STX $2006
c029    LDX #0
c02b    STX $2006
c02e    LDX #0
c030    LDY #15
c032    LDA #0
c034    STA $2007
c037    DEX 
c038    BNE $fa
c03a    DEY 
c03b    BNE $f7
c03d    LDA #63
c03f    STA $2006
c042    LDA #0
c044    STA $2006
c047    LDX #0
c049    LDA $ff78, x
c04c    STA $2007
c04f    INX 
c050    CPX #32
c052    BNE $f5
c054    LDA #192
c056    STA $4017
c059    LDA #0
c05b    STA $4015
c05e    LDA #120
c060    STA $d0
c062    LDA #251
c064    STA $d1
c066    LDA #127
c068    STA $d3
c06a    LDY #0
c06c    STY $2006
c06f    STY $2006
c072    LDA #0
c074    STA $d7
c076    LDA #7
c078    STA $d0
c07a    LDA #195
c07c    STA $d1
c07e    JSR $c2a7
c081    JSR $c28d
c084    LDX #18
c086    JSR $c261
c089    LDA $d5
c08b    LSR 
c08c    LSR 
c08d    LSR 
c08e    BCS $1c
c090    LSR 
c091    BCS $0c
c093    LSR 
c094    BCS $27
c096    LSR 
c097    BCS $03
c099    JMP $c081
c261    LDA $d7
c263    CLC 
c264    ADC #4
c266    TAY 
c267    LDA #132
c269    STA $2000
c26c    LDA #32
c26e    STA $2006
c271    LDA #2
c273    STA $2006
c276    LDA #32
c278    DEY 
c279    INY 
c27a    BNE $02
c27c    LDA #42
c27e    STA $2007
c281    DEY 
c282    DEX 
c283    BNE $f1
c285    LDA #128
c287    STA $2000
c28a    JMP $c294
c28d    LDA $d2
c28f    CMP $d2
c291    BEQ $fc
c293    RTS 
c294    LDA #0
c296    STA $2005
c299    STA $2005
c29c    LDA #0
c29e    STA $2006
c2a1    LDA #0
c2a3    STA $2006
c2a6    RTS 
c2a7    LDA #0
c2a9    STA $2000
c2ac    STA $2001
c2af    JSR $c2ed
c2b2    LDA #32
c2b4    STA $2006
c2b7    LDY #0
c2b9    STY $2006
c2bc    LDX #32
c2be    LDA ($d0), y
c2c0    BEQ $20
c2c2    CMP #255
c2c4    BEQ $0d
c2c6    STA $2007
c2c9    INY 
c2ca    BNE $02
c2cc    INC $d1
c2ce    DEX 
c2cf    BNE $ed
c2d3    INY 
c2d4    BNE $02
c2d8    LDA #32
c2da    STA $2007
c2dd    DEX 
c2de    BNE $f8
c2e0    BEQ $da
c2e2    LDA #128
c2e4    STA $2000
c2e7    LDA #14
c2e9    STA $2001
c2ec    RTS 
c2ed    LDA #32
c2ef    STA $2006
c2f2    LDA #0
c2f4    STA $2006
c2f7    LDX #30
c2f9    LDA #32
c2fb    LDY #32
c2fd    STA $2007
c300    DEY 
c301    BNE $fa
c303    DEX 
c304    BNE $f5
c306    RTS 
c5af    PHA 
c5b0    TXA 
c5b1    PHA 
c5b2    LDA $2002
c5b5    LDA #32
c5b7    STA $2006
c5ba    LDA #64
c5bc    STA $2006
c5bf    INC $d2
c5c1    LDA #0
c5c3    STA $2005
c5c6    STA $2005
c5c9    LDA #0
c5cb    STA $2006
c5ce    LDA #0
c5d0    STA $2006
c5d3    LDX #9
c5d5    STX $4016
c5d8    DEX 
c5d9    STX $4016
c5dc    LDA $4016
c5df    LSR 
c5e0    ROL $d4
c5e2    DEX 
c5e3    BNE $f7
c5e5    LDA $d4
c5e7    TAX 
c5e8    EOR $d6
c5ea    AND $d4
c5ec    STA $d5
c5ee    STX $d6
c5f0    PLA 
c5f1    TAX 
c5f2    PLA 
c5f3    RTI 
