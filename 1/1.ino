
#include <SPI.h>

//这是没有悬挂物体的初始值 
const float Origin = 350;

// 定义引脚
const int CS_PIN = 10;    
const int CS2_PIN = 2;
const int WR_PIN = 3;
const int DATA_PIN = 4;
const int VCC_PIN = 6;
const int LED_PIN = 5;

const int STACK_SIZE = 100;
float angleStack[STACK_SIZE];
int stackTop = -1;

void setup() {
  pinMode(CS2_PIN, OUTPUT);
  pinMode(WR_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(VCC_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // 启动供电和背光
  digitalWrite(VCC_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);

  // 初始化显示仪（假设初始化时需要CS和WR置高）
  digitalWrite(CS2_PIN, HIGH);
  digitalWrite(WR_PIN, HIGH);

  Serial.begin(115200);
  
  // 初始化CS引脚
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  
  // 初始化SPI
  SPI.begin();
  
  // 设置SPI模式和时钟分频器
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1)); // 设置SPI: 1MHz时钟, MSB先传, 模式1
}

void sendData(byte data) {
  // 选择显示仪
  digitalWrite(CS2_PIN, LOW);

  // 准备写入数据
  digitalWrite(WR_PIN, LOW);
  
  // 发送数据
  shiftOut(DATA_PIN, WR_PIN, MSBFIRST, data);

  // 完成写入
  digitalWrite(WR_PIN, HIGH);
  digitalWrite(CS2_PIN, HIGH);
}

uint16_t readAS5047P() {
  uint16_t command = 0x3FFF; // 读角度的命令
  uint16_t result = 0;

  // 开始SPI通信
  digitalWrite(CS_PIN, LOW);
  
  // 发送命令并读取高字节
  result = SPI.transfer16(command);

  // 结束SPI通信
  digitalWrite(CS_PIN, HIGH);

  return result & 0x3FFF; // 提取角度数据（14位）
}

float convertToDegrees(uint16_t rawValue) {
  return (rawValue * 360.0) / 16384.0; // 将14位值转换为度数
}

float convertToAbsoluteDegree(float relative_angle){
  if(relative_angle > 350){
    return (relative_angle - Origin);
  }
  else{
    return (10 + relative_angle);
  }
}

void push(float value) {
  if (stackTop < STACK_SIZE - 1) {
    stackTop++;
    angleStack[stackTop] = value;
  } else {
    for (int i = 0; i < STACK_SIZE - 1; i++) {
      angleStack[i] = angleStack[i + 1];
    }
    angleStack[STACK_SIZE - 1] = value;
  }
}

float calculateAverage(float* stack, int size) {
  float sum = 0;
  for (int i = 0; i <= stackTop; i++) {
    sum += stack[i];
  }
  return sum / (stackTop + 1);
}
double pow(double x, int n)  
{  
    if(n==0)  
        return 1.0;  
    if(n<0)  
        return 1.0/pow(x,-n);  
    return x*pow(x,n-1);  
}  
void loop() {
  uint16_t rawAngle = readAS5047P();
  float angle = convertToDegrees(rawAngle);
  
  // 将新的角度值压入栈
  push(angle);
  
  // 计算均值
  float averageAngle = calculateAverage(angleStack, STACK_SIZE);
  float x;
  x=250;
//  Serial.print("Angle: ");
  float relative_angle = convertToAbsoluteDegree(averageAngle);
  Serial.print(averageAngle, 10); // 打印角度，精确到小数点后4位
  Serial.print(" ");
  Serial.print(relative_angle, 10); // 打印相对角度，精确到小数点后4位
  Serial.print(" ");
  //拟合的角度
//  float mass = 0.0018 * relative_angle*relative_angle*relative_angle + (-0.1754) * relative_angle*relative_angle +  8.2051 * relative_angle - 4.1904;

//  float mass = -0.2136 * relative_angle * relative_angle +   12.2974 * relative_angle  - 57.6033;
  float mass = 1000* ( 0.0001 * relative_angle * relative_angle * relative_angle + ( -0.0093) * relative_angle * relative_angle +  0.2055  * relative_angle  -1.4225);
  Serial.print(mass,10); //打印质量
  Serial.print("\n");
//  x=(x*x*x*x*(-0.00002)+0.0247*x*x*x-9.1759*x*x+1518*x-94039);
//  Serial.println(x);

  char dataToSend = '2';
  sendData(dataToSend);
  
  delay(50); // 延迟以便于读取
}
