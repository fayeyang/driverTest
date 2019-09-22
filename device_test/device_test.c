#include  <linux/kernel.h>
#include  <linux/module.h>
#include  <linux/init.h>
#include  <linux/string.h>
#include  <linux/device.h>

MODULE_AUTHOR( "faye" );
MODULE_LICENSE( "GPL" );    /* 注意,本行不可省略,否则即使能成功编译,但在加载本模块时,会提示"Unknown symbol in module",并会在dmesg命令中给出所缺少的符号 */

extern struct bus_type   faye_bus;
extern struct device     faye_busDevice;

extern struct class      faye_class;
extern void faye_class_dev_release( struct device *dev );

/* device_attribute对象缓冲区 */
char dev_attr1_Buf[ PAGE_SIZE+1 ] = "device attribute 1 data";
char dev_attr2_Buf[ PAGE_SIZE+1 ] = "device attribute 2 data";

char dev_attrGroup_Buf[ PAGE_SIZE+1 ] = "device attribute group data";

char subDev_attr_Buf[ PAGE_SIZE+1 ]   = "sudDevice attribute data";

struct device faye_device_class = {
    .init_name = "faye_device_class",
    .class     = &faye_class,
    .parent    = &faye_busDevice,
    .release   = &faye_class_dev_release, 
};

void  faye_device_release( struct device *dev ){
    printk( "### in faye_device_release() start ###\n" );
    printk(  "device is:%s\n", dev_name(dev) );
    printk( "### in faye_device_release() end ###\n" );
}

void faye_subDevice_release( struct device *dev ){
    printk( "### in faye_subDevice_release() start ###\n" );
    printk( "device is:%s\n", dev_name(dev) );
    printk( "### in faye_subDevice_release() end ###\n" );
}

/* 定义一个device_attribute对象,将其封装到attribute_group对象中 */
static ssize_t device_attribute_group_show( struct device *dev, struct device_attribute *attr, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", dev_attrGroup_Buf );
}
static ssize_t device_attribute_group_store( struct device *dev, struct device_attribute *attr, const char *buf, size_t count ){
    return sprintf( dev_attrGroup_Buf, "%s", buf );
}
static DEVICE_ATTR( faye_device_attribute_group, (S_IRUGO|S_IWUSR|S_IWGRP), device_attribute_group_show, device_attribute_group_store );

struct attribute_group faye_device_attrGroup = {
    .name  = "faye_device_attrGroup_name",
    .attrs = (struct attribute*[]){ &(dev_attr_faye_device_attribute_group.attr), NULL },
};

struct device faye_device = {
    .init_name  = "faye_device",
    .bus        = &faye_bus,
    .parent     = &faye_busDevice,
            /* 若未设置.parent字段，但其所属总线的bus.dev_root字段被赋值，
             * 则本device对象会成为dev_root的子设备
             * 可参考本文中的“bus_type数据结构：”部分 */
    .release    =  faye_device_release,
    .groups     = ( const struct attribute_group*[] ){ &faye_device_attrGroup, NULL },
    //.class        = &faye_class,
};

static ssize_t faye_device_attr1_show( struct device *dev, struct device_attribute *attr, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", dev_attr1_Buf );
}
static ssize_t faye_device_attr1_store( struct device *dev, struct device_attribute *attr, const char *buf, size_t count ){
    return sprintf( dev_attr1_Buf, "%s", buf ); 
}
static DEVICE_ATTR( faye_device_attr1, (S_IRUGO|S_IWUSR|S_IWGRP), faye_device_attr1_show, faye_device_attr1_store );

static ssize_t faye_device_attr2_show( struct device *dev, struct device_attribute *attr, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", dev_attr2_Buf );
}
static ssize_t faye_device_attr2_store( struct device *dev, struct device_attribute *attr, const char *buf, size_t count ){
    return sprintf( dev_attr2_Buf, "%s", buf ); 
}
static DEVICE_ATTR( faye_device_attr2, (S_IRUGO|S_IWUSR|S_IWGRP), faye_device_attr2_show, faye_device_attr2_store );

struct device faye_subDevice = {
    .init_name  = "faye_subDevice",
    //.bus        = &faye_bus,
    .parent     = &faye_device,
    .release    =  faye_subDevice_release,
};

static ssize_t faye_subDevice_attr_show( struct device *dev, struct device_attribute *attr, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", subDev_attr_Buf );   
}
static ssize_t faye_subDevice_attr_store( struct device *dev, struct device_attribute *attr, const char *buf, size_t count ){
    return sprintf( subDev_attr_Buf, "%s", buf );    
}
static DEVICE_ATTR( faye_subDevice_attr, (S_IRUGO|S_IWUSR|S_IWGRP), faye_subDevice_attr_show, faye_subDevice_attr_store );

static int __init faye_device_init( void ){
    int ret;

    printk( "/**** faye_device_init() start ***************************************/\n" );

	ret = device_register( &faye_device_class );
    if( ret ){
        printk( KERN_DEBUG "Unable to register faye_device_class\n" );
        return ret;
    }
    printk( KERN_DEBUG "faye_device_class register success\n" );

    ret = device_register( &faye_device );
    if( ret ){
        printk( KERN_DEBUG "Unable to register faye_device\n" );
        return ret;
    }
    ret = device_create_file( &faye_device, &dev_attr_faye_device_attr1 );
    if( ret ){
        printk( KERN_DEBUG "Unable to create device attribute file\n" );
        return ret;
    }    
    ret = device_create_file( &faye_device, &dev_attr_faye_device_attr2 );
    if( ret ){
        printk( KERN_DEBUG "Unable to create device attribute file\n" );
        return ret;
    }
    printk( KERN_DEBUG "faye_device register success\n" );
    
    ret = device_register( &faye_subDevice );
    if( ret ){
        printk( KERN_DEBUG "unable to register faye_subDevice\n" );
        return ret;
    }
    ret = device_create_file( &faye_subDevice, &dev_attr_faye_subDevice_attr );
    if( ret ){
        printk( KERN_DEBUG "unable to create device attribute file\n" );
        return ret;
    }
    printk( KERN_DEBUG "faye_subDevice register success\n" );

    printk( "/**** faye_device_init end ***************************************/\n" );
    return 0;
}

static void __exit faye_device_exit( void ){
    printk( "/**** faye_device_exit() start ***************************************/\n" );

    device_unregister( &faye_subDevice );
    printk( "faye_subDevice exit success!\n" );

	device_unregister( &faye_device_class );
    printk( "faye_device_class exit success!\n" );

    device_unregister( &faye_device );
    printk( "faye_device exit success!\n" );
        
    printk( "/**** faye_device_exit() end ***************************************/\n" );
}

module_init( faye_device_init );
module_exit( faye_device_exit );


