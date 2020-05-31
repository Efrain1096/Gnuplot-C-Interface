

/*
A simple program that interfaces with the gnuplot terminal application to process data in C, then write it to a file.
To then plot the results out to the terminal into a specified terminal mode or plot format.

By just changing the plot file command, the program can be used to graph a file and it overwrites another

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define MAX_STRING_SIZE 80

void graphDataFromText(char *nameOfFile) // The pass in the char array of the file name.
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


    char commandString[MAX_STRING_SIZE] = "plot ";
    char singleQuote1[MAX_STRING_SIZE] = "'";
    //char fileName[MAX_STRING_SIZE] = fileReceived[0]; // This don't work SONNY JIM!!!
    char singleQuote2[MAX_STRING_SIZE] = "'";
    char fileExtension[MAX_STRING_SIZE] = ".txt";

    strcat(singleQuote1, fileReceived[0]);
    strcat(singleQuote1, fileExtension);
    strcat(singleQuote1, singleQuote2);
    strcat(commandString, singleQuote1);




    /*char doubleQuote1[MAX_STRING_SIZE] = "\"";
    char doubleQuote2[MAX_STRING_SIZE] = "\"";

    strcat(doubleQuote1, fileReceived[0]);
    strcat(doubleQuote1, fileExtension);
    strcat(doubleQuote1, doubleQuote2);*/

    //char *commandsForGnuplot[] = {"set title "\"Title Example\"", "plot 'testData.txt'"}; is the format
    char *commandsForGnuplot[] = {setTitleCommand1, commandString};

    int pointPairs = 10;
    int gnuplotCommands = 2;
    double xVals[pointPairs];
    double yVals[pointPairs];

    // Fill the x and y arrays with dummy data to test array

    for (int i = 0; i < pointPairs; i++)
    {
        xVals[i] = i;
    }

    for (int j = 0; j < pointPairs; j++)
    {
        yVals[j] = j;
    }
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
        fprintf(temp, "%lf %lf \n", xVals[i], yVals[i]); //Write the data to a temporary file
    }

    // Send commands to the interactive gnuplot command terminal

    for (int i = 0; i < gnuplotCommands; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }
}

int main()
{

    char name[MAX_STRING_SIZE];

    printf("Enter your name: ");
    fgets(name, MAX_STRING_SIZE, stdin); // Use fgets() function to store strings.

    graphDataFromText(name); // Pass in the address of the array to the file name string, or array of characters.

    return 0;
}
