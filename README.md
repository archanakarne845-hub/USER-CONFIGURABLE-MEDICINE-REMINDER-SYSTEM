💊 User-Configurable Medicine Reminder System using LPC2148 ARM7

⭐ Overview

The User-Configurable Medicine Reminder System is an embedded healthcare application developed using the LPC2148 ARM7 Microcontroller. The system allows users to configure up to three medicine schedules through a 4×4 Matrix Keypad while continuously monitoring the internal Real-Time Clock (RTC).

The current date and time are displayed on a 16×2 LCD. Whenever the RTC matches a stored medicine schedule, the controller displays "Take Medicine Now" on the LCD and activates a buzzer. The user can acknowledge the reminder by pressing a switch connected to External Interrupt 1 (EINT1). If no acknowledgment is received within one minute, the reminder automatically stops and the system resumes normal RTC monitoring.

The configuration mode is entered through External Interrupt 0 (EINT0), allowing the user to modify the RTC date, time, day, and medicine schedules without stopping the system.

🎯 Project Objectives

Display real-time date and time using the LPC2148 RTC.
Allow users to edit RTC time, date, and day.
Configure up to three medicine reminder schedules.
Continuously compare RTC time with stored medicine timings.
Generate medicine reminders using LCD and buzzer.
Allow medicine acknowledgment through EINT1.
Automatically stop reminders after one minute if not acknowledged.
Provide a simple keypad-based user interface.
Demonstrate interrupt-driven embedded system design.
