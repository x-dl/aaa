
#include <dht11.h>

#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 设置LCD1602的地址
const int lcdAddr = 0x27; // 默认I2C地址，如果不生效，可能需要调整

// 设置LCD1602的列数和行数
const int lcdCols = 16;
const int lcdRows = 2;

// 创建LiquidCrystal_I2C对象
LiquidCrystal_I2C lcd(lcdAddr, lcdCols, lcdRows);
#define DHT11PIN 2
// 定义软串口引脚
const int rxPin = A0;
const int txPin = A1;
dht11 DHT11;
SoftwareSerial mySerial(rxPin, txPin);
const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;
const int trigPin = 9; // Trig引脚连接到数字引脚9
const int echoPin = 10; // Echo引脚连接到数字引脚10
const int ledPin1 = 13; // 板载LED连接在引脚13
long duration;
float distance = 0.0 ;
int motorSpeedA = 255; // 0-255，255为全速
int motorSpeedB = 255; // 0-255，255为全速
int DHT11_read(){
    DHT11.read(DHT11PIN);
//    Serial.print("Humidity (%): ");
//    Serial.println((float)DHT11.humidity, 2);
//    Serial.print("Temperature (oC): ");
//    Serial.println((float)DHT11.temperature, 2);
 }
int LCD_show(){
//    lcd.setCursor(0, 0);
//  lcd.print("Hello,");
//  lcd.setCursor(0, 1);
//  lcd.print("Arduino!");
  lcd.setCursor(0,0);
  String str = "humid:";
  str += DHT11.humidity;
  str+= "kg/m^3";
  lcd.print(str.c_str());
  lcd.setCursor(0, 1);
  str = "temp:";
  str += DHT11.temperature;
  str+="oC";
  lcd.print(str.c_str());
}
void forward() {
        digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

}

void backward() {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
}

void turnLeft() {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
}

void turnRight() {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

}

void stop() {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
}
//void loopDHT11()
//{
//  Serial.println("\n");
//  DHT11_read();
//  delay(2000);
//}
void setupDHT11()
{
  Serial.begin(9600);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
}
void setupLCD() {
   Wire.begin();
  // 初始化LCD
  lcd.init();
  // 打开LCD背光
  lcd.backlight();
  // 在LCD上显示初始消息

}

void loopLCD() {
  DHT11_read();
  LCD_show();
  //delay(500);
}
void setupMotor() {
  // 设置电机控制引脚为输出模式
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loopMotor() {
      forward();  // 前进
      delay(2000); // 延时2秒
      backward(); // 后退
      delay(2000); // 延时2秒
      turnLeft(); // 左转
      delay(2000); // 延时2秒
      turnRight(); // 右转
      delay(2000); // 延时2秒
      stop();  // 停止
      delay(2000); // 延时2秒
}
void setupUltra() {

  // 设置Trig引脚为输出
  pinMode(trigPin, OUTPUT);

  // 设置Echo引脚为输入
  pinMode(echoPin, INPUT);
}
char rev = 's'; //f b l r s
void loopUltra() {
  // 清空Trig引脚
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // 发送10微秒的高电平脉冲
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 读取Echo引脚的高电平持续时间（单位：微秒）
  duration = pulseIn(echoPin, HIGH);

  // 计算距离（单位：厘米）
  distance = duration * 0.034 / 2;
  // 打印距离到串口监视器
//  Serial.print("Distance: ");
//  Serial.print((float)distance, 2);
//  Serial.println(" cm");

  // 等待一段时间
  //delay(1000); // 延时1秒
}
void setup() {
  // 初始化硬件串口用于调试
  pinMode(ledPin1, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; // 等待串口连接
  }
  Serial.println("Arduino Serial Started");
  setupLCD();
  setupMotor();
  setupUltra();
  // 初始化软串口
  mySerial.begin(9600);
 
}

void loopMove(){
  
    switch (rev) {
            case 'f':
                forward();
                break;
            case 'b':
                backward(); // 后退
                break;
            case 'l':
                turnLeft(); // 左转
                break;
            case 'r':
                turnRight(); // 右转
                break;
            default:
                stop();  // 停止
        }
}
unsigned long previousMillis = 0;  // 保存上一次执行代码的时间
const long interval = 1000;  // 执行代码的间隔（毫秒）
void loop() {
   

  // 检查是否有来自 ESP32 的数据
  if (mySerial.available()) {
    rev= mySerial.read();
    Serial.print("arduinoCar: ");
    Serial.println(rev);   
    loopMove();
    delay(9);
    stop();
    delay(1);
  }
  loopLCD();
}
