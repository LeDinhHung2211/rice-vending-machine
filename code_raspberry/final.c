# include <stdio.h>
# include <wiringPi.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "string.h"
#include "unistd.h"
#include "stdlib.h"

#include "LCD_i2c/LCD_I2C.h"
#include "MQTTClient.h"
#include "MQTT/mqtt_pub/mqtt_pub.h"
#include "MQTT/mqtt_sub/mqtt_sub.h"

#define s0 0
#define s1 1
#define s2 2
#define s3 3
#define out 4
#define size 3
// lcd address
int LCDAddr = 0x27;// I2C address for module PCF on LCD
extern int fd, rawData[3];
const int PWM_pin = 24,Button_pin = 27; //Servo pin: 24 (wiringPi), Button pin: 27 (wiringPi)
int intensity = 0; // Mức xung đầu ra của servo
// giá trị tare của cân, các giá trị cân nặng theo màu, khoảng offset van gạo
int tare_wt, target_wt_green = 0,target_wt_white = 0,target_wt_others = 0 , offset_servo = 4;
bool flag_loadcell = 0; //cờ chạy cân và van gạo
//uint8_t dataConfig[3][1];

bool latch_green =0, latch_white =0, latch_others =0; // cờ chốt cảm biến màu

// hàm cấu hình servo
void servoInit()
{
	pinMode (PWM_pin, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(38);
	pwmSetRange(1024);
}
// hàm chạy servo theo xung từ hardware pwm
void servoWrite(int pwm)
{
	pwmWrite (PWM_pin, pwm); 
}

// struct weight để chứa các giá trị về khối lượng
int wt; 
 /*-----------------struct-----------------*/
 struct hx711_pin{
     int SCK;
      int SDA;
     int EN;                        // kich hoat tieu chuan
     int calibration;            //dcalibration
     int coefficient;            //coef
     int weight;                    //weight
    unsigned long value;        //record value
 };
 // hàm cấu hình chân của hx711
 void set_pin(struct hx711_pin *value){
     value->SCK             = 29;
     value->SDA             = 28;
     value->EN            = 1;
     value->coefficient    = 100;
 }
 //Hàm khởi tạo HX711
 void init_pin(struct hx711_pin *value){
     pinMode(value->SCK,OUTPUT);
     pinMode(value->SDA,INPUT);
     pullUpDnControl(value->SDA,PUD_UP);
 }
 // Hàm đọc HX711
 void start(struct hx711_pin *value){
     static uint32_t count =0;
     int i;
     value -> value =0;
     digitalWrite(value->SCK,LOW);        //enable AD
	//  printf("0\n");
      while(digitalRead(value->SCK));
	//  printf("1\n");
     value->value = 0;                    //Numerical value
     while(digitalRead(value->SDA));        //Wait until AD conversion is not completed.
     delay(100);
    //  printf("2\n");
	 for(i=0;i<24;i++){
         digitalWrite(value->SCK,HIGH);
         while(0 == digitalRead(value->SCK))delay(10);
//         printf("3\n");
         value->value = value->value*2;
         digitalWrite(value->SCK,LOW);
         while(digitalRead(value->SCK));
         if(digitalRead(value->SDA))
             value->value = value->value + 1;
     }
     digitalWrite(value->SCK,HIGH);
     digitalWrite(value->SCK,LOW);
     if((value->EN == 1)&&(value->value<25000)){
         value->EN = 0;
         value->calibration = value->value;
     }else {
         i = (value->value)/value->coefficient;
     }
     //printf("i: %d\n",i);
     uint32_t calib;
     if(i<6000)
	 {
	 	value->weight = i+280;
		if (value->weight>400) value->weight = i/100 + 480; 
	 	//count++;
	 }
     else
	{
		calib = (i - 163000)/10 -82;
		if (calib>100) calib -= 7;
		value->weight = calib;
		if (value->weight <=10) value->weight =0;
		count =0;
	}
	wt =  value->weight;
	char text[10];
	if (!flag_loadcell)
		tare_wt = wt;
	else 
	{
		wt = wt - tare_wt;
		
	}
	sprintf(text, "%d", wt);
	write_lcd(0, 0, "weight: ");
	write_lcd(7, 0, text);
	write_lcd(10, 0, " g");
	//printf("%d Weight is：%d g\n",count,rs);
    // printf("calib: %d\n",calib);     
}
 
// hàm setup hx711 trước khi bắt đầu vòng lặp
 int setup(struct hx711_pin *value){    
     if(wiringPiSetup()==-1)return 1;
     set_pin(value);
     init_pin(value);
     return 0;
 }

uint8_t color;
static uint32_t count; // Biến đếm xung từ cảm biến màu trả về
static uint32_t time_count; 
uint32_t data;
// hàm lấy dữ liệu màu từ cảm biến (xanh lá, trắng, màu còn lại)
uint8_t getColor()
{		
	if (data >= 100 && data < 128) return 0;
	else if (data >=130) return 1;
	else return 2;
}

// Hàm ngắt để đọc nút nhấn
void checkButton()
{
	flag_loadcell = 1;
	printf("ok\n");
	switch(color)
	{
		case 0: latch_green =1;
				printf("green\n");
				break;
		case 1: latch_white = 1;
				printf("white\n");
				break;
		case 2: latch_others = 1;
				printf("othes\n");
				break;
	}
}
//Hàm chờ dữ liệu trả bằng MQTT 
int on_message_topic_1(void *context, char *topicName, int topicLen, MQTTClient_message *message) {

	char* payload = message->payload;
    printf("Received message from topic %s: %s\n",topicName, payload);
	//splitConvertData(payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
	//char string[50] = "36/54/0/78";
	//printf(payload);
	splitConvertData(payload,":");
	//check_topic(topicName);	
	/* for (int i=0;i<size;i++)
	{
		printf("data[%d]: %d\n",i,rawData[i]);
		
	} */
	target_wt_green = rawData[0];
	target_wt_white = rawData[1];
	target_wt_others = rawData[2];
	printf("green: %d\n", target_wt_green);
    printf("white: %d\n", target_wt_white);
	printf("others: %d\n", target_wt_others);
	return 1;
}

//Hàm ngắt đọc số xung trả về từ cảm biến màu
void changeState()
{
	count++;
	
}
 
 
 //Hàm main dể chạy chương trình
 int main(void){
	wiringPiSetup();
	fd = wiringPiI2CSetup(LCDAddr);
    lcd_init();
	//int data =10;
	
	
    struct hx711_pin value;
	pinMode(Button_pin,INPUT);
	//GY-31 init//
	pinMode(out,INPUT);
	pinMode(s2,OUTPUT);
	pinMode(s3,OUTPUT);
	digitalWrite(s2,0);
	digitalWrite(s3,0);
	//interrupt init//
	wiringPiISR(Button_pin, INT_EDGE_RISING, &checkButton);
	wiringPiISR(out, INT_EDGE_RISING, &changeState);
	//servo init//
	servoInit();
	servoWrite(150);
	//MQTT init//
	MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID_PUB, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	
	MQTTClient_setCallbacks(client, NULL, NULL, on_message_topic_1, NULL);
	
    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
	
	MQTTClient_subscribe(client, SUB_TOPIC, 0);
	printf("Press button to set tare\n");
	
    if(0 == setup(&value))
	{ 
		//loop(&value);
		while(1)
		{
			start(&value);// Đọc giá trị loadcell		
			if(!wt) clearAt(8,9,0); // Xóa LCD tại vị trí từ 8 ->9, hàng 0
			data = count /100;
			count =0;
			time_count = millis();	
			color = getColor();
			if (flag_loadcell)
			{
				servoWrite(245);
				
				if (latch_green)
				{
					if (wt >= ((target_wt_green - offset_servo)))
					{
						delay(5);
						servoWrite(150);
						printf("wt: %d\n",wt);
						printf("target: %d\n",target_wt_green -offset_servo);
						flag_loadcell =0;
						latch_green =0;
						char data[50];
						//Gửi chuỗi JSON về khối lượng và màu sắc: xanh
						sprintf(data,"{\"color\":\"green\",\"weight\":%d}",target_wt_green);
						publish(client, PUB_TOPIC, data);
					}
				}
				else if (latch_white)
				{
					if (wt >= ((target_wt_white - offset_servo)))
					{
						delay(5);
						servoWrite(150);
						printf("wt: %d\n",wt);
						printf("target: %d\n",target_wt_white);
						flag_loadcell =0;
						latch_white =0;
						char data[50];
						// Gửi chuỗi JSON về khối lượng và màu sắc: trắng
						sprintf(data,"{\"color\":\"white\",\"weight\":%d}",target_wt_white);
						publish(client, PUB_TOPIC, data);
					}
				}
				else 
				{
					if (wt >= ((target_wt_others - offset_servo)))
					{
						delay(5);
						servoWrite(150);
						printf("wt: %d\n",wt);
						printf("target: %d\n",target_wt_others -offset_servo);
						flag_loadcell =0;
						latch_others =0;
						char data[50];
						// Gửi chuỗi JSON về khối lượng và màu sắc: màu còn lại
						sprintf(data,"{\"color\":\"others\",\"weight\":%d}",target_wt_others);
						publish(client, PUB_TOPIC, data);
						//delay(1000);
						/* sprintf(text, "%d", wt);
						write(0, 0, "weight: ");
						write(7, 0, text);
						write(10, 0, "g"); */
					}
				}
			}
		
		    switch (color)
			{
				case 0: //clearAt(7,14,1);
						write_lcd(0, 1, "color: ");
						write_lcd(7, 1, "green  ");
						break;
				case 1: //clearAt(7,14,1);
						write_lcd(0, 1, "color: ");
						write_lcd(7, 1, "white   ");
						break;
				case 2: //clearAt(7,14,1);
						write_lcd(0, 1, "color: ");
						write_lcd(7, 1, "others   ");
						break;
			} 
		}
	}
	
     return 0;
}

