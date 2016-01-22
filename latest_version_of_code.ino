#include <Servo.h>
Servo Front_right_servo;
Servo Back_right_servo;
Servo Front_left_servo;
Servo Back_left_servo;

int rc_1_radio_min = 1137;    //sets up the minimum value from receiver
int rc_2_radio_min = 1137;    // make sure the calibration is at max/upwards
int rc_3_radio_min = 1137;
int rc_4_radio_min = 1137;
int rc_1_radio_max = 1950;    //make sure the calibration is at the rightmost
int rc_2_radio_max = 1950;
int rc_3_radio_max = 1950;
int rc_4_radio_max = 1950;

// 3 is not trimmed
int rc_1_radio_trim = 1464;   //+/- 4      //setup neutral value
int rc_2_radio_trim = 1540;   //+/- 4
int rc_3_radio_trim = 1459;
int rc_4_radio_trim = 1455;   // +/- 3

int motor_max = 2000; // Motor Setup
int motor_min = 1000;
int motor_trim = 1500;

int serial_max = 255;
int serial_min = 0;
int serial_trim = 127;

// assigns pin numbers for rc
int RC1 = 2; // left and right
int RC2 = 3; // Manual throttle
int RC3 = 4; //mode
int RC4 = 5;
int A_LED = 13;



int Motor1 = 6;   // front right
int Motor2 = 7;   // front left
int Motor3 = 8;   // back right
int Motor4 = 9;   // back left



int twist_y = 0; //autonomous throttle
int twist_z = 0; //autonomous rotation

int Otwist_y = 0; // old throttle
int Otwist_z = 0; // old rotation

int front_right_motor_cal = 1; // needs to be modified later after testing
int front_left_motor_cal = 1;
int back_right_motor_cal = 1;
int back_left_motor_cal = 1;

unsigned long duration_rc1;
unsigned long duration_rc2;
unsigned long duration_rc3;
unsigned long duration_rc4;

int LEDcount = 0; //counting how many loops went through in autonomous mode

int rc1_value = 0;
int rc2_value = 0;
int rc3_value = 0;
int rc4_value = 0;


boolean first_time = true;
//boolean gradian = false;

int motor1value = 90;
int motor2value = 90;
int motor3value = 90;
int motor4value = 90;

int old_motor1value;
int old_motor2value;
int old_motor3value;
int old_motor4value;


void setup()

{

  Front_right_servo.attach(Motor1);
  Front_left_servo.attach(Motor2);
  Back_right_servo.attach(Motor3);
  Back_left_servo.attach(Motor4);
 
  pinMode(RC1, INPUT); //receive input from pin 13
  pinMode(RC2, INPUT);
  pinMode(RC3, INPUT);
  pinMode(RC4, INPUT);

  pinMode(A_LED, OUTPUT); //send output to A_LED


  Serial.begin(9600);

}



void loop()

{

  /* analogWrite(Motor1,240);
  */

  move_pwm();
  //duration_rc1 =pulseIn(RC1,HIGH);
  /*
    Serial.print("rc1 pulse  ");
    Serial.println(duration_rc1);
    Serial.print("rc1 after  ");// printing a word
    Serial.println(rc1_value);// printing an integer
    Serial.print("rc2 pulse  ");
    Serial.println(duration_rc2);
    Serial.print("rc2 after  ");// printing a word
    Serial.println(rc2_value);// printing an integer
    Serial.print("\n");
  */
  /*
    Serial.print("twisty ");
    Serial.println(twist_y);
    Serial.print("twistz ");
    Serial.println(twist_z);

    Serial.print("rc3 after");// printing a word
    Serial.println(rc3_value);// printing an integer
  */
  /*
      Serial.print("rc1 before");// printing a word
      Serial.println(duration_rc1);// printing an integer

      Serial.print("rc2 before");// printing a word
      Serial.println(duration_rc2);// printing an integer

      Serial.print("rc3 before");// printing a word
      Serial.println(duration_rc3);// printing an integer
  */


}


void move_pwm() {

  if (!first_time) {
    old_motor1value = motor1value;
    old_motor2value = motor2value;
    old_motor3value = motor3value;
    old_motor4value = motor4value;
  }
  

  uint16_t wheels[4];                         //an array of 16 bits unsigned integers
  duration_rc1 = pulseIn(RC1, HIGH);
  duration_rc2 = pulseIn(RC2, HIGH);
  duration_rc3 = pulseIn(RC3, HIGH);
  duration_rc4 = pulseIn(RC4, HIGH);

  /* Serial.print("pulse ");// printing a word
    Serial.println(duration_rc3);// printing an integer
    Serial.print("\n");*/

  if (duration_rc1 < 1600 && duration_rc1 > 1400) {
    duration_rc1 = rc_1_radio_trim;
  }
  else if (duration_rc1 > 1940) {
    duration_rc1 = rc_1_radio_max;
  }

  if (duration_rc2 < 1600 && duration_rc2 > 1450) {
    duration_rc2 = rc_2_radio_trim;
  }
  else if (duration_rc2 > 1940) {
    duration_rc2 = rc_2_radio_max;
  }




  if (duration_rc1 >= rc_1_radio_trim && duration_rc1 <= rc_1_radio_max ) {
    rc1_value = map(duration_rc1, rc_1_radio_trim, rc_1_radio_max, 0, 500);
  }
  else if (duration_rc1 <= rc_1_radio_trim && duration_rc1  >= rc_1_radio_min) {
    rc1_value = map(duration_rc1, rc_1_radio_min, rc_1_radio_trim, -500, 0);
  }
  // rc1_value is assigned the value read from pin RC1 (Manual throttle)


  if (duration_rc2 >= rc_2_radio_trim && duration_rc2 <= rc_2_radio_max ) {
    rc2_value = map(duration_rc2, rc_2_radio_trim, rc_2_radio_max, 0, 500);
  }
  else if (duration_rc2 <= rc_2_radio_trim && duration_rc2  >= rc_2_radio_min) {
    rc2_value = map(duration_rc2, rc_2_radio_min, rc_2_radio_trim, -500, 0);
  }
  // rc2_value is assigned the value read from pin RC2 (Choose mode)

  if (duration_rc3 >= rc_3_radio_trim && duration_rc3 <= rc_3_radio_max ) {
    rc3_value = map(duration_rc3, rc_3_radio_min, rc_3_radio_max, 0, 500);
  }
  else if (duration_rc3 <= rc_3_radio_trim && duration_rc3  >= rc_3_radio_min) {
    rc3_value = map(duration_rc3, rc_3_radio_min, rc_3_radio_trim, -500, 0);
  }
  // rc3_value is assigned the value read from pin RC3 (Manual rotation)

  if (duration_rc4 >= rc_4_radio_trim && duration_rc4 <= rc_4_radio_max ) {
    rc4_value = map(duration_rc4, rc_4_radio_min, rc_4_radio_max, 0, 500);
  }
  else if (duration_rc4 <= rc_4_radio_trim && duration_rc4  >= rc_4_radio_min) {
    rc4_value = map(duration_rc4, rc_4_radio_min, rc_4_radio_trim, -500, 0);
  }
  // rc4_value is assigned the value read from pin RC4 (Unknown for now)


  if (rc3_value < 25 && rc3_value > -25) {
    rc3_value = 0;
  }
  if (rc4_value < 25 && rc4_value > -25) {
    rc4_value = 0;
  }



  //wireless estop if value from pin RC2 read is below 300
  if (rc3_value > -350 && rc3_value < 350) {  //stop
    digitalWrite(A_LED, LOW);
    wheels[0] = 1500; //front right wheel
    wheels[1] = 1500; //front left wheel
    wheels[2] = 1500; // rear right wheel
    wheels[3] = 1500; // rear left wheel
  }
  // manual control if value read from pin RC2 is above 300 or below 650
  else if ( rc3_value >= 350) {  // RC

    //each wheel is sent the result of the value for: “at rest” + (the rotation value - the throttle value) all multiplied by the respective motor calibration value.

    wheels[0] = 1500 + (rc1_value - rc2_value) * front_right_motor_cal;
    wheels[2] = 1500 + (rc1_value - rc2_value) * back_right_motor_cal;
    wheels[1] = 1500 + (rc1_value + rc2_value) * front_left_motor_cal ;
    //same computation as right motor, but different calibration value (gurjeet took out the +30)
    wheels[3] = 1500 + (rc1_value + rc2_value) * back_left_motor_cal; //may need to add further calibration such as the +30 for wheel[1]
    digitalWrite(A_LED, HIGH);

  }

  // autonomous mode if value read from pin RC2 is above 650
  else {
    //each wheel is sent the result of the value for : “at rest” + ( the autonomous rotation value - the autonomous throttle value) all multiplies by the respective motor calibration value.
    talk();
    wheels[0] = 1500 + (twist_z - twist_y) * front_right_motor_cal;
    wheels[2] = 1500 + (twist_z - twist_y) * back_right_motor_cal;
    wheels[1] = 1500 + (twist_z + twist_y) * front_left_motor_cal; //may need to add further calibration such as the +30 for wheel[1] in manual
    wheels[3] = 1500 + (twist_z + twist_y) * back_left_motor_cal;
    if (LEDcount < 20) {
      digitalWrite(A_LED, HIGH);
    }
    else {
      digitalWrite(A_LED, LOW);
      if (LEDcount > 40) {
        LEDcount = 0;
      }
    }
    LEDcount++;
  }

  Serial.print("wheel_2 ");
  Serial.print(wheels[2]);
  Serial.print("\n");
  //right wheels
  //front
  if (wheels[0] >= motor_trim && wheels[0] <= motor_max ) {
    motor1value = map(wheels[0], motor_trim, motor_max, 90, 170);
  }
  else if ( wheels[0] <= motor_trim && wheels[0]  >= motor_min) {
    motor1value = map(wheels[0], motor_min, motor_trim, 10, 90);
  }
  //back
  if (wheels[2] >= motor_trim && wheels[2] <= motor_max ) {
    motor3value = map(wheels[2], motor_trim, motor_max, 90, 170);
  }
  else if ( wheels[2] <= motor_trim && wheels[2]  >= motor_min) {
    motor3value = map(wheels[2], motor_min, motor_trim, 10, 90);
  }
  //left wheels
  //front
  if (wheels[1] >= motor_trim && wheels[1] <= motor_max ) {
    motor2value = map(wheels[1], motor_trim, motor_max, 90, 170);
  }
  else if ( wheels[1] <= motor_trim && wheels[1]  >= motor_min) {
    motor2value = map(wheels[1], motor_min, motor_trim, 10, 90);
  }
  //back
  if (wheels[3] >= motor_trim && wheels[3] <= motor_max ) {
    motor4value = map(wheels[3], motor_trim, motor_max, 90, 170);
  }
  else if ( wheels[3] <= motor_trim && wheels[3]  >= motor_min) {
    motor4value = map(wheels[3], motor_min, motor_trim, 10, 90);
  }
/*
    Front_right_servo.write(motor1value);
    Back_right_servo.write(motor3value);
    Front_left_servo.write(motor2value);
    Back_left_servo.write(motor4value);
    
*/
  if (!first_time) {
    int difference1 = old_motor1value - motor1value;
    int difference2 = old_motor2value - motor2value;
    int difference3 = old_motor3value - motor3value;
    int difference4 = old_motor4value - motor4value;

    if (difference1 > 45 ) {
      while (old_motor1value <= motor1value) {
        old_motor1value = old_motor1value - 1;
        Front_right_servo.write(old_motor1value);
      }
    }
    else if (difference1 < -45){
      while (old_motor1value >= motor1value) {
        old_motor1value = old_motor1value + 1;
        Front_right_servo.write(old_motor1value);
      }
    }
    else {
      Front_right_servo.write(motor1value);
    }

    if (difference3 > 45 ) {
      while (old_motor3value <= motor3value) {
        old_motor3value = old_motor3value - 1;
        Back_right_servo.write(old_motor3value);
      }
    }
    else if (difference3 < -45){
      while (old_motor3value >= motor3value) {
        old_motor3value = old_motor3value + 1;
        Back_right_servo.write(old_motor3value);
      }
    }
    else {
      Back_right_servo.write(motor3value);
    }

    if (difference2 > 45 ) {
      while (old_motor2value <= motor2value) {
        old_motor2value = old_motor2value - 1;
        Front_left_servo.write(old_motor2value);
      }
    }
    else if (difference2 < -45){
      while (old_motor2value >= motor2value) {
        old_motor2value = old_motor2value + 1;
        Front_left_servo.write(old_motor2value);
      }
    }
    else {
      Front_left_servo.write(motor2value);
    }

    if (difference4 > 45 ) {
      while (old_motor4value <= motor4value) {
        old_motor4value = old_motor4value - 1;
        Back_left_servo.write(old_motor4value);
      }
    }
    else if (difference4 < -45){
      while (old_motor4value >= motor4value) {
        old_motor4value = old_motor4value + 1;
         Back_left_servo.write(old_motor4value);
      }
    }
    else {
      Back_left_servo.write(motor4value);
    }

  }
  else {
  
    Front_right_servo.write(motor1value);
    Back_right_servo.write(motor3value);
    Front_left_servo.write(motor2value);
    Back_left_servo.write(motor4value);
    first_time = false;
  }
  
  
}





void talk() {

  char Byte[7];                              // used to receive values from serial
  int Bints[2];                                //used when chars is bitshifted into ints

  if (Serial.available() == 7) {         // if sent B, but no other numbers, garbage values sent

    // read the incoming byte:
    Serial.readBytes(Byte, 7);
    //Serial.print("Byte[]: ");
    for (int count = 0; count < 7; count++) {
      // Serial.readBytes(Byte, 7);
      Serial.print(Byte[count]);
      Serial.print("\n");
    }
  }
  else {
    Serial.readBytes(Byte, 7);
  }

  if (Byte[0] == 'B' && (Byte[6] == '1' || Byte[6] == '2' || Byte[6] == '3' || Byte[6] == '4' || Byte[6] == '5' || Byte[6] == '6' || Byte[6] == '7' || Byte[6] == '8' || Byte[6] == '9' || Byte[6] == '0')) {

    Bints[0] = 100 * (int)(Byte[1] - '0') + 10 * (int)(Byte[2] - '0') + (int)(Byte[3] - '0');
    Bints[1] = 100 * (int)(Byte[4] - '0') + 10 * (int)(Byte[5] - '0') + (int)(Byte[6] - '0');


    if (Bints[0] >= serial_trim && Bints[0] <= serial_max ) {
      twist_y = map(Bints[0], serial_trim, serial_max, 0, 500);
    }
    else if (Bints[0] <= serial_trim && Bints[0]  >= serial_min) {
      twist_y = map(Bints[0], serial_min, serial_trim, -500, 0);
    }

    if (Bints[1] >= serial_trim && Bints[1] <= serial_max ) {
      twist_z = map(Bints[1], serial_trim, serial_max, 0, 500);
    }
    else if (Bints[1] <= serial_trim && Bints[1]  >= serial_min) {
      twist_z = map(Bints[1], serial_min, serial_trim, -500, 0);
    }

  }
}
