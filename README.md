# TSOEnabler for Linux

A Linux Kernel Module that enables Total-Store-Ordering on Apple Silicon with Linux.

It was tested on `linux-asahi-5.19.0-5-1-ARCH` with Apple Mac mini (M1, 2020).

## How to Use?

### Load the kernel module

```bash
make
sudo insmod m1tso.ko
```

### Query status

```bash
sudo cat /sys/kernel/m1tso/status
```

```
CPU[0].TSO=0
CPU[1].TSO=0
CPU[2].TSO=0
CPU[3].TSO=0
CPU[4].TSO=0
CPU[5].TSO=0
CPU[6].TSO=0
CPU[7].TSO=0
```

### Turn on TSO

```bash
echo s | sudo tee /sys/kernel/m1tso/status
```

### Turn off TSO

```bash
echo c | sudo tee /sys/kernel/m1tso/status
```

## How to Test?

See [testtso](https://github.com/saagarjha/TSOEnabler/blob/master/testtso/main.c) in [saagarjha/TSOEnabler](https://github.com/saagarjha/TSOEnabler).

## Note for AsahiLinux Kernel > v6.3

Please recompile the kernel with `CONFIG_ARM64_ACTLR_STATE=n` and `CONFIG_ARM64_MEMORY_MODEL_CONTROL=n` in your kernel `.config`.

Related Issues: [https://github.com/AsahiLinux/linux/issues/189](https://github.com/AsahiLinux/linux/issues/189) [https://github.com/cyyself/m1tso-linux/issues/2](https://github.com/cyyself/m1tso-linux/issues/2)
