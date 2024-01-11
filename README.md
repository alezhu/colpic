# Генератор превью в gcode-файлах созданных в PrusaSlicer для принтеров Elegoo Neptune

## Основная информация

[PrusaSlicer](https://www.prusa3d.com/page/prusaslicer_424/) может генерировать превью (thumbnail) печатаемой модели в gcode-файлах.
Принтеры Elegoo Neptune не понимают такие превью. Данная программа конвертирует превью в нужный формат.

Уже есть похожий проект - [ElegooNeptuneThumbnails-Prusa](https://github.com/Molodos/ElegooNeptuneThumbnails-Prusa)
Данный проект является [улучшенной](#чем-лучше-elegooneptunethumbnails-prusa) версией.  

Код не содержит платформозависимых конструкций, но релизы собираются только для Windows.

## Использование

1. Скачайте архив из раздела [релизов](https://github.com/alezhu/colpic/releases)
2. Распакуйте exe-файл по удобному пути
3. Укажите полный путь до exe-файла в настройках PrusaSlicer как post-process script
4. Укажите в настройках печати генерировать preview нужного размера

Подробности с картинками есть в оригинальном проекте [ElegooNeptuneThumbnails-Prusa](https://github.com/Molodos/ElegooNeptuneThumbnails-Prusa)

Программу также можно запускать из командной строки, передавай ей путь к gcode-файлу и другие параметры.

## Чем лучше [ElegooNeptuneThumbnails-Prusa](https://github.com/Molodos/ElegooNeptuneThumbnails-Prusa)?

1. Меньше весит
2. Быстрее работает
3. Есть дополнительные возможности

[ElegooNeptuneThumbnails-Prusa](https://github.com/Molodos/ElegooNeptuneThumbnails-Prusa) написан на Python и скомпонован в exe-файл.
Внутри exe-файл содержит сам python, используемые библиотеки ну и код самого скрипта (возможно, скомпилированный в байт-код python). При каждом запуске всё это дело распаковывается во временную папку, скрипт выполняется, потом всё удаляется. Понятно это отъедает впустую время и ресурсы.
Сам скрипт использует биндинги к Qt6 (для работы с PNG) и библиотеку ColPic_X64 (для перекодирования) - которые написаны на C++. Это улучшает картину, но не сильно, к тому же сам ColPic_X64 написан местами так себе. Начная с версии 2 автор переписал логику без использования библиотек ColPic_X64, но это как минимум ничего не ускорило (скорее даже слегка замедлило) и не уменьшило размер. 

Данная программа (elegoo_neptune_thumbnails_prusa_az) написана целиком на С++.
В скомпилированном виде без всяких зависимостей она весит около 800Кб (против примерно 27,5Мб у [ElegooNeptuneThumbnails-Prusa](https://github.com/Molodos/ElegooNeptuneThumbnails-Prusa)).
Работает примерно в 3 раза быстрее. В абсолютных цифрах это не так чтобы уж очень важно (например, файл, который [ElegooNeptuneThumbnails-Prusa](https://github.com/Molodos/ElegooNeptuneThumbnails-Prusa) обработает за 1,5 секунды, эта программа обработает за 0,5 сек), но почему бы не ускориться, если это возможно?

Кроме того, программе можно передать дополнительные параметры, перечисленные в следующем разделе, которые также могут повлиять на производительность.

Также существует мод первоначальной версии [ElegooNeptuneThumbnailPrusaMod](https://github.com/fifonik/ElegooNeptuneThumbnailPrusaMod) от другого автора - в нем есть дополнительные возможности, но он также написан на Python, "весит" примерно столько же как оригинал, а работает даже медленнее оригинала (скорее всего из-за того, что добавляет к картинке дополнительный текст).  

## Синтакс использования

`elegoo_neptune_thumbnails_prusa_az [-h] [-p PRINTER] [-s SIZE] [-d] [-b] [-g] [-e PNGFILE] GCODE`

GCODE - это путь до gcode-файла. PrusaSlicer передает его скриптам постобработки автоматически.

### Опции:

<table>
    <tr>
        <th>Опция</th>
        <th>Альтернативная опция</th>
        <th>Описание</th>
    </tr>
    <tr>
        <td>-h</td>
        <td>--help</td>
        <td>Показать справку по использованию и выйти из программы</td>
    </tr>
    <tr>
        <td>-p</td>
        <td>--printer</td>
        <td>
            Указывает модель принтера PRINTER для которого генерируется превью.
            Если не указано, определяется из gcode-файла из значения printer_model.
            От него зависит в каком формате будет сгенерирован превью (для принтеров NEPTUNE2*|NEPTUNEX старая версия)
        </td>
    </tr>
    <tr>
        <td>-s</td>
        <td>--size</td>
        <td>
            PrusaSlicer может генерировать несколько превью разных размеров в одном файле. SIZE указывает какой размер оригинального превью брать для конвертации.
            По умолчанию берется размер 600x600. ElegooNeptuneThumbnails-Prusa также ищет только данный размер.

Но по факту принтер использует размер превью 160x160 для новых принтеров и 100x100 для старых (NEPTUNE2*|NEPTUNEX).
Поэтому нет необходимости генерировать превью в PrusaSlicer размером 600x600, можно сразу 200x200 или даже 160x160 (
100x100) (см. опцию -g). Это уменьшит размер gcode-файла

</td>
</tr>
<tr>
<td>-d</td>
<td>--delete-thumbnails </td>
<td>
По умолчанию оригинальные превью остаются в gcode-файле. Указав данную опицию их можно удалить, что уменьшит размер файла.  
</td>
</tr>
<tr>
<td>-b</td>
<td>--backup</td>
<td>
При указании этой опции перед изменением gcode-файла будет сделана его копия с расширением .bak  
</td>
</tr>
<tr>
<td>-g</td>
<td>--no-gimage</td>
<td>
По умолчанию генерируется 2 превью: gimage размером 200x200 и simage размером 160x160 (100x100 для NEPTUNE2*|NEPTUNEX) Но судя по <a href="https://github.com/NARUTOfzr/Elegoo-Neptune-marlin2.1.1/blob/main/Marlin/src/lcd/extui/dgus/elegoo/DGUSDisplayDef.cpp">исходникам прошивки</a>, реально используется только simage. Данная опция отключает генерирование gimage-превью, что уменьшит размер файла   
</td>
</tr>
<tr>
<td>-e</td>
<td>--extract</td>
<td>
При указании данной опции оригинальное превью будет сохранено в файл PNGFILE как png. Если в качестве PNGFILE указать "." (точку, без кавычек), имя файла будет взято из GCODE с изменением расширения на png.

</td>
</tr>
</table>

Учитывайте, что начиная с какой-то версии PrusaSlicer перед тем как записать gcode-файл в указанное вами место создает его вначале во временной папке и потом копирует его в нужное место. Скрипты пост-обработки получают путь именно до временного файла, соответсвенно, и bak-файл, и извлеченный png будут находиться во временной папке.

# Библиотеки
В данной разработке используются следующие библиотеки

- [AVIR](https://github.com/avaneev/avir) - AVIR image resizing algorithm designed by Aleksey Vaneev
- [libspng](https://github.com/randy408/libspng/) - library for reading and writing Portable Network Graphics (PNG) format files
- [base64](https://github.com/aklomp/base64) - Fast Base64 stream encoder/decoder
- [mbits-args](https://github.com/mbits-libs/args) - library for program argument parser



