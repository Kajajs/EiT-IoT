import serial, time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

def run():
    plotdata = []
    plt.plot(np.array(plotdata))
    plt.show(block = False)

    with serial.Serial("COM4", 9600) as ser:
        while True:
            data = int(ser.readline().decode("utf-8"))
            if (len(plotdata) > 10):
                plotdata = plotdata[1:]
            plotdata.append(data)

            plt.clf()
            plt.plot(np.array(plotdata), color = "blue")
            plt.pause(0.05)
            

if __name__ == "__main__":
	run()