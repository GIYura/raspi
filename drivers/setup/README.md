#### Общая информация

Перед началом работы над модулями ядра необходимо выполнить:

1. Установить заголовочные файлы ядра:
```
sudo apt install raspberrypi-kernel-headers
```

2. Проверить содержимое каталога **/usr/src/**, который содержит
установленные заголовочные файлы ядра:

```
ls -l /usr/src/
```

3. Проверить, что версия ядра соответсвует версии модудей ядра:
```
uname -r
```

**Примечание:** после установки OS raspbian версия ядра может не совпадать с 
заголовочными файлами, при этом невозможно загрузить модуль ядра, т.к. собран он для другой версии.

Для того чтобы версия ядра и заголовочные файлы совпадали необходимо выполнить:

1. Обновить ядро:
```
sudo apt install raspberrypi-kernel
```
2. Проверить список **.img** файлов:
```
ls -l /boot
```
 
3. В файле **/boot/config.txt** добавить строку **kernel=kernel7vl.img**

4. Выполнить перезагрузку.

5. Проверить, что версия ядра и заголовочных файлов ядра совпалают:
```
uname -r
ls -l /usr/src
```

**NOTE:**

Модули ядра находятся по пути:
```
ls -l /lib/modules/'(uname -r)'
```

Заголовочный файлы ядра находятся: 
```
ls -l /usr/src/linux-headers-'(uname -r)'
```
**NOTE:**
В каталоге /lib/modules/'(uname -r)' должна быть софт линк на  /usr/src/linux-headers-'(uname -r)'.

**Полезные ссылки:**

Video: [Source](https://www.youtube.com/playlist?list=PLCGpd0Do5-I3b5TtyqeF1UdyD4C-S-dMa)

Code: [Repo](https://github.com/Johannes4Linux/Linux_Driver_Tutorial)

Theory: [Reference](https://linux-kernel-labs.github.io/refs/heads/master/labs/device_drivers.html)

