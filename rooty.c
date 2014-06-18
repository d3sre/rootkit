#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
int rooty_init(void);
void rooty_exit(void);
module_init(rooty_init);
module_exit(rooty_exit);

int rooty_init(void) {
#  list_del_init(&__this_module.list);
#  kobject_del(&THIS_MODULE->mkobj.kobj);
  
 printk("rooty: module loaded\n");
 return 0;
}

void rooty_exit(void) {
 printk("rooty: module removed\n");
}
