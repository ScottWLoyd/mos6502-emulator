
.dsb $1000,$0

LDA #$01
STA $0200

LDX #$02
STX $0201

LDY #$03
STY $0202

end:
    .dsb $fffa-end,$0
    .word $1000,$1000,$0