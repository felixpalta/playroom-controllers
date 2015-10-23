#ifndef __SWITCHABLE_PRINTER_H__
#define __SWITCHABLE_PRINTER_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

class SwitchablePrinter : public Print
{
private:
    Print& m_print;
    bool m_enabled;
public:
    SwitchablePrinter(Print& p) :
        m_print(p), m_enabled(true) {}
        
    void print_enable(bool on)
    {
        m_enabled = on;
    }
    
    size_t write(uint8_t c) /* Print::write override */
    {
        if (m_enabled)
            return m_print.write(c);
        return 1;
    }
    
    size_t write(const uint8_t *buffer, size_t size) /* Print::write override */
    {
        if (m_enabled)
            return m_print.write(buffer, size);
        return size;
    }
};

#endif // __SWITCHABLE_PRINTER_H__
