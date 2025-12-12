/*
This is the header file for the Auto Light System project, used for storing constant and configuration.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
If you have any varaible or constant that you want to use across multiple files, put it here.
*/

#ifndef _config_h_
#define _config_h_

#include <Arduino.h>

#define WIFI_CONN (1 << 0)


#define WIFISSID "AutoLightSys" // Replace with your WiFi SSID
#define WIFIPWD "111114115" // Replace with your WiFi password

const TickType_t wifiReconnPeriod = 20000; // Period to attempt WiFi reconnection in milliseconds

/* ThingsCloud / MQTT 配置占位符
 * 请根据你的 ThingsCloud 平台/设备信息替换下面的占位宏：
 * - THINGSCLOUD_HOST: MQTT Broker 地址（域名或 IP）
 * - THINGSCLOUD_PORT: MQTT Broker 端口（通常 1883 或 8883）
 * - THINGSCLOUD_USER / THINGSCLOUD_PWD: 连接凭据（如果需要）
 * - THINGSCLOUD_CLIENTID_PREFIX: 生成 clientId 的前缀
 */
#define THINGSCLOUD_HOST "broker.thingscloud.example" // TODO: replace with real host
#define THINGSCLOUD_PORT 1883
#define THINGSCLOUD_USER "your_username"
#define THINGSCLOUD_PWD "your_password"
#define THINGSCLOUD_CLIENTID_PREFIX "AutoLightSys-"
// ThingsCloud SDK specific placeholders (fill with your platform/device values)
#define THINGSCLOUD_PROJECT_KEY "your_project_key"
#define THINGSCLOUD_DEVICE_KEY "your_device_key"
#define THINGSCLOUD_TYPE_KEY "your_type_key"
#define THINGSCLOUD_API_ENDPOINT "https://api.thingscloud.example" // used when fetching tokens (if needed)

#endif