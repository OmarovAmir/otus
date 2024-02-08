# otus project: Adapter
## Сборка проекта
```
mkdir build
cd build
cmake ..
cmake --build .
```
## Сборка и запуск контейнеров
### Проверка запущены ли контейнеры
```
sudo docker compose -f docker/adapter.yaml ps -a
```
### Сборка образов и запуск контейнеров
```
sudo docker compose -f docker/adapter.yaml up -d
```
### Вход в контейнер client
```
sudo docker compose -f docker/adapter.yaml exec --privileged client bash
```
### Вход в контейнер server
```
sudo docker compose -f docker/adapter.yaml exec --privileged server bash
```
### Вход в контейнер adapter
```
sudo docker compose -f docker/adapter.yaml exec --privileged adapter bash
```
### Выключение контейнеров и удаление образов
```
sudo docker compose -f docker/adapter.yaml down --rmi all
```
## Подготовка к работе
### Настройка контейнера client
```
sudo docker/scripts/client/client.sh
```
### Настройка контейнера server
```
sudo docker/scripts/server/server.sh
```
### Настройка контейнера adapter
```
sudo docker/scripts/adapter/adapter.sh
```
