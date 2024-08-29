TheGarageSandbox
================

Just a bunch of projects to play in ciandt the garage (will be splitted into others projects if they grow)

# PROJECTS
 
## AnalogSingleDisplay (By rmarco)
- Using 7 segments display, this project show all hexadecimal chars (0-F) into a loop. Enjoy the funny initialization =D
 
## leapServoController
- Using LeapMotion js API, is a web page sending http requests with hand's position data to an Arduino (using Ethernet Shield) to control a servo motor.
 
## rgbLed
- Using a html/js color picker, change the color of a RGB led with arduino
 
## restArdueasy
- A easy way to start a arduino+ethernet project. (still big =/)
- This lib provides:
  1. beginSerial : Init the serial comunication (9600) 
  2. beginEthernet : Init the server
  3. bind : Bind a request method and URI with a function to handle the request (WOW! Like SpringMVC =P)
  4. restLoop : Put this function on your loop to read http requests and automaticaly call your binds! (WOW! Like SpringMVC =P)
 
## LCD Temperature
- This small project was made to meet the requirements of a challenge of the CI&T 3D Printer
- Requirements of this project:
	- Use a temperature sensor and show the current temperature on the LCD display, and
	have two buttons to control a limit temperature one degree at a time. 
	If the max. temperature is reached, it will make a buzz. Altogether, the LED must show
	a color for each temperature range, as follows:
		- Less than 12 celsius of the limit temperature: Blue 
		- Between 12(inclusive) and 8 celsius below the limit: Green
		- Between 8 and 5 celsius below the limit: Yellow
		- Between 4 and 1 celsius below the limit: Orange
		- Equal or less than 5 celsius over the limit: Red
		- More than 5 celsius over the limit: Blinking Red
 
#Disclaimer
*Use by your own risks! Have fun and feel free to copy and use all this code!*
 
So Long and Thanks for all the Fish.