const int pwm_Pin = 9;          //sets PWM output as pin 8 (remains constant)
const int volt_pin = A0;        //sets voltage input as analog pin 0 (remains constant)
const int current_pin = A2;     //sets current input as analog pin 2 (remains constant)

int k = 0;                      //int to hold PWM value
float pre_volt = 0.0;           //sets variable for raw voltage before conversion
float pre_current = 0.0;        //sets variable for raw current before conversion
float final_volt = 0.0;         //sets variable for final voltage value after conversion
float final_current = 0.0;      //sets variable for final current value after conversion

const float bit_to_volt_conversion = 0.0048875855;    //conversion factor to convert 10 bit input to voltage (5/1023)

const float voltage_gain = 2.0;         //value set based on gain going into A0.  May be subject to change based on max voltage values and resistor changes
const float voltage_gate = 1/6;         //value set based on gain of voltage going into voltage_gain

const float current_gain = 52.0;        //value set based on gain going into A2.  May be subject to change based on max voltage values and resistor changes
const float current_resistor = 2.26;    //2.26 ohm resistor used for heat dissipation, resistor affects gain on current measurment

const float amp_to_mA_conversion = 1000.0;  //multiplier to convert from amps to mA for more precise measurement



void setup() {
  pinMode(pwm_Pin, OUTPUT);     //sets PWM pin as output
  pinMode(volt_pin, INPUT);     //sets analog pin 0 as input
  pinMode(current_pin, INPUT);  //sets analog pin 5 as input
  Serial.begin(9600);           //open serial at baud rate 9600
}
void loop() {
  while (Serial.available() > 0) {  //while serial is detected, must send any string to activate e.g. type "l" and send and it will begin
    k = 0;                          //resets k=0 after each loop
    Serial.println();               //new line
    Serial.println();               //new line
    Serial.println();               //new lines for start of next dataset
    delay(1500);                    //delays 1.5 seconds between each data sample

    while (k < 255) {                        //performs loop until k=255

      analogWrite(pwm_Pin, k);               //writes int value of k to pin 9
      delay(50);                             //50 millisecond delay for circuit latency

      pre_volt = analogRead(volt_pin);       //read voltage input pin in 10 bits
      pre_current = analogRead(current_pin); //read current input pin in 10 bits

      final_volt = pre_volt*bit_to_volt_conversion*(1/voltage_gain)*(1/voltage_gate);                                     //converts pre_voltage value to actual voltage value. Raw input, convert from 10 bits, inverse voltage gain, inverse voltage gate. Note we use inverse because we are un-doing the gain to read original voltage.
      final_current = pre_current*bit_to_volt_conversion*(1/current_gain)*(1/current_resistor)*(amp_to_mA_conversion);    //converts pre_current value to actual current in mA. Raw input, convert from 10 bits, inverst current gain, inverse current resistor, multiplied by 1000 to convert from A to mA.
      
      Serial.println();
      Serial.print(k);              //prints the present value of PWM signal
      Serial.print("    ");         //adds a space in between values
      Serial.print(pre_volt);     //prints the final voltage value for present PWM value
      Serial.print("    ");         //adds a space in between values
      Serial.print(pre_current);  //prints the final current value for the present PWM value
      k = k + 1;                    //increases the PWM value by one for the next loop
      delay(50);                    //50 millisecond delay for circuit reset
    }
  }
}









