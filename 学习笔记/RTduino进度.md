## 一、前言

最近在和一位小伙伴共同完成LPC55s69-evk的Arduino的生态兼容层（简称RTduino），目前已经完成硬件部分的引脚对接及整体框架的搭建。本文我将以RTduino外设篇（i2c、uart）进行记录。

## 二、i2c测试

首先我们来看到arduino对应LPC的i2c外设引脚为D0和D1，这也是Arduino默认的串口引脚，我们接下来的工作就是在LPC中添加PIN设备的引脚定义。

```c
const pin_map_t pin_map_table[]=
{
    {D0, GET_PINS(1,24), "uart2"},      /* Serial2-RX */
    {D1, GET_PINS(0,27), "uart2"},      /* Serial2-TX */
    {D2, GET_PINS(0,15)},
    {D3, GET_PINS(1,6)},
    {D4, GET_PINS(1,7), "pwm2", 2},     /* PWM */
    {D5, GET_PINS(1,4), "pwm2", 1},     /* PWM */
    {D6, GET_PINS(1,10)},
    {D7, GET_PINS(1,9), "pwm1", 0},     /* PWM */
    {D8, GET_PINS(1,8)},
    {D9, GET_PINS(1,5)},
    {D10, GET_PINS(1,1)},
    {D11, GET_PINS(0,26)},
    {D12, GET_PINS(1,3), "pwm0", 3},    /* PWM */
    {D13, GET_PINS(1,2)},
    {D14, GET_PINS(1,21), "i2c4"},      /* I2C-SDA (Wire) */
    {D15, GET_PINS(1,20), "i2c4"},      /* I2C-SCL (Wire) */
    {A0, GET_PINS(0,16), "adc0", 8},    /* ADC */
    {A1, GET_PINS(0,23), "adc0", 0},    /* ADC */
    {A2, RT_NULL},
    {A3, GET_PINS(1,31)},
    {A4, GET_PINS(0,13), "i2c1"},       /* I2C-SDA (Wire) */
    {A5, GET_PINS(1,14), "i2c1"},       /* I2C-SCL (Wire) */
    {A6, RT_NULL, "adc0", 13},          /* ADC, On-Chip: internal reference voltage, ADC_CHANNEL_VREFINT */
    {A7, RT_NULL, "adc0", 26},          /* ADC, On-Chip: internal temperature sensor, ADC_CHANNEL_TEMPSENSOR */
};
```

这里需要我们注意一点：

**引脚的定义配置需要结合NXP的`MCUXpresso Config Tools`，虽然在`.\rt-thread\bsp\lpc55sxx\lpc55s69_nxp_evk\board\MCUX_Config\LPCXpresso55S69.mex`存在这个配置文件，但是个人建议还是配合`MCUXpresso IDE`先生成一个模板工程，选择所需的引脚定义后再粘贴到我们的MDK工程中，因为直接使用bsp目录下的`LPCXpresso55S69.mex`配置文件生成的代码，会将PLL0的150M时钟的相关配置给清除掉！**

所以正确的做法如下：

#### 1、打开MCUXpresso IDE先随意导入一个示范工程，注意选中正确SDK包，如果对环境配置还不太熟悉的朋友可以看看这一篇文章[**LPC55S69开发环境搭建**](https://www.nxpic.org.cn/module/forum/thread-630377-1-2.html)

![image-20230227165342013](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302271653421.png)

![image-20230227165441811](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302271654923.png)

![image-20230227165749637](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302271657805.png)

#### 2、选中我们需要的引脚，在代码预览中复制生成的相关代码，由于RTduino对接的是i2c1，所对应的引脚为IO0-13（I2C-SDA）及引脚IO1-14（I2C-SCL），所以我们使能FLEXCOMM2

![image-20230227172054841](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302271720920.png)

在右侧找到代码预览，复制对应生成的代码到MDK工程中

![image-20230227172353340](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302271723494.png)

```c
// pin_mux.c

    IOCON->PIO[0][13] = ((IOCON->PIO[0][13] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT013 (pin 71) is configured as FC1_RXD_SDA_MOSI_DATA. */
                         | IOCON_PIO_FUNC(PIO0_13_FUNC_ALT5)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO0_13_DIGIMODE_DIGITAL));

    IOCON->PIO[0][14] = ((IOCON->PIO[0][14] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT014 (pin 72) is configured as FC1_TXD_SCL_MISO_WS. */
                         | IOCON_PIO_FUNC(PIO0_14_FUNC_ALT6)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO0_14_DIGIMODE_DIGITAL));
```

```c
// pin_mux.h

/*!
 * @brief Select Digital mode.: Enable Digital mode. Digital input is enabled. */
#define PIO0_13_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 5. */
#define PIO0_13_FUNC_ALT5 0x05u
/*!
 * @brief Select Digital mode.: Enable Digital mode. Digital input is enabled. */
#define PIO0_14_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 6. */
#define PIO0_14_FUNC_ALT6 0x06u
/*!
```

#### 3、在drv_i2c.c中添加i2c1的结构体声明

![image-20230227172602553](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302271726635.png)

```c
enum
{
#ifdef BSP_USING_I2C1
    I2C1_INDEX,
#endif
#ifdef BSP_USING_I2C4
    I2C4_INDEX,
#endif
};

-------------------------------------------------------------------------------------------------------------------

struct lpc_i2c_bus lpc_obj[] =
{
#ifdef BSP_USING_I2C1
        {
            .I2C = I2C1,
            .DMA = DMA0,
            .dma_chl = 12,
            .device_name = "i2c1",
            .baud = 100000U,
            .instance = 1U,
            .i2c_clock_id = kFRO12M_to_FLEXCOMM1,
        },
#endif
#ifdef BSP_USING_I2C4
        {
            .I2C = I2C4,
            .DMA = DMA0,
            .dma_chl = 13,
            .device_name = "i2c4",
            .baud = 400000U,
            .instance = 4U,
            .i2c_clock_id = kFRO12M_to_FLEXCOMM4,
        },
#endif
};

```

此时，RTduino关于i2c部分的引脚对接已经完成，下面我们进行RTduino的测试。

#### 4、LPC55s69驱动RTduino（i2c）

(1) 首先打开env工具，使能RTduino框架`Location: -> Hardware Drivers Config-> Onboard Peripheral Drivers-> Compatible with Arduino Ecosystem (RTduino)`

![image-20230227173448431](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302271734514.png)

拉取软件包索引至本地，并使用env工具生成MDK工程：

```c
pkgs --update
    
scons --target=mdk5
```

此时在工程中就可以看到RTduino所对应的软件包及arduino_main.cpp了

![image-20230227173614508](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302271736647.png)

我们添加测试代码，同时确保AHT10连接到LPC55s69开发板的A4和A5号引脚。

```cpp
#include <Arduino.h>
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C test");
}

void loop() {
  Wire.beginTransmission(0x50);  // I2C地址为0x50的设备
  Wire.write(0x00);              // 寄存器地址
  Wire.write(0xFF);              // 数据
  Wire.endTransmission();

  delay(500);	

  Wire.beginTransmission(0x50);  // I2C地址为0x50的设备
  Wire.write(0x00);              // 寄存器地址
  Wire.endTransmission();

  Wire.requestFrom(0x50, 1);     // 读取一个字节的数据

  if (Wire.available()) {
    byte data = Wire.read();
    Serial.print("Received data: ");
    Serial.println(data, HEX);
  } else {
    Serial.println("No data received");
  }

  delay(500);
}
```

![image-20230227174003096](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302271740178.png)

我们将LPC开发板的

## 三、uart测试

有了前面的经验，这里我们直接上代码测试，在arduino_main.cpp文件中添加如下代码：

```cpp
#include <Arduino.h>

void setup() {
  Serial.begin(115200); // 初始化串口通信，波特率为 115200
}

void loop() {
	// 向串口发送数据
	Serial.write("Hello RTduino!");
	delay(1000); // 等待 1 秒
}
```

全局搜索rtconfig.h文件，修改`RT_CONSOLE_DEVICE_NAME`为"uart2"，对接RTduino的uart设备

![image-20230227214053678](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302272140760.png)

我们使用一个USB转TTL模块，分别连接RX、TX到单片机的D0和D1，打开串口助手，波特率为115200，找到对应的USB设备，复位开发板，即可观察到uart设备成功在console控制台跑起来了。由于RTduino立项不久，所以还有一些功能还未完善，这里也仅仅能使用Serial.write函数，不过只要RTduino的BSP框架搭建好了，后续等待RTduino进一步完善就可以畅玩Arduino了！

![image-20230227213523916](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302272135982.png)

## 四、结语

目前的i2c及uart部分已经全部通过测试，后续的外设也会一一对接完成，各位感兴趣的小伙伴可以尝尝新，也欢迎大家一起交流！
