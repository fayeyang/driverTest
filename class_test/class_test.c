#include  <linux/kernel.h>
#include  <linux/module.h>
#include  <linux/init.h>
#include  <linux/string.h>
#include  <linux/device.h>

char author[ PAGE_SIZE+1 ] = "FayeYang";
char class_attr_Buf[ PAGE_SIZE+1 ] = "faye_class_attribute data";
char class_attrGroup_Buf[ PAGE_SIZE+1 ] = "faye_class_attribute_group data";

static int faye_class_dev_uevent( struct device *dev, struct kobj_uevent_env *env ){
	printk( "~~~~~~~ in faye_class_dev_uevent() start ~~~~~~~\n" );
	printk( "device is: %s\n", dev_name(dev) );
	printk( "~~~~~~~ in faye_class_dev_uevent() end ~~~~~~~\n" );
	return 0;
}

void faye_class_dev_release( struct device *dev ){
	printk( "~~~~~~~ in faye_class_dev_release() start ~~~~~~~\n" );
	printk( "device is: %s\n", dev_name(dev) );
	printk( "~~~~~~~ in faye_class_dev_release() end ~~~~~~~\n" );
}

/* 定义一个class_attribute对象 */
static ssize_t class_author_show( struct class *class, struct class_attribute *attr, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", author );
}
static ssize_t class_author_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ){
	return sprintf( author, "%s", buf );	
}
static CLASS_ATTR( faye_class_author, 0x06, class_author_show, class_author_store );
#define __ATTR_RW(_name) __ATTR(_name, 0644, _name##_show, _name##_store)

static int __init faye_class_init( void ){
	return 0;
}

static void __init faye_class_exit( void ){
	
}

module_init( faye_class_init );
module_exit( faye_class_exit );

