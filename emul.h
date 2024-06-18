// Mémoire 64ko
#define RAM_SIZE 0x10000
extern unsigned char ram[];
#define JOYPAD_ADDR   0x1F40
#define KEYBOARD_ADDR 0x1F50
#define SOUND_ADDR    0x1F60

void run_next_opcode(void);
void reset(void);
void load_program(const unsigned char *data, int size);

size_t emul_serialize_size(void);
void emul_serialize(void *data, size_t size);
void emul_unserialize(const void *data, size_t size);
