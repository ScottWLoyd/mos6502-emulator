
#define DEBUG

#define Unimplemented (*(int*)0=1)

#ifdef DEBUG
#define DPRINT(F, ...) fprintf(stderr, (F), __VA_ARGS__)
#else
#define DPRINT
#endif

#define KB(X) ((X)*1024)

#pragma pack(push, 1)
union status_flags_type
{
   struct
   {
      u8 Carry      : 1;
      u8 Zero       : 1;
      u8 IntDisable : 1;
      u8 Decimal    : 1;
      u8 Spare      : 2;
      u8 Overflow   : 1;
      u8 Sign       : 1;
   };
   u8 V;
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

internal void
InitChip(mos6502* Chip, u8* Rom, u32 RomLen, u16 LoadAddress)
{
   Chip->A = Chip-> X = Chip->Y = 0;
   memcpy(Chip->Ram + LoadAddress, Rom, RomLen);
}

enum interrupt_type
{
   Interrupt_Nmi,
   Interrupt_Reset,
   Interrupt_Irq,
};

u16 InterruptVectors[3][2] = 
{
   {0xFFFA, 0xFFFB}, // NMI
   {0xFFFC, 0xFFFD}, // Reset
   {0xFFFE, 0xFFFF}, // IRQ
};

inline u8
Read(mos6502* Chip, u16 Addr)
{
   u8 Result = Chip->Ram[Addr];
   return Result;
}

inline void
Write(mos6502* Chip, u16 Addr, u8 Value)
{
   Chip->Ram[Addr] = Value;
}

inline u16
ReadAddress(mos6502* Chip, u16 Addr)
{
   u16 Low = (u16)Read(Chip, Addr);
   u16 High = (u16)Read(Chip, Addr + 1);
   u16 Result = (High << 8) | Low;
   return Result;
}

inline void
HandleInterrupt(mos6502* Chip, interrupt_type Type)
{
   u16 Addr = ReadAddress(Chip, InterruptVectors[Type][0]);
   Chip->PC = Addr;
}

//
// Addressing modes
//
inline u16 imm(mos6502* Chip)
{
   return Chip->Ram[Chip->PC++];
}

inline u16 zp(mos6502* Chip)
{
   u8 Offset = Chip->Ram[Chip->PC++];
   return Chip->Ram[Offset];
}

inline u16 zpx(mos6502* Chip)
{
   u16 Base = Chip->Ram[Chip->PC++];
   u16 Result = (Base + Chip->X) % 0x0100;
   return Result;
}

inline u16 zpy(mos6502* Chip)
{
   u16 Base = Chip->Ram[Chip->PC++];
   u16 Result = (Base + Chip->Y) % 0x0100;
   return Result;
}

inline u16 abs(mos6502* Chip)
{
   u16 Result = ReadAddress(Chip, Chip->PC);
   Chip->PC += 2;
   return Result;
}

inline u16 abx(mos6502* Chip)
{
   u16 Base = ReadAddress(Chip, Chip->PC);
   Chip->PC += 2;
   u16 Result = (Base + Chip->X) % 0x0100;
   return Result;
}

inline u16 aby(mos6502* Chip)
{
   u16 Base = ReadAddress(Chip, Chip->PC);
   Chip->PC += 2;
   u16 Result = (Base + Chip->Y) % 0x0100;
   return Result;
}

inline u16 inx(mos6502* Chip)
{
   u16 Addr = (Chip->Ram[Chip->PC++] + Chip->X) % 0x0100;
   u8 Result = Chip->Ram[Addr];
   return Result;
}

inline u16 iny(mos6502* Chip)
{
   u16 Addr = (Chip->Ram[Chip->PC++] + Chip->Y) % 0x0100;
   u8 Result = Chip->Ram[Addr];
   return Result;
}


// LDA Variants
#define LDA(MN) inline void lda_##MN(mos6502* Chip) \
   {  \
      u8 Arg = (u8)##MN(Chip);      \
      Chip->A = Arg;                \
      DPRINT("LDA: A = %02X\n", Chip->A);  \
   }

LDA(imm)
LDA(zp)
LDA(zpx)
LDA(abs)
LDA(abx)
LDA(aby)
LDA(inx)
LDA(iny)

// STA Variants
#define STA(MN) inline void sta_##MN(mos6502* Chip) \
   {  \
      u16 Addr = ##MN(Chip);     \
      Chip->Ram[Addr] = Chip->A; \
      DPRINT("STA: 0x%04X = %02X\n", Addr, Chip->A);  \
   }

STA(zp)
STA(zpx)
STA(abs)
STA(abx)
STA(aby)
STA(inx)
STA(iny)

// STX Variants
#define STX(MN) inline void stx_##MN(mos6502* Chip) \
   {  \
      u16 Addr = ##MN(Chip);     \
      Chip->Ram[Addr] = Chip->X; \
      DPRINT("STX: 0x%04X = %02X\n", Addr, Chip->X);  \
   }

STX(zp)
STX(abs)
STX(zpy)

// STY Variants
#define STY(MN) inline void sty_##MN(mos6502* Chip) \
   {  \
      u16 Addr = ##MN(Chip);     \
      Chip->Ram[Addr] = Chip->Y; \
      DPRINT("STY: 0x%04X = %02X\n", Addr, Chip->Y);  \
   }

STY(zp)
STY(zpx)
STY(abs)

// LDX Variants
#define LDX(MN) inline void ldx_##MN(mos6502* Chip) \
   {  \
      u8 Arg = (u8)##MN(Chip);   \
      Chip->X = Arg;             \
      DPRINT("LDX: X = %02X\n", Chip->X); \
   }

LDX(imm)
LDX(zp)
LDX(zpy)
LDX(abs)
LDX(aby)

// LDY Variants
#define LDY(MN) inline void ldy_##MN(mos6502* Chip) \
   {  \
      u8 Arg = (u8)##MN(Chip);   \
      Chip->Y = Arg;             \
      DPRINT("LDY: Y = %02X\n", Chip->Y); \
   }

LDY(imm)
LDY(zp)
LDY(zpx)
LDY(abs)
LDY(abx)

// LDY Variants
#define CMP(MN) inline void cmp_##MN(mos6502* Chip) \
   {  \
      u8 Arg = (u8)##MN(Chip);            \
      Chip->P.Carry  = (Chip->A >= Arg);  \
      Chip->P.Zero = (Chip->A == 0);      \
      Chip->P.Sign = (Chip->A >= 0x80);   \
      DPRINT("CMP: A,V = %02X, %02X, Flags=%02X\n", Chip->A, Arg, Chip->P.V); \
   }

CMP(abs)

inline void beq(mos6502* Chip)
{
   if (Chip->P.Zero)
   {
      u16 Displacement = Chip->Ram[Chip->PC++];
      Chip->PC += *(i8*)&Displacement;
   }
}

inline void
ExecInstruction(mos6502* Chip, u8 Opcode)
{
   switch(Opcode)
   {
      case 0x81: { sta_inx(Chip); } break;
      case 0x84: { sty_zp(Chip);  } break;
      case 0x85: { sta_zp(Chip);  } break;      
      case 0x86: { stx_zp(Chip);  } break;
      case 0x8C: { sty_abs(Chip); } break;
      case 0x8D: { sta_abs(Chip); } break;
      case 0x8E: { stx_abs(Chip); } break;
      case 0x91: { sta_iny(Chip); } break;
      case 0x94: { sty_zpx(Chip); } break;
      case 0x95: { sta_zpx(Chip); } break;
      case 0x96: { stx_zpy(Chip); } break;
      case 0x99: { sta_aby(Chip); } break;
      case 0x9D: { sta_abx(Chip); } break;
      case 0xA0: { ldy_imm(Chip); } break;
      case 0xA1: { lda_inx(Chip); } break;
      case 0xA2: { ldx_imm(Chip); } break;
      case 0xA4: { ldy_zp(Chip);  } break;
      case 0xA5: { lda_zp(Chip);  } break;
      case 0xA6: { ldx_zp(Chip);  } break;
      case 0xA9: { lda_imm(Chip); } break;
      case 0xAC: { ldy_abs(Chip); } break;
      case 0xAD: { lda_abs(Chip); } break;
      case 0xAE: { ldx_abs(Chip); } break;
      case 0xB1: { lda_iny(Chip); } break;
      case 0xB4: { ldy_zpx(Chip); } break;
      case 0xB5: { lda_zpx(Chip); } break;
      case 0xB6: { ldx_zpy(Chip); } break;
      case 0xB9: { lda_aby(Chip); } break;      
      case 0xBC: { ldy_abx(Chip); } break;     
      case 0xBD: { lda_abx(Chip); } break;
      case 0xBE: { ldx_aby(Chip); } break;
      case 0xCD: { cmp_abs(Chip); } break;
      case 0xF0: { beq(Chip);     } break;
      default: {
         fprintf(stderr, "Unimplemented Opcode: %02X\n", Opcode);
         exit(1);
      } break;
   }
}