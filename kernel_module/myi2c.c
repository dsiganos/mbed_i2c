/*  
 *  i2c_test.c
 */

#include <linux/slab.h>			/* kzalloc */
#include <linux/module.h>   	/* Needed by all modules */
#include <linux/kernel.h>   	/* KERN_INFO */
#include <linux/timer.h>		/* timer_list */
#include <linux/workqueue.h>	/* schedule_work */

#include <linux/input.h>
#include <linux/i2c.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jack Newcombe");
MODULE_DESCRIPTION("Manages a single i2c analog stick using a pcf8591 ADC");
MODULE_VERSION("0.1");

struct i2c_adapter* i2c_dev;
struct i2c_client* i2c_client;

static struct i2c_board_info __initdata board_info[] =  {
	{
		I2C_BOARD_INFO("dimitris_i2c", 0x10),
	}
};

int i2c_test_init(void)
{
	printk(KERN_INFO "loading i2c driver v0.1.\n");
	
	// Setup device
	i2c_dev = i2c_get_adapter(2);
	i2c_client = i2c_new_device(i2c_dev, board_info);

	i2c_smbus_write_byte(i2c_client, 65);
//	i2c_smbus_read_byte(i2c_client); // Flush first byte
//    i2c_smbus_read_byte(i2c_client);

	return 0;
}

void i2c_test_exit(void)
{
    printk(KERN_INFO "Goodbye world\n");
    i2c_smbus_write_byte(i2c_client, 0);
    i2c_unregister_device(i2c_client);
}

module_init(i2c_test_init);
module_exit(i2c_test_exit);

