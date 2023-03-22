![image-20230222202522100](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302222025186.png)

![image-20230222202458036](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302222024126.png)

![详解Arduino Uno开发板的引脚分配图及定义- Arduino专区- 一板网电子技术论坛- 手机版- Powered by Discuz!](https://www.yiboard.com/data/attachment/forum/202204/26/155358es4pjrscqrj4sa1d.png)

![image-20230225145238322](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202302251452741.png)



```cpp
我们常见在单片机中有ADC的通道功能：EXTVOL INTVOL INTTEP这三个功能什么意思?

"EXTVOL"、"INTVOL" 和 "INTTEP" 是一些通常与模拟-数字转换器 (ADC) 有关的术语。它们的含义如下：

EXTVOL (External Voltage)：指的是单片机 ADC 的输入通道，可以通过该通道将外部电压信号转换成数字信号。这种信号通常来自于外部传感器或其他外部电路。

INTVOL (Internal Voltage)：指单片机内部提供的参考电压，通常用于校准和测量外部电压信号。ADC 需要知道参考电压的大小，以便正确地将模拟电压转换为数字值。INTVOL 也被称为参考电压源。

INTTEP (Internal Temperature)：指的是单片机内部的温度传感器，它可以通过 ADC 的一个通道将温度转换为数字信号。这种信号通常用于监测单片机芯片的温度，并采取相应的措施来保护芯片免受过热的影响。

这些功能对于许多嵌入式系统来说都是非常重要的，因为它们提供了对外部环境和单片机内部状态的重要信息，可以帮助开发者进行精确的测量和控制。
```



