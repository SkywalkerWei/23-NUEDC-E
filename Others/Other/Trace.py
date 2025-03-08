import sensor, image, time, pyb, math, random
from pyb import UART
from pyb import LED
from pyb import Pin
from pid import PID
rho_pid = PID(p=0.4, i=0)
theta_pid = PID(p=0.001, i=0)
THRESHOLD = (5, 70, -23, 15, -57, 0) # 线LAb
LED(1).on()
LED(2).on()
LED(3).on()

sensor.reset()
# sensor.set_vflip(True) # 根据实际安装情况，竖直镜像
# sensor.set_hmirror(True) # 根据实际安装情况，水平镜像
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQQVGA) # 80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
#sensor.set_windowing([0,20,80,40])
sensor.skip_frames(time = 2000)
uart = UART(3,115200)
clock = time.clock()

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
    img = sensor.snapshot().binary([THRESHOLD]) # 二值化成黑白图
    line = img.get_regression([(100,100)], robust = True) # 线性回归，返回值是一条直线
    if (line):
        rho_err = abs(line.rho())-img.width()/2 # 相比画面中心偏移的距离 中心=0 左负右正
        if line.theta()>90: #相比竖直方向偏移的角度 中心=0 左负右正各90度
            theta_err = line.theta()-180
        else:
            theta_err = line.theta()
        img.draw_line(line.line(), color = 127)
        # print(rho_err,line.magnitude(),rho_err)
        if line.magnitude()>8: # 如果拟合度好
            #if -40<b_err<40 and -30<t_err<30:
            # send_data_8(rho_err,theta_err,0,0,0,0,0,0)
            rho_output = rho_pid.get_pid(rho_err,1)
            theta_output = theta_pid.get_pid(theta_err,1)
            output = rho_output+theta_output
            # 以右转为正，综合得到校正值，左轮加速右轮减速
            send_data_8(output,-output,0,0,0,0,0,0)
        else: # 拟合度不行
            send_data_8(0,0,0,0,0,0,0,0)
    else: # 没找到线，拉满原地转圈
        send_data_8(50,-50,0,0,0,0,0,0)