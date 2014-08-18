#include <linux/module.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

LIST_HEAD(my_head);

struct list_node {
	int id;
	struct list_head list;
};


static __init int minit(void)
{
	int i;
	struct list_node *my_list_node;

	printk(KERN_DEBUG "Hello World!\n");

	for (i = 0; i < 5; i++) {
		my_list_node = kmalloc(sizeof(struct list_node), GFP_KERNEL);
		my_list_node->id = i;
		list_add(&my_list_node->list, &my_head);
		printk("add %d\n", my_list_node->id);
	}

	return 0;
}

static __exit void mexit(void)
{
	struct list_node *my_list_node;
   	struct list_head *pos, *tmp;

	list_for_each_safe(pos, tmp, &my_head) {
		list_del(pos);
		my_list_node = list_entry(pos, struct list_node, list);
		printk("del %d\n", my_list_node->id);
		kfree(my_list_node);
	}
}

module_init(minit)
module_exit(mexit)

