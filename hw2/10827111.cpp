# include <iostream>
# include <vector>
# include <fstream>
# include <deque>

using namespace std;

struct processblock {
    int id;
    int burst;
    int arrive;
    int finsh;
    int turnaround;
    int wait;
    int priority;
    int lastburst;
    double ratio;
};

typedef vector< processblock > vec;

char idTransform ( int id );
void Readfile( vector< processblock > & data, string & filename, int & sel, int & slice );
void Writefile( vector< processblock > ans, int sel, string fileName, string gantt );
void ALL( vector< processblock > data, int sel, int slice, string & filename, string & gantt );
void sortResult ( vector< processblock > & result  );
vec FCFS( vector< processblock > data, string & gantt );
vec RR ( vector< processblock > data, string & gantt, int slice );
vec SJF( vector< processblock > data, string & gantt );
vec SRTF ( vector< processblock > data, string & gantt );
vec HRRN( vector< processblock > data, string & gantt );
vec PPRR( vector< processblock > data, string & gantt, int slice );




int main() {
    string filename;
    int sel, slice;
    bool quit = false;
    vector< processblock > data, ans;
    do {
        Readfile( data, filename, sel, slice );
        string gantt;
        switch ( sel ) {
            case 1 :
                if ( data.size() == 0 ) break;
                ans = FCFS( data, gantt );
                Writefile( ans, sel, filename, gantt );
                break;
            case 2 :
                if ( data.size() == 0 ) break;
                ans = RR ( data, gantt, slice );
                Writefile ( ans, sel, filename, gantt );
                break;
            case 3 :
                if ( data.size() == 0 ) break;
                ans = SJF( data, gantt );
                Writefile ( ans, sel, filename, gantt );
                break;
            case 4 :
                if ( data.size() == 0 ) break;
                ans = SRTF( data, gantt );
                Writefile ( ans, sel, filename, gantt );
                break;
            case 5 :
                if ( data.size() == 0 ) break;
				ans = HRRN ( data, gantt );
				Writefile ( ans, sel, filename, gantt );
                break;
            case 6 :
                if ( data.size() == 0 ) break;
                ans = PPRR ( data, gantt, slice );
                Writefile ( ans, sel, filename, gantt );
                break;
            case 7 :
                if ( data.size() == 0 ) break;
                ALL( data, sel, slice, filename, gantt );
                break;
            default :
                quit = true;
                break;
        }

        //if ( sel == 1 || sel == 2 || sel == 3 || sel == 4 ) Writefile( arr, SizeOfData, filename, sel, CPUtime );

        data.clear();


    } while ( !quit );
    return 0;
}


void Readfile( vector<processblock> & data, string & filename, int & sel, int & slice ) {
    string str;
    processblock p;
    cout << "input file name ( ex: input1_method1 or input1 ) :" << endl ;
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

    file >> sel >> slice;
    getline( file, str );
    getline( file, str );
    while ( !file.eof() ) {
        file >> p.id >> p.burst >> p.arrive >> p.priority;
        p.lastburst = p.burst;
        data.push_back(p);
    }

    file.close();
    data.pop_back();
}

void Writefile( vector< processblock > ans, int sel, string filename, string gantt ) {
	fstream out;
	filename = "out_" + filename + ".txt";		
	out.open( filename.c_str(), fstream::out );
	if ( sel == 1 ) {
		out << "FCFS" << endl;
		out << "==        FCFS==" << endl;
    }
    else if ( sel == 2 ) {
    	out << "RR" << endl;
		out << "==          RR==" << endl;
    } 
    else if ( sel == 3 ) {
    	out << "SJF" << endl;
		out << "==         SJF==" << endl;
	} 
	else if ( sel == 4 ) {
		out << "SRTF" << endl;
		out << "==        SRTF==" << endl;
	} 
	else if ( sel == 5 ) {
		out << "HRRN" << endl;
		out << "==        HRRN==" << endl;
	} 
	else if ( sel == 6 ) {
		out << "Priority RR" << endl;
		out << "==        PPRR==" << endl;
	} 
    
    out << gantt << endl;
	out << "===========================================================" << endl << endl;
	out << "Waiting Time" << endl;
	if( sel == 1 ) 	out << "ID\tFCFS" << endl;
    else if ( sel == 2 ) out << "ID\tRR" << endl;
    else if ( sel == 3 ) out << "ID\tSJF" << endl;
	else if ( sel == 4 ) out << "ID\tSRTF" << endl;
	else if ( sel == 5 ) out << "ID\tHRRN" << endl;
	else if ( sel == 6 ) out << "ID\tPPRR" << endl ;
	 
	
	out << "===========================================================" << endl;
	for ( int a = 0 ; a < ans.size(); a++ ) {
		out << ans[a].id << "\t" << ans[a].wait << endl;
	} 

	out << "===========================================================" << endl << endl;
	out << "Turnaround Time" << endl ;
 	if( sel == 1 ) 	out << "ID\tFCFS" << endl;
    else if ( sel == 2 ) out << "ID\tRR" << endl;
    else if ( sel == 3 ) out << "ID\tSJF" << endl;
	else if ( sel == 4 ) out << "ID\tSRTF" << endl;
	else if ( sel == 5 ) out << "ID\tHRRN" << endl;
	else if ( sel == 6 ) out << "ID\tPPRR" << endl ;
	
	out << "===========================================================" << endl;
	for ( int a = 0 ; a < ans.size(); a++ ) {
		out << ans[a].id << "\t" << ans[a].turnaround << endl;
	} 

	out << "===========================================================" << endl << endl;
	out.close();
} 

void ALL( vector< processblock > data, int sel, int slice, string & filename, string & gantt ) {
	fstream out;
	filename = "out_" + filename + ".txt";
    out.open( filename.c_str(), ios :: out );
	vector < string > all_gantt;
	vector < vector< processblock > > ans;
	
	out << "All" << endl << "==        FCFS==" << endl;
    ans.push_back( FCFS( data, gantt ) );
    out << gantt << endl;
    gantt = "\0";
    
    out << "==          RR==" << endl;
    ans.push_back( RR ( data, gantt, slice ) );
    out << gantt << endl;
    gantt = "\0";
    
    out << "==         SJF==" << endl;
    ans.push_back( SJF( data, gantt ) );
    out << gantt << endl;
    gantt = "\0";
    
    out << "==        SRTF==" << endl;
    ans.push_back( SRTF( data, gantt ) );
    out << gantt << endl;
    gantt = "\0";
    
    out << "==        HRRN==" << endl;
    ans.push_back( HRRN( data, gantt ) );
    out << gantt << endl;
    gantt = "\0";
    
    out << "==        PPRR==" << endl;
    ans.push_back( PPRR ( data, gantt, slice ) );
    out << gantt << endl;
    gantt = "\0";
    
    out << "===========================================================" << endl;
	out << endl << "Waiting Time" << endl;
	out << "ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR" << endl;
	out << "===========================================================" << endl;
    for ( int a = 0 ; a < ans[0].size() ; a++ ) {
    	out << ans[0][a].id << "\t" << ans[0][a].wait << "\t" << ans[1][a].wait << "\t" 
		    << ans[2][a].wait << "\t"<< ans[3][a].wait << "\t" << ans[4][a].wait << "\t" 
			<< ans[5][a].wait << endl;
	}
	
	out << "===========================================================" << endl;
	out << endl << "Turnaround Time" << endl;
	out << "ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR" << endl;
	out << "===========================================================" << endl;
	for ( int a = 0 ; a < ans[0].size() ; a++ ) {
    	out << ans[0][a].id << "\t" << ans[0][a].turnaround << "\t" << ans[1][a].turnaround << "\t" 
		    << ans[2][a].turnaround << "\t"<< ans[3][a].turnaround << "\t" << ans[4][a].turnaround << "\t" 
			<< ans[5][a].turnaround << endl;
	}
	out << "===========================================================" << endl << endl;
    

}

vec FCFS ( vector< processblock > data, string & gantt ) {
    int count = 0, run = 0;
    vector< processblock > result;
    for ( int a = 0 ; a < data.size() ; a++ ) {
        for ( int b = a+1 ; b < data.size() ; b++  ) {
            if ( data[a].arrive > data[b].arrive ) swap( data[a], data[b] );
            else if ( data[a].arrive == data[b].arrive && data[a].id > data[b].id ) swap( data[a], data[b] );
        }
    }


    // for ( int a = 0 ; a < data.size() ; a++ )cout << "id : " << data[a].id << "\t arrive : " << data[a].arrive << endl;
    while ( run < data.size() ) {
        if ( count >= data[run].arrive ) {
            char ch = idTransform( data[run].id ) ;
            for ( int a = 0 ; a < data[run].burst ; a++ ) gantt = gantt + ch;
            count = count + data[run].burst;
            data[run].finsh = count;
            data[run].turnaround = data[run].finsh - data[run].arrive;
            data[run].wait = data[run].turnaround - data[run].burst;

            run++;
        } else {
            gantt = gantt + "-";
            count++;
        }

    }

    result = data;
    sortResult ( result );
    return result;

}

vec RR( vector< processblock > data, string & gantt, int slice ) {

    int run = 0, count = 0;
    vector < processblock > queue, result;
    queue.clear();
    result.clear();
    for ( int a = 0 ; a < data.size() ; a++ ) {
        for ( int b = a+1 ; b < data.size() ; b++  ) {
            if ( data[a].arrive > data[b].arrive ) swap( data[a], data[b] );
            else if ( data[a].arrive == data[b].arrive && data[a].id > data[b].id ) swap( data[a], data[b] );
        }
    }

    // for ( int a = 0 ; a < data.size() ; a++ )  cout << "id : " << data[a].id << "\t arrive : " << data[a].arrive << "\t brust : " << data[a].burst << endl;

    for ( int complete = data.size() ; complete != 0 ; ) {
        for ( ; run < data.size() && count >= data[run].arrive ; run++ ) {
            queue.push_back(data[run]);
        }


        if ( !queue.empty() ) {
            char ch = idTransform( queue[0].id );
            for ( int a = 0 ; a < slice && queue[0].lastburst != 0 ; a++ ) {
                gantt = gantt + ch;
                count++;
                for ( ; run < data.size() && count >= data[run].arrive ; run++ )	{
                    queue.push_back( data[run] );
                }

                queue[0].lastburst--;
            }

            if ( queue[0].lastburst == 0 ) {
                queue[0].finsh = count;
                queue[0].turnaround = queue[0].finsh - queue[0].arrive;
                queue[0].wait = queue[0].turnaround - queue[0].burst;
                result.push_back( queue[0] );
                complete--;
                // cout << complete << endl;
            } // 做完了
            else {
                queue.push_back( queue[0] );
            } // 還沒做完 要重新排隊

            queue.erase( queue.begin() );
        } else {
            gantt = gantt + "-";
            count++;
        }

        // cout << gantt << endl;
    }
    
    queue.clear();
    
    sortResult ( result );
    return result;
   

}


vec SJF ( vector< processblock > data, string & gantt ) {
    //int count = 0, run = 0;
    vector < processblock > queue, result;
    for ( int a = 0 ; a < data.size() ; a++ ) {
        for ( int b = a+1 ; b < data.size() ; b++  ) {
            if ( data[a].arrive > data[b].arrive ) swap( data[a], data[b] );
            else if ( data[a].arrive == data[b].arrive && data[a].id > data[b].id ) swap( data[a], data[b] );
        }
    }

    //for ( int a = 0 ; a < data.size() ; a++ )  cout << "id : " << data[a].id << "\t arrive : " << data[a].arrive << "\t brust : " << data[a].burst << endl;

    for ( int count = 0 ; !data.empty() ; ) {

        int min = 0;
        bool find = false;
        for ( int b = 0 ; count >= data[b].arrive && b < data.size() ; b++ ) {
            find = true;

            if ( data[min].burst > data[b].burst ) min = b;
            else if ( data[min].burst == data[b].burst && data[min].arrive > data[b].arrive ) min = b;
            else if ( data[min].burst == data[b].burst && data[min].arrive == data[b].arrive && data[min].id> data[b].id ) min = b;
        }

        if ( find ) {
            char ch = idTransform( data[min].id ) ;
            for ( int a = 0 ; a < data[min].burst ; a++ ) gantt = gantt + ch;
            count = count + data[min].burst;
            data[min].finsh = count;
            data[min].turnaround = data[min].finsh - data[min].arrive;
            data[min].wait = data[min].turnaround - data[min].burst;
            result.push_back( data[min] );
            //cout << gantt << "\t\t remain data: " << data.size() << endl;
            data.erase( data.begin()+min );
            
            //for ( int a = 0 ; a < data.size() ; a++ ) cout << "id : " << data[a].id << "\t arrive : " << data[a].arrive << "\t brust : " << data[a].burst << endl;
        } else  {
            gantt = gantt + "-";
            count++;
        }

        //cout << gantt << "\t\t remain data: " << data.size() << endl;
    }
    
    sortResult ( result );
    return result;


}


vec SRTF ( vector< processblock > data, string & gantt ) {

    vector < processblock > queue, result;
    for ( int a = 0 ; a < data.size() ; a++ ) {
        for ( int b = a+1 ; b < data.size() ; b++  ) {
            if ( data[a].arrive > data[b].arrive ) swap( data[a], data[b] );
            else if ( data[a].arrive == data[b].arrive && data[a].id > data[b].id ) swap( data[a], data[b] );
        }
    }

    // for ( int a = 0 ; a < data.size() ; a++ )  cout << "id : " << data[a].id << "\t arrive : " << data[a].arrive << "\t brust : " << data[a].burst << endl;

    for ( int count = 0 ; !data.empty() ; ) {

        int min = 0;
        bool find = false;
        for ( int b = 0 ; count >= data[b].arrive && b < data.size() ; b++ ) {
            find = true;

            if ( data[min].lastburst > data[b].lastburst ) min = b;
            else if ( data[min].lastburst == data[b].lastburst && data[min].arrive > data[b].arrive ) min = b;
            else if ( data[min].lastburst == data[b].lastburst && data[min].arrive == data[b].arrive && data[min].id> data[b].id ) min = b;
        }

        if ( find ) {
            char ch = idTransform( data[min].id ) ;
            if ( data[min].lastburst != 0 ) {
                gantt = gantt + ch;
                count++;
                data[min].lastburst--;
            } else {
                data[min].finsh = count;
                data[min].turnaround = data[min].finsh - data[min].arrive;
                data[min].wait = data[min].turnaround - data[min].burst;
                result.push_back( data[min] );
                //cout << gantt << "\t\t remain data: " << data.size() << endl;
                data.erase( data.begin()+min );
            }
        } else  {
            gantt = gantt + "-";
            count++;
        }

        //cout << gantt << "\t\t remain data: " << data.size() << endl;
    }
    
    sortResult ( result );
    return result;

}

char idTransform ( int id ) {
    char ch;
    if ( id > 9 ) return id + 55;
    else return id + 48;
}


vec HRRN ( vector< processblock > data, string & gantt ) {
    //int count = 0, run = 0;
    vector < processblock > queue, result;
    for ( int a = 0 ; a < data.size() ; a++ ) {
        for ( int b = a+1 ; b < data.size() ; b++  ) {
            if ( data[a].arrive > data[b].arrive ) swap( data[a], data[b] );
            else if ( data[a].arrive == data[b].arrive && data[a].id > data[b].id ) swap( data[a], data[b] );
        }
    }

    //for ( int a = 0 ; a < data.size() ; a++ )  cout << "id : " << data[a].id << "\t arrive : " << data[a].arrive << "\t brust : " << data[a].burst << endl;

    for ( int count = 0 ; !data.empty() ; ) {

        int target = 0 ;
        bool find = false;
        
        for ( int b = 0 ; count >= data[b].arrive && b < data.size() ; b++ ) {
            find = true;
            if ( data[target].ratio < data[b].ratio ) target = b;
            else if ( data[target].ratio == data[b].ratio && data[target].arrive > data[b].arrive ) target = b;
            else if ( data[target].ratio == data[b].ratio && data[target].arrive == data[b].arrive && data[target].id > data[b].id ) target = b;
        }
        
        

        if ( find ) {
            char ch = idTransform( data[target].id ) ;
            for ( int a = 0 ; a < data[target].burst ; a++ ) gantt = gantt + ch;
            count = count + data[target].burst;
            data[target].finsh = count;
            data[target].turnaround = data[target].finsh - data[target].arrive;
            data[target].wait = data[target].turnaround - data[target].burst;
            result.push_back( data[target] );
            //cout << gantt << "\t\t remain data: " << data.size() << endl;
            data.erase( data.begin()+target );
            
            for ( int a = 0 ; a < data.size() && count >= data[a].arrive ; a++ ) {
        		data[a].wait = count - data[a].arrive;
        		double twait = data[a].wait;
        		double tburst = data[a].burst;
        		data[a].ratio = ( twait + tburst) / tburst ;
        		//cout << data[a].id  << " 's ratio : " << data[a].ratio << endl;
			}
        
        } 
		else {
            gantt = gantt + "-";
            count++;
        }

        //cout << gantt << "\t\t remain data: " << data.size() << endl;
    }   
    
    sortResult ( result );
    return result;
}

/*
vec PPRR( vector< processblock > data, string & gantt, int slice ) {

    int run = 0, count = 0;
    bool newHighP = false;
    vector < processblock > queue, result, sameP;
    vector < vector < processblock > > preempQueue;
    queue.clear();
    result.clear();
    for ( int a = 0 ; a < data.size() ; a++ ) {
        for ( int b = a+1 ; b < data.size() ; b++  ) {
            if ( data[a].arrive > data[b].arrive ) swap( data[a], data[b] );
            else if ( data[a].arrive == data[b].arrive && data[a].priority > data[b].priority ) swap( data[a], data[b] );
        }
    }

    //for ( int a = 0 ; a < data.size() ; a++ )  cout << "id : " << data[a].id << "\t arrive : " << data[a].arrive << "\t priority : " << data[a].priority << "\t burst : " << data[a].burst << endl;

    for ( int complete = data.size() ; complete != 0 ; ) {
    	
        for ( ; run < data.size() && count >= data[run].arrive ; run++ ) queue.push_back(data[run]);
        
        if ( !queue.empty() ) {

            int high = 0;
            sameP.clear();
            if ( preempQueue.size() == 0 || newHighP ) {
                for ( int a = 1 ; a < queue.size() ; a++ ) {
                    if ( queue[high].priority > queue[a].priority ) {
                    	high = a;
                    	sameP.clear();
					}
                    else if ( queue[high].priority == queue[a].priority ) sameP.push_back( queue[a] );
                } // for 找出最高優先度的
                
                sameP.insert( sameP.begin(), queue[high] );
                newHighP = false; 
            } // 沒有上次被奪取的
			else {
				sameP.assign( preempQueue[0].begin(), preempQueue[0].end()  );
				preempQueue.erase( preempQueue.begin() );
			} // 繼續執行上次被奪取的


            while ( !sameP.empty() ) {
                char ch = idTransform( sameP[0].id );    
                for ( int a = 0 ; a < slice && sameP[0].lastburst != 0 ; a++ ) {
                    
                    gantt = gantt + ch;
                    count++;
                    cout << "count:" << count <<" a:" << a << " id:" << sameP[0].id << endl; 
					sameP[0].lastburst--;
                    for ( ; run < data.size() && count >= data[run].arrive ; run++ ) {
                        queue.push_back( data[run] );
                        if ( data[run].priority < sameP[0].priority ) newHighP = true;
						else if ( data[run].priority == sameP[0].priority ) sameP.insert( sameP.begin()+1, data[run] );
                    }

                    if ( newHighP ) break;
                }

                if ( sameP[0].lastburst == 0 ) {
                    sameP[0].finsh = count;
                    sameP[0].turnaround = sameP[0].finsh - sameP[0].arrive;
                    sameP[0].wait = sameP[0].turnaround - sameP[0].burst;
                    result.push_back( sameP[0] );

                    for ( int a = 0 ; a < queue.size() ; a++  ) {
                        if ( sameP[0].id == queue[a].id ) queue.erase( queue.begin()+a );
                    } //

                    complete--;
                } // 做完了
                else {
                    sameP.push_back( sameP[0] );
                } // 還沒做完 要重新排隊

				sameP.erase( sameP.begin() );

                if ( newHighP ) {
                	
                    //for ( int a = 0 ; a < sameP.size() ; a++ ) {
                       // for ( int b = 0 ; b < queue.size() ; b++  )
                            //if ( sameP[a].id == queue[b].id ) queue[b] = sameP[a];
                    //}
					
					
                    if ( !sameP.empty() )  preempQueue.insert( preempQueue.begin(), sameP );
                    sameP.clear();
                    break;
                }

            } // while 

        } 
		else {
            gantt = gantt + "-";
            count++;
        }

    }


    cout << gantt << endl;

    for ( int a = 0 ; a < result.size() ; a++ ) {
        for ( int b = a+1 ; b < result.size() ; b++  ) {
            if ( result[a].id > result[b].id ) swap( result[a], result[b] );
        }
    }

    cout << "wait" << endl;
    for ( int a = 0 ; a < result.size() ; a++ ) {
        cout << result[a].id << "\t" << result[a].wait << endl;
    }

    cout << "turnaround" << endl;
    for ( int a = 0 ; a < result.size() ; a++ ) {
        cout << result[a].id << "\t" << result[a].turnaround << endl;
    }

    queue.clear();
    return result;
}
*/

vec PPRR( vector< processblock > data, string & gantt, int slice ) {

    int run = 0, count = 0;
    bool newHighP = false;
    vector < processblock > queue, result, sameP;
    vector < vector < processblock > > preempQueue;
    queue.clear();
    result.clear();
    for ( int a = 0 ; a < data.size() ; a++ ) {
        for ( int b = a+1 ; b < data.size() ; b++  ) {
            if ( data[a].arrive > data[b].arrive ) swap( data[a], data[b] );
            else if ( data[a].arrive == data[b].arrive && data[a].priority > data[b].priority ) swap( data[a], data[b] );
            else if ( data[a].arrive == data[b].arrive && data[a].priority == data[b].priority && data[a].id > data[b].id ) swap( data[a], data[b] );
        }
    }

    //for ( int a = 0 ; a < data.size() ; a++ )  cout << "id : " << data[a].id << "\t arrive : " << data[a].arrive << "\t priority : " << data[a].priority << "\t burst : " << data[a].burst << endl;

    for ( int complete = data.size() ; complete != 0 ; ) {
    	
        for ( ; run < data.size() && count >= data[run].arrive ; run++ ) queue.push_back(data[run]);
        
        if ( !queue.empty() ) {

            int high = 0;
            sameP.clear();
            if ( preempQueue.size() == 0 || newHighP ) {
                for ( int a = 1 ; a < queue.size() ; a++ ) {
                    if ( queue[high].priority > queue[a].priority ) {
                    	high = a;
                    	sameP.clear();
					} // 有更高優先度的 
                    else if ( queue[high].priority == queue[a].priority ) {
                    	sameP.push_back( queue[a] );
					}
                } // for 找出最高優先度的
                
                sameP.insert( sameP.begin(), queue[high] ); // 將第一個找到的process重新放入sameP裡第一個
				
				 
                newHighP = false; 
            } // 沒有上次被奪取的 從queue裡面找高優先度的來排 
			else {
				//cout << endl<< "來到preempQueue的部分" << endl;
				bool highThenpreempQueue = false;
				sameP.assign( preempQueue[0].begin(), preempQueue[0].end()  );		// 取回上次被奪取的	
				//cout <<">>>" <<sameP.size() << "<<<" << endl;
				//for ( int a = 0 ; a< sameP.size() ; a++ ) cout << "sameP[0].id:" << sameP[a].id << "\t";
				//cout <<">>><<<" << endl;
				for ( int a = 0 ; a < queue.size() ; a++ ) {
					if ( sameP[0].priority > queue[a].priority   ) {
                    	high = a;
                    	sameP.clear();
                    	highThenpreempQueue = true;
                    	sameP.push_back(queue[a]);
					} // 有更高優先度的 
						
				} // 新的process在那段期間可能有進到queue裡，同時優先度可能高於被奪取的queue
				
				for ( int a = 0 ; a < queue.size() ; a++  ) {
					bool repeat = false;
					if ( sameP[0].priority == queue[a].priority ) {
						for ( int b = 0 ; b < sameP.size() ; b++ ) {
							if ( queue[a].id == sameP[b].id ) repeat = true;
						} // 看該process是否已存在sameP
						
						if ( !repeat ) sameP.push_back( queue[a] ); // 不存在sameP裡，就加入一起RR 
					}
				} // 除了上次被奪取的， 可能還有新的process在那段期間進到queue裡 所以要更新sameP
				
				if ( !highThenpreempQueue )  preempQueue.erase( preempQueue.begin() ); 
				
				
				//for ( int a = 0 ; a< sameP.size() ; a++ ) cout << "sameP[0].id:" << sameP[a].id << "\t";
				//cout << sameP.size() << endl;
			} // 繼續執行上次被奪取的


            while ( !sameP.empty() ) {
                char ch = idTransform( sameP[0].id );    
                for ( int a = 0 ; a < slice && sameP[0].lastburst != 0 ; a++ ) {
                    
                    gantt = gantt + ch;
                    count++;
                    sameP[0].lastburst--;
                    //cout <<sameP.size() << " count:" << count <<" a:" << a << " id:" << sameP[0].id << " last:" << sameP[0].lastburst<<endl; 
					
                    for ( ; run < data.size() && count >= data[run].arrive ; run++ ) {
                    	//cout << "有要更新大小  run:" << run << "   data[run].priority : "<< data[run].priority << endl; 
                        queue.push_back( data[run] );
                        //cout << "sameP[0].priority : "<< sameP[0].priority << endl; 
                        if ( data[run].priority < sameP[0].priority ) newHighP = true; // 有更高優先度的 
						else if ( data[run].priority == sameP[0].priority ) {
							//cout << "insert 前::::" <<endl; 
							//for ( int a = 0 ; a< sameP.size() ; a++ ) cout << "sameP.id:" << sameP[a].id << "\t";
							if ( sameP[0].arrive != data[run].arrive ) sameP.push_back( data[run] );
							else sameP.insert( sameP.begin()+1, data[run] );
							//cout << "insert 後::::" <<endl; 
							//for ( int a = 0 ; a< sameP.size() ; a++ ) cout << "sameP.id:" << sameP[a].id << "\t";
						}
                    } // for 執行期間 要更新queue 並偵測是否有process優先度高於目前在跑的process

                    if ( newHighP ) {
                    	//cout << ">>" << sameP.size(); 
                    	break; 
					}
					 
                }

                if ( sameP[0].lastburst == 0 ) {
                    sameP[0].finsh = count;
                    sameP[0].turnaround = sameP[0].finsh - sameP[0].arrive;
                    sameP[0].wait = sameP[0].turnaround - sameP[0].burst;
                    result.push_back( sameP[0] );

                    for ( int a = 0 ; a < queue.size() ; a++  ) {
                        if ( sameP[0].id == queue[a].id ) queue.erase( queue.begin()+a );
                    } // 做完了 所以要把process從queue移除 
                    
                    //cout << "finish:" << sameP[0].id <<endl;
                    complete--;
                } // 做完了
                else {
                    sameP.push_back( sameP[0] );
                } // 還沒做完 要重新排隊

				sameP.erase( sameP.begin() );

                if ( newHighP ) {
                	/* 
                    for ( int a = 0 ; a < sameP.size() ; a++ ) {
                        for ( int b = 0 ; b < queue.size() ; b++  )
                            if ( sameP[a].id == queue[b].id ) queue[b] = sameP[a];  
                    } // 因在sameP裡的process有在執行，更新相應的queue資訊
					*/ 
					
                    if ( !sameP.empty() )  {
                    	preempQueue.insert( preempQueue.begin(), sameP );  
                    	//cout << "preempQueue更新" <<endl;
                    	//for ( int a = 0 ; a< sameP.size() ; a++ ) cout << "sameP[0].id:" << sameP[a].id << "\t";
					}
                    sameP.clear();
                    break;
                } //  

            } // while 

        } 
		else {
            gantt = gantt + "-";
            count++;
        }

    }

    queue.clear();
    sortResult ( result );
    return result;
}

void sortResult ( vector< processblock > & result  ){
	for ( int a = 0 ; a < result.size() ; a++ ) {
        for ( int b = a+1 ; b < result.size() ; b++  ) {
            if ( result[a].id > result[b].id ) swap( result[a], result[b] );
        }
    }
}

