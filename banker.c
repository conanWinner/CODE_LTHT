#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

// Hàm kiểm tra trạng thái an toàn
bool isSafe(int processes[], int available[], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], int n, int m, FILE *outputFile)
{
    int work[MAX_RESOURCES];              // Tạo bản sao của available
    bool finish[MAX_PROCESSES] = {false}; // Trạng thái của các tiến trình
    int safeSeq[MAX_PROCESSES];           // Lưu chuỗi an toàn
    int count = 0;                        // Số tiến trình đã hoàn thành

    // Bước 1: Khởi tạo work với available
    for (int i = 0; i < m; i++)
    {
        work[i] = available[i];
    }

    for (int l = 0; l < m; l++)
    {
        printf("%d ", work[l]);
    }

    printf("%s\n", "");

    // Bước 2: Tìm tiến trình có thể hoàn thành
    while (count < n)
    {
        bool found = false;
        for (int p = 0; p < n; p++)
        {
            // Nếu tiến trình chưa hoàn thành và có thể yêu cầu tài nguyên
            if (!finish[p])
            {
                int j;
                for (j = 0; j < m; j++)
                {
                    if (need[p][j] > work[j])
                    {
                        break;
                    }
                }

                // Nếu tiến trình p có thể hoàn thành
                if (j == m)
                {
                    for (int k = 0; k < m; k++)
                    {
                        work[k] += allocation[p][k]; // Giải phóng tài nguyên
                    }

                    // In tài nguyên đang có sẵn
                    for (int l = 0; l < m; l++)
                    {
                        printf("%d ", work[l]);
                    }

                    printf("%s\n", "");

                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }

        // Nếu không tìm thấy tiến trình nào có thể hoàn thành
        if (!found)
        {
            fprintf(outputFile, "The system is not safe.\n");
            printf("The system is not safe.\n");
            return false;
        }
    }

    // In chuỗi an toàn
    fprintf(outputFile, "The system is safe. The safe process sequence is: ");
    printf("The system is safe. The safe process sequence is: ");
    for (int i = 0; i < n; i++)
    {
        fprintf(outputFile, "%d ", safeSeq[i]);
        printf("%d ", safeSeq[i]);
    }
    fprintf(outputFile, "\n");
    printf("\n");
    return true;
}

// Hàm tạo tên file output dựa trên thời gian hiện tại
void createOutputFilename(char *filename)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    // Tạo tên file với định dạng ddMMyyHHmmss.txt
    strftime(filename, 20, "%d%m%y%H%M%S.txt", tm_info);
}

// Hàm nhập dữ liệu từ file
int inputData(const char *filename, int *n, int *m, int allocation[MAX_PROCESSES][MAX_RESOURCES], int max[MAX_PROCESSES][MAX_RESOURCES], int available[MAX_RESOURCES])
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Cannot open the file");
        return 0;
    }

    fscanf(file, "%d", n);
    fscanf(file, "%d", m);

    // Nhập ma trận allocation
    for (int i = 0; i < *n; i++)
    {
        for (int j = 0; j < *m; j++)
        {
            fscanf(file, "%d", &allocation[i][j]);
        }
    }

    // Nhập ma trận max
    for (int i = 0; i < *n; i++)
    {
        for (int j = 0; j < *m; j++)
        {
            fscanf(file, "%d", &max[i][j]);
        }
    }

    // Nhập vector available
    for (int i = 0; i < *m; i++)
    {
        fscanf(file, "%d", &available[i]);
    }

    fclose(file);
    return 1;
}

int main(int argc, char *argv[])
{
    int n, m;
    int allocation[MAX_PROCESSES][MAX_RESOURCES], max[MAX_PROCESSES][MAX_RESOURCES], available[MAX_RESOURCES];
    int processes[MAX_PROCESSES];

    char path_input[100] = "./banker_input/";

    if (argc == 1)
    {
        printf("%s", "You need to pass in the input file!\nUsage: ./banker <file_name>\n");
        return 0;
    }

    strcat(path_input, argv[1]);
    int check_open_file = inputData(path_input, &n, &m, allocation, max, available);

    if (check_open_file == 0)
    {
        return 0;
    }

    // Tính toán ma trận need
    int need[MAX_PROCESSES][MAX_RESOURCES];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    char outputFilename[20];
    createOutputFilename(outputFilename);
    char path_output[100] = "./banker_output/";
    strcat(path_output, outputFilename);

    // Mở file output
    FILE *outputFile = fopen(path_output, "w");
    if (outputFile == NULL)
    {
        printf("Cannot create the file %s\n", path_output);
        return 1;
    }

    // Kiểm tra trạng thái an toàn
    isSafe(processes, available, max, allocation, need, n, m, outputFile);

    fclose(outputFile);
    return 0;
}
