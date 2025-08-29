# Robot Controller System
## Arduino Mega + Xbox 360 Controller

ระบบควบคุมหุ่นยนต์รถโดยใช้ Arduino Mega และจอยสติก Xbox 360 Controller

### ภาพรวมโครงการ (Project Overview)

โครงการนี้เป็นระบบควบคุมหุ่นยนต์รถที่ใช้:
- **Arduino Mega 2560** เป็นตัวควบคุมหลัก
- **Xbox 360 Wireless Controller** สำหรับการควบคุม
- **USB Host Shield 2.0** สำหรับเชื่อมต่อจอยสติก
- **L298N Motor Driver** สำหรับควบคุมมอเตอร์
- **DC Motors** สำหรับการเคลื่อนที่

### คุณสมบัติหลัก (Key Features)

✅ **การควบคุมแบบ Tank Drive**
- ใช้ Left Stick สำหรับเคลื่อนที่ (หน้า-หลัง, ซ้าย-ขวา)
- ใช้ Right Stick สำหรับการหมุนตัว

✅ **ระบบ PWM Speed Control ที่ปรับปรุงแล้ว**
- PWM มีการจำกัดค่าในช่วง 0-255 อย่างปลอดภัย
- ค่า PWM ขั้นต่ำสำหรับการเริ่มต้นมอเตอร์ (50)
- ฟังก์ชันควบคุมความเร็วแบบค่อยเป็นค่อยไป
- ตัวเลือกปรับความถี่ PWM สำหรับการทำงานที่เงียบขึ้น

✅ **ระบบความปลอดภัย**
- Emergency Stop (ปุ่ม Back)
- Enable/Disable Robot (ปุ่ม Start)
- Quick Stop (ปุ่ม B)

✅ **การควบคุมความเร็ว**
- Speed Boost (ปุ่ม RT - Right Trigger)
- Precision Mode (ปุ่ม LT - Left Trigger)
- Deadzone สำหรับป้องกัน drift

✅ **ฟังก์ชันพิเศษ**
- Preset Movements (D-pad buttons)
- Status Indicators (LEDs)
- Serial Monitor สำหรับ debugging

### ไฟล์ในโครงการ (Project Files)

1. **`robot_controller.ino`** - โค้ดหลักสำหรับควบคุมหุ่นยนต์ (ปรับปรุง PWM แล้ว)
2. **`motor_control_examples.ino`** - ตัวอย่างการควบคุมมอเตอร์และการทดสอบ
3. **`pwm_test.ino`** - สคริปต์ทดสอบ PWM Speed Controller
4. **`WIRING_DIAGRAM.md`** - คู่มือการเชื่อมต่อสายไฟ
5. **`libraries_setup.md`** - คู่มือการติดตั้ง Library

### การติดตั้งและใช้งาน (Installation & Usage)

#### 1. ติดตั้ง Library
```
Arduino IDE → Tools → Manage Libraries
ค้นหาและติดตั้ง: "USB Host Shield Library 2.0"
```

#### 2. การเชื่อมต่อฮาร์ดแวร์
ดูรายละเอียดใน `WIRING_DIAGRAM.md`

#### 3. อัปโหลดโค้ด
1. เปิดไฟล์ `robot_controller.ino`
2. เลือก Board: Arduino Mega 2560
3. เลือก Port ที่ถูกต้อง
4. กดปุ่ม Upload

#### 4. การใช้งาน Xbox Controller

**ปุ่มควบคุมหลัก:**
- **Left Stick**: เคลื่อนที่ (หน้า-หลัง, ซ้าย-ขวา)
- **Right Stick**: หมุนตัว (ซ้าย-ขวา)
- **Start**: เปิด/ปิดระบบหุ่นยนต์
- **Back**: Emergency Stop
- **B**: หยุดฉับพลัน

**ปุ่มเสริม:**
- **RT (Right Trigger)**: เพิ่มความเร็ว
- **LT (Left Trigger)**: โหมดแม่นยำ (ช้า)
- **D-pad**: การเคลื่อนที่แบบกำหนด
- **A**: แตร/เสียงเตือน
- **Y**: แสดงสถานะ

### การทดสอบระบบ (System Testing)

#### 1. ทดสอบ PWM Speed Controller
```cpp
// ใช้ไฟล์ pwm_test.ino สำหรับทดสอบ PWM
// เปิด Serial Monitor ที่ 115200 baud
// ระบบจะทดสอบการจำกัดค่า PWM และการควบคุมความเร็วอัตโนมัติ
```

#### 2. ทดสอบการเชื่อมต่อ
```cpp
// ใช้ไฟล์ motor_control_examples.ino
// เปิด Serial Monitor ที่ 115200 baud
// ส่งคำสั่ง 1-8 เพื่อทดสอบ
```

#### 2. ทดสอบมอเตอร์
- `1`: เดินหน้า
- `2`: ถอยหลัง  
- `3`: เลี้ยวซ้าย
- `4`: เลี้ยวขวา
- `0`: หยุด

#### 3. ทดสอบ Xbox Controller
1. เชื่อมต่อ Xbox 360 Wireless Receiver
2. กดปุ่ม Sync บน Receiver และ Controller
3. ตรวจสอบ LED สถานะ

### การแก้ไขปัญหา (Troubleshooting)

**มอเตอร์ไม่หมุน:**
- ตรวจสอบการเชื่อมต่อไฟ 12V
- ตรวจสอบสายสัญญาณไปยัง L298N
- ทดสอบมอเตอร์โดยตรง

**Controller ไม่เชื่อมต่อ:**
- ตรวจสอบ USB Host Shield
- กดปุ่ม Sync ใหม่
- ตรวจสอบแบตเตอรี่ Controller

**หุ่นยนต์เดินไม่ตรง:**
- ปรับค่าความเร็วมอเตอร์ในโค้ด
- ตรวจสอบการติดตั้งล้อ
- ใช้ฟังก์ชัน calibrateMotors()

### ข้อมูลทางเทคนิค (Technical Specifications)

- **Microcontroller**: Arduino Mega 2560 (ATmega2560)
- **Operating Voltage**: 5V (Logic), 12V (Motors)
- **Motor Driver**: L298N (2A per channel)
- **Communication**: USB 2.0 (Xbox Controller)
- **PWM Frequency**: ~490Hz (Arduino default)
- **Control Range**: ±255 (8-bit PWM)

### ความปลอดภัย (Safety Notes)

⚠️ **คำเตือน:**
- ใช้ฟิวส์ป้องกันไฟฟ้าลัดวงจร
- ตรวจสอบขั้วไฟฟ้าก่อนเชื่อมต่อ
- ทดสอบด้วยแรงดันต่ำก่อน
- ใช้ Emergency Stop เมื่อจำเป็น

### การพัฒนาต่อ (Future Enhancements)

🚀 **ฟีเจอร์ที่สามารถเพิ่มได้:**
- เซ็นเซอร์หลีกหลีกสิ่งกีดขวาง
- กล้องสำหรับ FPV
- ระบบ GPS Navigation
- การควบคุมผ่าน WiFi
- แขนหุ่นยนต์

### ผู้พัฒนา (Developer)

ระบบนี้พัฒนาสำหรับการเรียนรู้และทดลองด้วย Arduino และการควบคุมหุ่นยนต์

### License

Open Source - ใช้และแก้ไขได้อย่างอิสระ

---

**หมายเหตุ:** อ่านคู่มือการเชื่อมต่อใน `WIRING_DIAGRAM.md` ก่อนเริ่มประกอบ