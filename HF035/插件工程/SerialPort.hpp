#pragma once

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 1024

#include <windows.h>
#include <iostream>

class SerialPort
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    SerialPort();
    ~SerialPort();

    bool openSerial(const char *portName);
    int readSerialPort(const char *buffer, unsigned int buf_size);
    bool writeSerialPort(const char *buffer, unsigned int buf_size);
    bool isConnected();
    void closeSerial();
    bool autoConnect();
};