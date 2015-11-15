// 
// 
// 

#include "door_lock.h"
#include "lockbox-controller-pin-config.h"

void door_lock_init()
{
  pinMode(DOOR_LOCK_PIN, OUTPUT);
  digitalWrite(DOOR_LOCK_PIN, LOW);
}


bool door_lock_open()
{
  digitalWrite(DOOR_LOCK_PIN, LOW);
  return true;
}

bool door_lock_close()
{
  digitalWrite(DOOR_LOCK_PIN, HIGH);
  return true;
}

