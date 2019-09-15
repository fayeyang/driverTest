#include  <linux/kernel.h>
#include  <linux/module.h>
#include  <linux/init.h>
#include  <linux/string.h>
#include  <linux/device.h>
//#include  <asm/uaccess.h>
//#include  <linux/uaccess.h>

MODULE_AUTHOR( "faye" );
MODULE_LICENSE( "GPL" );    /* 
			* 注意,本行不可省略,否则即使能成功编译,但在加载本模块时,
			* 会提示"Unknown symbol in module",并会在dmesg命令中给出所缺少的符号 */

extern struct bus_type faye_bus;

char author[ PAGE_SIZE+1 ] = "faye";
char driver_attr1_Buf[ PAGE_SIZE+1 ] = "driver_attribute_1 data";
char driver_attr2_Buf[ PAGE_SIZE+1 ] = "driver_attribute_2 data";

/* driver_attribute对象缓冲区 */
char driver_attrGroup_Buf[ PAGE_SIZE+1 ] = "driver attribute of attribute_Group";

int faye_driver_remove( struct device *dev ){
	printk( "*** in faye_driver_remove() start ***\n" );
	printk(  "device is:%s\n", dev_name(dev) );
	printk( "*** in faye_driver_remove() end ***\n" );
	return 0;
}

int faye_driver_probe( struct device *dev ){
	printk( "*** in faye_driver_probe() start ***\n" );
	printk(  "device is:%s\n", dev_name(dev) );
	printk( "*** faye device driver can handle the device ***\n" );
	printk( "*** in faye_driver_probe() end ***\n" );
	return 0;
}

void faye_driver_shutdown( struct device *dev ){
	printk( "*** in faye_driver_shutdown() start ***\n" );
	printk(  "device is:%s\n", dev_name(dev) );
	printk( "*** in faye_driver_shutdown() end ***\n" );
}

/* 定义一个device_attribute对象,将其封装到attribute_group对象中 */
static ssize_t faye_driver_attribute_group_show( struct device_driver *drv, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", driver_attrGroup_Buf );
}
static ssize_t faye_driver_attribute_group_store( struct device_driver *drv, const char *buf, size_t count ){
	return sprintf( driver_attrGroup_Buf, "%s", buf );
}
static DRIVER_ATTR_RW( faye_driver_attribute_group );

struct attribute_group faye_driver_attrGroup = {
	.name  = "faye_driver_attrGroup_name",
	.attrs = ( struct attribute*[] ){ &(driver_attr_faye_driver_attribute_group.attr), NULL },
};

struct device_driver faye_device_driver = {
	.name	    = "faye_device",
	.bus	    = &faye_bus,
	.groups     = ( const struct attribute_group*[] ){ &faye_driver_attrGroup, NULL },
	//.probe	    =  faye_driver_probe,
	//.remove	    =  faye_driver_remove,
	//.shutdown   	=  faye_driver_shutdown,
};

static ssize_t faye_driver_author_show( struct device_driver *driver, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", author );
}
static ssize_t faye_driver_author_store( struct device_driver *driver, const char *buf, size_t count ){
	return sprintf( author, "%s", buf );
}
static DRIVER_ATTR_RW( faye_driver_author );

static ssize_t faye_driver_attr1_show( struct device_driver *driver, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", driver_attr1_Buf );
}
static ssize_t faye_driver_attr1_store( struct device_driver *driver, const char *buf, size_t count ){
	return sprintf( driver_attr1_Buf, "%s", buf );
}
static DRIVER_ATTR_RW( faye_driver_attr1 );

static ssize_t faye_driver_attr2_show( struct device_driver *driver, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", driver_attr2_Buf );
}
static ssize_t faye_driver_attr2_store( struct device_driver *driver, const char *buf, size_t count ){
	return sprintf( driver_attr2_Buf, "%s", buf );
}
static DRIVER_ATTR_RW( faye_driver_attr2 );

static int __init faye_driver_init( void ){
	int ret;
	
	printk( "/**** faye_driver_init() start ***************************************/\n" );
	ret = driver_register( &faye_device_driver );
	if( ret ){
		printk( "faye device driver register failed!\n" );
		return ret;
	}
	
	if( driver_create_file( &faye_device_driver, &driver_attr_faye_driver_author ) ){
		printk( "unable create faye device driver author file!\n" );
		return ret;
	}
	
	if( driver_create_file( &faye_device_driver, &driver_attr_faye_driver_attr1 ) ){
		printk( "unable create faye device driver attribute1 file\n" );
	}
	
	if( driver_create_file( &faye_device_driver, &driver_attr_faye_driver_attr2 ) ){
		printk( "unable create faye device driver attribute2 file\n" );
	}

	printk( "faye device driver register success!\n" );
	printk( "/**** faye_driver_init() end ***************************************/\n" );
	return ret;
}

static void __exit faye_driver_exit( void ){
	printk( "/**** faye_driver_exit() start ***************************************/\n" );
	driver_unregister( &faye_device_driver );
	printk( "faye_device_driver exit success!\n" );
	printk( "/**** faye_driver_exit() end ***************************************/\n" );
}

module_init( faye_driver_init );
module_exit( faye_driver_exit );

