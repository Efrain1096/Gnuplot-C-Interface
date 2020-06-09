/*
A simple program that interfaces with the gnuplot terminal application to process data in C, then write it to a file.
To then plot the results out to the terminal into a specified terminal mode or plot format.

By just changing the plot file command, the program can be used to graph a file and it overwrites another

*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include<time.h>

#define MAX_ARRAY_SIZE 80



// Pass by reference
int fibonacciData (int *array, int n) // Test function to write to arrays and then write the values stored in the arrays to the data file.
{
    int fibIterations = 0;
    int fibSum = 0;
    int fib1 = 0;
    int fib2 = 1;

    while(fibIterations < n)
    {
        fibSum = fib1 + fib2;
        fib1 = fib2;
        fib2 = fibSum;
        array[fibIterations] = fibSum;

        fibIterations++;
    }

return fibSum;
    /*Fibonacci Sequence formula: Fn = Fsub(n-1) + Fsub(n-2)
     0,1,1,2,3,5,8,13,21,34,55,89,244......
    
    
    */
}

void graphDataFromText(char *nameOfFile, int *dataArray, int arraySize) // The pass in the address char array of the file name and then the array where to plot data from.
{

    /*  From what I have seen and experienced before, the newline character is included with the file name "string". 
        in order to remefy that, I use the sizeof() function to find how many actual characters or elements are in the array.
        Once found, I use that length value to replace the newline character with a null character. Without doinf this, there is
        a newline included at the end of the name, causing problems and frustration. The name char array is read as two separate commands
        or parameters by the Gnuplot terminal, which is not fun in the slightest!
    */

    nameOfFile[strlen(nameOfFile)-1] = '\0'; // As mentioned before, replace the newline character with a null character.
    char *fileReceived[] = {nameOfFile}; // Set an array that holds pointers to strings.
                                         // Namely hold the name of the file to write to and graph from.
    char setTitleCommand1[] = "set title";

    char graphTitle1[] = "\"Example Title\""; // This variable will store the label name of the graph that is prompted to the user.
                                              // It also needs literal double quotes. Otherwise, it's recognized as a token.

    strcat(setTitleCommand1, graphTitle1);

    /*  What is going on below with the strcat() functions is tat I am essentially "gluing" together the proper commands
        to run in the Gnuplot terminal at runtime. Concatenating the proper characters together to form the command string.
    */

    char commandString[MAX_ARRAY_SIZE] = "plot ";
    char singleQuote1[MAX_ARRAY_SIZE] = "'";
    char singleQuote2[MAX_ARRAY_SIZE] = "'";
    char fileExtension[MAX_ARRAY_SIZE] = ".txt";

    strcat(singleQuote1, fileReceived[0]);
    strcat(singleQuote1, fileExtension);
    strcat(singleQuote1, singleQuote2);
    strcat(commandString, singleQuote1);

    /*char doubleQuote1[MAX_ARRAY_SIZE] = "\"";
    char doubleQuote2[MAX_ARRAY_SIZE] = "\"";

    strcat(doubleQuote1, fileReceived[0]);
    strcat(doubleQuote1, fileExtension);
    strcat(doubleQuote1, doubleQuote2);*/

    //char *commandsForGnuplot[] = {"set title "\"Title Example\"", "plot 'testData.txt'"}; is the format
    char *commandsForGnuplot[] = {setTitleCommand1, commandString};

    int pointPairs = arraySize;
    int gnuplotCommands = 2;
    double xVals[pointPairs];
    double yVals[pointPairs];

    // Fill the x and y arrays with dummy data to test array

    /*
    for (int i = 0; i < pointPairs; i++)
    {
        xVals[i] = i;
    }

    for (int j = 0; j < pointPairs; j++)
    {
        yVals[j] = j;
    }
*/

    strcat(fileReceived[0], fileExtension);
    FILE *temp = fopen(fileReceived[0], "w");

    /* The popen() function opens a process by creating a pipe, forking, and invoking the shell. Basically calls and runs the gnuplot 
        command as an interface to a C program that processes or reads in data to then write it to a file and finally graph it
        out using the gnuplot's plot() function.  
     */

    FILE *gnuplotPipe = popen("gnuplot", "w");

    fprintf(gnuplotPipe, "%s \n", "set terminal dumb"); // Here tthe terminal is set to a plotting mode.

    for (int i = 0; i < pointPairs; i++) 
    {
        fprintf(temp, "%d %d \n", i, dataArray[i]); //Write the data to a temporary file at every new line.
    }

    // Send commands to the interactive gnuplot command terminal

    for (int i = 0; i < gnuplotCommands; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }
}

int main()
{
    int dataArray[MAX_ARRAY_SIZE];

    int arrayLength = 10;
    
    fibonacciData(dataArray, arrayLength); // Populate with fib values.
    char name[MAX_ARRAY_SIZE];

    printf("Enter your name: ");
    fgets(name, MAX_ARRAY_SIZE, stdin); // Use fgets() function to store strings.

    graphDataFromText(name, dataArray, arrayLength); // Pass in the address of the array to the file name string, or array of characters.

return 0;
}
