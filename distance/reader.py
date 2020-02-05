import serial

with serial.Serial('COM4', 115200) as ser:
	while ser.read():
		x = ser.readline()
		print(x)#.decode("utf-8"))