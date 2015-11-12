int val = 0;

void setup() {
     val = analogRead(AIN0);
     printf("val is %d\n",val);
}

void loop() {
     exit(0);
}