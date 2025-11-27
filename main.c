#include <linux/module.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <linux/device.h>

#define BUF_SIZE 32
#define RING_BUF_SIZE 4000
#define RING_BUFFER_IOCTL_CLEAR _IO('c', 0)

static DEFINE_RWLOCK(lock);
static char usr_buf[BUF_SIZE] = {0, };
static char ringBuffer[RING_BUF_SIZE] = {0, };
static int rear = 0, front = 0;

static ssize_t ringbuffer_device_read(struct file *file, char __user *buf, size_t len, loff_t *ppos){
        ssize_t ret;
        read_lock(&lock);
        if(BUF_SIZE <= len + *ppos) {
                len = BUF_SIZE - *ppos;
        }
        ret = len - copy_to_user(buf, usr_buf + *ppos, len);
        int range = rear - front;
        for(int i = 0; i < range; i++) {
                if(rear ==  front)
                        return ret;
                else{
                        usr_buf[i] = ringBuffer[front];
                        ringBuffer[front] = 0;
                        front = (front + 1) % RING_BUF_SIZE;
                }
        }
        *ppos += ret;
        read_unlock(&lock);
        return ret;
}

static ssize_t ringbuffer_device_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos){
        ssize_t ret;
        write_lock(&lock);
        if(BUF_SIZE <= len + *ppos) {
                len = BUF_SIZE - *ppos;
        }
        ret = len - copy_from_user(usr_buf + *ppos, buf, len);
        for(int i = 0; i < len; i++) {
                if((rear + 1) % RING_BUF_SIZE == front)
                        return ret;
                else{
                        ringBuffer[rear++] =  usr_buf[i];
                        rear %= RING_BUF_SIZE;
                    }
        }
        *ppos += ret;
        write_unlock(&lock);
        return ret;
}

static long ringbuffer_devie_ioctl(struct file *fp, unsigned int cmd, unsigned long arg) {
        switch (cmd) {
        case RING_BUFFER_IOCTL_CLEAR:
                memset(usr_buf, 0, BUF_SIZE);
                break;
        default:
                printk(KERN_DEBUG "%s failed - %d\n", __func__, cmd);
                return -EINVAL;
        }
        return 0;
}

static struct file_operations fops = {
        .read = ringbuffer_device_read,
        .write = ringbuffer_device_write,
        .unlocked_ioctl = ringbuffer_devie_ioctl,
};

static struct class *class;
static struct device *device;

static int __init ringbuffer_module_init(void){
        printk(KERN_DEBUG "%s\n", __func__);
        int major = register_chrdev(0, "ringbuffer", &fops);
        int minor = 0;
        class = class_create("ringbuffer");
        device = device_create(class, NULL, MKDEV(major, minor), NULL, "%s%d", "ringbuffer", minor);
        return 0;
}

static void __exit ringbuffer_module_exit(void){
        unregister_chrdev(0,"ringbuffer");
        printk(KERN_DEBUG "%s\n", __func__);
}

module_init(ringbuffer_module_init);
module_exit(ringbuffer_module_exit);

MODULE_AUTHOR("kcjin<wlsrudcjs@naver.com>");
MODULE_DESCRIPTION("Ring Buffer");
MODULE_LICENSE("GPL v2");
