# otus project: Adapter
**Требования для сборки и запуска:**
* Debian
* Cmake
* Docker Compose

## Сборка проекта
**Команды по сборке выполняются из корневой директории проекта**
```
mkdir build
cd build
cmake ..
cmake --build .
```
## Сборка и запуск контейнеров
**Представленные команды выполняются из корневой директории проекта**\
**Необходимо иметь на машине установленный docker**
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
**Представленные команды выполняются из директории в которой был запущен контейнер (/home/user/)**\
**Команды достаточно выполнить один раз при первом старте контейнеров**\
**Это необходимо для настройки маршрутизации и возможности работы с сокетами не относящимися к локальной сети**\
**Более подробную информацию можно получить здесь:**
* https://docs.kernel.org/networking/tproxy.html
* https://www.netfilter.org/projects/nftables/manpage.html
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
### Описание сети между контейнерами
![Alt image](./diagrams/ContainerNetwork.svg)
## Запуск ПО
**Представленные команды выполняются из директории в которой был запущен контейнер (/home/user/)**
### Контейнер server
```
nc -l -p [port]
```
### Контейнер adapter
#### Опции командной строки
```
sudo ./build/adapter -h

Options:
  -h [ --help ]                        this screen
  -p [ --ports ] arg                   ports
  -c [ --context-number ] arg (=4)     context number possible values [1;8]
  -s [ --logger-severity ] arg (=none) logger severity {none, info, warning, 
                                       error, debug}
  -v [ --verbose ] [=arg(=1)] (=0)     verbose
```
#### Запуск без логирования
```
sudo ./build/adapter -p 5555
```
#### Запуск с логированием
```
sudo ./build/adapter -p 5555 -s <logger severity>
```
### Контейнер client
```
nc 192.168.2.12 [port]
```
# Описание проекта
Описание проекта представлено по модели C4 (https://c4model.com/)
## Диаграмма контекста системы
![Alt image](./diagrams/SystemContextDiagram.svg)
## Диаграмма контейнеров
![Alt image](./diagrams/ContainerDiagram.svg)
## Диаграмма компонентов
![Alt image](./diagrams/ComponentDiagram.svg)