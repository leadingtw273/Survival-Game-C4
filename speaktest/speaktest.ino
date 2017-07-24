const int speaker=13;
void setup()
{}
void loop()
{
  for(int i = 0;i<10;i++)  
  {
    tone(speaker,2000);
    delay(50);
  }
  noTone(speaker);
  delay(2000);

}
