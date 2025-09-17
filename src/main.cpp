#include "mbed.h"

int16_t wheelangle[] = {0,0,0,0};
float speed = 0; //走行速度 0 ~ 1.0
int angle = 0; //進行方向 0 ~ 360°
int X = 0; //-1.0 ~ 1.0
int Y = 0; //-1.0 ~ 1.0
int basespeed = 1; //速度倍率

BufferedSerial pc(PA_9, PA_10, 115200);

int main()
{
    
    CAN omnimotor(PA_11,PA_12,(int)1e6);
    CANMessage omnimotor_msg;

    
    while (true)
    {
        if(pc.readable())
        {
            char buf[64];
            string receive;
            int len = pc.read(buf,sizeof(buf));
            if(len > 0)
            {
                buf[len] = '\0';        
                receive = buf;
                size_t posX = receive.find("X");
                size_t posY = receive.find("Y");

                X = stoi(receive.substr(0,posX));
                Y = stoi(receive.substr(posX + 1,posY));
            }
        }
        
        speed = sqrt(X * X + Y * Y); //速度計算
        angle = atan2(Y,X); //移動方向計算

        for(int i = 0; i > 4; i++)
        {
            wheelangle[i] = sin((angle + 45 + i * 90) * M_PI / 180) * speed  * basespeed;
        }
        CANMessage omnimotor_msg(3, (const uint8_t *)wheelangle, 8);
        omnimotor.write(omnimotor_msg);
    }
    
}


