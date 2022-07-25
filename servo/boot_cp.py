# -*- coding:utf-8 -*-
'''
功能描述
    串口解析信息 二进制数据 获取的舵机云台的角度
    然后执行命令，舵机旋转对应的角度

接线：

1.USB转TTL模块
    ESP32,TDX,D19 -> RXD,USB转TTL
    ESP32,RXD,D18 -> TXD,USB转TTL
2.PCA9685(I2C舵机驱动板)
    ESP32,SCL，D25 -> PCA9685,SCL
    ESP32,SDA, D26 -> PCA9685,SDA
'''
from machine import UART, Pin, I2C
import struct
import utime
import micropython
from cloud_platform import CloudPlatform

# UART串口通信协议的长度
uart_protocal_len = 9

# PCA9685模块的I2C接口接线
gpio_scl = 25
gpio_sda = 26

# 初始化I2C
scl_pin = Pin(gpio_scl)
sda_pin = Pin(gpio_sda)
i2c = I2C(scl=scl_pin, sda=sda_pin, freq=10000)  # 创建I2C实例
# 创建一个云台对象
cp = CloudPlatform(i2c, 0, 1)

# 设定紧急意外缓冲区的大小为100
micropython.alloc_emergency_exception_buf(100)

degrees = [0, 30, 60, 90, 120, 150, 180, 210, 240, 270]

for degree in degrees:
    cp.set_btm_servo_angle(degree)
    # cp.set_top_servo_angle(degree)
    print('set degree: ', degree)
    utime.sleep_ms(5000)


def auto():
    bottom_degree = 0
    top_degree = 0
    flag_add = True
    while True:
        if flag_add:
            bottom_degree += 1
            top_degree += 1
        else:
            bottom_degree -= 1
            top_degree -= 1

        if bottom_degree >= 180:
            flag_add = False
        if bottom_degree <= 0:
            flag_add = True

        cp.set_btm_servo_angle(bottom_degree)
        # cp.set_top_servo_angle(top_degree)
        print('bottom_degree:', bottom_degree)
        # print('top_degree:', top_degree)
        utime.sleep_ms(100)
