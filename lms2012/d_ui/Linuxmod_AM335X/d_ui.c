#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/hrtimer.h>
#include <linux/platform_device.h>
#include <linux/pinctrl/consumer.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/property.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/bitops.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mm.h>

#include  "../../lms2012/source/lms2012.h"
#include "d_ui.h"

#define MODULE_NAME "ui_module"
#define DEVICE1_NAME UI_DEVICE

#define DEBUG 0

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Sean Chiang <n2i911@gmail.com>");
MODULE_DESCRIPTION(MODULE_NAME);
MODULE_SUPPORTED_DEVICE(DEVICE1_NAME);

struct ev3_ui_led_data {
    const char *label;
    struct gpio_desc *gpiod;
    int index;
};

struct ev3_ui_button_data {
    const char *label;
    struct gpio_desc *gpiod;
    int index;
};

struct ev3_ui_priv {
    int num_leds;
    int num_buttons;
    struct ev3_ui_led_data *leds;
    struct ev3_ui_button_data *buttons;
};

/*
 * Button Status
 */
static UI UiDefault;
static UI *pUi = &UiDefault;

static void *ptr;

#define NPAGES (((sizeof(UiDefault)) + PAGE_SIZE - 1) / PAGE_SIZE)

/*
 * Led status
 */
int PatternBlock = 0;     // Block pattern update
int PatternBits = 20;     // Pattern bits
int PatternBit = 0;       // Pattern bit pointer

ULONG ActPattern[LEDS];
ULONG TmpPattern[LEDS];

struct ev3_ui_led_data *leds;

/*
 * hrtimer
 */
static struct hrtimer Device1Timer;
static ktime_t Device1Time;
static struct hrtimer Device2Timer;
static ktime_t Device2Time;

static enum hrtimer_restart Device1TimerInterrupt1(struct hrtimer *pTimer)
{
    int i;

    if (PatternBlock) {

        PatternBlock = 0;
    } else {

        for (i = 0; i < LEDS; i++) {

            if (PatternBit == 0) {
                TmpPattern[i] = ActPattern[i];
            }

            if ((TmpPattern[i] & 0x00000001)) {

                gpiod_set_value(leds[i].gpiod, 1);
            } else {

                gpiod_set_value(leds[i].gpiod, 0);
            }

            TmpPattern[i] >>= 1;
        }

        if (++PatternBit >= PatternBits) {
            PatternBit = 0;
        }
    }

    // restart timer
    hrtimer_forward_now(pTimer, ktime_set(0, 50000000));

    return HRTIMER_RESTART;
}

static enum hrtimer_restart Device2TimerInterrupt1(struct hrtimer *pTimer)
{
    /*
     * Do nothing here
     */

    // restart timer
    hrtimer_forward_now(pTimer, ktime_set(0, 10000000));

    return HRTIMER_RESTART;
}

ULONG LEDPATTERNDATA[LEDS + 1][LEDPATTERNS] = {

    /*
     * LED_BLACK,
     * LED_GREEN,
     * LED_RED,
     * LED_ORANGE,
     * LED_GREEN_FLASH,
     * LED_RED_FLASH,
     * LED_ORANGE_FLASH,
     * LED_GREEN_PULSE,
     * LED_RED_PULSE,
     * LED_ORANGE_PULSE
     */

    {
        // RR
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0b00000000000000000000000000000000,
        0b00000000000000000111110000011111,
        0b00000000000000000111110000011111,
        0b00000000000000000000000000000000,
        0b00000000000000000000000001110111,
        0b00000000000000000000000001110111
    },

    {
        // RG
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0xFFFFFFFF,
        0b00000000000000000111110000011111,
        0b00000000000000000000000000000000,
        0b00000000000000000111110000011111,
        0b00000000000000000000000001110111,
        0b00000000000000000000000000000000,
        0b00000000000000000000000001110111
    },

    {
        // LR
        0x00000000,
        0x00000000,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0b00000000000000000000000000000000,
        0b00000000000000000111110000011111,
        0b00000000000000000111110000011111,
        0b00000000000000000000000000000000,
        0b00000000000000000000000001110111,
        0b00000000000000000000000001110111
    },

    {
        // LG
        0x00000000,
        0xFFFFFFFF,
        0x00000000,
        0xFFFFFFFF,
        0b00000000000000000111110000011111,
        0b00000000000000000000000000000000,
        0b00000000000000000111110000011111,
        0b00000000000000000000000001110111,
        0b00000000000000000000000000000000,
        0b00000000000000000000000001110111
    },

    { 0 }
};

static ssize_t Device1Write(struct file *File, const char *Buffer, size_t Count, loff_t *Data)
{
    char buf[2];
    int i, index, len = 0;

    pr_info("%s, %s()\n", MODULE_NAME, __func__);

    if (Count >= 2) {

        len = Count;

        if (copy_from_user(buf, Buffer, 2)) {
            return -EFAULT;
        }

        pr_info("%s, %s(): %s\n", MODULE_NAME, __func__, buf);

        index = buf[0] - '0';
        if ((index >= 0) && (index < LEDPATTERNS)) {

            PatternBlock = 1;

            PatternBits = 20;
            PatternBit = 0;

            for (i = 0; i < LEDS; i++) {
                ActPattern[i] = LEDPATTERNDATA[i][index];
            }

            PatternBlock  =  0;
        }
    }

    return len;
}

static ssize_t Device1Read(struct file *File, char *Buffer, size_t Count, loff_t *Offset)
{
    int len;

    pr_info("%s, %s()\n", MODULE_NAME, __func__);

    len = snprintf(Buffer, Count, "BeagleBone Black Rev C\n");

    return len;
}

static int Device1Mmap(struct file *filp, struct vm_area_struct *vma)
{
    int ret;

    pr_info("%s, %s()\n", MODULE_NAME, __func__);

    ret = remap_pfn_range(vma, vma->vm_start, virt_to_phys((void*)((unsigned long)pUi)) >> PAGE_SHIFT,(vma->vm_end - vma->vm_start), PAGE_SHARED);

    if (ret)
        return -EAGAIN;

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
    int i, ret = 0;

    UWORD *pTmp;

    pr_info("%s, %s()\n", MODULE_NAME, __func__);

    ret = misc_register(&Device1);

    if (ret) {
        pr_info("%s, %s device register failed\n", MODULE_NAME, DEVICE1_NAME);
    }
    else {

        if ((ptr = kmalloc((NPAGES + 2) * PAGE_SIZE, GFP_KERNEL)) != NULL) {

            pTmp = (UWORD*) ((((unsigned long) ptr) + PAGE_SIZE - 1) & PAGE_MASK);

            for (i = 0; i < (NPAGES * PAGE_SIZE); i += PAGE_SIZE) {
                SetPageReserved(virt_to_page(((unsigned long) pTmp) + i));
            }

            pUi = (UI*) pTmp;

            memset(pUi, 0, sizeof(UI));
        }

        Device1Time = ktime_set(0, 50000000);
        hrtimer_init(&Device1Timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        Device1Timer.function = Device1TimerInterrupt1;
        hrtimer_start(&Device1Timer, Device1Time, HRTIMER_MODE_REL);

        Device2Time = ktime_set(0,10000000);
        hrtimer_init(&Device2Timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        Device2Timer.function = Device2TimerInterrupt1;
        hrtimer_start(&Device2Timer, Device2Time, HRTIMER_MODE_REL);

        pr_info("%s, %s device register succes\n", MODULE_NAME, DEVICE1_NAME);
    }

    return ret;
}

static void Device1Exit(void)
{
    int i;
    UWORD *pTmp;

    pr_info("%s, %s()\n", MODULE_NAME, __func__);

    hrtimer_cancel(&Device1Timer);
    hrtimer_cancel(&Device2Timer);

    misc_deregister(&Device1);

    // free shared memory
    pTmp = (UWORD*) pUi;
    pUi = &UiDefault;

    for (i = 0; i < (NPAGES * PAGE_SIZE); i += PAGE_SIZE) {
        ClearPageReserved(virt_to_page(((unsigned long) pTmp) + i));
    }

    if (ptr)
        kfree(ptr);

    pr_info("%s, %s device unregistered\n", MODULE_NAME, DEVICE1_NAME);
}

static irqreturn_t ev3_ui_button_isr(int irq, void *dev_id)
{
    struct ev3_ui_button_data *bdata = dev_id;
    int index = bdata->index;

    pUi->Pressed[index] = gpiod_get_value(bdata->gpiod);

    pr_info("%s, %s(): (index, value) = (%d, %d)\n", MODULE_NAME, __func__, index, pUi->Pressed[index]);

	return IRQ_HANDLED;
}

static int ev3_ui_setup_key(struct platform_device *pdev, struct ev3_ui_button_data *bdata)
{
	int ret;
    int irq;
	unsigned long irqflags;

    unsigned gpio = desc_to_gpio(bdata->gpiod);

    irq = gpio_to_irq(gpio);
    irqflags = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;

	ret = devm_request_irq(&pdev->dev, irq, ev3_ui_button_isr, irqflags, bdata->label, bdata);
	if (ret < 0) {
		pr_err("%s, %s(): Unable to claim irq %d; error %d\n", MODULE_NAME, __func__, irq, ret);
		return ret;
	}

    disable_irq(irq);

    return 0;
}

static struct ev3_ui_priv *ev3_ui_create(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct ev3_ui_priv *priv;

    struct fwnode_handle *child;
	struct device_node *pp;

    const char *type;
    int count_led = 0;
    int count_button = 0;

    struct ev3_ui_led_data *led;
    struct ev3_ui_button_data *button;

    /*
     * allocate memory
     */
    priv = devm_kzalloc(dev, sizeof(struct ev3_ui_priv), GFP_KERNEL);
	if (!priv)
		return ERR_PTR(-ENOMEM);

    device_for_each_child_node(dev, child) {
		if (fwnode_property_present(child, "type")) {
			fwnode_property_read_string(child, "type", &type);

            if (!strcmp("led", type))
                priv->num_leds++;
            else if(!strcmp("key", type))
                priv->num_buttons++;
        }
    }

    if (priv->num_leds) {
        priv->leds = devm_kzalloc(dev, (sizeof(struct ev3_ui_led_data) * priv->num_leds), GFP_KERNEL);

        if (!priv->leds)
            return ERR_PTR(-ENOMEM);

        leds = priv->leds;
    }

    if (priv->num_buttons) {
        priv->buttons = devm_kzalloc(dev, (sizeof(struct ev3_ui_button_data) * priv->num_buttons), GFP_KERNEL);

        if (!priv->buttons)
            return ERR_PTR(-ENOMEM);
    }

    /*
     * TODO: how to improve this, I don't want to do the same thing here and above
     */
    device_for_each_child_node(dev, child) {

        unsigned gpio;

		fwnode_property_read_string(child, "type", &type);

        pp = of_node(child);

        if (!strcmp("led", type)) {

            led = &priv->leds[count_led++];

		    led->label = of_get_property(pp, "label", NULL);
            of_property_read_u32(pp, "index", &led->index);

            led->gpiod = devm_get_gpiod_from_child(dev, NULL, child);

            gpiod_direction_output(led->gpiod, 1);
        }
        else if(!strcmp("key", type)) {
            button = &priv->buttons[count_button++];

		    button->label = of_get_property(pp, "label", NULL);
            of_property_read_u32(pp, "index", &button->index);

            button->gpiod = devm_get_gpiod_from_child(dev, NULL, child);

            ev3_ui_setup_key(pdev, button);

            gpio = desc_to_gpio(button->gpiod);

            enable_irq(__gpio_to_irq(gpio));
        }
    }

    pr_info("%s, %s(): (leds, buttons) = (%d, %d)\n", MODULE_NAME, __func__, priv->num_leds, priv->num_buttons);

    return priv;
}

#if DEBUG
static void dump(struct platform_device *pdev)
{
    int i;
    struct ev3_ui_priv *priv;

    priv = (struct ev3_ui_priv *) platform_get_drvdata(pdev);

    for (i = 0; i < priv->num_leds; i++) {
        pr_info("label: %s, index: %d\n", priv->leds[i].label, priv->leds[i].index);
    }

    for (i = 0; i < priv->num_buttons; i++) {
        pr_info("label: %s, index: %d\n", priv->buttons[i].label, priv->buttons[i].index);
    }

    return;
}
#endif

static const struct of_device_id of_ev3_ui_match[] = {
	{ .compatible = "ev3-ui", },
	{},
};

MODULE_DEVICE_TABLE(of, of_ev3_ui_match);

static int ev3_ui_probe(struct platform_device *pdev)
{
    struct ev3_ui_priv *priv;

    pr_info("%s, %s()\n", MODULE_NAME, __func__);

    priv = ev3_ui_create(pdev);

    platform_set_drvdata(pdev, priv);

#if DEBUG
    dump(pdev);
#endif

    Device1Init();

    return 0;
}

static int ev3_ui_remove(struct platform_device *pdev)
{
    struct ev3_ui_priv *priv;
    struct device *dev = &pdev->dev;

    pr_info("%s, %s()\n", MODULE_NAME, __func__);

    priv = (struct ev3_ui_priv *) platform_get_drvdata(pdev);

    if (priv) {

        if (priv->leds)
            devm_kfree(dev, priv->leds);

        if (priv->buttons)
            devm_kfree(dev, priv->buttons);

        devm_kfree(dev, priv);
    }

    Device1Exit();

    return 0;
}

#ifdef CONFIG_PM_SLEEP
static int ev3_ui_suspend(struct device *dev)
{
	/* Select sleep pin state */
	pinctrl_pm_select_sleep_state(dev);

	return 0;
}

static int ev3_ui_resume(struct device *dev)
{
	/* Select default pin state */
	pinctrl_pm_select_default_state(dev);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(ev3_ui_pm_ops, ev3_ui_suspend, ev3_ui_resume);

static struct platform_driver ev3_ui_driver = {
	.probe		= ev3_ui_probe,
	.remove		= ev3_ui_remove,
	.driver		= {
		.name	= "ev3-ui",
		.of_match_table = of_ev3_ui_match,
		.pm	= &ev3_ui_pm_ops,
	},
};

module_platform_driver(ev3_ui_driver);
