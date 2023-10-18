### Intro
#### Передача параметров модулю

1. Для передачи параметров используется макрос:

```
module_param()
module_param_array()
module_param_string()
```

2. Для просмотра информации o модуле:

* modinfo -p <module-name.ko> - Список возможных параметров.
* modinfo -a <module-name.ko> - Информация об авторе.
* modinfo -d <module-name.ko> - Описание модуля.

3. Передача параметров

sudo insmod <module-name.ko> [param-name=value]

**Example:**

```
sudo insmod params.ko valueInt=2222
```
