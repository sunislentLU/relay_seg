#ifndef _UART_H_
#define _UART_H_
#include<iostm8s105c6.h>
#include <stdint.h>
#include <stdio.h>


#ifdef UART_USE_STDIO
#if __ICCARM__
#include <yfuns.h>
#endif
#endif
void Uart_Init(unsigned int baudrate);
#endif
