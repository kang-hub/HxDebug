#include "debug.h"
#include <stdio.h>

struct mosquitto *mosq = NULL;
bool session = true;

int main(int argc, char* argv[])
{
    int num;

    mosquitto_lib_init();

    mosq = mosquitto_new(NULL,session,NULL);
    if(!mosq)
    {
        printf("create client failed..\n");
        mosquitto_lib_cleanup();
        return 1;
    }

    mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_message_callback_set(mosq, my_message_callback);

    if(mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE))
    {
        fprintf(stderr, "Unable to connect.\n");
        return -1;
    }

    int loop = mosquitto_loop_start(mosq);
    if(loop != MOSQ_ERR_SUCCESS)
    {
        printf("mosquitto loop error\n");
        return -1;
    }

    while(1)
    {
       PrintInfo();
        
        scanf("%d", &num);
        switch(num)
        {
            case 0:
                GetDatabaseVersion();
                break;
            case 1:
                GetDatabaseModel();
                break;
            case 2:
                GetDatabaseModelTpye();
                break;
            case 3:
                GetRegister();
                break;
            case 4:
                Pub_Real_Data();
                break;
            case 5:
                PubParameter_data();
                break;
            case 6:
                Set_PubParameter();
                
                break;
            case 7:
                SetChangePhase();

                break;
            case 8:
                PubUnmodel_data();                
                break;
            default:
                printf(" ‰»Î¥Ì¡À!\n");
                break;
        }
        sleep(2);
    }
}
