## 1.宏定义分析

#### SPI工作时序

```c
#define RT_SPI_CPHA     (1<<0)                             /* bit[0]:CPHA, clock phase */
#define RT_SPI_CPOL     (1<<1)                             /* bit[1]:CPOL, clock polarity */
```

* RT_SPI_CPHA(时钟相位)：表示**在哪个时钟沿采样数据**，CPHA为0表示在首个时钟变化沿采样数据，而CPHA为1则表示在第二个时钟变化沿采样数据。

* RT_SPI_CPOL(时钟极性)：表示**时钟信号的初始电平的状态**，CPOL为0表示时钟信号初始状态为低电平，为1表示时钟信号的初始电平是高电平。

```c
#define RT_SPI_MODE_0       (0 | 0)                        /* CPOL = 0, CPHA = 0 */
#define RT_SPI_MODE_1       (0 | RT_SPI_CPHA)              /* CPOL = 0, CPHA = 1 */
#define RT_SPI_MODE_2       (RT_SPI_CPOL | 0)              /* CPOL = 1, CPHA = 0 */
#define RT_SPI_MODE_3       (RT_SPI_CPOL | RT_SPI_CPHA)    /* CPOL = 1, CPHA = 1 */
```

#### SPI传输位序

```c
#define RT_SPI_LSB      (0<<2)                             /* bit[2]: 0-LSB */
#define RT_SPI_MSB      (1<<2)                             /* bit[2]: 1-MSB */
```

* RT_SPI_LSB(低位传输)

* RT_SPI_MSB(高位传输)

#### SPI接口模式

```c
#define RT_SPI_MASTER   (0<<3)                             /* SPI master device */
#define RT_SPI_SLAVE    (1<<3)                             /* SPI slave device */
```

* RT_SPI_MASTER(主机)
* RT_SPI_SLAVE(从机)

#### SPI片选控制方式

```c
#define RT_SPI_CS_HIGH  (1<<4)                             /* Chipselect active high */
#define RT_SPI_NO_CS    (1<<5)                             /* No chipselect */
#define RT_SPI_3WIRE    (1<<6)                             /* SI/SO pin shared */
#define RT_SPI_READY    (1<<7)                             /* Slave pulls low to pause */
```

---

## 2.SPI结构体

#### SPI message structure

```c
struct rt_spi_message
{
    const void *send_buf;
    void *recv_buf;
    rt_size_t length;
    struct rt_spi_message *next;

    unsigned cs_take    : 1;
    unsigned cs_release : 1;
};
```

#### SPI configuration structure

```c
struct rt_spi_configuration
{
    rt_uint8_t mode;
    rt_uint8_t data_width;
    rt_uint16_t reserved;

    rt_uint32_t max_hz;
};
```

#### spi bus structure

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

#### SPI operators structure

```c
struct rt_spi_ops
{
    rt_err_t (*configure)(struct rt_spi_device *device, struct rt_spi_configuration *configuration);
    rt_uint32_t (*xfer)(struct rt_spi_device *device, struct rt_spi_message *message);
};
```

#### rt_spi_device structure

```c
struct rt_spi_device
{
    struct rt_device parent;
    struct rt_spi_bus *bus;

    struct rt_spi_configuration config;
    void   *user_data;
};
```

---

## 3.SPI操作函数

#### rt_spi_bus_register

```c
/* register a SPI bus */
rt_err_t rt_spi_bus_register(struct rt_spi_bus       *bus,
                             const char              *name,
                             const struct rt_spi_ops *ops);
```

#### rt_spi_bus_attach_device

```c
/* attach a device on SPI bus */
rt_err_t rt_spi_bus_attach_device(struct rt_spi_device *device,
                                  const char           *name,
                                  const char           *bus_name,
                                  void                 *user_data);
```

#### rt_spi_take_bus

```c
/**
 * This function takes SPI bus.
 *
 * @param device the SPI device attached to SPI bus
 *
 * @return RT_EOK on taken SPI bus successfully. others on taken SPI bus failed.
 */
rt_err_t rt_spi_take_bus(struct rt_spi_device *device);
```

#### rt_spi_release_bus

```c
/**
 * This function releases SPI bus.
 *
 * @param device the SPI device attached to SPI bus
 *
 * @return RT_EOK on release SPI bus successfully.
 */
rt_err_t rt_spi_release_bus(struct rt_spi_device *device);
```

#### rt_spi_take

```c
/**
 * This function take SPI device (takes CS of SPI device).
 *
 * @param device the SPI device attached to SPI bus
 *
 * @return RT_EOK on release SPI bus successfully. others on taken SPI bus failed.
 */
rt_err_t rt_spi_take(struct rt_spi_device *device);
```

#### rt_spi_release

```c
/**
 * This function releases SPI device (releases CS of SPI device).
 *
 * @param device the SPI device attached to SPI bus
 *
 * @return RT_EOK on release SPI device successfully.
 */
rt_err_t rt_spi_release(struct rt_spi_device *device);
```

#### rt_spi_configure

```c
/* set configuration on SPI device */
rt_err_t rt_spi_configure(struct rt_spi_device        *device,
                          struct rt_spi_configuration *cfg);
```

#### rt_spi_send_then_recv &rt_spi_send_then_send

```c
/* send data then receive data from SPI device */
rt_err_t rt_spi_send_then_recv(struct rt_spi_device *device,
                               const void           *send_buf,
                               rt_size_t             send_length,
                               void                 *recv_buf,
                               rt_size_t             recv_length);

rt_err_t rt_spi_send_then_send(struct rt_spi_device *device,
                               const void           *send_buf1,
                               rt_size_t             send_length1,
                               const void           *send_buf2,
                               rt_size_t             send_length2);
```

#### rt_spi_transfer

```c
/**
 * This function transmits data to SPI device.
 *
 * @param device the SPI device attached to SPI bus
 * @param send_buf the buffer to be transmitted to SPI device.
 * @param recv_buf the buffer to save received data from SPI device.
 * @param length the length of transmitted data.
 *
 * @return the actual length of transmitted.
 */
rt_size_t rt_spi_transfer(struct rt_spi_device *device,
                          const void           *send_buf,
                          void                 *recv_buf,
                          rt_size_t             length);
```

#### rt_spi_transfer_message

```c
/**
 * This function transfers a message list to the SPI device.
 *
 * @param device the SPI device attached to SPI bus
 * @param message the message list to be transmitted to SPI device
 *
 * @return RT_NULL if transmits message list successfully,
 *         SPI message which be transmitted failed.
 */
struct rt_spi_message *rt_spi_transfer_message(struct rt_spi_device  *device,
                                               struct rt_spi_message *message);
```

#### rt_spi_recv

```c
rt_inline rt_size_t rt_spi_recv(struct rt_spi_device *device,
                                void                 *recv_buf,
                                rt_size_t             length)
{
    return rt_spi_transfer(device, RT_NULL, recv_buf, length);
}
```

#### rt_spi_send

```c
rt_inline rt_size_t rt_spi_send(struct rt_spi_device *device,
                                const void           *send_buf,
                                rt_size_t             length)
{
    return rt_spi_transfer(device, send_buf, RT_NULL, length);
}
```

#### rt_spi_sendrecv8

```c
rt_inline rt_uint8_t rt_spi_sendrecv8(struct rt_spi_device *device,
                                      rt_uint8_t            data)
{
    rt_uint8_t value = 0;

    rt_spi_send_then_recv(device, &data, 1, &value, 1);

    return value;
}
```

#### rt_spi_sendrecv16

```c
rt_inline rt_uint16_t rt_spi_sendrecv16(struct rt_spi_device *device,
                                        rt_uint16_t           data)
{
    rt_uint16_t value = 0;

    rt_spi_send_then_recv(device, &data, 2, &value, 2);

    return value;
}
```

#### rt_spi_message_append

```c
/**
 * This function appends a message to the SPI message list.
 *
 * @param list the SPI message list header.
 * @param message the message pointer to be appended to the message list.
 */
rt_inline void rt_spi_message_append(struct rt_spi_message *list,
                                     struct rt_spi_message *message)
{
    RT_ASSERT(list != RT_NULL);
    if (message == RT_NULL)
        return; /* not append */

    while (list->next != RT_NULL)
    {
        list = list->next;
    }

    list->next = message;
    message->next = RT_NULL;
}
```
