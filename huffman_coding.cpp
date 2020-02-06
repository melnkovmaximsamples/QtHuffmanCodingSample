#include "huffman_coding.h"
#include "ui_huffman_coding.h"
#include <iostream>

Huffman_coding::Huffman_coding(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Huffman_coding)
{
    ui->setupUi(this);

    connect(ui->arrow_right, SIGNAL (released()), this, SLOT (createTable()));
    connect(ui->arrow_down, SIGNAL (released()), this, SLOT (createTree()));
    connect(ui->arrow_left, SIGNAL (released()), this, SLOT (createBinary()));
    connect(ui->arrow_up, SIGNAL (released()), this, SLOT (createString()));
}

Huffman_coding::~Huffman_coding()
{
    delete ui;
}

void Huffman_coding::createTable()
{
    ui->top_right_box->setText(NULL);

    QString source_string;

    tMap.clear();

    source_string = ui->top_left_box->toPlainText().split('\n').at(0);

    for (QChar c : source_string){
        tMap.insert(std::pair<QChar, quint8>(c, tMap.count(c) > 0 ? tMap.find(c)->second ++ : 1));
    }

    printTable(tMap);
}

void Huffman_coding::createTree() {
    tree.clear();
    struct minToMaxSort{
        bool operator() (Node* left, Node* right) const{
            return left->count < right->count;
        }
    };

    std::map<QChar, quint8>::iterator itr;

    for (itr = tMap.begin(); itr != tMap.end(); ++itr){
        Node* n = new Node(itr->first, itr->second);
        tree.push_back(n);
    }

    while(tree.size() != 1){
        tree.sort(minToMaxSort());

        Node* nLeft = tree.front();
        tree.pop_front();
        Node* nRight = tree.front();
        tree.pop_front();

        Node* nParent = new Node('*', 0, nLeft, nRight);
        tree.push_back(nParent);
    }

    printTree(tree.front(), 0, true);
}

void Huffman_coding::createBinary() {

    std::vector<bool> v_bits;
    searchingCharInTree(tree.front(), v_bits, binary_table);

    std::map<QChar, std::vector<bool>>::iterator itr;

    ui->bot_left_box->setText("String in binary:\n");

    strInBinary.clear();

    for (QChar c : ui->top_left_box->toPlainText()){
        for (itr = binary_table.begin(); itr != binary_table.end(); itr++){
            if (c == itr->first){
                strInBinary.append(tablePass(itr));
                break;
            }
        }
    }

    ui->bot_left_box->setText(ui->bot_left_box->toPlainText() + strInBinary);

    ui->bot_left_box->setText(ui->bot_left_box->toPlainText() + "\n\nChars in binary");

    for (itr = binary_table.begin(); itr != binary_table.end(); itr++){
        ui->bot_left_box->setText(ui->bot_left_box->toPlainText() + "\n'" + itr->first + "'\t: " + tablePass(itr));
    }
}

QString Huffman_coding::tablePass(std::map<QChar, std::vector<bool>>::iterator& itr){
    QString numbers;

    for (int i = 0; i < itr->second.size(); i ++){
        numbers.append(QString::number(itr->second[i]));
    }

    return numbers;
}


void Huffman_coding::searchingCharInTree(Node* root, std::vector<bool>& v_bits, std::map<QChar, std::vector<bool>>& binary_table){

    if (root->left != NULL){
        v_bits.push_back(0);
        searchingCharInTree(root->left, v_bits, binary_table);
    }

    if (root->right != NULL){
        v_bits.push_back(1);
        searchingCharInTree(root->right, v_bits, binary_table);
    }

    if (root->symbol != '*'){
        binary_table[root->symbol] = v_bits;
    }

    if (v_bits.size() > 0)
        v_bits.pop_back();

}

void Huffman_coding::createString() {
    Node* curNode = tree.front();
    Node* prevNode = tree.front();
    QString sStr;

    for (QChar c : strInBinary){

        sStr += curNode->symbol + ' ';
        if (c == '0' && curNode->left != nullptr){
            prevNode = curNode;
            curNode = curNode->left;

            continue;
        }

        if (c == '1' && curNode->right != nullptr){
            prevNode = curNode;
            curNode = curNode->right;

            continue;
        }
/*

        if (curNode->symbol != '*')
            sStr.append(curNode->symbol + ' ');
        else
            sStr.append(prevNode->symbol + ' ');
*/
        curNode = tree.front();

    }

    ui->top_left_box->setText(sStr);
}

void Huffman_coding::printTable(std::map<QChar, quint8>& tMap){
    ui->top_right_box->setText(NULL);

    std::multimap<quint8, QString> tMMap = {};

    for (std::pair<QString, quint8> pair : tMap) {
        tMMap.insert(std::pair<quint8, QString>(pair.second, pair.first));
    }

    for (std::multimap<quint8, QString>::iterator it=tMMap.begin(); it!=tMMap.end(); it++){
        ui->top_right_box->setText(ui->top_right_box->toPlainText() + "char = '" + it->second + "', count = '" + QString::number(it->first) + "';\n");
    }
}

void Huffman_coding::printTree(Node* node, uint depth = 0, bool first_pass){

    if (first_pass == true)
        ui->bot_right_box->setText("");

    if (node->left != nullptr)
        printTree(node->left, depth + 1);

    QString sheet = "";

    for (int i = depth; i > 0; i--)
        sheet += "\t";

    if (node->symbol != '\0')
        sheet += node->symbol + QString::fromUtf8(" (") + QString::number(node->count) + QString::fromUtf8(")") + '\n';

    ui->bot_right_box->setText(ui->bot_right_box->toPlainText() + sheet);

    if (node->right != nullptr)
        printTree(node->right, depth + 1);
}

void Huffman_coding::printBinary(){

}

void Huffman_coding::printString(){

}




