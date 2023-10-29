#### СОДЕРЖАНИЕ

```
src-01 - пример драйвера LED (абстракция - ч.1);
src-02 - пример драйвера LED (talk-to-hardware - ч.2);
src-03 - пример драйвера LED (led framework - ч.3); 
src-04 - пример драйвера LED (access to HW via GPIOLIB - ч.4);
src-05 - пример драйвера LED (device tree, bus infrastructure - ч.5).
```

#### Введение

1. Скомпилировать драйвер. Для этого в корневом каталоге исходников драйвера выполнить:
```
make
```

2. Создать файл устройства. Для этого необходимо выполнить:
```
sudo mknod /dev/device-name c MAJOR MINOR
```
- device-name - имя файла устройства;
- с - указание, того что это character device;

**NOTE:** для просмотра MAJOR номера выполнить:
```
cat /proc/devices
```

3. Загрузка драйвера. Для этого необходимо выполнить:
```
make load
```

4. Тестирование. 
```
echo 1 > /dev/device-name
echo 0 > /dev/device-name
cat /dev/device-name
```

**NOTE:** изменить владельца файла /dev/device-name
```
sudo chown USER:GROUP /dev/device-name
```

5. Выгрузка драйвера. Для этого необходимо выполнить:
```
make unload
```

#### Аппаратура

Существует 2 основных способа MCU общения с переферийным устройством:
- Port I/O - выделенная шина используется для общения с устройством;
- Memory-mapped I/O - отображение памяти устройства в процесс драйвера.

1. Запрос доступа к MMIO;
```
request_mem_region();
```

2. Отображение адресов физической памяти в адреса виртуальной памяти;
```
ioremap();
```

3. Использовать API ядра для чтения / записи регистров.
```
readl()/writel();
```

**NOTE:**
Адрес периферии можно узнать выполнив команду:
```
sudo cat /proc/iomem
```

#### Архитектура драйвера

**Примечание:** Недостатки примера src-02:

- интерфейс пользователя не стандартизирован;
- использование одной линии gpio приводит к блокировки всего gpio модуля, другие драйверы (приложения)
не могут пользоваться gpio;
- драйвер имеет информацию о железе, при смене железа, драйвер тоже нужно менять. Драйвер должен
предоставлять абстракцию и уметь работать с любым железом.

#### Решение первого недостатка. LED framework.

- инициализация структуры **led_classdev**;
- привязка callback ф-ии для изменения состояни LED;
- регистрация драйвера в framework **led_classdev_register()**.

**Тест:**
1. Выполнить команды:

```
make load
cd /sys/class/leds
ls -l
```

2. Убедиться что есть новый интерфейс ipe:blue:user

3. Выполнить:
```
cd ipe:blue:user
```
4. Изменить влыдельца:
```
sudo chown USER:GPOUP brightness
sudo chown USER:GPOUP trigger
```
5. Выполнить:
```
echo heartbeat > trigger
```

6. Выполнить:
```
make unload
```

#### Решение второго недостатка. GPIO lib

Тестирование как в предыдущем пункте

#### Решение третьего недостатка. Device treee (bus infrastructure)

4 основных компонента:
- bus core - API impelentation of a given bus (USB core, SPI core etc) represented in the kernel by **bus_type** structure;
- bus adapter - bus controller driver, represented in the kernel by **device_driver** structure;
- bus driver - drivers responsible for managing a device conneted to the bus, represented in the kernel by **device_driver** structure;
- bus devices - device connected to the bus, represented in the kernel by **device** structure;

![driver-bus](https://github.com/GIYura/raspi/blob/main/drivers/ldd/led/driver-bus.png)

