/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

// lab 4
static uint howmany = 1;
module_param(howmany, uint, S_IRUGO);
MODULE_PARM_DESC(howmany, "Number of times to print 'Hello, world!' string.");

// lab 5
struct my_data {
	struct list_head list_node;
	ktime_t ktm;
};
static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{
	ktime_t ktm;
	struct my_data *ptr, *tmp;

	uint i = 0;

	if (howmany == 0 || ((howmany >= 5) && (howmany <= 10))) {
		printk(KERN_EMERG "Attention!\nYou have typed the value that is 0 or in range [5, 10].");
	} else if (howmany > 10) {
		return -EINVAL;
	}

	for (; i < howmany; i++) {
		ktm = ktime_get();
		printk(KERN_EMERG "Hello, world!\n");

		// allocating memory
		ptr = kmalloc(sizeof(struct my_data), GFP_KERNEL);

		// handling error
		if (ZERO_OR_NULL_PTR(ptr))
			goto err_exit;

		*ptr = (struct my_data)
		{
			(struct list_head) {NULL, NULL}, ktm
		};
		list_add_tail(&ptr->list_node, &my_list_head);
	}
	return 0;

err_exit:
		list_for_each_entry_safe(ptr, tmp, &my_list_head, list_node) {
			list_del(&ptr->list_node);
			kfree(ptr);
		};
		//kfree(ptr);
		printk(KERN_ERR "Error: check your memory.\n");
		return -1;
}

static void __exit hello_exit(void)
{
	struct my_data *ptr, *tmp;

	int i = 0;

	list_for_each_entry_safe(ptr, tmp, &my_list_head, list_node) {
		i++;
		printk(KERN_INFO "time of the %d unit is %lld\n", i, ptr->ktm);
		list_del(&ptr->list_node);
		kfree(ptr);
	};
	printk(KERN_EMERG "This module was annihilated.\n");
}

module_init(hello_init);
module_exit(hello_exit);
