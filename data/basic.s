
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

end:
    .dsb $fffa-end,$0
    .word $1000,$1000,$0