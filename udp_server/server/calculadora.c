#include <stdio.h>

float calculadora(char operador, float arg1, float arg2)
{
    float resultado = 0;
    switch (operador) {
        case '+':
            resultado = arg1 + arg2;
            break;
        case '-':
            resultado = arg1 - arg2;
            break;
        case '.':
            resultado = arg1 * arg2;
            break;
        case '/':
            if (arg2 == 0) {
                printf("Não é possivel realizar divisão por zero!\n");
                resultado = 0;
                break;
            }
            resultado = arg1 / arg2;
            break;
        default:
            printf("Entrada Inválida, tente novamente...\n");
            break;
    }
    return resultado;
}