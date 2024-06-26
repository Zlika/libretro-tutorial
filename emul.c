#include <string.h>
#include "emul.h"

// Définition des registres
short r1, r2;
unsigned short ip;
unsigned char cc;
// Mémoire 64ko
unsigned char ram[RAM_SIZE];
// Adresse mémoire du début du programme
#define USER_ADDR_START 0x2000
// Taille de la mémoire vidéo
#define VIDEO_MEM_SIZE 0x1F40

static short read_short_from_ram(int pos)
{
  return (short)((ram[pos] << 8) + ram[pos+1]);
}

static void write_short_in_ram(int pos, short value)
{
  if (pos < VIDEO_MEM_SIZE)
  {
    // Ecriture en mémoire vidéo : on considère la mémoire vidéo
    // comme des mots de 16bits pour éviter les problèmes d'endianess
    // entre la machine émulée et la machine émulante
    ((short *)&ram[pos])[0] = value;
  }
  else
  {
    ram[pos] = (value & 0xFF00) >> 8;
    ram[pos+1] = value & 0xFF;
  }
}

// Exécute la prochaine instruction en mémoire
void run_next_opcode(void)
{
  unsigned char opcode = ram[ip];
  unsigned char reg_num = ram[ip+1];
  short val = read_short_from_ram(ip+2);
  short *reg = (reg_num == 1) ? &r1 : &r2;
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
      short *reg2 = (val == 1) ? &r1 : &r2;
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
      ip = (unsigned short)(val & 0xFFFF);
      break;
    // BEQ 0xABCD
    case 0x31:
      if ((cc & 0x01) == 1)
      {
        ip = (unsigned short)(val & 0xFFFF);
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
  if (size <= RAM_SIZE - USER_ADDR_START)
  {
    memcpy(&ram[USER_ADDR_START], data, size);
  }
}

size_t emul_serialize_size(void)
{
  return RAM_SIZE + sizeof(r1) + sizeof(r2) + sizeof(cc) + sizeof(ip);
}

void emul_serialize(void *data, size_t size)
{
  int offset = 0;
  char *buffer = (char *) data;

  memcpy(buffer+offset, ram, RAM_SIZE);
  offset += RAM_SIZE;
  memcpy(buffer+offset, &r1, sizeof(r1));
  offset += sizeof(r1);
  memcpy(buffer+offset, &r2, sizeof(r2));
  offset += sizeof(r2);
  memcpy(buffer+offset, &cc, sizeof(cc));
  offset += sizeof(cc);
  memcpy(buffer+offset, &ip, sizeof(ip));
}

void emul_unserialize(const void *data, size_t size)
{
  int offset = 0;
  const char *buffer = (const char *) data;

  memcpy(ram, buffer+offset, RAM_SIZE);
  offset += sizeof(ram);
  memcpy(&r1, buffer+offset, sizeof(r1));
  offset += sizeof(r1);
  memcpy(&r2, buffer+offset, sizeof(r2));
  offset += sizeof(r2);
  memcpy(&cc, buffer+offset, sizeof(cc));
  offset += sizeof(cc);
  memcpy(&ip, buffer+offset, sizeof(ip));
}

