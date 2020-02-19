import RPi.GPIO as GPIO
import time 
import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient
import datetime

broker_address="192.168.1.27"
client = mqtt.Client()
client.connect(broker_address)

GPIO.setmode(GPIO.BCM)
GPIO.setup(23, GPIO.OUT)

dbclient = InfluxDBClient('0.0.0.0', 8086, 'root', 'root', 'mydb')
query = 'select mean("value") from "test" where "time" > now() - 10s'

receiveTime = datetime.datetime.utcnow()

def on_message(client, userdata, message):
	print(message.topic + " " + str(message.payload))
	
	message = message.payload
	val = float(message)
	json_body = [
		{
			"measurement": 'test',
			"time": receiveTime,
			"fields": {
				"value": val
			}
		}
	]

	dbclient.write_points(json_body)

client.subscribe("/data")
client.on_message=on_message

client.loop_start()

try:
	while True:
		try:
			if(mean >= 200):
				GPIO.output(23, GPIO.HIGH)

			if(mean < 200):
				GPIO.output(23, GPIO.LOW)
		except:
			pass
except KeyboardInterrupt:
	pass

client.loop_stop()