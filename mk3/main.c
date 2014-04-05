#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This module is used for kernel thread test.");


static unsigned int bitmap;

static void set_read_bitmap(int reader)
{
	bitmap = 0;
	while (reader) {
		bitmap |= (1 << --reader);
	}
}

struct _my_data {
	int count1;
	int count2;
};

static struct _my_data my_data;
static struct _my_data __rcu *pmy_data = &my_data;

static void show_my_data(void)
{
	printk("count1 %d, count2 %d\n", pmy_data->count1, pmy_data->count2);
}

#define MAX_KTHREAD 10

static struct task_struct *threads[MAX_KTHREAD];

static void reader_do(void)
{
	struct _my_data *data;

	rcu_read_lock();
	data = rcu_dereference(pmy_data);
	printk("read count1 %d, count2 %d.\n", data->count1, data->count2);
	rcu_read_unlock();
}

static void writer_do(void)
{
	struct _my_data *data, *tmp = pmy_data;

	data = kmalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return;

	/* Read & Copy */
	memcpy(data, pmy_data, sizeof(*data));
	data->count1 ++;
	data->count2 += 10;

	rcu_assign_pointer(pmy_data, data);//pmy_data = data;
	
	if (tmp != &my_data) {
		synchronize_rcu();
		kfree(tmp);
	}
}

static int thread_do(void *data)
{
	long int i = (long int)data;
	int reader = bitmap & (1 << i);
	printk("run %ld %s...\n", i, reader ? "reader" : "writer");
	while(!kthread_should_stop()) {
		if (reader)
			reader_do();
		else 
			writer_do();
		msleep(20);
	}
	return 0;
}

static int create_threads(void)
{
	long int i;

	set_read_bitmap(9);
	for (i=0; i<MAX_KTHREAD; i++) {
		struct task_struct *thread;
		thread = kthread_run(thread_do, (void*)i, "thread-%ld", i);
		if (IS_ERR(thread))
			return -1;

		threads[i] = thread;
	}

	return 0;
}

static int cleanup_threads(void)
{
	int i;

	for (i=0; i<MAX_KTHREAD; i++) {
		if (threads[i])
			kthread_stop(threads[i]);	
	}

	return 0;
}

static __init int minit(void)
{
	printk("call %s\n", __FUNCTION__);
	if (create_threads())
		goto err;
	return 0;
err:
	cleanup_threads();
	return -1;
}

static __exit void mexit(void)
{
	printk("call %s\n", __FUNCTION__);
	show_my_data();
	cleanup_threads();
}

module_init(minit)
module_exit(mexit)
