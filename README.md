## Тестовое задание [task.pdf](task.pdf)

### Requirements
- **Ubuntu 22.04+**
- **Python 3.12+**
- **Pip**
### **or**
- **Docker**: [Install Docker Ubuntu](https://docs.docker.com/engine/install/ubuntu/)
- **Docker Compose**: [Install Docker Compose on Linux](https://docs.docker.com/compose/install/linux/)

```
$ git clone https://github.com/avtakhov/yadro-test.git
$ cd yadro-test
$ chmod +x ./parse/install.sh
$ chmod +x ./parse/start.sh
```

## Легкий путь (при наличии docker compose)
```
$ docker compose up --build
```

Запускает fastapi сервер, на [localhost:8080/docs](http://localhost:8080/docs) можно посмотреть
swagger документацию и самостоятельно отправить запрос (например, `/clear` для удаления всех точек)

Как только сервер ответит на healthcheck запрос, запустится установка boost и сборка C++.
На сервер будут отправлены файлы из директории example. [Подробнее](parse/Dockerfile)

После отпрвки контейнер завершится с кодом 0. Сервер продолжит работу до ручной остановки.
Картинку можно получить на [localhost:8080/points/show](http://localhost:8080/points/show)


## Путь ниндзя (самостоятельная сборка)
#### Запуск сервера
```
$ cd server
$ pip install -r requirements.txt
$ uvicorn main:app --host 0.0.0.0 --port 8080
```

Swagger также расположен на [localhost:8080/docs](http://localhost:8080/docs)

```
$ cd parse
$ sudo ./install.sh
$ export PARSE_API_HOST=localhost
$ export PARSE_API_PORT=8080
$ ./start.sh ~/yadro-test/example/a.txt ...
```

## Пояснения к решению

Передача точек происходит по сети, протокол http. Не самое эффективное решение,
есть свои плюсы: масштабируемость, абстракция от процессов и машин, готовые решения в виде fastapi и pydantic

### Интерфейс парсера
```c++
class PointProducer {
public:
  using Generator = external::generator<Point>;
  virtual Generator get_points() = 0;
  virtual std::string get_source() = 0;
  virtual ~PointProducer() = default;
};
```
Класс `generator` позволяет обрабатывать точки по мере поступления, **без использования callback'ов!**

С gcc-14 можно было бы воспользоваться `std::generator` из c++23, но gcc-14 не завезли в Ububntu 22.04.

### Отправка точек

Точки группируются в пачки и отправляются на сервер. Данные для подключения приходят из переменных окружения
`PARSE_API_HOST` и `PARSE_API_PORT`.

С gcc-14 можно было группировать точки с `std::views::chunk` из c++23, но gcc-14 не завезли в Ububntu 22.04.

### Парсер
Для генерации парсеров использовалась шаблонная библиотека комбинаторных парсеров boost spirit.
Boost spirit умеет парсить текстовые **и бинарные** данные.
```c++
x3::lexeme [ +(x3::char_ - ':' - x3::eol) ]
>> ':' 
>> x3::int_ 
>> ',' 
>> x3::int_ 
>> -x3::eol
```
