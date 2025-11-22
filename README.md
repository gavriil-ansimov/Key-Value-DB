# README

Небольшой TCP key-value сервер (аналог простейшего Redis).

## Опции запуска
Server:
- `-p <port>`
Client:
- `-p <port>`
- `-a <addr>`

## Команды

-   `PUT <key> <value>` --- сохранить значение
-   `GET <key>` --- получить значение
-   `DEL <key>` --- удалить ключ
-   `COUNT` --- количество ключей
-   `DUMP <filename>` --- сохранить базу в указанный файл
-   `LOAD <filename>` --- загрузить базу из указанного файла

## Пример

    PUT name Vasya
    OK

    GET name
    OK Vasya

## Сборка
```bash
    mkdir build && cd build
    cmake ..
    make
    ./kvserver
```
