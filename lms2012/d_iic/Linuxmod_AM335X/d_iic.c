#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/hrtimer.h>

#include  "../../lms2012/source/lms2012.h"

#define MODULE_NAME "iic_module"
#define DEVICE1_NAME IIC_DEVICE

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Sean Chiang <n2i911@gmail.com>");
MODULE_DESCRIPTION(MODULE_NAME);
MODULE_SUPPORTED_DEVICE(DEVICE1_NAME);

static int ModuleInit(void)
{
    pr_info("%s, %s\n", MODULE_NAME, __func__);
    return 0;
}

static void ModuleExit(void)
{
    pr_info("%s, %s\n", MODULE_NAME, __func__);
}

module_init(ModuleInit);
module_exit(ModuleExit);
