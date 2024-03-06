#define ARR_SIZE 4096

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);

  Serial.println("Setup Complete");
}

void loop() {
  static String input = "";
  static uint32_t output[ARR_SIZE];
  static uint index = 0;

  if(Serial.available()){
    while(Serial.available()){
      input += Serial.read();
    }
    Serial.print("Input: "); Serial.println(input);
    Serial1.write(0); Serial1.write(1); // Serial1.print(input); 

    input = "";

    for(int i = 0; i < ARR_SIZE; ++i){
      output[i] = 0;
    }
  }

  if(Serial1.available()){
    Serial.println("Receiving MCA Data");

    output[index] = Serial1.read();

    if(index > 3 && (output[index - 2] == 0 && output[index - 1] == 0 && output[index] == 0)){
      index -= 3;
    }
    index++;

    Serial.println("Output: ");
    for(int i = 0; i < ARR_SIZE; ++i){
      Serial.print(output[i]); Serial.print(',');
    }
    Serial.println("\n");

    Serial.println("Read All MCA Data");
  }
}