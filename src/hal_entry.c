/*
 * �����嵥������һ�� ���� �豸ʹ������
 * ���̵����� uart_sample ��������ն�
 * ������ø�ʽ��uart_sample uart2
 * ������ͣ�����ڶ���������Ҫʹ�õĴ����豸���ƣ�Ϊ����ʹ��Ĭ�ϵĴ����豸
 * �����ܣ�ͨ����������ַ���"hello RT-Thread!"��Ȼ���λ���������ַ�
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

/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/* �������ݻص����� */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

/*uart0��������߳�*/
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

/*���������߳�*/
void control_thread_entry(void *parameter)
{
    char ch;
    while (1)
    {
        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* ��ȡ��������ͨ�����ڴ�λ��� */
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

/*��ʼ����������������Ϳ����߳�*/
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

    /* ����ϵͳ�еĴ����豸 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* ��ʼ���ź��� */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(serial, uart_input);
    /* �����ַ��� */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* ���� control_thread �߳� */
    rt_thread_t control_thread = rt_thread_create("control", control_thread_entry, RT_NULL, 1024, 30, 10);
    /* �����ɹ��������߳� */
    if (control_thread != RT_NULL)rt_thread_startup(control_thread);

    /* ���� serial_thread �߳� */
    rt_thread_t serial_thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* �����ɹ��������߳� */
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
/* ������ msh �����б��� */
MSH_CMD_EXPORT(uart_sample, uart device sample);
