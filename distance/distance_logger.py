import serial, time
import numpy as np
import matplotlib.pyplot as plt

class FlowPlotter:
	def __init__(self):
		self.comm_port = "COM4"
		self.baud_rate = 115200
		self.start_time = time.time()

	def run(self):
		with serial.Serial(self.comm_port, self.baud_rate) as ser:
			while True:
				data = int(ser.readline().decode("utf-8"))
				with open("log.txt", "a+") as log:
					log.write(f"{data}:{time.time() - self.start_time}\n")

	def plot(self):
		timestamp = [0.0]
		passes = [0]
		nr = 0
		with open("log.txt", "r") as log:
			lines = log.read()
			lines = lines.split("\n")
			for line in lines[0:-1]:
				data = line.split(":")
				timestamp.append(float(data[1]))
				timestamp.append(float(data[1]))
				if (int(data[0])):
					passes.append(nr)
					passes.append(nr - 1)
					nr -= 1
				else:
					passes.append(nr)
					passes.append(nr + 1)
					nr += 1
		

		plt.plot(np.array(timestamp), np.array(passes))
		plt.show()

if __name__ == "__main__":
	plotter = FlowPlotter()
	#plotter.run()
	plotter.plot()

