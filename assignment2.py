import RPi.GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(21, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(18, GPIO.OUT)
clickedflag = False
ledon = False
while True:
	
	buttonstate = GPIO.input(21)
	
	if(buttonstate and not clickedflag):
		clickedflag = True;
		
	if(not buttonstate and clickedflag):
		clickedflag = False;
		
		if(not ledon):
		
			GPIO.output(18, GPIO.HIGH)
			ledon = True;
			
		else:
		
			GPIO.output(18, GPIO.LOW)
			ledon = False;