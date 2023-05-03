#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <bits/stdc++.h>
#include<algorithm>

using namespace std;



int BLOCKSIZE ;
int L1_SIZE ;
int L1_ASSOC; 
int L2_SIZE ;
int L2_ASSOC ;










struct cache{
    int index_size_l1 = L1_SIZE/(BLOCKSIZE*L1_ASSOC);
    int index_size_l2 = L2_SIZE/(BLOCKSIZE*L2_ASSOC);    
    // vector<vector<int>> tag_l1(index_size_l1,{-1});
    // vector<vector<int>> tag_l2(index_size_l2,{-1});
    // vector<vector<int>> data_l1(index_size_l1,{-1});
    // vector<vector<int>> data_l2(index_size_l2,{-1});
    vector<vector<int>> tag1;
	vector<vector<int>> tag2;
    vector<vector<int>> data1;
	vector<vector<int>> data2;
	vector<vector<int>>lru_l1;
	vector<vector<int>>lru_l2;
    
//     void make_array(int index_size,int assoc, int value , vector<vector<int>> vec){
//         for (int i = 0 ; i<index_size; i++){
//     vector<int> t;
//     for (int j=0 ; j< assoc ; j++){
//         t.push_back(value);

//     }
//     vec.push_back(t);
// }
//     }


//     make_array(index_size_l1,L1_ASSOC,0,lru_l1);
//     make_array(index_size_l2,L2_ASSOC,0,lru_l2);
//     make_array(index_size_l1,L1_ASSOC,0,data1);
//     make_array(index_size_l2,L2_ASSOC,0,data2);
//     make_array(index_size_l1,L1_ASSOC,-1,tag1);
//     make_array(index_size_l2,L2_ASSOC,-1,tag2);
void act(){
	for (int i = 0 ; i<index_size_l1 ; i++){
		vector<int> t;
		for (int j=0 ; j< L1_ASSOC ; j++){
			t.push_back(0);

		}
		lru_l1.push_back(t);
	}
	for (int i = 0 ; i<index_size_l2 ; i++){
		vector<int> t1;
		for (int j=0 ; j< L2_ASSOC ; j++){
			t1.push_back(0);

		}
		lru_l2.push_back(t1);
	}
	for (int i = 0 ; i<index_size_l1 ; i++){
		vector<int> t2;
		for (int j=0 ; j< L1_ASSOC ; j++){
			t2.push_back(-1);

		}
		tag1.push_back(t2);
	}
	for (int i = 0 ; i<index_size_l2 ; i++){
		vector<int> t3;
		for (int j=0 ; j< L2_ASSOC ; j++){
			t3.push_back(-1);

		}
		tag2.push_back(t3);
	}
	for (int i = 0 ; i<index_size_l1 ; i++){
		vector<int> t4;
		for (int j=0 ; j< L1_ASSOC ; j++){
			t4.push_back(0);

		}
		data1.push_back(t4);
	}
	for (int i = 0 ; i<index_size_l2 ; i++){
		vector<int> t5;
		for (int j=0 ; j< L2_ASSOC ; j++){
			t5.push_back(0);

		}
		data2.push_back(t5);
	}}

















    vector<vector<string>> commands;
    int l1_read;
    int l1_read_miss;
    int l1_wrt;
    int l1_wrt_miss;
    int l1_miss_rate;
    int l1_wrtbk;
    int l2_read;
    int l2_read_miss;
    int l2_wrt;
    int l2_wrt_miss;
    int l2_miss_rate;
    int l2_wrtbk;


    cache(ifstream &file){
            constructCommands(file);
    }


    void constructCommands(ifstream &file)
        {
            string line;
            while (getline(file, line))
                parseCommand(line);
            file.close();
        }
    void parseCommand(string line)
        {
        vector<string> command ;
        boost::tokenizer<boost::char_separator<char>> tokens(line, boost::char_separator<char>(" \t"));
        for (auto&s : tokens){
            command.push_back(s);}

        commands.push_back(command);
        }

    void executeCommands(){
        for(int i = 0 ; i < commands.size() ; i++){
            if(commands[i][0]=="r"){
                read_req_l1(commands[i]);
            }
            else{
                write_req_l1(commands[i]);
            }
        }
    }

    bool isTag(vector<vector<int>> tag_list , int tag, int index){
        bool result = false;
        for(int i = 0;i<tag_list[index].size();i++){
            if(tag_list[index][i]==tag){
                result = true;
                break;
            }
        }
        return result;

    }




    void write_req_l1(vector<string> commander)
    {
            int address = stoi(commander[1],nullptr,16);
            int offset = address % BLOCKSIZE;
            // int index_size_l1 = size/(BLOCKSIZE*L1_ASSOC);
            int index_l1 = (address/BLOCKSIZE) % index_size_l1;
            int tag_l1 = (address / BLOCKSIZE) / index_size_l1;



		int found = 0;
        int it;
		for (int i=0;i<tag1[index_l1].size();i++)
            {
                if (tag1[index_l1][i]==tag_l1)
                    {
                        int it = i;
                        found = 1;
                        l1_wrt++;
                        data1[index_l1][i] = 1;
                        break;
                    }

            }
        if (found == 1) 
            {
				 // Check if the element was found
            
            for (int j=0;j<L1_ASSOC;j++)
            {
                if (j!=it)
                {
                    lru_l1[index_l1][j]++;
                }
                else{
                    lru_l1[index_l1][j] = 0;
                }

            }
            

            }

        else
        {   read_req_l2(address);
            l1_wrt_miss++;


            int idx = -1;
			for (int i=0;i<tag1[index_l1].size();i++)
                {
                    if (tag1[index_l1][i] == -1)
                    {
                        idx = i;
                        tag1[index_l1][i] = tag_l1;
                        data1[index_l1][i] = 1;
                        for (int j=0;j<L1_ASSOC;j++)
                            {
                                if (j!=i)
                                {
                                    lru_l1[index_l1][j]++;
                                }
                                else
                                {
                                    lru_l1[index_l1][j] = 0;
                                }

                            }
                        break;
                    }

                
                }

            if(idx == -1)
                {
                    vector<int> jack = lru_l1[index_l1];
                    int max_idx = distance(jack.begin(), max_element(jack.begin(), jack.end()));
                    int dirtyness = data1[index_l1][max_idx];
                    if (dirtyness == 1)
                        {
                            l1_wrtbk++;
                            wrt_back_l1(tag1[index_l1][idx]*index_size_l1+index_l1);
                        }
                    tag1[index_l1][max_idx] = tag_l1;
                    data1[index_l1][max_idx] = 1;
                    for (int j=0;j<L1_ASSOC;j++)
                        {
                            if (j!=max_idx)
                            {
                                lru_l1[index_l1][j]++;
                            }
                            else
                            {
                                lru_l1[index_l1][j] = 0;
                            }

                        }
                }

            
        }

    }
    
    
    
    void read_req_l1(vector<string> commander){
		int address = stoi(commander[1],nullptr,16);
        int index_l1 = (address/BLOCKSIZE) % index_size_l1;
    	int tag_l1 = (address / BLOCKSIZE) / index_size_l1;
		int found = 0;
        int it;
		for (int i=0;i<tag1[index_l1].size();i++){
			if (tag1[index_l1][i]==tag_l1){
					int it = i;
					found = 1;
                    break;
				}
        }
    	if (found == 1) {
				 // Check if the element was found
            l1_read++;
            for (int j=0;j<L1_ASSOC;j++)
            {
                if (j!=it)
                {
                    lru_l1[index_l1][j]++;
                }
                else{
                    lru_l1[index_l1][j] = 0;
                }

            }
            // if( data1[index_l1][it] == 1){
            //     wrt_back_l1(address);
            //     data[index_l1][it] = 0;
            // }

        }
        else{
            l1_read_miss++;
			read_req_l2(address);
            int idx = -1;
			for (int i=0;i<tag1[index_l1].size();i++){
                if (tag1[index_l1][i] == -1){
                    idx = i;
                    tag1[index_l1][i] = tag_l1;
                    data1[index_l1][i] = 1;
                    for (int j=0;j<L1_ASSOC;j++)
                    {
                    if (j!=i)
                    {
                        lru_l1[index_l1][j]++;
                    }
                    else{
                        lru_l1[index_l1][j] = 0;
                    }

                     }
                    break;
                }

            
            }
            if(idx == -1){
                int idx = distance(lru_l2[index_l1].begin(), max_element(lru_l2[index_l1].begin(), lru_l2[index_l1].end()));
                int dirtyness = data1[index_l1][idx];
                if (dirtyness == 0)
                {
                    tag1[index_l1][idx] = tag_l1;
                    data1[index_l1][idx] = 0;
                    for (int j=0;j<L1_ASSOC;j++)
                    {
                    if (j!=idx)
                    {
                        lru_l1[index_l1][j]++;
                    }
                    else
                    {
                        lru_l1[index_l1][j] = 0;
                    }

                     }

                }
                else
                {
                   wrt_back_l1(tag1[index_l1][idx]*index_size_l1+index_l1);
                    tag1[index_l1][idx] = tag_l1;
                    data1[index_l1][idx] = 0;
                    for (int j=0;j<L1_ASSOC;j++)
                    {
                    if (j!=idx)
                    {
                        lru_l1[index_l1][j]++;
                    }
                    else
                    {
                        lru_l1[index_l1][j] = 0;
                    }

                    }     
                }

                        }


        }
    }
    

    void wrt_back_l1(int add){
        l1_wrtbk++;
		// int address = stoi(commands[1],nullptr,16);
        int index_l2 = (add) % index_size_l2;
    	int tag_l2 = (add) / index_size_l2;
		int found = 0;
        int it;
		for (int i=0;i<tag2[index_l2].size();i++)
            {
                if (tag2[index_l2][i]==tag_l2)
                    {
                        int it = i;
                        found = 1;
                        l2_wrt++;
                        data2[index_l2][i] = 1;
                        break;
                    }

            }
        if (found == 1) 
            {
				 // Check if the element was found
            
            for (int j=0;j<L2_ASSOC;j++)
            {
                if (j!=it)
                {
                    lru_l2[index_l2][j]++;
                }
                else{
                    lru_l2[index_l2][j] = 0;
                }

            }
            

            }

        else
        {
            l2_wrt_miss++;


            int idx = -1;
			for (int i=0;i<tag2[index_l2].size();i++)
            {
                if (tag2[index_l2][i] == -1)
                {
                    idx = i;
                    tag2[index_l2][i] = tag_l2;
                    data2[index_l2][i] = 1;
                    for (int j=0;j<L2_ASSOC;j++)
                    {
                    if (j!=i)
                    {
                        lru_l2[index_l2][j]++;
                    }
                    else{
                        lru_l2[index_l2][j] = 0;
                    }

                     }
                    break;
                }

            
            }

            if(idx == -1)
            {
                vector<int> jack = lru_l2[index_l2];
                int max_idx = distance(jack.begin(), max_element(jack.begin(), jack.end()));
                int dirtyness = data2[index_l2][max_idx];
                if (dirtyness == 1){
                    l2_wrtbk++;
                }
                tag2[index_l2][max_idx] = tag_l2;
                data2[index_l2][max_idx] = 1;
                for (int j=0;j<L2_ASSOC;j++)
                {
                    if (j!=max_idx)
                    {
                        lru_l2[index_l2][j]++;
                    }
                    else
                    {
                        lru_l2[index_l2][j] = 0;
                    }

                }
            }

            
        }


            


    }

    void read_req_l2(int address){
        int index_l2 = (address/BLOCKSIZE) % index_size_l2;
    	int tag_l2 = (address / BLOCKSIZE) / index_size_l2;
		int found = 0;
        int it;
		for (int i=0;i<tag2[index_l2].size();i++)
            {
                if (tag2[index_l2][i]==tag_l2)
                    {
                        int it = i;
                        found = 1;
                        break;
                    }
            }
    	if (found == 1) 
            {
                    // Check if the element was found
                l2_read++;
                for (int j=0;j<L2_ASSOC;j++)
                {
                    if (j!=it)
                    {
                        lru_l2[index_l2][j]++;
                    }
                    else
                    {
                        lru_l2[index_l2][j] = 0;
                    }

                }


            }
        else
        {
            l2_read_miss++;
            int idx = -1;
			for (int i=0;i<tag2[index_l2].size();i++)
            {
                if (tag2[index_l2][i] == -1)
                {
                    idx = i;
                    tag2[index_l2][i] = tag_l2;
                    data2[index_l2][i] = 0;
                    for (int j=0;j<L2_ASSOC;j++)
                    {
                    if (j!=i)
                    {
                        lru_l2[index_l2][j]++;
                    }
                    else{
                        lru_l2[index_l2][j] = 0;
                    }

                    }
                    break;
                }

            
            }

                if(idx == -1)
                    {
                        vector<int> jack = lru_l2[index_l2];
                        int max_idx = distance(jack.begin(), max_element(jack.begin(), jack.end()));
                        int dirtyness = data2[index_l2][max_idx];
                        if (dirtyness == 1){
                            l2_wrtbk++;
                        }
                        tag2[index_l2][max_idx] = tag_l2;
                        data2[index_l2][max_idx] = 0;
                        for (int j=0;j<L2_ASSOC;j++)
                        {
                            if (j!=max_idx)
                            {
                                lru_l2[index_l2][j]++;
                            }
                            else
                            {
                                lru_l2[index_l2][j] = 0;
                            }

                        }
                    }






        }



        

        
    }
};



int main(int argc, char *argv[]){



    if (argc !=7) {
        std:: cerr << "Please provide all the arguments!\n";
        return 0;
    }
    else{
    ifstream file(argv[6]);
    int BLOCKSIZE = stoi(argv[1]);
    int L1_SIZE = stoi(argv[2]);
    int L1_ASSSOC = stoi(argv[3]);
    int L2_SIZE = stoi(argv[4]);
    int L2_ASSOC = stoi(argv[5]);

    cache *cac;
    if (file.is_open()){
    	cac = new cache(file);
    	}
        cac->act();
    	cac->executeCommands();

    return 0;
    }
}
