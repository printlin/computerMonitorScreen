#pragma once
#include "SerialPort.hpp"
#include "PluginInterface.h"

class HF035 : public ITMPlugin
{
private:
    HF035();

public:
    static HF035& Instance();

    // 通过 ITMPlugin 继承
    virtual IPluginItem* GetItem(int index) override;
    virtual void DataRequired() override;
    virtual const wchar_t* GetInfo(PluginInfoIndex index) override;
    /**
     * @brief   主程序调用此函数以向插件传递所有获取到的监控信息
     */
    virtual void OnMonitorInfo(const MonitorInfo& monitor_info) override;

private:
    static HF035 m_instance;
    SerialPort *serialPort;
    char speedStr1[5];
    char speedStr2[5];
    
    int formatUsage(int num);
    void formatSpeed(unsigned long long  speed, char * buf);
    char * getSpeedPix(unsigned long long  speed);
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) ITMPlugin* TMPluginGetInstance();

#ifdef __cplusplus
}
#endif