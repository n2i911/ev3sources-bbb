#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/hrtimer.h>
#include <linux/mm.h>

#include  "../../lms2012/source/lms2012.h"

#define MODULE_NAME "iic_module"
#define DEVICE1_NAME IIC_DEVICE

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Sean Chiang <n2i911@gmail.com>");
MODULE_DESCRIPTION(MODULE_NAME);
MODULE_SUPPORTED_DEVICE(DEVICE1_NAME);

#define     SHM_LENGTH    (sizeof(IicDefault))
#define     NPAGES        ((SHM_LENGTH + PAGE_SIZE - 1) / PAGE_SIZE)
static void *kmalloc_ptr;

static IIC IicDefault;
static IIC *pIic = &IicDefault;

#define   IIC_TIMER_RESOLUTION            20 // [100u]
static    struct hrtimer Device1Timer;
static    ktime_t        Device1Time;

static enum hrtimer_restart Device1TimerInterrupt1(struct hrtimer *pTimer)
{
    hrtimer_forward_now(pTimer, Device1Time);

    return HRTIMER_RESTART;
}

static int Device1Ioctl(struct file *File, unsigned int Request, unsigned long Pointer)
{
    switch (Request) {
    case IIC_SET_CONN:
        break;
    case IIC_SET:
        break;
    case IIC_SETUP:
        break;
    case IIC_READ_TYPE_INFO:
        break;
    default:
        break;
    }

    return 0;
}

static ssize_t Device1Write(struct file *File, const char *Buffer, size_t Count,loff_t *Data)
{
    /*
     * do nothing
     */
    int len = Count;

    return len;
}

static ssize_t Device1Read(struct file *File, char *Buffer, size_t Count, loff_t *Offset)
{
    int len = 0;

    return len;
}

static int Device1Mmap(struct file *filp, struct vm_area_struct *vma)
{
    int ret;

    ret = remap_pfn_range(vma, vma->vm_start, virt_to_phys((void*)((unsigned long)pIic)) >> PAGE_SHIFT, vma->vm_end-vma->vm_start, PAGE_SHARED);

    if (ret)
        ret = -EAGAIN;

    return ret;
}

static const struct file_operations Device1Entries =
{
    .owner = THIS_MODULE,
    .read = Device1Read,
    .write = Device1Write,
    .mmap = Device1Mmap,
    .unlocked_ioctl = Device1Ioctl,
};

static struct miscdevice Device1 =
{
    MISC_DYNAMIC_MINOR,
    DEVICE1_NAME,
    &Device1Entries
};

static int Device1Init(void)
{
    return 0;
}

static void Device1Exit(void)
{
    return;
}

static int ModuleInit(void)
{
    pr_info("%s, %s()\n", MODULE_NAME, __func__);
    return 0;
}

static void ModuleExit(void)
{
    pr_info("%s, %s()\n", MODULE_NAME, __func__);
}

module_init(ModuleInit);
module_exit(ModuleExit);
