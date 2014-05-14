#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include "RDFields.h"


string convertFileToString(char*);
string sanitizeName(string);
void writeOutputToFile(string);

using namespace std;

char* myFile = "textscans/1979/1979Z.tsv";//structured tab separated file

RDFields myobj(myFile);


int main()
{
	cout<<"Start Program"<<endl;

	char* newFile = "newtextscans/NewTextScans_1979/1979_ListingZ.txt"; //file that has reporting structure
	string repStructure="", prevSubId="", prevParId="";
	int noOfDashes=0;
	string finalReportingStructure="";

	cout<<"Start Program"<<endl;

	//convert reporting structure file to str
	repStructure = convertFileToString(newFile);
	string sanitizedName="", prevParentSubId="";

	for(int i=0;i<myobj.getNumberOfRecords();i++)
	{
	myobj.getNextRecord();
	//cout<<"Got next record"<<endl;
	myobj.printFields();
	//cout<<"Printed Fields"<<endl;

	if(myobj.recordIsSubsidiary())
	{
		//cout<<"Previous Par ID::"<<prevParId;
		//cout<<"MyObj.getpid"<<myobj.getPID();
		if(myobj.getPID().compare(prevParId)!=0 && prevParentSubId.compare("ERR_RepStructure")!=0)
		{
			myobj.resetPrevLevel();
			prevSubId="";
		}
		//cout<<endl<<"Name::"<<myobj.getName()<<endl;
		//cout<<"Previous Sub ID::"<<prevSubId;

		sanitizedName = sanitizeName(myobj.getName());
		//cout<<"Sanitized Name::"<<sanitizedName<<endl;
		noOfDashes = myobj.getReportingStructure(sanitizedName, repStructure, prevSubId);

		if(noOfDashes==0)
		{
			//string subIdToSearchFor = myobj.getSID();
			string cIdToSearchFor = myobj.getPID();
			noOfDashes= myobj.getReportingStructureSecondPhase(cIdToSearchFor,sanitizedName,repStructure,prevSubId);
		}

		if(noOfDashes==0)
		{
			string subIdToSearchFor = myobj.getSID();
			string cIdToSearchFor = myobj.getPID();
			//noOfDashes= myobj.getReportingStructureThirdPhase(cIdToSearchFor,subIdToSearchFor,repStructure,prevSubId);
		}

		if(noOfDashes!=0)
		{
			finalReportingStructure = to_string(noOfDashes);
			cout<<"Number of Dashes::"<<finalReportingStructure<<endl;
		}
		else
		{
			finalReportingStructure = "Err_CompNotFound";
			cout<<"Number of Dashes::"<<finalReportingStructure<<endl;
			//myobj.setParentSubID("Err_CompNotFound");
		}



		cout<<"Parent Sub ID::"<<myobj.getParentSubID()<<endl<<endl;
		prevParentSubId = myobj.getParentSubID();



		if(i!=0) prevSubId = myobj.getSID();
		prevParId = myobj.getPID();

		//cout<<"Reached here"<<endl;
	}
	else{
		finalReportingStructure = "";
	}



	writeOutputToFile(finalReportingStructure);

	myobj.resetFields();
	}



	return 0;
}

/*takes new textscan and converts it into string*/
string convertFileToString(char* newFile)
{
	 string buffer="";
	FILE* newfilepointer = fopen(newFile, "rb");
	rewind(newfilepointer);
	if (newfilepointer == NULL) cout<<"Error opening file to convert";
	else
	{
		while(!feof(newfilepointer))
		{
					char c;
					c=getc(newfilepointer);
					if(feof(newfilepointer)) break;
					buffer+=c;
		}
	}
	return buffer;
}


/*Reformats Company Name to make suitable for searching*/
string sanitizeName(string cName)
{
	string buffer="";
	int i=0;
	while(cName[i]!=',' && cName[i]!='\0') i++;
	buffer = cName.substr(0,i);
	return buffer;
}

/*Writes final output to tsv file*/


void writeOutputToFile(string repStr)
{
	fstream filePointer;
	filePointer.open("outputs/1979Z_op.txt", std::fstream::app);

	//cout<<"Entered function"<<endl;

	if(filePointer.is_open())
	{

	//write strings to file
	//cout<<"File opened to write"<<endl;

	filePointer<<myobj.getYear()<<"\t";
	filePointer<<myobj.getPID()<<"\t";
	filePointer<<myobj.getName()<<"\t";
	filePointer<<myobj.getSID()<<"\t";
	filePointer<<myobj.getZip()<<"\t";
	filePointer<<myobj.getProfStaff()<<"\t";
	filePointer<<myobj.getDoctorates()<<"\t";
	filePointer<<myobj.getTechAndAux()<<"\t";
	filePointer<<myobj.getRDCode()<<"\t";

	//write parentSubID to file

	filePointer<<myobj.getParentSubID();
	filePointer<<"\t";

	//write repStr to file

	filePointer << repStr;

	//write newline
	filePointer<<"\n";

	filePointer.close();
	}
	else
		cout<<"Could not open output file to write \n";
}
