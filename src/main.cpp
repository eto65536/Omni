#include "mbed.h"

int16_t wheelangle[] = {0,0,0,0};   //オムニホイール4輪
int16_t etcmotor[] = {0,0,0};       //ボール回収,かご,コーン
float speed = 0;                    //走行速度 0 ~ 1.0
int angle = 0;                      //進行方向 0 ~ 360°
float rotation = 0;                 //回転速度 -1.0 ~ 1.0
float X = 0;                        //スティックX軸 -1.0 ~ 1.0
float Y = 0;                        //スティックY軸 -1.0 ~ 1.0
float ball = 0;                    //ボール回収駆動速度 -1.0 ~ 1.0
int basket = 0;                     //かご回収駆動速度   -1 or 0 or 1
int corn = 0;                       //コーン回収駆動速度 -1 or 0 or 1

BufferedSerial esp(PA_9, PA_10, 115200);

float limitRange(float value, float min, float max, float deadzone)
{
    if(value < min) return min;         //範囲制限
    if(value > max) return max;         //範囲制限
    if(abs(value) < deadzone) return 0; //デッドゾーン処理
    return value;                       //変更なし
}

int main()
{
    CAN motor(PB_12,PB_13,(int)1e6);
    CANMessage omnimotor_msg;
    CANMessage etcmotor_msg;

    char buf[128];
    int buf_count = 0;
    while(true)
    {
        if(esp.readable())
        {
            char r;
            int len = esp.read(&r,1);
            if(len > 0)
            {
                if(r == 'X')
                {
                    buf[buf_count] = '\0';
                    X = stof(buf) / 127;    // -1.0 ~ 1.0に変換
                    memset(buf,0,sizeof(buf));
                    buf_count = 0;
                }
                else if(r == 'Y')
                {
                    buf[buf_count] = '\0';
                    Y = stof(buf) / 127;    // -1.0 ~ 1.0に変換
                    memset(buf,0,sizeof(buf));
                    buf_count = 0;
                }
                else if(r == 'R')
                {
                    buf[buf_count] = '\0';
                    rotation = stof(buf) / 127; // -1.0 ~ 1.0に変換
                    memset(buf,0,sizeof(buf));
                    buf_count = 0;
                }
                else if(r == 'C')
                {
                    buf[buf_count] = '\0';
                    ball = stof(buf) / 127; // -1.0 ~ 1.0に変換
                    memset(buf,0,sizeof(buf));
                    buf_count = 0;
                }
                else if(r == 'B')
                {
                    buf[buf_count] = '\0';
                    basket = stoi(buf); // intに変換
                    memset(buf,0,sizeof(buf));
                    buf_count = 0;
                }
                else if (r == 'N')
                {
                    buf[buf_count] = '\0';
                    corn = stoi(buf); // intに変換
                    memset(buf,0,sizeof(buf));
                    buf_count = 0;
                }
                else
                {
                    buf[buf_count++] = r;
                }
            }
        }

        speed = sqrt(X * X + Y * Y);     //速度計算
        angle = atan2(Y,X) * 180 / M_PI; //移動方向計算

        speed = limitRange(speed, -1.0, 1.0, 0.1);
        rotation = limitRange(rotation, -1.0, 1.0, 0.1);
        ball = limitRange(ball, -1.0, 1.0, 0.1);
        basket = limitRange(basket, -1.0, 1.0, 0);
        corn = limitRange(corn, -1.0, 1.0, 0);

        etcmotor[0] = ball * 12000;   //ボール回収
        etcmotor[1] = basket * 12000;  //かご
        etcmotor[2] = corn * 5000;     //コーン

        for(int i = 0; i < 4; i++)
        {
            if(abs(rotation) > 0.1)
            {
                wheelangle[i] = rotation * 12000; //回転計算
            }
            else
            {
                wheelangle[i] = sin((angle + 45 + i * 90) * M_PI / 180) * speed  * 12000; //走行計算
            }
        }
        
        CANMessage omnimotor_msg(325, (const uint8_t *)wheelangle, 8);
        motor.write(omnimotor_msg);
        CANMessage etcmoter_msg(4, (const uint8_t *)etcmotor, 8);
        motor.write(etcmotor_msg);
    }
    
}
