import RPi.GPIO as GPIO
import time 
import paho.mqtt.client as mqtt

broker_address="192.168.1.27"
client = mqtt.Client()
client.connect(broker_address)

GPIO.setmode(GPIO.BCM)
GPIO.setup(21, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(18, GPIO.OUT)
clickedflag = False
ledon = False

def on_message(client, userdata, message):
	print(message.topic + " " + str(message.payload))
	lightstate = GPIO.input(18)

	if(lightstate):
		GPIO.output(18, GPIO.LOW)
		lightstate = False;

	else:
		GPIO.output(18, GPIO.HIGH)
		lightstate = True;

client.subscribe("/led")
client.on_message=on_message

client.loop_start()
while True:

	buttonstate = GPIO.input(21)

	if(buttonstate and not clickedflag):
		clickedflag = True;

	if(not buttonstate and clickedflag):
		clickedflag = False;

		if(not ledon):
			client.publish("/button","on")
			ledon = True;

		else:
			client.publish("/button","off")
			ledon = False;

client.loop_stop()





