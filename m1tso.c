#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/types.h>

MODULE_AUTHOR("Yangyu Chen <cyy@cyyself.name>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TSO Enabler for Apple M1");

static uint64_t m1tso_read_actlr_el1(void) {
    uint64_t res;
    asm volatile(
        "mrs %0, ACTLR_EL1;\n"
        : "=r"(res)
        :
    );
    return res;
}

static void m1tso_set_actlr_el1(uint64_t val) {
    asm volatile(
        "msr ACTLR_EL1, %0;\n"
        : 
        : "r"(val)
    );
    pr_info("on CPU [%d], actlr_el1 set to %llx, value after set is %llx.\n", smp_processor_id(), val, m1tso_read_actlr_el1());
}

int m1tso_status[NR_CPUS];

static void m1tso_query(void *info) {
    m1tso_status[smp_processor_id()] = (m1tso_read_actlr_el1() & 0x2) >> 1;
}

static void m1tso_settso(void *info) {
    m1tso_set_actlr_el1(m1tso_read_actlr_el1()|0x2);
    m1tso_query(info);
}

static void m1tso_cleartso(void *info) {
    m1tso_set_actlr_el1(m1tso_read_actlr_el1()&(~(0x2*1llu)));
    m1tso_query(info);
}


static ssize_t m1tso_status_load(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    on_each_cpu(m1tso_query, NULL, 0);
    ssize_t size = 0;
    for (int i=0;i<NR_CPUS;i++) if (m1tso_status[i] != -1) {
        size += sprintf(buf+size, "CPU[%d].TSO=%d\n", i, m1tso_status[i]);
    }
    return size;
}

static ssize_t m1tso_status_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t cnt) {
    if (cnt >= 1) {
        if (buf[0] == 's') {
            on_each_cpu(m1tso_settso, NULL, 0);
        }
        else if (buf[0] == 'c') {
            on_each_cpu(m1tso_cleartso, NULL, 0);
        }
    }
    return cnt;
}

static struct kobj_attribute m1tso_status_query = __ATTR(status, 0664, m1tso_status_load, m1tso_status_store);

static struct attribute *m1tso_attrs[] = {
    &m1tso_status_query.attr,
    NULL,
};

static struct attribute_group m1tso_attr_group = {
    .attrs = m1tso_attrs,
};

struct kobject *m1tso_kobj;

static int __init m1tso_init(void) {
    int ret = 0;
    for (int i=0;i<NR_CPUS;i++) m1tso_status[i] = -1;
    on_each_cpu(m1tso_query, NULL, 0);
    m1tso_kobj = kobject_create_and_add("m1tso", kernel_kobj);
    if (!(m1tso_kobj)) ret = -ENOMEM;
    ret = sysfs_create_group(m1tso_kobj, &m1tso_attr_group);
    if (ret) kobject_put(m1tso_kobj);
    return ret;
}

static void __exit m1tso_exit(void) {
    sysfs_remove_group(m1tso_kobj, &m1tso_attr_group);
    kobject_put(m1tso_kobj);
}

module_init(m1tso_init);
module_exit(m1tso_exit);