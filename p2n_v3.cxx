#include <fstream>
#include <vector>
#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <errno.h>
#include <iomanip>
#include <cmath>
#include <assert.h>
#include "TROOT.h"
#include "TRint.h"
#include "TObject.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TBranch.h"
#include "TBranchElement.h"
#include "TH1.h"
#include "TF1.h"
#include "TString.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <bits/stdc++.h>
#include <sstream>
#include <iostream>
#include <iomanip>
class CSVReader
{
	std::string fileName;
	std::string delimeter;
public:
	CSVReader(std::string filename, std::string delm = ",") :
            fileName(filename), delimeter(delm)
	{ }
	std::vector<std::vector<std::string> > getData();
};

std::vector<std::vector<std::string> > CSVReader::getData()
{
	std::ifstream file(fileName);
	std::vector<std::vector<std::string> > dataList;
	std::string line = "";
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	file.close();
	return dataList;
}

	int main(int argc, char **argv){

	Int_t split = 1;
   Int_t bsize = 64000;
    Int_t nchipid = 0;
	char* pathrootout=new char[400];
	char* filetext=new char[400];
	char* numefisier=new char[200];
	char* currentdir=new char[400];
	getcwd(currentdir,100);
	if (argc > 1)
      strcpy(numefisier, argv[1]);
	else {
      printf("Usage: read_binary <filename>\n");
    return 0;
    }
	sprintf(filetext,"%s/%s.csv",currentdir,numefisier);
	sprintf(pathrootout,"%s/%s.root",currentdir,numefisier);
	printf("Processing file: %s \n",filetext);
	printf("Processed file: %s \n",pathrootout);
    TFile *hfout = new TFile(Form("%s",pathrootout),"RECREATE");
    TTree *eventdt = new TTree("tests_ntuple","events");
    TBranch *names_strings[6];
    TBranch *vec_voltages[57];
    TBranch *bchipid;
    CSVReader reader_head(filetext);
	std::vector<std::vector<std::string> > data_header = reader_head.getData();
    int general_index=0;
    int local_index=0;
    int strings_index=0;
    int integer_index=0;
    std::vector<std::string> ptr_stringuri1;
    ptr_stringuri1.clear();
    ptr_stringuri1.reserve(6);
    ptr_stringuri1.assign (6,"test");
    std::vector<double> voltages1;
    voltages1.clear();
    voltages1.reserve(57);
    voltages1.assign (57,0.);


	for(std::vector<std::string> vec : data_header)
	{
        local_index=0;
        strings_index=0;
        integer_index=0;
        for(std::string data : vec)
		{
            if (general_index==0){
                std::cout<<data << " , ";
                char const *cstr=data.data();
                if (local_index==1 || local_index==2 || local_index==4||local_index==60|| local_index==61|| local_index==62){
                    names_strings[strings_index]=eventdt->Branch(Form("%s",cstr),&ptr_stringuri1[strings_index],bsize,split);
                    strings_index++;
                }
                else{
                    if (local_index==0){
                        vec_voltages[integer_index]=eventdt->Branch(Form("%s",cstr), &voltages1[integer_index],Form("%s/D",cstr),bsize);
                        bchipid=eventdt->Branch(Form("bc%s",cstr), &nchipid,Form("bc%s/L",cstr),split);
                        integer_index++;
                    }
                    else{
                        vec_voltages[integer_index]=eventdt->Branch(Form("%s",cstr), &voltages1[integer_index],Form("%s/D",cstr),bsize);
                        integer_index++;
                    }
                }
            }
            else if (general_index >0 ){
                if(local_index!=1 && local_index!=2 && local_index!=4 && local_index!=60 && local_index!=61 && local_index!=62){
                    if(local_index>20 && local_index <37){
                       std::istringstream converter { data.data() };
                       converter >> std::hex >> voltages1.at(integer_index);

                        //voltages1.at(integer_index)=std::stod(std::hex << data.data());
                    }
                    else if (local_index==0){
                        voltages1.at(integer_index)=std::stoi(data.data());
                        nchipid=(int)voltages1.at(integer_index);
                         //std::cout<<voltages1.at(integer_index)<<std::endl;
                         std::cout<<nchipid<<std::endl;
                    }
                    else{
                    voltages1.at(integer_index)=std::stod(data.data());
                    }
                    integer_index++;
                }
                else {
                 ptr_stringuri1.at(strings_index)=(data.data());
                 strings_index++;
                }
            }
            local_index++;
        }
        if (general_index>0){
        eventdt->Fill();
        }
        if (general_index==0)
            std::cout<<std::endl;
        general_index++;
	}

	std::cout<<std::endl;
    std::cout<<"print arrays"<<std::endl;
    for (auto i: voltages1){
        std::cout<< i<<", ";
    }
    std::cout<<std::endl;
    for (auto j: ptr_stringuri1){
        std::cout<< (std::string)j<<", ";
    }
    std::cout<<std::endl;
    hfout->Write();
	hfout->Close();
	delete[] pathrootout;
	delete[] filetext;
	delete[] numefisier;
	delete[] currentdir;

return 0;
}
