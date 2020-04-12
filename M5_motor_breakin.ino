#include <M5Stack.h>

#define ADDR 0x65
#define ADDR2 0x60

#define CONTR 0x00
#define FAULT 0x01

//manual or auto
//voltage
//time
//reverse

float voltage = 0;
float hours = 0;
float minutes = 0;
float reversal = 0;
int flg = 0;

void setup() {
  // put your setup code here, to run once:
  M5.Lcd.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("M5 motor breakin");
  M5.Lcd.setTextSize(2);

  Wire.begin();
  Wire.beginTransmission(ADDR);
  Wire.write(CONTR);
  Wire.write(0b00000000);
  Wire.endTransmission();
}

int vset(float volt) {
  return (int)(volt * 16 / 1.285);
  
}

void inputparam(float* param, int* n, float max = 10, float stp = 1.0) {
  if (M5.BtnA.wasPressed()) {
    if (*param < max) {
      *param += stp;
    }
  }
  if (M5.BtnB.wasPressed()) {
    if (*param > 0) {
      *param -= stp;
    }
  }
  if (M5.BtnC.wasPressed()) {
    *n += 1;
  }
  return;
}

int setparams() {
  int n = 0;
  while (1) {
    if (n == 0) {
      M5.Lcd.setCursor(0, 30+n*20);
      M5.Lcd.printf("voltage:%01.2f", voltage);
      inputparam(&voltage, &n, 5, 0.5);

    }else if(n==1){
      M5.Lcd.setCursor(0,30+n*20);
      M5.Lcd .printf("hours:%03.0f",hours);
      inputparam(&hours,&n,100); 
    }else if (n == 2) {
      M5.Lcd.setCursor(0, 30+n*20);
      M5.Lcd.printf("minutes:%03.0f", minutes);
      inputparam(&minutes, &n, 60);
    } else if (n == 3) {
      M5.Lcd.setCursor(0, 30+n*20);
      M5.Lcd.printf("reverse:%1.0f", reversal);
      inputparam(&reversal, &n,1);
    } else if (n == 4) {
      break;
    }
    M5.update();
  }
  return 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  flg = setparams();
  int motor = (vset(voltage) << 2) + ((int)reversal) + 1;
  if (flg == 1) {
    int i=0;
    Wire.beginTransmission(ADDR);
    Wire.write(CONTR);
    Wire.write(motor);
    Wire.endTransmission();
    while (1) {
      i++;
      delay(1000);
      M5.Lcd.setCursor(0,130);
      M5.Lcd.printf("%02d:%02d:%02d",i/3600,i/60%60,i%60);
      if(i>=minutes*60+hours*3600){
        break;
      }
    }
  }
  Wire.beginTransmission(ADDR);
  Wire.write(CONTR);
  Wire.write(0b00000000);
  Wire.endTransmission();
}
