#include  <linux/kernel.h>
#include  <linux/module.h>
#include  <linux/init.h>
#include  <linux/string.h>
#include  <linux/device.h>

MODULE_AUTHOR( "faye" );
MODULE_LICENSE( "GPL" );

extern struct bus_type faye_bus;

/* driver_attribute对象缓冲区 */
char attrBuf[ PAGE_SIZE+1 ] = "driver attribute of attribute_Group";

char *author = "faye";

int faye_driver_remove( struct device *dev ){
	printk( "*** in faye_driver_remove start ***\n" );
	printk(  "device is:%s\n", dev_name(dev) );
	printk( "*** in faye_driver_remove end ***\n" );
	return 0;
}

int faye_driver_probe( struct device *dev ){
	printk( "*** in faye_driver_probe start ***\n" );
	printk(  "device is:%s\n", dev_name(dev) );
	printk( "*** faye device driver can handle the device ***\n" );
	printk( "*** in faye_driver_probe end ***\n" );
	return 0;
}

void faye_driver_shutdown( struct device *dev ){
	printk( "*** in faye_driver_shutdown start ***\n" );
	printk(  "device is:%s\n", dev_name(dev) );
	printk( "*** in faye_driver_shutdown end ***\n" );
}

/* 定义一个device_attribute对象,将其封装到attribute_group对象中 */
static ssize_t driver_attribute_group_show( struct device_driver *drv, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", attrBuf );
}
static ssize_t driver_attribute_group_store( struct device_driver *drv, const char *buf, size_t count ){
	return sprintf( attrBuf, "%s", buf );
}
static DRIVER_ATTR( faye_driver_attribute_group, ( S_IRUSR | S_IWUSR ), driver_attribute_group_show, driver_attribute_group_store );

struct attribute_group faye_driver_attrGroup = {
	.name  = "faye_driver_attrGroup_name",
	.attrs = (struct attribute*[]){ &(driver_attr_faye_driver_attribute_group.attr), NULL },
};

struct device_driver faye_device_driver = {
	.name	    = "faye_device",
	.bus	    = &faye_bus,
	.groups     = ( const struct attribute_group*[] ){ &faye_driver_attrGroup, NULL },
	//.probe	    =  faye_driver_probe,
	//.remove	    =  faye_driver_remove,
	//.shutdown   =  faye_driver_shutdown,
};

static ssize_t show_faye_driver_author( struct device_driver *driver, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", author );
}
static DRIVER_ATTR( faye_driver_author, S_IRUGO, show_faye_driver_author, NULL );

static ssize_t faye_device_driver_attr1( struct device_driver *driver, char *buf ){
	return snprintf( buf, PAGE_SIZE, "faye device driver attr1\n" );
}
static DRIVER_ATTR( faye_device_driver_attr1, S_IRUGO, faye_device_driver_attr1, NULL );

static ssize_t faye_device_driver_attr2( struct device_driver *driver, char *buf ){
	return snprintf( buf, PAGE_SIZE, "faye device driver attr2\n" );
}
static DRIVER_ATTR( faye_device_driver_attr2, S_IRUGO, faye_device_driver_attr2, NULL );

static int __init faye_driver_init( void ){
	int ret;
	
	printk( "/**** faye_device_driver_init ***************************************/\n" );
	ret = driver_register( &faye_device_driver );
	if( ret ){
		printk( "faye device driver register failed!\n" );
		return ret;
	}
	
	if( driver_create_file( &faye_device_driver, &driver_attr_faye_driver_author ) ){
		printk( "unable create faye device driver author file!\n" );
		return ret;
	}
	
	if( driver_create_file( &faye_device_driver, &driver_attr_faye_device_driver_attr1 ) ){
		printk( "unable create faye device driver attribute1 file\n" );
	}
	
	if( driver_create_file( &faye_device_driver, &driver_attr_faye_device_driver_attr2 ) ){
		printk( "unable create faye device driver attribute2 file\n" );
	}

	printk( "faye device driver register success!\n" );
	printk( "/**** faye_device_driver_init ***************************************/\n" );
	return ret;
}

static void __exit faye_driver_exit( void ){
	driver_unregister( &faye_device_driver );
	printk( "faye_device_driver exit success!\n" );
}

module_init( faye_driver_init );
module_exit( faye_driver_exit );

