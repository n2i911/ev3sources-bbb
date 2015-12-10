#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/hrtimer.h>

#include  "../../lms2012/source/lms2012.h"

#define MODULE_NAME "ui_module"
#define DEVICE1_NAME UI_DEVICE

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Sean Chiang <n2i911@gmail.com>");
MODULE_DESCRIPTION(MODULE_NAME);
MODULE_SUPPORTED_DEVICE(DEVICE1_NAME);

static struct hrtimer Device1Timer;
static ktime_t        Device1Time;
static struct hrtimer Device2Timer;
static ktime_t        Device2Time;

static enum hrtimer_restart Device1TimerInterrupt1(struct hrtimer *pTimer)
{
    // restart timer
    hrtimer_forward_now(pTimer, ktime_set(0, 50000000));

    return HRTIMER_RESTART;
}

static enum hrtimer_restart Device2TimerInterrupt1(struct hrtimer *pTimer)
{
    // restart timer
    hrtimer_forward_now(pTimer, ktime_set(0, 10000000));

    return HRTIMER_RESTART;
}

static ssize_t Device1Write(struct file *File, const char *Buffer, size_t Count, loff_t *Data)
{
    pr_info("%s, %s\n", MODULE_NAME, __func__);
    return 0;
}

static ssize_t Device1Read(struct file *File, char *Buffer, size_t Count, loff_t *Offset)
{
    int len;

    pr_info("%s, %s\n", MODULE_NAME, __func__);

    len = snprintf(Buffer, Count, "BeagleBone Black Rev C\n");

    return len;
}

static int Device1Mmap(struct file *filp, struct vm_area_struct *vma)
{
    pr_info("%s, %s\n", MODULE_NAME, __func__);
    return 0;
}

static const struct file_operations Device1Entries =
{
    .owner = THIS_MODULE,
    .read = Device1Read,
    .write = Device1Write,
    .mmap = Device1Mmap
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

        Device1Time = ktime_set(0, 50000000);
        hrtimer_init(&Device1Timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        Device1Timer.function = Device1TimerInterrupt1;
        hrtimer_start(&Device1Timer, Device1Time, HRTIMER_MODE_REL);

        Device2Time = ktime_set(0,10000000);
        hrtimer_init(&Device2Timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        Device2Timer.function = Device2TimerInterrupt1;
        hrtimer_start(&Device2Timer, Device2Time, HRTIMER_MODE_REL);

        pr_info("%s device register succes\n", DEVICE1_NAME);
    }

    return ret;
}

static void Device1Exit(void)
{
    pr_info("%s, %s\n", MODULE_NAME, __func__);

    hrtimer_cancel(&Device1Timer);
    hrtimer_cancel(&Device2Timer);

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
