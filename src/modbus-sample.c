#include "modbus.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>

//#define RS485_RE GET_PIN(G, 8)

static void test_thread(void *param)
{
		rt_kprintf("=== test_thread in ===\n");
    uint8_t tab_reg[64] = {0};
    modbus_t *ctx = RT_NULL;
    ctx = modbus_new_gd_rtu("uart2", 9600, 'N', 8, 1);
		rt_kprintf("=== test_thread create rtu ===\n");
//    modbus_rtu_set_rts(ctx, RS485_RE, MODBUS_RTU_RTS_UP);
    modbus_set_slave(ctx, 2);
    modbus_connect(ctx);
    modbus_set_response_timeout(ctx, 0, 200 * 1000);
    int num = 0;
		
    while (1)
    {
//				rt_kprintf("=== test_thread loop ===\n");
        memset(tab_reg, 0, 64);
        int regs = modbus_read_input_bits(ctx, 0, 8, tab_reg);
        rt_kprintf("-------------------------------------------\n");
        rt_kprintf("[%4d][read num = %d]", num, regs);
        num++;
        int i;
        for (i = 0; i < 8; i++)
        {
            rt_kprintf("<%#x>", tab_reg[i]);
        }
        rt_kprintf("\n");
        rt_kprintf("-------------------------------------------\n");
        rt_thread_mdelay(5000);
    }
    //7-关闭modbus端口
    modbus_close(ctx);

    //8-释放modbus资源
    modbus_free(ctx);
}

static int rtu_test_init(void)
{
//    rt_pin_mode(RS485_RE, PIN_MODE_OUTPUT);
    rt_thread_t tid;
    tid = rt_thread_create("test",
                           test_thread, RT_NULL,
                           2048,
                           12, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return RT_EOK;
}
INIT_APP_EXPORT(rtu_test_init);