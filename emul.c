#include <string.h>

// Définition des registres
unsigned short r1, r2;
unsigned short ip;
unsigned char cc;
// Mémoire 64ko
#define RAM_SIZE 0xFFFF
#define USER_ADDR_START 0x2000
unsigned char ram[RAM_SIZE];

static short read_short_from_ram(int pos)
{
  return (unsigned short)((ram[pos] << 8) + ram[pos+1]);
}

static void write_short_in_ram(int pos, short value)
{
  ram[pos] = (value & 0xFF00) >> 8;
  ram[pos+1] = value & 0xFF;
}

// Exécute la prochaine instruction en mémoire
void run_next_opcode(void)
{
  unsigned char opcode = ram[ip];
  unsigned char reg_num = ram[ip+1];
  unsigned short val = read_short_from_ram(ip+2);
  unsigned short *reg = (reg_num == 1) ? &r1 : &r2;
  ip += 4;
  switch (opcode)
  {
    // LD Rx,0xABCD
    case 0x00:
      *reg = val;
      break;
    // LD Rx,$0xABCD
    case 0x01:
      *reg = read_short_from_ram(val);
      break;
    // ST Rx,$0xABCD
    case 0x10:
      write_short_in_ram(val, *reg);
      break;
    // ST Rx,Ry
    case 0x11:
      unsigned short *reg2 = (val == 1) ? &r1 : &r2;
      write_short_in_ram(*reg2, *reg);
      break;
    // CMP Rx,0xABCD
    case 0x20:
      cc = (*reg == val) ? 1 : 0;
      break;
    // CMP Rx,Ry
    case 0x21:
      cc = (r1 == r2) ? 1 : 0;
      break;
    // BRA 0xABCD
    case 0x30:
      ip = val;
      break;
    // BEQ 0xABCD
    case 0x31:
      if ((cc & 0x01) == 1)
      {
        ip = val;
      }
      break;
    // ADD Rx,0xABCD
    case 0x40:
      *reg += val;
      break;
    // NOP ou instruction invalide
    default:
      // On ne fait rien
  }
}

// Reset du processeur
void reset(void)
{
  memset(ram, 0, RAM_SIZE);
  ip = USER_ADDR_START;
}

// Chargement d'un disque en mémoire
void load_program(const unsigned char *data, int size)
{
  if (size <= RAM_SIZE)
  {
    memcpy(&ram[USER_ADDR_START], data, size);
  }
}

