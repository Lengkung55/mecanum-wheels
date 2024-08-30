// Define Input Connections
#define CH1 A0
#define CH2 A1
#define CH3 A2
#define CH4 A3
#define CH5 A4
#define CH6 A5
#define PWM_left 3
#define IN1 4
#define IN2 5
#define IN3 8
#define IN4 9
#define PWM_right 10

// Integers to represent values from sticks and pots
int ch1Value;
int ch2Value;
int ch3Value;
int ch4Value;
int ch5Value;

// Boolean to represent switch value
bool ch6Value;

// Read the number of a specified channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 50000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the switch channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup() {
  // Set up serial monitor
  Serial.begin(115200);

  // FLYSKY Remote control
  // Set all pins as inputs
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);

  //Motor Input
  pinMode(PWM_left, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWM_right, OUTPUT);
}


void loop() {
  remote();
  int motor_speed = ch5Value;
  int motor_direction = ch1Value;
  int motor_front_back = ch2Value;
  int servo_enable = ch6Value;
  int servo_angle = ch3Value;

  motor_control(motor_speed, motor_direction, motor_front_back);
  servo_back(servo_enable, servo_angle);
}


void remote() {
  // Get values for each channel
  // LEFT RIGHT
  ch1Value = readChannel(CH1, -255, 255, 0);

  // FRONT BACK
  ch2Value = readChannel(CH2, -255, 255, 0);

  // Servo angle
  ch3Value = readChannel(CH3, -100, 100, 0);

  // DID NOT USE
  ch4Value = readChannel(CH4, -100, 100, 0);

  // SPEED KNOB
  ch5Value = readChannel(CH5, 0, 255, 0);

  // Servo Enable
  ch6Value = readSwitch(CH6, false);

  // Print to Serial Monitor
  // Serial.print("Ch1: ");
  // Serial.print(ch1Value);
  // Serial.print(" | Ch2: ");
  // Serial.print(ch2Value);
  // Serial.print(" | Ch3: ");
  // Serial.print(ch3Value);
  // Serial.print(" | Ch4: ");
  // Serial.print(ch4Value);
  // Serial.print(" | Ch5: ");
  // Serial.print(ch5Value);
  // Serial.print(" | Ch6: ");
  // Serial.println(ch6Value);

  delay(2);
}


void motor_control(int speed, int direction, int front_back) {
  Serial.print(speed);
  Serial.print(" ");
  Serial.print(direction);
  Serial.print(" ");
  Serial.print(front_back);
  Serial.print(" ");
  Serial.println("MOTOR_CONTROL");

  forward_backward(speed,direction);
}


void servo_back(int enable, int angle) {
  Serial.print(enable);
  Serial.print(" ");
  Serial.print(angle);
  Serial.print(" ");
  Serial.println("SERVO_BACK");
}

void forward_backward(int speed, int direction) {
  // SPEED OF MOTORS
  analogWrite(PWM_left, abs(speed));
  analogWrite(PWM_right, abs(speed));
  
  //DIRECTION
  if (direction < 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
}
