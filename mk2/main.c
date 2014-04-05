#include <linux/module.h>
#include <fun.h>

MODULE_LICENSE("GPL");

static int s_init = 0;

module_param(s_init, int, S_IRUGO|S_IWUSR);

static __init int minit(void)
{
	printk("call %s: %d\n", __FUNCTION__, s_init);
	fun();
	return 0;
}

static __exit void mexit(void)
{
	printk("call %s\n", __FUNCTION__);
}

module_init(minit)
module_exit(mexit)
