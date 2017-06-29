///
/// \file       Container.cpp
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      This file describes the password container
///             struct. Do not remove entries and do not change
///             the ID of an entry. This will cause in
///             compatibility problems with existing containers
///


#include "Container.h"
#include <cstring>
#include <ctime>
#include <iostream>
#include <stdio.h>

#include "../Crypto/uuidgenerator.h"
#include "../NativeFunctions/NativeFunctions.h"


const char* Container::mSessionKey=0;

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
Container::Container(const char* sessionKey):
    Nested(1),
    mGroupFilter(0)
{
    mSessionKey=sessionKey;
    mData[1] = &mGroups;
    mData[2] = &mPasswords;
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
Container::~Container()
{
}

///
/// \brief      Remove the group at the given index
///             and all its child groups
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  idx     Index of the group which should be removed
///
void Container::RemoveGroupEntry(int32_t idx)
{
    GroupEntry* groupToErase = GetGroup(idx);
    RemoveGroupEntry(groupToErase);
}

///
/// \brief      Remove the given group
///             and all its child groups
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  groupToErase    Pointer to the group which should be removed
///
void Container::RemoveGroupEntry(GroupEntry* groupToErase)
{
    uint32_t grpIdSize;
    char* groupId = groupToErase->GetGroupId(grpIdSize);

    // Remove all child groups of this group
    uint32_t groupNr = GetNumberOfGroupEntry();
    for(uint32_t n=0;n<groupNr;n++){
        GroupEntry* group = GetGroup(n);
        uint32_t size;
        char* parentId = group->GetParentId(size);
        if(0==std::memcmp(groupId,parentId,grpIdSize)){
            RemoveGroupEntry(group);
        }
    }

    mGroups.EraseElement(groupToErase);
}

///
/// \brief      Apply a group filter
///             Only passwords which matches the group ID will
///             ba added to the filter vector
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  groupFilter     Pointer to the group from which
///                             the passwords should be shown
///
void Container::FilterGroup(GroupEntry *groupFilter)
{
    mGroupFilter = groupFilter;
    mFilteredPasswordElements.clear();
    for(uint32_t n=0;n<mPasswords.GetNumberOfElements();n++){
        PasswordEntry* entry = mPasswords.GetElement(n);
        if(0!=groupFilter){
            uint32_t groupS;char* groupId   = entry->GetGroupId(groupS);
            uint32_t groupS2;char* groupId2 = groupFilter->GetGroupId(groupS2);
            if(0==std::memcmp(groupId,groupId2,groupS)){
                mFilteredPasswordElements.push_back(entry);
            }
        }else{
            mFilteredPasswordElements.push_back(entry);
        }
    }
}

///
/// \brief      Returns the group element with the given ID
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  groupId     Group ID
/// \param[in]  idSize      Size of the group ID
///
Container::GroupEntry* Container::GetGroup(char* groupIdin, uint32_t idSize)
{
    for(uint32_t n=0;n<mGroups.GetNumberOfElements();n++){
        GroupEntry* entry = mGroups.GetElement(n);
        uint32_t groupS;char* groupId   = entry->GetGroupId(groupS);
        if(0==std::memcmp(groupIdin,groupId,idSize)){
            return entry;
        }
    }
    return 0;
}

///
/// \brief      Apply a passwird title and tag filter
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \param[in]  titleString   Search string
///
void Container::FilterTitle(std::string searchString)
{
    mTitleFilter = searchString;
    mFilteredPasswordElements.clear();
    for(uint32_t n=0;n<mPasswords.GetNumberOfElements();n++){
        PasswordEntry* entry = mPasswords.GetElement(n);
        uint32_t titleS;char* title = entry->GetPasswordElement(0)->GetTitle(titleS);
        uint32_t tagS;char* tag = entry->GetPasswordElement(0)->GetTags(tagS);
        uint32_t urlS;char* url = entry->GetPasswordElement(0)->GetURL(urlS);

        std::string titelString(title,titleS);
        std::string TagString(tag,tagS);
        std::string UrlString(url,urlS);

        std::transform(searchString.begin(), searchString.end(), searchString.begin(), ::tolower);
        std::transform(titelString.begin(), titelString.end(), titelString.begin(), ::tolower);
        std::transform(TagString.begin(), TagString.end(), TagString.begin(), ::tolower);
        std::transform(UrlString.begin(), UrlString.end(), UrlString.begin(), ::tolower);


        if( titelString.find(searchString)!=std::string::npos ||
            TagString.find(searchString)!=std::string::npos ||
            UrlString.find(searchString)!=std::string::npos ||
            0==searchString.size()){
            mFilteredPasswordElements.push_back(entry);
        }
    }
}

///
/// \brief      Select an entry via URL
/// \author     Joachim Danmayr
/// \date       2016-11-27
///
/// \param[in]  url    URL to look for
///
std::vector<Container::PasswordEntry*> Container::SelectFromURL(std::string urlIn)
{
    std::vector<Container::PasswordEntry*> filteredData;

    for(uint32_t n=0;n<mPasswords.GetNumberOfElements();n++){
        PasswordEntry* entry = mPasswords.GetElement(n);
        uint32_t urlS;char* url = entry->GetPasswordElement(0)->GetURL(urlS);
        std::string UrlString(url,urlS);
        std::transform(UrlString.begin(), UrlString.end(), UrlString.begin(), ::tolower);
        std::transform(urlIn.begin(), urlIn.end(), urlIn.begin(), ::tolower);

        if(UrlString.find(urlIn)!=std::string::npos || 0==urlIn.size()){
            filteredData.push_back(entry);
        }
    }
    return filteredData;
}

///
/// \brief      Updates the password filter
///             This should be called if a password
///             has been removed or added
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void Container::UpdatePasswordFilter(void)
{
    if(mTitleFilter.size()>0){
        FilterTitle(mTitleFilter);
    }else{
        FilterGroup(mGroupFilter);
    }
}

/// \brief      Reset all filters
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void Container::ResetFilter(void)
{
    mTitleFilter.clear();
    mGroupFilter = 0;
    FilterTitle("");
}

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
Container::GroupEntry::GroupEntry(void):
    Nested(1)
{
    mData[1] = &mIcon;
    mData[2] = &mGroupId;
    mData[3] = &mParentId;
    mData[4] = &mName;

    uint32_t size;
    char* uuID = crypto::UUIDgenerator::GenerateUUID(size);
    SetGroupId(uuID,size);
}

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
Container::PasswordEntry::PasswordEntry():
    Nested(2)
{
    mData[1] = &mUID;
    mData[2] = &mGroupId;
    mData[3] = &mPasswordElement;

    uint32_t size;
    char* uuID = crypto::UUIDgenerator::GenerateUUID(size);
    SetUID(uuID,size);
}

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
Container::PasswordEntry::PasswordElement::PasswordElement():
    Nested(3)
{
    mData[1] = &mIcon;
    mData[2] = &mTitle;
    mData[3] = &mUsername;
    mData[4] = &mPassword;
    mData[5] = &mURL;
    mData[6] = &mNotes;
    mData[7] = &mTags;
    mData[8] = &mDateCreate;
    mData[9] = &mDateModified;
    mData[10] = &mDateExpired;
}

///
/// \brief      Set creation date entry
///             Format YYYY-MM-ddTHH:mm:ss,f
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void Container::PasswordEntry::PasswordElement::SetDateCreated(void)
{
    std::string time = GetActualDateAndTime();
    mDateCreate.SetData(time.data(),time.size());
}

///
/// \brief      Set modified date entry
///             Format YYYY-MM-ddTHH:mm:ss,f
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void Container::PasswordEntry::PasswordElement::SetDateModified(void)
{
    std::string time = GetActualDateAndTime();
    mDateModified.SetData(time.data(),time.size());
}

///
/// \brief      Set expire
///             Format YYYY-MM-ddTHH:mm:ss,f
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
void Container::PasswordEntry::PasswordElement::SetDateExpired(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second)
{
    std::tm timeInfo;
    timeInfo.tm_year = year-1900;
    timeInfo.tm_mon = month-1;
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = hour;
    timeInfo.tm_min=minute;
    timeInfo.tm_sec = second;

    char buffer [20];
    std::strftime(buffer,20,"%Y-%m-%dT%H:%M:%S",&timeInfo);
    mDateExpired.SetData(buffer,19);
}

///
/// \brief      Returns true if this entry is expired
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \return     TRUE if act date >= expired date
///
bool  Container::PasswordEntry::PasswordElement::IsExpired()
{

    uint32_t size;char* date = GetDateExpire(size);
    if(size>0){
        std::string dateString(date);
        std::string dateActString = GetActualDateAndTime();
        int result = std::strcmp( dateString.c_str(), dateActString.c_str() );

        if( result < 0 ) return true;
        else if( result > 0 ) return false;
        else return true;
    }
    return false;
}

///
/// \brief      Returns the actual date and time
///             in the format YYYY-MM-ddTHH:mm:ss,f
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
std::string Container::PasswordEntry::PasswordElement::GetActualDateAndTime()
{
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer,80,"%Y-%m-%dT%H:%M:%S",timeinfo);
    std::string timeAsString(buffer);
    return timeAsString;
}
