#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

typedef struct People
{

    char *name;
   

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
    char **strings = malloc(sizeof(char *) * 5); // Максимальное количество полей в строке
    char *token;
    int i = 0;

    token = strtok(full_line, ",");
    while (token != NULL)
    {
        strings[i] = token;
        token = strtok(NULL, ",");
        i++;
    }
    
    return strings;
}

people *load_to_struct(char *full_line)
{
    people *people1 = malloc(sizeof(people));
    char **data = split_the_line(full_line);
    people1->name = calloc(strlen(data[2]) + 1, sizeof(char)); // Увеличиваем размер на 1 для нулевого символа
    strcpy(people1->name, data[3]);

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



void swap(people **arr, int i, int j)
{
    people *temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void bubble_sort(people **arr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (strcoll(arr[j]->name, arr[j + 1]->name) > 0)
            {
                swap(arr, j, j + 1);
            }
        }
    }
}





int main()
{
    char *file_name = "people.csv";
    people **people_array = load_txt_data(file_name);

    // Sort the array of people by name
    int array_size = 0;
    while (people_array[array_size] != NULL)
    {
        array_size++;
    }
    bubble_sort(people_array, array_size);

    // Printing loaded data after sorting
    for (int i = 0; people_array[i] != NULL; i++)
    {
        printf("Name: %s\n", people_array[i]->name);
    }

    // Free allocated memory
    for (int i = 0; people_array[i] != NULL; i++)
    {
        free(people_array[i]->name);
        free(people_array[i]);
    }
    free(people_array);

    return 0;
}
