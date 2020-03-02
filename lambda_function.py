import paho.mqtt.client as mqtt
import time
import random
import json

def publish(client,topic):
    client.publish(topic)

def main(event, context):
    message = event["queryStringParameters"]["topic"]
    
    # Connection parameters for Solace Event broker
    solace_url = "mr2ko4me0p6h2f.messaging.solace.cloud"
    solace_port = 20134
    solace_user = "solace-cloud-client"
    solace_passwd = "u3ve6gpbjs62v5efj3uobk2564"
    solace_clientid = "lambda"

    if message == 1:
        solace_topic = "openDoor"
    elif message == 2:
        solace_topic = "closeDoor"
    elif message == 3:
        solace_topic = "light"
    elif message == 4:
        solace_topic = "fan"
    elif message == 5:
        solace_topic = "music"
    elif message == 6:
        solace_topic = "alarm"
    else:
        solace_topic = "blank"
        
    # Instantiate/connect to mqtt client
    client = mqtt.Client(solace_clientid)
    client.username_pw_set(username=solace_user, password=solace_passwd)
    client.connect(solace_url, port=solace_port)
    client.loop_start()

    # Publishes message to the MQTT bridge
    publish(client, solace_topic)
 
    client.loop_stop()
    client.disconnect()

    return {
        'statusCode': 200,
        'body': "Success"
    }
