# Automatic School Bell Controller

This project is an **Automatic School Bell System** built using **Arduino Uno, DS3231/DS3232 RTC module, LCD, EEPROM, relay, buzzer, and push buttons**.  
It allows schools to ring the bell automatically at pre-programmed times without manual intervention.

The system can be simulated in **Proteus 8 Professional** and also implemented in real hardware.

---

## 📌 Features
- **Automatic Bell Ringing** at predefined times using a real-time clock (RTC).
- **Relay Control** for driving an electric bell or lamp.
- **LCD Display (16x2)** to show current date, time, and bell status.
- **Menu System with Push Buttons** to set time and bell schedules:
  - `SET` → enter/exit setting mode  
  - `NEXT` → move between fields  
  - `UP` / `DOWN` → adjust values  
- **EEPROM Storage** so settings are saved even after reset/power off.
- **Buzzer Indication** for short alerts and feedback.
- **Proteus Simulation** available for testing before hardware implementation.

---

## 🖼️ Circuit Diagram
The project uses the following connections (see Proteus schematic):

- **Arduino UNO** as main controller
- **RTC DS3232 / DS3231** connected via I²C (SDA → A4, SCL → A5)
- **LCD 16x2** in 4-bit mode (pins D4–D7)
- **Push buttons** connected to A0–A3 (`SET`, `NEXT`, `UP`, `DOWN`)
- **Relay driver circuit** with transistor + diode protection for bell/lamp
- **Buzzer** connected to pin 13
- **Relay** controls an AC lamp or electric bell

---

## ⚙️ Hardware Components
- Arduino Uno (ATmega328P)
- RTC Module (DS3231 or DS3232)
- LCD 16x2 Display + 10k potentiometer
- 4 Push buttons
- Buzzer
- Relay Module + driver circuit
- Resistors, diode, transistor (Q2 = MPS6562 or equivalent)
- AC Load (Lamp/Bell)

---

## 🔧 Software
- **Arduino IDE** for programming
- **Proteus 8** for simulation

### Required Libraries
- `LiquidCrystal.h`
- `Wire.h` (I²C)
- `EEPROM.h`
- `RTClib.h` (for DS3231/DS3232)

Install missing libraries from Arduino Library Manager.

---

## 🚀 How It Works
1. The **RTC** keeps track of the current time.
2. The **LCD** continuously displays current **day, time, and bell status**.
3. When the current time matches a scheduled bell time:
   - The **relay is activated**, ringing the bell (or turning on a lamp).
   - The **LCD shows "Bell On"** with the duration.
4. The **buttons** allow the user to set the clock and schedule.
5. The **EEPROM** stores schedules, so they remain after restart.

---

## ▶️ Running the Project
### 1. Simulation in Proteus
- Open the provided Proteus file.  
- Load the compiled `.hex` file from Arduino into the UNO.  
- Run the simulation to see the LCD update and relay activation.

### 2. On Hardware
```bash
# Compile and upload the Arduino sketch
Arduino IDE → Select Board: Arduino Uno → Upload
