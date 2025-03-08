import sensor, image, time, math
from pyb import LED
from pyb import UART

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)

#thresholds = [(0, 20, -128, 127, -128, 127)]
#thresholds = [(100, 0, -128, -38, -128, 127)]
thresholds = [(10, 100, 127, 10, -128, 127)]

sensor.reset() #初始化设置
sensor.set_pixformat(sensor.RGB565) #设置为彩色
sensor.set_framesize(sensor.HQVGA) #设置清晰度
sensor.skip_frames(time = 2000) #跳过前2000ms的图像
#sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock() #创建一个clock便于计算FPS，看看到底卡不卡

uart = UART(3, 115200)
readflag=0
flag=0

while(True): #不断拍照
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8) #拍摄一张照片，lens_corr函数用于非鱼眼畸变矫正，默认设置参数为1.8
    if uart.any():
        readflag = uart.read()
        print(readflag)
        if readflag == b'\xa5':
            flag=1

    if flag==1:
        #img = sensor.snapshot().lens_corr(1.8) #拍摄一张照片，lens_corr函数用于非鱼眼畸变矫正，默认设置参数为1.8
        for blob in img.find_blobs(thresholds,pixels_threshold=2,area_threshold=15):
            #print(blob.solidity())
            if blob.solidity()>0.50:
                img.draw_rectangle(blob.rect(),color = (255, 0, 0))
                print([blob.cx(),blob.cy()]);
                a = bytearray([0xA5,blob.cx()-120,blob.cy()-90,0x5A])
                uart.write(a)
                if abs(blob.cx()-120)<10 and abs(blob.cy()-90)<10:
                    blue_led.on()
                else:
                    blue_led.off()

    #print("FPS %f" % clock.fps())
    #if flag==0:
        #a = bytearray([0xA5,0,0,0x5A])
        #uart.write(a)


