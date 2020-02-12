from nbiot import nbiot
import json, time, datetime

address = "https://api.nbiot.telenor.io"
api_token = "bde332ec188ee5e679deda22b7770b66f9b2a2ccd6f7f15a8154fbeeb8825534"
collectionId = "17dh0cf43jg03b"
deviceId = "17dh0cf43jg755"

client = nbiot.Client(addr = address, token = api_token)
data = client.collection_data(collectionId, limit = 1)
for i in data:
    print(f"{i.payload.decode('utf-8')} {i.received.timestamp()}")