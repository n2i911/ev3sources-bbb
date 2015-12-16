#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/hrtimer.h>

#include  "../../lms2012/source/lms2012.h"

#define MODULE_NAME "usbhost_module"
#define DEVICE1_NAME USBHOST_DEVICE

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Sean Chiang <n2i911@gmail.com>");
MODULE_DESCRIPTION(MODULE_NAME);
MODULE_SUPPORTED_DEVICE(DEVICE1_NAME);

static ssize_t Device1Write(struct file *File, const char *Buffer, size_t Count, loff_t *Data)
{
    pr_info("%s, %s\n", MODULE_NAME, __func__);

    return Count;
}

static ssize_t Device1Read(struct file *File, char *Buffer, size_t Count, loff_t *Offset)
{
    int len;

    pr_info("%s, %s\n", MODULE_NAME, __func__);

    len = snprintf(Buffer, Count, "%s\n", DEVICE1_NAME);

    return len;
}

static const struct file_operations Device1Entries =
{
    .owner = THIS_MODULE,
    .read = Device1Read,
    .write = Device1Write,
};

static struct miscdevice Device1 =
{
    MISC_DYNAMIC_MINOR,
    DEVICE1_NAME,
    &Device1Entries
};

static int Device1Init(void)
{
    int ret = 0;

    pr_info("%s, %s\n", MODULE_NAME, __func__);

    ret = misc_register(&Device1);

    if (ret) {
        pr_info("%s device register failed\n", DEVICE1_NAME);
    }
    else {
        pr_info("%s device register succes\n", DEVICE1_NAME);
    }

    return ret;
}

static void Device1Exit(void)
{
    pr_info("%s, %s\n", MODULE_NAME, __func__);

    misc_deregister(&Device1);

    pr_info("%s device unregistered\n", DEVICE1_NAME);
}

static int ModuleInit(void)
{
    pr_info("%s, %s\n", MODULE_NAME, __func__);

    Device1Init();

    return 0;
}

static void ModuleExit(void)
{
    pr_info("%s, %s\n", MODULE_NAME, __func__);

    Device1Exit();
}

module_init(ModuleInit);
module_exit(ModuleExit);
