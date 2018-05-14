#include  <linux/kernel.h>
#include  <linux/module.h>
#include  <linux/init.h>
#include  <linux/string.h>
#include  <linux/device.h>

MODULE_AUTHOR( "faye" );
MODULE_LICENSE( "GPL" );

extern struct bus_type    faye_bus;
extern struct device      faye_busDevice;

void  faye_device_release( struct device *dev ){
	printk( "### in faye_device_release start ###\n" );
	printk(  "device is:%s\n", dev_name(dev) );
	printk( "### in faye_device_release end ###\n" );
}

struct device faye_device = {
	.init_name	= "faye_device",
	.bus 		= &faye_bus,
	.parent		= &faye_busDevice,
	.release	=  faye_device_release,
};

static ssize_t show_faye_device_attr_1( struct device *dev, struct device_attribute *attr, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", "faye_device attribute 1" );
}
static DEVICE_ATTR( faye_device_attr_1, S_IRUGO, show_faye_device_attr_1, NULL );

static ssize_t show_faye_device_attr_2( struct device *dev, struct device_attribute *attr, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", "faye_device attribute 2" );
}
static DEVICE_ATTR( faye_device_attr_2, S_IRUGO, show_faye_device_attr_2, NULL );

static int __init faye_device_init( void ){
	int ret;

    printk( "/**** faye_device_init ***************************************/\n" );

	ret = device_register( &faye_device );
	if( ret ){
		printk( KERN_DEBUG "Unable to register device\n" );
		return ret;
	}
	
	if( device_create_file( &faye_device, &dev_attr_faye_device_attr_1 ) ){
		printk( KERN_DEBUG "Unable to create device attribute file\n" );
		return ret;
	}
	
	if( device_create_file( &faye_device, &dev_attr_faye_device_attr_2 ) ){
		printk( KERN_DEBUG "Unable to create device attribute file\n" );
		return ret;
	}
	
	printk( KERN_DEBUG "faye_device register success\n" );
	printk( "/**** faye_device_init ***************************************/\n" );
	return 0;
}

static void __exit faye_device_exit( void ){
	device_unregister( &faye_device );
	printk( "faye_device exit success!\n" );
}

module_init( faye_device_init );
module_exit( faye_device_exit );


