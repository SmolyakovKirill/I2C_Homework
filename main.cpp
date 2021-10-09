#include <stdio.h>
#include "mbed.h"
#include "BMP180.h"
#include "beep.h"

I2C i2c(I2C_SDA, I2C_SCL);
BMP180 bmp180(&i2c);
DigitalOut myled_1(D7);
DigitalOut myled_2(D9);
Beep buzzer(D8);
DigitalIn mybutton_1(USER_BUTTON);


int main(void) {
    mybutton_1.mode(PullUp);
    
    while(1) {                                  // Проверка подключения к BMP180
        if (bmp180.init() != 0) {
            printf("Ошибка связи с BMP180\n");
        } else {
            printf("Подключение к BMP180\n");
            break;
        }
        wait(1);
    }

    while(1) {                              // Получаем значение температуры и давления
        bmp180.startTemperature();
        wait_ms(5);     
        float temp;
        if(bmp180.getTemperature(&temp) != 0) {
            printf("Сложности с получением значения температуры\n");
            continue;
        }

        bmp180.startPressure(BMP180::ULTRA_LOW_POWER);
        wait_ms(10);    
        int pressure;
        if(bmp180.getPressure(&pressure) != 0) {
            printf("Сложности с получением значения давления\n");
            continue;
        }

        if(mybutton_1 == 0) // По нажатию получаем значение температуры и давления 
        {
            printf("Атмосферное давление = %d Pa Температура = %f C\n", pressure, temp);
        }
        
        if(temp > 30.0) // Если температура больше 30, то включаем пьезодинамик и красный светодиод, если меньше, то зеленый светодиод
        {
            myled_1 = 1;
            myled_2 = 0;
            buzzer.beep(870,0.025); 
        }
        else 
        {
            myled_1 = 0;
            myled_2 = 1;
        }
        wait(1); 
    }
}