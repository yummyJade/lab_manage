#ifndef MYC1_BOOKINSTANCE_H
#define MYC1_BOOKINSTANCE_H

#include <iostream>
#include <vector>
#include "../../linkDatebase/include/Bookcopy.h"
#include "../core/Date.h"
class BookInstance {
    static const std::string BOOK_INSTANCE_FIELDS[];
public:
    int id;
    std::string isbn;     //ͼ�����������isbn
    int status;     //״̬ 1 �ɽ� 2 �ѱ����� 3 ��ɾ��(��ʧ���¼�) 4 �ѱ��������ѱ�ԤԼ 5 �ѱ�ԤԼ
    std::string position;       // ͼ��λ��
    Date planReturnDate; // Ԥ�ƹ黹����

public:

    BookInstance(const std::string &isbn, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position, const Date &planReturnDate);

    BookInstance(int id, const string &isbn, int status, const string &position, const Date &planReturnDate);

    int getId() const;

    const std::string &getIsbn() const;

    int getStatus() const;

    const std::string &getPosition() const;

    const Date &getPlanReturnDate() const;

    int readAndSetStatus();

    std::string readAndSetPosition();

    std::string readAndSetIsbn();


    void setIsbn(const std::string &isbn);

    void setStatus(int status);

    void setPosition(const std::string &position);


    void setPlanReturnDate(const Date &planReturnDate);

    // �ı�state ���־û������ݿ�
    bool changeStateAndPersistence(int newState);

    // �޸Ŀɱ��޸ĵ�ʵ����Ϣ(״̬,λ��)
    bool updateBookInstanceModifiableInfo();

    // ���л�ΪRecord
    Bookcopy toBookCopy();

    // Recordת��ΪOrder
    static BookInstance BookCopyToBookInstance(Bookcopy bookcopy);

private:
    //------------------------------------------------------
    //----������Щ�������ݿ⽻���Ľӿ�,��private����------------
    // ���һ��ʵ�������ݿ�,������id
    static int addBookInstance(BookInstance instance, int firstId = -1);

public:
    // ��ӡһ����Ϣ
    bool printLine();

    // ��ȡ���ڴ�ӡ������б���ַ���
    std::vector<std::string> getPrintLineStr();

    // ��ȡͼ��״̬
    std::string getStatusStr();
public:
    /**
     * ��������ͬһ�����ʵ��,�������׸�id
     * @param instances
     * @return
     */
    static int importBookInstances(std::vector<BookInstance> instances, int firstId);


    // �ֶ����һ�����ʵ��
    static bool addOneBookInstancesService();

    // ���ָ��id��ʵ���Ƿ����,������
    static bool checkAssignBookInstanceIdExist(int id);


    // ���ָ��isbn��ʵ���ܷ�ԤԼ��������һ���ڽ�״̬���ѱ�ԤԼ����
    static bool checkAssignBookCanAppointmentInstanceExist(std::string isbn);


    // ��̬����, ��ӡ��ѯ�����Ľ����
    static void printBookInstanceList(std::vector<BookInstance> instances);




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

    // �޸�ָ��id��instance��״̬��Ӧ��ʱ��,���ڽ軹��
    static bool updateStateAndReturnTimeById(BookInstance book);


};

#endif //MYC1_BOOKINSTANCE_H

