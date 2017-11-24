//
// Load immediate / Store absolute
// 

// LDA #$01  { A9 }
u8 Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);   
Assert(Chip.A == 1);
// STA $0200
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0200] == 1);
// LDX #$02
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.X == 2);
// STX $0201
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0201] == 2);
// LDY #$03
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Y == 3);

// STY $0202
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0202] == 3);

//
// Store zero page / Load zero page
//

// STA $04
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0004] == 1);

// STX $05
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0005] == 2);

// STY $06
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0006] == 3);

// LDA $06
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 3);

// LDX $04
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.X == 1);

// LDY $05
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Y == 2);

//
// Reset the registers
//
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0);
Assert(Chip.X == 0);
Assert(Chip.Y == 0);

//
// Load absolute
//

// LDA $0200
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);      
Assert(Chip.A == 1);

// LDX $0201
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.X == 2);

// LDY $0202
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Y == 3);

// Just resetting here...
// LDA #$0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0);

//
// Load zero page X/Y
//

// LDA $02,X
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 1);

// LDX $01,Y
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.X == 1);

// LDY $03,X
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Y == 1);

//
// Store zero page X/Y
//

// STA $00,X
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0001] == 1);
// STX $00,Y
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0002] == 1);
// STY $00,X
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0003] == 1);

//
// Load absolute X/Y
//

// LDA $0201,X
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 3);
// LDX $0201,Y
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.X == 3);
// LDA $01FF,X
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Y == 3);
// LDA $0200,Y
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0);

//
// Setup for indirect ops
//
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xBE);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0302] == 0xBE);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xAD);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0204] == 0xAD);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x0);

//
// Load indirect/indexed X/Y
//

// LDA ($02,X)
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xBE);

// LDA ($04),Y
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xAD);

//
// Store absolute, indexed
// 

// STA $0205,X
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0208] == 0xAD);

// STA $0206,Y
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0209] == 0xAD);

// Setup
// LDA #$DA
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDA);

//
// Store indexed, indirect
//

// STA ($02,X)
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0302] == 0xDA);

// STA ($04),Y
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0204] == 0xDA);

//
// Compare
// 

// CMP $0209
//$DA-$AD=$2d, Z=0, S=0, C=1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDA);
Assert(Chip.Ram[0x0209] == 0xAD);
Assert(Chip.P.Z == 0);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);

// CMP #$FF
//$DA-$FF=$DB, Z=0, S=1, C=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDA);
Assert(Chip.P.Z == 0);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 0);

// CMP ($02,X)
//$DA-$DA=$0, Z=1, S=0, C=1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDA);
Assert(Chip.X == 0x03);
Assert(Chip.P.Z == 1);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);

// CMP ($02),Y
//$DA-$DA=$0, Z=1, S=0, C=1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDA);
Assert(Chip.Y == 0x03);
Assert(Chip.P.Z == 1);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);

// CMP $0205,X 
// $DD-$AD=$2d, Z=0, S=0, C=1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDA);
Assert(Chip.X == 0x03);
Assert(Chip.Ram[0x0208] == 0xAD);
Assert(Chip.P.Z == 0);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);

// CMP $0205,Y 
// $DD-$AD=$2d, Z=0, S=0, C=1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDA);
Assert(Chip.Y == 0x03);
Assert(Chip.Ram[0x0208] == 0xAD);
Assert(Chip.P.Z == 0);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);

// CMP $06     
// $DA-$03=$D7, Z=0, S=0, C=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDA);
Assert(Chip.Ram[0x06] == 0x03);
Assert(Chip.P.Z == 0);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 1);

// CMP $03,X
// $DA-$03=$D7, Z=0, S=0, C=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDA);
Assert(Chip.Ram[0x06] == 0x03);
Assert(Chip.X == 0x03);
Assert(Chip.P.Z == 0);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 1);

//
// Setup for aritmatic tests
//

// LDY #$DF    ; Y = $DF
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
// STY $0A     ; $0A=$DF
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Y == 0xDF);
Assert(Chip.Ram[0x0A] == 0xDF);

// LDY #$AB    ; Y = $AB
// STY $10     ; $10 = $AB
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
// LDY #$44    ; Y = $44
// STY $11     ; $11 = $44
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
// LDY #$55    ; Y = $55
// STY $4500   ; $4500 = $55
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Y == 0x55);
Assert(Chip.Ram[0x4500] == 0x55);

//
// ADC tests
//

// ADC #$44    
// 0x69 $DA+$44=$1E, C=1 -> $1F
Assert(Chip.A == 0xDA);
Assert(Chip.P.C == 1);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x1F);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// ADC $06     
// $1F+$03+(C=1)=23, C=0,S=0,Z=0,O=0
Assert(Chip.A == 0x1F);
Assert(Chip.P.C == 1);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x23);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// ADC $03,X   
// $23+$03+(C=0)=$26, C=0,S=0,Z=0,O=0
Assert(Chip.A == 0x23);
Assert(Chip.P.C == 0);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x26);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// ADC $4500
// $26+$55+(C=0)=$7B, C=0,S=0,Z=0,O=0
Assert(Chip.A == 0x26);
Assert(Chip.P.C == 0);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x7B);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// ADC $44FD,X 
// $7B+$55+(C=0)=D0, C=0,S=1,Z=0,O=1
Assert(Chip.X == 0x03);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xD0);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 1);

// ADC $44AB,Y 
// $D0+$55+(C=0)=25, C=1,S=0,Z=0,O=0
Assert(Chip.Y == 0x55);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x25);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// ADC ($03,X) 
// $25+$01+(C=1)=$27, C=0,S=0,Z=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x27);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// ADC ($B1),Y 
// $27+$55+(C=0)=$7C, C=0,S=0,Z=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x7C);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// SBC ($10),Y
// $7C-$55+(C=0)=$26, C=0,S=0,Z=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x26);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// SBC ($03,X)
// $26-$01+(C=1)-1=$25, C=1,S=0,Z=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x25);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// SBC $44AB,Y 
// $25-$55+(C=1)-1=$D0, C=0,S=1,Z=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xD0);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// SBC $44FD,X ;  
// $D0-$55+(C=0)-1=$7A, C=1,S=0,Z=0,O=1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x7A);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 1);

// SBC $4500   
// $7A-$55+(C=1)-1=$25, C=1,S=0,Z=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x25);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// SBC $03,X
// $25-$03+(C=1)-1=$22, C=1,S=0,Z=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x22);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// SBC $06
// $22-$03+(C=1)-1=$1f, C=1,S=0,Z=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x1F);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 1);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// SBC #$44    
// $1f-$44+(C=1)-1=$DB, C=0,S=1,Z=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDB);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);
Assert(Chip.P.O == 0);

// AND #$ff    ; $DB&$FF=$DB, S=1,Z=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xDB);
Assert(Chip.P.S == 1);
Assert(Chip.P.Z == 0);

// AND $10     ; $DB&$AB=$8B, S=1,Z=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x8B);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 0);

// AND $0E     ; $8B&$FF=$8B, S=1,Z=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x8B);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 0);

// AND $0208   ; $8B&$AD=$89, S=1,Z=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x89);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 0);

// AND $0205,X ; $89&$AD=$89, S=1,Z=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x89);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 0);

// AND $01AD,Y ; $89&$03=$01, S=0,Z=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x01);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);

// AND ($03,X) ; $01&$01=$01, S=0,Z=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x01);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);

// AND ($10),Y ; $01&$01=$01, S=0,Z=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x01);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);

// ASL         ; A=$02
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x02);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);

// ASL $01     ; $01=$02
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x01] == 0x02);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);

// ASL $01,X   ; $04=$02
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x04] == 0x02);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);

// ASL $0201   ; $0201=$04
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0201] == 0x04);
Assert(Chip.P.S == 0);
Assert(Chip.P.C == 0);
Assert(Chip.P.Z == 0);

// ASL $0201,X ; $0204=$B4, C=1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.Ram[0x0204] == 0xB4);
Assert(Chip.P.S == 1);
Assert(Chip.P.C == 1);
Assert(Chip.P.Z == 0);

// BIT $0A     ; Z=0,S=1,O=1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.S == 1);
Assert(Chip.P.O == 1);
Assert(Chip.P.Z == 0);

// BIT $0201   ; Z=1,N=0,O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.S == 0);
Assert(Chip.P.O == 0);
Assert(Chip.P.Z == 1);

//
// Branching Tests
//

// SEC
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.C == 1);
// LDA #$02
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 2);
// decrement: SBC #$01
u16 DecrementAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 1);
Assert(Chip.P.S == 0);
// BPL decrement 
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == DecrementAddr);
// decrement: SBC #$01
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0);
Assert(Chip.P.S == 0);
// BPL decrement 
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == DecrementAddr);
// decrement: SBC #$01
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xFF);
Assert(Chip.P.S == 1);
// BPL decrement 
u16 TempAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == TempAddr + 2);
// compare: CMP #$FF
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.Z == 1);
// BEQ decrement
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == DecrementAddr);
// decrement: SBC #$01
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xFE);
Assert(Chip.P.S == 1);
// BPL decrement 
TempAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == TempAddr + 2);
// compare: CMP #$FF
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.Z == 0);
// BEQ decrement
TempAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == TempAddr + 2);
// BMI increment
TempAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == TempAddr + 2);
// increment: CLC
u16 IncAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.C == 0);
// ADC #$01
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xFF);
// CMP #$00
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.Z == 0);
// BNE increment
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == IncAddr);
// increment: CLC
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.C == 0);
// ADC #$01
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0);
// CMP #$00
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.Z == 1);
// BNE increment
TempAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == TempAddr + 2);

// LDA #$7F ; A=$7F=(127)
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x7F);
// tp1: CLC
u16 tp1_Addr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.C == 0);
// ADC #$01 ; A=$80=(128), O=1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x80);
Assert(Chip.P.O == 1);
// BVS tp1
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == tp1_Addr);
// tp1: CLC
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.C == 0);
// ADC #$01 ; A=$81=(129), O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x81);
Assert(Chip.P.O == 0);
// BVS tp1
TempAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == TempAddr + 2);
// tp2: CLC
u16 tp2_Addr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.C == 0);
// ADC #$FF ; A=(-128), O=0
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x80);
Assert(Chip.P.O == 0);
// BVC tp2
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == tp2_Addr);
// tp2: CLC
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.C == 0);
// ADC #$FF ;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x7F);
Assert(Chip.P.O == 1);
// BVC tp2
TempAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == TempAddr + 2);
// CLV
Assert(Chip.P.O == 1);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.O == 0);

// CLC
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.C == 0);
// LDA #$FE
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xFE);
// tp3: ADC #$01
u16 Tp3Addr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xFF);
Assert(Chip.P.C == 0);
// BCC tp3
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == Tp3Addr);
// tp3: ADC #$01
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x00);
Assert(Chip.P.C == 1);
// BCC tp3
TempAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == TempAddr + 2);
// ADC #$01
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x02);
Assert(Chip.P.C == 0);
// tp4: SBC #$01 
u16 Tp4Addr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0x00);
Assert(Chip.P.C == 1);
// BCS tp4
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == Tp4Addr);
// tp4: SBC #$01 
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.A == 0xFF);
Assert(Chip.P.C == 0);
// BCS tp4
TempAddr = Chip.PC;
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.PC == TempAddr + 2);

// SEI     ; set interrupt
Assert(Chip.P.I == 0);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.I == 1);
// CLI     ; clear interrupt
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.I == 0);
// SED     ; set decimal
Assert(Chip.P.D == 0);
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.D == 1);
// CLD     ; clear decimal
Op = Chip.Ram[Chip.PC++];
ExecInstruction(&Chip, Op);
Assert(Chip.P.D == 0);