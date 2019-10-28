#include "service/BookInstanceService.h"
#include "../../include/model/BookInstance.h"

// 根据输入选择一个instance, 取消返回NULL
BookInstance *choseOneBookInstance() {
    int id;
    while (true) {
        printf("请输入要操作的书的条码号(输入0返回):");
        cin >> id;
        if (id == 0) {
            return NULL;
        }

        if (BookInstance::checkAssignBookInstanceIdExist(id)) {
            BookInstance *instance = BookInstance::getInstanceById(id);
            return instance;
        }
        printf("条码号为%d的用户不存在!", id);
    }
}


bool printAssignInstanceInfo(BookInstance instance) {
    vector<BookInstance> instances;
    instances.push_back(instance);
    printf("检索到如下信息\n");
    BookInstance::printBookInstanceList(instances);
	return true;
}

//===============================================================
//===============================================================

// 下架指定书籍
bool deleteAssignBookInstance(int id=-1) {
	BookInstance* instance = NULL;
	if (id == -1) {
		while (true) {
			printf("请输入要下架的书籍的条码号(输入0返回):");
			cin >> id;
			if (id == 0) 
				return false;
			if (BookInstance::checkAssignBookInstanceIdExist(id)) { 
				instance = BookInstance::getInstanceById(id);
				if (instance->getStatus() != 3) {// 图书存在
					break;
				}
			}
			printf("条码号为%d的图书不存在!\n", id);
		}
	}

	
	printf("检索到如下信息\n");
	instance->printLine();

	//由用户选择是否要下架
	cout << "输入Y下架条码号为" << id << "的图书,输入N取消:";
	char operate;
	cin >> operate;
	if (operate == 'Y' || operate == 'y') {// 下架图书
		// 修改Book馆藏量减1
		Book book=Book::searchBooksBySingleField("isbn", instance->getIsbn())[0];
		Book::updateBooks("isbn", instance->getIsbn(), "count", to_string(book.getCount() - 1));

		// 修改instance的状态
		instance->setStatus(3);// 3为已下架
		BookInstance::updateStateAndReturnTimeById(*instance);
		delete instance;
		cout << "已成功下架条码号为" << id << "的图书" << endl;
		return true;
	}
	cout << "已取消操作" << endl;
	return false;
}