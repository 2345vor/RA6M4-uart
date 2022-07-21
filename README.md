@[toc](【Renesas RA6M4开发板之UART与Serial studio串口交互】)

# 1.0 UART简介
UART（Universal Asynchronous Receiver/Transmitter）通用异步收发传输器，UART 作为异步串口通信协议的一种，工作原理是将传输数据的每个字符一位接一位地传输。是在应用程序开发过程中使用频率最高的数据总线。

![在这里插入图片描述](https://img-blog.csdnimg.cn/33cdaa2e3db3494b98eeb3c3e56955ab.png)

## 1.1 原理
UART 串口的特点是将数据一位一位地顺序传送，只要 2 根传输线就可以实现双向通信，一根线发送数据的同时用另一根线接收数据。UART 串口通信有几个重要的参数，分别是波特率、起始位、数据位、停止位和奇偶检验位，对于两个使用 UART 串口通信的端口，这些参数必须匹配，否则通信将无法正常完成。UART 串口传输的数据格式如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3911ba1f77c549cf8b8a883f7966face.png)
- 起始位：表示数据传输的开始，电平逻辑为 “0” 。

- 数据位：可能值有 5、6、7、8、9，表示传输这几个 bit 位数据。一般取值为 8，因为一个 ASCII 字符值为 8 位。

- 奇偶校验位：用于接收方对接收到的数据进行校验，校验 “1” 的位数为偶数(偶校验)或奇数(奇校验)，以此来校验数据传送的正确性，使用时不需要此位也可以。

- 停止位： 表示一帧数据的结束。电平逻辑为 “1”。

- 波特率：串口通信时的速率，它用单位时间内传输的二进制代码的有效位(bit)数来表示，其单位为每秒比特数 bit/s(bps)。常见的波特率值有 4800、9600、14400、38400、115200等，数值越大数据传输的越快，波特率为 115200 表示每秒钟传输 115200 位数据。

## 1.2 访问 PWM 设备
应用程序通过 RT-Thread 提供的 PWM 设备管理接口来访问 PWM 设备硬件，相关接口如下所示：

|函数	|描述|
| ------ | ------ |
|函数|	描述|
|[rt_device_find()](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%E6%9F%A5%E6%89%BE%E4%B8%B2%E5%8F%A3%E8%AE%BE%E5%A4%87)|	查找设备|
|[rt_device_open()](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%E6%89%93%E5%BC%80%E4%B8%B2%E5%8F%A3%E8%AE%BE%E5%A4%87)	|打开设备|
|[rt_device_read()](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%E6%8E%A5%E6%94%B6%E6%95%B0%E6%8D%AE)	|读取数据|
|[rt_device_write()](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%E5%8F%91%E9%80%81%E6%95%B0%E6%8D%AE)	|写入数据|
|[rt_device_control()](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%E6%8E%A7%E5%88%B6%E4%B8%B2%E5%8F%A3%E8%AE%BE%E5%A4%87)	|控制设备|
|[rt_device_set_rx_indicate()](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%E8%AE%BE%E7%BD%AE%E6%8E%A5%E6%94%B6%E5%9B%9E%E8%B0%83%E5%87%BD%E6%95%B0)	|设置接收回调函数|
|[rt_device_set_tx_complete()](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%E8%AE%BE%E7%BD%AE%E5%8F%91%E9%80%81%E5%AE%8C%E6%88%90%E5%9B%9E%E8%B0%83%E5%87%BD%E6%95%B0)	|设置发送完成回调函数|
|[rt_device_close()](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%E5%85%B3%E9%97%AD%E4%B8%B2%E5%8F%A3%E8%AE%BE%E5%A4%87)	|关闭设备|

# 2. RT-theard配置
## 2.1 硬件需求
> 实现功能：
> 采用UART0输出数据，Serial Studio作为上位机接收数据，显示对应端口的状态和传感器的动态显示，并且实现简单控制。

1、RA6M4开发板
![在这里插入图片描述](https://img-blog.csdnimg.cn/4c5dcda23c6d4afaacb393dc46a7ae51.png)
2、USB下载线，两个ch340串口和附带4根母母线，第一路串口是UART0端口**rx---p101;tx---p100**，实现Serial Studio上位机交互操作和可视化；第二路串口是UART7端口**rx---p613;tx---p614**，实现CMD命令输入。其他不变
![在这里插入图片描述](https://img-blog.csdnimg.cn/51aa8153f6434230a25eb2bae3ae62b5.png)

硬件到此配置完成


## 2.2 软件配置
Renesas RA6M4开发板环境配置参照：[【基于 RT-Thread Studio的CPK-RA6M4 开发板环境搭建】](https://blog.csdn.net/vor234/article/details/125634313)
1、新建项目RA6M4-uart工程
![在这里插入图片描述](https://img-blog.csdnimg.cn/db8cad1586bd406784650f04192d17c5.png)


2、查阅RA6M4硬件资源，相关资料，在RT-theard Setting 硬件中需要使能uart0
![在这里插入图片描述](https://img-blog.csdnimg.cn/6202d5be8bb3447597ed6599c9b8b5ab.png)


3、打开RA Smart Congigurator，在Stacks中New Stack添加r_gpt
![在这里插入图片描述](https://img-blog.csdnimg.cn/eac41ce0974b45e1a1264ad8e1bad7b2.png)


4、在Property的Module的General中name定义为`g_uart0`,Channel选0，回调函数改为`user_uart0_callback` ,Pins选择`P100和P101`
![在这里插入图片描述](https://img-blog.csdnimg.cn/d7a97e99426841c59fef8368b7383c9c.png)
5、然后Generate Project Content 同步更新刚刚配置的文件

图形化配置已经完成，接下来配置相关代码
# 3. 代码分析
1、修改`hal_entry.c`函数，屏蔽LED3普通GPIO输出

```cpp
/*
 * 程序清单：这是一个 串口 设备使用例程
 * 例程导出了 uart_sample 命令到控制终端
 * 命令调用格式：uart_sample
 * 命令解释：命令第二个参数是要使用的串口设备名称，为空则使用默认的串口设备
 * 程序功能：通过串口输出字符串"hello RT-Thread!"，然后输出输入的字符
*/

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>
#include <stdio.h>
#include <string.h>

#define LED3_PIN    BSP_IO_PORT_01_PIN_06

void hal_entry(void)
{
    rt_kprintf("\nHello RT-Thread!\n");

    while (1)
    {
        rt_thread_mdelay(10);
    }
}

#define SAMPLE_UART_NAME       "uart0"

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

/*uart0串口输出线程*/
void serial_thread_entry(void *parameter)
{

    char str1[50];
    char HeadByte[50] ="/*KAANSATQRO";
    while (1)
    {
        sprintf(str1, "%s,%d", HeadByte, rt_pin_read(LED3_PIN));
        sprintf(str1, "%s,%d", str1,rt_pin_read(LED3_PIN));
        sprintf(str1, "%s%s", str1,"*/ \r\n");

        rt_device_write(serial, 0, str1, (sizeof(str1) - 1));
        rt_thread_mdelay(200);
    }
}

/*控制命令线程*/
void control_thread_entry(void *parameter)
{
    char ch;
    while (1)
    {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* 读取到的数据通过串口错位输出 */
        ch = ch + 0;
        if(ch=='1')
        {
            rt_pin_write(LED3_PIN, PIN_HIGH);
            rt_thread_mdelay(50);
            rt_kprintf("\n PIN_HIGH \n");
        }
        else if(ch=='0')
        {
            rt_pin_write(LED3_PIN, PIN_LOW);
            rt_thread_mdelay(50);
            rt_kprintf("\n PIN_LOW \n");
        }
        else {
            rt_kprintf("\n error \n");
        }
        rt_device_write(serial, 0, &ch, 1);
        rt_thread_mdelay(5);
    }
}

/*初始化和启动串口输出和控制线程*/
static int uart_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    char str[] = "hello RT-Thread!\r\n";
    rt_kprintf("Hello RT-Thread!\n");
    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }

    /* 查找系统中的串口设备 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);
    /* 发送字符串 */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* 创建 control_thread 线程 */
    rt_thread_t control_thread = rt_thread_create("control", control_thread_entry, RT_NULL, 1024, 30, 10);
    /* 创建成功则启动线程 */
    if (control_thread != RT_NULL)rt_thread_startup(control_thread);

    /* 创建 serial_thread 线程 */
    rt_thread_t serial_thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (serial_thread != RT_NULL)
    {
        rt_thread_startup(serial_thread);
    }
    else
    {
        ret = RT_ERROR;
    }
    return ret;

}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(uart_sample, uart device sample);

```

其他不变。

**保存完是灰色，没有保存是蓝色。**
`uart_sample`导 出 到 msh 命 令 列 表 中，实现pwm8的两路输出


# 4. 下载验证
1、编译重构
![在这里插入图片描述](https://img-blog.csdnimg.cn/1be071528909403793520449f4afc22c.png)

编译成功

2、下载程序
![在这里插入图片描述](https://img-blog.csdnimg.cn/5964b3a9e1234451b07d001bfc57d185.png)
下载成功

3、CMD串口调试

![在这里插入图片描述](https://img-blog.csdnimg.cn/181227ee2ed64ef2801477ece50cf41c.png)
然后板载复位，输入：`uart_sample`

此时可以与Serial Studio一起联动啦！🎉🎉🎉
请参照[【开源的串口可视化工具——Serial Studio】](https://blog.csdn.net/VOR234/article/details/125910113)
效果如下
![请添加图片描述](https://img-blog.csdnimg.cn/e165573593de4fcebc29671cec17313b.gif)

这样我们就可以天马行空啦!
![请添加图片描述](https://img-blog.csdnimg.cn/92099d4d054b4b2cbd39b95719739a90.gif)

参考文献；
[【基于 RT-Thread Studio的CPK-RA6M4 开发板环境搭建】](https://blog.csdn.net/vor234/article/details/125634313)
[【开源的串口可视化工具——Serial Studio】](https://blog.csdn.net/VOR234/article/details/125910113)
