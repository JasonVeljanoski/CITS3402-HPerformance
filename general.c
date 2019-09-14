#include "sparse_matrices.h"

//  REMOVE ANY TRAILING end-of-line CHARACTERS FROM THE LINE
void trim_line(char line[])
{
    int i = 0;
    //  LOOP UNTIL WE REACH THE END OF line
    while (line[i] != '\0')
    {
        //  CHECK FOR CARRIAGE-RETURN OR NEWLINE
        if (line[i] == '\r' || line[i] == '\n')
        {
            line[i] = '\0'; // overwrite with null-byte
            break;          // leave the loop early
        }
        i = i + 1; // iterate through character array
    }
}
