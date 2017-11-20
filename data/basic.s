
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


end:
    .dsb $fffa-end,$0
    .word $1000,$1000,$0