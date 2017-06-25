/**
* @file	Consol.cpp
*
* Consol view
*
* @author	Joachim Danmayr
* @date	206-07-16
*
*/

#include "Consol.h"
#include <iostream>
#include <string>
#include "..\Secure\Secure.h"

using namespace std;

/**
 * Constructor
 *
 */
Consol::Consol(Container* passwordContainer):
	mPasswordContainer(passwordContainer)
{

}


/**
 * Start Console
*/
int Consol::Start()
{
	std::cout << "Welcome to joPassDB 1.0" << std::endl;
    std::cout  << endl;
    
	int choos = 0;
	
	while (choos < 100) {

        std::cout << "--------------------" << endl;
        std::cout << "1...Open container" << endl;
        std::cout << "2...New  container" << endl;
        std::cout << "--------------------" << endl;
        std::cout << "> "; std::cin >> choos;

        char fileName[] = { "example.txt\0" };
        string password;
        

        switch (choos) {
        // Open
        case 1:
            system("CLS");
            password.clear();
            cout << "Password to open ";cin.ignore();std::getline(cin, password);
            if (false == mPasswordContainer->Open(fileName, password.c_str(), password.size())) {
                sec::SecureClearString(&password);
                system("CLS");
                std::cout << "Wrong password!" << endl;
            }
            else
            {
                sec::SecureClearString(&password);
                system("CLS");
                ShowMenuForOpenContainer();
            }
            break;
       
        // Generate new container
        case 2:
            system("CLS");
            password.clear();
            cout << "Password for new container: ";cin.ignore();std::getline(cin, password);
            mPasswordContainer->New(fileName, password.c_str(), password.size());
            sec::SecureClearString(&password);
            system("CLS");
            break;
            
        // Show statistic
        case 8:
            sec::SecureClearString(&password);
            system("CLS");
            ShowStatistic();
            break;

        default:
            sec::SecureClearString(&password);
            system("CLS");
            delete mPasswordContainer;
            ShowStatistic();
            
            int a;cin >> a;
            return 0;
		}
	}

	return 0;
}

/**
 * Show submenu
 */
void Consol::ShowMenuForOpenContainer()
{
    int choos = 0;
    while (choos < 100) {
        std::cout << "--------------------" << endl;
        std::cout << "1...Close container" << endl;
        std::cout << "2...Save  container" << endl;
        std::cout << "3...Load  container" << endl;
        std::cout << "4...Add entry" << endl;
        std::cout << "5...Change password" << endl;
        std::cout << "--------------------" << endl;
        std::cout << "> "; std::cin >> choos;

        string password;


        switch (choos) {
            // Close
        case 1:
            mPasswordContainer->Close();
            sec::SecureClearString(&password);
            system("CLS");
            return;
            
            // Save
        case 2:
            system("CLS"); std::cout << "Save container..." << endl;
            if (false == mPasswordContainer->Save()) {
                std::cout << "Error saving" << endl;
            }
            else {
                system("CLS");
            }
            break;
            // Load container
        case 3:
            ShowEntry();

            break;
            // Add new entry
        case 4:
            system("CLS");
            AddEntry();
            system("CLS");
            break;

     
            // Change password
        case 5:
            system("CLS");
            password.clear();
            std::cout << "New password ";cin.ignore();std::getline(cin, password);
            mPasswordContainer->ChangePassword(password.c_str(), password.size());
            sec::SecureClearString(&password);
            system("CLS");
            break;

            // Close
        default:
            system("CLS");
            mPasswordContainer->Close();
            ShowStatistic();
            int a;cin >> a;
            return;
        }
    }
}




/**
 * Add a new entry
 */
void Consol::AddEntry()
{
    system("CLS");
	string titel, username,password;
    cout << "Titel ";cin.ignore();std::getline(cin, titel);
    cout << "Username ";std::getline(cin, username);
    cout << "Password ";std::getline(cin, password);

	DataContainer* entry = new DataContainer();
    const char* str;
    str = titel.c_str();    entry->AddEntry('TIT',    str, titel.length());
    sec::SecureClearString(&titel);
    str = username.c_str(); entry->AddEntry('USR', str, username.length());
    sec::SecureClearString(&username);
    str = password.c_str(); entry->AddEntry('PWD', str, password.length());
    sec::SecureClearString(&password);

    if (false == mPasswordContainer->AddEntry(entry)) {
        cout << "Error adding entry " << endl;
    }
    else {
        mPasswordContainer->Save();
    }
}

/**
 * Show entry
 */
void Consol::ShowEntry()
{
    system("CLS");
    std::cout.fill('-');std::cout << "+"; std::cout.width(TABLE_WIDTH+1); 
    std::cout << "+";std::cout.width(TABLE_WIDTH+1); 
    std::cout << "+";std::cout.width(TABLE_WIDTH+1); 
    std::cout << "+";std::cout.width(TABLE_WIDTH + 1);
    std::cout << "+";std::cout.width(TABLE_WIDTH + 1);
    std::cout << "+";std::cout.width(TABLE_WIDTH + 1);
    std::cout << "+"; cout  << endl;
    
    std::cout.fill(' ');std::cout.flags(std::ios::left);
    std::cout << "|"; std::cout.width(TABLE_WIDTH); cout << "Title";
    std::cout << "|"; std::cout.width(TABLE_WIDTH); cout << "Username";
    std::cout << "|"; std::cout.width(TABLE_WIDTH); cout << "Password";
    std::cout << "|"; std::cout.width(TABLE_WIDTH); cout << "URL";
    std::cout << "|"; std::cout.width(TABLE_WIDTH); cout << "Notes";
    std::cout << "|"; std::cout.width(TABLE_WIDTH); cout << "Tags"; cout << "| "<< endl;
    
    std::cout.fill('-'); std::cout.width(TABLE_WIDTH+1); 
    std::cout << "+";std::cout.width(TABLE_WIDTH+1); 
    std::cout << "+";std::cout.width(TABLE_WIDTH+1); 
    std::cout << "+";std::cout.width(TABLE_WIDTH + 1);
    std::cout << "+";std::cout.width(TABLE_WIDTH + 1);
    std::cout << "+";std::cout.width(TABLE_WIDTH + 1);
    std::cout << "+"; cout << "+" << endl;


    std::cout.fill(' ');
    for (unsigned int idx = 0;idx < mPasswordContainer->GetEntryNumber();idx++) {
        DataContainer* entry = 0;
        mPasswordContainer->GetEntry(idx, &entry);

        char* data = 0;
        int size = entry->GetEntry('TIT', &data);
        PrintEntry(data, size);
        
        size = entry->GetEntry('USR', &data);
        PrintEntry(data, size);
        
        size = entry->GetEntry('PWD', &data);
        PrintEntry(data, size);

        size = entry->GetEntry('URL', &data);
        PrintEntry(data, size);

        size = entry->GetEntry('NOT', &data);
        PrintEntry(data, size);

        size = entry->GetEntry('TAG', &data);
        PrintEntry(data,size);
        
        std::cout << "|"<< std::endl;
    }
    std::cout << std::endl;
}


void Consol::PrintEntry(char* data, unsigned int size)
{
    std::cout << "|";
    for (unsigned int n = 0;n < TABLE_WIDTH;n++) {
        if (n < size) {
            std::cout << data[n];
        }
        else {
            std::cout << " ";
        }
    }
}


void Consol::ShowStatistic()
{
    system("CLS");
    //cout << "Created: " << newCounter << endl;
    //cout << "deleted: " << deleteCounter << endl;
}
