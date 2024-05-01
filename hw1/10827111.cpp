# include <iostream>
# include <vector>
# include <fstream>
# include <unistd.h>
#include <time.h>
# include <pthread.h>
# include <thread>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ipc.h>
# include <sys/shm.h>
#include <chrono>

using namespace std;

void Readfile( vector<int> & data, string & filename );
void Writefile( int * arr, int SizeOfData, string filename, int sel, auto CPUtime );
void DetailRequire( int & sel, int & segmentation, int size );

void Bubble( int * arr, int SizeOfData, int st );
void BubbleMerge( int * arr, int SizeOfData, int segmentation );
void Merge( int * arr, int left, int mid, int right );
void updateRemain( int & remain, int * sortRange );

void MultiProcessBubbleMerge( int * arr, int SizeOfData, int segmentation  );
void MultiThreadBubbleMerge( int * arr, int SizeOfData, int segmentation );
void *Bubble( void *Struct );
void *Merge( void *Struct );

struct ThreadBubbleSt {
  int *arr;
  int st;
  int ed;
};

struct ThreadMergeSt {
  int *arr;
  int left;
  int mid;
  int right;
};

int main()
{
    string filename;
    int sel, segmentation;
    bool quit = false;
    vector<int> data;
    do {
        //clock_t time1, time2;
        Readfile( data, filename );
        int SizeOfData = data.size();
        int * arr = new int[ SizeOfData ];
        copy ( data.begin(), data.end(), arr );
        DetailRequire( sel, segmentation, SizeOfData );
        auto stTime = chrono::steady_clock::now();
        switch ( sel ) {
            case 1 :
                if ( SizeOfData == 0 ) break;
                Bubble( arr, SizeOfData, 0 );
                break;
            case 2 :
                if ( SizeOfData == 0 ) break;
                BubbleMerge( arr, SizeOfData, segmentation );
                break;
            case 3 :
                if ( SizeOfData == 0 ) break;
                MultiProcessBubbleMerge( arr, SizeOfData, segmentation );
                break;
            case 4 :
                if ( SizeOfData == 0 ) break;
                MultiThreadBubbleMerge( arr, SizeOfData, segmentation );
                break;
            default :
                quit = true;
                break;
        }

        auto edTime = chrono::steady_clock::now();
        auto CPUtime = chrono::duration_cast<chrono::milliseconds>(edTime - stTime);
        cout <<  "CPU time : " <<  CPUtime.count() << endl;
        if ( sel == 1 || sel == 2 || sel == 3 || sel == 4 ) Writefile( arr, SizeOfData, filename, sel, CPUtime );
        delete[] arr;
        data.clear();


    } while ( !quit );
    return 0;
}


void Readfile( vector<int> & data, string & filename ) {
    string str;
    cout << "input file name ( ex:input_1w ) :" << endl ;
    cin >> filename;
    str = filename + ".txt";
    fstream file;
    file.open( str.c_str() );
    while ( !file.is_open() ) {
        cout << "input right file name (no .txt):" << endl;
        cin >> filename;
        str = filename + ".txt";
        file.open( str.c_str() );
    }



    while ( !file.eof() ) {
        int t ;
        file >> t;
        data.push_back(t);
    }

    file.close();
    data.pop_back();

    /*
    fstream re;
    string rebuild = "REBUILD" + filename;
    re.open( rebuild.c_str(), ios::out );
    for ( int a = 0 ; a < data.size() ; a++ ){
        re <<  data[a] << endl;
    }
    */
}

void Writefile( int * arr, int SizeOfData, string filename, int sel, auto CPUtime ) {

    if ( sel == 1 ) filename = filename + "_output1.txt";
    else if ( sel == 2 ) filename = filename + "_output2.txt";
    else if ( sel == 3 ) filename = filename + "_output3.txt";
    else if ( sel == 4 ) filename = filename + "_output4.txt";

    fstream out;
    out.open( filename.c_str(), ios :: out );
    out <<  "Sort : " << endl;
    for ( int a = 0 ; a < SizeOfData ; a++ ){
        out <<  arr[a] << endl;
    }

    out <<  "CPU time : " <<  CPUtime.count() << endl;

    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep));
    out <<  "OutPut time : " <<  tmp  << "+08:00" << endl;
    out.close();
}


void Bubble( int * arr, int ed, int st ) {
    for ( int a = 0 ; a < ed-st-1 ; a++ ){ //999  // 2000-1000-1 = 999

        for ( int b = st ; b < ed-1-a ; b++) { //999  // 2000-1-0=1999
            if ( arr[b] >= arr[b+1] ) {
                int temp = arr[b+1];
                arr[b+1] = arr[b];
                arr[b] = temp;
            }
        }

    }

}


void BubbleMerge( int * arr, int SizeOfData, int segmentation ) {

    int i = SizeOfData / segmentation;
    int pre = 0, pos = i;
    int * sortRange = new int[segmentation+1];
    for ( int a = 0 ; a < segmentation ; a++ ) { // 0(1) 2(3) 4(5) 6(7) 8(9) // 0(1) 2(3)
        if ( a + 1 == segmentation ) {
            sortRange[a] = pre;
            sortRange[a+1] = SizeOfData;
            //cout << sizeof(sortRange) << endl;
        }
        else {
            sortRange[a] = pre;
            sortRange[a+1] = pos;
            pre = pos ;
            pos = pre + i ;
        }
        Bubble( arr, sortRange[a+1], sortRange[a] );
    }

    // for ( int a = 0 ; a < segmentation+1 ; a++ ) cout << sortRange[a] << endl;

     //cout << needMerge.size() << endl;

    int remain = segmentation;

    while ( remain > 1 ) {
        for ( int a = 0 ; a < remain - 1 ; a+=2 ) { // 0k 1k 2k 3k 4k 5k 6k 7k 8k 9k 1w // 0 2k 4k 6k 8k 1w  a<4 0 2
            cout << "now merge : " << sortRange[a] << " "<< sortRange[a+1] << " " << sortRange[a+2] << endl;
            Merge( arr, sortRange[a], sortRange[a+1], sortRange[a+2] );
        }

        //cout << "finish : " << remain << endl;
        updateRemain( remain, sortRange );
    }

    delete[] sortRange;

}


void Merge( int * arr, int left, int mid, int right ) { // 10 5 3 2 1
    int subSize1 = mid - left;
    int subSize2 = right - mid;
    int *sub1 = new int[ subSize1 ];
    int *sub2 = new int[ subSize2 ];
    copy( arr + left, arr + mid, sub1 );
    copy( arr + mid, arr + right, sub2 );
		//string test = "testans.txt";
		//fstream out;
		//out.open(test.c_str(), ios::out);
		//for ( int a = 0 ; a < temp.size() ; a++ ) cout << temp[a]<<endl;
    int k = left ;
    int i = 0, j = 0;
    while (  k < right && i < subSize1 && j < subSize2  ){
        if ( sub1[i] <= sub2[j] ) {
            arr[k] = sub1[i];
            i++;
        }
        else {
            arr[k] = sub2[j];
            j++;
        }

        k++;
    }

    while( i < subSize1 ) {
        arr[k] = sub1[i];
        i++;
        k++;
    }

    while( j < subSize2 ){
        arr[k] = sub2[j];
        j++;
        k++;
    }

					//cout << temp.size();
    delete[] sub1;
    delete[] sub2;
}

/*
void updateRemain( int & remain, int * sortRange ) { // 10份要變成5份  5份要變成3份 0 1 2 3 4 5 6 7 8 9 10
    int newSize = remain/2 + remain%2 ; // 10>>5   5>>3
    for ( int a = 0 ; a < newSize ; a++ ) {
        // remain=5 a<6 // a=0 b=0 // a=1 b=2 // a=2 b=4 // a=3 b=6 // a=4 b=8 // a=5 b=10 ////// 0 2k 4k 6k 8k 1w
        // remain=3 a<4 // a=0 b=0 // a=1 b=2 // a=2 b=4 // a=3 b=6 ////// 0 4k 8k

        sortRange[a] = sortRange[a*2];
    }

    sortRange[newSize] = sortRange[remain];


    remain = newSize;
}
*/

void updateRemain( int & remain, int * sortRange ) { // 10份要變成5份  5份要變成3份 0 1 2 3 4 5 6 7 8 9 10
    int newSize = (remain+1)/2 ; // 10>>5   5>>3
    int * temp = new int[ newSize+1 ]; // [6]  [4]
    for ( int a = 0, b = 0 ; a < newSize+1 ; a++ , b+=2 ) {
        // remain=5 a<6 // a=0 b=0 // a=1 b=2 // a=2 b=4 // a=3 b=6 // a=4 b=8 // a=5 b=10 ////// 0 2k 4k 6k 8k 1w
        // remain=3 a<4 // a=0 b=0 // a=1 b=2 // a=2 b=4 // a=3 b=6 ////// 0 4k 8k
        if ( b > remain ) temp[a] = sortRange[b-1];
        else temp[a] = sortRange[b];
        //sortRange[a] = sortRange[a*2];
        //temp[a+1] = sortRange[b+2];
    }

    //sortRange[newSize] = sortRange[remain];
    remain = newSize;
    //delete [] sortRange;

    //for ( int a = 0 ; a < newSize+1 ; a++ ) cout << temp[a] << endl;

    //sortRange = new int[ newSize+1 ];
    for ( int a = 0 ; a < newSize+1 ; a++ ) sortRange[a] = temp[a];
    //for ( int a = 0 ; a < newSize+1 ; a++ ) cout << "new sortRange:" << sortRange[a] << endl;
    delete[] temp;

}


void MultiProcessBubbleMerge( int * arr, int SizeOfData, int segmentation ) {

    int shmID = shmget( IPC_PRIVATE, SizeOfData * sizeof( int ), IPC_CREAT | 0600 );
    int *shmaddr = NULL;
    int i = SizeOfData / segmentation;
    int pre = 0, pos = i;
    int * sortRange = new int[segmentation+1];

    for ( int a = 0 ; a < segmentation ; a++ ) {
        if ( a + 1 == segmentation ) {
            sortRange[a] = pre;
            sortRange[a+1] = SizeOfData;
        }
        else {
            sortRange[a] = pre;
            sortRange[a+1] = pos;
            pre = pos ;
            pos = pre + i ;
        }
    }

    vector<pid_t> pids;

    for ( int a = 0 ; a < segmentation ; a++ ) {
        pid_t pid = fork();
        if ( pid == 0 ) {  // child process
            shmaddr = ( int * )shmat( shmID, NULL, 0 );
            copy( arr + sortRange[ a ], arr + sortRange[ a + 1 ], shmaddr + sortRange[ a ] );
            Bubble( shmaddr, sortRange[ a + 1 ], sortRange[ a ] );
            shmdt( shmaddr );
            exit( EXIT_SUCCESS );
        }  // else
        else pids.push_back( pid );
    }


    for ( int a = 0 ; a < pids.size() ; a++ ) {
        waitpid( pids.at(a), nullptr, 0 );
    } // 等待所有child排完氣泡



    int remain = segmentation;
    while ( remain > 1 ) {
        pids.clear();
        for ( int a = 0 ; a < remain - 1 ; a = a + 2 ) {
            pid_t pid = fork();
            if ( pid == 0 ) {  // child process
                shmaddr = ( int * )shmat( shmID, NULL, 0 );
                Merge( shmaddr, sortRange[ a ], sortRange[ a + 1 ], sortRange[ a + 2 ] );
                shmdt( shmaddr );
                exit( EXIT_SUCCESS );
            }
            else {    // parent process
                pids.push_back( pid );
            }


        }

        for ( int a = 0 ; a < pids.size() ; a++ ) {
            waitpid( pids.at( a ), nullptr, 0 );
        } // 等待所有child合併完

        updateRemain( remain, sortRange );
    }

    delete[] sortRange;
    shmaddr = ( int * )shmat( shmID, NULL, 0 );
    copy( shmaddr, shmaddr + SizeOfData, arr );
    shmdt( shmaddr );
    shmctl( shmID, IPC_RMID, NULL );


}


void DetailRequire( int & sel, int & segmentation, int size ) {

    cout << "input segmentation to cut data : " << endl ;
    cin >> segmentation;
    //cout << segmentation;
    while ( segmentation < 0 && segmentation > size ) {
        cout << "segmentation need > 0 && < data size" << endl;
        cin >> segmentation;
    }
    cout << "input 1 ~ 4 to do mission ( another is exit ) " << endl ;
    cin >> sel;
}


void MultiThreadBubbleMerge( int * arr, int SizeOfData, int segmentation ){
    vector < pthread_t >  pthreads ;

    int i = SizeOfData / segmentation;
    int pre = 0, pos = i;
    int * sortRange = new int[ segmentation + 1 ];
    ThreadBubbleSt **threadBubbleArr = new ThreadBubbleSt*[ segmentation ];

    for ( int a = 0 ; a < segmentation ; a++ ) {
        if ( a + 1 == segmentation ) {
            sortRange[a] = pre;
            sortRange[a+1] = SizeOfData;
        }
        else {
            sortRange[a] = pre;
            sortRange[a+1] = pos;
            pre = pos ;
            pos = pre + i ;
        }
    }



    for ( int a = 0 ; a < segmentation ; a++ ) {
        pthread_t t;
        threadBubbleArr[ a ] = new ThreadBubbleSt;
        threadBubbleArr[ a ]->arr = arr;
        threadBubbleArr[ a ]->st = sortRange[ a ];
        threadBubbleArr[ a ]->ed = sortRange[ a + 1 ];
        pthread_create( &t, NULL, Bubble, ( void * )threadBubbleArr[ a ] );  // 創thread進行氣泡
        pthreads.push_back( t );
    }


    for ( int a = 0 ; a < pthreads.size() ; a++ ) {
        pthread_join( pthreads.at( a ), NULL ); // 等代所有執行完畢
        delete[] threadBubbleArr[ a ];
    }

    delete[] threadBubbleArr;
    int remain = segmentation;

	while( remain > 1 ) {
        int count = 0;
		pthreads.clear();
		ThreadMergeSt **threadMergeArr = new ThreadMergeSt*[ segmentation/2 ];
		//if remain = 8 = segmentation
        for ( int a = 0 ; a < remain - 1 ; a = a + 2 ) { // 0 2 4 6

            pthread_t t;
            threadMergeArr[ count ] = new ThreadMergeSt;
            threadMergeArr[ count ]->arr = arr;
            threadMergeArr[ count ]->left = sortRange[ a ];
            threadMergeArr[ count ]->mid = sortRange[ a + 1 ];
            threadMergeArr[ count ]->right = sortRange[ a + 2 ];
            pthread_create( &t, NULL, Merge, ( void * )threadMergeArr[ count ] );
            pthreads.push_back( t );
            count++;
        }


        for ( int a = 0 ; a < pthreads.size() ; a++ ) {
            pthread_join( pthreads.at( a ), NULL );
            delete[] threadMergeArr[ a ];
        }

        delete[] threadMergeArr;
		updateRemain( remain, sortRange );

	}


    delete[] sortRange;
}

void *Merge( void *Struct ) {
  ThreadMergeSt *mergeSt = ( ThreadMergeSt * )Struct;
  Merge( mergeSt->arr, mergeSt->left, mergeSt->mid, mergeSt->right );
  pthread_exit( 0 );
  cout << "!!!!should not appear!!!!" << endl;
}

void *Bubble( void *Struct ) {
  ThreadBubbleSt *bubbleSt = ( ThreadBubbleSt * )Struct;
  Bubble( bubbleSt->arr, bubbleSt->ed, bubbleSt->st  );
  pthread_exit( 0 );
  cout << "!!!!should not appear!!!!" << endl;
}
