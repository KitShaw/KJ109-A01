
/*
 * dust.h
 * Kit Shaw
 * 2019/10/02
 */
 
#ifndef __DUST_H_
#define __DUST_H_

#define DUST_PWR_PIN P53
#define	DUST_PIN P54
#define DUST_SIZE  10

void dust_init(void);
void dust_task(void);
void adc_init(unsigned int channel);
void adc_start(void);
unsigned short read_dust_adc_value(void);

#endif
