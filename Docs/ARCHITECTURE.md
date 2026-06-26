# Flight Controller Architecture

## Hardware

### Flight Controller
- STM32F411CEU6 Black Pill

### Sensors
- MPU6500 IMU
- NEO-6M GPS

### Communication
- AS32 TTL-1W LoRa
- ESP32 Ground Station
- PS5 Controller

---

## Flight Stack Modules

### Drivers
- MPU6500 Driver
- GPS Driver
- LoRa Driver

### Core Modules
- Scheduler
- Receiver
- Attitude Estimation
- PID Controller
- Mixer
- Failsafe

---

## Main Flight Loop

```text
Receiver Update
↓
Read IMU
↓
Apply Calibration
↓
Attitude Estimation
↓
PID Controller
↓
Mixer
↓
PWM Output
↓
Telemetry
```

---

## Communication Flow

```text
PS5 Controller
↓ Bluetooth
ESP32 Ground Station
↓ UART
AS32 LoRa TX
↓ LoRa Link
AS32 LoRa RX
↓ UART
STM32 Flight Controller
```

---

## Future Features

- GPS Hold
- Return To Home (RTH)
- Altitude Hold
- Blackbox Logging