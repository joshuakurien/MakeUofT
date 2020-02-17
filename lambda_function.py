import paho.mqtt.client as mqtt
import time
import random
import json

def publish(client,sensor,topic,qos,simulated_reading,trend):
    simulated_reading = simulated_reading + trend * random.normalvariate(0.01, 0.005)
    payload = {"timestamp": int(time.time()), "device": sensor, sensor: simulated_reading}
    jsonpayload_sensor = json.dumps(payload, indent=4)
    client.publish(topic, jsonpayload_sensor, qos=qos)

def main(event, context):
    message = event["queryStringParameters"]["topic"]
    print("passed fron button click" + message)
    # Connection parms for Solace Event broker
    solace_url = "mr2j0vvhki1l0v.messaging.solace.cloud"
    solace_port = 20358
    solace_user = "solace-cloud-client"
    solace_passwd = "h9opao4l88b69t3nvqo2svv0b0"
    solace_clientid = "lambda"

   
    if message == 1:
        solace_topic_pressure = "openDoor"
    elif message == 2:
        solace_topic_pressure = "closeDoor"
    elif message == 3:
        solace_topic_pressure = "light"
    elif message == 4:
        solace_topic_pressure = "fan"
    elif message == 5:
        solace_topic_pressure = "music"
    elif message == 6:
        solace_topic_pressure = "alarm"
    else:
        solace_topic_pressure = "blank"
        
        

    solace_topic_sox = "devices/sox/events"
    solace_topic_level = "devices/level/events"

    # Instantiate/connect to mqtt client
    client = mqtt.Client(solace_clientid)
    client.username_pw_set(username=solace_user, password=solace_passwd)
    print("Connecting to solace {}:{} as {}".format(solace_url, solace_port, solace_user))
    client.connect(solace_url, port=solace_port)
    client.loop_start()

    #random.seed("temperature")  # A given device ID will always generate
    # the same random data

    simulated_nox = 50 + random.random() * 20
    simulated_sox = 40 + random.random() * 20
    simulated_pressure = 30 + random.random() * 20
    simulated_level = 15 + random.random() * 20

    if random.random() > 0.5:
        sensor_trend = +1  # value will slowly rise
    else:
        sensor_trend = -1  # value will slowly fall

    # Publish num_messages mesages to the MQTT bridge once per second.
    
    publish(client, "light", solace_topic_pressure, 1, simulated_pressure, sensor_trend)
 
    client.loop_stop()
    client.disconnect()

    return {
        'statusCode': 200,
        'body': "Success"
    }