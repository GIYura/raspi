#### СОДЕРЖАНИЕ

```
src-0 - пример драйвера LED (абстракция);
src-1 - 
src-2 - 
src-3 - 
src-4
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

