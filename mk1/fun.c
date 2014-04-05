#include <linux/module.h>
#include <fun.h>

void fun(void)
{
	printk("call %s\n", __FUNCTION__);
}
EXPORT_SYMBOL(fun);
