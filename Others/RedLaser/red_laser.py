import sensor, image, time, pyb, math
import random
from pyb import UART
from pyb import LED
from pyb import Pin

thresholds = [(30, 100, 127, 12, -128, 127)]

sensor.reset() #初始化设置
sensor.set_pixformat(sensor.RGB565) #设置为彩色
sensor.set_framesize(sensor.QVGA) #设置清晰度
sensor.skip_frames(time = 2000) #跳过前2000ms的图像
#sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking

clock = time.clock()
uart = UART(3,115200)
blue_led = pyb.LED(3)

def LED_tip():
    for tt in range(5):
        LED(1).on()
        LED(2).off()
        LED(3).off()
        time.sleep_ms(100)
        LED(1).on()
        LED(2).on()
        LED(3).on()
        time.sleep_ms(100)

def LED_red():
    for tt in range(5):
        LED(1).on()
        time.sleep_ms(100)
        LED(1).on()
        time.sleep_ms(100)

def send_data(data1,data2):
    header = bytearray([0x2C, 0x12])
    data1_bytes = data1.to_bytes(2, 'big')
    data2_bytes = data2.to_bytes(2, 'big')
    footer = bytearray([0x5B])
    uart.write(header +  data1_bytes +data2_bytes+ footer)
    print("串口发送:",data1,data2)

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

readflag=0
flag1=0
flag2=0
q=[0]*20
cx=0
cy=0
stage=0
while(True): #不断拍照
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8) #拍摄一张照片，lens_corr函数用于非鱼眼畸变矫正，默认设置参数为1.8
    i=0
    # if uart.any():
    #     readflag = uart.read()
    #     print(readflag)

    # if readflag == b'\xa5':
    if True:
        #img = sensor.snapshot().lens_corr(1.8) #拍摄一张照片，lens_corr函数用于非鱼眼畸变矫正，默认设置参数为1.8
        #readflag=0
        for blob in img.find_blobs(thresholds,pixels_threshold=10,area_threshold=10):
            #print(blob.solidity())
            if blob.solidity()>0.50 and flag1<6:
                flag1=flag1+1
                img.draw_rectangle(blob.rect(),color = (255, 0, 0))
                # print([blob.cx(),blob.cy()]);
                cx=cx+blob.cx()
                cy=cy+blob.cy()
                # send_data(cx,cy)
                # print("Red:",cx,cy)

        for r in img.find_rects(threshold = 22000):
            #print(r)
            if r.w() > 30 and r.h() > 30 and r.w() <150 and r.h() < 150 and flag2<6:
                flag2=flag2+1
                img.draw_rectangle(r.rect(), color = (0, 0, 255))
                for p in r.corners():
                    img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
                    q[i]=q[i]+p[0]
                    i=i+1
                    q[i]=q[i]+p[1]
                    i=i+1

        # print([0xA5,cx,cy,q[0], q[1],q[2], q[3],q[4],q[5],q[6], q[7],0xA6])
        if flag1>=5 and flag2>=5:
            readflag=0
            cx=cx//flag1
            cy=cy//flag1
            q[0]=q[0]//flag2
            q[1]=q[1]//flag2
            q[2]=q[2]//flag2
            q[3]=q[3]//flag2
            q[4]=q[4]//flag2
            q[5]=q[5]//flag2
            q[6]=q[6]//flag2
            q[7]=q[7]//flag2
            if stage==0:
                send_data_8((q[0]+q[2]+q[4]+q[6])//4,(q[1]+q[3]+q[5]+q[7])//4,0,0,0,0,0,0)
                LED_tip()
                stage=1
            if stage==1:
                send_data_8(q[0],q[1],q[2],q[3],q[4],q[5],q[6],q[7])
                print("LD:",q[0],",",q[1],"RD:",q[2],",",q[3],"RU:",q[4],",",q[5],"LU:",q[6],",",q[7])
                LED_tip()
                stage=2
            if stage==2:
                send_data_8(cx,cy,0,0,0,0,0,0)
                print("Red:",cx,cy)
                LED_red()
            # a = bytearray([0xA5,cx,cy,q[0], q[1],q[2], q[3],q[4],q[5],q[6], q[7],0xA6])
            #a = bytearray([0xA5,100,0x5A])
            #uart.write(a)
            #print([0xA5,cx,cy,q[0], q[1],q[2], q[3],q[4],q[5],q[6], q[7],0xA6])
            q=[0]*20
            cx=0
            cy=0
            flag1=0
            flag2=0