
#define DEBUG

#define Unimplemented (*(int*)0=1)

#ifdef DEBUG
#define DPRINT(F, ...) fprintf(stderr, (F), __VA_ARGS__)
#else
#define DPRINT
#endif

#define KB(X) ((X)*1024)

// TODO(scott): Implement and test decimal mode
// Reference simulator: https://skilldrick.github.io/easy6502/

#pragma pack(push, 1)
union status_flags_type
{
   struct
   {
      u8 C      : 1;
      u8 Z      : 1;
      u8 I      : 1;
      u8 D      : 1;
      u8 B      : 1;
      u8 Spare  : 1;
      u8 O      : 1;
      u8 S      : 1;
   };
   u8 Value;
};
#pragma pack(pop)

struct mos6502
{
   u8 A;    // Accumulator
   u8 X;    
   u8 Y;

   status_flags_type P;    // Status register
   u8 S;    // Stack pointer
   u16 PC;  // Program counter

   u8 Ram[KB(64)];

   u32 CyclesRemaining;
};

enum interrupt_type
{
   Interrupt_Nmi,
   Interrupt_Reset,
   Interrupt_Irq,
   Interrupt_Brk,
};

u16 InterruptVectors[4] = 
{
   0xFFFA, // NMI
   0xFFFC, // Reset
   0xFFFE, // IRQ
   0xFFFE, // BRK
};

//
// User should supply the Read() and Write() functions.
// Allows the usage of various memory mappers.
//
u16 Read(mos6502* Chip, u16 Addr);
void Write(mos6502* Chip, u16 Addr, u8 Value);

inline u16
ReadAddress(mos6502* Chip, u16 Addr)
{
   u16 Low = (u16)Read(Chip, Addr);
   u16 High = (u16)Read(Chip, Addr + 1);
   u16 Result = (High << 8) | Low;
   return Result;
}

inline void
Push(mos6502* Chip, u8 V)
{
   Chip->Ram[0x100 + (Chip->S--)] = V;
}

inline u8
Pop(mos6502* Chip)
{
   return Chip->Ram[0x100 + (++Chip->S)];
}

inline void
HandleInterrupt(mos6502* Chip, interrupt_type Type)
{
   if (Type != Interrupt_Reset)
   {
      Push(Chip, Chip->PC >> 8); Push(Chip, Chip->PC & 0xFF);
      Chip->P.B = (Type == Interrupt_Brk);
      Push(Chip, Chip->P.Value);
   }
   else
   {
      Chip->S -= 3;
   }
   Chip->P.I = 1;
   u16 Addr = ReadAddress(Chip, InterruptVectors[Type]);
   Chip->PC = Addr;
}


internal void
InitChip(mos6502* Chip, u8* Rom, u32 RomLen, u16 LoadAddress)
{
   Chip->A = Chip-> X = Chip->Y = 0;
   memcpy(Chip->Ram + LoadAddress, Rom, RomLen);

   HandleInterrupt(Chip, Interrupt_Reset);
}


#define SetSZ(v) Chip->P.S=(v>>7)&1; Chip->P.Z=(v==0)
#define LD(r,v) *r=v; SetSZ(v)


#define NextByte Read(Chip, Chip->PC++)
#define rd16(a,b) (Read(Chip, a)|(Read(Chip, b)<<8))

#define AM_imm u16 r=NextByte
#define AM_a   u16 r=(NextByte|(NextByte)<<8)
#define AM_zp  u16 r=NextByte
#define AM_r   u16 r=(((i8)NextByte)+(Chip->PC-1)
#define AM_ax  u16 r=Chip->X+(NextByte|(NextByte)<<8)
#define AM_ay  u16 r=Chip->Y+(NextByte|(NextByte)<<8)
#define AM_zpx u16 r=(Chip->X+NextByte)&0xff
#define AM_zpy u16 r=(Chip->Y+NextByte)&0xff
#define AM_ix  u16 b=(Chip->X+NextByte)&0xff; u16 r=rd16(b, b+1)
#define AM_iy	u16 i=NextByte; u16 r=rd16(i, (i+1)%0x0100) + Chip->Y;


inline void
Add(mos6502* Chip, u8 V)
{
   u16 Result = Chip->A + Chip->P.C + V;
   // TODO(scott): Implement decimal here
   // if(Chip->P.D)
   //{
   //
   //}
   //else
   {
      //Chip->P.V = ((Chip->A ^ Result) & (V ^ Result) & 0x80) == 0x80 ? 1 : 0;
      Chip->P.O = (~(Chip->A^V) & (Chip->A^Result) & 0x80) >> 7;
      Chip->P.C = (Result > 0xFF);
      SetSZ(Result);
   }
   Chip->A = (Result & 0xFF);
}

inline void
And(mos6502* Chip, u8 V)
{
   Chip->A = Chip->A & V;
   SetSZ(Chip->A);
}

inline void
Asl(mos6502* Chip, u16 Addr)
{
   u8 V = Read(Chip, Addr);
   Chip->P.C = (V & 0x80) >> 7;
   V = (V << 1);
   SetSZ(V);
   Write(Chip, Addr, V);
}

inline void
Cmp(mos6502* Chip, u8 V)
{   
   Chip->P.C = (Chip->A >= V);
   SetSZ(Chip->A - V);
}

inline void
Branch(mos6502* Chip, i8 Rel)
{
   Chip->PC += Rel;
}

inline void
ExecInstruction(mos6502* Chip, u8 Opcode)
{
   switch(Opcode)
   {
      case 0x00: {
         printf("BRK\n");
         Chip->P.B = 1;
         HandleInterrupt(Chip, Interrupt_Brk);
      } break;
      case 0x06: {
         AM_zp;
         Asl(Chip, r);
      } break; // ASL zp
      case 0x10: {
         AM_zp;
         if (Chip->P.S == 0) 
         {
            Branch(Chip, *(i8*)&r);
         }
      } break; // BPL
      case 0x0A: {
         Chip->P.C = (Chip->A & 0x8) >> 7;
         Chip->A = (Chip->A << 1);
         Chip->P.S = (Chip->A & 0x8) >> 7;
         Chip->P.Z = (Chip->A == 0);
      } break; // ASL A
      case 0x0E: {
         AM_a;
         Asl(Chip, r);
      } break; // ASL abs
      case 0x16: {
         AM_zpx;
         Asl(Chip, r);
      } break; // ASL zpx
      case 0x18: {
         Chip->P.C = 0;
      } break; // CLC
      case 0x1E: {
         AM_ax;
         Asl(Chip, r);
      } break; // ASL ax
      case 0x21: {
         AM_ix;
         And(Chip, Read(Chip, r));
      } break; // AND ix
      case 0x24: {
         AM_zp;
         u8 V = Read(Chip, r);
         Chip->P.Z = ((Chip->A & V) == 0);
         Chip->P.S = (V & 0x80) >> 7;
         Chip->P.O = (V & 0x40) >> 6;
      } break; // BIT zp
      case 0x25: {
         AM_zp;
         And(Chip, Read(Chip, r));
      } break; // AND zp
      case 0x29: {
         AM_imm;
         And(Chip, r);
      } break; // AND imm
      case 0x2C: {
         AM_a;
         u8 V = Read(Chip, r);
         Chip->P.Z = ((Chip->A & V) == 0);
         Chip->P.S = (V & 0x80) >> 7;
         Chip->P.O = (V & 0x40) >> 6;
      } break; // BIT abs
      case 0x2D: {
         AM_a;
         And(Chip, Read(Chip, r));
      } break; // AND abs
      case 0x30: {
         AM_zp;
         if (Chip->P.S) 
         {
            Branch(Chip, *(i8*)&r);
         }
      } break; // BMI
      case 0x31: {
         AM_iy;
         And(Chip, Read(Chip, r));
      } break; // AND iy
      case 0x35: {
         AM_zpx;
         And(Chip, Read(Chip, r));
      } break; // AND zpx
      case 0x38: {
         Chip->P.C = 1;
      } break; // SEC
      case 0x3D: {
         AM_ax;
         And(Chip, Read(Chip, r));
      } break; // AND ax
      case 0x39: {
         AM_ay;
         And(Chip, Read(Chip, r));
      } break; // AND ay
      case 0x4C: {
         AM_a;
         Chip->PC = r;
      } break;
      case 0x50: {
         AM_zp;
         if (!Chip->P.O)
         {
            Branch(Chip, *(i8*)&r);
         }
      } break; // BVS
      case 0x58: {
         Chip->P.I = 0;
      } break; // CLI
      case 0x61: {
         AM_ix;
         Add(Chip, Read(Chip, r));
      } break; // ADC ix
      case 0x65: {
         AM_zp;
         Add(Chip, Read(Chip, r));
      } break; // ADC zp
      case 0x69: {
         AM_imm;
         Add(Chip, r);
      } break; // ADC imm
      case 0x6C: {
         AM_a;
         u16 Second = r + 1;
         if ((Second / 0x100) != (r / 0x100))
         {
            Second -= 0x100;
         }
         u16 Hi = Read(Chip, Second);
         u16 Lo = Read(Chip, r);
         Chip->PC = (Hi << 8) | Lo;
      } break;
      case 0x6D: {
         AM_a;
         Add(Chip, Read(Chip, r));
      } break; // ADC abs
      case 0x70: {
         AM_zp;
         if (Chip->P.O)
         {
            Branch(Chip, *(i8*)&r);
         }
      } break; // BVS
      case 0x71: {
         AM_iy;
         Add(Chip, Read(Chip, r));
      } break; // ADC iy
      case 0x75: {
         AM_zpx;
         Add(Chip, Read(Chip, r));
      } break; // ADC zpx
      case 0x78: {
         Chip->P.I = 1;
      } break; // SEI
      case 0x79: {
         AM_ay;
         Add(Chip, Read(Chip, r));
      } break; // ADC zpy
      case 0x7D: {
         AM_ax;
         Add(Chip, Read(Chip, r));
      } break; // ADC zpx
      case 0x81: {
         AM_ix;
         Write(Chip, r, Chip->A);
      } break; // STA ix
      case 0x84: { 
         AM_zp; 
         Write(Chip, r, Chip->Y);
      } break; // STY zp
      case 0x85: { 
         AM_zp; 
         Write(Chip, r, Chip->A);
      } break; // STA zp
      case 0x86: { 
         AM_zp; 
         Write(Chip, r, Chip->X);
      } break; // STX zp
      case 0x8C: { 
         AM_a; 
         Write(Chip, r, Chip->Y);     
      } break; // STY addr
      case 0x8D: { 
         AM_a; 
         Write(Chip, r, Chip->A);     
      } break; // STA addr
      case 0x8E: { 
         AM_a; 
         Write(Chip, r, Chip->X);
      } break; // STX addr
      case 0x90: { 
         AM_zp; 
         if(!Chip->P.C)
         {
            Branch(Chip, *(i8*)&r);
         }
      } break; // BCC
      case 0x91: { 
         AM_iy;
         Write(Chip, r, Chip->A);
      } break; // STA iy
      case 0x94: { 
         AM_zpx; 
         Write(Chip, r, Chip->Y);
      } break; // STY zpx
      case 0x95: { 
         AM_zpx; 
         Write(Chip, r, Chip->A);
      } break; // STA zpx
      case 0x96: { 
         AM_zpy; 
         Write(Chip, r, Chip->X);
      } break; // STX zpy
      case 0x99: { 
         AM_ax; 
         Write(Chip, r, Chip->A);    
      } break; // STA ay
      case 0x9D: { 
         AM_ax; 
         Write(Chip, r, Chip->A);    
      } break; // STA ax
      case 0xA0: { 
         AM_imm; 
         LD(&Chip->Y, r);
      } break; // LDY #
      case 0xA1: { 
         AM_ix; 
         LD(&Chip->A,Read(Chip, r));
      } break; // LDA inx
      case 0xA2: { 
         AM_imm; 
         LD(&Chip->X,r);
      } break; // LDX #
      case 0xA4: { 
         AM_zp; 
         LD(&Chip->Y,Read(Chip, r)); 
      } break; // LDY zp
      case 0xA5: { 
         AM_zp; 
         LD(&Chip->A,Read(Chip, r)); 
      } break; // LDA zp
      case 0xA6: { 
         AM_zp; 
         LD(&Chip->X,Read(Chip, r)); 
      } break; // LDX zp
      case 0xA9: { 
         AM_imm; 
         LD(&Chip->A,r);            
      } break; // LDA #
      case 0xAC: { 
         AM_a; 
         LD(&Chip->Y,Read(Chip, r));
      } break; // LDY a
      case 0xAD: { 
         AM_a; 
         LD(&Chip->A,Read(Chip, r)); 
      } break; // LDA a
      case 0xAE: { 
         AM_a; 
         LD(&Chip->X,Read(Chip, r));
      } break; // LDX a
      case 0xB0: { 
         AM_zp; 
         if(Chip->P.C)
         {
            Branch(Chip, *(i8*)&r);
         }
      } break; // BCS
      case 0xB1: { 
         AM_iy; 
         LD(&Chip->A,Read(Chip, r));
      } break; // LD(&LDA i,
      case 0xB4: {
         AM_zpx; 
         LD(&Chip->Y,Read(Chip, r));
      } break; // LDY zpx
      case 0xB5: { 
         AM_zpx; 
         LD(&Chip->A,Read(Chip, r));
      } break; // LDA zpx
      case 0xB6: { 
         AM_zpy;
         LD(&Chip->X,Read(Chip, r));
      } break; // LDX zpy
      case 0xB8: { 
         Chip->P.O = 0;
      } break; // CLV
      case 0xB9: { 
         AM_ay;
         LD(&Chip->A,Read(Chip, r));
      } break; // LDY ay
      case 0xBC: { 
         AM_ax;
         LD(&Chip->Y,Read(Chip, r));
      } break; // LDY ax
      case 0xBD: { 
         AM_ax;
         LD(&Chip->A,Read(Chip, r));
      } break; // LDA ax
      case 0xBE: { 
         AM_ay;
         LD(&Chip->X,Read(Chip, r));
      } break; // LDX ay
      case 0xC1: {
         AM_ix;
         Cmp(Chip, Read(Chip, r));
      } break; // CMP ix
      case 0xC5: {
         AM_zp;
         Cmp(Chip, Read(Chip, r));
      } break; // CMP zp
      case 0xC9: {
         AM_imm;
         Cmp(Chip, r);
      } break; // CMP imm
      case 0xCD: {
         AM_a;
         Cmp(Chip, Read(Chip, r));
      } break; // CMP abs
      case 0xD0: {
         AM_zp;
         if (!Chip->P.Z)
         {
            Branch(Chip, *(i8*)&r);
         }
      } break; // BEQ
      case 0xD1: {
         AM_iy;
         Cmp(Chip, Read(Chip, r));
      } break; // CMP iy
      case 0xD5: {
         AM_zpx;
         Cmp(Chip, Read(Chip, r));
      } break; // CMP zpx
      case 0xD8: {
         Chip->P.D = 0;
      } break; // CLD
      case 0xD9: {
         AM_ay;
         Cmp(Chip, Read(Chip, r));
      } break; // CMP ay
      case 0xDD: {
         AM_ax;
         Cmp(Chip, Read(Chip, r));
      } break; // CMP ax
      case 0xE1: {
         AM_ix;
         Add(Chip, ~Read(Chip, r));
      } break; // SBC ix
      case 0xE5: {
         AM_zp;
         Add(Chip, ~Read(Chip, r));
      } break; // SBC zp
      case 0xE9: {
         AM_imm;
         Add(Chip, ~r);
      } break; // SBC abs
      case 0xED: {
         AM_a;
         Add(Chip, ~Read(Chip, r));
      } break; // SBC abs
      case 0xF0: {
         AM_zp;
         if (Chip->P.Z)
         {
            Branch(Chip, *(i8*)&r);
         }
      } break; // BEQ
      case 0xF1: {
         AM_iy;
         Add(Chip, ~Read(Chip, r));
      } break; // SBC iy
      case 0xF5: {
         AM_zpx;
         Add(Chip, ~Read(Chip, r));
      } break; // SBC zpx
      case 0xF8: {
         Chip->P.D = 1;
      } break; // CLD
      case 0xF9: {
         AM_ay;
         Add(Chip, ~Read(Chip, r));
      } break; // SBC ay
      case 0xFD: {
         AM_ax;
         Add(Chip, ~Read(Chip, r));
      } break; // SBC ax


      default: {
         fprintf(stderr, "Unimplemented Opcode: %02X\n", Opcode);
         exit(1);
      } break;
   }

}