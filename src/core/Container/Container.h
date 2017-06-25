///
/// \file       Container.h
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      This file describes the password container
///             struct. Do not remove entries and do not change
///             the ID of an entry. This will cause in
///             compatibility problems with existing containers
///

#ifndef CONTAINER_H
#define CONTAINER_H

#include "../Message/elementary.h"
#include "../Message/elementaryencrypted.h"
#include "../Message/repeated.h"
#include "../Message/nested.h"


class Container : public msg::Nested{
public:
    ///-----------------------------------------------------------------------------------
    class GroupEntry: public msg::Nested{
        msg::Elementary mIcon{1};
        msg::Elementary mGroupId{2};
        msg::Elementary mParentId{3};
        msg::Elementary mName{4};
    public:
       GroupEntry(void);
       void SetIcon(const char* data, uint32_t size){mIcon.SetData(data,size);}
       void SetGroupId(const char* data, uint32_t size){mGroupId.SetData(data,size);}
       void SetParentId(const char* data, uint32_t size){mParentId.SetData(data,size);}
       void SetName(const char* data, uint32_t size){mName.SetData(data,size);}

       char* GetIcon(uint32_t& size){return mIcon.GetData(size);}
       char* GetGroupId(uint32_t& size){return mGroupId.GetData(size);}
       char* GetParentId(uint32_t& size){return mParentId.GetData(size);}
       char* GetName(uint32_t& size){return mName.GetData(size);}
    };
    ///-----------------------------------------------------------------------------------
    class PasswordEntry: public msg::Nested{
    public:
        ///-------------------------------------------------------------------------------
        class PasswordElement: public msg::Nested{
            msg::Elementary mIcon{1};
            msg::Elementary mTitle{2};
            msg::Elementary mUsername{3};
            msg::ElementaryEncrypted mPassword{4,mSessionKey};
            msg::Elementary mURL{5};
            msg::Elementary mNotes{6};
            msg::Elementary mTags{7};
            msg::Elementary mDateCreate{8};
            msg::Elementary mDateModified{9};
            msg::Elementary mDateExpired{10};
        public:
            PasswordElement();
            void SetDateCreated(void);
            void SetDateExpired(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second);
            void ClearDateExpired(){mDateExpired.Clear();}
            void SetIcon(const char* data, uint32_t size){mIcon.SetData(data,size);SetDateModified();}
            void SetTitle(const char* data, uint32_t size){mTitle.SetData(data,size);SetDateModified();}
            void SetUsername(const char* data, uint32_t size){mUsername.SetData(data,size);SetDateModified();}
            void SetPassword(const char* data, uint32_t size){mPassword.SetData(data,size);SetDateModified();}
            void SetURL(const char* data, uint32_t size){mURL.SetData(data,size);SetDateModified();}
            void SetNotes(const char* data, uint32_t size){mNotes.SetData(data,size);SetDateModified();}
            void SetTags(const char* data, uint32_t size){mTags.SetData(data,size);SetDateModified();}

            char* GetIcon(uint32_t& size){return mIcon.GetData(size);}
            char* GetTitle(uint32_t& size){return mTitle.GetData(size);}
            char* GetUsername(uint32_t& size){return mUsername.GetData(size);}
            char* GetPassword(uint32_t& size){return mPassword.GetData(size);}
            char* GetURL(uint32_t& size){return mURL.GetData(size);}
            char* GetNotes(uint32_t& size){return mNotes.GetData(size);}
            char* GetTags(uint32_t& size){return mTags.GetData(size);}
            char* GetDateCreated(uint32_t& size){return mDateCreate.GetData(size);}
            char* GetDateModified(uint32_t& size){return mDateModified.GetData(size);}
            char* GetDateExpire(uint32_t& size){return mDateExpired.GetData(size);}
            bool  IsExpired();

            void ClosePassword(void){mPassword.Encrypt();}

        private:
            void SetDateModified(void);
            std::string GetActualDateAndTime();
        };

        msg::Elementary mUID{1};
        msg::Elementary mGroupId{2};
        msg::Repeated<PasswordElement> mPasswordElement;

    public:
        PasswordEntry();
        uint32_t GetNumberOfPasswordElement(void){return mPasswordElement.GetNumberOfElements();}
        void AddPasswordElement(PasswordElement* element){mPasswordElement.AddElement(element);}
        void RemovePasswordElement(uint32_t idx){mPasswordElement.EraseElement(idx);}
        PasswordElement* GetPasswordElement(uint32_t idx){return mPasswordElement.GetElement(idx);}

        void SetUID(const char* data, uint32_t size){mUID.SetData(data,size);}
        void SetGroupId(const char* data, uint32_t size){mGroupId.SetData(data,size);}

        char* GetUID(uint32_t& size){return mUID.GetData(size);}
        char* GetGroupId(uint32_t& size){return mGroupId.GetData(size);}
    };

    msg::Repeated<GroupEntry>    mGroups;
    msg::Repeated<PasswordEntry> mPasswords;

public:
    Container(const char* sessionKey=0);
    virtual ~Container();
    uint32_t GetNumberOfGroupEntry(void){return mGroups.GetNumberOfElements();}
    void AddGroup(GroupEntry* group){mGroups.AddElement(group);}
    GroupEntry* GetGroup(int32_t idx){return mGroups.GetElement(idx);}
    GroupEntry* GetGroup(char* groupIdin, uint32_t idSize);
    void RemoveGroupEntry(int32_t idx);
    void RemoveGroupEntry(GroupEntry* group);

    uint32_t GetNumberOfPasswordEntry(void){return mFilteredPasswordElements.size();}
    void AddPasswordEntry(PasswordEntry* entry){mPasswords.AddElement(entry);UpdatePasswordFilter();}
    PasswordEntry* GetPasswordEntry(uint32_t idx){return mFilteredPasswordElements.at(idx);}
    void RemovePasswordEntry(uint32_t idx){mPasswords.EraseElement(mFilteredPasswordElements.at(idx));UpdatePasswordFilter();}
    void FilterGroup(GroupEntry* groupFilter);
    void FilterTitle(std::string searchString);
    void UpdatePasswordFilter(void);
    void ResetFilter(void);

    std::vector<PasswordEntry*> SelectFromURL(std::string urlIn);

private:
    ///////////////////////////////////////////////////
    /// Members
    std::vector<PasswordEntry*> mFilteredPasswordElements;
    std::string mTitleFilter;
    GroupEntry* mGroupFilter; 
    static const char* mSessionKey;               ///< Session key for in RAM encryption
};

#endif // CONTAINER_H
