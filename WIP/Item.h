#pragma once
#include <iostream>
#include <string>
using namespace std;

class Item
{
public:
	Item();
	~Item();
	string name;
	int quantity;	
	bool medicalItem;		// dont use anything below here.
	int medalHitPts;
	int key;
};

