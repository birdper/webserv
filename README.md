
# Webserv
Однопоточный HTTP-сервер с неблокирующим вводом-выводом. Сервер поддерживает версию HTTP 1.1.

Приложение написано на c++ 98, без использования стооронних библиотек.  
Для опроса событий используется метод `poll`.  


## How to Start

Для сборки проекта выполните команду `make` в корне проекта.
```
make
```

Для запуска приложения вы можете указать конфигурационный файл. Или оставить пустым, тогда будет использоваться конфигурационный файл по умолчанию `webserv.conf`
```
./webserv [path_to_config_file](opt)
```


## Конфигурация
Конфигурация вдохновлена nginx, и во многом имеет схожее поведение.


В общем случае конфигурационный файл может содержать несколько блоков server, 
различаемых по портам, на которых они слушают, и по имени сервера.  

Определив, какой server будет обрабатывать запрос, nginx сравнивает URI, указанный в заголовке запроса, 
с параметрами директив location, определённых внутри блока server.


Пример:
```
server { 
    listen 127.0.0.1:80;
    
    location / {
        root /html/www;
    }
    
    location /some_url {
        root /html/www;
    }
}

server { 
    listen 127.0.0.1:80;
    server_name ;
    
    location / {
        root /html/www/another;
    }
    
    location /some_url {
        root /html/www/another;
    }
}
```

### Parameters:
[server](#server)  
[listen](#listen)  
[server_name](#server_name)  
[location](#location)  
[root](#root)  
[index](#index)  
[autoindex](#autoindex)  
[error_page](#error_page)  
[client_max_body_size](#client_max_body_size)  
[cgi_path](#cgi_path)  
[upload_directory](#upload_directory)  


<a name="server"><h3>server</h3></a>  
```
Синтаксис:	server { ... }
Умолчание:	—
Контекст:	http
```
Задаёт конфигурацию для виртуального сервера. Чёткого разделения виртуальных серверов на IP-based (на основании IP-адреса) и 
name-based (на основании поля “Host” заголовка запроса) нет.   
Вместо этого директивами `listen` описываются все адреса и порты, 
на которых нужно принимать соединения для этого сервера, а в директиве `server_name` указываются все имена серверов.
***



<a name="listen"><h3>listen</h3></a>
```
Синтаксис:	listen адрес[:порт]; listen порт;
Умолчание:	listen *:80 | *:8000;
Контекст:	server
```
Задаёт адрес и порт для IP, на которых сервер будет принимать запросы. 
Можно указать адрес и порт, либо только адрес или только порт. 
Кроме того, адрес может быть именем хоста, например:
```
listen 127.0.0.1:8000;
listen 127.0.0.1;
listen 8000;
listen localhost:8000;
```
Если указан только адрес, то используется порт 80.
Cервером по умолчанию будет первый сервер, в котором описана пара `адрес:порт`.
***



<a name="server_name"><h3>server_name</h3></a>
```
Синтаксис:	server_name имя ...;
Умолчание:	server_name "";
Контекст:	server
```
Задаёт имена виртуального сервера, например:
```
server {
    server_name example.com www.example.com;
}
```
Первое имя становится основным именем сервера.
***



<a name="location"><h3>location</h3></a>
```
Синтаксис:	location uri { ... }
Умолчание:	—
Контекст:	server
```
Устанавливает конфигурацию в зависимости от URI запроса.
***



<a name="root"><h3>root</h3></a>
```
Синтаксис: root путь;
Умолчание: root html;
Контекст: server, location
```
Задаёт корневой каталог для запросов. Например, при такой конфигурации
```
location /i/ {
    root /data/w3;
}
```
в ответ на запрос `/i/top.gif` будет отдан файл `/data/w3/top.gif`.

Путь к файлу формируется путём замены URI к значению директивы root.
***



<a name="index"><h3>index</h3></a>
```
Синтаксис:	index файл ...;
Умолчание:	index index.html;
Контекст:	server, location
```
Определяет файлы, которые будут использоваться в качестве индекса.  
Наличие файлов проверяется в порядке их перечисления.
***



<a name="autoindex"><h3>autoindex</h3></a>
```
Синтаксис:	autoindex on | off;
Умолчание:	autoindex off;
Контекст:	server, location
```
Разрешает или запрещает вывод листинга каталога.
***



<a name="error_page"><h3>error_page</h3></a>
```
Синтаксис:	error_page код ... [=[ответ]] uri;  
Умолчание:	—  
Контекст:	server, location  
```
Пример:
```
error_page 404             /404.html;
error_page 500 502 503 504 /50x.html;
```
***



<a name="client_max_body_size"><h3>client_max_body_size</h3></a>
```
Синтаксис:	client_max_body_size размер;
Умолчание:	client_max_body_size 1m;
Контекст:	server, location
```
Задаёт максимально допустимый размер тела запроса клиента. Если размер больше заданного, то клиенту возвращается ошибка 413 (Request Entity Too Large).  
Установка параметра размер в 0 отключает проверку размера тела запроса клиента.
***



<a name="methods_allowed"><h3>methods_allowed</h3></a>
```
Синтаксис: methods_allowed метод ... ;
Умолчание: 
Контекст: server, location
```
Содержит методы которые разрешены в контексте.  

Example:
```
methods_allowed post, delete;
```
***



<a name="upload_directory"><h3>upload_directory</h3></a>
```
Синтаксис:	upload_directory <directry>;
Умолчание:	none;
Контекст:	server, location
```
Указывает каталог в который будут записаны загружаемые файлы
***


<a name="cgi_path"><h3>cgi_path</h3></a>
```
Синтаксис:	cgi_path <directry>;
Умолчание:	none;
Контекст:	server, location
```
Указывает cgi обработчик
***



[//]: # (```)
[//]: # (cgi_extension)
[//]: # (return 301 - [String] &#40;path or url to redirect&#41;  )
[//]: # (```)

[//]: # (Example configurations files are server.conf, tester_config.conf)
