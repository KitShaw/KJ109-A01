
/*
 * dust.h
 * Kit Shaw
 * 2019/10/02
 */
 
#ifndef __DUST_H_
#define __DUST_H_

#define DUST_PWR_PIN P53
#define	DUST_PIN P54
#define DUST_SIZE  20

void dust_init(void);
void dust_task(void);
void adc_init(unsigned int channel);
void adc_start(void);
unsigned short read_dust_adc_value(void);
unsigned short read_dust_display_value(void);

#endif
