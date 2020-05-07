#include "debug.h"
#include <mosquitto.h>
#include "cJSON.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>


void PrintInfo(void)
{
    printf("\t换相开关调试-v1.0\n");
    printf("0:查看当前数据中心的版本信息\n");
    printf("1:查看当前数据中心模型\n");
    printf("2:查看模型表\n");
    printf("3:查看设备信息\n");
    printf("4.查看实时数据\n");
    printf("5:读取换相开关APP的定值\n");
    printf("6:修改换相开关APP定值\n");
    printf("7:换相开关APP遥控换相\n");
    printf("8.删除模型\n");
}



void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
    if (!result)
	{
		mosquitto_subscribe(mosq, NULL, Topic_Get_Real_Data, 2);
        mosquitto_subscribe(mosq, NULL, Topic_Get_parameter, 2);
        mosquitto_subscribe(mosq, NULL, Topic_Sub_Unmodel, 2);
        mosquitto_subscribe(mosq, NULL, Topic_Set_Parameter_ack, 2);
        mosquitto_subscribe(mosq, NULL, Topic_Sub_Register, 2);
        mosquitto_subscribe(mosq, NULL, Topic_Sub_Model, 2);
        mosquitto_subscribe(mosq, NULL, Topic_Sub_Model_Type, 2);
        mosquitto_subscribe(mosq, NULL, Topic_Sub_Database_version, 2);
	}
    else
    {
        fprintf(stderr, "Connect failed\n");
    }
}








void deal_data(char *data)
{
    char *p = data;
    cJSON* root;
	cJSON* element;
	cJSON* element2;
    cJSON* body_index;
	cJSON* body_child;

    root = cJSON_Parse(p);
    if (!root) 
    {
        printf("root error!\n");
		return ;
	}

    for(element=root->child; element!=NULL; element=element->next)
    {
        if (element->type == cJSON_String)
        {
            
        }
        else if (element->type == cJSON_Object)
        {

        }
        else if (element->type == cJSON_Array)
        {
            
            int i = 0;
            int num = cJSON_GetArraySize(element);
            for(i=0; i<num; i++)
            {
                body_index = cJSON_GetArrayItem(element,i);
				body_child = cJSON_GetObjectItem(body_index, "dev");
                printf("dev:%s\n",body_child->valuestring);
            }
        }
    }
}

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	printf("Topic : %s\r\n",message->topic);
    printf("Get MQTT mesage : %s\r\n",message->payload);
	
    /*if (message->payloadlen)
	{
        deal_data((char *)(message->payload));
	}*/
}

void getutctime(char *utctime)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    char buf[32];
    char tmp_utctime[32];
    strftime(buf, sizeof(buf), "%FT%T", localtime(&tv.tv_sec));
    sprintf(tmp_utctime, "%s.%03dZ", buf, tv.tv_usec/1000);
    //cout << tmp_utctime << endl;
    strcpy(utctime, tmp_utctime);
}

void Pub_Real_Data(void)
{
    char guid[30] = {0};
    printf("请输入设备的dev\n");
    scanf("%s",guid);

    char Topic[255];
	cJSON* root;
	cJSON* body;
	char* pMsg = NULL;
	char JsonCurrentTime[32] = {0};
	cJSON* body_root;
	memset(Topic, '\0', sizeof(Topic));
	strcpy(Topic, Topic_Pub_Real_Data);
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"token",cJSON_CreateString("123"));
	getutctime(JsonCurrentTime);
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));
	body = cJSON_AddArrayToObject(root, "body");

	body_root = cJSON_CreateObject();
	cJSON_AddItemToObject(body_root, "dev", cJSON_CreateString(guid));
	cJSON_AddItemToObject(body_root, "totalcall", cJSON_CreateString("1"));
    cJSON_AddItemToObject(body_root, "body", cJSON_CreateString(""));
	cJSON_AddItemToArray(body, body_root);

	pMsg = cJSON_Print(root);
    printf("%s\n",pMsg);

    mosquitto_publish(mosq,NULL,Topic,strlen(pMsg),pMsg,2,0);
	free(pMsg);
	cJSON_Delete(root);
}

void PubParameter_data(void) //读取换相开关定植
{
    char guid[30] = {0};
    printf("请输入数据中心第一台换相开关的dev\n");
    scanf("%s",guid);
    char Topic[255];
	cJSON* root;
	cJSON* body;
	char* pMsg = NULL;
	char JsonCurrentTime[32] = {0};
	cJSON* body_root;
	memset(Topic, '\0', sizeof(Topic));
	strcpy(Topic, Topic_Pub_parameter);
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"token",cJSON_CreateString("123"));
	getutctime(JsonCurrentTime);
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));
	body = cJSON_AddArrayToObject(root, "body");

	body_root = cJSON_CreateObject();
	cJSON_AddItemToObject(body_root, "dev", cJSON_CreateString(guid));
	cJSON_AddItemToObject(body_root, "totalcall", cJSON_CreateString("1"));
    cJSON_AddItemToObject(body_root, "body", cJSON_CreateString(""));
	cJSON_AddItemToArray(body, body_root);

	pMsg = cJSON_Print(root);
    printf("%s\n",pMsg);

    mosquitto_publish(mosq,NULL,Topic,strlen(pMsg),pMsg,2,0);
	free(pMsg);
	cJSON_Delete(root);
}

int Set_PubParameter(void)   //换相开关定值写入
{
    char guid[64] = {0};
    char imbalance[10];
    char imbalance_time[10];
     printf("请输入换相开关的dev\n");
    scanf("%s",guid);
    printf("请输入不平衡度\n");
    scanf("%s",imbalance);
    printf("请输入不平衡时间\n");
    scanf("%s",imbalance_time);

    char Topic[255];
	cJSON* root;
	cJSON* body;
	char* pMsg = NULL;
	char JsonCurrentTime[32] = {0};
	cJSON* body_root;
    cJSON* body_child;
    cJSON* body_child_obj;
	memset(Topic, '\0', sizeof(Topic));
	strcpy(Topic, Topic_Set_Parameter);
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"token",cJSON_CreateString("123"));
	getutctime(JsonCurrentTime);
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));
	body = cJSON_AddArrayToObject(root, "body");

	body_root = cJSON_CreateObject();
	cJSON_AddItemToObject(body_root, "dev", cJSON_CreateString(guid));

    body_child = cJSON_AddArrayToObject(body_root, "body");

    body_child_obj = cJSON_CreateObject();
	cJSON_AddItemToObject(body_child_obj, "name", cJSON_CreateString("span"));
    cJSON_AddItemToObject(body_child_obj, "val", cJSON_CreateString(imbalance));
    cJSON_AddItemToObject(body_child_obj, "dataType", cJSON_CreateString("Int"));
    cJSON_AddItemToArray(body_child, body_child_obj);
    body_child_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(body_child_obj, "name", cJSON_CreateString("ratio"));
    cJSON_AddItemToObject(body_child_obj, "val", cJSON_CreateString(imbalance_time));
    cJSON_AddItemToObject(body_child_obj, "dataType", cJSON_CreateString("Int"));
    cJSON_AddItemToArray(body_child, body_child_obj);

	cJSON_AddItemToArray(body, body_root);
    
	pMsg = cJSON_Print(root);
    printf("%s\n",pMsg);

    mosquitto_publish(mosq,NULL,Topic,strlen(pMsg),pMsg,2,0);
	free(pMsg);
	cJSON_Delete(root);
}

int GetRegister(void)
{
    char Topic[255];
	cJSON* root;
	cJSON* body;
	char* pMsg = NULL;
	char JsonCurrentTime[32] = {0};
	cJSON* body_root;
	memset(Topic, '\0', sizeof(Topic));
	strcpy(Topic, Topic_Pub_Register);
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"token",cJSON_CreateString("123"));
	getutctime(JsonCurrentTime);
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));
	body = cJSON_AddArrayToObject(root, "body");
	cJSON_AddItemToArray(body, body_root);

	pMsg = cJSON_Print(root);
    printf("%s\n",pMsg);

    mosquitto_publish(mosq,NULL,Topic,strlen(pMsg),pMsg,2,0);
	free(pMsg);
	cJSON_Delete(root);
}

void PubUnmodel_data() //删除模型
{
    char model[20] = {0};
    printf("请输入要删除的模型\n");
    scanf("%s", model);
    char Topic[255];
	cJSON* root;
	cJSON* body;
	char* pMsg = NULL;
	char JsonCurrentTime[32] = {0};
	cJSON* body_root;
	memset(Topic, '\0', sizeof(Topic));
	strcpy(Topic, Topic_Pub_Unmodel);
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"token",cJSON_CreateString("123"));
	getutctime(JsonCurrentTime);
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));
	body = cJSON_AddArrayToObject(root, "body");

    cJSON_AddItemToArray(body, cJSON_CreateString(model));
	//body_root = cJSON_CreateObject();
    
	//cJSON_AddItemToObject(body_root, "dev", cJSON_CreateString("PhaseSwitch_8abaf1c4455ce0d6"));
	//cJSON_AddItemToArray(body, body_root);

	pMsg = cJSON_Print(root);
    printf("%s\n",pMsg);

    mosquitto_publish(mosq,NULL,Topic,strlen(pMsg),pMsg,2,0);
	free(pMsg);
	cJSON_Delete(root);
}

void SetChangePhase() //读取换相开关定植
{
    char data[20] = {0};
    printf("请按如下格式：换相开关地址相位，如10008003A\n");
    scanf("%s", data);
    char Topic[255];
	cJSON* root;
	cJSON* body;
	char* pMsg = NULL;
	char JsonCurrentTime[32] = {0};
	cJSON* body_root;
	memset(Topic, '\0', sizeof(Topic));
	strcpy(Topic, Topic_Pub_ChangePhase);
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"token",cJSON_CreateString("123"));
	getutctime(JsonCurrentTime);
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));
    cJSON_AddItemToObject(root, "data_row", cJSON_CreateString("single"));
	body = cJSON_AddArrayToObject(root, "body");

	body_root = cJSON_CreateObject();
	cJSON_AddItemToObject(body_root, "name", cJSON_CreateString("Change"));
	cJSON_AddItemToObject(body_root, "val", cJSON_CreateString(data));
    cJSON_AddItemToObject(body_root, "quality", cJSON_CreateString("0"));
	cJSON_AddItemToArray(body, body_root);

	pMsg = cJSON_Print(root);
    printf("%s\n",pMsg);

    mosquitto_publish(mosq,NULL,Topic,strlen(pMsg),pMsg,2,0);
	free(pMsg);
	cJSON_Delete(root);
}

void GetDatabaseModel(void) //获取数据中心当前已有模型
{
    char Topic[255];
	cJSON* root;
	cJSON* body;
	char* pMsg = NULL;
	char JsonCurrentTime[32] = {0};
	cJSON* body_root;
	memset(Topic, '\0', sizeof(Topic));
	strcpy(Topic, Topic_Pub_Model);
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"token",cJSON_CreateString("123"));
	getutctime(JsonCurrentTime);
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));
	body = cJSON_AddArrayToObject(root, "body");
	cJSON_AddItemToArray(body, body_root);

	pMsg = cJSON_Print(root);
    printf("%s\n",pMsg);

    mosquitto_publish(mosq,NULL,Topic,strlen(pMsg),pMsg,2,0);
	free(pMsg);
	cJSON_Delete(root);
}

void GetDatabaseModelTpye(void) //获取数据中心当前已有的模型的数据类型
{
    char model_name[20] = {0};
    printf("请输入要查看的模型的名称\n");
    scanf("%s", model_name);

    char Topic[255];
	cJSON* root;
	cJSON* body;
	char* pMsg = NULL;
	char JsonCurrentTime[32] = {0};
	cJSON* body_root;
	memset(Topic, '\0', sizeof(Topic));
	strcpy(Topic, Topic_Pub_Model_Type);
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"token",cJSON_CreateString("123"));
	getutctime(JsonCurrentTime);
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));
	body = cJSON_AddArrayToObject(root, "body");

	cJSON_AddItemToArray(body, cJSON_CreateString(model_name));

	pMsg = cJSON_Print(root);
    printf("%s\n",pMsg);

    mosquitto_publish(mosq,NULL,Topic,strlen(pMsg),pMsg,2,0);
	free(pMsg);
	cJSON_Delete(root);
}

void GetDatabaseVersion(void) //获取数据中心当前已有模型
{
    char Topic[255];
	cJSON* root;
	cJSON* body;
	char* pMsg = NULL;
	char JsonCurrentTime[32] = {0};
	cJSON* body_root;
	memset(Topic, '\0', sizeof(Topic));
	strcpy(Topic, Topic_Pub_Database_version);
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"token",cJSON_CreateString("123"));
	getutctime(JsonCurrentTime);
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(JsonCurrentTime));
	body = cJSON_AddArrayToObject(root, "body");
	cJSON_AddItemToArray(body, body_root);

	pMsg = cJSON_Print(root);
    printf("%s\n",pMsg);

    mosquitto_publish(mosq,NULL,Topic,strlen(pMsg),pMsg,2,0);
	free(pMsg);
	cJSON_Delete(root);
}