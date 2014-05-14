/*
 * RDFields.cpp
 *
 *  Created on: 25-Feb-2014
 *      Author: Apoorva
 */

#include "RDFields.h"

using namespace std;

//Constructor accepts filepath of TSV file, and opens file
RDFields::RDFields(char* filePath) {
	// TODO Auto-generated constructor stub

	this->filePath = filePath;
	fields = new string[9];
	parentSubCode = new string[10]; //stores subcode of parent in reporting structure


	parentSubCode[0] = "ERR_RepStructure";
	parentSubCode[1] = "0";
	filePointer = fopen(filePath, "rb");
	rewind(filePointer);
	if (filePointer == NULL)
		cout << "Error opening file";

	prevLevel = 0, currentLevel = 0;
	parID = "";
}

RDFields::~RDFields() {
	// TODO Auto-generated destructor stub
}

void RDFields::getNextRecord() {

	string buffer = "";
	char c;
	int i = 0, j = 0;
	c = getc(filePointer);

	while (c != '\n' && !feof(filePointer)) {
		buffer += c;
		c = getc(filePointer);
	}

	if (feof(filePointer))
		putc('\n', filePointer);

//	cout<<"Buffer:"<<buffer<<endl;

	for (i = 0; i < buffer.length(); i++) {
		if (buffer[i] == '\t') {
			//cout<<"FIELDS:"<<fields[j];
			j++;
			continue;
		}

		fields[j] += buffer[i];
	}

}

void RDFields::printFields() {
	//cout<<"LENGTH::"<<fields->length()<<endl;
	for (int i = 0; i < 8; i++) {
		if (fields[i] == "")
			fields[i] = "<empty>";
		cout << fields[i] << "::";
	}
	cout << fields[8];
	//cout<<endl;

}

void RDFields::resetFields() {
	for (int i = 0; i < 9; i++)
		fields[i] = "";
}

int RDFields::getNumberOfRecords() {
	int numberOfFiles = 0;
	char c;
	FILE* recordsPointer = fopen(filePath, "rb");
	rewind(recordsPointer);
	if (recordsPointer == NULL)
		cout << "Error opening file for counting records";

	while (!feof(recordsPointer)) {
		c = getc(recordsPointer);
		if (c == '\n')
			numberOfFiles++;
	}

	numberOfFiles++; //for last record

	return numberOfFiles;
}

bool RDFields::recordIsSubsidiary() {
	if (fields[3].compare("<empty>") == 0)
		return false;
	return true;
}

int RDFields::getReportingStructure(string companyName, string repStructStr,
		string previousSubId) {
	int posOfName = 0, i = 0, count = 0;

	posOfName = repStructStr.find(companyName);

	//cout<<"CHK:posOfName:"<<posOfName<<endl;

	if (posOfName != string::npos) {
		i = posOfName - 1;

		//cout<<"CHK:posOfName,i:"<<i<<endl;

		prevLevel = currentLevel;

		//cout<<"chk:"<<repStructStr[i];

		while (repStructStr[i] == '-') {
			//cout<<"entered";
			count++;
			i--;
		}

		currentLevel = count;
	}
	parID = getParentID(previousSubId);

	return count;

}

int RDFields::getReportingStructureSecondPhase(string compID,
		string companyName, string repStructStr, string previousSubId) {

	int posOfName = 0, count = 0;

	posOfName = repStructStr.find(compID);
	if (posOfName == string::npos)
		return 0;

	string subFile = repStructStr.substr(posOfName, repStructStr.length());

	//cout<<"CHK:posOfName:"<<posOfName<<endl;

	count = getReportingStructure(companyName, subFile, previousSubId);
	return count;
}


int RDFields::getReportingStructureThirdPhase(string compID, string subID, string repStructStr, string previousSubId)
{
	int posOfParent = 0, i = 0, count = 0;
	posOfParent = repStructStr.find(compID);

	if (posOfParent == string::npos)
		return 0;

	try {
		string subFile = repStructStr.substr(posOfParent, repStructStr.length());

		//i = posOfParent;

		while (true && subFile[i] != '\0')
		{
			i++;

			if (subFile[i] == '.' || subFile[i] == ',')
			{
				i++;
				cout<<"subID[0]:"<<subID[0]<<endl;
				if (subFile[i] == subID[0] || subFile[i] == ' ')
				{
					if (subFile[i] == ' ')
						{
							if (subFile[i] == subID[0])
							{
								while (subFile[i] != '-')
									i++;
								while (subFile[i] == '-')
								{
									count++; break;}

							}
						}
				}
			}
		}
	} catch (exception & e)
	{
		//return 0;
	}

	return count;

}

string RDFields::getParentID(string prevSubId) //code can be cleansed
		{
	string pid = "";
	//cout<<"PREVLEVEL::"<<prevLevel<<"CURRLEVEL::"<<currentLevel<<endl;
	if (prevLevel == currentLevel)
		pid = parentSubCode[currentLevel];

	else if (currentLevel < prevLevel)
		pid = parentSubCode[currentLevel];
	else if (currentLevel > prevLevel) {
		if (currentLevel != 1)
			parentSubCode[currentLevel] = prevSubId;
		pid = parentSubCode[currentLevel];
	}

	//cout<<"PID in function::"<<pid<<endl;
	return pid;

}

string RDFields::getYear() {
	return fields[0];
}

string RDFields::getPID() {
	return fields[1];
}

string RDFields::getName() {
	return fields[2];
}

string RDFields::getSID() {
	return fields[3];
}

string RDFields::getZip() {
	return fields[4];
}

string RDFields::getProfStaff() {
	return fields[5];
}

string RDFields::getDoctorates() {
	return fields[6];
}

string RDFields::getTechAndAux() {
	return fields[7];
}

string RDFields::getRDCode() {
	string fieldcode = fields[8].substr(0, fields[8].length()-1);
	return fieldcode;
}

string RDFields::getParentSubID() {
	return parID;
}

void RDFields::resetPrevLevel() {
	prevLevel = 0;
}

void RDFields::setParentSubID(string parSubId){
	parID = parSubId;
}
