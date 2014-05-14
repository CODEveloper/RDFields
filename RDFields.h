/*
 * RDFields.h
 *
 *  Created on: 25-Feb-2014
 *      Author: Apoorva
 */

#ifndef RDFIELDS_H_
#define RDFIELDS_H_

#pragma once

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

class RDFields {

	FILE* filePointer;
	string* fields;
	char* filePath;
	string* parentSubCode;
	int prevLevel, currentLevel;
	string parID;
	string previousSubID;

	string getParentID(string);

public:

	RDFields(char* filePath);
	virtual ~RDFields();

	void getNextRecord();
	void printFields();
	void resetFields();
	int getNumberOfRecords();
	bool recordIsSubsidiary();
	int getReportingStructure(string, string, string);
	int getReportingStructureSecondPhase(string, string, string, string);
	int getReportingStructureThirdPhase(string, string, string, string);
	string getYear();
	string getPID();
	string getName();
	string getSID();
	string getZip();
	string getProfStaff();
	string getDoctorates();
	string getTechAndAux();
	string getRDCode();
	string getParentSubID();
	void setParentSubID(string);
	void resetPrevLevel();


};

#endif /* RDFIELDS_H_ */
