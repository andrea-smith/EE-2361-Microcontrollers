Lab 5: I2C 3.3V SwitchScience Small LCD Display Library
EE 2361- Spring 2020
Andrea Smith

Overview:
————————-
Inter-Integrated Circuit, or I2C, is a method of intermittent simple, low-bandwidth, short-distance, communication for devices with slow onboard peripherals. This C library utilizes this type of serial interface to provide a way to display messages on the Switch Science I2C LCD Display with the PIC24FJ64GA002 micro-controller.

Dependencies:
————————————
- std.h
- string.h

Under the Hood:
——————————————

1. void lcd_init(void);
Initializes the LCD by issuing a complete sequence of lcd_cmd() calls. No arguments.

2. void lcd_setCursor(char x, char y);
Sets the LCD screen cursor to column x and row y. No arguments.

3. void lcd_printChar(char myChar);
This function served mostly the LCD’s functionality before moving on to the lcd_printStr() function. It contains a start bit, address + R/nW, control, and command bytes, and stop bit. Capable of printing a single char to the LCD. Takes a single char as args.

4. void lcd_printStr(const char s[]);
This function takes arbitrary length string and sends it to the LCD. Contains multiple command/data byte pairs. Takes a string as args.