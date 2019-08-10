#ifndef _MOS6502_H_
#define _MOS6502_H_

#include "common.h"

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

enum interrupt_type
{
   Interrupt_Nmi,
   Interrupt_Reset,
   Interrupt_Irq,
   Interrupt_Brk,
};

//
// User should supply the Read() and Write() functions.
// Allows the usage of various memory mappers.
//
struct mos6502; // forward declared for function pointers

typedef u8 (*BusReadFunc)(mos6502* Chip, u16 Addr);
typedef void (*BusWriteFunc)(mos6502* Chip, u16 Addr, u8 Value);

struct mos6502
{
   // Registers
   u8 A;    
   u8 X;    
   u8 Y;
   status_flags_type P;    
   u8 S;    // Stack pointer
   u16 PC;  // Program counter

   u8 Ram[KB(64)];
   u16 InterruptVectors[4];
   
   void Init(u8* Rom, size_t RomLen, u16 LoadAddress); 
   void Exec();
   void Interrupt(interrupt_type Type);
   BusReadFunc Read;
   BusWriteFunc Write;

private:
   u16 ReadAddress(u16 Addr);
   void Push(u8 V);
   u8 Pop();
   void Add(u8 V);
   void And(u8 V);
   void Asl(u16 Addr);
   void Cmp(u8 V);
   void Branch(i8 Rel);
   void ExecInstruction(u8 Opcode);
};

#endif