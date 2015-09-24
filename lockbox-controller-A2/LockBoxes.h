// LockBoxes.h

#ifndef _LOCKBOXES_h
#define _LOCKBOXES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class LockBoxes
{
public:
	LockBoxes() {};
	void begin();
	boolean open(int n);
	boolean open_all();
	boolean close(int n);
	boolean close_all();

	const char *get_last_error() const;

private:
	static boolean check_number_convert_to_internal(int& n);

	typedef enum
	{
		ERROR_NONE,
		ERROR_INVALID_LOCKBOX_NUMBER,
	} ErrorCode;

	ErrorCode error_code;


};

#endif

