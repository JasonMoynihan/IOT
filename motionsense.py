import urllib.request
from PIL import image
import RPi.GPIO as GPIO
import time
from pydub import AudioSegment
from pydub.playback import play
import paho.mqtt.client as mqtt

broker_address="192.168.1.224"
client = mqtt.Client()
client.connect(broker_address)

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(12, GPIO.IN)         #Read output from PIR motion sensor
GPIO.setup(3, GPIO.OUT)         #LED output pin

def intruder():
    song = AudioSegment.from_wav("sound.wav")
    play(song)
    url='http://192.168.1.226/picture/1/current/'
    Image.open(urllib.request.urlopen(url))
    #grab image as well
    image=Image.open(urllib.request.urlopen(url))

def on_message(client, userdata, message):
    message = message.payload
    val = float(message)
    if(val > 100):
        intruder()
        
client.subscribe("/data")
client.on_message=on_message
client.loop_start()

while True:
    i=GPIO.input(12)
    if i==0:                 #When output from motion sensor is LOW
        print("No intruders",i)
        #GPIO.output(3, 0)  #Turn OFF LED
        time.sleep(0.1)
    elif i==1:               #When output from motion sensor is HIGH
        print("Intruder detected",i)
        #GPIO.output(3, 1)  #Turn ON LED
        time.sleep(0.1)
        
client.loop_stop()