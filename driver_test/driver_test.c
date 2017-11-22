#include  <linux/module.h>
#include  <linux/init.h>
#include  <linux/string.h>
#include  <linux/device.h>
#include  <linux/kernel.h>

MODULE_AUTHOR( "faye" );
MODULE_LICENSE( "GPL" );

extern struct bus_type faye_bus;

char *author = "faye";

int faye_driver_remove( struct device *dev ){
	printk( "faye device driver is removed!\n" );
	return 0;
}

int faye_driver_probe( struct device *dev ){
	printk( "faye device driver can handle the device!\n" );
	return 0;
}

void faye_driver_shutdown( struct device *dev ){
	printk( "in faye_driver shutdown\n" );
}

struct device_driver faye_device_driver = {
	.name	= "faye_device1",
	.bus	= &faye_bus,
	.probe	= faye_driver_probe,
	.remove	= faye_driver_remove,
	.shutdown = faye_driver_shutdown,
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
	return ret;
}

static void __exit faye_driver_exit( void ){
	driver_unregister( &faye_device_driver );
}

module_init( faye_driver_init );
module_exit( faye_driver_exit );

