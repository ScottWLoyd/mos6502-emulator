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