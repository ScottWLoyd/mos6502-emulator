
.dsb $1000,$0

LDA #$01    ; 0xA9
STA $0200   ; 0x8D
LDX #$02    ; 0xA2
STX $0201   ; 0x8E
LDY #$03    ; 0xA0
STY $0202   ; 0x8C
STA $04     ; 0x85
STX $05     ; 0x86
STY $06     ; 0x84

LDA $06     ; 0xA5
LDX $04     ; 0xA6
LDY $05     ; 0xA4

; Reset the registers
LDA #$00
LDX #$00
LDY #$00

LDA $0200   ; 0xAD A=1
LDX $0201   ; 0xAE X=2
LDY $0202   ; 0xAC Y=3

LDA #$0

LDA $02,X   ; 0xB5 A=1
LDX $01,Y   ; 0xB6 X=1
LDY $03,X   ; 0xB4 Y=1

STA $00,X   ; 0x95 0x0001=1
STX $01,Y   ; 0x96 0x0002=1
STY $02,X   ; 0x94 0x0003=1

LDA $0201,X ; 0xBD A=3
LDX $0201,Y ; 0xBE X=3
LDY $01FF,X ; 0xBC Y=3
LDA $0200,Y ; 0xB9 A=0

; Some setup for indirect ops
LDA #$BE
STA $0302
LDA #$AD
STA $0204
LDA #$00

LDA ($02,X) ; 0xA1 A=BE
LDA ($04),Y ; 0xB1 A=AD

STA $0205,X ; 0x9D $0208=AD
STA $0206,Y ; 0x99 $0209=AD

LDA #$DA
STA ($02,X) ; 0x81 $0302=DA
STA ($04),Y ; 0x91 $0204=DA

CMP $0209   ; 0xCD $DA-$AD=$2d, Z=0, S=0, C=1
CMP #$FF    ; 0xC9 $DA-$FF=$DB, Z=0, S=1, C=0
CMP ($02,X) ; 0xC1 $DA-DA=$0, Z=1, S=0, C=1
CMP ($04),Y ; 0xD1 $DA-DA=$0, Z=1, S=0, C=1
CMP $0205,X ; 0xDD $DA-$AD=$2d, Z=0, S=0, C=1
CMP $0205,Y ; 0xD9 $DA-$AD=$2d, Z=0, S=0, C=1
CMP $06     ; 0xC5 $DA-$03=$D7, Z=0, S=1, C=0
CMP $03,X   ; 0xD5 $DA-$03=$D7, Z=0, S=1, C=0

; Setup for arithmatic tests
LDY #$DF    ; Y = $DF
STY $0A     ; $0A=$DF
LDY #$AB    ; Y = $AB
STY $10     ; $10 = $AB
LDY #$44    ; Y = $44
STY $11     ; $11 = $44
LDY #$55    ; Y = $55
STY $4500   ; $4500 = $55

ADC #$44    ; 0x69 $DA+$44+(C=1)=1F, C=1,S=0,Z=0,O=0
ADC $06     ; 0x65 $1F+$03+(C=1)=$23, C=0,S=0,Z=0,O=0
ADC $03,X   ; 0x75 $23+$03+(C=0)=$26, C=0,S=0,Z=0,O=0
ADC $4500   ; 0x6D $26+$55+(C=0)=$7B, C=0,S=0,Z=0,O=0
ADC $44FD,X ; 0x7D $7B+$55+(C=0)=$D0, C=0,S=1,Z=0,O=1
ADC $44AB,Y ; 0x79 $D0+$55+(C=0)=$25, C=1,S=0,Z=0,O=0
ADC ($03,X) ; 0x61 $25+$01+(C=1)=$27, C=0,S=0,Z=0,O=0
ADC ($10),Y ; 0x71 $27+$55+(C=0)=$7C, C=0,S=0,Z=0,O=0

end:
    .dsb $fffa-end,$0
    .word $1000,$1000,$0