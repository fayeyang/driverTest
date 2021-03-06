#include  <linux/kernel.h>
#include  <linux/module.h>
#include  <linux/init.h>
#include  <linux/string.h>
#include  <linux/device.h>

MODULE_AUTHOR( "faye" );
MODULE_LICENSE( "GPL" );  /* 注意,本行不可省略,否则即使能成功编译,但在加载本模块时,会提示"Unknown symbol in module",并会在dmesg命令中给出所缺少的符号 */

extern struct class faye_class;

struct device faye_busDevice;

char author[ PAGE_SIZE + 1 ]			= "FayeYang";
char bus_attr1_Buf[ PAGE_SIZE + 1 ]		= "bus_attribute1 data";
char busDev_attr_Buf[ PAGE_SIZE +1 ]	= "bus_device_attribute data";

/* bus_attribute对象缓冲区 */
char bus_attrGroup_Buf[ PAGE_SIZE + 1 ]       = "bus attribute_Group data";

/* struct bus_type->match()函数指针会指向本函数 */
static int faye_bus_match( struct device *dev, struct device_driver *drv ){
    printk( "=== in faye_bus_match() start ===\n" );
    printk( "device is: %s\n", dev_name(dev) );
    printk( "device driver is: %s\n", drv->name );
    printk( "=== in faye_bus_match() end ===\n" );
    return !strncmp( dev_name( dev ), drv->name, strlen(drv->name) );
}

static int faye_bus_uevent( struct device *dev, struct kobj_uevent_env *env ){
    printk( "=== in faye_bus_uevent() start ===\n" );
    printk( "device is: %s\n", dev_name(dev) );
    printk( "=== in faye_bus_uevent() end ===\n" );
    return 0;
}

static int faye_bus_probe( struct device *dev ){
    printk( "=== in faye_bus_probe() start ===\n" );
    printk(  "device is:%s\n", dev_name(dev) );
    printk( "=== in faye_bus_probe() end ===\n" );
    return 0;
}

/* struct bus_type->remove()函数指针会指向本函数 */
static int faye_bus_remove( struct device *dev ){
    printk( "=== in faye_bus_remove() start ===\n" );
    printk(  "device is:%s\n", dev_name(dev) );
    printk( "=== in faye_bus_remove() end ===\n" );
    return 0;
}

static void faye_bus_shutdown( struct device *dev ){
    printk( "=== in faye_bus_shutdown() start ===\n" );
    printk(  "device is:%s\n", dev_name(dev) );
    printk( "=== in faye_bus_shutdown() end ===\n" );
}

/* 定义1个总线属性对象 */
static ssize_t bus_author_show( struct bus_type *bus, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", author );
}
static ssize_t bus_author_store( struct bus_type *bus, const char *buf, size_t count ){
    return sprintf( author, "%s", buf );
}
static BUS_ATTR( faye_bus_author, (S_IRUGO|S_IWUSR|S_IWGRP), bus_author_show, bus_author_store );  /*
                                    * 关于BUS_ATTR()宏可参考上文中的“bus_attribute数据类型：”部分
                                    * 在本处会生成名为bus_attr_faye_bus_author的bus_attribute类型对象
                                    */

/* 定义1个总线属性对象 */
static ssize_t bus_attr1_show( struct bus_type *bus, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", bus_attr1_Buf);
}
static ssize_t bus_attr1_store( struct bus_type *bus, const char *buf, size_t count ){
    return sprintf( bus_attr1_Buf, "%s", buf );
}
static BUS_ATTR( faye_bus_attribute1, (S_IRUGO|S_IWUSR|S_IWGRP), bus_attr1_show, bus_attr1_store );

/* 定义一个bus_attribute对象,并将其封装到attribute_group对象中 */
static ssize_t bus_attribute_group_show( struct bus_type *bus, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", bus_attrGroup_Buf );
}
static ssize_t bus_attribute_group_store( struct bus_type *bus, const char *buf, size_t count ){
	return sprintf( bus_attrGroup_Buf, "%s", buf );
}
static BUS_ATTR( faye_bus_attribute_group, (S_IRUGO|S_IWUSR|S_IWGRP), bus_attribute_group_show, bus_attribute_group_store );

struct attribute_group faye_bus_attrGroup = {
	.name  = "faye_bus_attrGroup_name",
	.attrs = (struct attribute*[]){ &(bus_attr_faye_bus_attribute_group.attr), NULL },
};

/************************************************************************************************************/

/* 用户自定义总线相关bus_type对象 */
struct bus_type  faye_bus = {
    .name     = "faye_bus",   /* 总线名，注册总线后，会在/sys/bus/目录下建立该名称的目录 */
    //.dev_root = &faye_busDevice,
    .match    =  faye_bus_match,
    .uevent   =  faye_bus_uevent,
    .shutdown =  faye_bus_shutdown,
    .probe    =  faye_bus_probe,
    .remove   =  faye_bus_remove,
    .bus_groups = ( const struct attribute_group*[] ){ &faye_bus_attrGroup, NULL },
};
EXPORT_SYMBOL( faye_bus );

/***************************************************************************************************************/

/* struct device::void ( *release )( struct device *dev )函数指针会指向本函数 */
void faye_busDevice_release( struct device *dev ){
	printk( "=== in faye_busDevice_release() start ===\n" );
	printk(  "device is:%s\n", dev_name(dev) );
	printk( "=== in faye_busDevice_release() end ===\n" );
}

/* 用户自定义总线所对应的device对象,每个总线还需要对应1个device对象？？？？ */
struct device  faye_busDevice = {
    .init_name  = "faye_busDevice",  /* 设备名，注册总线设备后，会在/sys/devices/目录下建立该名称的目录 */
    .bus 		= &faye_bus,         /*
                        * 若指定所属总线为faye_bus，则会在/sys/bus/faye_bus/devices/目录
                        * 下建立到/sys/devices/faye_busDevice/设备目录的符号链接文件
                        */
    .release    =  faye_busDevice_release,
    //.class		= &faye_class,
};
EXPORT_SYMBOL( faye_busDevice );

static ssize_t busDevice_attr_show( struct device *dev, struct device_attribute *attr, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", busDev_attr_Buf );
}
static ssize_t busDevice_attr_store( struct device *dev, struct device_attribute *attr, const char *buf, size_t count ){
    return sprintf( busDev_attr_Buf, "%s", buf );    
}
static DEVICE_ATTR( faye_busDevice_attr, (S_IRUGO|S_IWUSR|S_IWGRP), busDevice_attr_show, busDevice_attr_store );

static int __init faye_bus_init( void ){
    int ret;

    printk( "/**** faye_bus_init() start ***************************************/\n" );

    /* 向内核注册用户自定义总线，注册成功后，会在/sys/bus/目录下建立以faye_bus.name命名的目录 */
    ret = bus_register( &faye_bus );
    if( ret )
        return ret;

	//bus_add_groups( &faye_bus, faye_attrGroup );

    /*
     * 向用户自定义总线faye_bus中添加用户自定义的总线属性对象文件，
     * 该bus_attribute结构体对象是上述代码中通过BUS_ATTR()宏生成的
     */
    if( bus_create_file(&faye_bus, &bus_attr_faye_bus_author) )
        printk( KERN_NOTICE "Unable to create author attribute\n" );
    if( bus_create_file(&faye_bus, &bus_attr_faye_bus_attribute1) )
        printk( KERN_NOTICE "Unable to create faye_bus_attribute_1\n" );

    printk( "faye_bus register success!\n" );

#if 1
    /*
     * 向内核注册用户自定义设备,faye_busDevice是1个device对象，每个总线还需要对应1个device对象？？？？
     * 注册成功后，会在/sys/devices/目录下建立以faye_busDevice.init_name命名的目录
     */
    if( device_register( &faye_busDevice ) )
        printk( KERN_NOTICE "Fail to register faye_busDevice\n" );

    if( device_create_file( &faye_busDevice, &dev_attr_faye_busDevice_attr ) )
        printk( KERN_NOTICE "Unable to create faye_device_attr" );
    printk( "faye_busDevice register success!\n" );
#endif

    faye_bus.dev_root = &faye_busDevice;

    printk( "/**** faye_bus_init() end ***************************************/\n" );
    return ret;
}

static void __exit faye_bus_exit( void ){

    printk( "/**** faye_bus_exit() start ***************************************/\n" );
 
    /* 注意卸载顺序,先卸载总线下设备,再卸载总线! */

#if 0
    device_unregister( &faye_busDevice );       /*
            * 卸载用户自定义设备，若卸载成功,则会删除/sys/devices/目录下以faye_busDevice.initname命名的目录
            */
#endif

    bus_unregister( &faye_bus );  /* 卸载用户自定义总线，若卸载成功，则会删除/sys/bus/目录下以faye_bus.name命名的目录 */
	
    printk( "/**** faye_bus_exit() end ***************************************/\n" );
  
}

module_init( faye_bus_init );
module_exit( faye_bus_exit );


