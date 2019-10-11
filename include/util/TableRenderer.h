//
// Created by Tjm on 2019/10/10.
//

#ifndef MYC1_TABLERENDERER_H
#define MYC1_TABLERENDERER_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

class TableRenderer {
private:
    // ---------- 标题栏-----------------
    string title;

    // ---------- 导航栏-----------------
    vector<string> navigations; // 导航栏们
    vector<int> maxNavLength; // 当前列的最大长度

    // ---------- 内容栏-----------------
    vector<vector<string>> rows;

    // ---------- 属性栏-----------------
    int columeNum; // 列数
    int rowNum; // 行数
    int offset; // 偏移量

private:
    // 更新显示的每列的长度
    bool updateMaxNavLength(vector<string> lines);

    // 渲染导航栏
    bool renderNav(vector<string> lines);

    // 渲染行内容
    bool renderLine(vector<string> lines);

public:
    TableRenderer(vector<string> navs, int offset = 0);

    // 增加一行数据
    bool addColume(vector<string> line);

    /* 渲染表格 */
    bool render();

};

#endif //MYC1_TABLERENDERER_H
