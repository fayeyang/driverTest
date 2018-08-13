#include  <linux/kernel.h>
#include  <linux/module.h>
#include  <linux/init.h>
#include  <linux/string.h>
#include  <linux/device.h>

MODULE_AUTHOR( "faye" );
MODULE_LICENSE( "GPL" );    /* 注意,本行不可省略,否则即使能成功编译,但在加载本模块时,会提示"Unknown symbol in module",并会在dmesg命令中给出所缺少的符号 */

char author[ PAGE_SIZE+1 ] = "FayeYang";
char class_attr_Buf[ PAGE_SIZE+1 ] = "faye_class_attribute data";
char class_attrGroup_Buf[ PAGE_SIZE+1 ] = "faye_class_attribute_group data";

static int faye_class_dev_uevent( struct device *dev, struct kobj_uevent_env *env ){
	printk( "~~~~~~~ in faye_class_dev_uevent() start ~~~~~~~\n" );
	printk( "device is: %s\n", dev_name(dev) );
	printk( "~~~~~~~ in faye_class_dev_uevent() end ~~~~~~~\n" );
	return 0;
}

void faye_class_cls_release( struct class *cls ){
	printk( "~~~~~~~ in faye_class_cls_release() start ~~~~~~~\n" );
	printk( "class is: %s\n", cls->name );
	printk( "~~~~~~~ in faye_class_cls_release() end ~~~~~~~\n" );
}

void faye_class_dev_release( struct device *dev ){
	printk( "~~~~~~~ in faye_class_dev_release() start ~~~~~~~\n" );
	printk( "device is: %s\n", dev_name(dev) );
	printk( "~~~~~~~ in faye_class_dev_release() end ~~~~~~~\n" );
}

static ssize_t faye_class_attrGroup_show( struct class *class, struct class_attribute *attr, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", class_attrGroup_Buf );
}
static ssize_t faye_class_attrGroup_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ){
	return sprintf( class_attrGroup_Buf, "%s", buf );	
}
static CLASS_ATTR_RW( faye_class_attrGroup );

struct attribute_group faye_class_attrGroup = {
	.name = "faye_class_attrGroup",
	.attrs = (struct attribute*[]){ &(class_attr_faye_class_attrGroup.attr), NULL },
};

struct class faye_class = {
	.name         = "faye_class",
	.dev_uevent   =  faye_class_dev_uevent,
	.class_release = faye_class_cls_release,
	.dev_release  =  faye_class_dev_release,
	.dev_groups   = (const struct attribute_group*[]){ &faye_class_attrGroup, NULL },
};
EXPORT_SYMBOL( faye_class );

/* 定义一个class_attribute对象 */
static ssize_t faye_class_author_show( struct class *class, struct class_attribute *attr, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", author );
}
static ssize_t faye_class_author_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ){
	return sprintf( author, "%s", buf );	
}
static CLASS_ATTR_RW( faye_class_author );

/* 定义一个class_attribute对象 */
static ssize_t faye_class_attr_show( struct class *class, struct class_attribute *attr, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", class_attr_Buf );
}
static ssize_t faye_class_attr_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ){
	return sprintf( class_attr_Buf, "%s", buf );	
}
static CLASS_ATTR_RW( faye_class_attr );

static int __init faye_class_init( void ){
	
	int tmp;

    printk( "/**** faye_class_init() start ***************************************/\n" );
	
	tmp = 0;
	tmp = class_register( &faye_class );
	if( tmp ){
		printk( KERN_DEBUG "Unable to register faye_class\n" );
		return tmp;
	}


	if( class_create_file( &faye_class, &class_attr_faye_class_author ) ){
		printk( KERN_DEBUG "Unable to create faye_class author file\n" );
		return tmp;
	}

	if( class_create_file( &faye_class, &class_attr_faye_class_attr ) ){
		printk( KERN_DEBUG "Unable to create faye_class attribute file\n" );
		return tmp;
	}

	printk( KERN_DEBUG "faye_class register success\n" );
    printk( "/**** faye_class_init() end ***************************************/\n" );

	return 0;
}

static void __exit faye_class_exit( void ){
	printk( "/**** faye_class_exit() start ***************************************/\n" );
	
	class_unregister( &faye_class );

	printk( "faye_class exit success!\n" );
	printk( "/**** faye_class_exit() end ***************************************/\n" );
}

module_init( faye_class_init );
module_exit( faye_class_exit );

