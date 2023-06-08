# CAN NodeID Filter

Фильтрация строк текстового вывода CAN dump по nodeID и пересылка удовлетворяющих фильтру строк по UDP.

## Сборка

Файл Config.h содержит директивы для настроки сборки программы. Предусмотрено переключение между WinSock и классическим сокетом (для отладки), учёт символа переноса строки и выбор типа входного потока, который на данный момент единственный.

Для сборки (исполняемый файл CANFilter):

> make
 
Для очистки (.o и исполняемый файл):

> make clean

## Использование

Пример запуска:

> CANFilter -f ~/can_dump.txt -id 1 2 39 -ip 192.168.1.8 -port 1337

- При отсутствии аргумента файла -f будет вызвано исключение "Filename cannot be null or empty", при отсутствии файла по указанному пути - "Cannot open file". Пробелы допускаются, но являются нежелательными.
- Аргумент -id может принимать несколько nodeID (в десятичной системе счисления), просто потому что способ разбора аргументов позволяет это легко сделать.
- Аргументы -ip и -port задают адрес сервера для переброса отфильтрованных строк. Если не указаны, будет выбрано значение по-умолчанию: 127.0.0.1:1337.

## Ограничения

Файл может иметь сколь угодно большой объём, но должен быть разделён строками согласно тому, как это делает CAN dump, т.е. короткими строчками. Строки хранятся в памяти целиком до момента завершения их анализа.

Нет как такового обнаружения формата строки. Любая таблица с допустимой третьей колонкой будет принята фильтром. Обнаружение формата строки, доколе можно сделать, многократно замедлит анализ.
