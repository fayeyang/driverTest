#include  <linux/kernel.h>
#include  <linux/module.h>
#include  <linux/init.h>
#include  <linux/string.h>
#include  <linux/device.h>

MODULE_AUTHOR( "faye" );
MODULE_LICENSE( "GPL" );

extern int subDevice_init( void );
extern void subDevice_exit( void );

char *author = "FayeYang";

/* bus_attribute对象缓冲区 */
char attrBuf[ PAGE_SIZE+1 ] = "bus attribute of attribute_Group";

/* struct bus_type->match()函数指针会指向本函数 */
static int faye_bus_match( struct device *dev, struct device_driver *drv ){
    printk( "=== in faye_bus_match start ===\n" );
    printk( "device is: %s\n", dev_name(dev) );
    printk( "device driver is: %s\n", drv->name );
    printk( "=== in faye_bus_match end ===\n" );
    return !strncmp( dev_name( dev ), drv->name, strlen(drv->name) );
}

static int faye_bus_probe( struct device *dev ){
    printk( "=== in faye_bus_probe start ===\n" );
    printk(  "device is:%s\n", dev_name(dev) );
    printk( "=== in faye_bus_probe end ===\n" );
    return 0;
}

/* struct bus_type->remove()函数指针会指向本函数 */
static int faye_bus_remove( struct device *dev ){
    printk( "=== in faye_bus_remove start ===\n" );
    printk(  "device is:%s\n", dev_name(dev) );
    printk( "=== in faye_bus_remove end ===\n" );
    return 0;
}

static void faye_bus_shutdown( struct device *dev ){
    printk( "=== in faye_bus_shutdown start ===\n" );
    printk(  "device is:%s\n", dev_name(dev) );
    printk( "=== in faye_bus_shutdown end ===\n" );
}


/* 定义1个总线属性对象 */
static ssize_t show_bus_author( struct bus_type *bus, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", author );
}
static BUS_ATTR( faye_bus_author, S_IRUGO, show_bus_author, NULL );  /*
                                    * 关于BUS_ATTR()宏可参考上文中的“bus_attribute数据类型：”部分
                                    * 在本处会生成名为bus_attr_faye_bus_author的bus_attribute类型对象
                                    */

/* 定义1个总线属性对象 */
static ssize_t show_bus_attribute_1( struct bus_type *bus, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", "bus_attribute_1" );
}
static BUS_ATTR( faye_bus_attribute_1, S_IRUGO, show_bus_attribute_1, NULL );

/* 定义一个bus_attribute对象,将其封装到attribute_group对象中 */
static ssize_t bus_attribute_group_show( struct bus_type *bus, char *buf ){
	return snprintf( buf, PAGE_SIZE, "%s\n", attrBuf );
}
static ssize_t bus_attribute_group_store( struct bus_type *bus, const char *buf, size_t count ){
	return sprintf( attrBuf, "%s", buf );
}
static BUS_ATTR( faye_bus_attribute_group, ( S_IRUSR | S_IWUSR ), bus_attribute_group_show, bus_attribute_group_store );

struct attribute_group faye_bus_attrGroup = {
	.name  = "faye_bus_attrGroup_name",
	.attrs = (struct attribute*[]){ &(bus_attr_faye_bus_attribute_group.attr), NULL },
};

/************************************************************************************************************/

/* 用户自定义总线相关bus_type对象 */
struct bus_type  faye_bus = {
    .name     = "faye_bus",   /* 总线名，注册总线后，会在/sys/bus/目录下建立该名称的目录 */
    .match    =  faye_bus_match,
    .shutdown =  faye_bus_shutdown,
    .probe    =  faye_bus_probe,
    .remove   =  faye_bus_remove,
    .bus_groups = ( const struct attribute_group*[] ){ &faye_bus_attrGroup, NULL },
};
EXPORT_SYMBOL( faye_bus );

/***************************************************************************************************************/

/* struct device::void ( *release )( struct device *dev )函数指针会指向本函数 */
void faye_busDevice_release( struct device *dev ){
	printk( "=== in faye_busDevice_release start ===\n" );
	printk(  "device is:%s\n", dev_name(dev) );
	printk( "=== in faye_busDevice_release end ===\n" );
}

/* 用户自定义总线所对应的device对象,每个总线还需要对应1个device对象？？？？ */
struct device  faye_busDevice = {
    .init_name  = "faye_busDevice",  /* 设备名，注册总线设备后，会在/sys/devices/目录下建立该名称的目录 */
    .bus 		= &faye_bus,         /*
                        * 若指定所属总线为faye_bus，则会在/sys/bus/faye_bus/devices/目录
                        * 下建立到/sys/devices/faye_busDevice/设备目录的符号链接文件
                        */
    .release    =  faye_busDevice_release,
};
EXPORT_SYMBOL( faye_busDevice );

static ssize_t show_busDevice_attr( struct device *dev, struct device_attribute *attr, char *buf ){
    return snprintf( buf, PAGE_SIZE, "%s\n", "faye_busDevice_attribute" );
}
static DEVICE_ATTR( faye_busDevice_attr, S_IRUGO, show_busDevice_attr, NULL );

static int __init faye_bus_init( void ){
    int ret;

    printk( "/**** faye_bus_init ***************************************/\n" );

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
    if( bus_create_file(&faye_bus, &bus_attr_faye_bus_attribute_1) )
        printk( KERN_NOTICE "Unable to create faye_bus_attribute_1" );

    printk( "faye_bus register success!\n" );

    /*
     * 向内核注册用户自定义设备,faye_busDevice是1个device对象，每个总线还需要对应1个device对象？？？？
     * 注册成功后，会在/sys/devices/目录下建立以faye_busDevice.init_name命名的目录
     */
    if( device_register( &faye_busDevice ) )
        printk( KERN_NOTICE "Fail to register device\n" );

    if( device_create_file( &faye_busDevice, &dev_attr_faye_busDevice_attr ) )
        printk( KERN_NOTICE "Unable to create faye_device_attr" );
    printk( "faye_busDevice register success!\n" );

	//subDevice_init();

    printk( "/**** faye_bus_init ***************************************/\n" );
    return ret;
}

static void __exit faye_bus_exit( void ){

    //subDevice_exit();
 
    /* 注意卸载顺序,先卸载总线下设备,再卸载总线! */

    device_unregister( &faye_busDevice );       /*
            * 卸载用户自定义设备，若卸载成功,则会删除/sys/devices/目录下以faye_busDevice.initname命名的目录
            */
    bus_unregister( &faye_bus );  /* 卸载用户自定义总线，若卸载成功，则会删除/sys/bus/目录下以faye_bus.name命名的目录 */
    printk( "faye_bus exit success\n" );    
}

module_init( faye_bus_init );
module_exit( faye_bus_exit );


