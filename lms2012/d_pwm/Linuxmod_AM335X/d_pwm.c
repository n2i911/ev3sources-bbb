#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/hrtimer.h>
#include <linux/mm.h>
#include <linux/uaccess.h>

#include  "../../lms2012/source/lms2012.h"

#define MODULE_NAME "pwm_module"
#define DEVICE1_NAME PWM_DEVICE
#define DEVICE2_NAME MOTOR_DEVICE

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Sean Chiang <n2i911@gmail.com>");
MODULE_DESCRIPTION(MODULE_NAME);
MODULE_SUPPORTED_DEVICE(DEVICE1_NAME);

#define BUFSIZE 32

/*
 *  NO_OF_TACHO_SAMPLES holds the number of recent tacho samples
 */
#define   NO_OF_TACHO_SAMPLES           128
#define   NO_OF_OUTPUT_PORTS            4

static UBYTE ReadyStatus = 0;
static UBYTE TestStatus = 0;

static MOTORDATA MotorData[NO_OF_OUTPUT_PORTS];
static MOTORDATA *pMotor = MotorData;

static ssize_t Device1Write(struct file *File, const char *Buffer, size_t Count, loff_t *Data)
{
    int len = 0;
    char buf[BUFSIZE];

    len = Count;

    copy_from_user(buf, Buffer, Count);

    switch (buf[0]) {
    case opPROGRAM_STOP:
        break;
    case opPROGRAM_START:
        break;
    case opOUTPUT_SET_TYPE:
        break;
    case opOUTPUT_RESET:
        break;
    case opOUTPUT_CLR_COUNT:
        break;
    case opOUTPUT_STOP:
        break;
    case opOUTPUT_POWER:
        break;
    case opOUTPUT_SPEED:
        break;
    case opOUTPUT_START:
        break;
    case opOUTPUT_POLARITY:
        break;
    case opOUTPUT_POSITION:
        break;
    case opOUTPUT_STEP_POWER:
        break;
    case opOUTPUT_TIME_POWER:
        break;
    case opOUTPUT_STEP_SPEED:
        break;
    case opOUTPUT_TIME_SPEED:
        break;
    case opOUTPUT_STEP_SYNC:
        break;
    case opOUTPUT_TIME_SYNC:
        break;
    default:
        break;
    }

    return len;
}

static ssize_t Device1Read(struct file *File, char *Buffer, size_t Count, loff_t *Offset)
{
    int len = 0;

    len += snprintf(&Buffer[0], Count, "%01u ", ReadyStatus);
    len += snprintf(&Buffer[len], Count - len, "%01u ", TestStatus);

    return len;
}

static const struct file_operations Device1Entries =
{
    .owner = THIS_MODULE,
    .read = Device1Read,
    .write = Device1Write
};

static struct miscdevice Device1 =
{
    MISC_DYNAMIC_MINOR,
    DEVICE1_NAME,
    &Device1Entries
};

static ssize_t Device2Write(struct file *File, const char *Buffer, size_t Count, loff_t *Data)
{
    int len = 0;
    char buf[BUFSIZE];

    copy_from_user(buf, Buffer, Count);

    len = Count;

    /*
     * Set Ready flag and Test flag
     */
    ReadyStatus |= buf[0];
    TestStatus |= buf[0];

    return len;
}

static ssize_t Device2Read(struct file *File, char *Buffer, size_t Count, loff_t *Offset)
{
    int len = 0;

    return len;
}

#define     SHM_LENGTH    (sizeof(MotorData))
#define     NPAGES        ((SHM_LENGTH + PAGE_SIZE - 1) / PAGE_SIZE)
static void *kmalloc_ptr;

static int Device2Mmap(struct file *filp, struct vm_area_struct *vma)
{
    int ret;

    ret = remap_pfn_range(vma, vma->vm_start, virt_to_phys((void*)((unsigned long)pMotor)) >> PAGE_SHIFT, vma->vm_end-vma->vm_start, PAGE_SHARED);
    if (ret)
        ret = -EAGAIN;

    return ret;
}

static const struct file_operations Device2Entries =
{
    .owner = THIS_MODULE,
    .read = Device2Read,
    .write = Device2Write,
    .mmap = Device2Mmap,
};

static struct miscdevice Device2 =
{
    MISC_DYNAMIC_MINOR,
    DEVICE2_NAME,
    &Device2Entries
};

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
