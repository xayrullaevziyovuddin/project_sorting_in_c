#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

typedef struct People
{
    char *state;
    char gender;
    int year;
    char *name;
    int number;

} people;

char *read_line(int fd)
{
    char *full_line = calloc(128, sizeof(char)); // Увеличиваем размер начального буфера
    char c = 0;
    int n;
    int i = 0;
    
    while ((n = read(fd, &c, 1)) > 0) // Проверяем успешность чтения
    {
        if (c == '\n')
        {
            full_line[i] = '\0';
            break;
        }
        full_line[i] = c;
        i++;

        if (i % 128 == 0) // Увеличиваем буфер при необходимости
        {
            full_line = realloc(full_line, (i + 128) * sizeof(char));
        }
    }

    return full_line;
}

char **split_the_line(char *full_line)
{
    int length = 1;
    char **strings = malloc(sizeof(char *));
    int len = strlen(full_line);
    int i = 0;
    int start = 0;

    for (i = 0; i < len; i++)
    {
        if (full_line[i] == ',')
        {
            full_line[i] = '\0';
            length++;
            strings = realloc(strings, length * sizeof(char *));
            strings[length - 1] = &full_line[start];
            start = i + 1;
        }
    }
    strings = realloc(strings, (length + 1) * sizeof(char *)); // Добавляем место для последнего элемента
    strings[length] = &full_line[start]; // Добавляем последний элемент

    return strings;
}

people *load_to_struct(char *full_line)
{
    people *people1 = malloc(sizeof(people));
    char **data = split_the_line(full_line);
    people1->name = calloc(strlen(data[3]) + 1, sizeof(char)); // Увеличиваем размер на 1 для нулевого символа
    strcpy(people1->name, data[4]);

    free(full_line);
    return people1;
}

people **load_txt_data(char *file_name)
{
    int fd = open(file_name, O_RDONLY); // Используем переданный параметр
    if (fd == -1) // Проверяем успешность открытия файла
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char *full_line;
    people **people_array = malloc(sizeof(people *));
    int count = 0;

    while (1)
    {
        full_line = read_line(fd);
        if (strlen(full_line) == 0)
        {
            free(full_line); // Освобождаем память для пустой строки
            break;
        }
        people *student_1 = load_to_struct(full_line);
        people_array = realloc(people_array, (count + 2) * sizeof(people *));
        people_array[count++] = student_1;
    }
    people_array[count] = NULL; // Добавляем завершающий NULL

    close(fd);
    return people_array;
}

int main()
{
    char *file_name = "people.csv";
    people **people_array = load_txt_data(file_name);

    // Example: printing loaded data
    for (int i = 0; people_array[i] != NULL; i++)
    {
        printf("Name: %s\n", people_array[i]->name);
        // Print other fields as needed
        free(people_array[i]->name); // Освобождаем память для имени
        free(people_array[i]); // Освобождаем память для структуры
    }
    free(people_array); // Освобождаем память для массива указателей

    return 0;
}
