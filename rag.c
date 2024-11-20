#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int graph[MAX_PROCESSES + MAX_RESOURCES][MAX_PROCESSES + MAX_RESOURCES]; // Ma trận biểu diễn đồ thị
int numProcesses, numResources;

bool visited[MAX_PROCESSES + MAX_RESOURCES];
bool recStack[MAX_PROCESSES + MAX_RESOURCES];

// Hàm kiểm tra chu kỳ (cycle detection) trong đồ thị
bool isCyclic(int node)
{
    if (!visited[node])
    {
        visited[node] = true;
        recStack[node] = true;

        // Duyệt qua các đỉnh kề của đỉnh node
        for (int i = 0; i < numProcesses + numResources; i++)
        {
            if (graph[node][i])
            {
                if (!visited[i] && isCyclic(i))
                {
                    return true;
                }
                else if (recStack[i])
                {
                    return true;
                }
            }
        }
    }
    recStack[node] = false;
    return false;
}

// Hàm kiểm tra deadlock
bool checkDeadlock()
{
    for (int i = 0; i < numProcesses + numResources; i++)
    {
        visited[i] = false;
        recStack[i] = false;
    }

    // Duyệt qua tất cả các đỉnh để phát hiện chu kỳ
    for (int i = 0; i < numProcesses + numResources; i++)
    {
        if (isCyclic(i))
        {
            return true;
        }
    }
    return false;
}

// Hàm nhập dữ liệu đồ thị từ file
int inputGraph(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Cannot open the file");
        return 0;
    }

    fscanf(file, "%d", &numProcesses);
    fscanf(file, "%d", &numResources);

    for (int i = 0; i < numProcesses + numResources; i++)
    {
        for (int j = 0; j < numProcesses + numResources; j++)
        {
            fscanf(file, "%d", &graph[i][j]);
        }
    }

    fclose(file);
    return 1;
}

// Hàm tạo tên file output dựa trên thời gian hiện tại
void createOutputFilename(char *filename)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    // Tạo tên file với định dạng ddmmyyhhmm.txt
    strftime(filename, 20, "%d%m%y%H%M%S.txt", tm_info);
}

// Hàm ghi kết quả vào file
void writeOutput(const char *filename, bool hasDeadlock)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Cannot create the file %s\n", filename);
        return;
    }

    if (hasDeadlock)
    {
        fprintf(file, "Deadlock detected in the system!\n");
        printf("Deadlock detected in the system!\n");
    }
    else
    {
        fprintf(file, "No deadlock in the system.\n");
        printf("No deadlock in the system.\n");
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    char path_input[100] = "./rag_input/";
    strcat(path_input, argv[1]);
    int check_open_file = inputGraph(path_input);
    if (check_open_file == 0)
    {
        return 0;
    }

    bool hasDeadlock = checkDeadlock();

    char outputFilename[20];
    createOutputFilename(outputFilename);
    char path_output[100] = "./rag_output/";
    strcat(path_output, outputFilename);
    writeOutput(path_output, hasDeadlock);

    return 0;
}
