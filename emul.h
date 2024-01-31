// Mémoire 64ko
extern unsigned char ram[];

void run_next_opcode(void);
void reset(void);
void load_program(const unsigned char *data, int size);

