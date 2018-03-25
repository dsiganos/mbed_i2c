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

static struct kobject *example_kobject;
static int foo;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf, size_t count)
{
        sscanf(buf, "%du", &foo);
        return count;
}


static struct kobj_attribute foo_attribute =__ATTR(foo, 0660, foo_show, (void *) foo_store);

static int __init my_sysfs_init(void)
{
        int error = 0;

        pr_debug("Module initialized successfully \n");

        example_kobject = kobject_create_and_add("kobject_example", kernel_kobj);
        if(!example_kobject) return -ENOMEM;

        error = sysfs_create_file(example_kobject, &foo_attribute.attr);
        if (error) {
                pr_debug("failed to create the foo file in /sys/kernel/kobject_example \n");
        }

        return error;
}

static void __exit my_sysfs_exit (void)
{
        pr_debug ("Module un initialized successfully \n");
        kobject_put(example_kobject);
}
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
    my_sysfs_init();

	return 0;
}

void i2c_test_exit(void)
{
    printk(KERN_INFO "Goodbye world\n");
    my_sysfs_exit();
    i2c_smbus_write_byte(i2c_client, 0);
    i2c_unregister_device(i2c_client);
}

module_init(i2c_test_init);
module_exit(i2c_test_exit);

