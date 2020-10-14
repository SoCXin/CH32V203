#ifndef _HARDWARE_H
#define _HARDWARE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Ӳ���жϹ���API */
unsigned long hw_interrupt_disable(void);
void hw_interrupt_enable(unsigned long level);

/* Ӳ���ڴ��д����API */
void memory_erase(unsigned long adderss, unsigned long length);
void memory_write(unsigned long adderss, unsigned char *buffer, unsigned long length);
unsigned long memory_read(unsigned long adderss, unsigned char *buffer, unsigned long length);
unsigned long memory_crc32(unsigned long address, unsigned long length);
void hardware_init(void);

/* Ӧ������API */
void copy_to_runmem(unsigned long jmpaddr, unsigned long length);
void jmp_application(unsigned long address);

#ifdef __cplusplus
}
#endif

#endif
