# M1 Linux TSO

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

### Turn on TSO

```bash
echo s | sudo tee /sys/kernel/m1tso/status
```

### Turn off TSO

```bash
echo c | sudo tee /sys/kernel/m1tso/status
```

## How to Test?

See [tsettso](https://github.com/saagarjha/TSOEnabler/blob/master/testtso/main.c) in [saagarjha/TSOEnabler](https://github.com/saagarjha/TSOEnabler).