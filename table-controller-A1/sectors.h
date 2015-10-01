#ifndef __SECTORS_H__
#define __SECTORS_H__


void sectors_init();
bool sector_number_led_pin_write(int n, bool on);
bool sector_arrow_led_pin_write(int n, bool on);
void sectors_process_sensors();

extern void sectors_rotation_started_callback();
extern void sectors_rotation_stopped_callback(int sector_n);

#endif // __SECTORS_H__
