Драйвер - это модуль ядра, файл с суффиксом .ko (kernel object).
В исходнике двайвера реализованы функции открытия / закрытия, а также
операции над драйвером.
Драйвер - это часть ядра и работает в привелегированном режиме.
Драйвер может бьыть загружен в ядро командой **insmod**.
Драйвер может быть выгружен из ядра командой **rmmod**.
Команда **lsmod** покажет статус модулей в ядре.

Для просмотра доступных MAJOR номеров нужно сделать следующее:

```
cat /proc/devices
```

Для общения с драйвером из пространства польтзователя необходимо 
создать спецальный файл устройства:

mknod [device-name] [device-type] [MAJOR MINOR]

Example:
```
mknod /dev/mydevice c 90 0
```
Для обращения к драйверу достаточно выполнить открытие файла с 
именем файоа устройства.

```
open("/dev/mydevice", ...);
```
