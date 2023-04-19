RT-Thread spi驱动框架中，当用户手动初始化一个总线设备(rt_spi_bus)并进行设备挂载后，会相应的注册一个从设备(rt_spi_device)。

1.总线设备框架如下：

```c
struct rt_spi_bus
{
    struct rt_device parent;
    rt_uint8_t mode;
    const struct rt_spi_ops *ops;
    
    struct rt_mutex lock;
    struct rt_spi_device *owner;
};
```

2.硬件spi初始化流程(rt_hw_spi_init)：

```c
rt_hw_spi_bus_init();
    ->rt_spi_bus_register();
		->rt_spi_bus_device_init();
            ->rt_ddevice_register();
```

3.从设备框架：

```c
struct rt_spi_device
{
    struct rt_device parent;
    struct rt_spi_bus *bus;
    
    struct rt_spi_configuration config;
    void *user_data;
};
```

4.总线挂载从设备

```c
rt_err_t rt_spi_bus_attach_device(struct rt_spi_device *device,
                                  const char           *name,
                                  const char           *bus_name,
                                  void                 *user_data)
{
	……
    rt_spidev_device_init(device, name);
    	->rt_device_register(device, name, RT_DEVICE_FLAG_RDWR);
    ……
}
```























5.struct rt_spi_bit_ops

主要定义了模拟SPI总线时序时需要的数据结构，这里便携模拟SPI底层驱动时需要适配这个结构

- data : 一般用于关联GPIO操作端口、引脚、时钟等配置信息
- tog_sclk : SCLK脚电平翻转函数
- set_sclk : 配置SCLK脚电平状态函数
- set_mosi : 配置MOSI脚电平状态函数
- set_miso : 配置MISO脚电平状态函数
- get_sclk : 获取SCLK脚电平状态函数
- get_mosi : 获取MOSI脚电平状态函数
- get_miso : 获取MISO脚电平状态函数
- dir_mosi : 配置MOSI脚输入输出状态函数
- dir_miso : 配置MISO脚输入输出状态函数
- udelay : 微秒延时函数
- delay_us : 微秒延时时间，不用用户设定

```c
static struct rt_spi_bit_ops lpc_soft_spi_ops =
    {
        .data = RT_NULL,
        .tog_sclk = lpc_tog_sclk,
        .set_sclk = lpc_set_sclk,
        .set_mosi = lpc_set_mosi,
        .set_miso = lpc_set_miso,
        .get_sclk = lpc_get_sclk,
        .get_mosi = lpc_get_mosi,
        .get_miso = lpc_get_miso,
        .dir_mosi = lpc_dir_mosi,
        .dir_miso = lpc_dir_miso,
        .udelay = lpc_udelay,
        .delay_us = 1,
};
```

