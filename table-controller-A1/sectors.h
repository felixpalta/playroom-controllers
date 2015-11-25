#ifndef __SECTORS_H__
#define __SECTORS_H__


void sectors_init();
bool sector_number_led_pin_write(int n, bool on);
bool sector_arrow_led_pin_write(int n, bool on);
bool sector_all_number_leds_write(bool on);
bool sector_all_arrow_leds_write(bool on);
bool sectors_all_leds_write(bool on);
bool sectors_show_playing_sector(int actual_sector, int playing_sector);
void sectors_process_sensors();

typedef enum
{
  SECTOR_EVENT_NONE,
  SECTOR_EVENT_STARTED,
  SECTOR_EVENT_STOPPED,
} SectorEvent;

struct SectorEventData
{
  SectorEvent event;
  int sector_number;
  void set_event_started() { event = SECTOR_EVENT_STARTED; };
  void set_event_stopped(int sector) { event = SECTOR_EVENT_STOPPED; sector_number = sector; }
  void reset() { event = SECTOR_EVENT_NONE; sector_number = -1; }
  SectorEventData()
    : event(SECTOR_EVENT_NONE), sector_number(-1) {};
};
  
bool is_sector_event_ready(SectorEventData *data);

void reset_sector_event();

void set_wait_for_spin_flag();

#endif // __SECTORS_H__
