#### Cross-compilation

**Host:**

1. Download cross compiler:
```
git clone git@github.com:raspberrypi/tools.git
```

2. Specify PATH in ~/.bashrc:
```
export PATH=$PATH:~/playground/rpi/toolchain/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
```

3. Create test **.c** file:
```
touch test.c
```

4. Compile:
```
arm-linux-gnueabihf-gcc test.c -o test
```

5. Check file format:
```
file [exe-file-name]
```

6. Transfer **exe** file to target (via sftp):
```
sftp [user-name]@[target-ip]
put [exe-file][path]
```

**Target:**

1. Run **exe** file:
```
./[exe-file-name]
```

