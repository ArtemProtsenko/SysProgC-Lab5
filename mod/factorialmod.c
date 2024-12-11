#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // Для роботи з користувацькою пам'яттю
#include <linux/slab.h>

#define DEVICE_NAME "factorialdev"

MODULE_AUTHOR("Artem Protsenko");
MODULE_DESCRIPTION("Lab 5");
MODULE_LICENSE("MIT");

// Змінні для модуля
static int major;
static char result_buffer[256]; // Буфер для зберігання результату

// Функція обчислення факторіала
static long factorial(int n) {
    long res = 1;
    int i;
    for (i = 1; i <= n; i++) {
        res *= i;
    }
    return res;
}

// write: Приймає число від користувача
static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    int n;
    kstrtoint_from_user(buf, len, 10, &n);
    snprintf(result_buffer, sizeof(result_buffer), "Factorial of %d is %ld\n", n, factorial(n));
    return len;
}

// read: Передає результат користувачу
static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    size_t msg_len = strlen(result_buffer);
    len = msg_len - *offset;
    copy_to_user(buf, result_buffer + *offset, len);
    *offset += len;
    return len;
}

// Операції над пристроєм
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = dev_write,
    .read = dev_read,
};

// Завантаження модуля
static int __init factorial_module_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ERR "Factorial module: Failed to register device\n");
        return major;
    }
    printk(KERN_INFO "Factorial module: Registered with major number %d\n", major);
    return 0;
}

// Вивантаження модуля
static void __exit factorial_module_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Factorial module: Unregistered device\n");
}

module_init(factorial_module_init);
module_exit(factorial_module_exit);
