import paho.mqtt.client as mqtt
import time
import random
import json

def publish(client, topic,qos):
    payload = {1}
    jsonpayload_sensor = json.dumps(payload, indent=4)
    client.publish(topic, jsonpayload_sensor, qos=qos)

def main(event):
    message = event["queryStringParameters"]["topic"]
    print("passed fron button click" + message)
    # Connection parms for Solace Event broker
    solace_url = "mr2j0vvhki1l0v.messaging.solace.cloud"
    solace_port = 20358
    solace_user = "solace-cloud-client"
    solace_passwd = "h9opao4l88b69t3nvqo2svv0b0"
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
    print("Connecting to solace {}:{} as {}".format(solace_url, solace_port, solace_user))
    client.connect(solace_url, port=solace_port)
    client.loop_start()

    # Publishes mesages to the MQTT bridge.
    
    publish(client, "light", solace_topic, 1)
 
    return {
        'statusCode': 200,
        'body': "Success"
    }
