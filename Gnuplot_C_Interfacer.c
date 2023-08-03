/*
A simple program that interfaces with the gnuplot terminal application to process data in C, then write it to a file.
To then plot the results out to the terminal into a specified terminal mode or plot format.
By just changing the plot file command, the program can be used to graph a file and it overwrites another


I have figured out, months later, how to output an actual PNG graph to help illustrate data in a more elegant and understandable fashion


Enter the following commmands:

In the Linux terminal:
1. gnuplot

In the Gnuplot terminal:

1. set output "Name_of_file.png" 
Example: "FibPlot.png"


2. set terminal pngcairo size Width, Length enhanced font "Font, Font_size"
Example: set terminal pngcairo size 800,600 enhanced font "Verdana, 18"

3. set xlabel "X_label_name"
Example: set xlabel "n"
4. set ylabel "Y_label_name"
Example: set ylabel "F(n)"

5. set grid

6. plot "Insert_name_of_file.fileType"
Example: plot "FibNumbers.txt"

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

        if (fibIterations == 0) // Take into account the two initial base case initial values for 0 and 1.
        {
            array[0] = 0;
            fibIterations++;
        }
        
        if (fibIterations == 1)
        {
            array[1] = 1;
            fibIterations++;
            fibSum = 1;
        }
        
        fibSum = fib1 + fib2;
        fib1 = fib2;
        fib2 = fibSum;
        array[fibIterations] = fibSum;

        fibIterations++;
    }
return fibSum;
    /*
    Fibonacci Sequence formula: Fn = Fsub(n-1) + Fsub(n-2)
     0,1,1,2,3,5,8,13,21,34,55,89,244......
    */
}

void graphDataFromText(char *nameOfFile, int *dataArray, int arraySize) // The pass in the address char array of the file name and then the array where to plot data from.
{
    /*  From what I have seen and experienced before, the newline character is included with the file name "string". 
        in order to remedy that, I use the sizeof() function to find how many actual characters or elements are in the array.
        Once found, I use that length value to replace the newline character with a null character. Without doing this, there is
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
    char lineStyle[MAX_ARRAY_SIZE] = "with linespoint"; // Here the line style is specified.

    strcat(singleQuote1, fileReceived[0]);
    strcat(singleQuote1, fileExtension);
    strcat(singleQuote1, singleQuote2);
    strcat(commandString, singleQuote1);
    strcat(commandString, lineStyle);
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

    fprintf(gnuplotPipe, "%s \n", "set terminal dumb"); // Here the terminal is set to a plotting mode.

    for (int i = 0; i < pointPairs; i++) 
    {
        fprintf(temp, "%d %d \n", i, dataArray[i]); //Write the data to a temporary file at every new line.
    }

    // Send commands to the interactive gnuplot command terminal.

    for (int i = 0; i < gnuplotCommands; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }
}

int main()
{
    int dataArray[MAX_ARRAY_SIZE];

    // int arrayLength = 10;
    int arrayLength;
    char name[MAX_ARRAY_SIZE]; // Set the maximum file name to 80 characters.

    printf("Enter name of file to plot Fibonacci numbers: ");
    fgets(name, MAX_ARRAY_SIZE, stdin); // Use fgets() function to store strings.



    printf("Enter number of fibonacci iterations: ");
    //fgets(arrayLength, MAX_ARRAY_SIZE, stdin);
    scanf("%d", &arrayLength);
    fibonacciData(dataArray, arrayLength); // Populate with fib values.

    graphDataFromText(name, dataArray, arrayLength); // Pass in the address of the array to the file name string, or array of characters.

return 0;
}