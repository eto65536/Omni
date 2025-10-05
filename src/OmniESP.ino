#include <PS4Controller.h>

int8_t X = 0;         //スティックX軸   -128~127
int8_t Y = 0;         //スティックY軸   -128~127
int8_t rotation = 0;  //回転速度        -128~127
int8_t ballC = 0;     //ボール回収      -128~127
int basket = 0;       //かご回収        -1~1
int corn = 0;         //コーン回収      -1~1

void setup() {
  Serial.begin(115200);
  PS4.begin("e8:d8:19:ed:30:ea");
}

void loop() {
  if (PS4.isConnected()) {
    X = PS4.LStickX();
    Y = PS4.LStickY();
    rotation = PS4.RStickX();
    if(PS4.R2())
    {
      ballC = PS4.R2Value() + 127;
    }
    else if(PS4.L2())
    {
      ballC = PS4.L2Value() - 127;
    }
    else
    {
      ballC = 0;
    }

    if(PS4.R1())
    {
      basket = 1;
    }
    else if(PS4.L1())
    {
      basket = -1;
    }
    else
    {
      basket = 0;
    }

    if(PS4.Triangle())
    {
      corn = 1;
    }
    else if(PS4.Square())
    {
      corn = -1;
    }
    else
    {
      corn = 0;
    }

    Serial.printf("%dX%dY%dR%dC%dB%dN\r",X,Y,rotation,ballC,basket,corn);
  }
  delay(100);
}
