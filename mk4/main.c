#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

#define MAX_LIST 10

struct our_data {
	int i;
	struct list_head list;
};

static void list_test(void)
{
	int i;
	struct our_data *node, *next;
	LIST_HEAD(lhead);

	for (i=0; i<MAX_LIST; i++) {
		struct our_data *data;

		data = kmalloc(sizeof(*data), GFP_KERNEL);
		if (!data)
			goto clean;

		data->i = i;
		list_add(&data->list, &lhead);
	}
	
	list_for_each_entry(node, &lhead, list)
		printk("list entry: %d.\n", node->i);

clean:
	list_for_each_entry_safe(node, next, &lhead, list)
		kfree(node);
}

static __init int minit(void)
{
	printk("call %s\n", __FUNCTION__);
	list_test();
	return 0;
}

static __exit void mexit(void)
{
	printk("call %s\n", __FUNCTION__);
}

module_init(minit)
module_exit(mexit)
