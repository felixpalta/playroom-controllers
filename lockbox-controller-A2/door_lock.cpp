// 
// 
// 

#include "door_lock.h"
#include "lockbox-controller-pin-config.h"

static bool door_locked;

void door_lock_init()
{
  pinMode(DOOR_LOCK_PIN, OUTPUT);
  digitalWrite(DOOR_LOCK_PIN, LOW);
  door_locked = false;
}


bool door_lock_open()
{
  digitalWrite(DOOR_LOCK_PIN, LOW);
  door_locked = false;
  return true;
}

bool door_lock_close()
{
  digitalWrite(DOOR_LOCK_PIN, HIGH);
  door_locked = true;
  return true;
}

bool door_lock_switch()
{
  if (door_locked)
    return door_lock_open();
  return door_lock_close();
}