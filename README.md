# otus

bayan

## Примеры

В репозитории присутствует папка Test относительно неё будут показаны примеры. Все команды вызываются из корня репозитория.

## Параметры
```
$ bayan -h
Options:
  -h [ --help ]                        this screen
  -i [ --include-paths ] arg           include paths
  -e [ --exclude-paths ] arg           exclude paths
  -d [ --depth ] arg (=0)              scan depth
  -s [ --min-file-size ] arg (=1)      minimum file size
  -p [ --patterns ] arg                file name pattern
  -b [ --block-size ] arg (=5)         block size
  -a [ --hash-algorithm ] arg (=crc16) hash algorithm {crc16, crc32, md5, sha1}
  -v [ --verbose ] [=arg(=1)] (=0)     verbose
```

### Примеры
```
$ bayan -i Test/
```
```
$ bayan -i Test/ -d 1
Test/0_2/1.txt
Test/1.txt
```
```
$ bayan -i Test/ -d 2
Test/0_2/1.txt
Test/1.txt

Test/0_2/2.txt
Test/0_2/1_1/1.txt

Test/0_2/1_1/2.txt
Test/2.txt
```
```
$ ./build/bayan -i Test/ -d 2 -p 2
Test/0_2/1_1/2.txt
Test/2.txt
```
```
$ ./build/bayan -i Test/ -d 2 -p 1
Test/0_2/1.txt
Test/1.txt
```
```
$ ./build/bayan -i Test/ -d 2 -p 1 -s 1
Test/0_2/1.txt
Test/1.txt
```
```
$ ./build/bayan -i Test/ -d 2 -p 1 -s 2
```
```
$ ./build/bayan -i Test/ -d 2 -e 1_1
Test/0_2/1.txt
Test/1.txt
```
```
$ ./build/bayan -i Test/ -d 2 -v
Options:
Block size: 5
Depth: 2
Hash algorithm: crc16
Include paths: ["Test/"]
Minimum file size: 1
Verbose: true

Hash: 34a2
Test/0_2/1.txt
Test/1.txt

Hash: ae47
Test/0_2/2.txt
Test/0_2/1_1/1.txt

Hash: eaf3
Test/0_2/1_1/2.txt
Test/2.txt
```
```
$ ./build/bayan -i Test/ -d 2 -v -a crc16
Options:
Block size: 5
Depth: 2
Hash algorithm: crc16
Include paths: ["Test/"]
Minimum file size: 1
Verbose: true

Hash: 34a2
Test/0_2/1.txt
Test/1.txt

Hash: ae47
Test/0_2/2.txt
Test/0_2/1_1/1.txt

Hash: eaf3
Test/0_2/1_1/2.txt
Test/2.txt
```
```
$ ./build/bayan -i Test/ -d 2 -v -a crc32
Options:
Block size: 5
Depth: 2
Hash algorithm: crc32
Include paths: ["Test/"]
Minimum file size: 1
Verbose: true

Hash: a8ff9891
Test/0_2/1_1/2.txt
Test/2.txt

Hash: e1ea3986
Test/0_2/1.txt
Test/1.txt

Hash: ffacf3f6
Test/0_2/2.txt
Test/0_2/1_1/1.txt
```
```
$ ./build/bayan -i Test/ -d 2 -v -a md5
Options:
Block size: 5
Depth: 2
Hash algorithm: md5
Include paths: ["Test/"]
Minimum file size: 1
Verbose: true

Hash: 44f317d4c47ebf9ac75fc4effe75c790
Test/0_2/1_1/2.txt
Test/2.txt

Hash: 4838898280633b15ab1f168584348239
Test/0_2/2.txt
Test/0_2/1_1/1.txt

Hash: 893cad4413996c289e06e988cbc70f24
Test/0_2/1.txt
Test/1.txt
```
```
$ ./build/bayan -i Test/ -d 2 -v -a sha1
Options:
Block size: 5
Depth: 2
Hash algorithm: sha1
Include paths: ["Test/"]
Minimum file size: 1
Verbose: true

Hash: 2a57199422a8c07c87b25b215462272e5cafc327
Test/0_2/1_1/2.txt
Test/2.txt

Hash: 5600353ac49429d26faad910942b3e5b65a0738a
Test/0_2/2.txt
Test/0_2/1_1/1.txt

Hash: c8dcf54c92f83d38b48185306a9a2723a8d9ea4c
Test/0_2/1.txt
Test/1.txt
```