
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

inline u16
Read(mos6502* Chip, u16 Addr)
{
   u16 Result = Chip->Ram[Addr];
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


internal void
InitChip(mos6502* Chip, u8* Rom, u32 RomLen, u16 LoadAddress)
{
   Chip->A = Chip-> X = Chip->Y = 0;
   memcpy(Chip->Ram + LoadAddress, Rom, RomLen);

   HandleInterrupt(Chip, Interrupt_Reset);
}


#define NextByte Read(Chip, Chip->PC++)
#define rd16(a,b) (((u16)Chip->Ram[a])<<8|Chip->Ram[b])

#define AM_imm u16 r=NextByte
#define AM_a   u16 r=(NextByte|(NextByte)<<8)
#define AM_zp  u16 r=Chip->Ram[NextByte]
#define AM_r   u16 r=(((i8)NextByte)+(Chip->PC-1)
#define AM_ax  u16 r=Chip->Ram[Chip->X+(NextByte|(NextByte)<<8)]
#define AM_ay  u16 r=Chip->Ram[Chip->Y+(NextByte|(NextByte)<<8)]
#define AM_zpx u16 r=Chip->Ram[(Chip->X+NextByte)&0xff]
#define AM_zpy u16 r=Chip->Ram[(Chip->Y+NextByte)&0xff]
#define AM_ix  u16 b=(Chip->X+NextByte)&0xff; u8 r=Chip->Ram[(((u16)Chip->Ram[b+1])<<8|(u16)Chip->Ram[b])]
#define AM_iy	u16 i=NextByte; u16 r=rd16(i, (i+1)%0x0100) + Chip->Y;

inline void
ExecInstruction(mos6502* Chip, u8 Opcode)
{
   switch(Opcode)
   {
      case 0x84: { 
         AM_zp; 
         Chip->Ram[r]=Chip->Y;    
      } break; // STY zp
      case 0x85: { 
         AM_zp; 
         Chip->Ram[r]=Chip->A;    
      } break; // STA zp
      case 0x8C: { 
         AM_a; 
         Chip->Ram[r]=Chip->Y;     
      } break; // STY addr
      case 0x8D: { 
         AM_a; 
         Chip->Ram[r]=Chip->A;     
      } break; // STA addr
      case 0x8E: { 
         AM_a; 
         Chip->Ram[r]=Chip->X;
      } break; // STX addr
      case 0x91: { 
         AM_iy; 
         Chip->Ram[r]=Chip->A;    
      } break; // STA iy
      case 0x95: { 
         AM_zpx; 
         Chip->Ram[r]=Chip->A;   
      } break; // STA zpx
      case 0x9D: { 
         AM_ax; 
         Chip->Ram[r]=Chip->A;    
      } break; // STA ax
      case 0xA0: { 
         AM_imm; 
         Chip->Y=r;              
      } break; // LDY #
      case 0xA2: { 
         AM_imm; 
         Chip->X=r;              
      } break; // LDX #
      case 0xA5: { 
         AM_zp; 
         Chip->A=r;               
      } break; // LDA zp
      case 0xA9: { 
         AM_imm; 
         Chip->A=r;              
      } break; // LDA #
      case 0xAD: { 
         AM_a; 
         Chip->A=Chip->Ram[r];     
      } break; // LDA a      
      case 0xB1: { 
         AM_iy; 
         Chip->A=Chip->Ram[r];    
      } break; // LDA iy
      case 0xB5: { 
         AM_zpx; 
         Chip->A=Chip->Ram[r];   
      } break; // LDA zpx

/*
      // TODO(scott): finish implementing this
      case 0xBD: { 
         AM_ax; 
         Chip->A=Chip->Ram[r];    
      } break; // LDA ax
      */

      default: {
         fprintf(stderr, "Unimplemented Opcode: %02X\n", Opcode);
         exit(1);
      } break;
   }

#if 0
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
#endif
}