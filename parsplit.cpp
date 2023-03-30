/*
PRL projekt 1 - PARALLEL SPLITTING
autor: Žaneta Grossová (xgross11)
*/

#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const string fileName = "numbers";

bool getNumbers(vector<int> *numbers)
{
    int number;                      
    fstream fin;
        
    fin.open("numbers", ios::in); 
        
    if (!fin.is_open())
    {
        cerr << "Soubor numbers se nepodarilo otevrit" << endl;
        return false;
    }
    
    while(fin.good()){
        number = fin.get();
        if(!fin.good()) { //EOF
            break;
        }
        numbers->push_back(number);
    }
    
    fin.close();
    return true; 
}




int main(int argc, char *argv[])
{

    int rank, size, num_size;
    vector<int> numbers;
    bool err;
    int middle[1];    

    MPI_Init( &argc, &argv);                           
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );             
    MPI_Comm_size( MPI_COMM_WORLD, &size );                                          

    if (rank == 0){ // 0 root proces

        err = getNumbers(&numbers); // returns vector n with numbers

        if(err) {

            num_size = numbers.size();
            if(num_size % 2 == 0) { //sudy pocet
                middle[0] = numbers[num_size/2 - 1 ]; // -1 protoze pole zacina na 0
            } else {                //lichy pocet
                middle[0] = numbers[(num_size / 2) + 1 - 1]; //-1 protoze pole zacina na 0
            }

        }

        else {
            MPI_Finalize( ); 
            return -1;
        }
    } 

    // root broadcast middle number and size of numbers to slaves
    MPI_Bcast( &middle, 1, MPI_INT, 0, MPI_COMM_WORLD );
    MPI_Bcast( &num_size, 1, MPI_INT, 0, MPI_COMM_WORLD );

    MPI_Barrier( MPI_COMM_WORLD );

    int recvbuf[num_size/size];

    // vector to array
    int numbers_arr[num_size];
    if (rank == 0){     //root
        for (int i = 0; i < num_size; i++) {
            numbers_arr[i] = numbers[i];
            printf("%i\n", numbers[i]);
        }
    }

    MPI_Scatter(numbers_arr,
                num_size/size,
                MPI_INT,
                &recvbuf,
                num_size/size,
                MPI_INT,
                0,
                MPI_COMM_WORLD);

    //paralel splitting algorithm
    /*int L, E, G [num_size/size];

    for (int s : recvbuf) {
        if( s < middle[1] ) {
            L.back() = s;
        } else if ( s = middle[1] ) {
            E.back() = s;
        } else if ( s > middle[1] ) {
            G.back() = s;
        }
        
    }*/

    //printf ("rank: %i, value: %i\n", rank, middle[0]);

    /*int buffer[4] = {0, 100, 200, 300};
    MPI_Scatter(buffer, 1, MPI_INT, &my_value, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received value = %d.\n", rank, my_value);*/

    //MPI_Gather(&middle, 1, MPI_INT, middle, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // vector to array
    /*int numbers_arr[num_size];

    if (rank == 0){
        for (int i = 0; i < num_size; i++) {
            numbers_arr[i] = numbers[i];
            printf("%i\n", numbers[i]);
        }
    }

    int recvbuf[num_size];

    MPI_Scatter(numbers_arr, num_size / size, MPI_INT, &recvbuf, num_size / size, MPI_INT, 0, MPI_COMM_WORLD);*/

    /*for (int i : recvbuf) {
        printf ("rank: %i, value: %i\n", rank, i);
    }*/

    MPI_Finalize( );                                    
    return 0;  
        
}
