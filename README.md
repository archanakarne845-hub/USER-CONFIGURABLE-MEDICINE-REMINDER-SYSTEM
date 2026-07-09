
# 💊 Medicine Reminder System using LPC2148 ARM7

<p align="center">

![Embedded C](https://img.shields.io/badge/Language-Embedded%20C-blue)
![ARM7](https://img.shields.io/badge/Microcontroller-LPC2148-green)
![RTC](https://img.shields.io/badge/RTC-Internal-orange)
![Status](https://img.shields.io/badge/Status-Completed-success)

</p>

---

# 📖 Overview

The **Medicine Reminder System using LPC2148 ARM7** is an embedded healthcare application that reminds users to take medicines at scheduled times.

The system uses the **internal Real-Time Clock (RTC)** of the LPC2148 to continuously monitor the current date and time. Users can configure up to **three medicine schedules** using a **4×4 matrix keypad**. When the current time matches a stored schedule, the system displays a reminder on a **16×2 LCD** and activates a **buzzer**.

Configuration mode is entered using **External Interrupt 0 (EINT0)**, while **External Interrupt 1 (EINT1)** is used to acknowledge the reminder and stop the buzzer.

---

# 🎯 Project Objectives

- Display current date and time using RTC
- Allow users to edit RTC settings
- Configure three medicine schedules
- Compare RTC time with medicine timings
- Generate medicine reminders
- Stop reminder using EINT1
- Automatically stop reminder after one minute
- Provide a keypad-driven user interface

---

# ✨ Features

- Internal RTC support
- LCD date and time display
- 4×4 Matrix Keypad interface
- Three configurable medicine schedules
- External Interrupt-based configuration
- External Interrupt-based reminder acknowledgment
- Automatic reminder timeout
- Embedded C firmware
- ARM7 LPC2148 implementation

---

# 🏗️ System Architecture

<p align="center">

<img src="images/system architecture.png" width="800"/>

</p>

---

# 📦 Block Diagram

<p align="center">

<img src="images/bolck diagram.png" width="750"/>

</p>

---

# ⚙️ Hardware Components

| Component | Description |
|------------|-------------|
| LPC2148 ARM7 | Main Controller |
| Internal RTC | Real-Time Clock |
| 16×2 LCD | Display |
| 4×4 Matrix Keypad | User Input |
| Buzzer | Reminder Alert |
| Push Button (EINT0) | Configuration Mode |
| Push Button (EINT1) | Reminder Acknowledgment |

---

# 💻 Software Requirements

- Embedded C
- Keil μVision
- Flash Magic

---

# 📌 Pin Configuration

| Peripheral | LPC2148 Pin |
|------------|-------------|
| LCD Data | P0.8 – P0.15 |
| LCD RS | P0.16 |
| LCD RW | P0.17 |
| LCD EN | P0.18 |
| Buzzer | P0.0 |
| EINT0 | P0.1 |
| EINT1 | P0.3 |
| Keypad Rows | P1.16 – P1.19 |
| Keypad Columns | P1.20 – P1.23 |

---

# 🔄 Working Principle

1. Initialize LCD, RTC, Keypad, and Interrupts.
2. Display current date and time.
3. Press EINT0 to enter Configuration Mode.
4. Configure RTC or medicine schedules.
5. Store medicine timings.
6. Continuously compare RTC time with stored schedules.
7. If a match occurs:
   - Display "Take Medicine Now"
   - Turn ON buzzer
   - Wait for EINT1 acknowledgment
8. If acknowledged, stop the reminder.
9. Otherwise, stop automatically after one minute.
10. Continue monitoring.

---

# 🕒 RTC Configuration

Users can edit:

- Hour
- Minute
- Second
- Day
- Date
- Month
- Year

---

# 💊 Medicine Schedule

The system supports three independent medicine schedules.

Example:

| Medicine | Time |
|----------|------|
| Medicine 1 | 08:30 |
| Medicine 2 | 14:00 |
| Medicine 3 | 21:15 |

---
# 🔔Alert configuration
<p align="center">

<img src="images/medicine alert.png" width="800"/>

</p>

# 🔔 Reminder Process

<p align="center">

<img src="images/reminder process.png" width="750"/>

</p>


---

# 🔄 Software Flow
<p align="center">

<img src="images/software workflow.png" width="750"/>

</p>

---

# 📂 Project Structure

```text
Medicine-Reminder-System-using-LPC2148
│
├── README.md
├── Source_Code
│   ├── main.c
│   ├── lcd.c
│   ├── rtc.c
│   └── kpm.c
│
├── Header_Files
│   ├── LCD.h
│   ├── rtc_main.h
│   ├── lcd_defines.h
│   └── kpm_defines.h
│
├── Images
│   ├── system_architecture.png
│   ├── block_diagram.png
│   ├── hardware_setup.jpg
│   ├── lcd_output.jpg
│   └── flowchart.png
│
└── Documents
    └── Project_Report.pdf
```

---

# 📸 Project Gallery

| Hardware | LCD Output |
|-----------|------------|
| ![](images/hardware_setup.jpg) | ![](images/lcd_output.jpg) |
<p align="center">

<img src="images/image1.png" width="750"/>

</p>

<p align="left">

<img src="images/image2 (2).png" width="750"/>

</p>

<p align="left">

<img src="images/image3.png" width="750"/>

</p>

<p align="left">

<img src="images/image4.png" width="750"/>

</p>

<p align="left">

<img src="images/image5.png" width="750"/>

</p>

<p align="left">

<img src="images/image6.png" width="750"/>

</p>

---

# 🚀 Future Enhancements

- GSM-based SMS reminders
- IoT monitoring
- Mobile application
- EEPROM storage
- Voice alerts
- Wi-Fi connectivity
- Multiple user support

---

# 🌍 Applications

- Home healthcare
- Medicine reminder devices
- Elderly patient care
- Hospital monitoring
- Embedded healthcare systems

---

# 👩‍💻 Author

**Karne Archana**

Bachelor of Technology (Electronics and Communication Engineering)

Embedded Systems | Embedded C | ARM7 LPC2148 | Embedded Linux

---

# 📄 License

This project is intended for educational and learning purposes.

---

⭐ If you found this project useful, consider giving it a **Star** on GitHub!
