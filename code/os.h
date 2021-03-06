#ifndef OS_H
#define OS_H

#define SYSCALL_WRITE 0x01
#define SYSCALL_READ 0x02

#define SYSCALL_NOT_RECOGNIZED -1

void outl(short int address, int data);
int inl(short int address);

void outb(unsigned short int address, unsigned char data);
unsigned char inb(short int address);

int int80h(int syscall, int first, int second, int third, int fourth);

extern int lidt_asm(void *);
extern int lgdt_asm(void *);

/* ISRs */
extern int idt32();
extern int idt128();
extern int default_isr_no_error();
extern int default_isr();

#endif