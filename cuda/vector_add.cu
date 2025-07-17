#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

// Kernel-функция, которая будет выполняться на GPU
__global__ void vectorAdd(float *a, float *b, float *c, int n) {
    // Получаем глобальный индекс потока
    int index = blockIdx.x * blockDim.x + threadIdx.x;

    // Проверяем, чтобы не выйти за пределы массива
    // (важно, если размер массива не кратен числу потоков в блоке)
    if (index < n) {
        c[index] = a[index] + b[index];
    }
}

int main() {
    int n = 1000000; // Размер векторов
    size_t size = n * sizeof(float);

    // 1. Выделяем память на хосте (CPU)
    float *h_a = (float *)malloc(size);
    float *h_b = (float *)malloc(size);
    float *h_c = (float *)malloc(size);

    // Инициализируем векторы на хосте
    for (int i = 0; i < n; i++) {
        h_a[i] = (float)i;
        h_b[i] = (float)i * 2.0f;
    }

    // 2. Выделяем память на устройстве (GPU)
    float *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, size);
    cudaMalloc(&d_b, size);
    cudaMalloc(&d_c, size);

    // 3. Копируем данные с хоста на устройство
    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    // 4. Запускаем Kernel на GPU
    int threadsPerBlock = 256;
    // Рассчитываем количество блоков, чтобы покрыть все n элементов
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    // <<<...>>> - это синтаксис для вызова Kernel
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, n);

    // 5. Копируем результат с устройства на хост
    cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);

    // 6. Проверяем результат (опционально, но рекомендуется)
    for (int i = 0; i < 10; i++) {
        if (h_c[i] != (h_a[i] + h_b[i])) {
            printf("Ошибка на индексе %d!\n", i);
            break;
        }
    }
    printf("Вычисления на GPU завершены успешно.\n");
    printf("Пример результата: h_c[100] = %f\n", h_c[100]);

    // 7. Освобождаем память
    free(h_a);
    free(h_b);
    free(h_c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}