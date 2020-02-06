#include "huffman_coding.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Huffman_coding w;
    w.show();

    return a.exec();
}
