# playroom-controllers

# building instructions
1. On the Github page press "Download ZIP" button on the right.
2. Download and install Arduino IDE 1.0.x (for example, Arduino 1.0.6). Arduino 1.5.x or 1.6.x won't work!
3. Extract the ZIP archive from #1 to the Arduino sketch folder (specified in Arduino IDE settings). Allow merging the library folder if necessary.
4. Edit header files to configure project settings (ip, port, pin config), as shown below.
5. Build the sketch in Arduino IDE.(Ctrl+R).
6. Connect Arduino Mega 2560 board to the PC via USB cable. Select appropriate COM port in Arduino IDE settings.
7. Upload the sketch to the board (Ctrl+U).
8. Open Serial Monitor (Ctrl+Shift+M) to see serial output from the board.

lockbox-controller-A2
==================
1. To assign Arduino pins to each lockbox, edit "lockbox-controller-pin-config.h".
2. To change server network configuration (mac, ip, port), edit "lockbox-controller-server-config.h".
3. To change device properties (protocol version, serial number, etc), edit "lockbox-controller-rq-valid-protocol-values.h"

