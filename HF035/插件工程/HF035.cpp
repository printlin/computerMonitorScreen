#include "HF035.h"
#include "stdio.h"
#include<stdlib.h>

HF035 HF035::m_instance;
SerialPort HF035::*serialPort;

HF035::HF035()
{
    serialPort = new SerialPort();
}

HF035& HF035::Instance()
{
    return m_instance;
}

IPluginItem* HF035::GetItem(int index)
{
    printf("GetItem\n");
    return nullptr;
}

void HF035::DataRequired()
{
    printf("DataRequired\n");
}

const wchar_t* HF035::GetInfo(PluginInfoIndex index)
{
    switch (index)
    {
    case TMI_NAME:
        return L"HF035";
    case TMI_DESCRIPTION:
        return L"HF035 test";
    case TMI_AUTHOR:
        return L"ylcoder";
    case TMI_COPYRIGHT:
        return L"-";
    case TMI_VERSION:
        return L"1.0";
    case ITMPlugin::TMI_URL:
        return L"-";
        break;
    default:
        break;
    }
    return L"";
}

void HF035::OnMonitorInfo(const MonitorInfo& monitor_info)
{
    // 日志调试
	// FILE* fp = fopen("hf035.log", "a+");
    char buf[255];
    // 上行网速
    this->formatSpeed(monitor_info.up_speed, this->speedStr1);
    // 下行网速
    this->formatSpeed(monitor_info.down_speed, this->speedStr2);
    // 乱序格式化串口数据，注意末尾必须‘\r\n’
    sprintf(buf, "SET_TXT(8,'%s');SET_TXT(9,'%s');SET_NUM(0,%d,2);SET_TXT(11,'%s');SET_NUM(1,%d,2);SET_TXT(10,'%s');SET_NUM(2,%d,2);SET_NUM(7,%d,2);SET_NUM(3,%d,2);SET_NUM(6,%d,2);SET_NUM(4,%d,2);SET_NUM(5,%d,2);\r\n",
        //上行网速尾标
        this->getSpeedPix(monitor_info.up_speed),
        //下行网速尾标
        this->getSpeedPix(monitor_info.down_speed),
        // CPU使用率
        this->formatUsage(monitor_info.cpu_usage),
        this->speedStr2,
        // CPU温度
        this->formatUsage(monitor_info.cpu_temperature),
        this->speedStr1,
        // GPU使用率
        this->formatUsage(monitor_info.gpu_usage),
        // 硬盘温度
        this->formatUsage(monitor_info.hdd_temperature),
        // GPU温度
        this->formatUsage(monitor_info.gpu_temperature),
        // 硬盘使用率
        this->formatUsage(monitor_info.hdd_usage),
        // 内存使用率
        this->formatUsage(monitor_info.memory_usage),
        // 主板温度
        this->formatUsage(monitor_info.main_board_temperature));

    // 如果已连接则进行发送，否则进行自动重连
    if(this->serialPort->isConnected()){
        // 发送串口数据
        this->serialPort->writeSerialPort(buf, strlen(buf));
    }else{
        // fwrite("autoConnect\n", 1, 13, fp);
        // 自动扫描连接串口
        this->serialPort->autoConnect();
    }

	// 关闭文件
	// fclose(fp);
    // printf("cpu: %d \n", monitor_info.cpu_usage);
}

/* 格式化使用率，保证数值长度不超过2位，避免界面超出 */
int HF035::formatUsage(int num)
{
    return num < 0 ? 0 : (num > 99 ? 99 : num);
}

/* 格式化网速，最大显示99MB，最小显示0.1KB*/
void HF035::formatSpeed(unsigned long long speed, char * buf)
{
    float temp = 0.1f;
    if(speed > 103809024){
        // 99 MB
        temp = 99.0f;
    }else if(speed > 104858){
        // MB
        temp = speed / 1048576.0f;
    }else if(speed > 103){
        // 0.1KB
        temp = speed / 1024.0f;
    }
    // 数值大于2位时不显示小数，避免界面超出
    if(temp > 9.9f){
        sprintf(buf, "%d", (int)temp);
    }else{
        sprintf(buf, "%.1f", temp);
    }
}

/* 获取网速后缀 */
char * HF035::getSpeedPix(unsigned long long  speed)
{
    if(speed > 1048576){
        // MB
        return "M";
    }
    return "K";
}

ITMPlugin* TMPluginGetInstance()
{
    return &HF035::Instance();
}

// 编译命令：g++ HF035.cpp SerialPort.cpp  -lstdc++ -shared -o HF035.dll
