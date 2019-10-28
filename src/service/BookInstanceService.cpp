#include "service/BookInstanceService.h"
#include "../../include/model/BookInstance.h"

// ��������ѡ��һ��instance, ȡ������NULL
BookInstance *choseOneBookInstance() {
    int id;
    while (true) {
        printf("������Ҫ��������������(����0����):");
        cin >> id;
        if (id == 0) {
            return NULL;
        }

        if (BookInstance::checkAssignBookInstanceIdExist(id)) {
            BookInstance *instance = BookInstance::getInstanceById(id);
            return instance;
        }
        printf("�����Ϊ%d���û�������!", id);
    }
}


bool printAssignInstanceInfo(BookInstance instance) {
    vector<BookInstance> instances;
    instances.push_back(instance);
    printf("������������Ϣ\n");
    BookInstance::printBookInstanceList(instances);
	return true;
}

//===============================================================
//===============================================================

// �¼�ָ���鼮
bool deleteAssignBookInstance(int id=-1) {
	BookInstance* instance = NULL;
	if (id == -1) {
		while (true) {
			printf("������Ҫ�¼ܵ��鼮�������(����0����):");
			cin >> id;
			if (id == 0) 
				return false;
			if (BookInstance::checkAssignBookInstanceIdExist(id)) { 
				instance = BookInstance::getInstanceById(id);
				if (instance->getStatus() != 3) {// ͼ�����
					break;
				}
			}
			printf("�����Ϊ%d��ͼ�鲻����!\n", id);
		}
	}

	
	printf("������������Ϣ\n");
	instance->printLine();

	//���û�ѡ���Ƿ�Ҫ�¼�
	cout << "����Y�¼������Ϊ" << id << "��ͼ��,����Nȡ��:";
	char operate;
	cin >> operate;
	if (operate == 'Y' || operate == 'y') {// �¼�ͼ��
		// �޸�Book�ݲ�����1
		Book book=Book::searchBooksBySingleField("isbn", instance->getIsbn())[0];
		Book::updateBooks("isbn", instance->getIsbn(), "count", to_string(book.getCount() - 1));

		// �޸�instance��״̬
		instance->setStatus(3);// 3Ϊ���¼�
		BookInstance::updateStateAndReturnTimeById(*instance);
		delete instance;
		cout << "�ѳɹ��¼������Ϊ" << id << "��ͼ��" << endl;
		return true;
	}
	cout << "��ȡ������" << endl;
	return false;
}