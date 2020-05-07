#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <mosquitto.h>
#include <stdbool.h>

#define HOST "localhost"
#define PORT  1883
#define KEEP_ALIVE 6
#define MSG_MAX_SIZE  512

#define Topic_Get_Real_Data "database/get/response/showdb/realtime"
#define Topic_Pub_Real_Data "showdb/get/request/database/realtime"

#define Topic_Get_parameter "database/get/response/showdb/parameter"
#define Topic_Pub_parameter "showdb/get/request/database/parameter"

#define Topic_Set_Parameter "showdb/set/request/database/parameter"
#define Topic_Set_Parameter_ack "database/set/response/showdb/parameter"

#define Topic_Pub_Unmodel  "showdb/action/request/database/deletemodel"
#define Topic_Sub_Unmodel  "database/action/response/showdb/deletemodel"

#define Topic_Pub_Register "showdb/get/request/database/register"
#define Topic_Sub_Register "database/get/response/showdb/register"

#define Topic_Pub_ChangePhase  "showdb/notify/event/database/PhaseSwitch/PhaseSwitch_YK" //遥控换相

#define Topic_Pub_Model "showdb/get/request/database/modelschema" //获取数据中心当前已有的模型的名称
#define Topic_Sub_Model "database/get/response/showdb/modelschema"

#define Topic_Pub_Model_Type "showdb/get/request/database/model" //获取数据中心当前已有的模型的数据类型
#define Topic_Sub_Model_Type "database/get/response/showdb/model"

#define Topic_Pub_Database_version "showdb/get/request/database/version"
#define Topic_Sub_Database_version "database/get/response/showdb/version"



extern struct mosquitto *mosq;
extern bool session;

void PrintInfo(void); 
void my_connect_callback(struct mosquitto *mosq, void *userdata, int result);
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);

#endif