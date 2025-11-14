**Структура проекта**

  - include/ - h-файлы
  - src/ - cpp-файлы
  - CMakeLists.txt - cmake файл
  - Doxyfile - конфиг генерации документации для doxygen

**Сборка проекта**

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`

**Запуск программы**

`./hatch_generator --points x1 y1 x2 y2 x3 y3 x4 y4 --angle <degrees> --step <distance> [--svg <filename>]`

**Документация**

`doxygen Doxyfile`
`open docs/html/indexcd .html`