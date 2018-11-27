# -*- coding: utf-8 -*-
"""
Created on Fri Nov  2 16:35:09 2018

@author: Luca
"""

##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial
from datetime import datetime  

#def AskForPortnumber():
#    port = str(input("Choose Serial Port COM: "))
#    port = "COM" + port
#    return port

#locations=['/dev/ttyACM0','/dev/ttyUSB0','/dev/ttyUSB1','/dev/ttyUSB2','/dev/ttyUSB3','COM10','COM11']

COM_Number_min=8
COM_Number_max=20
locations=[None] * (COM_Number_max-COM_Number_min)

for x in range(COM_Number_min, COM_Number_max):
    locations[x-COM_Number_min]='COM'+ str(x)
    
baud_rate = 9600 #In arduino, Serial.begin(baud_rate)

for serial_port in locations:
    try:
        print("Trying...",serial_port)
        ser = serial.Serial(serial_port, baud_rate)
        ser.timeout = None
        break
    except:
        print("Failed to connect on",serial_port)

## loop until the arduino tells us it is ready
connected = False
while not connected:
    serin = ser.read()
    ser.flushInput()
    ser.flushOutput()
    connected = True
    
write_to_file_path = datetime.now().strftime('%Y%m%d-%H%M%S-')+"Messung_Geschwindigkeit.txt"
print("Write to: ",write_to_file_path)
output_file = open(write_to_file_path, 'w')

output_file.write(write_to_file_path+'\n')
output_file.write("Messungen Arduino"+ '\t' + "Luca Mazzoleni"+'\n')

print("== Start Serial read ==")
try:
    while True:
        line = ser.readline()
        line = line.decode("utf-8") #ser.readline returns a binary, convert to string
        line = line.rstrip()
        print(line)
        line=datetime.now().strftime('%Y/%m/%d - %H:%M:%S')+'\t'+line+'\n'
        output_file.write(line)
except KeyboardInterrupt:
    ser.close()
    output_file.close()
    pass

print("End Script")