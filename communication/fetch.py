from nbiot import nbiot
import json, time, datetime
import matplotlib.pyplot as plt
import numpy as np

class Plotter:
    def __init__(self):
        self.address = "https://api.nbiot.telenor.io"
        self.api_token = "bde332ec188ee5e679deda22b7770b66f9b2a2ccd6f7f15a8154fbeeb8825534"
        self.collectionId = "17dh0cf43jg03b"
        self.deviceId = "17dh0cf43jg755"
        self.client = nbiot.Client(addr = self.address, token = self.api_token)

    def fetch_data(self):
        return self.client.collection_data(self.collectionId, limit = 1)
    
    def get_flow_data(self, starttime, stoptime):
        data = []
        for i in self.fetch_data():
            if ((i.received.timestamp() > starttime) & (i.received.timestamp() < stoptime)):
                splitdata = f"{i.payload.decode('utf-8')}".split("-")
                tempdata = []
                tempdata.append(int(splitdata[0]))
                tempdata.append(int(splitdata[1]))
                if (len(splitdata) > 3):
                    tempdata.append(int(splitdata[3]))
                else:
                    tempdata.append(-1 * int(splitdata[2]))
                data.append(tempdata)
        return data[::-1]
    
    def show_plot(self, data, manualdata):
        yplt = []
        pltarr = []
        for i in range(len(data)):
            #if (i > 0):
                #pltarr.append(data[i-1][0])
                #yplt.append(i)
            pltarr.append(data[i][0])
            yplt.append(i)

        yplt = np.array(yplt)
        pltarr = np.array(pltarr)
        manualdata = np.array(manualdata)
        compare = abs(pltarr - manualdata)
        plt.bar(yplt, compare, color = "blue")
        #plt.plot(np.array(yplt), np.array(pltarr))
        plt.show()

    def print_all(self):
        for i in self.fetch_data()[::-1]:
            print(f"{i.payload.decode('utf-8')} : {i.received}")

                
if __name__ == "__main__":
    p = Plotter()
    #p.print_all()
    #exit()
    manualdata = [0,10,0,0,1,2,0,3,3,0,4]
    data = p.get_flow_data(1582705000.0, 1582707000.0)#1582704845.0)
    print(data)
    print(manualdata)
    p.show_plot(data, manualdata)