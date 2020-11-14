#ifndef _HARDWARE_H
#define _HARDWARE_H

#ifdef __cplusplus
extern "C" {
#endif

/* 硬件中断管理API */
unsigned long hw_interrupt_disable(void);
void hw_interrupt_enable(unsigned long level);

/* 硬件内存读写控制API */
void memory_erase(unsigned long adderss, unsigned long length);
void memory_write(unsigned long adderss, unsigned char *buffer, unsigned long length);
unsigned long memory_read(unsigned long adderss, unsigned char *buffer, unsigned long length);
unsigned long memory_crc32(unsigned long address, unsigned long length);
void hardware_init(void);

/* 应用启动API */
void copy_to_runmem(unsigned long jmpaddr, unsigned long length);
void jmp_application(unsigned long address);

#ifdef __cplusplus
}
#endif

#endif
