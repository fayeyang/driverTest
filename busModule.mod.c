#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xb6fd8534, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x4eaf6cfd, __VMLINUX_SYMBOL_STR(bus_register) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x5a83da65, __VMLINUX_SYMBOL_STR(device_register) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x5a921311, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0x4607da2, __VMLINUX_SYMBOL_STR(bus_unregister) },
	{ 0x4a022a43, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0xf9e97f92, __VMLINUX_SYMBOL_STR(bus_create_file) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xd34f0915, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "29C1F6D1F0D6CD1FEF283FF");
