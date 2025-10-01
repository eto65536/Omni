#include "mbed.h"

int16_t wheelangle[] = {0,0,0,0};
float speed = 0; //走行速度 0 ~ 1.0
int angle = 0; //進行方向 0 ~ 360°
int X = 0; 
int Y = 0; 
int basespeed = 1; //速度倍率

BufferedSerial pc(PA_9, PA_10, 115200);

int main()
{
    CAN omnimotor(PA_11,PA_12,(int)1e6);
    CANMessage omnimotor_msg;

    
    char buf[64];
    int buf_count = 0;
    while(true)
    {
        if(pc.readable())
        {
            char r;
            int len = pc.read(&r,1);
            if(len > 0)
            {
                if(r == 'X')
                {
                    buf[buf_count] = '\0';
                    X = stoi(buf);
                    buf_count = 0;
                }
                else if(r == '\n')
                {
                    buf[buf_count] = '\0';
                    Y = stoi(buf);
                    buf_count = 0;
                    printf("X=%d, Y=%d\n", X, Y);
                }
                else
                {
                    buf[buf_count++] = r;
                }
            }
        }

        speed = sqrt(X * X + Y * Y); //速度計算
        angle = atan2(Y,X); //移動方向計算

        for(int i = 0; i < 4; i++)
        {
            wheelangle[i] = sin((angle + 45 + i * 90) * M_PI / 180) * speed  * basespeed;
        }
        CANMessage omnimotor_msg(3, (const uint8_t *)wheelangle, 8);
        omnimotor.write(omnimotor_msg);

        ThisThread::sleep_for(10ms);
    }
    
}

