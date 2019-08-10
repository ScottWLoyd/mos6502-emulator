
#include <stdlib.h>
#include <string.h>

#include "mos6502.h"


void mos6502::Init(u8* Rom, size_t RomLen, u16 LoadAddress)
{
   InterruptVectors[Interrupt_Nmi]   = 0xFFFA;
   InterruptVectors[Interrupt_Reset] = 0xFFFC, // Reset
   InterruptVectors[Interrupt_Irq]   = 0xFFFE, // IRQ
   InterruptVectors[Interrupt_Brk]   = 0xFFFE, // BRK

   // Ref: http://wiki.nesdev.com/w/index.php/CPU_power_up_state
   P.Value = 0x34;
   A = X = Y = 0;
   S = 0xFD;
   memcpy(Ram + LoadAddress, Rom, RomLen);
}

void mos6502::Exec()
{
   u8 Opcode = Ram[PC++];
   ExecInstruction(Opcode);
}

inline u16 mos6502::ReadAddress(u16 Addr)
{
   u16 Low = (u16)Read(this, Addr);
   u16 High = (u16)Read(this, Addr + 1);
   u16 Result = (High << 8) | Low;
   return Result;
}

inline void mos6502::Push(u8 V)
{
   Ram[0x100 + (S--)] = V;
}

inline u8 mos6502::Pop()
{
   return Ram[0x100 + (++S)];
}

inline void mos6502::Interrupt(interrupt_type Type)
{
   // If interrupt disable is true, all interrupts
   // are ignored except NMI
   if (P.I && Type != Interrupt_Nmi)
   {
      return;
   }
   P.I = 1;  // Disable interrupts
   // Store the Program Counter and Status Register
   if (Type != Interrupt_Reset)
   {
      Push(PC >> 8); Push(PC & 0xFF);
      P.B = (Type == Interrupt_Brk);
      Push(P.Value);
   }
   else
   {
      S -= 3;
   }
   u16 Addr = ReadAddress(InterruptVectors[Type]);
   PC = Addr;
}


#define SetSZ(v) P.S=((v)>>7)&1; P.Z=((v)==0)

inline void mos6502::Add(u8 V)
{
   u16 Result = A + P.C + V;
   // TODO(scott): Implement decimal here
   // if(P.D)
   //{
   //
   //}
   //else
   {
      //P.V = ((A ^ Result) & (V ^ Result) & 0x80) == 0x80 ? 1 : 0;
      P.O = (~(A^V) & (A^Result) & 0x80) >> 7;
      P.C = (Result > 0xFF);
      SetSZ(Result);
   }
   A = (Result & 0xFF);
}

inline void mos6502::And(u8 V)
{
   A = A & V;
   SetSZ(A);
}

inline void mos6502::Asl(u16 Addr)
{
   u8 V = Read(this, Addr);
   P.C = (V & 0x80) >> 7;
   V = (V << 1);
   SetSZ(V);
   Write(this, Addr, V);
}

inline void mos6502::Cmp(u8 V)
{
   P.C = (A >= V);
   SetSZ(A - V);
}

inline void mos6502::Branch(i8 Rel)
{
   PC += Rel;
}

#define LD(r,v) *(r)=(v); SetSZ((v))

#define NextByte Read(this, PC++)
#define rd16(a,b) (Read(this, (a))|(Read(this, (b))<<8))

#define AM_imm u8 r=NextByte
#define AM_a   u16 r=(NextByte|(NextByte)<<8)
#define AM_zp  u16 r=NextByte
#define AM_r   u16 r=(((i8)NextByte)+(PC-1)
#define AM_ax  u16 r=X+(NextByte|(NextByte)<<8)
#define AM_ay  u16 r=Y+(NextByte|(NextByte)<<8)
#define AM_zpx u16 r=(X+NextByte)&0xff
#define AM_zpy u16 r=(Y+NextByte)&0xff
#define AM_ix  u16 b=(X+NextByte)&0xff; u16 r=rd16(b, b+1)
#define AM_iy  u16 i=NextByte; u16 r=rd16(i, (i+1)%0x0100) + Y;

inline void mos6502::ExecInstruction(u8 Opcode)
{
   switch (Opcode)
   {
   case 0x00: {
      P.B = 1;
      Interrupt(Interrupt_Brk);
   } break;
   case 0x06: {
      AM_zp;
      Asl(r);
   } break; // ASL zp
   case 0x10: {
      AM_zp;
      if (P.S == 0)
      {
         Branch(*(i8*)&r);
      }
   } break; // BPL
   case 0x0A: {
      P.C = (A & 0x8) >> 7;
      A = (A << 1);
      P.S = (A & 0x8) >> 7;
      P.Z = (A == 0);
   } break; // ASL A
   case 0x0E: {
      AM_a;
      Asl(r);
   } break; // ASL abs
   case 0x16: {
      AM_zpx;
      Asl(r);
   } break; // ASL zpx
   case 0x18: {
      P.C = 0;
   } break; // CLC
   case 0x1E: {
      AM_ax;
      Asl(r);
   } break; // ASL ax
   case 0x21: {
      AM_ix;
      And(Read(this, r));
   } break; // AND ix
   case 0x24: {
      AM_zp;
      u8 V = Read(this, r);
      P.Z = ((A & V) == 0);
      P.S = (V & 0x80) >> 7;
      P.O = (V & 0x40) >> 6;
   } break; // BIT zp
   case 0x25: {
      AM_zp;
      And(Read(this, r));
   } break; // AND zp
   case 0x29: {
      AM_imm;
      And(r);
   } break; // AND imm
   case 0x2C: {
      AM_a;
      u8 V = Read(this, r);
      P.Z = ((A & V) == 0);
      P.S = (V & 0x80) >> 7;
      P.O = (V & 0x40) >> 6;
   } break; // BIT abs
   case 0x2D: {
      AM_a;
      And(Read(this, r));
   } break; // AND abs
   case 0x30: {
      AM_zp;
      if (P.S)
      {
         Branch(*(i8*)&r);
      }
   } break; // BMI
   case 0x31: {
      AM_iy;
      And(Read(this, r));
   } break; // AND iy
   case 0x35: {
      AM_zpx;
      And(Read(this, r));
   } break; // AND zpx
   case 0x38: {
      P.C = 1;
   } break; // SEC
   case 0x3D: {
      AM_ax;
      And(Read(this, r));
   } break; // AND ax
   case 0x39: {
      AM_ay;
      And(Read(this, r));
   } break; // AND ay
   case 0x4C: {
      AM_a;
      PC = r;
   } break;
   case 0x50: {
      AM_zp;
      if (!P.O)
      {
         Branch(*(i8*)&r);
      }
   } break; // BVS
   case 0x58: {
      P.I = 0;
      // TODO(Scott): Implement discrete lines. When I bit is
      // cleared, if /IRQ line is low, an interrupt is immediately
      // triggered.
   } break; // CLI
   case 0x61: {
      AM_ix;
      Add(Read(this, r));
   } break; // ADC ix
   case 0x65: {
      AM_zp;
      Add(Read(this, r));
   } break; // ADC zp
   case 0x69: {
      AM_imm;
      Add(r);
   } break; // ADC imm
   case 0x6C: {
      AM_a;
      u16 Second = r + 1;
      if ((Second / 0x100) != (r / 0x100))
      {
         Second -= 0x100;
      }
      u16 Hi = Read(this, Second);
      u16 Lo = Read(this, r);
      PC = (Hi << 8) | Lo;
   } break;
   case 0x6D: {
      AM_a;
      Add(Read(this, r));
   } break; // ADC abs
   case 0x70: {
      AM_zp;
      if (P.O)
      {
         Branch(*(i8*)&r);
      }
   } break; // BVS
   case 0x71: {
      AM_iy;
      Add(Read(this, r));
   } break; // ADC iy
   case 0x75: {
      AM_zpx;
      Add(Read(this, r));
   } break; // ADC zpx
   case 0x78: {
      P.I = 1;
   } break; // SEI
   case 0x79: {
      AM_ay;
      Add(Read(this, r));
   } break; // ADC zpy
   case 0x7D: {
      AM_ax;
      Add(Read(this, r));
   } break; // ADC zpx
   case 0x81: {
      AM_ix;
      Write(this, r, A);
   } break; // STA ix
   case 0x84: {
      AM_zp;
      Write(this, r, Y);
   } break; // STY zp
   case 0x85: {
      AM_zp;
      Write(this, r, A);
   } break; // STA zp
   case 0x86: {
      AM_zp;
      Write(this, r, X);
   } break; // STX zp
   case 0x8C: {
      AM_a;
      Write(this, r, Y);
   } break; // STY addr
   case 0x8D: {
      AM_a;
      Write(this, r, A);
   } break; // STA addr
   case 0x8E: {
      AM_a;
      Write(this, r, X);
   } break; // STX addr
   case 0x90: {
      AM_zp;
      if (!P.C)
      {
         Branch(*(i8*)&r);
      }
   } break; // BCC
   case 0x91: {
      AM_iy;
      Write(this, r, A);
   } break; // STA iy
   case 0x94: {
      AM_zpx;
      Write(this, r, Y);
   } break; // STY zpx
   case 0x95: {
      AM_zpx;
      Write(this, r, A);
   } break; // STA zpx
   case 0x96: {
      AM_zpy;
      Write(this, r, X);
   } break; // STX zpy
   case 0x99: {
      AM_ax;
      Write(this, r, A);
   } break; // STA ay
   case 0x9D: {
      AM_ax;
      Write(this, r, A);
   } break; // STA ax
   case 0xA0: {
      AM_imm;
      LD(&Y, r);
   } break; // LDY #
   case 0xA1: {
      AM_ix;
      LD(&A, Read(this, r));
   } break; // LDA inx
   case 0xA2: {
      AM_imm;
      LD(&X, r);
   } break; // LDX #
   case 0xA4: {
      AM_zp;
      LD(&Y, Read(this, r));
   } break; // LDY zp
   case 0xA5: {
      AM_zp;
      LD(&A, Read(this, r));
   } break; // LDA zp
   case 0xA6: {
      AM_zp;
      LD(&X, Read(this, r));
   } break; // LDX zp
   case 0xA9: {
      AM_imm;
      LD(&A, r);
   } break; // LDA #
   case 0xAC: {
      AM_a;
      LD(&Y, Read(this, r));
   } break; // LDY a
   case 0xAD: {
      AM_a;
      LD(&A, Read(this, r));
   } break; // LDA a
   case 0xAE: {
      AM_a;
      LD(&X, Read(this, r));
   } break; // LDX a
   case 0xB0: {
      AM_zp;
      if (P.C)
      {
         Branch(*(i8*)&r);
      }
   } break; // BCS
   case 0xB1: {
      AM_iy;
      LD(&A, Read(this, r));
   } break; // LD(&LDA i,
   case 0xB4: {
      AM_zpx;
      LD(&Y, Read(this, r));
   } break; // LDY zpx
   case 0xB5: {
      AM_zpx;
      LD(&A, Read(this, r));
   } break; // LDA zpx
   case 0xB6: {
      AM_zpy;
      LD(&X, Read(this, r));
   } break; // LDX zpy
   case 0xB8: {
      P.O = 0;
   } break; // CLV
   case 0xB9: {
      AM_ay;
      LD(&A, Read(this, r));
   } break; // LDY ay
   case 0xBC: {
      AM_ax;
      LD(&Y, Read(this, r));
   } break; // LDY ax
   case 0xBD: {
      AM_ax;
      LD(&A, Read(this, r));
   } break; // LDA ax
   case 0xBE: {
      AM_ay;
      LD(&X, Read(this, r));
   } break; // LDX ay
   case 0xC1: {
      AM_ix;
      Cmp(Read(this, r));
   } break; // CMP ix
   case 0xC5: {
      AM_zp;
      Cmp(Read(this, r));
   } break; // CMP zp
   case 0xC9: {
      AM_imm;
      Cmp(r);
   } break; // CMP imm
   case 0xCD: {
      AM_a;
      Cmp(Read(this, r));
   } break; // CMP abs
   case 0xD0: {
      AM_zp;
      if (!P.Z)
      {
         Branch(*(i8*)&r);
      }
   } break; // BEQ
   case 0xD1: {
      AM_iy;
      Cmp(Read(this, r));
   } break; // CMP iy
   case 0xD5: {
      AM_zpx;
      Cmp(Read(this, r));
   } break; // CMP zpx
   case 0xD8: {
      P.D = 0;
   } break; // CLD
   case 0xD9: {
      AM_ay;
      Cmp(Read(this, r));
   } break; // CMP ay
   case 0xDD: {
      AM_ax;
      Cmp(Read(this, r));
   } break; // CMP ax
   case 0xE1: {
      AM_ix;
      Add(~Read(this, r));
   } break; // SBC ix
   case 0xE5: {
      AM_zp;
      Add(~Read(this, r));
   } break; // SBC zp
   case 0xE9: {
      AM_imm;
      Add(~r);
   } break; // SBC abs
   case 0xED: {
      AM_a;
      Add(~Read(this, r));
   } break; // SBC abs
   case 0xF0: {
      AM_zp;
      if (P.Z)
      {
         Branch(*(i8*)&r);
      }
   } break; // BEQ
   case 0xF1: {
      AM_iy;
      Add(~Read(this, r));
   } break; // SBC iy
   case 0xF5: {
      AM_zpx;
      Add(~Read(this, r));
   } break; // SBC zpx
   case 0xF8: {
      P.D = 1;
   } break; // CLD
   case 0xF9: {
      AM_ay;
      Add(~Read(this, r));
   } break; // SBC ay
   case 0xFD: {
      AM_ax;
      Add(~Read(this, r));
   } break; // SBC ax


   default: {
      fprintf(stderr, "Unimplemented Opcode: %02X\n", Opcode);
      exit(1);
   } break;
   }
}