#define SOUND_1_PIN A3
#define SOUND_2_PIN A2
#define SOUND_3_PIN A1
#define SOUND_4_PIN A0



void setup() {
  // put your setup code here, to run once:
  pinMode(SOUND_1_PIN, OUTPUT);
  pinMode(SOUND_2_PIN, OUTPUT);
  pinMode(SOUND_3_PIN, OUTPUT);
  pinMode(SOUND_4_PIN, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    char in = Serial.read();
    if (in == 'a')
    {
      digitalWrite(SOUND_1_PIN, HIGH);
      delay(200);
      digitalWrite(SOUND_1_PIN, LOW);
      Serial.println("Trigger sound 1");      
    }
    else if (in == 'b')
    {
      digitalWrite(SOUND_2_PIN, HIGH);
      delay(200);
      digitalWrite(SOUND_2_PIN, LOW);
      Serial.println("Trigger sound 2");      
    }
    else if (in == 'c')
    {
      digitalWrite(SOUND_3_PIN, HIGH);
      delay(200);
      digitalWrite(SOUND_3_PIN, LOW);
      Serial.println("Trigger sound 3");      
    }
    else if (in == 'd')
    {
      digitalWrite(SOUND_4_PIN, HIGH);
      delay(200);
      digitalWrite(SOUND_4_PIN, LOW);
      Serial.println("Trigger sound 4");      
    }
    else
    {
      Serial.println("Do nothing");
    } 
  }

  delay(300);

}
