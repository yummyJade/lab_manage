#ifndef MYC1_BOOKINSTANCE_H
#define MYC1_BOOKINSTANCE_H

#include <iostream>
#include <vector>
#include "../core/Date.h"
class BookInstance {
public:
    long long id;
    std::string isbn;     //ͼ�����������isbn
    int status;     //״̬
    std::string position;       // ͼ��λ��
    Date planReturnDate; // Ԥ�ƹ黹����

public:

    BookInstance(const std::string &isbn, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position, const Date &planReturnDate);


private:
    //------------------------------------------------------
    //----������Щ�������ݿ⽻���Ľӿ�,��private����------------
    // ���һ��ʵ�������ݿ�,������id
    static int addBookInstance(BookInstance instance, int firstId = -1);

public:
    /**
     * ��������ͬһ�����ʵ��,�������׸�id
     * @param instances
     * @return
     */
    static int importBookInstances(std::vector<BookInstance> instances, int firstId);


    // �ֶ����һ�����ʵ��
    static bool addOneBookInstancesService();

    // �ֶ�ɾ��һ�����ʵ��
    static bool deleteOneBookInstancesService();

    /**
     * ����id�ֶ�ֵΪ����id������
     * @param id
     * @return
     */
    static BookInstance *getInstanceById(int id);

    /**
     * ��ȡĳ���������ʵ��
     * @param firstId
     * @return
     */
    static std::vector<BookInstance> getInstancesByFirstId(int firstId);

    // ɾ��ָ��
    static bool deleteInstancesByAssignInstanceId(int id);

};

#endif //MYC1_BOOKINSTANCE_H

