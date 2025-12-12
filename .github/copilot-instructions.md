## 快速目标（供 AI 代码助手使用）

这个仓库是一个基于 PlatformIO/Arduino 的嵌入式工程（ESP32 系列）。以下说明帮助 AI 代理快速定位修改点、理解约定、并给出可执行的补丁或 PR。

### 一句话概览
主程序在 `src/main.cpp` 中；系统由三大子模块构成：WiFi（`include/WiFiCmd.h` + `src/WiFiCmd.cpp`）、Bluetooth/BLE（`include/BTCmd.h` + `src/BTCmd.cpp`）、舵机控制（`include/servoCmd.h` + `src/servoCmd.cpp`）。跨模块共享常量和配置在 `include/config.h`。FreeRTOS 用于任务与事件（例如 `wifiEventGroup`）。

## 要点与文件引用（高优先级）
- 构建配置：`platformio.ini`（env: `airm2m_core_esp32c3`，board=`airm2m_core_esp32c3`，framework=`arduino`，使用 `max_app_4MB.csv` 分区表）。
- 启动流程：`src/main.cpp`。
  - 调用 `systemInit()`（串口、WiFi、BT、servo 初始化）。
  - 创建 BLE 服务：`btSetUp()` 并创建 `btMaintain` 任务。
  - WiFi 连接逻辑分散在 `WiFiCmd.*`：`wifiScan()`、`wifiLogin()`、`wifiInit()`。
- BLE：`src/BTCmd.cpp` 使用 `BLESecurity`、设置 MTU（517）、并实现基于授权的读（AUTH_PASSKEY）。修改 BLE 行为通常在此文件与 `include/BTCmd.h` 中进行。
- 共享配置：`include/config.h`（包含 `WIFISSID` / `WIFIPWD`、`WIFI_CONN` 位和 `wifiReconnPeriod`）。修改默认 WiFi 凭据请直接更新此文件。

## 构建 / 上传 / 调试（在 Windows PowerShell）
- 本地构建：
```powershell
pio run -e airm2m_core_esp32c3
```
- 烧录设备：
```powershell
pio run -e airm2m_core_esp32c3 -t upload
```
- 串口监视（Serial Monitor，波特率由 `platformio.ini` 的 `monitor_speed` 决定，默认为 115200）：
```powershell
pio device monitor -e airm2m_core_esp32c3
```
- 说明：使用 VSCode + PlatformIO 插件时，环境名 `airm2m_core_esp32c3` 是关键，修改/测试不同板卡时请参考 `platformio.ini`。

## 项目约定与习惯（可直接采纳）
- 风格：注释里标注为 Google C++ Style / C++11。请保持现有命名与返回约定（多数 init 函数返回 `bool` 表示成功/失败）。
- FreeRTOS：使用 `EventGroupHandle_t wifiEventGroup` 保存 WiFi 状态位（`WIFI_CONN`），使用 `xTaskCreate` 启动后台任务（例如 `btMaintain`, `keepLoginWiFi`）。修改任务栈/优先级时请在 `src/main.cpp` 中对应位置处理。
- 分区表：项目根有 `max_app_4MB.csv`，构建时通过 `board_build.partitions` 生效——不要更换除非确实需要更多/更少应用空间。

## 常见改动举例（参照代码）
- 变更 WiFi 重连策略：编辑 `src/WiFiCmd.cpp` 的 `wifiLogin()` 和 `wifiReconnPeriod`（`include/config.h`）。
- 添加/修改 BLE 特性或授权流程：编辑 `src/BTCmd.cpp`（注意 `AUTH_PASSKEY`, MTU 与 `BLESecurity` 设置），并更新 `include/BTCmd.h` 的常量。
- 实现舵机动作：`src/servoCmd.cpp` 目前是 stub，实际逻辑（PWM、BJT 电源控制）应遵循 `servoSetPower` / `servoSetMode` 的签名并在头文件中声明的语义。

## 集成点与注意事项（针对 AI 代理）
- 未定义/散布的硬件宏：代码中会引用像 `BOOT_PIN` 这样的硬件宏（见 `src/BTCmd.cpp`）。在修改前，先在仓库内全局搜索宏定义或在提交说明中提醒开发者补充具体引脚定义（可能在 `config.h` 或 board-specific header 中）。
- 全局变量：`wifiEventGroup` 在 `include/main.h` 中声明为全局。对其使用必须小心（避免重复创建），并确保在 `systemInit()` 中创建成功。
- 持久化/配对数据：`BTCmd.cpp` 会调用 `nvs_flash_erase()` / `nvs_flash_init()` 来清空配对缓存——如果你的改动影响配对行为，请在提交说明中说明会清除 NVS 的副作用。

## 质量与验证建议（轻量、可执行）
- 编译验证：每次改动后执行 `pio run -e airm2m_core_esp32c3` 并修复编译错误。
- 运行验证：在设备上通过串口日志（115200）观察 `System Initialization Successful!`、`WiFi Connected!`、`BLE Server is running!` 等关键日志。

## 变更提交模板（建议在 PR 描述中包含）
- 改动范围（文件/模块）：例如 `src/WiFiCmd.cpp`、`include/config.h`
- 运行验证步骤：编译命令、是否在硬件上验证、关键串口日志行
- 影响边界：是否会清除 NVS、是否更改分区表或板级设置

如果有不清楚的硬件定义（例如 `BOOT_PIN`，其他 IO 映射）或需要访问硬件原理图，请指出，我会把这些点列为需要人工确认的 TODO。

---
请审阅本说明是否覆盖你期望的使用场景（特别是硬件宏/板定义和上传端口流程），我可以据此迭代更新文档。 
