import sensor, image, time, pyb, math, random
from pyb import UART
from pyb import LED
from pyb import Pin
yellow_threshold = ( 56,83,5,57,63,80) #LAB格式，标记物的颜色，可以通过ide内置筛选器拿到
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(10)
sensor.set_auto_whitebal(False)
uart = UART(3,115200)
clock = time.clock()

LED(1).on()
LED(2).on()
LED(3).on()

K=5000 #计算得到该值：在已知条件下，获取到某距离时标记在摄像头中的像素长度。例如标记距离10cm，在摄像头中长度500像素

def send_data_8(data1,data2,data3,data4,data5,data6,data7,data8):
    header = bytearray([0x2C, 0x12])
    data1_bytes = data1.to_bytes(1, 'big')
    data2_bytes = data2.to_bytes(1, 'big')
    data3_bytes = data3.to_bytes(1, 'big')
    data4_bytes = data4.to_bytes(1, 'big')
    data5_bytes = data5.to_bytes(1, 'big')
    data6_bytes = data6.to_bytes(1, 'big')
    data7_bytes = data7.to_bytes(1, 'big')
    data8_bytes = data8.to_bytes(1, 'big')
    footer = bytearray([0x5B])
    uart.write(header +  data1_bytes +data2_bytes +data3_bytes+data4_bytes +data5_bytes+data6_bytes +data7_bytes+data8_bytes+ footer)
    print("串口发送:",data1,data2,data3,data4,data5,data6,data7,data8)

while(True):
    clock.tick()
    img = sensor.snapshot()
    blobs = img.find_blobs([yellow_threshold])
    if len(blobs) == 1:
        b = blobs[0]
        img.draw_rectangle(b[0:4])
        img.draw_cross(b[5],b[6])
        Lm = (b[2]+b[3])/2 # 圆形标记，取均值，测量时均测量直径
        length = K/Lm
        print(length)
        send_data_8(length,0,0,0,0,0,0,0)