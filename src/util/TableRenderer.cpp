#include "util/TableRenderer.h"
#include <Windows.h>

bool TableRenderer::updateMaxNavLength(vector<string> lines) {
    if (lines.size() != this->columeNum) {
        cout << "���±�񳤶�ʧ��,���ݳ��Ȳ���";
        return false;
    }
    if (this->maxNavLength.size() == 0) {
        vector<int> maxLength;
        for (int i = 0; i < lines.size(); i++) {
            maxLength.push_back(lines[i].length() + 2);
        }
        this->maxNavLength = maxLength;
    } else {
        for (int i = 0; i < lines.size(); i++) {
            if (this->maxNavLength[i] < lines[i].length() + 2) {
                this->maxNavLength[i] = lines[i].length() + 2;
            }
        }
    }


    return true;
}

bool TableRenderer::renderNav(vector<string> lines) {
    int length = 0;

    cout << setw(this->offset - 1) << ""; // �����ǿ��� ƫ����,-1��Ϊ�˵����������|�ĳ���
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_BLUE);
    for (int i = 0; i < lines.size(); i++) {
        cout << "|" << setw(1) << setw(this->maxNavLength[i]) << lines[i].data() << setw(1) << "|";
        length = length + 2 + this->maxNavLength[i];
    }
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


    cout << setw(this->offset - 1) << "";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_BLUE);
    for (int i = 0; i < lines.size(); i++) {
        cout << "|" << setw(1) << setfill('-') << setw(this->maxNavLength[i]) << "" << setw(1) << "|";
        length = length + 2 + this->maxNavLength[i];
    }
    cout << setfill(' ') << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    //cout << setfill('-') << setw(length)<<""<< setfill(' ') <<endl ;
    return true;
}

bool TableRenderer::renderLine(vector<string> lines) {
    int length = 0;


    // ������ɫ,ѭ��ÿ�δ�ӡһ��
    cout << setw(this->offset);
    for (int i = 0; i < lines.size(); i++) {

        cout << "|" << setw(1) << setw(this->maxNavLength[i]) << lines[i].data() << setw(1) << "|";
        length = length + 2 + this->maxNavLength[i];
    }

    cout << endl;
    cout << setw(this->offset);
    for (int i = 0; i < lines.size(); i++) {
        cout << "|" << setw(1) << setfill('-') << setw(this->maxNavLength[i]) << "" << setw(1) << "|";
        length = length + 2 + this->maxNavLength[i];
    }
    cout << setfill(' ') << endl;

    //cout << setfill('-') << setw(length)<<""<< setfill(' ') <<endl ;
    return true;
}

TableRenderer::TableRenderer(vector<string> navs, int offset) {
    this->offset = offset;
    this->navigations = navs;
    this->columeNum = navs.size();
    this->updateMaxNavLength(navs);
}


bool TableRenderer::addColume(vector<string> line) {
    if (line.size() != this->columeNum) {
        cout << "�������ʧ��,���ݳ��Ȳ���";
        return false;
    }
    this->rows.push_back(line);
    this->updateMaxNavLength(line);
    this->rowNum++;
    return true;
}

bool TableRenderer::render() {
    this->renderNav(this->navigations);

    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN);
    for (int i = 0; i < rows.size(); i++) {
        this->renderLine(rows[i]);
    }
    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return false;
}
