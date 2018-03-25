#include "mbed.h"

Serial pc (USBTX,USBRX);
I2CSlave slave(PA_10, PA_9);
DigitalOut myled(LED1);

int main() {
   char buf[10];
   char msg[] = "Slave!";
   pc.printf("start\r\n");
   slave.address(0x20);
   while (1) {
       int i = slave.receive();
       static int j = 0;
       
       if (j == 100) pc.printf("alive\r\n");
       j++;
       if (i) pc.printf("i=%d\r\n", i);

       switch (i) {
           case I2CSlave::ReadAddressed:
               slave.write(msg, strlen(msg) + 1); // Includes null char
               break;
           case I2CSlave::WriteGeneral:
               slave.read(buf, 10);
               printf("Read G: %s\n", buf);
               break;
           case I2CSlave::WriteAddressed:
               slave.read(buf, 10);
               printf("Read A: %s\n", buf);
               
               if (buf[0])
               {
                   myled = 0; // LED is OFF
               }
               else
               {
                    myled = 1; // LED is ON
               }
               
               break;
       }
       for(int i = 0; i < 10; i++) buf[i] = 0;    // Clear buffer
   }
}


#if 0
#include "mbed.h"
DigitalOut myled(LED1);
Serial pc (USBTX,USBRX);

int main() {
    pc.printf("first\r\n");
    while(1) {
        myled = 1; // LED is ON
        wait(0.2); // 200 ms
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
        pc.printf("hello\r\n");
    }
}
#endif
