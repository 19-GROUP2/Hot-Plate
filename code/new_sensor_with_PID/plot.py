import serial
import matplotlib.pyplot as plt

plt.ion()
fig = plt.figure()


i = 0
x = list()
y = list()
i = 0
ser = serial.Serial('COM4', 9600)
ser.close()
ser.open()
while True:

    data = list(map(float,ser.readline().decode()[:-2].split(',')))
    req,now,err,ang=data
    # print(data)
   

    print("Req Tmp: ",req)
    print("Now Tmp: ",now)
   

    plt.scatter(i, now)
    i += 1
    plt.show()
    plt.pause(0.0001)  # Note this correction

    # print("ang",ser.readline().decode())
    print("cur err:",err)
    print("fir ang:",ang,'\n')

    
