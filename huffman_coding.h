#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

#include <QMainWindow>

namespace Ui {
class Huffman_coding;
}

class Huffman_coding : public QMainWindow
{
    Q_OBJECT

public:
    explicit Huffman_coding(QWidget *parent = 0);
    ~Huffman_coding();

private:
    class Node{

    public:
        QChar symbol;
        quint8 count;
        Node* left;
        Node* right;

        explicit Node(QChar symbol, quint8 count, Node* left = nullptr, Node* right = nullptr){
            this->symbol = symbol;
            this->count = count;
            this->left = left;
            this->right = right;

            if (left && right)
                this->count = left->count + right->count;
        }

        ~Node(){
            delete left;
            delete right;
        }
    };

    Ui::Huffman_coding *ui;

    std::map<QChar, quint8> tMap = {};
    std::list<Node*> tree;
    std::map<QChar, std::vector<bool>> binary_table;
    QString strInBinary;


private slots:
    void createTable();
    void createTree();
    void createBinary();
    void createString();

    void printTable(std::map<QChar, quint8>& tMap);
    void printTree(Node* node, uint depth, bool first_pass = false);
    void printBinary();
    void printString();

    void searchingCharInTree(Node* root, std::vector<bool>& v_bits, std::map<QChar, std::vector<bool>>& binary_table);
    QString tablePass(std::map<QChar, std::vector<bool>>::iterator& itr);
};



#endif // HUFFMAN_CODING_H
