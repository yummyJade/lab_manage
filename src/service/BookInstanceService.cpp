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
bool outShelvesAssignBookInstance() {
    BookInstance *instance = choseOneBookInstance();
    if (instance == NULL) {
        return false;
    }


    printAssignInstanceInfo(*instance);
//    user->changePwdService(to_string(user->getJobNum()));

    printf("已下架条码号为:%d的图书\n", instance->getId());
    return true;
}