
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define PROGRAM_SIZE 16

typedef struct {
    int ip;
    int is;
    int r0;
    int r1;
    int sw;
    int error;
    void (*output_func)(int);
    int memory[PROGRAM_SIZE];
} cpu_t;

cpu_t * cpu_init( int program[PROGRAM_SIZE] ) {
    int i;
    cpu_t * result = (cpu_t *)calloc(sizeof(cpu_t),1);
    for (i=0;i<PROGRAM_SIZE;i++)
        result->memory[i] = program[i]&(PROGRAM_SIZE-1);
    return result;
}

void cpu_run( cpu_t * context ) {
    int ticks = 0;
    int running = 1;
    while ( ticks++, running ) {
        context->is = context->memory[context->ip];
        context->ip = (context->ip + 1)&(PROGRAM_SIZE-1);
        switch ( context->is ) {
            case 0:
                running = 0;
                break;
            case 1:
                context->r0 = (context->r0 + 1)&(PROGRAM_SIZE-1);
                break;
            case 2:
                context->r0 = (context->r0 + (PROGRAM_SIZE-1))&(PROGRAM_SIZE-1);
                break;
            case 3:
                context->r1 = (context->r1 + 1)&(PROGRAM_SIZE-1);
                break;
            case 4:
                context->r1 = (context->r1 + (PROGRAM_SIZE-1))&(PROGRAM_SIZE-1);
                break;
            case 5:
                context->r0 = (context->r0 + context->r1)&(PROGRAM_SIZE-1);
                break;
            case 6:
                context->r0 = ((PROGRAM_SIZE) + context->r0 - context->r1)&(PROGRAM_SIZE-1);
                break;
            case 7:
                if ( NULL != context->output_func )
                    context->output_func(context->r0);
                break;
            case 8:
                context->is = context->memory[context->ip];
                if ( context->r0 != 0 )
                    context->ip = context->is;
                else
                    context->ip = (context->ip + 1) & (PROGRAM_SIZE-1);
                break;
            case 9:
                context->is = context->memory[context->ip];
                if (context->r0 == 0)
                    context->ip = context->is;
                else
                    context->ip = (context->ip + 1)&(PROGRAM_SIZE-1);
                break;
            case 10:
                context->is = context->memory[context->ip];
                context->r0 = context->is;
                context->ip = (context->ip + 1)&(PROGRAM_SIZE-1);
                break;
            case 11:
                context->is = context->memory[context->ip];
                context->r1 = context->is;
                context->ip = (context->ip + 1)&(PROGRAM_SIZE-1);
                break;
            case 12:
                context->is = context->memory[context->ip];
                context->memory[context->is] = context->r0;
                context->ip = (context->ip + 1)&(PROGRAM_SIZE-1);
                break;
            case 13:
                context->is = context->memory[context->ip];
                context->memory[context->is] = context->r1;
                context->ip = (context->ip + 1)&(PROGRAM_SIZE-1);
                break;
            case 14:
                context->is = context->memory[context->ip];
                context->sw = context->memory[context->is];
                context->memory[context->is] = context->r0;
                context->r0 = context->sw;
                context->ip = (context->ip + 1)&(PROGRAM_SIZE-1);
                break;
            case 15:
                context->is = context->memory[context->ip];
                context->sw = context->memory[context->is];
                context->memory[context->is] = context->r1;
                context->r1 = context->sw;
                context->ip = (context->ip + 1)&(PROGRAM_SIZE-1);
                break;
        }
    }
}

void cpu_destory( cpu_t * ptr ) {
    free(ptr);
}

int main(int argc, char ** argv) {
    int program[16] = {6,3,15,15,7,3,7,9,3,8,4,7,5,8,0,0};
    cpu_t * c4004 = cpu_init(program);
    c4004->output_func = &count_it_callback;
    cpu_run(c4004);
    return 0;
}
