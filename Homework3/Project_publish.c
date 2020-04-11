#include "mbed.h"
#include "TCPSocket.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include <stdlib.h>
#include <time.h>
#define randnum(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))



WiFiInterface *wifi;

int main()
{
    int count = 0;
    int profit = 0;
    int loss = 0;
    int profit_percentage = 0;
    int loss_percentage = 0;
    int total_sale = 0;
    int total_cost = 0;
    int net_profit = 0;
    int net_loss = 0;
    
    char* topic = "Profit";

    printf("WiFi MQTT example\n");

    #ifdef MBED_MAJOR_VERSION
        printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    #endif

    wifi = WiFiInterface::get_default_instance();
    if (!wifi) 
    {
        printf("ERROR: No WiFiInterface found.\n");
        return -1;
    }

    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) 
    {
        printf("\nConnection error: %d\n", ret);
        return -1;
    }

    printf("Success\n\n");
    printf("MAC: %s\n", wifi->get_mac_address());
    printf("IP: %s\n", wifi->get_ip_address());
    printf("Netmask: %s\n", wifi->get_netmask());
    printf("Gateway: %s\n", wifi->get_gateway());
    printf("RSSI: %d\n\n", wifi->get_rssi());

    MQTTNetwork mqttNetwork(wifi);

    MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);

    const char* hostname = "192.168.88.10";
    int port = 1883;
    printf("Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNetwork.connect(hostname, port);
    if (rc != 0)
    {
        printf("rc from TCP connect is %d\r\n", rc);
    }
        
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "mbed-sample";
    data.username.cstring = "";
    data.password.cstring = "";
    
    if ((rc = client.connect(data)) != 0)
    {
        printf("rc from MQTT connect is %d\r\n", rc);
    }
    else
    {
        printf("Client Connected.\r\n");
    }
        

    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    
    while(count < 100) // client.isConnected()
    {
        int cost = randnum(1, 2000);
        int sale = randnum(1, 4000);
        total_cost = total_cost + cost;
        total_sale = total_sale + sale;
        if(sale > cost || sale == cost)
        {
            profit = sale - cost;
            profit_percentage = (profit*100)/sale;
            char buf[100];
            sprintf(buf, "{\"Cost\": %d, \"Sale\": %d, \"Profit\": %d, \"Loss\": %d}", cost, sale, profit_percentage, loss_percentage);
            printf("Clien sent->%s",buf);
            message.payload = buf;
            message.payloadlen = strlen(buf);
            rc = client.publish(topic, message);
            count++;
            sale=cost=profit=profit_percentage=0;
            //Wait for 10 seconds.
            wait(10);
        }
        else{
            loss = cost - sale;
            loss_percentage = (loss*100)/cost;
            char buf[100];
            sprintf(buf, "{\"Cost\": %d, \"Sale\": %d, \"Profit\": %d, \"Loss\": %d}", cost, sale, profit_percentage, loss_percentage);
            printf("Clien sent->%s",buf);
            message.payload = buf;
            message.payloadlen = strlen(buf);
            rc = client.publish(topic, message);
            count++;
            sale=cost=loss=loss_percentage=0;
            //Wait for 5 seconds.
            wait(10);
        }

    }
    if(total_sale > total_cost || total_sale == total_cost)
    {
        net_profit = total_sale - total_cost;
        char buf[100];
        sprintf(buf, "{\"Net_profit\": %d, \"Net_loss\": %d}", net_profit, net_loss);
        printf("Clien sent->%s",buf);
        message.payload = buf;
        message.payloadlen = strlen(buf);
        rc = client.publish(topic, message);
    }
    else{
        net_loss = total_cost - total_sale;
        char buf[100];
        sprintf(buf, "{\"Net_profit\": %d, \"Net_loss\": %d}", net_profit, net_loss);
        printf("Clien sent->%s",buf);
        message.payload = buf;
        message.payloadlen = strlen(buf);
        rc = client.publish(topic, message);
    }
    
    mqttNetwork.disconnect();

    if ((rc = client.disconnect()) != 0)
    {
        printf("rc from disconnect was %d\r\n", rc);
        printf("Client Disconnected.\r\n");
    }

    wifi->disconnect();

    printf("\nDone\n");
}
