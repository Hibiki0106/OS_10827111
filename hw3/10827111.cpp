# include <iostream>
# include <vector>
# include <fstream>
# include <deque>

using namespace std;

struct pageStruct {
	char ch;
	int count;
};

typedef struct  {
	vector < vector < pageStruct > > ans;
	vector < char > fault;
	int times;	
} output;

void Readfile( int & sel, int & frame, string & filename, string & pageString );
void WritefileALL( string filename, string pageString, output result, int sel, int frame );
void writefile( string filename, string pageString, output result, int sel, int frame );
output FIFO( int frame, string pageString, string filename );
output LRU( int frame, string pageString, string filename );
output LFU( int frame, string pageString, string filename );
output MFU( int frame, string pageString, string filename );
output LFUandLRU( int frame, string pageString, string filename );
void ALL( int frame, string pageString, string filename );


int main() {
    string filename, pageString;
    int sel, frame;
    bool quit = false;
    do {
        Readfile( sel, frame, filename, pageString );
        output result;
        switch ( sel ) {
            case 1 :
            	result = FIFO( frame, pageString, filename );
            	writefile( filename, pageString, result, sel, frame );
                break;
            case 2 :
            	result = LRU( frame, pageString, filename );
            	writefile( filename, pageString, result, sel, frame );
                break;
            case 3 :
            	result = LFU( frame, pageString, filename );
            	writefile( filename, pageString, result, sel, frame );
                break;
            case 4 :
            	result = MFU( frame, pageString, filename );
            	writefile( filename, pageString, result, sel, frame );
                break;
            case 5 :
            	result = LFUandLRU( frame, pageString, filename );
            	writefile( filename, pageString, result, sel, frame );
                break;
            case 6 :
            	ALL( frame, pageString, filename );
                break;
            default :
                quit = true;
                break;
        }

        //if ( sel == 1 || sel == 2 || sel == 3 || sel == 4 ) Writefile( arr, SizeOfData, filename, sel, CPUtime );



    } while ( !quit );
    return 0;
}

output FIFO( int frame, string pageString, string filename ) {
	vector < pageStruct > page; 
	vector < char > fault;
	vector < vector< pageStruct > > ans;
	output o;
	bool replace = true;
	int times = 0;
	for ( int a = 0 ; a < pageString.size() ; a++ ){
		if ( page.size() >= frame ) {
			for ( int b = 0 ; b < page.size() ; b++ ) {
				if ( page[b].ch == pageString[a] ) replace = false;
			}
			
			
			if ( replace ) {
				page.pop_back();
				times++;
			}
			
		}
		
		for ( int b = 0 ; b < page.size() ; b++ ) {
			if ( page[b].ch == pageString[a] ) replace = false;
		}
		
		if ( replace ) {
			pageStruct temp;
			temp.ch = pageString[a];
			page.insert( page.begin(), temp );
			fault.push_back('F');
		}
		else fault.push_back('\0');
		
		replace = true;
		ans.push_back( page );
	}
	
	o.ans.assign( ans.begin(), ans.end() );
	o.fault.assign( fault.begin(), fault.end() );
	o.times = times;
	
	return o;
}

output LRU( int frame, string pageString, string filename ) {
	vector < pageStruct > page;
	vector < char > fault;
	vector < vector< pageStruct > > ans;
	bool replace = true;
	output o;
	int times = 0;
	for ( int a = 0 ; a < pageString.size() ; a++ ){
		if ( page.size() >= frame ) {
			for ( int b = 0 ; b < page.size() ; b++ ) {
				if ( page[b].ch == pageString[a] ) {
					replace = false;
				    page.erase( page.begin()+b );
				}
			}
			
			if ( replace ) {
				page.pop_back();
				times++;
			}
		}
		
		for ( int b = 0 ; b < page.size() ; b++ ) {
			if ( page[b].ch == pageString[a] ) {
				replace = false;
				page.erase( page.begin()+b );
			}
		}
		
		pageStruct temp;
		temp.ch = pageString[a];
		page.insert( page.begin(), temp );
		if ( replace ) fault.push_back('F');
		else fault.push_back('\0');
		
		replace = true;
		ans.push_back( page );
	}
	
   	o.ans.assign( ans.begin(), ans.end() );
	o.fault.assign( fault.begin(), fault.end() );
	o.times = times;
	
	return o;
	
}

output LFU( int frame, string pageString, string filename ) {
	vector < pageStruct > page;
	vector < char > fault;
	vector < vector< pageStruct > > ans;
	bool replace = true;
	output o;
	int times = 0;
	for ( int a = 0 ; a < pageString.size() ; a++ ){
		if ( page.size() >= frame ) {
			for ( int b = 0 ; b < page.size() ; b++ ) {
				if ( page[b].ch == pageString[a] ) {
					replace = false;
					page[b].count++;
				}
			}
			
			
			if ( replace ) {
				int min = page[0].count;
				for ( int b = 1 ; b < page.size() ; b++ ){
					if ( min > page[b].count ) min = page[b].count;
				}
				
				for ( int b = frame-1 ; b >= 0 ; b-- ) {
					if ( page[b].count == min ) {
						page.erase( page.begin()+b );
						times++;
						break;
					}
				}
				
			}
			
		}
		
		for ( int b = 0 ; b < page.size() ; b++ ) {
			if ( page[b].ch == pageString[a] ) {
				replace = false;
				page[b].count++;
			}
		}
		
		if ( replace ) {
			pageStruct temp;
			temp.ch = pageString[a];
			page.insert( page.begin(), temp );
			fault.push_back('F');
		}
		else fault.push_back('\0');
		
		replace = true;
		ans.push_back( page );
	}
	
    o.ans.assign( ans.begin(), ans.end() );
	o.fault.assign( fault.begin(), fault.end() );
	o.times = times;
	
	return o;
}

output MFU( int frame, string pageString, string filename ) {
	vector < pageStruct > page;
	vector < char > fault;
	vector < vector< pageStruct > > ans;
	bool replace = true;
	output o;
	int times = 0;
	for ( int a = 0 ; a < pageString.size() ; a++ ){
		if ( page.size() >= frame ) {
			for ( int b = 0 ; b < page.size() ; b++ ) {
				if ( page[b].ch == pageString[a] ) {
					replace = false;
					page[b].count++;
				}
			}
			
			
			if ( replace ) {
				int max = page[0].count;
				for ( int b = 1 ; b < page.size() ; b++ ){
					if ( max < page[b].count ) max = page[b].count;
				}
				
				for ( int b = frame-1 ; b >= 0 ; b-- ) {
					if ( page[b].count == max ) {
						page.erase( page.begin()+b );
						times++;
						break;
					}
				}
				
			}
			
		}
		
		for ( int b = 0 ; b < page.size() ; b++ ) {
			if ( page[b].ch == pageString[a] ) {
				replace = false;
				page[b].count++;
			}
		}
		
		if ( replace ) {
			pageStruct temp;
			temp.ch = pageString[a];
			page.insert( page.begin(), temp );
			fault.push_back('F');
		}
		else fault.push_back('\0');
		
		replace = true;
		ans.push_back( page );
	}
	
   	o.ans.assign( ans.begin(), ans.end() );
	o.fault.assign( fault.begin(), fault.end() );
	o.times = times;
	
	return o;
}

output LFUandLRU( int frame, string pageString, string filename ) {
	vector < pageStruct > page;
	vector < char > fault;
	vector < vector< pageStruct > > ans;
	bool replace = true;
	output o;
	int times = 0;
	for ( int a = 0 ; a < pageString.size() ; a++ ){
		if ( page.size() >= frame ) {
			for ( int b = 0 ; b < page.size() ; b++ ) {
				if ( page[b].ch == pageString[a] ) {
					replace = false;
					page[b].count++;
					page.insert( page.begin(), page[b] );
					page.erase( page.begin()+b+1 ); // because insert page[b], the target of remove will + 1 
				}
			}
			
			
			if ( replace ) {
				int min = page[0].count;
				for ( int b = 1 ; b < page.size() ; b++ ){
					if ( min > page[b].count ) min = page[b].count;
				}
				
				for ( int b = frame-1 ; b >= 0 ; b-- ) {
					if ( page[b].count == min ) {
						page.erase( page.begin()+b );
						times++;
						break;
					}
				}
				
			} // if ( replace )
			
		} //if ( page.size() >= frame )
		
		for ( int b = 0 ; b < page.size() ; b++ ) {
			if ( page[b].ch == pageString[a] ) {
				replace = false;
				page[b].count++;
				page.insert( page.begin(), page[b] );
				page.erase( page.begin()+b+1 ); // because insert page[b], the target of remove will + 1 
			}
		}
		
		if ( replace ) {
			pageStruct temp;
			temp.ch = pageString[a];
			page.insert( page.begin(), temp );
			fault.push_back('F');
		}
		else fault.push_back('\0');
		
		replace = true;
		ans.push_back( page );
	}
	
   	o.ans.assign( ans.begin(), ans.end() );
	o.fault.assign( fault.begin(), fault.end() );
	o.times = times;
	
	return o;
}

void ALL( int frame, string pageString, string filename ) {
	output result;
	
	result = FIFO( frame, pageString, filename );
	WritefileALL( filename, pageString, result, 1, frame  );
	
	result = LRU( frame, pageString, filename );
	WritefileALL( filename, pageString, result, 2, frame  );
	
	result = LFU( frame, pageString, filename );
	WritefileALL( filename, pageString, result, 3, frame  );
	
	result = MFU( frame, pageString, filename );
	WritefileALL( filename, pageString, result, 4, frame  );
	
	result = LFUandLRU( frame, pageString, filename );
	WritefileALL( filename, pageString, result, 5, frame  );
}

void Readfile( int & sel, int & frame, string & filename, string & pageString ) {
    string str, s;
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

    file >> sel >> frame;
    
    //cout << "s:" << s << endl;
    pageString = "\0";
    while ( !file.eof() ) {
    	file >> s;
        if ( s != "\n" || s != " " || s != "\t" ) pageString = pageString + s;
        file >> s;
    }

    cout << "pageString:" << pageString << endl;
    cout << "pageString.size:" << pageString.size() << endl;
    cout << "sel:" << sel << endl; 
    file.close();
}



void WritefileALL( string filename, string pageString, output result, int sel, int frame  ) {
	fstream out;
	int faulTimes = 0;
	filename = "out_" + filename + ".txt";	
	out.open( filename.c_str(), fstream::app );
	out << "--------------";
	if ( sel == 1 ) out << "FIFO-----------------------" << endl;
	else if ( sel == 2 ) out << "LRU-----------------------" << endl;
	else if ( sel == 3 ) out << "Least Frequently Used Page Replacement-----------------------" << endl;
	else if ( sel == 4 ) out << "Most Frequently Used Page Replacement -----------------------" << endl;
	else if ( sel == 5 ) out << "Least Frequently Used LRU Page Replacement-----------------------" << endl;
	for ( int a = 0 ; a < pageString.size() ; a++ ){
		out << pageString[a] << "\t";
		
    	for ( int b = 0 ; b < result.ans[a].size() ; b++ ) out << result.ans[a][b].ch;
		
		if ( result.fault[a] != '\0' ) {
			faulTimes++;
			out << "\t" << result.fault[a] << endl;
		}
		else out << endl;
	}
	out << "Page Fault = " << faulTimes << "  Page Replaces = " << result.times << "  Page Frames = " << frame << endl ;
	if ( sel != 5 ) out << endl;
	else out.close();
}

void writefile( string filename, string pageString, output result, int sel, int frame  ) {
	fstream out;
	int faulTimes = 0;
	filename = "out_" + filename + ".txt";	
	out.open( filename.c_str(), fstream::out );
	out << "--------------";
	if ( sel == 1 ) out << "FIFO-----------------------" << endl;
	else if ( sel == 2 ) out << "LRU-----------------------" << endl;
	else if ( sel == 3 ) out << "Least Frequently Used Page Replacement-----------------------" << endl;
	else if ( sel == 4 ) out << "Most Frequently Used Page Replacement -----------------------" << endl;
	else if ( sel == 5 ) out << "Least Frequently Used LRU Page Replacement-----------------------" << endl;
	for ( int a = 0 ; a < pageString.size() ; a++ ){
		out << pageString[a] << "\t";
		
    	for ( int b = 0 ; b < result.ans[a].size() ; b++ ) out << result.ans[a][b].ch;
		
		if ( result.fault[a] != '\0' ) {
			faulTimes++;
			out << "\t" << result.fault[a] << endl;
		}
		else out << endl;
	}
	out << "Page Fault = " << faulTimes << "  Page Replaces = " << result.times << "  Page Frames = " << frame << endl ;
	
	out.close();
}
