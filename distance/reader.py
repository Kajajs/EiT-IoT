import serial, time
import numpy as np
import matplotlib.pyplot as plt

class FlowPlotter:
	def __init__(self):
		self.comm_port = "COM4"
		self.baud_rate = 115200
		self.start_time = time.time()
		self.passes = []
		self.durartion = 10

	def add_plot_data(self, timestamp, direction):
		self.passes.append((timestamp - self.start_time, direction))

	def run(self):
		with serial.Serial(self.comm_port, self.baud_rate) as ser:
			while True:
				data = int(ser.readline().decode("utf-8"))
				with open("log.txt", "a+") as log:
					log.write(f"{data}:{time.time() - self.start_time}\n")

	def plot(self):
		pass

if __name__ == "__main__":
	plotter = FlowPlotter()
	#try:
	plotter.run()
	#except:
		#print("Finish")

